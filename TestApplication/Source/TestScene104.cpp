// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-11-10

#include "TestScene104.h"

#include "TestApplicationData.h"
#include "TestResource_AntonioRegular_png.h"
#include "TestResource_AntonioRegular_ssf.h"

#include "Components/Camera.h"
#include "Components/RenderedModel.h"
#include "Math/MathUtilities.h"
#include "ComponentStores.h"
#include "FontLoader.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "RenderSystem.h"
#include "TextureLoader.h"
#include "Window.h"



// public ----------------------------------------------------------------------

TestScene104::TestScene104(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , instructionScene_(applicationPtr)
    , mainCamera_EntityId_((unsigned int)-1)
{
    GetSharedDataPtr<TestApplicationData>()->testScene104Id = GetId();
}

TestScene104::~TestScene104()
{}

void TestScene104::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene104::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene104::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene104::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene104::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene104::ProcessUpdateEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene104::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene104:            " << GetId());

    // -------------------------------------------------------------------------

    CreateMainCameraEntity();

    InitializeInstructionScene();
}

void TestScene104::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene104:            " << GetId());

    // -------------------------------------------------------------------------

    GetRendererPtr()->DeleteAllTextures();
    GetRendererPtr()->DeleteAllMeshes();
    GetComponentStoresPtr()->DeleteAllEntities();

    // Entities ----------------------------------------------------------------

    mainCamera_EntityId_ = (unsigned int)-1;
}

void TestScene104::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
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

void TestScene104::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    Project001::RenderSystem::Render(GetComponentStoresPtr(), GetRendererPtr());
}

void TestScene104::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    
}

void TestScene104::InitializeInstructionScene()
{
    Project001::FontData font01_FontData;
    FAIL_CHECK(Project001::FontLoader::LoadFontDataFromMemory(
        font01_FontData,
        g_AntonioRegular_ssf,
        sizeof(g_AntonioRegular_ssf)
    ));

    Project001::TextureData font01_TextureData;
    FAIL_CHECK(Project001::TextureLoader::LoadTextureFromMemory(
        font01_TextureData,
        g_AntonioRegular_png,
        sizeof(g_AntonioRegular_png)
    ));
    unsigned int font01_TextureId = (unsigned int)-1;
    GetRendererPtr()->CreateTexture(
        font01_TextureId,
        font01_TextureData.data,
        font01_TextureData.width,
        font01_TextureData.height,
        font01_TextureData.bytesPerPixel,
        true,
        false
    );

    const uint32_t uiCamera_Mask = 0b01000000000000000000000000000000;

    unsigned int uiCamera_EntityId = (unsigned int)-1;
    GetComponentStoresPtr()->CreateEntity(uiCamera_EntityId);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::Camera>(uiCamera_EntityId));
    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, uiCamera_EntityId));
    if (cameraPtr != nullptr)
    {
        int aspectRatioNumerator;
        int aspectRatioDenominator;
        GetWindowPtr()->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
        if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
        {
            float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;
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
        cameraPtr->SetCameraMask(uiCamera_Mask);
        cameraPtr->SetPriorityValue(1000000);
    }

    const Project001::KeyCode keyCode_toggleInstructions = Project001::KeyCode::KEY_CODE_TAB;

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "TODO:\n"
        "Press <Esc> to return to Main Menu.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = &font01_FontData;
    instructionSceneInfo.fontTextureId = font01_TextureId;
    instructionSceneInfo.cameraEntityId = uiCamera_EntityId;
    instructionSceneInfo.cameraMask = uiCamera_Mask;
    instructionSceneInfo.keyCode_toggleInstructions = keyCode_toggleInstructions;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene104::CreateMainCameraEntity()
{
    GetComponentStoresPtr()->CreateEntity(mainCamera_EntityId_);
    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::Camera>(mainCamera_EntityId_));

    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCamera_EntityId_));
    if (cameraPtr != nullptr)
    {
        int aspectRatioNumerator;
        int aspectRatioDenominator;
        GetWindowPtr()->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
        if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
        {
            float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;
            float uiCameraHalfHeight = 320.0f;
            float uiCameraHalfWidth = aspectRatio * uiCameraHalfHeight;
            cameraPtr->SetAspectRatio(aspectRatio);
            cameraPtr->SetTopCutoff(uiCameraHalfHeight);
            cameraPtr->SetBottomCutoff(-uiCameraHalfHeight);
            cameraPtr->SetLeftCutoff(-uiCameraHalfWidth);
            cameraPtr->SetRightCutoff(uiCameraHalfWidth);
            cameraPtr->SetNearCutoff(-32.0f);
            cameraPtr->SetFarCutoff(32.0f);
        }
        cameraPtr->AddYaw(glm::pi<float>());
        cameraPtr->SetProjection(Project001::Camera::CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC);
        cameraPtr->SetDepthTestEnabled(false);
        cameraPtr->SetCameraMask(s_mainCamera_Mask_);
    }
}