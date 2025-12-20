// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-20

#include "TestScene101.h"

#include "TestApplicationData.h"
#include "TestResource_AntonioRegular_png.h"
#include "TestResource_AntonioRegular_ssf.h"

#include "Components/Camera.h"
#include "Components/RenderedModel.h"
#include "Utilities/FontUtility.h"
#include "Utilities/MathUtility.h"
#include "Utilities/MeshUtility.h"
#include "Utilities/TextureUtility.h"
#include "ComponentStores.h"
#include "Logger.h"
#include "RenderSystem.h"
#include "Window.h"



// public ----------------------------------------------------------------------

TestScene101::TestScene101(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , instructionScene_(applicationPtr)
    , font01_FontDataPtr_(nullptr)
    , font01_TextureDataPtr_(nullptr)
    , font01_TextureId_(static_cast<unsigned int>(-1))
    , mainCameraEntityId_(static_cast<unsigned int>(-1))
    , uiCameraEntityId_(static_cast<unsigned int>(-1))
{
    GetSharedDataPtr<TestApplicationData>()->testScene101Id = GetId();
}

TestScene101::~TestScene101()
{}

void TestScene101::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene101::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene101::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene101::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene101::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene101::ProcessUpdateEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene101::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene101:            " << GetId());

    // FontUtils Data ---------------------------------------------------------------

    {
        font01_FontDataPtr_ = new Project001::FontData;
        FAIL_CHECK(Project001::Font::LoadFontDataFromMemory(
            *font01_FontDataPtr_,
            g_AntonioRegular_ssf,
            sizeof(g_AntonioRegular_ssf)
        ));

        font01_TextureDataPtr_ = new Project001::TextureData;
        FAIL_CHECK(Project001::Texture::LoadTextureFromMemory(
            *font01_TextureDataPtr_,
            g_AntonioRegular_png,
            sizeof(g_AntonioRegular_png)
        ));
        GetRendererPtr()->CreateTexture(
            font01_TextureId_,
            font01_TextureDataPtr_->data,
            font01_TextureDataPtr_->width,
            font01_TextureDataPtr_->height,
            font01_TextureDataPtr_->bytesPerPixel,
            true,
            false
        );
    }

    // Entity Ids --------------------------------------------------------------

    // Main Camera Entity

    float mainCameraHalfHeight = 0.0f;
    float mainCameraHalfWidth = 0.0f;
    {
        GetComponentStoresPtr()->CreateEntity(mainCameraEntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::Camera>(mainCameraEntityId_));
        Project001::Camera* cameraPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
        if (cameraPtr != nullptr)
        {
            int aspectRatioNumerator;
            int aspectRatioDenominator;
            GetWindowPtr()->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
            if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
            {
                float aspectRatio = static_cast<float>(aspectRatioNumerator) / static_cast<float>(aspectRatioDenominator);
                mainCameraHalfHeight = 2.75f;
                mainCameraHalfWidth = aspectRatio * mainCameraHalfHeight;
                cameraPtr->SetAspectRatio(aspectRatio);
                cameraPtr->SetTopCutoff(mainCameraHalfHeight);
                cameraPtr->SetBottomCutoff(-mainCameraHalfHeight);
                cameraPtr->SetLeftCutoff(-mainCameraHalfWidth);
                cameraPtr->SetRightCutoff(mainCameraHalfWidth);
                cameraPtr->SetNearCutoff(-1.0f);
                cameraPtr->SetFarCutoff(1.0f);
            }
            cameraPtr->AddYaw(glm::pi<float>());
            cameraPtr->SetProjection(Project001::Camera::CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC);
            cameraPtr->SetCameraMask(s_mainCameraMask_);
        }
    }

    // UI Camera Entity

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
                float uiCameraHalfHeight = 3.5f;
                float uiCameraHalfWidth = aspectRatio * uiCameraHalfHeight;
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

    // Member Scenes -----------------------------------------------------------

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "TODO:\n"
        "Press <Esc> to return to Main Menu.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = font01_FontDataPtr_;
    instructionSceneInfo.fontTextureId = font01_TextureId_;
    instructionSceneInfo.cameraEntityId = uiCameraEntityId_;
    instructionSceneInfo.cameraMask = s_uiCameraMask_;
    instructionSceneInfo.keyCode_toggleInstructions = s_keyCode_toggleInstructions_;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene101::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene101:            " << GetId());

    // -------------------------------------------------------------------------

    GetRendererPtr()->DeleteAllTextures();
    GetRendererPtr()->DeleteAllMeshes();
    GetComponentStoresPtr()->DeleteAllEntities();

    // FontUtils Data ---------------------------------------------------------------

    delete font01_FontDataPtr_;
    font01_FontDataPtr_ = nullptr;
    delete font01_TextureDataPtr_;
    font01_TextureDataPtr_ = nullptr;
    font01_TextureId_ = static_cast<unsigned int>(-1);

    // Entity Ids --------------------------------------------------------------

    mainCameraEntityId_ = static_cast<unsigned int>(-1);
    uiCameraEntityId_ = static_cast<unsigned int>(-1);
}

void TestScene101::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
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
}

void TestScene101::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    GetRenderSystemPtr()->Render(GetComponentStoresPtr(), GetRendererPtr());
}

void TestScene101::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    
}