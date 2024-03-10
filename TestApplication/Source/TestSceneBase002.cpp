#include "TestSceneBase002.h"

#include "TestSceneIds.h"

#include "Components/Camera.h"
#include "Components/CollisionBody2D.h"
#include "CollisionSystem2D.h"
#include "Components/RenderedMesh.h"
#include "Math/Overlap2D.h"
#include "Math/CoordinateSystems.h"
#include "Math/VectorUtilities.h"
#include "CollisionSystem2D.h"
#include "ComponentStores.h"
#include "FreetypeTextLoader.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "RenderSystem.h"
#include "Window.h"



// public ----------------------------------------------------------------------

TestSceneBase002::TestSceneBase002(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , windowPtr_(nullptr)
    , rendererPtr_(nullptr)
    , componentStoresPtr_(nullptr)
    , font01_FontDataPtr_(nullptr)
    , font01_TextureDataPtr_(nullptr)
    , font01_TextureId_((unsigned int)-1)
    , cursorLinePositions_()
    , cursorLineMeshDataPtr_(nullptr)
    , distanceMeshDataPtr_(nullptr)
    , meshDataPtrArray_()
    , mainCameraEntityId_((unsigned int)-1)
    , uiCameraEntityId_((unsigned int)-1)
    , cursorEntityId_((unsigned int)-1)
    , distanceEntityId_((unsigned int)-1)
    , entityIds_()
    , cursorGrabbingEntity_(false)
    , previousWorldCursorPosition_()
    , selectedEntityIdIndex_((unsigned int)-1)
{}

TestSceneBase002::~TestSceneBase002()
{}

void TestSceneBase002::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestSceneBase002::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestSceneBase002::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestSceneBase002::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestSceneBase002::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestSceneBase002::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestSceneBase002::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::ScrollEvent>(event, std::bind(&TestSceneBase002::ProcessScrollEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestSceneBase002::ProcessUpdateEvent, this, std::placeholders::_1));
}

// protected -------------------------------------------------------------------

void TestSceneBase002::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    _LOG_MESSAGE("INITIALIZING:   TestSceneBase002:        %u", GetId());

    windowPtr_ = GetApplicationWindowPtr();
    rendererPtr_ = GetApplicationRendererPtr();
    componentStoresPtr_ = GetApplicaitonComponentStoresPtr();

    // Load textures
    // -------------------------------------------------------------------------

    {
        font01_FontDataPtr_ = new Project001::FontData();
        font01_TextureDataPtr_ = new Project001::TextureData();
        std::vector<unsigned char> characterList;
        for (unsigned char c = 32; c < 127; ++c) // ASCII characters
        {
            characterList.push_back(c);
        }
        _FAIL_CHECK(Project001::FreetypeTextLoader::LoadTextureDataAndFontData(
            *font01_TextureDataPtr_,
            *font01_FontDataPtr_,
            characterList,
            "../Fonts/Antonio-Regular.ttf",
            48
        ));
        rendererPtr_->CreateTexture(
            font01_TextureId_,
            font01_TextureDataPtr_->data,
            font01_TextureDataPtr_->width,
            font01_TextureDataPtr_->height,
            font01_TextureDataPtr_->bytesPerPixel,
            false,
            false
        );
    }

    // Generate meshes
    // -------------------------------------------------------------------------

    cursorLineMeshDataPtr_ = new Project001::MeshData();

    distanceMeshDataPtr_ = new Project001::MeshData();

    // Main Camera Entity
    // -------------------------------------------------------------------------

    {
        componentStoresPtr_->CreateEntity(mainCameraEntityId_);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::Camera>(mainCameraEntityId_));
        Project001::Camera* cameraPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
        if (cameraPtr != nullptr)
        {
            int aspectRatioNumerator;
            int aspectRatioDenominator;
            windowPtr_->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
            if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
            {
                float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;
                float secondaryCameraHalfHeight = 3.5f;
                float secondaryCameraHalfWidth = aspectRatio * secondaryCameraHalfHeight;
                cameraPtr->SetAspectRatio(aspectRatio);
                cameraPtr->SetTopCutoff(secondaryCameraHalfHeight);
                cameraPtr->SetBottomCutoff(-secondaryCameraHalfHeight);
                cameraPtr->SetLeftCutoff(-secondaryCameraHalfWidth);
                cameraPtr->SetRightCutoff(secondaryCameraHalfWidth);
                cameraPtr->SetNearCutoff(-1.0f);
                cameraPtr->SetFarCutoff(1.0f);
            }
            cameraPtr->AddYaw(glm::pi<float>());
            cameraPtr->SetProjection(Project001::Camera::CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC);
            cameraPtr->SetDepthTestEnabled(false);
            cameraPtr->TurnOn();
        }
    }

    // UI Camera Entity
    // -------------------------------------------------------------------------

    float uiCameraHalfHeight = 0.0f;
    float uiCameraHalfWidth = 0.0f;

    {
        componentStoresPtr_->CreateEntity(uiCameraEntityId_);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::Camera>(uiCameraEntityId_));
        Project001::Camera* cameraPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(cameraPtr, uiCameraEntityId_));
        if (cameraPtr != nullptr)
        {
            int aspectRatioNumerator;
            int aspectRatioDenominator;
            windowPtr_->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
            if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
            {
                float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;
                uiCameraHalfHeight = 3.5f;
                uiCameraHalfWidth = aspectRatio * uiCameraHalfHeight;
                cameraPtr->SetAspectRatio(aspectRatio);
                cameraPtr->SetTopCutoff(uiCameraHalfHeight);
                cameraPtr->SetBottomCutoff(-uiCameraHalfHeight);
                cameraPtr->SetLeftCutoff(-uiCameraHalfWidth);
                cameraPtr->SetRightCutoff(uiCameraHalfWidth);
                cameraPtr->SetNearCutoff(-1.0f);
                cameraPtr->SetFarCutoff(1.0f);
            }
            cameraPtr->AddYaw(glm::pi<float>());
            cameraPtr->SetProjection(Project001::Camera::CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC);
            cameraPtr->SetDepthTestEnabled(false);
            cameraPtr->TurnOn();
            cameraPtr->SetCameraMask(s_uiCameraMask_);
            cameraPtr->SetPriorityValue(1000000);
        }
    }

    // Cursor Entity
    // -------------------------------------------------------------------------

    {
        componentStoresPtr_->CreateEntity(cursorEntityId_);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(cursorEntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, cursorEntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetMeshDataPtr(cursorLineMeshDataPtr_);
            renderedMeshPtr->SetColor(0.4f, 0.4f, 0.4f, 1.0f);
            renderedMeshPtr->SetRenderPriorityOverride(1);
        }

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(cursorEntityId_));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursorEntityId_));
        if (collisionBody2DPtr != nullptr)
        {
            std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
            collisionPoints.emplace_back(glm::vec3(), 1, true);
        }
    }

    // Distance Test Entity
    // -------------------------------------------------------------------------

    {
        componentStoresPtr_->CreateEntity(distanceEntityId_);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(distanceEntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, distanceEntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_uiCameraMask_);
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetMeshDataPtr(distanceMeshDataPtr_);
            renderedMeshPtr->SetTextureId(font01_TextureId_);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetPositionX(uiCameraHalfWidth - 0.9f);
            renderedMeshPtr->SetPositionY(uiCameraHalfHeight - 0.4f);
        }
    }
}

void TestSceneBase002::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    _LOG_MESSAGE("DEINITIALIZING: TestSceneBase002:        %u", GetId());

    rendererPtr_->DeleteAllTextures();
    rendererPtr_->DeleteAllMeshes();
    componentStoresPtr_->DeleteAllEntities();

    windowPtr_ = nullptr;
    rendererPtr_ = nullptr;
    componentStoresPtr_ = nullptr;

    // Texture Data: -----------------------------------------------------------

    delete font01_FontDataPtr_;
    font01_FontDataPtr_ = nullptr;

    delete font01_TextureDataPtr_;
    font01_TextureDataPtr_ = nullptr;

    font01_TextureId_ = (unsigned int)-1;

    // Mesh Data ---------------------------------------------------------------

    delete cursorLineMeshDataPtr_;
    cursorLineMeshDataPtr_ = nullptr;

    delete distanceMeshDataPtr_;
    distanceMeshDataPtr_ = nullptr;

    for (size_t i = 0; i < meshDataPtrArray_.size(); ++i)
    {
        delete meshDataPtrArray_[i];
    }
    meshDataPtrArray_.clear();

    // Entity Ids --------------------------------------------------------------

    mainCameraEntityId_ = (unsigned int)-1;
    uiCameraEntityId_ = (unsigned int)-1;

    cursorEntityId_ = (unsigned int)-1;

    distanceEntityId_ = (unsigned int)-1;

    entityIds_.clear();

    // -------------------------------------------------------------------------

    cursorGrabbingEntity_ = false;
    previousWorldCursorPosition_ = glm::vec2(0.0f, 0.0f);
    selectedEntityIdIndex_ = (unsigned int)-1;
}

void TestSceneBase002::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent)
{
    const float& xPosition = cursorPositionEvent.xPosition;
    const float& yPosition = cursorPositionEvent.yPosition;

    UpdateWorldCursor(xPosition, yPosition);
}

void TestSceneBase002::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_ESCAPE)
        {
            SendEventToApplication(Project001::SwitchSceneEvent(g_testScene001Id));
            if (GetActiveScene()->GetId() == g_testScene001Id)
            {
                SendEventToScene(GetId(), Project001::DeinitializeEvent());
                SendEventToApplication(Project001::InitializeEvent());
            }
            return;
        }
        else if (keyCode == Project001::KeyCode::KEY_CODE_N)
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

        Project001::CollisionSystem2D::CalculateCollisionsForGivenEntity(cursorEntityId_, componentStoresPtr_);

        Project001::CollisionBody2D* cursorCollisionBody2DPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(cursorCollisionBody2DPtr, cursorEntityId_));
        if(cursorCollisionBody2DPtr != nullptr)
        {
            const std::vector<Project001::CollisionData2D>& cursorCollisions = cursorCollisionBody2DPtr->GetCollisions();

            // This loop goes backwards to grab the component drawn last first.
            // This relies on the order the components and render bodies were 
            // added to select the one rendered behind.
            for (int i = (int)cursorCollisions.size() - 1; i >= 0; --i)
            // for (size_t i = 0; i < cursorCollisions.size(); ++i)
            {
                const Project001::CollisionData2D& currentCollision = cursorCollisions[i];

                for (unsigned int j = 0; j < entityIds_.size(); ++j)
                {
                    if (currentCollision.otherEntityId == entityIds_[j])
                    {
                        selectedEntityIdIndex_ = j;

                        cursorGrabbingEntity_ = true;
                        break;
                    }
                }
            }
        }
    }
}

void TestSceneBase002::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    Project001::RenderSystem::Render(componentStoresPtr_, rendererPtr_);
}

void TestSceneBase002::ProcessScrollEvent(Project001::ScrollEvent& scrollEvent)
{
    float& yOffset = scrollEvent.yOffset;

    float speedConstant = 0.1f;
    float cameraResize = 1.0f + speedConstant * yOffset;

    Project001::Camera* cameraPtr = nullptr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
    if (cameraPtr != nullptr)
    {
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
    }

    float xPosition = 0.0f;
    float yPosition = 0.0f;
    windowPtr_->GetCursorPosition(xPosition, yPosition);
    UpdateWorldCursor(xPosition, yPosition);
}

void TestSceneBase002::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    unsigned long long timestep_ns = updateEvent.timestep_ns;

    UpdatedSelectedEntityPosition(timestep_ns);
    Sync_RenderedMesh_CollisionBody_Components();

    Project001::CollisionSystem2D::CalculateCollisions(componentStoresPtr_);

    ColorCollisions();
    UpdateCursorLineAndDistanceText();
}

void TestSceneBase002::UpdateWorldCursor(float xPosition, float yPosition)
{
    int windowWidth, windowHeight;
    windowPtr_->GetWindowSize(windowWidth, windowHeight);

    unsigned int xOffset, yOffset, viewportWidth, viewportHeight;
    rendererPtr_->GetViewport(xOffset, yOffset, viewportWidth, viewportHeight);

    // Convert coordinates from window to viewport
    glm::vec2 viewportCursorPosition(
        xPosition - xOffset,
        windowHeight - yOffset - yPosition
    );

    if (viewportCursorPosition.x < viewportWidth || viewportCursorPosition.y < viewportHeight)
    {
        Project001::Camera* cameraPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
        Project001::CollisionBody2D* cursorCollisionBody2DPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(cursorCollisionBody2DPtr, cursorEntityId_));
        if (cameraPtr != nullptr && cursorCollisionBody2DPtr != nullptr)
        {
            // Convert coordinates from viewport to world
            glm::vec2 worldCursorPosition = cameraPtr->ConvertPointFromViewportToOrthoWorld(
                viewportWidth,
                viewportHeight,
                viewportCursorPosition
            );

            cursorCollisionBody2DPtr->SetPosition(worldCursorPosition);

            if (cursorGrabbingEntity_ && selectedEntityIdIndex_ < entityIds_.size())
            {
                unsigned int selectedEntityId = entityIds_[selectedEntityIdIndex_];

                Project001::CollisionBody2D* selectedCollisionBody2DPtr = nullptr;
                _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(selectedCollisionBody2DPtr, selectedEntityId));
                if (selectedCollisionBody2DPtr != nullptr)
                {
                    selectedCollisionBody2DPtr->AddTranslation(worldCursorPosition - previousWorldCursorPosition_);
                }
            }

            previousWorldCursorPosition_ = worldCursorPosition;
        }
    }
}

void TestSceneBase002::UpdatedSelectedEntityPosition(unsigned long long timestep_ns)
{
    Project001::Camera* cameraPtr = nullptr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
    if (cameraPtr != nullptr)
    {
        float timestep_s = (float)(timestep_ns / 1000000) / 1000;

        float speedConstant = cameraPtr->GetTopCutoff();
        glm::vec2 cameraUp = cameraPtr->GetUpVector();
        glm::vec2 cameraLeft = cameraPtr->GetLeftVector();

        float translationSpeed = speedConstant * 0.8f * timestep_s;
        float rotationSpeed = speedConstant * 1.2f * timestep_s;

        bool movingLeft = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_A);
        bool movingRight = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_D);
        bool movingUp = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_W);
        bool movingDown = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_S);
        bool rollingLeft = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_Q);
        bool rollingRight = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_E);

        if (selectedEntityIdIndex_ < entityIds_.size())
        {
            unsigned int selectedEntityId = entityIds_[selectedEntityIdIndex_];

            Project001::CollisionBody2D* collisionBodyPtr = nullptr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, selectedEntityId));
            if (collisionBodyPtr != nullptr)
            {
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
        }
        else
        {
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

            float xPosition = 0.0f;
            float yPosition = 0.0f;
            windowPtr_->GetCursorPosition(xPosition, yPosition);
            UpdateWorldCursor(xPosition, yPosition);
        }
    }
}

void TestSceneBase002::Sync_RenderedMesh_CollisionBody_Components()
{
    Project001::CollisionBody2D* collisionBody2DArray = nullptr;
    size_t collisionBodyCount = 0;

    componentStoresPtr_->GetAllComponents<Project001::CollisionBody2D>(collisionBody2DArray, collisionBodyCount);

    for (unsigned int i = 0; i < collisionBodyCount; ++i)
    {
        Project001::CollisionBody2D& collisionBody2D = collisionBody2DArray[i];

        unsigned int entityId;
        _FAIL_CHECK(componentStoresPtr_->GetComponentEntityId(entityId, &collisionBody2D));

        Project001::RenderedMesh* renderedMeshPtr;
        if (componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, entityId))
        {
            const float& positionX = collisionBody2D.GetPosition().x;
            const float& positionY = collisionBody2D.GetPosition().y;
            renderedMeshPtr->SetPosition(positionX, positionY, 0.0f);
            const float& rotation = collisionBody2D.GetRotation();
            renderedMeshPtr->ResetOrientation();
            renderedMeshPtr->AddWorldRotationZ(rotation);
        }
    }
}

void TestSceneBase002::ColorCollisions()
{
    Project001::CollisionBody2D* collisionBodyPtrs = nullptr;
    size_t collisionBodyCount = 0;
    componentStoresPtr_->GetAllComponents<Project001::CollisionBody2D>(collisionBodyPtrs, collisionBodyCount);
    for (size_t i = 0; i < collisionBodyCount; ++i)
    {
        bool collisionBodyColliding = false;

        Project001::CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];
        const std::vector<Project001::CollisionData2D>& currentCollisions = currentCollisionBody.GetCollisions();
        for (size_t j = 0; j < currentCollisions.size(); ++j)
        {
            const Project001::CollisionData2D& currentCollision = currentCollisions[j];
            if (currentCollision.otherShapeTag == 0)
            {
                collisionBodyColliding = true;
                break;
            }
        }

        unsigned int collidingEntityId;
        _FAIL_CHECK(componentStoresPtr_->GetComponentEntityId(collidingEntityId , &currentCollisionBody));

        // Don't recolor the cursor
        if (collidingEntityId == cursorEntityId_)
        {
            continue;
        }

        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        bool renderedMeshFound = componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, collidingEntityId);

        if (renderedMeshFound)
        {
            if (collisionBodyColliding)
            {
                if (selectedEntityIdIndex_ < entityIds_.size() && collidingEntityId == entityIds_[selectedEntityIdIndex_])
                {
                    renderedMeshPtr->SetColorRGB(1.0f, 0.75f, 0.25f);
                }
                else
                {
                    renderedMeshPtr->SetColorRGB(1.0f, 0.25f, 0.25f);
                }
            }
            else
            {
                if (selectedEntityIdIndex_ < entityIds_.size() && collidingEntityId == entityIds_[selectedEntityIdIndex_])
                {
                    renderedMeshPtr->SetColorRGB(0.25f, 0.25f, 1.0f);
                }
                else
                {
                    renderedMeshPtr->SetColorRGB(1.0f, 1.0f, 1.0f);
                }
            }
        }
    }
}

void TestSceneBase002::UpdateCursorLineAndDistanceText()
{
    if (selectedEntityIdIndex_ < entityIds_.size())
    {
        const unsigned int& selectedEntityId = entityIds_[selectedEntityIdIndex_];
        Project001::CollisionBody2D* collisionBodyPtr = nullptr;
        bool collisionBodyFound = componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, selectedEntityId);
        if (collisionBodyFound)
        {
            float distanceSquared = collisionBodyPtr->GetDistanceSquaredToPoint(previousWorldCursorPosition_);
            if (distanceSquared != std::numeric_limits<float>::infinity())
            {
                glm::vec2 closestPoint_position;
                float distanceSquared_unused = collisionBodyPtr->GetDistanceSquaredToPointAndClosestPointToPoint(previousWorldCursorPosition_, closestPoint_position);
                cursorLinePositions_.clear();
                cursorLinePositions_.emplace_back();
                cursorLinePositions_.emplace_back(closestPoint_position - previousWorldCursorPosition_);

                cursorLineMeshDataPtr_->Clear();
                Project001::MeshLoader::Generate2DLine(*cursorLineMeshDataPtr_, cursorLinePositions_, 0.01f); // No Fail Check

                distanceMeshDataPtr_->Clear();
                _FAIL_CHECK(Project001::FreetypeTextLoader::LoadMeshData(
                    *distanceMeshDataPtr_,
                    *font01_FontDataPtr_,
                    std::to_string(std::sqrtf(distanceSquared)),
                    fontPixelSize_
                ));
            }
            else
            {
                cursorLineMeshDataPtr_->Clear();

                distanceMeshDataPtr_->Clear();
            }
        }
    }
    else
    {
        cursorLineMeshDataPtr_->Clear();

        distanceMeshDataPtr_->Clear();
    }
}