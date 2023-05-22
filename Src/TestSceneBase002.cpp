#include "TestSceneBase002.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/CollisionBody2D.h"
#include "Engine/Components/RenderedModel.h"
#include "Engine/Math/Overlap2D.h"
#include "Engine/Math/CoordinateSystems.h"
#include "Engine/Math/VectorUtilities.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/Renderer.h"
#include "Engine/Window.h"



// public ----------------------------------------------------------------------

TestSceneBase002::TestSceneBase002()
    : cursorGrabbingEntity_(false)
    , previousWorldCursorDownPosition_(0.0f, 0.0f)
{
    ClearResources();
}

TestSceneBase002::~TestSceneBase002()
{}

const char* TestSceneBase002::Name()
{
    return "TestSceneBase002";
}

// protected -------------------------------------------------------------------

bool TestSceneBase002::OnInitialize()
{
    _LOG_MESSAGE("INITIALIZING: %s", Name());

    windowPtr_ = GetApplicationWindowPtr();

    rendererPtr_ = GetApplicationRendererPtr();
    rendererPtr_->SetDepthTesting(false);

    componentStoresPtr_ = GetApplicaitonComponentStoresPtr();

    // main camera entity
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(mainCameraEntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::Camera>(mainCameraEntityId_));

        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
        int aspectRatioNumerator;
        int aspectRatioDenominator;
        windowPtr_->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
        if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
        {
            float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;
            cameraPtr->SetAspectRatio(aspectRatio);
            cameraPtr->SetTopCutoff(3.5f);
            cameraPtr->SetBottomCutoff(-3.5f);
            cameraPtr->SetLeftCutoff(aspectRatio * -3.5f);
            cameraPtr->SetRightCutoff(aspectRatio * 3.5f);
        }
        cameraPtr->SetPosition(0.0f, 0.0f, 5.0f);
        cameraPtr->AddYaw(glm::pi<float>());
        cameraPtr->SetProjectionToOrthographic();
        cameraPtr->TurnOn();
    }

    return true;
}

bool TestSceneBase002::OnDeinitialize()
{
    rendererPtr_->DeleteAllTextures();
    componentStoresPtr_->DeleteAllEntities();
    ClearResources();

    return true;
}

void TestSceneBase002::OnHandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestSceneBase002::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::FrameBufferSizeEvent>(event, std::bind(&TestSceneBase002::ProcessFrameBufferSizeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestSceneBase002::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestSceneBase002::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestSceneBase002::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::ScrollEvent>(event, std::bind(&TestSceneBase002::ProcessScrollEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestSceneBase002::ProcessUpdateEvent, this, std::placeholders::_1));
}

void TestSceneBase002::ClearResources()
{
    selectedEntityIdIndex_ = (unsigned int)-1;

    for (size_t i = 0; i < meshDataPtrArray_.size(); ++i)
    {
        delete meshDataPtrArray_[i];
    }
    meshDataPtrArray_.clear();

    mainCameraEntityId_ = (unsigned int)-1;
    entityIds_.clear();
}

void TestSceneBase002::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent)
{
    if (cursorGrabbingEntity_)
    {
        glm::vec2 cursorPosition(cursorPositionEvent.xPosition, cursorPositionEvent.yPosition);

        int windowWidth, windowHeight;
        windowPtr_->GetWindowSize(windowWidth, windowHeight);

        unsigned int xOffset, yOffset, viewportWidth, viewportHeight;
        rendererPtr_->GetViewport(xOffset, yOffset, viewportWidth, viewportHeight);

        // Convert coordinates from window to viewport
        cursorPosition.y = windowHeight - yOffset - cursorPosition.y;
        cursorPosition.x = cursorPosition.x - xOffset;

        if (cursorPosition.x < viewportWidth || cursorPosition.y < viewportHeight)
        {
            Project001::Camera* cameraPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
            cursorPosition = cameraPtr->ConvertPointFromViewportToOrthoWorld(viewportWidth, viewportHeight, cursorPosition);

            float xOffset = cursorPosition.x - previousWorldCursorDownPosition_.x;
            float yOffset = cursorPosition.y - previousWorldCursorDownPosition_.y;

            if (selectedEntityIdIndex_ < entityIds_.size())
            {
                unsigned int selectedEntityId = entityIds_[selectedEntityIdIndex_];

                Project001::CollisionBody2D* collisionBody2DPtr;
                _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(selectedEntityId, collisionBody2DPtr));
                collisionBody2DPtr->AddTranslationX(xOffset);
                collisionBody2DPtr->AddTranslationY(yOffset);
            }

            previousWorldCursorDownPosition_.x = cursorPosition.x;
            previousWorldCursorDownPosition_.y = cursorPosition.y;
        }
    }

    cursorPositionEvent.handled = true;
}

void TestSceneBase002::ProcessFrameBufferSizeEvent(Project001::FrameBufferSizeEvent& frameBufferSizeEvent)
{
    const int& height = frameBufferSizeEvent.height;
    const int& width = frameBufferSizeEvent.width;

    int aspectRatioNumerator, aspectRatioDenominator;
    windowPtr_->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);

    if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
    {
        float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;

        int adjustedHeight = (int)(width / aspectRatio);
        int adjustedWidth = (int)(height * aspectRatio);

        if (adjustedWidth > width)
        {
            adjustedWidth = width;
        }

        if (adjustedHeight > height)
        {
            adjustedHeight = height;
        }

        int lowerLeftX = (width - adjustedWidth) / 2;
        int lowerLeftY = (height - adjustedHeight) / 2;

        rendererPtr_->SetFramebufferSize(adjustedWidth, adjustedHeight);
        rendererPtr_->SetViewport(lowerLeftX, lowerLeftY, adjustedWidth, adjustedHeight);
    }
    else
    {
        rendererPtr_->SetFramebufferSize(width, height);
        rendererPtr_->SetViewport(0, 0, width, height);
    }

    frameBufferSizeEvent.handled = true;
}

void TestSceneBase002::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_N)
        {
            selectedEntityIdIndex_++;
            if (selectedEntityIdIndex_ >= entityIds_.size())
            {
                selectedEntityIdIndex_ = 0;
            }
        }
        else if (keyCode == Project001::KeyCode::KEY_CODE_B)
        {
            if (selectedEntityIdIndex_ == 0)
            {
                selectedEntityIdIndex_ = (unsigned int)entityIds_.size() - 1;
            }
            else if (selectedEntityIdIndex_ >= entityIds_.size())
            {
                selectedEntityIdIndex_ = (unsigned int)entityIds_.size() - 1;
            }
            else
            {
                selectedEntityIdIndex_--;
            }
        }
    }
}

void TestSceneBase002::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;

    cursorGrabbingEntity_ = false;

    if (mouseButton == Project001::MouseButton::MOUSE_BUTTON_1 &&
        buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS)
    {
        selectedEntityIdIndex_ = (unsigned int)-1;

        glm::vec2 cursorPosition;
        windowPtr_->GetCursorPosition(cursorPosition.x, cursorPosition.y);

        int windowWidth, windowHeight;
        windowPtr_->GetWindowSize(windowWidth, windowHeight);

        unsigned int xOffset, yOffset, viewportWidth, viewportHeight;
        rendererPtr_->GetViewport(xOffset, yOffset, viewportWidth, viewportHeight);

        // Convert coordinates from window to viewport
        cursorPosition.y = windowHeight - yOffset - cursorPosition.y;
        cursorPosition.x = cursorPosition.x - xOffset;

        if (cursorPosition.x < viewportWidth || cursorPosition.y < viewportHeight)
        {
            Project001::Camera* cameraPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
            previousWorldCursorDownPosition_ = cameraPtr->ConvertPointFromViewportToOrthoWorld(viewportWidth, viewportHeight, cursorPosition);

            Project001::CollisionBody2D* collisionBody2DArray = nullptr;
            size_t collisionBodyCount = 0;

            componentStoresPtr_->GetAllComponents<Project001::CollisionBody2D>(collisionBody2DArray, collisionBodyCount);

            // This loop goes backwards to grab the component drawn last first.
            // This relies on the order the components and render bodies were 
            // added.
            for (int i = (int)collisionBodyCount - 1; i >= 0; --i)
            {
                Project001::CollisionBody2D& currentCollisionBody2D = collisionBody2DArray[i];
                if (currentCollisionBody2D.GetCollision(previousWorldCursorDownPosition_))
                {
                    unsigned int entityId;
                    _FAIL_CHECK(componentStoresPtr_->GetComponentEntityId(&currentCollisionBody2D, entityId));

                    for (unsigned int i = 0; i < entityIds_.size(); ++i)
                    {
                        if (entityId == entityIds_[i])
                        {
                            selectedEntityIdIndex_ = i;
                        }
                    }

                    cursorGrabbingEntity_ = true;
                    break;
                }
            }
        }
    }
}

void TestSceneBase002::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    rendererPtr_->ClearDirectionalLight();
    rendererPtr_->ClearPointLights();
    rendererPtr_->ClearSpotLights();

    rendererPtr_->BeginRendering();
    rendererPtr_->Clear();

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

    if (cameraPtr->IsTurnedOn())
    {
        rendererPtr_->SetViewMatrix(cameraPtr->GetViewMatrix());
        rendererPtr_->SetViewPosition(cameraPtr->GetPosition());
        rendererPtr_->SetProjectionMatrix(cameraPtr->GetProjectionMatrix());

        Project001::RenderedModel* renderedModelArray = nullptr;
        size_t renderedModelCount = 0;

        componentStoresPtr_->GetAllComponents<Project001::RenderedModel>(renderedModelArray, renderedModelCount);

        for (unsigned int i = 0; i < renderedModelCount; ++i)
        {
            Project001::RenderedModel& currentRenderedModel = renderedModelArray[i];

            if (currentRenderedModel.IsVisible())
            {
                const Project001::MeshData* currentMeshDataPtr = currentRenderedModel.GetMeshDataPtr();
                if (currentMeshDataPtr != nullptr)
                {
                    _FAIL_CHECK(rendererPtr_->AddMesh(
                        currentMeshDataPtr->meshVertexArray.data(),
                        (unsigned int)currentMeshDataPtr->meshVertexArray.size(),
                        currentMeshDataPtr->meshIndexArray.data(),
                        (unsigned int)currentMeshDataPtr->meshIndexArray.size(),
                        currentRenderedModel.GetTextureId(),
                        currentRenderedModel.GetSpecularId(),
                        currentRenderedModel.GetPosition(),
                        currentRenderedModel.GetOrientation(),
                        currentRenderedModel.GetScale(),
                        currentRenderedModel.GetColor(),
                        currentRenderedModel.GetShininess(),
                        currentRenderedModel.GetTranslucent(),
                        currentRenderedModel.GetLit()));
                }
            }
        }

        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

        rendererPtr_->Render();
    }

    rendererPtr_->FinishRendering();
    rendererPtr_->SwapBuffers();

    renderEvent.handled = true;
}

void TestSceneBase002::ProcessScrollEvent(Project001::ScrollEvent& scrollEvent)
{
    float& yOffset = scrollEvent.yOffset;

    float speedConstant = 0.1f;
    float cameraResize = 1.0f + speedConstant * yOffset;

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
    float newTopCutoff = cameraPtr->GetTopCutoff() * cameraResize;
    float newBottomCutoff = cameraPtr->GetBottomCutoff() * cameraResize;
    float newLeftCutoff = cameraPtr->GetLeftCutoff() * cameraResize;
    float newRightCutoff = cameraPtr->GetRightCutoff() * cameraResize;

    if (newTopCutoff > 0.1f && newTopCutoff < 10.0f)
    {
        cameraPtr->SetTopCutoff(newTopCutoff);
        cameraPtr->SetBottomCutoff(newBottomCutoff);
        cameraPtr->SetLeftCutoff(newLeftCutoff);
        cameraPtr->SetRightCutoff(newRightCutoff);
    }

    scrollEvent.handled = true;
}

void TestSceneBase002::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    unsigned long timestep_ns = updateEvent.timestep_ns;

    UpdatedSelectedEntityPosition(timestep_ns);
    Sync_RenderedModel_CollisionBody_Components();

    DetectCollisions();
}

void TestSceneBase002::UpdatedSelectedEntityPosition(unsigned long timestep_ns)
{
    float timestep_s = (float)(timestep_ns / 1000000) / 1000;

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
    float speedConstant = cameraPtr->GetTopCutoff();
    glm::vec2 cameraUp = cameraPtr->GetUpVector();
    glm::vec2 cameraLeft = cameraPtr->GetLeftVector();

    float translationSpeed = speedConstant * timestep_s;
    float rotationSpeed = speedConstant * 2.0f * timestep_s;

    bool movingLeft = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_A);
    bool movingRight = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_D);
    bool movingUp = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_W);
    bool movingDown = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_S);
    bool rollingLeft = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_Q);
    bool rollingRight = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_E);

    if (selectedEntityIdIndex_ < entityIds_.size())
    {
        unsigned int selectedEntityId = entityIds_[selectedEntityIdIndex_];

        Project001::CollisionBody2D* collisionBodyPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(selectedEntityId, collisionBodyPtr));

        if (movingLeft)
        {
            // collisionBodyPtr->TranslateRight(-1.0f * translationSpeed);
            collisionBodyPtr->AddTranslation(cameraLeft * translationSpeed);
        }

        if (movingRight)
        {
            // collisionBodyPtr->TranslateRight(translationSpeed);
            collisionBodyPtr->AddTranslation(cameraLeft * -1.0f * translationSpeed);
        }

        if (movingUp)
        {
            // collisionBodyPtr->TranslateUp(translationSpeed);
            collisionBodyPtr->AddTranslation(cameraUp * translationSpeed);
        }

        if (movingDown)
        {
            // collisionBodyPtr->TranslateUp(-1.0f * translationSpeed);
            collisionBodyPtr->AddTranslation(cameraUp * -1.0f * translationSpeed);
        }

        if (rollingLeft)
        {
            collisionBodyPtr->AddRotation(rotationSpeed);
        }

        if (rollingRight)
        {
            collisionBodyPtr->AddRotation(-1.0f * rotationSpeed);
        }
    }
    else
    {
        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

        if (movingLeft)
        {
            cameraPtr->MoveLeft(translationSpeed);
        }

        if (movingRight)
        {
            cameraPtr->MoveRight(translationSpeed);
        }

        if (movingUp)
        {
            cameraPtr->MoveUp(translationSpeed);
        }

        if (movingDown)
        {
            cameraPtr->MoveDown(translationSpeed);
        }

        if (rollingLeft)
        {
            cameraPtr->AddRoll(-1.0f * rotationSpeed);
        }

        if (rollingRight)
        {
            cameraPtr->AddRoll(rotationSpeed);
        }
    }
}

void TestSceneBase002::DetectCollisions()
{
    Project001::CollisionBody2D* collisionBody2DArray = nullptr;
    size_t collisionBodyCount = 0;

    componentStoresPtr_->GetAllComponents<Project001::CollisionBody2D>(collisionBody2DArray, collisionBodyCount);

    for (unsigned int i = 0; i < collisionBodyCount; ++i)
    {
        Project001::CollisionBody2D& currentCollisionBody2D = collisionBody2DArray[i];
        currentCollisionBody2D.CalculateTransforedShapes();
        currentCollisionBody2D.SetColliding(false);
    }

    if (collisionBodyCount > 0)
    {
        for (unsigned int i = 0; i < collisionBodyCount - 1; ++i)
        {
            Project001::CollisionBody2D& firstCollisionBody2D = collisionBody2DArray[i];
            for (unsigned int j = i + 1; j < collisionBodyCount; ++j)
            {
                Project001::CollisionBody2D& secondCollisionBody2D = collisionBody2DArray[j];
                firstCollisionBody2D.CalculateCollision(secondCollisionBody2D);
            }
        }
    }

    for (unsigned int i = 0; i < collisionBodyCount; ++i)
    {
        Project001::CollisionBody2D& collisionBody2D = collisionBody2DArray[i];

        unsigned int entityId;
        _FAIL_CHECK(componentStoresPtr_->GetComponentEntityId(&collisionBody2D, entityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(entityId, renderedModelPtr));

        if (collisionBody2D.GetColliding())
        {
            if (selectedEntityIdIndex_ < entityIds_.size() && entityId == entityIds_[selectedEntityIdIndex_])
            {
                renderedModelPtr->SetColorRGB(1.0f, 0.75f, 0.25f);
            }
            else
            {
                renderedModelPtr->SetColorRGB(1.0f, 0.25f, 0.25f);
            }
        }
        else
        {
            if (selectedEntityIdIndex_ < entityIds_.size() && entityId == entityIds_[selectedEntityIdIndex_])
            {
                renderedModelPtr->SetColorRGB(0.25f, 0.25f, 1.0f);
            }
            else
            {
                renderedModelPtr->SetColorRGB(1.0f, 1.0f, 1.0f);
            }
        }
    }
}

void TestSceneBase002::Sync_RenderedModel_CollisionBody_Components()
{
    Project001::CollisionBody2D* collisionBody2DArray = nullptr;
    size_t collisionBodyCount = 0;

    componentStoresPtr_->GetAllComponents<Project001::CollisionBody2D>(collisionBody2DArray, collisionBodyCount);

    for (unsigned int i = 0; i < collisionBodyCount; ++i)
    {
        Project001::CollisionBody2D& collisionBody2D = collisionBody2DArray[i];

        unsigned int entityId;
        _FAIL_CHECK(componentStoresPtr_->GetComponentEntityId(&collisionBody2D, entityId));
        
        Project001::RenderedModel* renderedModelPtr;
        if (componentStoresPtr_->GetComponent<Project001::RenderedModel>(entityId, renderedModelPtr))
        {
            const float& positionX = collisionBody2D.GetPosition().x;
            const float& positionY = collisionBody2D.GetPosition().y;
            renderedModelPtr->SetPosition(positionX, positionY, 0.0f);
            const float& rotation = collisionBody2D.GetRotation();
            renderedModelPtr->ResetOrientation();
            renderedModelPtr->AddWorldRotationZ(rotation);
        }
    }
}