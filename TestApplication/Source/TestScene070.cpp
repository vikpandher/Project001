// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-19

#include "TestScene070.h"

#include "TestApplicationData.h"
#include "TestResource_AntonioRegular_png.h"
#include "TestResource_AntonioRegular_ssf.h"

#include "Components/Camera.h"
#include "Components/RenderedModel.h"
#include "Resources/PixelFont5x6.h"
#include "Utilities/FontUtility.h"
#include "Utilities/IniReaderWriter.h"
#include "Utilities/MathUtility.h"
#include "Utilities/MeshUtility.h"
#include "Utilities/TextureUtility.h"
#include "ComponentStores.h"
#include "Logger.h"
#include "RenderSystem.h"
#include "Window.h"

#include <sstream>



// public ----------------------------------------------------------------------

TestScene070::TestScene070(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , instructionScene_(applicationPtr)
    , font01_FontDataPtr_(nullptr)
    , font01_TextureDataPtr_(nullptr)
    , font01_TextureId_(static_cast<unsigned int>(-1))
    , mainCamera_EntityId_(static_cast<unsigned int>(-1))
    , uiCamera_EntityId_(static_cast<unsigned int>(-1))
    , text01_EntityId_(static_cast<unsigned int>(-1))
    , pixel_TextureId_(static_cast<unsigned int>(-1))
    , text01_MeshDataPtr_(nullptr)
{
    GetSharedDataPtr<TestApplicationData>()->testScene070Id = GetId();
}

TestScene070::~TestScene070()
{}

void TestScene070::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene070::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene070::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene070::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene070::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene070::ProcessUpdateEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene070::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene070:            " << GetId());

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
        GetComponentStoresPtr()->CreateEntity(uiCamera_EntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::Camera>(uiCamera_EntityId_));
        Project001::Camera* cameraPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, uiCamera_EntityId_));
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

    CreateText01();

    // Member Scenes -----------------------------------------------------------

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "This Scene tests the \".ini\" reader and writer.\n"
        "Use <WASD> to move the camera up, left, down, and right.\n"
        "Press <Esc> to return to Main Menu.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = font01_FontDataPtr_;
    instructionSceneInfo.fontTextureId = font01_TextureId_;
    instructionSceneInfo.cameraEntityId = uiCamera_EntityId_;
    instructionSceneInfo.cameraMask = s_uiCameraMask_;
    instructionSceneInfo.keyCode_toggleInstructions = s_keyCode_toggleInstructions_;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene070::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene070:            " << GetId());

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

    mainCamera_EntityId_ = static_cast<unsigned int>(-1);
    uiCamera_EntityId_ = static_cast<unsigned int>(-1);

    text01_EntityId_ = static_cast<unsigned int>(-1);
    pixel_TextureId_ = static_cast<unsigned int>(-1);
    delete text01_MeshDataPtr_;
    text01_MeshDataPtr_ = nullptr;
}

void TestScene070::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
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

void TestScene070::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    Project001::RenderSystem::Render(GetComponentStoresPtr(), GetRendererPtr());
}

void TestScene070::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    unsigned long long timestep_ns = updateEvent.timestep_ns;
    float timestep_s = static_cast<float>(timestep_ns) / 1e9f;

    UpdateMainCameraEntityPosition(timestep_s);
}

void TestScene070::CreateText01()
{
    // Example ini string
    std::string inputString = R"(
    # A comment
    ; Another comment
    This line is ignored.

    key1 = 10
    key2 = 0.4f
    key3 = =

    [Section1]

    key4=value
    k e y 5 = v a l u e
    key6 = "value"

    [ Section 2 ]


    key7 = "abc#def"
    key8 = value # comment
    key9 = "value # not comment"

    [Section_3] # comment
    key10 = "quoted \"value\""
    key11 = "\"quoted\""
    key12 = \\value\\
    key13 = "\\value\\"
    key14 = "abc\zdef"

    [Section_4] ; comment
    key15 = "dangling\""
    key16 = "dangling\"
    key17 = "dangling
    key18 = "dangling # comment

    [Section_5]
    )";

    // Use string stream to simulate file input
    std::istringstream inputStream(inputString);
    std::map<std::string, std::map<std::string, std::string>> sections;

    // Read_H the INI data
    Project001::ReadIniStream(sections, inputStream);

    // Simulate output stream (writing the ini back out)
    std::ostringstream outputStream;
    Project001::WriteIniStream(outputStream, sections);

    std::string outputString = outputStream.str();

    const Project001::FontData& pixel_FontData = Project001::Get_PixelFont5x6_FontData();
    const Project001::TextureData& pixel_TextureData = Project001::Get_PixelFont5x6_TextureData();
    GetRendererPtr()->CreateTexture(
        pixel_TextureId_,
        pixel_TextureData.data,
        pixel_TextureData.width,
        pixel_TextureData.height,
        pixel_TextureData.bytesPerPixel,
        false,
        false
    );

    text01_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *text01_MeshDataPtr_,
        pixel_FontData,
        outputString,
        text01_pixelSize
    ));

    GetComponentStoresPtr()->CreateEntity(text01_EntityId_);
    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(text01_EntityId_));
    Project001::RenderedMesh* renderedMeshPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, text01_EntityId_));
    if (renderedMeshPtr != nullptr)
    {
        renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
        renderedMeshPtr->SetMeshDataPtr(text01_MeshDataPtr_);
        renderedMeshPtr->SetTextureId(pixel_TextureId_);
        renderedMeshPtr->SetPosition(-2.0f, 2.0f, 0.0f);
        renderedMeshPtr->SetUseLighting(false);
    }
}

void TestScene070::UpdateMainCameraEntityPosition(float timestep_s)
{
    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCamera_EntityId_));
    if (cameraPtr != nullptr)
    {
        constexpr float moveSpeed = 0.5f;
        float cameraMoveDelta = moveSpeed * timestep_s;

        const bool movingLeft = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_A);
        const bool movingRight = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_D);
        const bool movingUp = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_W);
        const bool movingDown = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_S);

        if (movingLeft)
        {
            cameraPtr->MoveLeft(cameraMoveDelta);
        }

        if (movingRight)
        {
            cameraPtr->MoveRight(cameraMoveDelta);
        }

        if (movingUp)
        {
            cameraPtr->MoveUp(cameraMoveDelta);
        }

        if (movingDown)
        {
            cameraPtr->MoveDown(cameraMoveDelta);
        }
    }
}