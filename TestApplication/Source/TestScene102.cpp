// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-08

#include "TestScene102.h"

#include "TestApplicationData.h"
#include "TestResource_AntonioRegular_png.h"
#include "TestResource_AntonioRegular_ssf.h"

#include "Components/Camera.h"
#include "Components/CollisionBody2D.h"
#include "Components/RenderedModel.h"
#include "Resources/PixelFont5x6.h"
#include "CollisionSystem2D.h"
#include "ComponentStores.h"
#include "FontLoader.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "RenderSystem.h"
#include "SoundLoader.h"
#include "SoundPlayer.h"
#include "TextureLoader.h"
#include "Window.h"



// public ----------------------------------------------------------------------

TestScene102::TestScene102(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , instructionScene_(applicationPtr)
    , font01_FontDataPtr_(nullptr)
    , font01_TextureDataPtr_(nullptr)
    , font01_TextureId_((unsigned int)-1)
    , pixelFont_FontDataPtr_(nullptr)
    , pixelFont_TextureDataPtr_(nullptr)
    , pixelFont_TextureId_((unsigned int)-1)
    , circle_MeshDataPtr_(nullptr)
    , floorGrid_MeshDataPtr_(nullptr)
    , floorGrid_MeshId_((unsigned int)-1)
    , floorGrid_MaxBoundingRadius_(0.0f)
    , floorGridLabels_MeshDataPtr_(nullptr)
    , floorGridLabels_MeshId_((unsigned int)-1)
    , floorGridLabels_MaxBoundingRadius_(0.0f)
    , mainCameraNearFrustum_MeshDataPtr_(nullptr)
    , mainCameraFarFrustum_MeshDataPtr_(nullptr)
    , ship_MeshDataPtr_(nullptr)
    , shipBeamSight_MeshDataPtr_(nullptr)
    , shipCollisionBody_MeshDataPtr_(nullptr)
    , border96x64_TextureId_((unsigned int)-1)
    , numbers16x4_TextureId_((unsigned int)-1)
    , mainCamera_EntityId_((unsigned int)-1)
    , uiCamera_EntityId_((unsigned int)-1)
    , cursor_EntityId_((unsigned int)-1)
    , cursorPositionRenderedMeshIndex_((unsigned int)-1)
    , cursorPressRenderedMeshIndex_((unsigned int)-1)
    , cursorReleaseRenderedMeshIndex_((unsigned int)-1)
    , cursorPositionCollisionPointIndex_((unsigned int)-1)
    , cursorPressCollisionPointIndex_((unsigned int)-1)
    , cursorReleaseCollisionPointIndex_((unsigned int)-1)
    , floor_EntityId_((unsigned int)-1)
    , player_EntityId_((unsigned int)-1)
    , cameraDisplacement_()
    , previousWorldCursorPosition_()
    , previousWorldCursorPress_()
    , previousWorldCursorRelease_()
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

    // Member Scenes -----------------------------------------------------------

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "TODO:\n"
        "Press <Esc> to return to Main Menu.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = font01_FontDataPtr_;
    instructionSceneInfo.fontTextureIdPtr = &font01_TextureId_;
    instructionSceneInfo.cameraEntityIdPtr = &uiCamera_EntityId_;
    instructionSceneInfo.cameraMaskPtr = &s_uiCameraMask_;
    instructionSceneInfo.keyCode_toggleInstructionsPtr = &s_keyCode_toggleInstructions_;
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

    // Font Data ---------------------------------------------------------------

    delete font01_FontDataPtr_;
    font01_FontDataPtr_ = nullptr;
    delete font01_TextureDataPtr_;
    font01_TextureDataPtr_ = nullptr;
    font01_TextureId_ = (unsigned int)-1;

    pixelFont_FontDataPtr_ = nullptr;
    pixelFont_TextureDataPtr_ = nullptr;
    pixelFont_TextureId_ = (unsigned int)-1;

    // Mesh Data ---------------------------------------------------------------

    delete circle_MeshDataPtr_;
    circle_MeshDataPtr_ = nullptr;

    delete floorGrid_MeshDataPtr_;
    floorGrid_MeshDataPtr_ = nullptr;
    floorGrid_MeshId_ = (unsigned int)-1;
    floorGrid_MaxBoundingRadius_ = 0.0f;

    delete floorGridLabels_MeshDataPtr_;
    floorGridLabels_MeshDataPtr_ = nullptr;
    floorGridLabels_MeshId_ = (unsigned int)-1;
    floorGridLabels_MaxBoundingRadius_ = 0.0f;

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

    border96x64_TextureId_ = (unsigned int)-1;
    numbers16x4_TextureId_ = (unsigned int)-1;

    // Entities ----------------------------------------------------------------

    mainCamera_EntityId_ = (unsigned int)-1;
    uiCamera_EntityId_ = (unsigned int)-1;

    cursor_EntityId_ = (unsigned int)-1;
    cursorPositionRenderedMeshIndex_ = (unsigned int)-1;
    cursorPressRenderedMeshIndex_ = (unsigned int)-1;
    cursorReleaseRenderedMeshIndex_ = (unsigned int)-1;
    cursorPositionCollisionPointIndex_ = (unsigned int)-1;
    cursorPressCollisionPointIndex_ = (unsigned int)-1;
    cursorReleaseCollisionPointIndex_ = (unsigned int)-1;

    floor_EntityId_ = (unsigned int)-1;

    player_EntityId_ = (unsigned int)-1;

    // -------------------------------------------------------------------------

    cameraDisplacement_ = glm::vec3(0.0f, 0.0f, 0.0f);

    previousWorldCursorPosition_ = glm::vec2(0.0f, 0.0f);
    previousWorldCursorPress_ = glm::vec2(0.0f, 0.0f);
    previousWorldCursorRelease_ = glm::vec2(0.0f, 0.0f);
}

void TestScene102::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent)
{
    UpdatePreviousWorldCursorPosition(cursorPositionEvent.xPosition, cursorPositionEvent.yPosition);

    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursor_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetVisible(true);
    }

    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        collisionBody2DPtr->SetTangible(true);
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
            Project001::Camera* cameraPtr;
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
                collisionPoints[cursorPressCollisionPointIndex_].tangible = true;
                collisionPoints[cursorReleaseCollisionPointIndex_].tangible = false;
            }

            previousWorldCursorPress_ = previousWorldCursorPosition_;
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
                collisionPoints[cursorReleaseCollisionPointIndex_].tangible = true;
            }

            previousWorldCursorRelease_ = previousWorldCursorPosition_;
        }
    }
}

void TestScene102::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    Project001::RenderSystem::Render(GetComponentStoresPtr(), GetRendererPtr());
}

void TestScene102::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    unsigned long long timestep_ns = updateEvent.timestep_ns;

    // Update entities
    UpdateMainCameraEntityPositionAndRoll(timestep_ns);

    // Update cursor because camera update
    // -------------------------------------------------------------------------
    float cursorX_position;
    float cursorY_position;
    GetWindowPtr()->GetCursorPosition(cursorX_position, cursorY_position);
    UpdatePreviousWorldCursorPosition(cursorX_position, cursorY_position);

    // Sync main camera rendered model
    // -------------------------------------------------------------------------
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

    // Sync cursor rendered model and collision body
    // -------------------------------------------------------------------------
    {
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursor_EntityId_));
        if (renderedModelPtr != nullptr)
        {
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            Project001::RenderedMesh& circleMesh01 = renderedMeshes[cursorPositionRenderedMeshIndex_];
            circleMesh01.SetPositionX(previousWorldCursorPosition_.x);
            circleMesh01.SetPositionY(previousWorldCursorPosition_.y);

            Project001::RenderedMesh& circleMesh02 = renderedMeshes[cursorPressRenderedMeshIndex_];
            circleMesh02.SetPositionX(previousWorldCursorPress_.x);
            circleMesh02.SetPositionY(previousWorldCursorPress_.y);

            Project001::RenderedMesh& circleMesh03 = renderedMeshes[cursorReleaseRenderedMeshIndex_];
            circleMesh03.SetPositionX(previousWorldCursorRelease_.x);
            circleMesh03.SetPositionY(previousWorldCursorRelease_.y);
        }

        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
        if (collisionBody2DPtr != nullptr)
        {
            std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();

            collisionPoints[cursorPositionCollisionPointIndex_].position.x = previousWorldCursorPosition_.x;
            collisionPoints[cursorPositionCollisionPointIndex_].position.y = previousWorldCursorPosition_.y;

            collisionPoints[cursorPressCollisionPointIndex_].position.x = previousWorldCursorPress_.x;
            collisionPoints[cursorPressCollisionPointIndex_].position.y = previousWorldCursorPress_.y;

            collisionPoints[cursorReleaseCollisionPointIndex_].position.x = previousWorldCursorRelease_.x;
            collisionPoints[cursorReleaseCollisionPointIndex_].position.y = previousWorldCursorRelease_.y;
        }
    }

    // Calculate collisions
    // -------------------------------------------------------------------------
    Project001::CollisionSystem2D::CalculateCollisions(GetComponentStoresPtr());
}

void TestScene102::LoadFontData()
{
    font01_FontDataPtr_ = new Project001::FontData;
    FAIL_CHECK(Project001::FontLoader::LoadFontDataFromMemory(
        *font01_FontDataPtr_,
        g_AntonioRegular_ssf,
        sizeof(g_AntonioRegular_ssf)
    ));

    font01_TextureDataPtr_ = new Project001::TextureData;
    FAIL_CHECK(Project001::TextureLoader::LoadTextureFromMemory(
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
    circle_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*circle_MeshDataPtr_, 4.0f, 12));

    {
        floorGrid_MeshDataPtr_ = new Project001::MeshData();
        const float gridSpacing = 64.0f;
        const float gridSize = 16.0f * gridSpacing;
        const float lineWidth = 8.0f;

        for (float f = -gridSize; f < gridSize + 1.0f; f += gridSpacing)
        {
            FAIL_CHECK(Project001::MeshLoader::Generate2DLine(
                *floorGrid_MeshDataPtr_,
                glm::vec2(-gridSize - 0.5f * lineWidth, f),
                glm::vec2(gridSize + 0.5f * lineWidth, f),
                lineWidth));
        }
        for (float f = -gridSize; f < gridSize + 1.0f; f += gridSpacing)
        {
            for (float g = -gridSize; g < gridSize - 1.0f; g += gridSpacing)
            {
                FAIL_CHECK(Project001::MeshLoader::Generate2DLine(
                    *floorGrid_MeshDataPtr_,
                    glm::vec2(f, g + 0.5f * lineWidth),
                    glm::vec2(f, g - 0.5f * lineWidth + gridSpacing),
                    lineWidth));
            }
        }

        GetRendererPtr()->CreateMesh(
            floorGrid_MeshId_,
            floorGrid_MeshDataPtr_->meshVertexArray.data(),
            (unsigned int)floorGrid_MeshDataPtr_->meshVertexArray.size(),
            floorGrid_MeshDataPtr_->meshIndexArray.data(),
            (unsigned int)floorGrid_MeshDataPtr_->meshIndexArray.size()
        );
        floorGrid_MaxBoundingRadius_ = floorGrid_MeshDataPtr_->maxBoundingRadius;

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

        float gridLabel_offsetY = pixelFont_pixelSize_ * 7.0f + 0.5f * lineWidth;
        float gridLabel_offsetX = pixelFont_pixelSize_ * 1.0f + 0.5f * lineWidth;

        for (size_t i = 0; i < gridLabels.size(); ++i)
        {
            const std::string& currentLabel = gridLabels[i];

            Project001::MeshData currentLabelMeshData;
            FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
                currentLabelMeshData,
                *pixelFont_FontDataPtr_,
                currentLabel,
                pixelFont_pixelSize_
            ));
            Project001::MeshLoader::TranslateMesh(
                currentLabelMeshData,
                glm::vec3(gridLabel_offsetX, gridLabel_offsetY, 0.0f)
            );

            Project001::MeshLoader::CopyMesh(*floorGridLabels_MeshDataPtr_, currentLabelMeshData);

            gridLabel_offsetX += gridSpacing;
        }

        // positive y-axis -----------------------------------------------------

        gridLabel_offsetY = pixelFont_pixelSize_ * 7.0f + 0.5f * lineWidth + gridSpacing;
        gridLabel_offsetX = pixelFont_pixelSize_ * 1.0f + 0.5f * lineWidth;

        for (size_t i = 1; i < gridLabels.size(); ++i)
        {
            const std::string& currentLabel = gridLabels[i];

            Project001::MeshData currentLabelMeshData;
            FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
                currentLabelMeshData,
                *pixelFont_FontDataPtr_,
                currentLabel,
                pixelFont_pixelSize_
            ));
            Project001::MeshLoader::TranslateMesh(
                currentLabelMeshData,
                glm::vec3(gridLabel_offsetX, gridLabel_offsetY, 0.0f)
            );

            Project001::MeshLoader::CopyMesh(*floorGridLabels_MeshDataPtr_, currentLabelMeshData);

            gridLabel_offsetY += gridSpacing;
        }

        // negative x-axis -----------------------------------------------------

        gridLabel_offsetY = pixelFont_pixelSize_ * -1.0f - 0.5f * lineWidth;
        gridLabel_offsetX = pixelFont_pixelSize_ * -6.0f - 0.5f * lineWidth;

        for (size_t i = 0; i < gridLabels.size(); ++i)
        {
            const std::string& currentLabel = gridLabels[i];

            float addition_offsetX = pixelFont_pixelSize_ * -6.0f * (float)(currentLabel.length() - 1);

            Project001::MeshData currentLabelMeshData;
            FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
                currentLabelMeshData,
                *pixelFont_FontDataPtr_,
                currentLabel,
                pixelFont_pixelSize_
            ));
            Project001::MeshLoader::TranslateMesh(
                currentLabelMeshData,
                glm::vec3(gridLabel_offsetX + addition_offsetX , gridLabel_offsetY, 0.0f)
            );

            Project001::MeshLoader::CopyMesh(*floorGridLabels_MeshDataPtr_, currentLabelMeshData);

            gridLabel_offsetX -= gridSpacing;
        }

        // negative y-axis -----------------------------------------------------

        gridLabel_offsetY = pixelFont_pixelSize_ * -1.0f - 0.5f * lineWidth - gridSpacing;
        gridLabel_offsetX = pixelFont_pixelSize_ * -6.0f - 0.5f * lineWidth;

        for (size_t i = 1; i < gridLabels.size(); ++i)
        {
            const std::string& currentLabel = gridLabels[i];

            float addition_offsetX = pixelFont_pixelSize_ * -6.0f * (float)(currentLabel.length() - 1);

            Project001::MeshData currentLabelMeshData;
            FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
                currentLabelMeshData,
                *pixelFont_FontDataPtr_,
                currentLabel,
                pixelFont_pixelSize_
            ));
            Project001::MeshLoader::TranslateMesh(
                currentLabelMeshData,
                glm::vec3(gridLabel_offsetX + addition_offsetX, gridLabel_offsetY, 0.0f)
            );

            Project001::MeshLoader::CopyMesh(*floorGridLabels_MeshDataPtr_, currentLabelMeshData);

            gridLabel_offsetY -= gridSpacing;
        }

        GetRendererPtr()->CreateMesh(
            floorGridLabels_MeshId_,
            floorGridLabels_MeshDataPtr_->meshVertexArray.data(),
            (unsigned int)floorGridLabels_MeshDataPtr_->meshVertexArray.size(),
            floorGridLabels_MeshDataPtr_->meshIndexArray.data(),
            (unsigned int)floorGridLabels_MeshDataPtr_->meshIndexArray.size()
        );
        floorGridLabels_MaxBoundingRadius_ = floorGridLabels_MeshDataPtr_->maxBoundingRadius;
    }

    mainCameraNearFrustum_MeshDataPtr_ = new Project001::MeshData();
    // Generated when Main Camera Entity created

    mainCameraFarFrustum_MeshDataPtr_ = new Project001::MeshData();
    // Generated when Main Camera Entity created

    ship_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::LoadMeshOBJ(*ship_MeshDataPtr_, "../Models/Ship.obj"));

    shipCollisionBody_MeshDataPtr_ = new Project001::MeshData();
    std::vector<glm::vec2> corners;
    corners.emplace_back(0.0f, 0.0f);
    corners.emplace_back(32.0f, -32.0f);
    corners.emplace_back(0.0f, 64.0f);
    corners.emplace_back(-32.0f, -32.0f);
    FAIL_CHECK(Project001::MeshLoader::Generate2DPolygon(*shipCollisionBody_MeshDataPtr_, corners));

    shipBeamSight_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*shipBeamSight_MeshDataPtr_, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 10000.0f), 2.0f));
}

void TestScene102::LoadTextureData()
{
    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/border_96x64.png"));
        GetRendererPtr()->CreateTexture(border96x64_TextureId_, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/16_4_numbers.png"));
        GetRendererPtr()->CreateTexture(numbers16x4_TextureId_, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }
}

void TestScene102::CreateCameraEntities()
{
    // Main Camera -------------------------------------------------------------
    {
        float mainCameraHalfHeight = 0.0f;
        float mainCameraHalfWidth = 0.0f;

        cameraDisplacement_.y = -640.0f;
        cameraDisplacement_.z = 640.0f;

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
            FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
                *mainCameraNearFrustum_MeshDataPtr_,
                corners[1], corners[0], corners[3], corners[2],
                0.0f, 1.0f, 0.0f, 1.0f
            ));
            Project001::MeshLoader::TranslateMesh(*mainCameraNearFrustum_MeshDataPtr_, glm::vec3(0.0f, 0.0f, 0.001f));
            FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
                *mainCameraFarFrustum_MeshDataPtr_,
                corners[5], corners[4], corners[7], corners[6],
                0.0f, 1.0f, 0.0f, 1.0f
            ));
            Project001::MeshLoader::TranslateMesh(*mainCameraFarFrustum_MeshDataPtr_, glm::vec3(0.0f, 0.0f, -1.0f));

            cameraPtr->SetPosition(cameraDisplacement_);
            cameraPtr->AddYaw(glm::pi<float>());
            cameraPtr->AddPitch(-glm::quarter_pi<float>());
            cameraPtr->SetProjection(Project001::Camera::CameraProjection::CAMERA_PROJECTION_PERSPECTIVE);
            cameraPtr->TurnOn();
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
            mesh01.SetLit(false);
            mesh01.SetVisible(false);

            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh02 = renderedMeshes.back();
            mesh02.SetMeshDataPtr(mainCameraFarFrustum_MeshDataPtr_);
            mesh02.SetTextureId(border96x64_TextureId_);
            mesh02.SetColor(0.0f, 1.0f, 0.0f, 0.2f);
            mesh02.SetTranslucent(true);
            mesh02.SetLit(false);
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
}

void TestScene102::CreateCursorEntity()
{
    float cursorX_position;
    float cursorY_position;
    GetWindowPtr()->GetCursorPosition(cursorX_position, cursorY_position);
    UpdatePreviousWorldCursorPosition(cursorX_position, cursorY_position);

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
        circleMesh01.SetMeshDataPtr(circle_MeshDataPtr_);
        circleMesh01.SetPositionZ(0.53f);
        circleMesh01.SetColor(0.8f, 0.8f, 0.8f, 0.4f);
        circleMesh01.SetTranslucent(true);
        circleMesh01.SetLit(false);

        cursorPressRenderedMeshIndex_ = renderedMeshes.size();
        renderedMeshes.emplace_back();
        Project001::RenderedMesh& circleMesh02 = renderedMeshes.back();
        circleMesh02.SetMeshDataPtr(circle_MeshDataPtr_);
        circleMesh02.SetPositionZ(0.52f);
        circleMesh02.SetColor(0.8f, 0.2f, 0.2f, 0.4f);
        circleMesh02.SetTranslucent(true);
        circleMesh02.SetLit(false);
        circleMesh02.SetVisible(false);

        cursorReleaseRenderedMeshIndex_ = renderedMeshes.size();
        renderedMeshes.emplace_back();
        Project001::RenderedMesh& circleMesh03 = renderedMeshes.back();
        circleMesh03.SetMeshDataPtr(circle_MeshDataPtr_);
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
        mesh01.SetMeshIdAndMaxBoundingRadius(floorGrid_MeshId_, floorGrid_MaxBoundingRadius_);
        mesh01.SetPositionZ(-0.1f);
        mesh01.SetColor(1.0f, 1.0f, 1.0f, 0.1f);
        mesh01.SetTranslucent(true);
        mesh01.SetLit(false);

        renderedMeshes.emplace_back();
        Project001::RenderedMesh& mesh02 = renderedMeshes.back();
        mesh02.SetMeshIdAndMaxBoundingRadius(floorGridLabels_MeshId_, floorGridLabels_MaxBoundingRadius_);
        mesh02.SetPositionZ(-0.1f);
        mesh02.SetColor(1.0f, 1.0f, 1.0f, 0.1f);
        mesh02.SetTranslucent(true);
        mesh02.SetLit(false);
        mesh02.SetTextureId(pixelFont_TextureId_);
    }
}

void TestScene102::CreatePlayerEntity()
{
    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.collisionGroupMask = s_mainCollisionGroupMask_;
    collisionBody2DCreationInfo.physicsType = Project001::CollisionBody2D::PhysicsType::PHYSICS_TYPE_REGULAR_PHYSICS;
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
        mesh01.SetLit(false);

        // renderedMeshes.emplace_back();
        // Project001::RenderedMesh& mesh02 = renderedMeshes.back();
        // mesh02.SetMeshDataPtr(shipCollisionBody_MeshDataPtr_);
        // mesh02.SetLit(false);

        // renderedMeshes.emplace_back();
        // Project001::RenderedMesh& mesh03 = renderedMeshes.back();
        // mesh03.SetMeshDataPtr(shipBeamSight_MeshDataPtr_);
        // mesh03.SetLit(false);
    }

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(player_EntityId_, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, player_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        std::vector<Project001::CollisionTriangle2D>& collisionTriangles = collisionBody2DPtr->GetCollisionTriangles();
        collisionTriangles.emplace_back(glm::vec2(0.0f, 64.0f), glm::vec2(-32.0f, -32.0f), glm::vec2(0.0f, 0.0f));
        collisionTriangles.emplace_back(glm::vec2(0.0f, 0.0f), glm::vec2(32.0f, -32.0f), glm::vec2(0.0f, 64.0f));
    }
}

void TestScene102::UpdateMainCameraEntityPositionAndRoll(unsigned long long timestep_ns)
{
    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCamera_EntityId_));
    if (cameraPtr != nullptr)
    {
        float timestep_s = (float)(timestep_ns / 1000000) / 1000;

        constexpr float translationSpeed_s = 128.0f;
        float cameraTranslationDelta = translationSpeed_s * timestep_s;
        constexpr float pitchSpeed_s = 0.05f * glm::pi<float>();
        float cameraPitchDelta = pitchSpeed_s * timestep_s;

        bool movingLeft = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_LEFT);
        bool movingRight = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_RIGHT);
        bool movingUp = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_UP);
        bool movingDown = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_DOWN);
        bool pitchingUp = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_PAGE_UP);
        bool pitchingDown = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_PAGE_DOWN);

        if (movingLeft)
        {
            cameraPtr->AddTranslationX(-cameraTranslationDelta);
        }

        if (movingRight)
        {
            cameraPtr->AddTranslationX(cameraTranslationDelta);
        }

        if (movingUp)
        {
            cameraPtr->AddTranslationY(cameraTranslationDelta);
        }

        if (movingDown)
        {
            cameraPtr->AddTranslationY(-cameraTranslationDelta);
        }

        if (pitchingUp)
        {
            cameraPtr->AddRelativeRotationX(cameraPitchDelta);
        }

        if (pitchingDown)
        {
            cameraPtr->AddRelativeRotationX(-cameraPitchDelta);
        }
    }
}

void TestScene102::UpdatePreviousWorldCursorPosition(float xPosition, float yPosition)
{
    int windowWidth, windowHeight;
    GetWindowPtr()->GetWindowSize(windowWidth, windowHeight);

    glm::vec2 viewportNormalizedCursorPosition = GetRendererPtr()->ConvertPointFromWindowToViewportNormalized(glm::vec2(xPosition, yPosition), (float)windowHeight);

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
            previousWorldCursorPosition_ = worldCursorPosition;
        }
    }
}