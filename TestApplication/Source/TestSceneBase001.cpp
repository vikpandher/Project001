// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-13

#include "TestSceneBase001.h"

#include "TestApplicationData.h"

#include "Components/Camera.h"
#include "Components/LightSource.h"
#include "Components/RenderedMesh.h"
#include "ComponentStores.h"
#include "Logger.h"
#include "RenderSystem.h"
#include "SoundPlayer.h"
#include "TimeProfiler.h"
#include "Window.h"



// public ----------------------------------------------------------------------

TestSceneBase001::TestSceneBase001(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , meshDataPtrArray_()
    , mainCameraEntityId_(static_cast<unsigned int>(-1))
    , uiCameraEntityId_(static_cast<unsigned int>(-1))
    , lightSourceEntityId_(static_cast<unsigned int>(-1))
    , entityIds_()
    , previousCursorDownPosition_()
    , remainingTimeRecordingDuration_ns_(0)
{}

TestSceneBase001::~TestSceneBase001()
{}

void TestSceneBase001::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestSceneBase001::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestSceneBase001::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestSceneBase001::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestSceneBase001::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestSceneBase001::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestSceneBase001::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::ScrollEvent>(event, std::bind(&TestSceneBase001::ProcessScrollEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestSceneBase001::ProcessUpdateEvent, this, std::placeholders::_1));
}

// protected -------------------------------------------------------------------

void TestSceneBase001::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestSceneBase001:        " << GetId());

    // Main Camera Entity
    // -------------------------------------------------------------------------
    {
        GetComponentStoresPtr()->CreateEntity(mainCameraEntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::Camera>(mainCameraEntityId_));
        Project001::Camera* cameraPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
        if (cameraPtr)
        {
            int aspectRatioNumerator;
            int aspectRatioDenominator;
            GetWindowPtr()->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
            if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
            {
                float aspectRatio = static_cast<float>(aspectRatioNumerator) / static_cast<float>(aspectRatioDenominator);
                cameraPtr->SetAspectRatio(aspectRatio);
            }
            cameraPtr->SetPosition(0.0f, 0.0f, 7.5f);
            cameraPtr->AddYaw(glm::pi<float>());
            cameraPtr->SetCameraMask(s_mainCameraMask_);
        }
    }

    // UI Camera Entity
    // -------------------------------------------------------------------------

    float uiCameraHalfHeight = 0.0f;
    float uiCameraHalfWidth = 0.0f;

    {
        GetComponentStoresPtr()->CreateEntity(uiCameraEntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::Camera>(uiCameraEntityId_));
        Project001::Camera* cameraPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, uiCameraEntityId_));
        if (cameraPtr != nullptr)
        {
            int aspectRatioNumerator;
            int aspectRatioDenominator;
            GetWindowPtr()->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
            if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
            {
                float aspectRatio = static_cast<float>(aspectRatioNumerator) / static_cast<float>(aspectRatioDenominator);
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
            cameraPtr->SetCameraMask(s_uiCameraMask_);
            cameraPtr->SetPriorityValue(1000000);
        }
    }

    // Light Source Entity
    // -------------------------------------------------------------------------
    {
        GetComponentStoresPtr()->CreateEntity(lightSourceEntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::LightSource>(lightSourceEntityId_));
        Project001::LightSource* lightSourcePtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::LightSource>(lightSourcePtr, lightSourceEntityId_));
        if (lightSourcePtr != nullptr)
        {
            lightSourcePtr->SetPosition(0.0f, 0.0f, 5.0f);
            lightSourcePtr->SetDirection(0.0f, 0.0f, -1.0f);
            lightSourcePtr->SetAmbientColor(0.1f, 0.1f, 0.1f);
            lightSourcePtr->SetSpecularColor(1.0f, 1.0f, 1.0f);
            lightSourcePtr->TurnOn();
        }
    }

    GetWindowPtr()->GetCursorPosition(previousCursorDownPosition_.x, previousCursorDownPosition_.y);
}

void TestSceneBase001::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    LOG_INFO("DEINITIALIZING: TestSceneBase001:        " << GetId());

    GetRendererPtr()->DeleteAllTextures();
    GetRendererPtr()->DeleteAllMeshes();
    GetSoundPlayerPtr()->DeleteAllSoundSources();
    GetSoundPlayerPtr()->DeleteAllSoundBuffers();
    GetSoundPlayerPtr()->ResetListener();
    GetComponentStoresPtr()->DeleteAllEntities();

    // Mesh Data ---------------------------------------------------------------

    for (size_t i = 0; i < meshDataPtrArray_.size(); ++i)
    {
        delete meshDataPtrArray_[i];
    }
    meshDataPtrArray_.clear();

    // Entity Ids --------------------------------------------------------------

    mainCameraEntityId_ = static_cast<unsigned int>(-1);
    uiCameraEntityId_ = static_cast<unsigned int>(-1);

    lightSourceEntityId_ = static_cast<unsigned int>(-1);

    entityIds_.clear();

    // -------------------------------------------------------------------------

    previousCursorDownPosition_ = glm::vec2(0.0f, 0.0f);

    if (remainingTimeRecordingDuration_ns_ > 0)
    {
        remainingTimeRecordingDuration_ns_ = 0;

        if (Project001::TimeProfiler::EndSession())
        {
            LOG_INFO("RECORDING END");
        }
    }
}

void TestSceneBase001::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorButtonEvent)
{
    bool mouseButton1Pressed = GetWindowPtr()->GetMouseButtonPressed(Project001::MouseButton::MOUSE_BUTTON_1);

    if (mouseButton1Pressed)
    {
        float& currentXPosition = cursorButtonEvent.xPosition;
        float& currentYPosition = cursorButtonEvent.yPosition;

        Project001::Camera* cameraPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
        if (cameraPtr != nullptr)
        {
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
            TestApplicationData* testApplicationDataPtr = GetSharedDataPtr<TestApplicationData>();
            SendEventToApplication(Project001::SwitchSceneEvent(testApplicationDataPtr->testScene001Id));
            if (GetActiveScene()->GetId() == testApplicationDataPtr->testScene001Id)
            {
                SendEventToScene(GetId(), Project001::DeinitializeEvent());
                SendEventToApplication(Project001::InitializeEvent());
            }
            return;
        }
    }
    else if (keyCode == Project001::KeyCode::KEY_CODE_0)
    {
        std::string timeProfileOutFileName = "TestSceneBase002_";
#ifndef NDEBUG
        timeProfileOutFileName += "D_";
#else
        timeProfileOutFileName += "R_";
#endif
        if (GetSleepyRunLoop())
        {
            timeProfileOutFileName += "Sleepy_";
        }
        else
        {
            timeProfileOutFileName += "Fast_";
        }
        timeProfileOutFileName += std::to_string(Project001::TimeProfiler::GetTimeStamp());
        timeProfileOutFileName += ".json";
        if (Project001::TimeProfiler::BeginSession(timeProfileOutFileName.c_str()))
        {
            LOG_INFO("RECORDING START");
        }
        remainingTimeRecordingDuration_ns_ = 1000000000;
    }
}

void TestSceneBase001::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;

    if (mouseButton == Project001::MouseButton::MOUSE_BUTTON_1 &&
        buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS)
    {
        GetWindowPtr()->GetCursorPosition(previousCursorDownPosition_.x, previousCursorDownPosition_.y);
    }
}

void TestSceneBase001::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    Project001::ScopeTimer scopeTimer("TestSceneBase002::ProcessRenderEvent");

    SyncLightSourcePosition();

    Project001::RenderSystem::Render(GetComponentStoresPtr(), GetRendererPtr());

    // _LOG_MESSAGE("timestep_ns: %llu", renderEvent.timestep_ns);
}

void TestSceneBase001::ProcessScrollEvent(Project001::ScrollEvent& scrollEvent)
{
    float& yOffset = scrollEvent.yOffset;

    float speedConstant = 0.2f;
    float cameraTranslation = speedConstant * yOffset;

    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
    if (cameraPtr != nullptr)
    {
        cameraPtr->MoveBack(cameraTranslation);
    }
}

void TestSceneBase001::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    Project001::ScopeTimer scopeTimer("TestSceneBase002::ProcessUpdateEvent");

    unsigned long long timestep_ns = updateEvent.timestep_ns;

    // Update Entities
    UpdateMainCameraEntityPositionAndRoll(timestep_ns);

    if (remainingTimeRecordingDuration_ns_ > 0)
    {
        if (remainingTimeRecordingDuration_ns_ < timestep_ns)
        {
            remainingTimeRecordingDuration_ns_ = 0;
        }
        else
        {
            remainingTimeRecordingDuration_ns_ -= timestep_ns;
        }

        if (remainingTimeRecordingDuration_ns_ == 0)
        {
            if (Project001::TimeProfiler::EndSession())
            {
                LOG_INFO("RECORDING END");
            }
        }
    }
}

void TestSceneBase001::UpdateMainCameraEntityPositionAndRoll(unsigned long long timestep_ns)
{
    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
    if (cameraPtr != nullptr)
    {
        float timestep_s = static_cast<float>(timestep_ns) / 1e9f;

        float speedConstant = 1.0f;
        float cameraTranslationSpeed = speedConstant * timestep_s;
        float cameraRotationSpeed = speedConstant * 2.0f * timestep_s;

        bool movingLeft = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_A);
        bool movingRight = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_D);
        bool movingUp = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_W);
        bool movingDown = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_S);
        bool rollingLeft = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_Q);
        bool rollingRight = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_E);

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
}

void TestSceneBase001::SyncLightSourcePosition()
{
    Project001::LightSource* lightSourcePtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::LightSource>(lightSourcePtr, lightSourceEntityId_));
    if (lightSourcePtr != nullptr)
    {
        Project001::Camera* cameraPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
        if (cameraPtr != nullptr)
        {
            lightSourcePtr->SetPosition(cameraPtr->GetPosition());
        }
    }
}