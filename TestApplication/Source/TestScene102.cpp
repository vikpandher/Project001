// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-20

#include "TestScene102.h"

#include "TestApplicationData.h"
#include "TestResource_AntonioRegular_png.h"
#include "TestResource_AntonioRegular_ssf.h"

#include "Components/Camera.h"
#include "Components/CollisionBody2D.h"
#include "Components/RenderedModel.h"
#include "Resources/PixelFont5x6.h"
#include "Utilities/FontUtility.h"
#include "Utilities/MeshUtility.h"
#include "Utilities/SoundUtility.h"
#include "Utilities/TextureUtility.h"
#include "CollisionSystem2D.h"
#include "ComponentStores.h"
#include "Logger.h"
#include "RenderSystem.h"
#include "SoundPlayer.h"
#include "Window.h"



// public ----------------------------------------------------------------------

TestScene102::TestScene102(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , instructionScene_(applicationPtr)
    , font01_FontDataPtr_(nullptr)
    , font01_TextureDataPtr_(nullptr)
    , font01_TextureId_(static_cast<unsigned int>(-1))
    , pixelFont_FontDataPtr_(nullptr)
    , pixelFont_TextureDataPtr_(nullptr)
    , pixelFont_TextureId_(static_cast<unsigned int>(-1))
    , cursorCircle_MeshDataPtr_(nullptr)
    , floorGrid_MeshDataPtr_(nullptr)
    , floorGrid_MeshId_(static_cast<unsigned int>(-1))
    , floorGridLabels_MeshDataPtr_(nullptr)
    , floorGridLabels_MeshId_(static_cast<unsigned int>(-1))
    , mainCameraNearFrustum_MeshDataPtr_(nullptr)
    , mainCameraFarFrustum_MeshDataPtr_(nullptr)
    , ship_MeshDataPtr_(nullptr)
    , shipBeamSight_MeshDataPtr_(nullptr)
    , shipCollisionBody_MeshDataPtr_(nullptr)
    , border96x64_TextureId_(static_cast<unsigned int>(-1))
    , numbers16x4_TextureId_(static_cast<unsigned int>(-1))
    , mainCamera_EntityId_(static_cast<unsigned int>(-1))
    , uiCamera_EntityId_(static_cast<unsigned int>(-1))
    , cursor_EntityId_(static_cast<unsigned int>(-1))
    , cursorPositionRenderedMeshIndex_(static_cast<unsigned int>(-1))
    , cursorPressRenderedMeshIndex_(static_cast<unsigned int>(-1))
    , cursorReleaseRenderedMeshIndex_(static_cast<unsigned int>(-1))
    , cursorPositionCollisionPointIndex_(static_cast<unsigned int>(-1))
    , cursorPressCollisionPointIndex_(static_cast<unsigned int>(-1))
    , cursorReleaseCollisionPointIndex_(static_cast<unsigned int>(-1))
    , floor_EntityId_(static_cast<unsigned int>(-1))
    , player_EntityId_(static_cast<unsigned int>(-1))
    , cameraDistanceFromPlayer_()
    , maxCollisionBodyVelocity_()
    , physicsStepsPerUpdate_(1)
{
    GetSharedDataPtr<TestApplicationData>()->testScene102Id = GetId();
}

TestScene102::~TestScene102()
{}

void TestScene102::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene102::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene102::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestScene102::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene102::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestScene102::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene102::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene102::ProcessUpdateEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene102::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene102:            " << GetId());

    LoadFontData();

    LoadMeshData();

    LoadTextureData();

    // Entities ----------------------------------------------------------------

    CreateCameraEntities();

    CreateCursorEntity();

    CreateFloorEntity();

    CreatePlayerEntity();

    cameraDistanceFromPlayer_ = 800.0f;
    maxCollisionBodyVelocity_ = 5120.0f;
    physicsStepsPerUpdate_ = 1;

    // Member Scenes -----------------------------------------------------------

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "This Scene tests cursor aim and movemnt.\n"
        "Use <WASD> to move player.\n"
        "Use the <Mouse> to rotate player.\n"
        "Use <Arrow Up> & <Arrow Down> to tilt camera.\n"
        "Press <P> to change the camera projection type.\n"
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

void TestScene102::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene102:            " << GetId());

    // -------------------------------------------------------------------------

    GetRendererPtr()->DeleteAllTextures();
    GetRendererPtr()->DeleteAllMeshes();
    GetSoundPlayerPtr()->DeleteAllSoundSources();
    GetSoundPlayerPtr()->DeleteAllSoundBuffers();
    GetComponentStoresPtr()->DeleteAllEntities();

    // FontUtils Data ---------------------------------------------------------------

    delete font01_FontDataPtr_;
    font01_FontDataPtr_ = nullptr;
    delete font01_TextureDataPtr_;
    font01_TextureDataPtr_ = nullptr;
    font01_TextureId_ = static_cast<unsigned int>(-1);

    pixelFont_FontDataPtr_ = nullptr;
    pixelFont_TextureDataPtr_ = nullptr;
    pixelFont_TextureId_ = static_cast<unsigned int>(-1);

    // Mesh Data ---------------------------------------------------------------

    delete cursorCircle_MeshDataPtr_;
    cursorCircle_MeshDataPtr_ = nullptr;

    delete floorGrid_MeshDataPtr_;
    floorGrid_MeshDataPtr_ = nullptr;
    floorGrid_MeshId_ = static_cast<unsigned int>(-1);

    delete floorGridLabels_MeshDataPtr_;
    floorGridLabels_MeshDataPtr_ = nullptr;
    floorGridLabels_MeshId_ = static_cast<unsigned int>(-1);

    delete mainCameraNearFrustum_MeshDataPtr_;
    mainCameraNearFrustum_MeshDataPtr_ = nullptr;

    delete mainCameraFarFrustum_MeshDataPtr_;
    mainCameraFarFrustum_MeshDataPtr_ = nullptr;

    delete ship_MeshDataPtr_;
    ship_MeshDataPtr_ = nullptr;

    delete shipBeamSight_MeshDataPtr_;
    shipBeamSight_MeshDataPtr_ = nullptr;

    delete shipCollisionBody_MeshDataPtr_;
    shipCollisionBody_MeshDataPtr_ = nullptr;

    // Texture Data ------------------------------------------------------------

    border96x64_TextureId_ = static_cast<unsigned int>(-1);
    numbers16x4_TextureId_ = static_cast<unsigned int>(-1);

    // Entities ----------------------------------------------------------------

    mainCamera_EntityId_ = static_cast<unsigned int>(-1);
    uiCamera_EntityId_ = static_cast<unsigned int>(-1);

    cursor_EntityId_ = static_cast<unsigned int>(-1);
    cursorPositionRenderedMeshIndex_ = static_cast<unsigned int>(-1);
    cursorPressRenderedMeshIndex_ = static_cast<unsigned int>(-1);
    cursorReleaseRenderedMeshIndex_ = static_cast<unsigned int>(-1);
    cursorPositionCollisionPointIndex_ = static_cast<unsigned int>(-1);
    cursorPressCollisionPointIndex_ = static_cast<unsigned int>(-1);
    cursorReleaseCollisionPointIndex_ = static_cast<unsigned int>(-1);

    floor_EntityId_ = static_cast<unsigned int>(-1);

    player_EntityId_ = static_cast<unsigned int>(-1);

    // -------------------------------------------------------------------------

    cameraDistanceFromPlayer_ = 0.0f;
    maxCollisionBodyVelocity_ = 0.0f;
    physicsStepsPerUpdate_ = 1;
}

void TestScene102::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent)
{
    UpdateCursorPosition(cursorPositionEvent.xPosition, cursorPositionEvent.yPosition);

    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
        collisionPoints[cursorPositionCollisionPointIndex_].enabled = true;
    }
}

void TestScene102::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
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
        else if (keyCode == Project001::KeyCode::KEY_CODE_P)
        {
            Project001::Camera* cameraPtr = nullptr;
            if (GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCamera_EntityId_))
            {
                if (cameraPtr->GetProjection() == Project001::Camera::CameraProjection::CAMERA_PROJECTION_PERSPECTIVE)
                {
                    cameraPtr->SetProjection(Project001::Camera::CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC);
                }
                else
                {
                    cameraPtr->SetProjection(Project001::Camera::CameraProjection::CAMERA_PROJECTION_PERSPECTIVE);
                }
            }
        }
    }
}

void TestScene102::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;

    if (mouseButton == Project001::MouseButton::MOUSE_BUTTON_1)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS)
        {
            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
            if (collisionBody2DPtr != nullptr)
            {
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
                collisionPoints[cursorPressCollisionPointIndex_].position = collisionPoints[cursorPositionCollisionPointIndex_].position;
                collisionPoints[cursorPressCollisionPointIndex_].enabled = true;
                collisionPoints[cursorReleaseCollisionPointIndex_].enabled = false;
            }
        }
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
        {
            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
            if (collisionBody2DPtr != nullptr)
            {
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
                collisionPoints[cursorReleaseCollisionPointIndex_].position = collisionPoints[cursorPositionCollisionPointIndex_].position;
                collisionPoints[cursorReleaseCollisionPointIndex_].enabled = true;
            }
        }
    }
}

void TestScene102::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    GetRenderSystemPtr()->Render(GetComponentStoresPtr(), GetRendererPtr());
}

void TestScene102::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    unsigned long long timestep_ns = updateEvent.timestep_ns;

    // Move and Calculate collisions
    // -------------------------------------------------------------------------
    float timestep_s = static_cast<float>(timestep_ns) / 1e9f;
    float physicsTimestep_s = timestep_s / static_cast<float>(physicsStepsPerUpdate_);
    for (size_t i = 0; i < physicsStepsPerUpdate_; ++i)
    {
        GetCollisionSystemPtr()->ApplyMovement(GetComponentStoresPtr(), physicsTimestep_s);
        GetCollisionSystemPtr()->CalculateCollisions(GetComponentStoresPtr());

        UpdatePlayerEntityVelocity(physicsTimestep_s);
        UpdateMainCameraEntityPosition(physicsTimestep_s);

        // Update cursor because camera updates
        // -------------------------------------------------------------------------
        float cursorX_position;
        float cursorY_position;
        GetWindowPtr()->GetCursorPosition(cursorX_position, cursorY_position);
        UpdateCursorPosition(cursorX_position, cursorY_position);
    }

    // Sync rendered models
    // -------------------------------------------------------------------------
    SyncMainCameraRenderedModel();
    SyncCursorRenderedModel();
    SyncPlayerRenderedModel();
}

void TestScene102::LoadFontData()
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

    pixelFont_FontDataPtr_ = &Project001::Get_PixelFont5x6_FontData();
    pixelFont_TextureDataPtr_ = &Project001::Get_PixelFont5x6_TextureData();
    GetRendererPtr()->CreateTexture(
        pixelFont_TextureId_,
        pixelFont_TextureDataPtr_->data,
        pixelFont_TextureDataPtr_->width,
        pixelFont_TextureDataPtr_->height,
        pixelFont_TextureDataPtr_->bytesPerPixel,
        false,
        false
    );
}

void TestScene102::LoadMeshData()
{
    cursorCircle_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::Generate2DRegularPolygon(*cursorCircle_MeshDataPtr_, 4.0f, 12));

    {
        floorGrid_MeshDataPtr_ = new Project001::MeshData();
        const float gridSpacing = 64.0f;
        const float gridSize = 16.0f * gridSpacing;
        const float lineWidth = 8.0f;

        for (float f = -gridSize; f < gridSize + 1.0f; f += gridSpacing)
        {
            FAIL_CHECK(Project001::Mesh::Generate2DLine(
                *floorGrid_MeshDataPtr_,
                glm::vec2(-gridSize - 0.5f * lineWidth, f),
                glm::vec2(gridSize + 0.5f * lineWidth, f),
                lineWidth));
        }
        for (float f = -gridSize; f < gridSize + 1.0f; f += gridSpacing)
        {
            for (float g = -gridSize; g < gridSize - 1.0f; g += gridSpacing)
            {
                FAIL_CHECK(Project001::Mesh::Generate2DLine(
                    *floorGrid_MeshDataPtr_,
                    glm::vec2(f, g + 0.5f * lineWidth),
                    glm::vec2(f, g - 0.5f * lineWidth + gridSpacing),
                    lineWidth));
            }
        }

        GetRendererPtr()->CreateMesh(
            floorGrid_MeshId_,
            floorGrid_MeshDataPtr_->meshVertexArray.data(),
            static_cast<unsigned int>(floorGrid_MeshDataPtr_->meshVertexArray.size()),
            floorGrid_MeshDataPtr_->meshIndexArray.data(),
            static_cast<unsigned int>(floorGrid_MeshDataPtr_->meshIndexArray.size())
        );

        floorGridLabels_MeshDataPtr_ = new Project001::MeshData();

        std::vector<std::string> gridLabels;
        gridLabels.push_back("0");
        gridLabels.push_back("1");
        gridLabels.push_back("2");
        gridLabels.push_back("3");
        gridLabels.push_back("4");
        gridLabels.push_back("5");
        gridLabels.push_back("6");
        gridLabels.push_back("7");
        gridLabels.push_back("8");
        gridLabels.push_back("9");
        gridLabels.push_back("10");
        gridLabels.push_back("11");
        gridLabels.push_back("12");
        gridLabels.push_back("13");
        gridLabels.push_back("14");
        gridLabels.push_back("15");

        // positive x-axis -----------------------------------------------------

        float gridLabel_offsetY = pixelFont_pixelSize_ * 1.0f + 0.5f * lineWidth;
        float gridLabel_offsetX = pixelFont_pixelSize_ * 1.0f + 0.5f * lineWidth;

        for (size_t i = 0; i < gridLabels.size(); ++i)
        {
            const std::string& currentLabel = gridLabels[i];

            Project001::MeshData currentLabelMeshData;
            FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
                currentLabelMeshData,
                *pixelFont_FontDataPtr_,
                currentLabel,
                pixelFont_pixelSize_
            ));
            Project001::Mesh::TranslateMesh(
                currentLabelMeshData,
                glm::vec3(gridLabel_offsetX, gridLabel_offsetY, 0.0f)
            );

            Project001::Mesh::CopyMesh(*floorGridLabels_MeshDataPtr_, currentLabelMeshData);

            gridLabel_offsetX += gridSpacing;
        }

        // positive y-axis -----------------------------------------------------

        gridLabel_offsetY = pixelFont_pixelSize_ * 1.0f + 0.5f * lineWidth + gridSpacing;
        gridLabel_offsetX = pixelFont_pixelSize_ * 1.0f + 0.5f * lineWidth;

        for (size_t i = 1; i < gridLabels.size(); ++i)
        {
            const std::string& currentLabel = gridLabels[i];

            Project001::MeshData currentLabelMeshData;
            FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
                currentLabelMeshData,
                *pixelFont_FontDataPtr_,
                currentLabel,
                pixelFont_pixelSize_
            ));
            Project001::Mesh::TranslateMesh(
                currentLabelMeshData,
                glm::vec3(gridLabel_offsetX, gridLabel_offsetY, 0.0f)
            );

            Project001::Mesh::CopyMesh(*floorGridLabels_MeshDataPtr_, currentLabelMeshData);

            gridLabel_offsetY += gridSpacing;
        }

        // negative x-axis -----------------------------------------------------

        gridLabel_offsetY = pixelFont_pixelSize_ * -7.0f - 0.5f * lineWidth;
        gridLabel_offsetX = pixelFont_pixelSize_ * -6.0f - 0.5f * lineWidth;

        for (size_t i = 0; i < gridLabels.size(); ++i)
        {
            const std::string& currentLabel = gridLabels[i];

            float addition_offsetX = pixelFont_pixelSize_ * -6.0f * static_cast<float>(currentLabel.length() - 1);

            Project001::MeshData currentLabelMeshData;
            FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
                currentLabelMeshData,
                *pixelFont_FontDataPtr_,
                currentLabel,
                pixelFont_pixelSize_
            ));
            Project001::Mesh::TranslateMesh(
                currentLabelMeshData,
                glm::vec3(gridLabel_offsetX + addition_offsetX , gridLabel_offsetY, 0.0f)
            );

            Project001::Mesh::CopyMesh(*floorGridLabels_MeshDataPtr_, currentLabelMeshData);

            gridLabel_offsetX -= gridSpacing;
        }

        // negative y-axis -----------------------------------------------------

        gridLabel_offsetY = pixelFont_pixelSize_ * -7.0f - 0.5f * lineWidth - gridSpacing;
        gridLabel_offsetX = pixelFont_pixelSize_ * -6.0f - 0.5f * lineWidth;

        for (size_t i = 1; i < gridLabels.size(); ++i)
        {
            const std::string& currentLabel = gridLabels[i];

            float addition_offsetX = pixelFont_pixelSize_ * -6.0f * static_cast<float>(currentLabel.length() - 1);

            Project001::MeshData currentLabelMeshData;
            FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
                currentLabelMeshData,
                *pixelFont_FontDataPtr_,
                currentLabel,
                pixelFont_pixelSize_
            ));
            Project001::Mesh::TranslateMesh(
                currentLabelMeshData,
                glm::vec3(gridLabel_offsetX + addition_offsetX, gridLabel_offsetY, 0.0f)
            );

            Project001::Mesh::CopyMesh(*floorGridLabels_MeshDataPtr_, currentLabelMeshData);

            gridLabel_offsetY -= gridSpacing;
        }

        GetRendererPtr()->CreateMesh(
            floorGridLabels_MeshId_,
            floorGridLabels_MeshDataPtr_->meshVertexArray.data(),
            static_cast<unsigned int>(floorGridLabels_MeshDataPtr_->meshVertexArray.size()),
            floorGridLabels_MeshDataPtr_->meshIndexArray.data(),
            static_cast<unsigned int>(floorGridLabels_MeshDataPtr_->meshIndexArray.size())
        );
    }

    mainCameraNearFrustum_MeshDataPtr_ = new Project001::MeshData();
    // Generated when Main Camera Entity created

    mainCameraFarFrustum_MeshDataPtr_ = new Project001::MeshData();
    // Generated when Main Camera Entity created

    ship_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::LoadMeshOBJ(*ship_MeshDataPtr_, "../Models/Ship.obj"));

    shipCollisionBody_MeshDataPtr_ = new Project001::MeshData();
    std::vector<glm::vec2> corners;
    corners.emplace_back(0.0f, 0.0f);
    corners.emplace_back(32.0f, -32.0f);
    corners.emplace_back(0.0f, 64.0f);
    corners.emplace_back(-32.0f, -32.0f);
    FAIL_CHECK(Project001::Mesh::Generate2DPolygon(*shipCollisionBody_MeshDataPtr_, corners));

    shipBeamSight_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::Generate2DLine(*shipBeamSight_MeshDataPtr_, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 10000.0f), 2.0f));
}

void TestScene102::LoadTextureData()
{
    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::Texture::LoadTexture(textureData, "../Textures/border_96x64.png"));
        GetRendererPtr()->CreateTexture(border96x64_TextureId_, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::Texture::LoadTexture(textureData, "../Textures/16_4_numbers.png"));
        GetRendererPtr()->CreateTexture(numbers16x4_TextureId_, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }
}

void TestScene102::CreateCameraEntities()
{
    // Main Camera -------------------------------------------------------------
    {
        float mainCameraHalfHeight = 0.0f;
        float mainCameraHalfWidth = 0.0f;

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
                mainCameraHalfHeight = 320.0f;
                mainCameraHalfWidth = aspectRatio * mainCameraHalfHeight;
                cameraPtr->SetAspectRatio(aspectRatio);
                cameraPtr->SetTopCutoff(mainCameraHalfHeight);
                cameraPtr->SetBottomCutoff(-mainCameraHalfHeight);
                cameraPtr->SetLeftCutoff(-mainCameraHalfWidth);
                cameraPtr->SetRightCutoff(mainCameraHalfWidth);
                cameraPtr->SetNearCutoff(mainCameraHalfHeight * 0.1f);
                cameraPtr->SetFarCutoff(mainCameraHalfHeight * 10.0f);
                // cameraPtr->SetCameraViewport(0.1f, 0.1f, 0.8f, 0.8f);
            }

            glm::vec3 corners[8] = {};
            cameraPtr->GetProjectionFrustumCorners(corners);
            FAIL_CHECK(Project001::Mesh::Generate2DSprite(
                *mainCameraNearFrustum_MeshDataPtr_,
                corners[1], corners[0], corners[3], corners[2],
                0.0f, 1.0f, 0.0f, 1.0f
            ));
            Project001::Mesh::TranslateMesh(*mainCameraNearFrustum_MeshDataPtr_, glm::vec3(0.0f, 0.0f, 0.001f));
            FAIL_CHECK(Project001::Mesh::Generate2DSprite(
                *mainCameraFarFrustum_MeshDataPtr_,
                corners[5], corners[4], corners[7], corners[6],
                0.0f, 1.0f, 0.0f, 1.0f
            ));
            Project001::Mesh::TranslateMesh(*mainCameraFarFrustum_MeshDataPtr_, glm::vec3(0.0f, 0.0f, -1.0f));

            cameraPtr->AddYaw(glm::pi<float>());
            cameraPtr->AddPitch(-glm::quarter_pi<float>());
            cameraPtr->SetProjection(Project001::Camera::CameraProjection::CAMERA_PROJECTION_PERSPECTIVE);
            cameraPtr->SetCameraMask(s_mainCameraMask_);

            GetSoundPlayerPtr()->SetListenerPosition(cameraPtr->GetPosition());
            GetSoundPlayerPtr()->SetListenerForwardDirection(cameraPtr->GetForwardVector());
            GetSoundPlayerPtr()->SetListenerUpDirection(cameraPtr->GetUpVector());
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(mainCamera_EntityId_));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, mainCamera_EntityId_));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetCameraMask(s_mainCameraMask_);
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh01 = renderedMeshes.back();
            mesh01.SetMeshDataPtr(mainCameraNearFrustum_MeshDataPtr_);
            mesh01.SetTextureId(border96x64_TextureId_);
            mesh01.SetColor(1.0f, 0.0f, 0.0f, 0.2f);
            mesh01.SetTranslucent(true);
            mesh01.SetUseLighting(false);
            mesh01.SetVisible(false);

            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh02 = renderedMeshes.back();
            mesh02.SetMeshDataPtr(mainCameraFarFrustum_MeshDataPtr_);
            mesh02.SetTextureId(border96x64_TextureId_);
            mesh02.SetColor(0.0f, 1.0f, 0.0f, 0.2f);
            mesh02.SetTranslucent(true);
            mesh02.SetUseLighting(false);
            mesh02.SetRenderPriorityOverride(-100);
        }
    }

    // UI Camera ---------------------------------------------------------------
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
}

void TestScene102::CreateCursorEntity()
{
    GetComponentStoresPtr()->CreateEntity(cursor_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(cursor_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursor_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraMask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        cursorPositionRenderedMeshIndex_ = renderedMeshes.size();
        renderedMeshes.emplace_back();
        Project001::RenderedMesh& circleMesh01 = renderedMeshes.back();
        circleMesh01.SetMeshDataPtr(cursorCircle_MeshDataPtr_);
        circleMesh01.SetPositionZ(0.53f);
        circleMesh01.SetColor(0.8f, 0.8f, 0.8f, 0.4f);
        circleMesh01.SetTranslucent(true);
        circleMesh01.SetUseLighting(false);

        cursorPressRenderedMeshIndex_ = renderedMeshes.size();
        renderedMeshes.emplace_back();
        Project001::RenderedMesh& circleMesh02 = renderedMeshes.back();
        circleMesh02.SetMeshDataPtr(cursorCircle_MeshDataPtr_);
        circleMesh02.SetPositionZ(0.52f);
        circleMesh02.SetColor(0.8f, 0.2f, 0.2f, 0.4f);
        circleMesh02.SetTranslucent(true);
        circleMesh02.SetUseLighting(false);
        circleMesh02.SetVisible(false);

        cursorReleaseRenderedMeshIndex_ = renderedMeshes.size();
        renderedMeshes.emplace_back();
        Project001::RenderedMesh& circleMesh03 = renderedMeshes.back();
        circleMesh03.SetMeshDataPtr(cursorCircle_MeshDataPtr_);
        circleMesh03.SetPositionZ(0.51f);
        circleMesh03.SetColor(0.8f, 0.8f, 0.2f, 0.4f);
        circleMesh03.SetTranslucent(true);
        circleMesh03.SetUseLighting(false);
        circleMesh03.SetVisible(false);
    }

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(cursor_EntityId_));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
        cursorPositionCollisionPointIndex_ = collisionPoints.size();
        collisionPoints.emplace_back(glm::vec2(), s_cursorPositionCollisionShapeId_, true);
        cursorPressCollisionPointIndex_ = collisionPoints.size();
        collisionPoints.emplace_back(glm::vec2(), s_cursorPressCollisionShapeId_, false);
        cursorReleaseCollisionPointIndex_ = collisionPoints.size();
        collisionPoints.emplace_back(glm::vec2(), s_cursorReleaseCollisionShapeId_, false);
    }

    float cursorX_position;
    float cursorY_position;
    GetWindowPtr()->GetCursorPosition(cursorX_position, cursorY_position);
    UpdateCursorPosition(cursorX_position, cursorY_position);
}

void TestScene102::CreateFloorEntity()
{
    GetComponentStoresPtr()->CreateEntity(floor_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(floor_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, floor_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraMask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        renderedMeshes.emplace_back();
        Project001::RenderedMesh& mesh01 = renderedMeshes.back();
        mesh01.SetMeshIdAndMaxBoundingRadius(floorGrid_MeshId_, floorGrid_MeshDataPtr_->maxBoundingRadius);
        mesh01.SetPositionZ(-0.1f);
        mesh01.SetColor(1.0f, 1.0f, 1.0f, 0.1f);
        mesh01.SetTranslucent(true);
        mesh01.SetUseLighting(false);

        renderedMeshes.emplace_back();
        Project001::RenderedMesh& mesh02 = renderedMeshes.back();
        mesh02.SetMeshIdAndMaxBoundingRadius(floorGridLabels_MeshId_, floorGridLabels_MeshDataPtr_->maxBoundingRadius);
        mesh02.SetPositionZ(-0.1f);
        mesh02.SetColor(1.0f, 1.0f, 1.0f, 0.1f);
        mesh02.SetTranslucent(true);
        mesh02.SetUseLighting(false);
        mesh02.SetTextureId(pixelFont_TextureId_);
    }
}

void TestScene102::CreatePlayerEntity()
{
    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.collisionGroupMask = s_mainCollisionGroupMask_;
    collisionBody2DCreationInfo.physicsType = Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY;
    collisionBody2DCreationInfo.friction = 1.0f;
    collisionBody2DCreationInfo.restitution = 0.4f;
    collisionBody2DCreationInfo.mass = 1.0f;

    GetComponentStoresPtr()->CreateEntity(player_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(player_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, player_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraMask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        renderedMeshes.emplace_back();
        Project001::RenderedMesh& mesh01 = renderedMeshes.back();
        mesh01.SetMeshDataPtr(ship_MeshDataPtr_);
        mesh01.LookAt(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        mesh01.SetScale(32.0f, 32.0f, 32.0f);
        mesh01.SetTextureId(numbers16x4_TextureId_);
        mesh01.SetUseLighting(false);

        // renderedMeshes.emplace_back();
        // Project001::RenderedMesh& mesh02 = renderedMeshes.back();
        // mesh02.SetMeshDataPtr(shipCollisionBody_MeshDataPtr_);
        // mesh02.SetLit(false);

        renderedMeshes.emplace_back();
        Project001::RenderedMesh& mesh03 = renderedMeshes.back();
        mesh03.SetMeshDataPtr(shipBeamSight_MeshDataPtr_);
        mesh03.SetColor(1.0f, 0.0f, 0.0f, 0.2f);
        mesh03.SetTranslucent(true);
        mesh03.SetUseLighting(false);
    }

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(player_EntityId_, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, player_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        // collisionBody2DPtr->SetVelocityDamping(10.0f);
        // collisionBody2DPtr->SetAngularVelocityDamping(10.0f);

        std::vector<Project001::CollisionTriangle2D>& collisionTriangles = collisionBody2DPtr->GetCollisionTriangles();
        collisionTriangles.emplace_back(glm::vec2(0.0f, 64.0f), glm::vec2(-32.0f, -32.0f), glm::vec2(0.0f, 0.0f));
        collisionTriangles.emplace_back(glm::vec2(0.0f, 0.0f), glm::vec2(32.0f, -32.0f), glm::vec2(0.0f, 64.0f));
    }
}

void TestScene102::UpdateMainCameraEntityPosition(float timestep_s)
{
    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCamera_EntityId_));
    if (cameraPtr != nullptr)
    {
        constexpr float orbitSpeed = 0.05f * glm::pi<float>();
        float cameraPitchDelta = orbitSpeed * timestep_s;

        bool pitchingUp = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_DOWN);
        bool pitchingDown = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_UP);

        if (pitchingUp)
        {
            cameraPtr->AddRelativeRotationX(-cameraPitchDelta);
        }

        if (pitchingDown)
        {
            cameraPtr->AddRelativeRotationX(cameraPitchDelta);
        }

        Project001::CollisionBody2D* collisionBodyPtr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, player_EntityId_));
        if (collisionBodyPtr != nullptr)
        {
            glm::vec3 playerPosition(collisionBodyPtr->GetPosition(), 0.0f);
            cameraPtr->FollowFocalPoint(playerPosition, cameraDistanceFromPlayer_);
        }
    }
}

void TestScene102::UpdatePlayerEntityVelocity(float timestep_s)
{
    Project001::CollisionBody2D* playerCollisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(playerCollisionBodyPtr, player_EntityId_));
    if (playerCollisionBodyPtr != nullptr)
    {
        constexpr float maxSpeed = 256.0f;
        constexpr float acceleration = 1024.0f;
        constexpr float friction = 768.0f;

        const bool movingLeft = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_A);
        const bool movingRight = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_D);
        const bool movingUp = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_W);
        const bool movingDown = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_S);

        const glm::vec2& velocity = playerCollisionBodyPtr->GetVelocity();
        float velocityMagnitude = glm::length(velocity);

        glm::vec2 accelerationDirection(0.0f, 0.0);
        if (movingLeft)
        {
            accelerationDirection.x -= 1.0f;
        }
        if (movingRight)
        {
            accelerationDirection.x += 1.0f;
        }
        if (movingUp)
        {
            accelerationDirection.y += 1.0f;
        }
        if (movingDown)
        {
            accelerationDirection.y -= 1.0f;
        }

        float accelerationMagnitude = glm::length(accelerationDirection);
        if (accelerationMagnitude > 0.0f)
        {
            accelerationDirection /= accelerationMagnitude;
        }

        if (accelerationMagnitude > 0.0f) // apply acceleration
        {
            glm::vec2 targetVelocity = accelerationDirection * maxSpeed;
            glm::vec2 neededVelocity = targetVelocity - velocity;

            float neededVelocityMagnitude = glm::length(neededVelocity);
            if (neededVelocityMagnitude > 0.0f)
            {
                glm::vec2 neededVelocityDirection = neededVelocity / neededVelocityMagnitude;
                glm::vec2 accelerationStep = neededVelocityDirection * acceleration * timestep_s;

                float accelerationStepMagnitude = glm::length(accelerationStep);
                if (accelerationStepMagnitude > neededVelocityMagnitude)
                {
                    accelerationStep = neededVelocity; // prevent acceleration from overshooting max speed
                }

                playerCollisionBodyPtr->SetVelocity(velocity + accelerationStep);
            }
        }
        else // apply deceleration
        {
            if (velocityMagnitude > 0.0f)
            {
                float decelerationStepMagnitude = friction * timestep_s;
                if (velocityMagnitude < decelerationStepMagnitude)
                {
                    playerCollisionBodyPtr->SetVelocity(glm::vec2(0.0f, 0.0f));
                }
                else
                {
                    glm::vec2 velocityDirection = velocity / velocityMagnitude;
                    playerCollisionBodyPtr->SetVelocity(velocity - velocityDirection * decelerationStepMagnitude);
                }
            }
        }

        Project001::CollisionBody2D* cursorCollisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(cursorCollisionBodyPtr, cursor_EntityId_));
        if (cursorCollisionBodyPtr != nullptr)
        {
            constexpr float maxAngularSpeed = glm::pi<float>() * 8.0f;
            constexpr float angularAcceleration = glm::pi<float>() * 12.0f;
            constexpr float angularFriction = glm::pi<float>() * 64.0f;

            std::vector<Project001::CollisionPoint2D>& collisionPoints = cursorCollisionBodyPtr->GetCollisionPoints();
            const glm::vec2& cursorPosition = collisionPoints[cursorPositionCollisionPointIndex_].position;
            glm::vec2 collisionBodyDirection = playerCollisionBodyPtr->GetForwardVector();
            glm::vec2 collisionBodyToCursor = cursorPosition - playerCollisionBodyPtr->GetPosition();

            const float& angularVelocity = playerCollisionBodyPtr->GetAngularVelocity();

            float angleToCursor = Project001::Math::Get2DVectorAngle(collisionBodyDirection, collisionBodyToCursor);
            if (glm::sign(angleToCursor) == glm::sign(angularVelocity) || glm::sign(angularVelocity) == 0.0f) // apply angular acceleration
            {
                float angularDifference = angleToCursor - angularVelocity * timestep_s;
                float angularDifferenceMagnitude = glm::abs(angularDifference);

                float angularAccelerationStep = angularAcceleration * timestep_s;
                if (angularAccelerationStep > angularDifferenceMagnitude)
                {
                    angularAccelerationStep = angularDifferenceMagnitude; // prevent angular acceleration from overshooting angular max speed
                }

                float newAngularVelocity = angularVelocity;

                if (angleToCursor > 0.0f)
                {
                    newAngularVelocity += angularAccelerationStep;
                }
                else
                {
                    newAngularVelocity -= angularAccelerationStep;
                }

                playerCollisionBodyPtr->SetAngularVelocity(newAngularVelocity);
            }
            else // apply angular deceleration
            {
                float newAngularVelocity;
                float frictionStep = angularFriction * timestep_s;
                if (angularVelocity > 0.0f)
                {
                    newAngularVelocity = glm::max(angularVelocity - frictionStep, 0.0f);
                }
                else
                {
                    newAngularVelocity = glm::min(angularVelocity + frictionStep, 0.0f);
                }

                playerCollisionBodyPtr->SetAngularVelocity(newAngularVelocity);
            }
        }
    }
}

void TestScene102::UpdateCursorPosition(float xPosition, float yPosition)
{
    int windowWidth, windowHeight;
    GetWindowPtr()->GetWindowSize(windowWidth, windowHeight);

    glm::vec2 viewportNormalizedCursorPosition = GetRendererPtr()->ConvertPointFromWindowToViewportNormalized(glm::vec2(xPosition, yPosition), static_cast<float>(windowHeight));

    Project001::Camera* cameraPtr;
    if (GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCamera_EntityId_))
    {
        glm::vec3 worldCursorPosition;
        glm::vec3 worldCursorNormal;
        bool cursorRayIntersected = cameraPtr->RaycastPointFromNormalizedViewportToPane(
            viewportNormalizedCursorPosition,
            glm::vec3(0.0f, 0.0f, 1.0f),
            0.0f,
            worldCursorPosition,
            worldCursorNormal);

        if (cursorRayIntersected)
        {
            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
            if (collisionBody2DPtr != nullptr)
            {
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();

                collisionPoints[cursorPositionCollisionPointIndex_].position = worldCursorPosition;
            }
        }
    }
}

void TestScene102::SyncMainCameraRenderedModel()
{
    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCamera_EntityId_));
    if (cameraPtr != nullptr)
    {
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, mainCamera_EntityId_));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetPosition(cameraPtr->GetPosition());
            renderedModelPtr->SetOrientation(cameraPtr->GetOrientation());
        }
    }
}

void TestScene102::SyncCursorRenderedModel()
{
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
        Project001::CollisionPoint2D collisionPoint01 = collisionPoints[cursorPositionCollisionPointIndex_];
        Project001::CollisionPoint2D collisionPoint02 = collisionPoints[cursorPressCollisionPointIndex_];
        Project001::CollisionPoint2D collisionPoint03 = collisionPoints[cursorReleaseCollisionPointIndex_];

        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursor_EntityId_));
        if (renderedModelPtr != nullptr)
        {
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            Project001::RenderedMesh& circleMesh01 = renderedMeshes[cursorPositionRenderedMeshIndex_];
            Project001::RenderedMesh& circleMesh02 = renderedMeshes[cursorPressRenderedMeshIndex_];
            Project001::RenderedMesh& circleMesh03 = renderedMeshes[cursorReleaseRenderedMeshIndex_];

            circleMesh01.SetPositionX(collisionPoint01.position.x);
            circleMesh01.SetPositionY(collisionPoint01.position.y);
            circleMesh01.SetVisible(collisionPoint01.enabled);


            circleMesh02.SetPositionX(collisionPoint02.position.x);
            circleMesh02.SetPositionY(collisionPoint02.position.y);
            circleMesh02.SetVisible(collisionPoint02.enabled);

            circleMesh03.SetPositionX(collisionPoint03.position.x);
            circleMesh03.SetPositionY(collisionPoint03.position.y);
            circleMesh03.SetVisible(collisionPoint03.enabled);
        }
    }
}

void TestScene102::SyncPlayerRenderedModel()
{
    Project001::CollisionBody2D* collisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, player_EntityId_));
    if (collisionBodyPtr != nullptr)
    {
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, player_EntityId_));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetPosition(glm::vec3(collisionBodyPtr->GetPosition(), 0.0f));
            renderedModelPtr->ResetOrientation();
            renderedModelPtr->AddRelativeRotationZ(collisionBodyPtr->GetRotation());
        }
    }
}

void TestScene102::CapVelocities()
{
    Project001::CollisionBody2D* collisionBody2DArray = nullptr;
    size_t collisionBodyCount = 0;

    GetComponentStoresPtr()->GetAllComponents<Project001::CollisionBody2D>(collisionBody2DArray, collisionBodyCount);

    for (unsigned int i = 0; i < collisionBodyCount; ++i)
    {
        Project001::CollisionBody2D& collisionBody2D = collisionBody2DArray[i];

        const uint32_t& collisionGroupMask = collisionBody2D.GetCollisionGroupMask();

        if (!(collisionGroupMask & s_mainCollisionGroupMask_))
        {
            continue;
        }

        const glm::vec2& velocity = collisionBody2D.GetVelocity();

        float velocityMagnitude = glm::length(velocity);

        if (velocityMagnitude > maxCollisionBodyVelocity_)
        {
            glm::vec2 newVelocity = velocity * maxCollisionBodyVelocity_ / velocityMagnitude;
            collisionBody2D.SetVelocity(newVelocity);
        }
    }
}