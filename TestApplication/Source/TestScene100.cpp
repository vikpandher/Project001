#include "TestScene100.h"

#include "TestSceneIds.h"

#include "Components/Camera.h"
#include "Components/RenderedModel.h"
#include "Math/MathUtilities.h"
#include "Math/Overlap2D.h"
#include "ComponentStores.h"
#include "FreetypeTextLoader.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "RenderSystem.h"
#include "TimeProfiler.h"
#include "Window.h"



// public ----------------------------------------------------------------------

TestScene100::TestScene100(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , instructionScene_(applicationPtr)
    , windowPtr_(nullptr)
    , rendererPtr_(nullptr)
    , componentStoresPtr_(nullptr)
    , font01_FontDataPtr_(nullptr)
    , font01_TextureDataPtr_(nullptr)
    , font01_TextureId_((unsigned int)-1)
    , mainCameraEntityId_((unsigned int)-1)
    , uiCameraEntityId_((unsigned int)-1)
{}
   
TestScene100::~TestScene100()
{}

void TestScene100::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene100::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene100::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene100::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene100::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene100::ProcessUpdateEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene100::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    _LOG_MESSAGE("INITIALIZING:   TestScene100:            %u", GetId());

    windowPtr_ = GetApplicationWindowPtr();
    rendererPtr_ = GetApplicationRendererPtr();
    componentStoresPtr_ = GetApplicaitonComponentStoresPtr();

    // Font Data ---------------------------------------------------------------

    {
        font01_FontDataPtr_ = new Project001::FontData;
        font01_TextureDataPtr_ = new Project001::TextureData;
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
            true,
            false
        );
    }

    // Entity Ids --------------------------------------------------------------

    // Main Camera Entity

    float mainCameraHalfHeight = 0.0f;
    float mainCameraHalfWidth = 0.0f;
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
            cameraPtr->TurnOn();
        }
    }

    // UI Camera Entity

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
            cameraPtr->TurnOn();
            cameraPtr->SetCameraMask(s_uiCameraMask_);
            cameraPtr->SetPriorityValue(1000000);
        }
    }

    // Member Scenes -----------------------------------------------------------

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "This Scene tests a joystick. It'll output the\n"
        "first joystick's axis and button values while\n"
        "the joystick is plugged in.\n"
        "Press <Esc> to return to Main Menu.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = font01_FontDataPtr_;
    instructionSceneInfo.fontTextureIdPtr = &font01_TextureId_;
    instructionSceneInfo.cameraEntityIdPtr = &uiCameraEntityId_;
    instructionSceneInfo.cameraMaskPtr = &s_uiCameraMask_;
    instructionSceneInfo.keyCode_toggleInstructionsPtr = &s_keyCode_toggleInstructions_;
    instructionScene_.Initialize(instructionSceneInfo);

    // -------------------------------------------------------------------------

    Test001();
}

void TestScene100::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    _LOG_MESSAGE("DEINITIALIZING: TestScene100:            %u", GetId());

    // -------------------------------------------------------------------------

    rendererPtr_->DeleteAllTextures();
    rendererPtr_->DeleteAllMeshes();
    componentStoresPtr_->DeleteAllEntities();

    windowPtr_ = nullptr;
    rendererPtr_ = nullptr;
    componentStoresPtr_ = nullptr;

    // Font Data ---------------------------------------------------------------

    delete font01_FontDataPtr_;
    font01_FontDataPtr_ = nullptr;
    delete font01_TextureDataPtr_;
    font01_TextureDataPtr_ = nullptr;
    font01_TextureId_ = (unsigned int)-1;

    // Entity Ids --------------------------------------------------------------

    mainCameraEntityId_ = (unsigned int)-1;
    uiCameraEntityId_ = (unsigned int)-1;
}

void TestScene100::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
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
    }
}

void TestScene100::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    Project001::RenderSystem::Render(componentStoresPtr_, rendererPtr_);
}

void TestScene100::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{

}

void TestScene100::Test001()
{
    /*
    if (Project001::TimeProfiler::BeginSession())
    {
        _LOG_MESSAGE("RECORDING START");
    }

    Project001::Timer timer;

    std::vector<glm::vec2> convexPolygonA_corners;
    convexPolygonA_corners.emplace_back(0.0f, 0.0f);
    convexPolygonA_corners.emplace_back(1.0f, 0.0f);
    convexPolygonA_corners.emplace_back(1.0f, 1.0f);
    convexPolygonA_corners.emplace_back(0.5f, 1.5f);
    convexPolygonA_corners.emplace_back(0.0f, 1.0f);

    std::vector<glm::vec2> convexPolygonB_corners;
    convexPolygonB_corners.emplace_back(0.5f, 0.5f);
    convexPolygonB_corners.emplace_back(1.5f, 0.5f);
    convexPolygonB_corners.emplace_back(1.5f, 1.5f);
    convexPolygonB_corners.emplace_back(0.5f, 1.5f);

    timer.Start("Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth");

    for (size_t i = 0; i < 100000; ++i)
    {
        glm::vec2 collisionPoint;
        glm::vec2 collisionNormal;
        float collisionDepth;
        bool result = Project001::Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth(
            convexPolygonA_corners.data(),
            convexPolygonA_corners.size(),
            convexPolygonB_corners.data(),
            convexPolygonB_corners.size(),
            collisionPoint,
            collisionNormal,
            collisionDepth);
    }

    timer.Stop();

    /*

    timer.Start("Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth_2");

    for (size_t i = 0; i < 100000; ++i)
    {
        glm::vec2 collisionPoint;
        glm::vec2 collisionNormal;
        float collisionDepth;
        bool result = Project001::Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth_2(
            convexPolygonA_corners.data(),
            convexPolygonA_corners.size(),
            convexPolygonB_corners.data(),
            convexPolygonB_corners.size(),
            collisionPoint,
            collisionNormal,
            collisionDepth);
    }

    timer.Stop();

    if (Project001::TimeProfiler::EndSession())
    {
        _LOG_MESSAGE("RECORDING END");
    }
    */
}