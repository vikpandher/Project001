#include "TestSceneBase001.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/LightSource.h"
#include "Engine/Components/RenderedModel.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/Renderer.h"
#include "Engine/SoundPlayer.h"
#include "Engine/Window.h"



// public: ---------------------------------------------------------------------

TestSceneBase001::TestSceneBase001()
    : componentStoresPtr_(nullptr)
    , rendererPtr_(nullptr)
    , windowPtr_(nullptr)
    , previousCursorDownPosition_(0.0f)
{
    ClearResources();
}

TestSceneBase001::~TestSceneBase001()
{}

const char* TestSceneBase001::Name()
{
    return "TestSceneBase001";
}

void TestSceneBase001::Initialize()
{
    _LOG_MESSAGE("INITIALIZING: %s", Name());

    windowPtr_ = GetApplicationWindowPtr();

    rendererPtr_ = GetApplicationRendererPtr();
    rendererPtr_->SetDepthTesting(true);

    soundPlayerPtr_ = GetApplicationSoundPlayerPtr();

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
        }
        cameraPtr->SetPosition(0.0f, 0.0f, 5.0f);
        cameraPtr->AddYaw(glm::pi<float>());
        // cameraPtr->SetProjectionToOrthographic();
        // cameraPtr->SetLeftCutoff(aspectRatio * -5.0f);
        // cameraPtr->SetRightCutoff(aspectRatio * 5.0f);
        cameraPtr->TurnOn();
    }

    // light source entity
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(lightSourceEntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::LightSource>(lightSourceEntityId_));

        Project001::LightSource* lightSourcePtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::LightSource>(lightSourceEntityId_, lightSourcePtr));
        lightSourcePtr->SetPosition(0.0f, 0.0f, 5.0f);
        lightSourcePtr->SetDirection(0.0f, 0.0f, -1.0f);
        lightSourcePtr->SetAmbientColor(0.1f, 0.1f, 0.1f);
        lightSourcePtr->SetSpecularColor(1.0f, 1.0f, 1.0f);
        lightSourcePtr->TurnOn();
    }
}

void TestSceneBase001::Deinitialize()
{
    ClearResources();
    rendererPtr_->DeleteAllTextures();
    soundPlayerPtr_->DeleteAllSoundSources();
    soundPlayerPtr_->DeleteAllSoundBuffers();
    componentStoresPtr_->DeleteAllEntities();
}

void TestSceneBase001::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestSceneBase001::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::FrameBufferSizeEvent>(event, std::bind(&TestSceneBase001::ProcessFrameBufferSizeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestSceneBase001::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestSceneBase001::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::ScrollEvent>(event, std::bind(&TestSceneBase001::ProcessScrollEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestSceneBase001::ProcessUpdateEvent, this, std::placeholders::_1));
}

// protected: ------------------------------------------------------------------

void TestSceneBase001::ClearResources()
{
    for (size_t i = 0; i < meshDataPtrArray_.size(); ++i)
    {
        delete meshDataPtrArray_[i];
    }
    meshDataPtrArray_.clear();

    mainCameraEntityId_ = (unsigned int)-1;
    lightSourceEntityId_ = (unsigned int)-1;
    entityIds_.clear();
}

void TestSceneBase001::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorButtonEvent)
{
    bool mouseButton1Pressed = windowPtr_->GetMouseButtonPressed(Project001::MouseButton::MOUSE_BUTTON_1);

    if (mouseButton1Pressed)
    {
        float& currentXPosition = cursorButtonEvent.xPosition;
        float& currentYPosition = cursorButtonEvent.yPosition;

        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

        float speedConstant = 0.005f;

        float xOffset = currentXPosition - previousCursorDownPosition_.x;
        float yOffset = currentYPosition - previousCursorDownPosition_.y;

        // moving cursor right = positive xOffset
        // moving cursor up = negative yOffset

        float cameraYaw = -1.0f * xOffset * speedConstant;
        float cameraPitch = yOffset * speedConstant;

        cameraPtr->AddYaw(cameraYaw);
        // cameraPtr->AddWorldRotationY(cameraYaw); // for fps camera
        cameraPtr->AddPitch(cameraPitch);

        previousCursorDownPosition_.x = currentXPosition;
        previousCursorDownPosition_.y = currentYPosition;
    }

    cursorButtonEvent.handled = true;
}

void TestSceneBase001::ProcessFrameBufferSizeEvent(Project001::FrameBufferSizeEvent& frameBufferSizeEvent)
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

void TestSceneBase001::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;

    if (mouseButton == Project001::MouseButton::MOUSE_BUTTON_1 &&
        buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS)
    {
        windowPtr_->GetCursorPosition(previousCursorDownPosition_.x, previousCursorDownPosition_.y);
    }

    mouseButtonEvent.handled = true;
}

void TestSceneBase001::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
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

        Project001::LightSource* lightSourceArray = nullptr;
        size_t lightSourceCount = 0;
        _FAIL_CHECK(componentStoresPtr_->GetAllComponents<Project001::LightSource>(lightSourceArray, lightSourceCount));

        for (unsigned int i = 0; i < lightSourceCount; ++i)
        {
            Project001::LightSource& currentLightSource = lightSourceArray[i];
            if (currentLightSource.IsTurnedOn())
            {
                if (currentLightSource.IsLightTypeDirectional())
                {
                    rendererPtr_->SetDirectionalLight(
                        currentLightSource.GetDirection(),
                        currentLightSource.GetAmbientColor(),
                        currentLightSource.GetDiffuseColor(),
                        currentLightSource.GetSpecularColor()
                    );
                }
                else if (currentLightSource.IsLightTypePoint())
                {
                    rendererPtr_->AddPointLight(
                        currentLightSource.GetPosition(),
                        currentLightSource.GetAttenuationConstant(),
                        currentLightSource.GetLinearAttenuation(),
                        currentLightSource.GetQuadraticAttenuation(),
                        currentLightSource.GetAmbientColor(),
                        currentLightSource.GetDiffuseColor(),
                        currentLightSource.GetSpecularColor()
                    );
                }
                else if (currentLightSource.IsLightTypeSpot())
                {
                    rendererPtr_->AddSpotLight(
                        currentLightSource.GetPosition(),
                        currentLightSource.GetDirection(),
                        currentLightSource.GetCutoff(),
                        currentLightSource.GetOuterCutoff(),
                        currentLightSource.GetAttenuationConstant(),
                        currentLightSource.GetLinearAttenuation(),
                        currentLightSource.GetQuadraticAttenuation(),
                        currentLightSource.GetAmbientColor(),
                        currentLightSource.GetDiffuseColor(),
                        currentLightSource.GetSpecularColor()
                    );
                }
            }
        }

        Project001::RenderedModel* renderedModelArray = nullptr;
        size_t renderedModelCount = 0;
        _FAIL_CHECK(componentStoresPtr_->GetAllComponents<Project001::RenderedModel>(renderedModelArray, renderedModelCount));

        std::vector<Project001::RenderedModel*> renderedModelPtrs;
        renderedModelPtrs.reserve(renderedModelCount);
        for (size_t i = 0; i < renderedModelCount; ++i)
        {
            renderedModelPtrs.emplace_back(&renderedModelArray[i]);
        }
        std::sort(renderedModelPtrs.begin(), renderedModelPtrs.end(),
            [cameraPtr](Project001::RenderedModel* a, Project001::RenderedModel* b)->bool
            {
                bool aTranslucent = a->GetTranslucent();
                bool bTranslucent = b->GetTranslucent();
                if (aTranslucent)
                {
                    if (bTranslucent)
                    {
                        // both are translucent so the farther is drawn first
                        glm::vec3 cameraPosition = cameraPtr->GetPosition();
                        float disatanceSquaredA = glm::dot(cameraPosition - a->GetPosition(), cameraPosition - a->GetPosition());
                        float disatanceSquaredB = glm::dot(cameraPosition - b->GetPosition(), cameraPosition - b->GetPosition());
                        return disatanceSquaredA > disatanceSquaredB;
                    }
                    else
                    {
                        // the one that is translucent is drawn last
                        return false;
                    }
                }
                if (bTranslucent)
                {
                    // the one that is translucent is drawn last
                    return true;
                }
                else
                {
                    // both are not translucent so the closer is drawn first
                    glm::vec3 cameraPosition = cameraPtr->GetPosition();
                    float disatanceSquaredA = glm::dot(cameraPosition - a->GetPosition(), cameraPosition - a->GetPosition());
                    float disatanceSquaredB = glm::dot(cameraPosition - b->GetPosition(), cameraPosition - b->GetPosition());
                    return disatanceSquaredA < disatanceSquaredB;
                }
            });

        for (unsigned int i = 0; i < renderedModelPtrs.size(); ++i)
        {
            Project001::RenderedModel*& currentRenderedModelPtr = renderedModelPtrs[i];

            if (currentRenderedModelPtr->IsVisible())
            {
                const Project001::MeshData* currentMeshDataPtr = currentRenderedModelPtr->GetMeshDataPtr();
                if (currentMeshDataPtr != nullptr)
                {
                    _FAIL_CHECK(rendererPtr_->AddMesh(
                        currentMeshDataPtr->meshVertexArray.data(),
                        (unsigned int)currentMeshDataPtr->meshVertexArray.size(),
                        currentMeshDataPtr->meshIndexArray.data(),
                        (unsigned int)currentMeshDataPtr->meshIndexArray.size(),
                        currentRenderedModelPtr->GetTextureId(),
                        currentRenderedModelPtr->GetSpecularId(),
                        currentRenderedModelPtr->GetPosition(),
                        currentRenderedModelPtr->GetOrientation(),
                        currentRenderedModelPtr->GetScale(),
                        currentRenderedModelPtr->GetColor(),
                        currentRenderedModelPtr->GetShininess(),
                        currentRenderedModelPtr->GetTranslucent(),
                        currentRenderedModelPtr->GetLit()));
                }
            }
        }

        rendererPtr_->Render();
    }

    rendererPtr_->FinishRendering();
    rendererPtr_->SwapBuffers();

    renderEvent.handled = true;
}

void TestSceneBase001::ProcessScrollEvent(Project001::ScrollEvent& scrollEvent)
{
    float& yOffset = scrollEvent.yOffset;

    float speedConstant = 0.1f;
    float cameraTranslation = speedConstant * yOffset;

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
    cameraPtr->MoveForward(cameraTranslation);

    scrollEvent.handled = true;
}

void TestSceneBase001::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    unsigned long timestep_ns = updateEvent.timestep_ns;

    // Update Entities
    UpdateMainCameraEntityPositionAndRoll(timestep_ns);

    SyncLightSourcePosition();

    // Delete all entities with marked for deletion component
    // DeleteDeadEntities();

    // Play all audio components???

    updateEvent.handled = true;
}

void TestSceneBase001::UpdateMainCameraEntityPositionAndRoll(unsigned long timestep_ns)
{
    float timestep_s = (float)(timestep_ns / 1000000) / 1000;

    float speedConstant = 1.0f;
    float cameraTranslationSpeed = speedConstant * timestep_s;
    float cameraRotationSpeed = speedConstant * 2.0f * timestep_s;

    bool movingLeft = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_A);
    bool movingRight = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_D);
    bool movingUp = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_W);
    bool movingDown = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_S);
    bool rollingLeft = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_Q);
    bool rollingRight = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_E);

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

    if (movingLeft)
    {
        cameraPtr->MoveLeft(cameraTranslationSpeed);
        // cameraPtr->RevolveAroundHorizontally(glm::vec3(0.0f, 0.0f, 0.0f), -1.0f * cameraRotationSpeed); // add to orbit
    }

    if (movingRight)
    {
        cameraPtr->MoveRight(cameraTranslationSpeed);
        // cameraPtr->RevolveAroundHorizontally(glm::vec3(0.0f, 0.0f, 0.0f), cameraRotationSpeed); // add to orbit
    }

    if (movingUp)
    {
        cameraPtr->MoveUp(cameraTranslationSpeed);
    }

    if (movingDown)
    {
        cameraPtr->MoveDown(cameraTranslationSpeed);
    }

    if (rollingLeft)
    {
        cameraPtr->AddRoll(-1.0f * cameraRotationSpeed);
    }

    if (rollingRight)
    {
        cameraPtr->AddRoll(cameraRotationSpeed);
    }

    // cameraPtr->LookAt(-1.0f * cameraPtr->GetPosition()); // add for hacky orbit
}

void TestSceneBase001::SyncLightSourcePosition()
{
    Project001::LightSource* lightSourcePtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::LightSource>(lightSourceEntityId_, lightSourcePtr));

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

    lightSourcePtr->SetPosition(cameraPtr->GetPosition());

    soundPlayerPtr_->UpdateListener(
        cameraPtr->GetPosition(),
        cameraPtr->GetForwardVector(),
        cameraPtr->GetUpVector(),
        glm::vec3(0.0f, 0.0f, 0.0f),
        1.0f
    );
}
