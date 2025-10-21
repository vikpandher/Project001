// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-20

#include "TestScene103.h"

#include "TestApplicationData.h"
#include "TestResource_AntonioRegular_png.h"
#include "TestResource_AntonioRegular_ssf.h"

#include "Components/Camera.h"
#include "Components/CollisionBody2D.h"
#include "Components/RenderedModel.h"
#include "Math/MathUtilities.h"
#include "Resources/PixelFont5x6.h"
#include "ComponentStores.h"
#include "FontLoader.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "RenderSystem.h"
#include "SoundPlayer.h"
#include "TextureLoader.h"
#include "Window.h"



// public ----------------------------------------------------------------------

TestScene103::TestScene103(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , instructionScene_(applicationPtr)
    , cursorCircle_MeshDataPtr_(nullptr)
    , pixelFont_FontDataPtr_(nullptr)
    , pixelFont_TextureDataPtr_(nullptr)
    , pixelFont_TextureId_((unsigned int)-1)
    , groundGrid_MeshDataPtr_(nullptr)
    , groundGrid_MeshId_((unsigned int)-1)
    , groundGridLabels_MeshDataPtr_(nullptr)
    , groundGridLabels_MeshId_((unsigned int)-1)
    , groundDark_MeshDataPtr_(nullptr)
    , groundDark_TextureDataPtr_(nullptr)
    , groundDark_TextureId_((unsigned int)-1)
    , groundLight_MeshDataPtr_(nullptr)
    , groundLight_TextureDataPtr_(nullptr)
    , groundLight_TextureId_((unsigned int)-1)
    , playerDark_MeshDataPtr_(nullptr)
    , playerDark_TextureDataPtr_(nullptr)
    , playerDark_TextureId_((unsigned int)-1)
    , playerLight_MeshDataPtr_(nullptr)
    , playerLight_TextureDataPtr_(nullptr)
    , playerLight_TextureId_((unsigned int)-1)
    , light01_MeshDataPtr_(nullptr)
    , mainCameraDark_EntityId_((unsigned int)-1)
    , mainCameraLight_EntityId_((unsigned int)-1)
    , mainCamera_LookAtPoint_()
    , mainCamera_DistanceAway_(0.0f)
    , uiCamera_EntityId_((unsigned int)-1)
    , cursor_EntityId_((unsigned int)-1)
    , cursorPositionRenderedMeshIndex_((unsigned int)-1)
    , cursorPressRenderedMeshIndex_((unsigned int)-1)
    , cursorReleaseRenderedMeshIndex_((unsigned int)-1)
    , cursorPositionCollisionPointIndex_((unsigned int)-1)
    , cursorPressCollisionPointIndex_((unsigned int)-1)
    , cursorReleaseCollisionPointIndex_((unsigned int)-1)
    , ground_EntityId_((unsigned int)-1)
    , player_EntityId_((unsigned int)-1)
    , light01_EntityId_((unsigned int)-1)
{
    GetSharedDataPtr<TestApplicationData>()->testScene103Id = GetId();
}

TestScene103::~TestScene103()
{}

void TestScene103::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene103::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene103::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestScene103::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene103::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestScene103::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene103::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene103::ProcessUpdateEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene103::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene103:            " << GetId());

    // Resources ---------------------------------------------------------------

    LoadCursorResources();
    LoadPixelFontResources();
    LoadGroundResources();
    LoadGroundGridResources();
    LoadPlayerResources();
    LoadLightResources();

    // Entities ----------------------------------------------------------------

    CreateMainCameraEntities();
    CreateUiCameraEntity();
    CreateCursorEntity();
    CreateGroundEntity();
    CreatePlayerEntity();
    CreateLightEntities();

    // Member Scenes -----------------------------------------------------------

    InitializeInstructionScene();
}

void TestScene103::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene103:            " << GetId());

    // -------------------------------------------------------------------------

    GetRendererPtr()->DeleteAllTextures();
    GetRendererPtr()->DeleteAllMeshes();
    GetComponentStoresPtr()->DeleteAllEntities();

    // Resources ---------------------------------------------------------------

    delete cursorCircle_MeshDataPtr_;
    cursorCircle_MeshDataPtr_ = nullptr;

    pixelFont_FontDataPtr_ = nullptr;
    pixelFont_TextureDataPtr_ = nullptr;
    pixelFont_TextureId_ = (unsigned int)-1;

    delete groundGrid_MeshDataPtr_;
    groundGrid_MeshDataPtr_ = nullptr;
    groundGrid_MeshId_ = (unsigned int)-1;
    delete groundGridLabels_MeshDataPtr_;
    groundGridLabels_MeshDataPtr_ = nullptr;
    groundGridLabels_MeshId_ = (unsigned int)-1;

    delete groundDark_MeshDataPtr_;
    groundDark_MeshDataPtr_ = nullptr;
    delete groundDark_TextureDataPtr_;
    groundDark_TextureDataPtr_ = nullptr;
    groundDark_TextureId_ = (unsigned int)-1;

    delete groundLight_MeshDataPtr_;
    groundLight_MeshDataPtr_ = nullptr;
    delete groundLight_TextureDataPtr_;
    groundLight_TextureDataPtr_ = nullptr;
    groundLight_TextureId_ = (unsigned int)-1;

    delete playerDark_MeshDataPtr_;
    playerDark_MeshDataPtr_ = nullptr;
    delete playerDark_TextureDataPtr_;
    playerDark_TextureDataPtr_ = nullptr;
    playerDark_TextureId_ = (unsigned int)-1;

    delete playerLight_MeshDataPtr_;
    playerLight_MeshDataPtr_ = nullptr;
    delete playerLight_TextureDataPtr_;
    playerLight_TextureDataPtr_ = nullptr;
    playerLight_TextureId_ = (unsigned int)-1;

    delete light01_MeshDataPtr_;
    light01_MeshDataPtr_ = nullptr;

    // Entities ----------------------------------------------------------------

    mainCameraDark_EntityId_ = (unsigned int)-1;
    mainCameraLight_EntityId_ = (unsigned int)-1;
    mainCamera_LookAtPoint_ = glm::vec3();
    mainCamera_DistanceAway_ = 0.0f;

    uiCamera_EntityId_ = (unsigned int)-1;

    cursor_EntityId_ = (unsigned int)-1;
    cursorPositionRenderedMeshIndex_ = (unsigned int)-1;
    cursorPressRenderedMeshIndex_ = (unsigned int)-1;
    cursorReleaseRenderedMeshIndex_ = (unsigned int)-1;
    cursorPositionCollisionPointIndex_ = (unsigned int)-1;
    cursorPressCollisionPointIndex_ = (unsigned int)-1;
    cursorReleaseCollisionPointIndex_ = (unsigned int)-1;

    ground_EntityId_ = (unsigned int)-1;

    player_EntityId_ = (unsigned int)-1;

    light01_EntityId_ = (unsigned int)-1;
}

void TestScene103::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent)
{
    UpdateCursorPosition(cursorPositionEvent.xPosition, cursorPositionEvent.yPosition);

    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
        collisionPoints[cursorPositionCollisionPointIndex_].tangible = true;
    }
}

void TestScene103::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
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
    else if (keyCode == Project001::KeyCode::KEY_CODE_P)
    {
        Project001::Camera* cameraPtr = nullptr;
        if (GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraDark_EntityId_))
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

        cameraPtr = nullptr;
        if (GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraLight_EntityId_))
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

void TestScene103::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;

    if (mouseButton == Project001::MouseButton::MOUSE_BUTTON_1)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS)
        {
            Project001::RenderedModel* renderedModelPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursor_EntityId_));
            if (renderedModelPtr != nullptr)
            {
                renderedModelPtr->SetVisible(true);
                std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
                renderedMeshes[cursorPressRenderedMeshIndex_].SetVisible(true);
                renderedMeshes[cursorReleaseRenderedMeshIndex_].SetVisible(false);
            }

            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
            if (collisionBody2DPtr != nullptr)
            {
                collisionBody2DPtr->SetTangible(true);
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
                collisionPoints[cursorPressCollisionPointIndex_].position = collisionPoints[cursorPositionCollisionPointIndex_].position;
                collisionPoints[cursorPressCollisionPointIndex_].tangible = true;
                collisionPoints[cursorReleaseCollisionPointIndex_].tangible = false;
            }
        }
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
        {
            Project001::RenderedModel* renderedModelPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursor_EntityId_));
            if (renderedModelPtr != nullptr)
            {
                renderedModelPtr->SetVisible(true);
                std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
                renderedMeshes[cursorReleaseRenderedMeshIndex_].SetVisible(true);
            }

            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
            if (collisionBody2DPtr != nullptr)
            {
                collisionBody2DPtr->SetTangible(true);
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
                collisionPoints[cursorReleaseCollisionPointIndex_].position = collisionPoints[cursorPositionCollisionPointIndex_].position;
                collisionPoints[cursorReleaseCollisionPointIndex_].tangible = true;
            }
        }
    }
}

void TestScene103::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    Project001::RenderSystem::Render(GetComponentStoresPtr(), GetRendererPtr());
}

void TestScene103::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    unsigned long long timestep_ns = updateEvent.timestep_ns;
    float timestep_s = (float)timestep_ns / 1e9f;

    UpdateMainCameraEntityPosition(timestep_s);

    // Update cursor because camera updates
    // -------------------------------------------------------------------------
    float cursorX_position;
    float cursorY_position;
    GetWindowPtr()->GetCursorPosition(cursorX_position, cursorY_position);
    UpdateCursorPosition(cursorX_position, cursorY_position);

    // Sync rendered models
    // -------------------------------------------------------------------------
    SyncCursorRenderedModel();
}

void TestScene103::InitializeInstructionScene()
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
        cameraPtr->TurnOn();
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
    instructionSceneInfo.fontTextureIdPtr = &font01_TextureId;
    instructionSceneInfo.cameraEntityIdPtr = &uiCamera_EntityId;
    instructionSceneInfo.cameraMaskPtr = &uiCamera_Mask;
    instructionSceneInfo.keyCode_toggleInstructionsPtr = &keyCode_toggleInstructions;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene103::LoadCursorResources()
{
    cursorCircle_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*cursorCircle_MeshDataPtr_, 4.0f, 12));
}

void TestScene103::LoadPixelFontResources()
{
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

void TestScene103::LoadGroundResources()
{
    groundDark_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*groundDark_MeshDataPtr_, 2048.0f, 2048.0f, 0.0f, 1.0f, 0.0f, 1.0f));

    groundDark_TextureDataPtr_ = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTexture(*groundDark_TextureDataPtr_, "../Textures/gridDark_2048x2048.png"));
    GetRendererPtr()->CreateTexture(
        groundDark_TextureId_,
        groundDark_TextureDataPtr_->data,
        groundDark_TextureDataPtr_->width,
        groundDark_TextureDataPtr_->height,
        groundDark_TextureDataPtr_->bytesPerPixel,
        false, false
    );

    groundLight_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*groundLight_MeshDataPtr_, 2048.0f, 2048.0f, 0.0f, 1.0f, 0.0f, 1.0f));

    groundLight_TextureDataPtr_ = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTexture(*groundLight_TextureDataPtr_, "../Textures/gridLight_2048x2048.png"));
    GetRendererPtr()->CreateTexture(
        groundLight_TextureId_,
        groundLight_TextureDataPtr_->data,
        groundLight_TextureDataPtr_->width,
        groundLight_TextureDataPtr_->height,
        groundLight_TextureDataPtr_->bytesPerPixel,
        false, false
    );
}

void TestScene103::LoadGroundGridResources()
{
    groundGrid_MeshDataPtr_ = new Project001::MeshData();
    const float gridSpacing = 64.0f;
    const float gridSize = 16.0f * gridSpacing;
    const float lineWidth = 2.0f;

    for (float f = -gridSize; f < gridSize + 1.0f; f += gridSpacing)
    {
        FAIL_CHECK(Project001::MeshLoader::Generate2DLine(
            *groundGrid_MeshDataPtr_,
            glm::vec2(-gridSize - 0.5f * lineWidth, f),
            glm::vec2(gridSize + 0.5f * lineWidth, f),
            lineWidth));
    }
    for (float f = -gridSize; f < gridSize + 1.0f; f += gridSpacing)
    {
        for (float g = -gridSize; g < gridSize - 1.0f; g += gridSpacing)
        {
            FAIL_CHECK(Project001::MeshLoader::Generate2DLine(
                *groundGrid_MeshDataPtr_,
                glm::vec2(f, g + 0.5f * lineWidth),
                glm::vec2(f, g - 0.5f * lineWidth + gridSpacing),
                lineWidth));
        }
    }

    GetRendererPtr()->CreateMesh(
        groundGrid_MeshId_,
        groundGrid_MeshDataPtr_->meshVertexArray.data(),
        (unsigned int)groundGrid_MeshDataPtr_->meshVertexArray.size(),
        groundGrid_MeshDataPtr_->meshIndexArray.data(),
        (unsigned int)groundGrid_MeshDataPtr_->meshIndexArray.size()
    );

    groundGridLabels_MeshDataPtr_ = new Project001::MeshData();

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

    const float pixelFont_pixelSize = 2.0f;
    float gridLabel_offsetY = pixelFont_pixelSize * 7.0f + 0.5f * lineWidth;
    float gridLabel_offsetX = pixelFont_pixelSize * 1.0f + 0.5f * lineWidth;

    for (size_t i = 0; i < gridLabels.size(); ++i)
    {
        const std::string& currentLabel = gridLabels[i];

        Project001::MeshData currentLabelMeshData;
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            currentLabelMeshData,
            *pixelFont_FontDataPtr_,
            currentLabel,
            pixelFont_pixelSize
        ));
        Project001::MeshLoader::TranslateMesh(
            currentLabelMeshData,
            glm::vec3(gridLabel_offsetX, gridLabel_offsetY, 0.0f)
        );

        Project001::MeshLoader::CopyMesh(*groundGridLabels_MeshDataPtr_, currentLabelMeshData);

        gridLabel_offsetX += gridSpacing;
    }

    // positive y-axis -----------------------------------------------------

    gridLabel_offsetY = pixelFont_pixelSize * 7.0f + 0.5f * lineWidth + gridSpacing;
    gridLabel_offsetX = pixelFont_pixelSize * 1.0f + 0.5f * lineWidth;

    for (size_t i = 1; i < gridLabels.size(); ++i)
    {
        const std::string& currentLabel = gridLabels[i];

        Project001::MeshData currentLabelMeshData;
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            currentLabelMeshData,
            *pixelFont_FontDataPtr_,
            currentLabel,
            pixelFont_pixelSize
        ));
        Project001::MeshLoader::TranslateMesh(
            currentLabelMeshData,
            glm::vec3(gridLabel_offsetX, gridLabel_offsetY, 0.0f)
        );

        Project001::MeshLoader::CopyMesh(*groundGridLabels_MeshDataPtr_, currentLabelMeshData);

        gridLabel_offsetY += gridSpacing;
    }

    // negative x-axis -----------------------------------------------------

    gridLabel_offsetY = pixelFont_pixelSize * -1.0f - 0.5f * lineWidth;
    gridLabel_offsetX = pixelFont_pixelSize * -6.0f - 0.5f * lineWidth;

    for (size_t i = 0; i < gridLabels.size(); ++i)
    {
        const std::string& currentLabel = gridLabels[i];

        float addition_offsetX = pixelFont_pixelSize * -6.0f * (float)(currentLabel.length() - 1);

        Project001::MeshData currentLabelMeshData;
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            currentLabelMeshData,
            *pixelFont_FontDataPtr_,
            currentLabel,
            pixelFont_pixelSize
        ));
        Project001::MeshLoader::TranslateMesh(
            currentLabelMeshData,
            glm::vec3(gridLabel_offsetX + addition_offsetX, gridLabel_offsetY, 0.0f)
        );

        Project001::MeshLoader::CopyMesh(*groundGridLabels_MeshDataPtr_, currentLabelMeshData);

        gridLabel_offsetX -= gridSpacing;
    }

    // negative y-axis -----------------------------------------------------

    gridLabel_offsetY = pixelFont_pixelSize * -1.0f - 0.5f * lineWidth - gridSpacing;
    gridLabel_offsetX = pixelFont_pixelSize * -6.0f - 0.5f * lineWidth;

    for (size_t i = 1; i < gridLabels.size(); ++i)
    {
        const std::string& currentLabel = gridLabels[i];

        float addition_offsetX = pixelFont_pixelSize * -6.0f * (float)(currentLabel.length() - 1);

        Project001::MeshData currentLabelMeshData;
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            currentLabelMeshData,
            *pixelFont_FontDataPtr_,
            currentLabel,
            pixelFont_pixelSize
        ));
        Project001::MeshLoader::TranslateMesh(
            currentLabelMeshData,
            glm::vec3(gridLabel_offsetX + addition_offsetX, gridLabel_offsetY, 0.0f)
        );

        Project001::MeshLoader::CopyMesh(*groundGridLabels_MeshDataPtr_, currentLabelMeshData);

        gridLabel_offsetY -= gridSpacing;
    }

    GetRendererPtr()->CreateMesh(
        groundGridLabels_MeshId_,
        groundGridLabels_MeshDataPtr_->meshVertexArray.data(),
        (unsigned int)groundGridLabels_MeshDataPtr_->meshVertexArray.size(),
        groundGridLabels_MeshDataPtr_->meshIndexArray.data(),
        (unsigned int)groundGridLabels_MeshDataPtr_->meshIndexArray.size()
    );
}

void TestScene103::LoadPlayerResources()
{
    playerDark_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*playerDark_MeshDataPtr_, 32.0f, 48.0f, 0.0f, 1.0f, 0.0f, 1.0f));
    Project001::MeshLoader::TranslateMesh(*playerDark_MeshDataPtr_, glm::vec3(0.0f, 24.0f, 0.0f));

    playerDark_TextureDataPtr_ = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTexture(*playerDark_TextureDataPtr_, "../Textures/dark_32x48.png"));
    GetRendererPtr()->CreateTexture(
        playerDark_TextureId_,
        playerDark_TextureDataPtr_->data,
        playerDark_TextureDataPtr_->width,
        playerDark_TextureDataPtr_->height,
        playerDark_TextureDataPtr_->bytesPerPixel,
        false, false
    );

    playerLight_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*playerLight_MeshDataPtr_, 32.0f, 48.0f, 0.0f, 1.0f, 0.0f, 1.0f));
    Project001::MeshLoader::TranslateMesh(*playerLight_MeshDataPtr_, glm::vec3(0.0f, 24.0f, 0.0f));

    playerLight_TextureDataPtr_ = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTexture(*playerLight_TextureDataPtr_, "../Textures/light_32x48.png"));
    GetRendererPtr()->CreateTexture(
        playerLight_TextureId_,
        playerLight_TextureDataPtr_->data,
        playerLight_TextureDataPtr_->width,
        playerLight_TextureDataPtr_->height,
        playerLight_TextureDataPtr_->bytesPerPixel,
        false, false
    );
}

void TestScene103::LoadLightResources()
{
    light01_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::GenerateTruncatedCone(*light01_MeshDataPtr_, 120.0f, 8.0f, 64.0f, 16, false));
    Project001::MeshLoader::TranslateMesh(*light01_MeshDataPtr_, glm::vec3(0.0f, -68.0f, 0.0f));
    Project001::MeshLoader::RotateMesh(*light01_MeshDataPtr_, glm::angleAxis(glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f)));
}

void TestScene103::CreateMainCameraEntities()
{
    GetComponentStoresPtr()->CreateEntity(mainCameraDark_EntityId_);
    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::Camera>(mainCameraDark_EntityId_));

    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraDark_EntityId_));
    if (cameraPtr != nullptr)
    {
        int aspectRatioNumerator;
        int aspectRatioDenominator;
        GetWindowPtr()->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
        if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
        {
            float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;
            float mainCameraHalfHeight = 320.0f;
            float mainCameraHalfWidth = aspectRatio * mainCameraHalfHeight;
            cameraPtr->SetAspectRatio(aspectRatio);
            cameraPtr->SetTopCutoff(mainCameraHalfHeight);
            cameraPtr->SetBottomCutoff(-mainCameraHalfHeight);
            cameraPtr->SetLeftCutoff(-mainCameraHalfWidth);
            cameraPtr->SetRightCutoff(mainCameraHalfWidth);
            cameraPtr->SetNearCutoff(mainCameraHalfHeight * 0.1f);
            cameraPtr->SetFarCutoff(mainCameraHalfHeight * 10.0f);
        }

        cameraPtr->AddYaw(glm::pi<float>());
        cameraPtr->AddPitch(-glm::quarter_pi<float>());
        cameraPtr->SetProjection(Project001::Camera::CameraProjection::CAMERA_PROJECTION_PERSPECTIVE);
        cameraPtr->TurnOn();
        cameraPtr->SetCameraMask(s_mainCameraDark_Mask_ | s_mainCameraDebug_Mask_);

        GetSoundPlayerPtr()->SetListenerPosition(cameraPtr->GetPosition());
        GetSoundPlayerPtr()->SetListenerForwardDirection(cameraPtr->GetForwardVector());
        GetSoundPlayerPtr()->SetListenerUpDirection(cameraPtr->GetUpVector());
    }

    GetComponentStoresPtr()->CreateEntity(mainCameraLight_EntityId_);
    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::Camera>(mainCameraLight_EntityId_));

    cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraLight_EntityId_));
    if (cameraPtr != nullptr)
    {
        int aspectRatioNumerator;
        int aspectRatioDenominator;
        GetWindowPtr()->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
        if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
        {
            float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;
            float mainCameraHalfHeight = 320.0f;
            float mainCameraHalfWidth = aspectRatio * mainCameraHalfHeight;
            cameraPtr->SetAspectRatio(aspectRatio);
            cameraPtr->SetTopCutoff(mainCameraHalfHeight);
            cameraPtr->SetBottomCutoff(-mainCameraHalfHeight);
            cameraPtr->SetLeftCutoff(-mainCameraHalfWidth);
            cameraPtr->SetRightCutoff(mainCameraHalfWidth);
            cameraPtr->SetNearCutoff(mainCameraHalfHeight * 0.1f);
            cameraPtr->SetFarCutoff(mainCameraHalfHeight * 10.0f);
        }

        cameraPtr->AddYaw(glm::pi<float>());
        cameraPtr->AddPitch(-glm::quarter_pi<float>());
        cameraPtr->SetProjection(Project001::Camera::CameraProjection::CAMERA_PROJECTION_PERSPECTIVE);
        cameraPtr->TurnOn();
        cameraPtr->SetCameraMask(s_mainCameraLight_Mask_ | s_mainCameraDebug_Mask_);
        cameraPtr->SetPriorityValue(-100);

        GetSoundPlayerPtr()->SetListenerPosition(cameraPtr->GetPosition());
        GetSoundPlayerPtr()->SetListenerForwardDirection(cameraPtr->GetForwardVector());
        GetSoundPlayerPtr()->SetListenerUpDirection(cameraPtr->GetUpVector());
    }

    mainCamera_DistanceAway_ = 800.0f;
}

void TestScene103::CreateUiCameraEntity()
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
        cameraPtr->TurnOn();
        cameraPtr->SetCameraMask(s_uiCamera_Mask_);
        cameraPtr->SetPriorityValue(100);
    }
}

void TestScene103::CreateCursorEntity()
{
    GetComponentStoresPtr()->CreateEntity(cursor_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(cursor_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursor_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        cursorPositionRenderedMeshIndex_ = renderedMeshes.size();
        renderedMeshes.emplace_back();
        Project001::RenderedMesh& circleMesh01 = renderedMeshes.back();
        circleMesh01.SetCameraMask(s_mainCameraDebug_Mask_);
        circleMesh01.SetMeshDataPtr(cursorCircle_MeshDataPtr_);
        circleMesh01.SetPositionZ(0.53f);
        circleMesh01.SetColor(0.8f, 0.8f, 0.8f, 0.4f);
        circleMesh01.SetTranslucent(true);
        circleMesh01.SetLit(false);

        cursorPressRenderedMeshIndex_ = renderedMeshes.size();
        renderedMeshes.emplace_back();
        Project001::RenderedMesh& circleMesh02 = renderedMeshes.back();
        circleMesh01.SetCameraMask(s_mainCameraDebug_Mask_);
        circleMesh02.SetMeshDataPtr(cursorCircle_MeshDataPtr_);
        circleMesh02.SetPositionZ(0.52f);
        circleMesh02.SetColor(0.8f, 0.2f, 0.2f, 0.4f);
        circleMesh02.SetTranslucent(true);
        circleMesh02.SetLit(false);
        circleMesh02.SetVisible(false);

        cursorReleaseRenderedMeshIndex_ = renderedMeshes.size();
        renderedMeshes.emplace_back();
        Project001::RenderedMesh& circleMesh03 = renderedMeshes.back();
        circleMesh01.SetCameraMask(s_mainCameraDebug_Mask_);
        circleMesh03.SetMeshDataPtr(cursorCircle_MeshDataPtr_);
        circleMesh03.SetPositionZ(0.51f);
        circleMesh03.SetColor(0.8f, 0.8f, 0.2f, 0.4f);
        circleMesh03.SetTranslucent(true);
        circleMesh03.SetLit(false);
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
}

void TestScene103::CreateGroundEntity()
{
    GetComponentStoresPtr()->CreateEntity(ground_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(ground_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, ground_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraLight_Mask_);
            mesh.SetMeshDataPtr(groundLight_MeshDataPtr_);
            mesh.SetTextureId(groundLight_TextureId_);
            mesh.SetLit(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDark_Mask_);
            mesh.SetMeshDataPtr(groundDark_MeshDataPtr_);
            mesh.SetTextureId(groundDark_TextureId_);
            mesh.SetLit(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshIdAndMaxBoundingRadius(groundGrid_MeshId_, groundGrid_MeshDataPtr_->maxBoundingRadius);
            mesh.SetPositionZ(0.1f);
            mesh.SetColor(0.2f, 0.2f, 1.0f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetLit(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshIdAndMaxBoundingRadius(groundGridLabels_MeshId_, groundGridLabels_MeshDataPtr_->maxBoundingRadius);
            mesh.SetTextureId(pixelFont_TextureId_);
            mesh.SetPositionZ(0.1f);
            mesh.SetColor(0.2f, 0.2f, 1.0f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetLit(false);
        }
    }
}

void TestScene103::CreatePlayerEntity()
{
    GetComponentStoresPtr()->CreateEntity(player_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(player_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, player_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraLight_Mask_);
            mesh.SetMeshDataPtr(playerLight_MeshDataPtr_);
            mesh.SetTextureId(playerLight_TextureId_);
            mesh.LookAt(glm::normalize(glm::vec3(0.0f, -1.0f, 0.5f)), glm::vec3(0.0f, 0.0f, 1.0f));
            mesh.SetPositionY(-8.0f);
            mesh.SetLit(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDark_Mask_);
            mesh.SetMeshDataPtr(playerDark_MeshDataPtr_);
            mesh.SetTextureId(playerDark_TextureId_);
            mesh.LookAt(glm::normalize(glm::vec3(0.0f, -1.0f, 0.5f)), glm::vec3(0.0f, 0.0f, 1.0f));
            mesh.SetPositionY(-8.0f);
            mesh.SetLit(false);
        }
    }
}

void TestScene103::CreateLightEntities()
{
    GetComponentStoresPtr()->CreateEntity(light01_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(light01_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, light01_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        renderedMeshes.emplace_back();
        Project001::RenderedMesh& mesh = renderedMeshes.back();
        mesh.SetCameraMask(s_mainCameraDark_Mask_);
        mesh.SetMeshDataPtr(light01_MeshDataPtr_);
        mesh.SetColor(0.0f, 0.0f, 0.0f, 0.0f);
        mesh.SetPosition(0.0f, 0.0f, 16.0f);
        mesh.SetTranslucent(true);
        mesh.SetRenderPriorityOverride(-100);
        mesh.SetLit(false);
    }
}

void TestScene103::UpdateMainCameraEntityPosition(float timestep_s)
{
    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraDark_EntityId_));
    if (cameraPtr != nullptr)
    {
        constexpr float orbitSpeed = 0.1f * glm::pi<float>();
        float cameraPitchDelta = orbitSpeed * timestep_s;

        bool pitchingUp = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_PAGE_DOWN);
        bool pitchingDown = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_PAGE_UP);

        const float moveSpeed = 128.0f;
        float moveSpeedDelta = moveSpeed * timestep_s;

        bool movingLeft = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_LEFT);
        bool movingRight = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_RIGHT);
        bool movingUp = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_UP);
        bool movingDown = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_DOWN);

        if (pitchingUp)
        {
            cameraPtr->AddRelativeRotationX(-cameraPitchDelta);
        }

        if (pitchingDown)
        {
            cameraPtr->AddRelativeRotationX(cameraPitchDelta);
        }

        if (movingLeft)
        {
            mainCamera_LookAtPoint_.x -= moveSpeedDelta;
        }

        if (movingRight)
        {
            mainCamera_LookAtPoint_.x += moveSpeedDelta;
        }

        if (movingUp)
        {
            mainCamera_LookAtPoint_.y += moveSpeedDelta;
        }

        if (movingDown)
        {
            mainCamera_LookAtPoint_.y -= moveSpeedDelta;
        }

        cameraPtr->FollowFocalPoint(mainCamera_LookAtPoint_, mainCamera_DistanceAway_);

        Project001::Camera* otherCameraPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(otherCameraPtr, mainCameraLight_EntityId_));
        if (cameraPtr != nullptr)
        {
            otherCameraPtr->SetPosition(cameraPtr->GetPosition());
            otherCameraPtr->SetOrientation(cameraPtr->GetOrientation());
        }
    }
}

void TestScene103::UpdateCursorPosition(float xPosition, float yPosition)
{
    int windowWidth, windowHeight;
    GetWindowPtr()->GetWindowSize(windowWidth, windowHeight);

    glm::vec2 viewportNormalizedCursorPosition = GetRendererPtr()->ConvertPointFromWindowToViewportNormalized(glm::vec2(xPosition, yPosition), (float)windowHeight);

    Project001::Camera* cameraPtr;
    if (GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraLight_EntityId_))
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

void TestScene103::SyncCursorRenderedModel()
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
            circleMesh01.SetVisible(collisionPoint01.tangible);


            circleMesh02.SetPositionX(collisionPoint02.position.x);
            circleMesh02.SetPositionY(collisionPoint02.position.y);
            circleMesh02.SetVisible(collisionPoint02.tangible);

            circleMesh03.SetPositionX(collisionPoint03.position.x);
            circleMesh03.SetPositionY(collisionPoint03.position.y);
            circleMesh03.SetVisible(collisionPoint03.tangible);
        }
    }
}