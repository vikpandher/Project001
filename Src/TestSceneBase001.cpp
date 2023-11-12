#include "TestSceneBase001.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/LightSource.h"
#include "Engine/Components/RenderedMesh.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/Renderer.h"
#include "Engine/RenderSystem.h"
#include "Engine/SoundPlayer.h"
#include "Engine/Window.h"



// public ----------------------------------------------------------------------

TestSceneBase001::TestSceneBase001(Project001::Application* applicationPtr, const std::string& name)
    : Scene(applicationPtr, name)
    , windowPtr_(nullptr)
    , rendererPtr_(nullptr)
    , soundPlayerPtr_(nullptr)
    , componentStoresPtr_(nullptr)
    , meshDataPtrArray_()
    , mainCameraEntityId_((unsigned int)-1)
    , uiCameraEntityId_((unsigned int)-1)
    , lightSourceEntityId_((unsigned int)-1)
    , entityIds_()
    , previousCursorDownPosition_()
{}

TestSceneBase001::~TestSceneBase001()
{}

void TestSceneBase001::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestSceneBase001::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestSceneBase001::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestSceneBase001::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::FrameBufferSizeEvent>(event, std::bind(&TestSceneBase001::ProcessFrameBufferSizeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestSceneBase001::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestSceneBase001::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestSceneBase001::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::ScrollEvent>(event, std::bind(&TestSceneBase001::ProcessScrollEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestSceneBase001::ProcessUpdateEvent, this, std::placeholders::_1));
}

// protected -------------------------------------------------------------------

void TestSceneBase001::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    _LOG_MESSAGE("INITIALIZING: %s", GetName().c_str());

    windowPtr_ = GetApplicationWindowPtr();

    rendererPtr_ = GetApplicationRendererPtr();

    soundPlayerPtr_ = GetApplicationSoundPlayerPtr();

    componentStoresPtr_ = GetApplicaitonComponentStoresPtr();

    // Main Camera Entity
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
        cameraPtr->SetPosition(0.0f, 0.0f, 7.5f);
        cameraPtr->AddYaw(glm::pi<float>());
        cameraPtr->TurnOn();
    }

    // UI Camera Entity
    // -------------------------------------------------------------------------

    float uiCameraHalfHeight = 0.0f;
    float uiCameraHalfWidth = 0.0f;

    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(uiCameraEntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::Camera>(uiCameraEntityId_));

        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(uiCameraEntityId_, cameraPtr));
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

    // Light Source Entity
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

    windowPtr_->GetCursorPosition(previousCursorDownPosition_.x, previousCursorDownPosition_.y);
}

void TestSceneBase001::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    // _LOG_MESSAGE("DEINITIALIZING: %s", GetName().c_str());

    rendererPtr_->DeleteAllTextures();
    rendererPtr_->DeleteAllMeshes();
    soundPlayerPtr_->DeleteAllSoundSources();
    soundPlayerPtr_->DeleteAllSoundBuffers();
    componentStoresPtr_->DeleteAllEntities();

    windowPtr_ = nullptr;
    rendererPtr_ = nullptr;
    soundPlayerPtr_ = nullptr;
    componentStoresPtr_ = nullptr;

    // Mesh Data ---------------------------------------------------------------

    for (size_t i = 0; i < meshDataPtrArray_.size(); ++i)
    {
        delete meshDataPtrArray_[i];
    }
    meshDataPtrArray_.clear();

    // Entity Ids --------------------------------------------------------------

    mainCameraEntityId_ = (unsigned int)-1;
    uiCameraEntityId_ = (unsigned int)-1;

    lightSourceEntityId_ = (unsigned int)-1;

    entityIds_.clear();

    // -------------------------------------------------------------------------

    previousCursorDownPosition_ = glm::vec2(0.0f, 0.0f);
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
}

void TestSceneBase001::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_ESCAPE)
        {
            SendEventToApplication(Project001::SwitchSceneEvent("TestScene001"));
            if (GetActiveScene()->GetName() == "TestScene001")
            {
                SendEventToScene(GetName(), Project001::DeinitializeEvent());
                SendEventToApplication(Project001::InitializeEvent());
            }
        }
    }
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
}

void TestSceneBase001::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    Project001::RenderSystem::Render(componentStoresPtr_, rendererPtr_);

    // _LOG_MESSAGE("timestep_ns: %llu", renderEvent.timestep_ns);
}

void TestSceneBase001::ProcessScrollEvent(Project001::ScrollEvent& scrollEvent)
{
    float& yOffset = scrollEvent.yOffset;

    float speedConstant = 0.2f;
    float cameraTranslation = speedConstant * yOffset;

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
    cameraPtr->MoveBack(cameraTranslation);
}

void TestSceneBase001::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    unsigned long long timestep_ns = updateEvent.timestep_ns;

    // Update Entities
    UpdateMainCameraEntityPositionAndRoll(timestep_ns);

    SyncLightSourcePosition();
}

void TestSceneBase001::UpdateMainCameraEntityPositionAndRoll(unsigned long long timestep_ns)
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
}