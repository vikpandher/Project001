// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-08-18

#include "Scene001.h"

#include "Resources/cursor_hand_png.h"
#include "Resources/dotted_1_1_png.h"
#include "Resources/dotted_1_3_png.h"
#include "Resources/hazard_4x4_png.h"
#include "Resources/penguin_beak_obj.h"
#include "Resources/penguin_body_obj.h"
#include "Resources/penguin_eyes_obj.h"
#include "Resources/penguin_eyes_v2_obj.h"
#include "Resources/penguin_flipper_left_obj.h"
#include "Resources/penguin_flipper_right_obj.h"
#include "Resources/penguin_foot_left_obj.h"
#include "Resources/penguin_foot_right_obj.h"
#include "Resources/penguin_glasses_obj.h"
#include "Resources/penguin_glasses_v2_obj.h"
#include "Resources/penguin_glasses_v3_obj.h"
#include "Resources/penguin_head_obj.h"
#include "Resources/penguin_texture_png.h"
#include "Resources/penguin_texture_v2_png.h"
#include "Resources/penguin_texture_v3_png.h"
#include "Resources/penguin_texture_v4_png.h"
#include "Resources/shark_back_obj.h"
#include "Resources/shark_front_obj.h"
#include "Resources/shark_jaw_obj.h"
#include "Resources/shark_texture_png.h"
#include "Scene003.h"

#include "Components/Camera.h"
#include "Components/RenderedModel.h"
#include "Resources/PixelFont5x6.h"
#include "Utilities/FontUtility.h"
#include "Utilities/IniReaderWriter.h"
#include "Utilities/MathUtility.h"
#include "Utilities/MeshUtility.h"
#include "Utilities/SoundUtility.h"
#include "Utilities/StringUtility.h"
#include "Utilities/TextureUtility.h"
#include "ComponentStores.h"
#include "Logger.h"
#include "RenderSystem.h"
#include "SoundPlayer.h"
#include "Window.h"

#include <fstream>



// public ----------------------------------------------------------------------

Scene001::Scene001(Project001::Application* applicationPtr)
    : BaseScene001(applicationPtr)
{
    sharedDataPtr_->scene001Id = GetId();

    LoadPixelFontResources();
    LoadGeneralResources();
    LoadMainMenuResources();
    LoadCursorResources();
    LoadImpactResources();
    LoadStageResources();
    LoadStageGridResources();
    LoadActorResources();
    LoadUiResources();
}

Scene001::~Scene001()
{
    FreeResources();

    LOG_INFO("Goodbye World.");
}

void Scene001::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&Scene001::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&Scene001::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&Scene001::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&Scene001::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&Scene001::ProcessUpdateEvent, this, std::placeholders::_1));

    BaseScene001::HandleEvent(event);
}

// protected -------------------------------------------------------------------

void Scene001::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   Scene001:            " << GetId());

    // -------------------------------------------------------------------------

    ReadConfigFile();

    CreateUiCameraEntity(uiCamera_entityId_, s_uiCamera_cameraMask_, 1000);
    CreateKillZoneMesh();
    CreateLoadingTextEntity();
}

void Scene001::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    LOG_INFO("DEINITIALIZING: Scene001:            " << GetId());

    // -------------------------------------------------------------------------

    GetComponentStoresPtr()->DeleteEntity(uiCamera_entityId_);
    uiCamera_entityId_ = static_cast<unsigned int>(-1);

    GetComponentStoresPtr()->DeleteEntity(loadingText_entityId_);
    loadingText_entityId_ = static_cast<unsigned int>(-1);
}

void Scene001::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    sharedDataPtr_->UpdateKeyboardButtonPresses(keyEvent);
}

void Scene001::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    sharedDataPtr_->UpdateMouseButtonPresses(mouseButtonEvent);
}

void Scene001::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    sharedDataPtr_->UpdateButtonPressCounts(GetWindowPtr());

    {
        SendEventToApplication(Project001::SwitchSceneEvent(sharedDataPtr_->scene002Id));
        if (GetActiveScene()->GetId() == sharedDataPtr_->scene002Id)
        {
            SendEventToScene(GetId(), Project001::DeinitializeEvent());
            SendEventToApplication(Project001::InitializeEvent());
        }
        return;
    }
}

void Scene001::LoadPixelFontResources()
{
    sharedDataPtr_->pixelFont_fontDataPtr = &Project001::Get_PixelFont5x6_FontData();
    sharedDataPtr_->pixelFont_textureDataPtr = &Project001::Get_PixelFont5x6_TextureData();
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->pixelFont_textureId,
        sharedDataPtr_->pixelFont_textureDataPtr->data,
        sharedDataPtr_->pixelFont_textureDataPtr->width,
        sharedDataPtr_->pixelFont_textureDataPtr->height,
        sharedDataPtr_->pixelFont_textureDataPtr->bytesPerPixel,
        false,
        false
    );
}

void Scene001::LoadGeneralResources()
{
    sharedDataPtr_->loadingText_meshDataPtr = new Project001::MeshData();

    {
        sharedDataPtr_->circle_meshDataPtr = new Project001::MeshData();
        FAIL_CHECK(Project001::Mesh::Generate2DRegularPolygon(
            *sharedDataPtr_->circle_meshDataPtr, 1.0f, 24
        ));
    }

    {
        sharedDataPtr_->orientationArrow_meshDataPtr = new Project001::MeshData();
        std::vector<glm::vec2> arrowPositions;
        arrowPositions.emplace_back(0.5f, 1.0f);
        arrowPositions.emplace_back(0.0f, 1.5f);
        arrowPositions.emplace_back(-0.5f, 1.0f);
        FAIL_CHECK(Project001::Mesh::Generate2DTriangleFan(
            *sharedDataPtr_->orientationArrow_meshDataPtr, arrowPositions
        ));
    }

    {
        sharedDataPtr_->hallowCircle_meshDataPtr = new Project001::MeshData();
        FAIL_CHECK(Project001::Mesh::Generate2DArc(
            *sharedDataPtr_->hallowCircle_meshDataPtr, 0.9f, 1.0f, 24, 0.0f, 0.0f
        ));
    }

    {
        sharedDataPtr_->player_grabZone_meshDataPtr = new Project001::MeshData();
        FAIL_CHECK(Project001::Mesh::Generate2DArc(
            *sharedDataPtr_->player_grabZone_meshDataPtr, SharedApplicationData::s_penguin_grabRadius - 2.0f, SharedApplicationData::s_penguin_grabRadius, 24, 0.0f, 0.0f
        ));
        float textureSections = glm::two_pi<float>() * SharedApplicationData::s_penguin_grabRadius / 4.0f;
        textureSections = std::roundf(textureSections);
        Project001::Mesh::ScaleTextureCoordinates(*sharedDataPtr_->player_grabZone_meshDataPtr, glm::vec2(textureSections, 1.0f));
        Project001::Mesh::RotateTextureCoordinates(*sharedDataPtr_->player_grabZone_meshDataPtr, glm::half_pi<float>());
        Project001::Mesh::TranslateTextureCoordinates(*sharedDataPtr_->player_grabZone_meshDataPtr, glm::vec2(0.0f, -0.25f));
    }

    sharedDataPtr_->player1_aimRay1_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->player1_aimRay2_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->player2_aimRay1_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->player2_aimRay2_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->player3_aimRay1_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->player3_aimRay2_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->player4_aimRay1_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->player4_aimRay2_meshDataPtr = new Project001::MeshData();

    sharedDataPtr_->dotted_1_1_textureDataPtr = new Project001::TextureData();
    FAIL_CHECK(Project001::Texture::LoadTextureFromMemory(
        *sharedDataPtr_->dotted_1_1_textureDataPtr,
        g_dotted_1_1_png,
        sizeof(g_dotted_1_1_png) / sizeof(unsigned char)
    ));
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->dotted_1_1_textureId,
        sharedDataPtr_->dotted_1_1_textureDataPtr->data,
        sharedDataPtr_->dotted_1_1_textureDataPtr->width,
        sharedDataPtr_->dotted_1_1_textureDataPtr->height,
        sharedDataPtr_->dotted_1_1_textureDataPtr->bytesPerPixel,
        false,
        false
    );

    sharedDataPtr_->dotted_1_3_textureDataPtr = new Project001::TextureData();
    FAIL_CHECK(Project001::Texture::LoadTextureFromMemory(
        *sharedDataPtr_->dotted_1_3_textureDataPtr,
        g_dotted_1_3_png,
        sizeof(g_dotted_1_3_png) / sizeof(unsigned char)
    ));
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->dotted_1_3_textureId,
        sharedDataPtr_->dotted_1_3_textureDataPtr->data,
        sharedDataPtr_->dotted_1_3_textureDataPtr->width,
        sharedDataPtr_->dotted_1_3_textureDataPtr->height,
        sharedDataPtr_->dotted_1_3_textureDataPtr->bytesPerPixel,
        false,
        false
    );
}

void Scene001::LoadMainMenuResources()
{
    sharedDataPtr_->uiMenuBackground_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::Generate2DRectangle(
        *sharedDataPtr_->uiMenuBackground_meshDataPtr, 192.0f, 192.0f
    ));

    sharedDataPtr_->uiMenuAuthorText_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->uiMenuConfigFileFoundText_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->uiMenuLeftText_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->uiMenuRightText_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->uiMenuPlayerText1_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->uiMenuPlayerText2_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->uiMenuPlayerText3_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->uiMenuPlayerText4_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->uiMenuStartText_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->uiMenuTitleText_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->uiMenuVersionText_meshDataPtr = new Project001::MeshData();
}

void Scene001::LoadCursorResources()
{
    sharedDataPtr_->cursorHandOpen_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::Generate2DSprite(
        *sharedDataPtr_->cursorHandOpen_meshDataPtr, 24.0f, 32.0f, 0.0f, 1.0f / 3.0f, 0.0f, 1.0f
    ));

    sharedDataPtr_->cursorHandPointer_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::Generate2DSprite(
        *sharedDataPtr_->cursorHandPointer_meshDataPtr, 24.0f, 32.0f, 1.0f / 3.0f, 2.0f / 3.0f, 0.0f, 1.0f
    ));

    sharedDataPtr_->cursorHandGrab_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::Generate2DSprite(
        *sharedDataPtr_->cursorHandGrab_meshDataPtr, 24.0f, 32.0f, 2.0f / 3.0f, 1.0f, 0.0f, 1.0f
    ));

    sharedDataPtr_->cursor_textureDataPtr = new Project001::TextureData();
    FAIL_CHECK(Project001::Texture::LoadTextureFromMemory(
        *sharedDataPtr_->cursor_textureDataPtr,
        g_cursor_hand_png,
        sizeof(g_cursor_hand_png) / sizeof(unsigned char)
    ));
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->cursor_textureId,
        sharedDataPtr_->cursor_textureDataPtr->data,
        sharedDataPtr_->cursor_textureDataPtr->width,
        sharedDataPtr_->cursor_textureDataPtr->height,
        sharedDataPtr_->cursor_textureDataPtr->bytesPerPixel,
        false,
        false
    );

    sharedDataPtr_->cursor_marker_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::Generate2DArc(
        *sharedDataPtr_->cursor_marker_meshDataPtr, 2.0f, 4.0f, 8, 0.0f, 0.0f
    ));

    sharedDataPtr_->cursor_aimRay1_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->cursor_aimRay2_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->cursor_aimRay3_meshDataPtr = new Project001::MeshData();
}

void Scene001::LoadImpactResources()
{
    for (size_t i = 0; i < 4; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        FAIL_CHECK(Project001::Mesh::Generate2DStarRing(
            *newMeshDataPtr, 8,
            0.3f, 0.0f, 0.2f, 0.0f,
            i,
            0.2f, 0.0f, 0.1f, 0.0f
        ));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
        Project001::Mesh::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        sharedDataPtr_->impactFrame_meshDataPtrs[0][i] = newMeshDataPtr;
    }

    for (size_t i = 0; i < 4; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        FAIL_CHECK(Project001::Mesh::Generate2DStarRing(
            *newMeshDataPtr, 10,
            0.6f, 0.2f, 0.4f, 0.2f,
            i,
            0.4f, 0.1f, 0.3f, 0.1f
        ));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
        Project001::Mesh::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        sharedDataPtr_->impactFrame_meshDataPtrs[1][i] = newMeshDataPtr;
    }

    for (size_t i = 0; i < 4; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        FAIL_CHECK(Project001::Mesh::Generate2DStarRing(
            *newMeshDataPtr, 10,
            0.8f, 0.4f, 0.7f, 0.3f,
            i,
            0.2f, 0.1f, 0.3f, 0.1f
        ));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
        Project001::Mesh::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        sharedDataPtr_->impactFrame_meshDataPtrs[2][i] = newMeshDataPtr;
    }

    for (size_t i = 0; i < 4; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        FAIL_CHECK(Project001::Mesh::Generate2DStarBurst(
            *newMeshDataPtr, 10,
            0.8f, 0.5f, 1.0f, 0.8f,
            glm::pi<float>() * 0.1f,
            i,
            0.4f, 0.1f, 0.4f, 0.4f
        ));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
        Project001::Mesh::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        sharedDataPtr_->impactFrame_meshDataPtrs[3][i] = newMeshDataPtr;
    }
}

void Scene001::LoadStageResources()
{
    sharedDataPtr_->ground_meshDataPtr = new Project001::MeshData();

    sharedDataPtr_->water_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::Generate2DSprite(
        *sharedDataPtr_->water_meshDataPtr,
        sharedDataPtr_->groundApothem * 24.0f,
        sharedDataPtr_->groundApothem * 24.0f,
        0.0f, 1.0f, 0.0f, 1.0f
    ));

    // std::vector<glm::vec2> groundCollisionCorners;
    // groundCollisionCorners.reserve(8);
    // groundCollisionCorners.emplace_back(SharedApplicationData::s_ground_size, groundCorner);
    // groundCollisionCorners.emplace_back(groundCorner, SharedApplicationData::s_ground_size);
    // groundCollisionCorners.emplace_back(-groundCorner, SharedApplicationData::s_ground_size);
    // groundCollisionCorners.emplace_back(-SharedApplicationData::s_ground_size, groundCorner);
    // groundCollisionCorners.emplace_back(-SharedApplicationData::s_ground_size, -groundCorner);
    // groundCollisionCorners.emplace_back(-groundCorner, -SharedApplicationData::s_ground_size);
    // groundCollisionCorners.emplace_back(groundCorner, -SharedApplicationData::s_ground_size);
    // groundCollisionCorners.emplace_back(SharedApplicationData::s_ground_size, -groundCorner);
    // sharedDataPtr_->groundCollision_meshDataPtr = new Project001::MeshData();
    // FAIL_CHECK(Project001::Mesh::Generate2DTriangleFan(
    //     *sharedDataPtr_->groundCollision_meshDataPtr,
    //     groundCollisionCorners
    // ));

    sharedDataPtr_->deadZone_meshDataPtr = new Project001::MeshData();

    sharedDataPtr_->hazard_textureDataPtr = new Project001::TextureData();
    FAIL_CHECK(Project001::Texture::LoadTextureFromMemory(
        *sharedDataPtr_->hazard_textureDataPtr,
        g_hazard_4x4_png,
        sizeof(g_hazard_4x4_png) / sizeof(unsigned char)
    ));
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->hazard_textureId,
        sharedDataPtr_->hazard_textureDataPtr->data,
        sharedDataPtr_->hazard_textureDataPtr->width,
        sharedDataPtr_->hazard_textureDataPtr->height,
        sharedDataPtr_->hazard_textureDataPtr->bytesPerPixel,
        false,
        false
    );
}

void Scene001::LoadStageGridResources()
{
    sharedDataPtr_->stageGrid_meshDataPtr = new Project001::MeshData();

    constexpr float gridSpacing = 32.0f;
    constexpr float gridSize = 16.0f * gridSpacing;
    constexpr float lineWidth = 2.0f;

    for (float f = -gridSize; f < gridSize + 1.0f; f += gridSpacing)
    {
        FAIL_CHECK(Project001::Mesh::Generate2DLine(
            *sharedDataPtr_->stageGrid_meshDataPtr,
            glm::vec2(-gridSize - 0.5f * lineWidth, f),
            glm::vec2(gridSize + 0.5f * lineWidth, f),
            lineWidth
        ));
    }
    for (float f = -gridSize; f < gridSize + 1.0f; f += gridSpacing)
    {
        for (float g = -gridSize; g < gridSize - 1.0f; g += gridSpacing)
        {
            FAIL_CHECK(Project001::Mesh::Generate2DLine(
                *sharedDataPtr_->stageGrid_meshDataPtr,
                glm::vec2(f, g + 0.5f * lineWidth),
                glm::vec2(f, g - 0.5f * lineWidth + gridSpacing),
                lineWidth
            ));
        }
    }

    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->stageGrid_meshId,
        sharedDataPtr_->stageGrid_meshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->stageGrid_meshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->stageGrid_meshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->stageGrid_meshDataPtr->meshIndexArray.size())
    );

    sharedDataPtr_->stageGridLabels_meshDataPtr = new Project001::MeshData();

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
    float gridLabel_offsetY = pixelFont_pixelSize * 1.0f + 0.5f * lineWidth;
    float gridLabel_offsetX = pixelFont_pixelSize * 1.0f + 0.5f * lineWidth;

    for (size_t i = 0; i < gridLabels.size(); ++i)
    {
        const std::string& currentLabel = gridLabels[i];

        Project001::MeshData currentLabelMeshData;
        FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
            currentLabelMeshData,
            *sharedDataPtr_->pixelFont_fontDataPtr,
            currentLabel,
            pixelFont_pixelSize
        ));
        Project001::Mesh::TranslateMesh(
            currentLabelMeshData,
            glm::vec3(gridLabel_offsetX, gridLabel_offsetY, 0.0f)
        );

        Project001::Mesh::CopyMesh(*sharedDataPtr_->stageGridLabels_meshDataPtr, currentLabelMeshData);

        gridLabel_offsetX += gridSpacing;
    }

    // positive y-axis -----------------------------------------------------

    gridLabel_offsetY = pixelFont_pixelSize * 1.0f + 0.5f * lineWidth + gridSpacing;
    gridLabel_offsetX = pixelFont_pixelSize * 1.0f + 0.5f * lineWidth;

    for (size_t i = 1; i < gridLabels.size(); ++i)
    {
        const std::string& currentLabel = gridLabels[i];

        Project001::MeshData currentLabelMeshData;
        FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
            currentLabelMeshData,
            *sharedDataPtr_->pixelFont_fontDataPtr,
            currentLabel,
            pixelFont_pixelSize
        ));
        Project001::Mesh::TranslateMesh(
            currentLabelMeshData,
            glm::vec3(gridLabel_offsetX, gridLabel_offsetY, 0.0f)
        );

        Project001::Mesh::CopyMesh(*sharedDataPtr_->stageGridLabels_meshDataPtr, currentLabelMeshData);

        gridLabel_offsetY += gridSpacing;
    }

    // negative x-axis -----------------------------------------------------

    gridLabel_offsetY = pixelFont_pixelSize * -7.0f - 0.5f * lineWidth;
    gridLabel_offsetX = pixelFont_pixelSize * -6.0f - 0.5f * lineWidth;

    for (size_t i = 0; i < gridLabels.size(); ++i)
    {
        const std::string& currentLabel = gridLabels[i];

        float addition_offsetX = pixelFont_pixelSize * -6.0f * static_cast<float>(currentLabel.length() - 1);

        Project001::MeshData currentLabelMeshData;
        FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
            currentLabelMeshData,
            *sharedDataPtr_->pixelFont_fontDataPtr,
            currentLabel,
            pixelFont_pixelSize
        ));
        Project001::Mesh::TranslateMesh(
            currentLabelMeshData,
            glm::vec3(gridLabel_offsetX + addition_offsetX, gridLabel_offsetY, 0.0f)
        );

        Project001::Mesh::CopyMesh(*sharedDataPtr_->stageGridLabels_meshDataPtr, currentLabelMeshData);

        gridLabel_offsetX -= gridSpacing;
    }

    // negative y-axis -----------------------------------------------------

    gridLabel_offsetY = pixelFont_pixelSize * -7.0f - 0.5f * lineWidth - gridSpacing;
    gridLabel_offsetX = pixelFont_pixelSize * -6.0f - 0.5f * lineWidth;

    for (size_t i = 1; i < gridLabels.size(); ++i)
    {
        const std::string& currentLabel = gridLabels[i];

        float addition_offsetX = pixelFont_pixelSize * -6.0f * static_cast<float>(currentLabel.length() - 1);

        Project001::MeshData currentLabelMeshData;
        FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
            currentLabelMeshData,
            *sharedDataPtr_->pixelFont_fontDataPtr,
            currentLabel,
            pixelFont_pixelSize
        ));
        Project001::Mesh::TranslateMesh(
            currentLabelMeshData,
            glm::vec3(gridLabel_offsetX + addition_offsetX, gridLabel_offsetY, 0.0f)
        );

        Project001::Mesh::CopyMesh(*sharedDataPtr_->stageGridLabels_meshDataPtr, currentLabelMeshData);

        gridLabel_offsetY -= gridSpacing;
    }

    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->stageGridLabels_meshId,
        sharedDataPtr_->stageGridLabels_meshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->stageGridLabels_meshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->stageGridLabels_meshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->stageGridLabels_meshDataPtr->meshIndexArray.size())
    );

    sharedDataPtr_->stageCollisionQuadTree_meshDataPtr = new Project001::MeshData();
}

void Scene001::LoadActorResources()
{
    sharedDataPtr_->penguin_body_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::LoadMeshOBJFromMemory(
        *sharedDataPtr_->penguin_body_meshDataPtr,
        g_penguin_body_obj
    ));
    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->penguin_body_meshId,
        sharedDataPtr_->penguin_body_meshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_body_meshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->penguin_body_meshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_body_meshDataPtr->meshIndexArray.size())
    );

    sharedDataPtr_->penguin_flipper_right_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::LoadMeshOBJFromMemory(
        *sharedDataPtr_->penguin_flipper_right_meshDataPtr,
        g_penguin_flipper_right_obj
    ));
    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->penguin_flipper_right_meshId,
        sharedDataPtr_->penguin_flipper_right_meshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_flipper_right_meshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->penguin_flipper_right_meshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_flipper_right_meshDataPtr->meshIndexArray.size())
    );

    sharedDataPtr_->penguin_flipper_left_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::LoadMeshOBJFromMemory(
        *sharedDataPtr_->penguin_flipper_left_meshDataPtr,
        g_penguin_flipper_left_obj
    ));
    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->penguin_flipper_left_meshId,
        sharedDataPtr_->penguin_flipper_left_meshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_flipper_left_meshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->penguin_flipper_left_meshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_flipper_left_meshDataPtr->meshIndexArray.size())
    );

    sharedDataPtr_->penguin_foot_right_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::LoadMeshOBJFromMemory(
        *sharedDataPtr_->penguin_foot_right_meshDataPtr,
        g_penguin_foot_right_obj
    ));
    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->penguin_foot_right_meshId,
        sharedDataPtr_->penguin_foot_right_meshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_foot_right_meshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->penguin_foot_right_meshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_foot_right_meshDataPtr->meshIndexArray.size())
    );

    sharedDataPtr_->penguin_foot_left_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::LoadMeshOBJFromMemory(
        *sharedDataPtr_->penguin_foot_left_meshDataPtr,
        g_penguin_foot_left_obj
    ));
    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->penguin_foot_left_meshId,
        sharedDataPtr_->penguin_foot_left_meshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_foot_left_meshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->penguin_foot_left_meshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_foot_left_meshDataPtr->meshIndexArray.size())
    );

    sharedDataPtr_->penguin_head_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::LoadMeshOBJFromMemory(
        *sharedDataPtr_->penguin_head_meshDataPtr,
        g_penguin_head_obj
    ));
    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->penguin_head_meshId,
        sharedDataPtr_->penguin_head_meshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_head_meshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->penguin_head_meshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_head_meshDataPtr->meshIndexArray.size())
    );

    sharedDataPtr_->penguin_eyes_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::LoadMeshOBJFromMemory(
        *sharedDataPtr_->penguin_eyes_meshDataPtr,
        g_penguin_eyes_obj
    ));
    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->penguin_eyes_meshId,
        sharedDataPtr_->penguin_eyes_meshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_eyes_meshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->penguin_eyes_meshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_eyes_meshDataPtr->meshIndexArray.size())
    );

    sharedDataPtr_->penguin_eyes_v2_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::LoadMeshOBJFromMemory(
        *sharedDataPtr_->penguin_eyes_v2_meshDataPtr,
        g_penguin_eyes_v2_obj
    ));
    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->penguin_eyes_v2_meshId,
        sharedDataPtr_->penguin_eyes_v2_meshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_eyes_v2_meshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->penguin_eyes_v2_meshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_eyes_v2_meshDataPtr->meshIndexArray.size())
    );

    sharedDataPtr_->penguin_beak_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::LoadMeshOBJFromMemory(
        *sharedDataPtr_->penguin_beak_meshDataPtr,
        g_penguin_beak_obj
    ));
    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->penguin_beak_meshId,
        sharedDataPtr_->penguin_beak_meshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_beak_meshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->penguin_beak_meshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_beak_meshDataPtr->meshIndexArray.size())
    );

    sharedDataPtr_->penguin_glasses_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::LoadMeshOBJFromMemory(
        *sharedDataPtr_->penguin_glasses_meshDataPtr,
        g_penguin_glasses_obj
    ));
    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->penguin_glasses_meshId,
        sharedDataPtr_->penguin_glasses_meshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_glasses_meshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->penguin_glasses_meshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_glasses_meshDataPtr->meshIndexArray.size())
    );

    sharedDataPtr_->penguin_glasses_v2_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::LoadMeshOBJFromMemory(
        *sharedDataPtr_->penguin_glasses_v2_meshDataPtr, 
        g_penguin_glasses_v2_obj
    ));
    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->penguin_glasses_v2_meshId,
        sharedDataPtr_->penguin_glasses_v2_meshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_glasses_v2_meshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->penguin_glasses_v2_meshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_glasses_v2_meshDataPtr->meshIndexArray.size())
    );

    sharedDataPtr_->penguin_glasses_v3_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::LoadMeshOBJFromMemory(
        *sharedDataPtr_->penguin_glasses_v3_meshDataPtr, 
        g_penguin_glasses_v3_obj
    ));
    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->penguin_glasses_v3_meshId,
        sharedDataPtr_->penguin_glasses_v3_meshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_glasses_v3_meshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->penguin_glasses_v3_meshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->penguin_glasses_v3_meshDataPtr->meshIndexArray.size())
    );

    sharedDataPtr_->penguin_textureDataPtr = new Project001::TextureData();
    FAIL_CHECK(Project001::Texture::LoadTextureFromMemory(
        *sharedDataPtr_->penguin_textureDataPtr,
        g_penguin_texture_png,
        sizeof(g_penguin_texture_png) / sizeof(unsigned char)
    ));
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->penguin_textureId,
        sharedDataPtr_->penguin_textureDataPtr->data,
        sharedDataPtr_->penguin_textureDataPtr->width,
        sharedDataPtr_->penguin_textureDataPtr->height,
        sharedDataPtr_->penguin_textureDataPtr->bytesPerPixel,
        false,
        false
    );

    sharedDataPtr_->penguin_v2_textureDataPtr = new Project001::TextureData();
    FAIL_CHECK(Project001::Texture::LoadTextureFromMemory(
        *sharedDataPtr_->penguin_v2_textureDataPtr,
        g_penguin_texture_v2_png,
        sizeof(g_penguin_texture_v2_png) / sizeof(unsigned char)
    ));
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->penguin_v2_textureId,
        sharedDataPtr_->penguin_v2_textureDataPtr->data,
        sharedDataPtr_->penguin_v2_textureDataPtr->width,
        sharedDataPtr_->penguin_v2_textureDataPtr->height,
        sharedDataPtr_->penguin_v2_textureDataPtr->bytesPerPixel,
        false,
        false
    );

    sharedDataPtr_->penguin_v3_textureDataPtr = new Project001::TextureData();
    FAIL_CHECK(Project001::Texture::LoadTextureFromMemory(
        *sharedDataPtr_->penguin_v3_textureDataPtr,
        g_penguin_texture_v3_png,
        sizeof(g_penguin_texture_v3_png) / sizeof(unsigned char)
    ));
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->penguin_v3_textureId,
        sharedDataPtr_->penguin_v3_textureDataPtr->data,
        sharedDataPtr_->penguin_v3_textureDataPtr->width,
        sharedDataPtr_->penguin_v3_textureDataPtr->height,
        sharedDataPtr_->penguin_v3_textureDataPtr->bytesPerPixel,
        false,
        false
    );

    sharedDataPtr_->penguin_v4_textureDataPtr = new Project001::TextureData();
    FAIL_CHECK(Project001::Texture::LoadTextureFromMemory(
        *sharedDataPtr_->penguin_v4_textureDataPtr,
        g_penguin_texture_v4_png,
        sizeof(g_penguin_texture_v4_png) / sizeof(unsigned char)
    ));
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->penguin_v4_textureId,
        sharedDataPtr_->penguin_v4_textureDataPtr->data,
        sharedDataPtr_->penguin_v4_textureDataPtr->width,
        sharedDataPtr_->penguin_v4_textureDataPtr->height,
        sharedDataPtr_->penguin_v4_textureDataPtr->bytesPerPixel,
        false,
        false
    );

    sharedDataPtr_->snowball_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::GenerateIcosphere(
        *sharedDataPtr_->snowball_meshDataPtr, 1.0f, 1, false)
    );
    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->snowball_meshId,
        sharedDataPtr_->snowball_meshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->snowball_meshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->snowball_meshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->snowball_meshDataPtr->meshIndexArray.size())
    );

    sharedDataPtr_->shark_back_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::LoadMeshOBJFromMemory(
        *sharedDataPtr_->shark_back_meshDataPtr,
        g_shark_back_obj
    ));
    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->shark_back_meshId,
        sharedDataPtr_->shark_back_meshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->shark_back_meshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->shark_back_meshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->shark_back_meshDataPtr->meshIndexArray.size())
    );

    sharedDataPtr_->shark_front_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::LoadMeshOBJFromMemory(
        *sharedDataPtr_->shark_front_meshDataPtr,
        g_shark_front_obj
    ));
    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->shark_front_meshId,
        sharedDataPtr_->shark_front_meshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->shark_front_meshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->shark_front_meshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->shark_front_meshDataPtr->meshIndexArray.size())
    );

    sharedDataPtr_->shark_jaw_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::LoadMeshOBJFromMemory(
        *sharedDataPtr_->shark_jaw_meshDataPtr,
        g_shark_jaw_obj
    ));
    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->shark_jaw_meshId,
        sharedDataPtr_->shark_jaw_meshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->shark_jaw_meshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->shark_jaw_meshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->shark_jaw_meshDataPtr->meshIndexArray.size())
    );

    sharedDataPtr_->shark_textureDataPtr = new Project001::TextureData();
    FAIL_CHECK(Project001::Texture::LoadTextureFromMemory(
        *sharedDataPtr_->shark_textureDataPtr,
        g_shark_texture_png,
        sizeof(g_shark_texture_png) / sizeof(unsigned char)
    ));
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->shark_textureId,
        sharedDataPtr_->shark_textureDataPtr->data,
        sharedDataPtr_->shark_textureDataPtr->width,
        sharedDataPtr_->shark_textureDataPtr->height,
        sharedDataPtr_->shark_textureDataPtr->bytesPerPixel,
        false,
        false
    );

    sharedDataPtr_->shark_back_collision_meshDataPtr = new Project001::MeshData();
    std::vector<glm::vec2> shark_back_collision_positions;
    shark_back_collision_positions.reserve(3);
    shark_back_collision_positions.emplace_back(30.0f, 14.0f);
    shark_back_collision_positions.emplace_back(-30.0f, 14.0f);
    shark_back_collision_positions.emplace_back(0.0f, -130.0f);
    FAIL_CHECK(Project001::Mesh::Generate2DTriangles(
        *sharedDataPtr_->shark_back_collision_meshDataPtr,
        shark_back_collision_positions
    ));

    sharedDataPtr_->shark_front_collision_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::Generate2DRectangle(
        *sharedDataPtr_->shark_front_collision_meshDataPtr,
        60.0f, 68.0f
    ));
    Project001::Mesh::TranslateMesh(
        *sharedDataPtr_->shark_front_collision_meshDataPtr,
        glm::vec3(0.0f, 48.0f, 0.0f)
    );

    sharedDataPtr_->shark_jaw_collision_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::Generate2DRegularPolygon(
        *sharedDataPtr_->shark_jaw_collision_meshDataPtr,
        32.0f, 16
    ));
    Project001::Mesh::TranslateMesh(
        *sharedDataPtr_->shark_jaw_collision_meshDataPtr,
        glm::vec3(0.0f, 80.0f, 0.0f)
    );

    sharedDataPtr_->shark_attackRay1_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->shark_attackRay2_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->shark_attackRay3_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->shark_attackRay4_meshDataPtr = new Project001::MeshData();
}

void Scene001::LoadUiResources()
{
    sharedDataPtr_->uiPauseBackground_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::Generate2DRectangle(
        *sharedDataPtr_->uiPauseBackground_meshDataPtr,
        360.0f, 360.0f
    ));

    sharedDataPtr_->uiPauseTitleText_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->uiPauseText01_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->uiPauseText02_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->uiGameOverTitleText_meshDataPtr = new Project001::MeshData();
}

void Scene001::FreeResources()
{
    GetRendererPtr()->DeleteAllTextures();
    GetRendererPtr()->DeleteAllMeshes();
    GetSoundPlayerPtr()->DeleteAllSoundBuffers();
    GetSoundPlayerPtr()->DeleteAllSoundSources();

    // General Resources
    delete sharedDataPtr_->loadingText_meshDataPtr;
    sharedDataPtr_->loadingText_meshDataPtr = nullptr;
    delete sharedDataPtr_->circle_meshDataPtr;
    sharedDataPtr_->circle_meshDataPtr = nullptr;
    delete sharedDataPtr_->orientationArrow_meshDataPtr;
    sharedDataPtr_->orientationArrow_meshDataPtr = nullptr;
    delete sharedDataPtr_->hallowCircle_meshDataPtr;
    sharedDataPtr_->hallowCircle_meshDataPtr = nullptr;

    delete sharedDataPtr_->player_grabZone_meshDataPtr;
    sharedDataPtr_->player_grabZone_meshDataPtr = nullptr;

    delete sharedDataPtr_->player1_aimRay1_meshDataPtr;
    sharedDataPtr_->player1_aimRay1_meshDataPtr = nullptr;
    delete sharedDataPtr_->player1_aimRay2_meshDataPtr;
    sharedDataPtr_->player1_aimRay2_meshDataPtr = nullptr;
    delete sharedDataPtr_->player2_aimRay1_meshDataPtr;
    sharedDataPtr_->player2_aimRay1_meshDataPtr = nullptr;
    delete sharedDataPtr_->player2_aimRay2_meshDataPtr;
    sharedDataPtr_->player2_aimRay2_meshDataPtr = nullptr;
    delete sharedDataPtr_->player3_aimRay1_meshDataPtr;
    sharedDataPtr_->player3_aimRay1_meshDataPtr = nullptr;
    delete sharedDataPtr_->player3_aimRay2_meshDataPtr;
    sharedDataPtr_->player3_aimRay2_meshDataPtr = nullptr;
    delete sharedDataPtr_->player4_aimRay1_meshDataPtr;
    sharedDataPtr_->player4_aimRay1_meshDataPtr = nullptr;
    delete sharedDataPtr_->player4_aimRay2_meshDataPtr;
    sharedDataPtr_->player4_aimRay2_meshDataPtr = nullptr;

    delete sharedDataPtr_->dotted_1_1_textureDataPtr;
    sharedDataPtr_->dotted_1_1_textureDataPtr = nullptr;
    sharedDataPtr_->dotted_1_1_textureId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->dotted_1_3_textureDataPtr;
    sharedDataPtr_->dotted_1_3_textureDataPtr = nullptr;
    sharedDataPtr_->dotted_1_3_textureId = static_cast<unsigned int>(-1);

    // Main Menu Resources
    delete sharedDataPtr_->uiMenuBackground_meshDataPtr;
    sharedDataPtr_->uiMenuBackground_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiMenuAuthorText_meshDataPtr;
    sharedDataPtr_->uiMenuAuthorText_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiMenuConfigFileFoundText_meshDataPtr;
    sharedDataPtr_->uiMenuConfigFileFoundText_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiMenuLeftText_meshDataPtr;
    sharedDataPtr_->uiMenuLeftText_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiMenuRightText_meshDataPtr;
    sharedDataPtr_->uiMenuRightText_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiMenuPlayerText1_meshDataPtr;
    sharedDataPtr_->uiMenuPlayerText1_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiMenuPlayerText2_meshDataPtr;
    sharedDataPtr_->uiMenuPlayerText2_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiMenuPlayerText3_meshDataPtr;
    sharedDataPtr_->uiMenuPlayerText3_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiMenuPlayerText4_meshDataPtr;
    sharedDataPtr_->uiMenuPlayerText4_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiMenuStartText_meshDataPtr;
    sharedDataPtr_->uiMenuStartText_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiMenuTitleText_meshDataPtr;
    sharedDataPtr_->uiMenuTitleText_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiMenuVersionText_meshDataPtr;
    sharedDataPtr_->uiMenuVersionText_meshDataPtr = nullptr;

    // Cursor Resources
    delete sharedDataPtr_->cursorHandOpen_meshDataPtr;
    sharedDataPtr_->cursorHandOpen_meshDataPtr = nullptr;
    delete sharedDataPtr_->cursorHandPointer_meshDataPtr;
    sharedDataPtr_->cursorHandPointer_meshDataPtr = nullptr;
    delete sharedDataPtr_->cursorHandGrab_meshDataPtr;
    sharedDataPtr_->cursorHandGrab_meshDataPtr = nullptr;
    delete sharedDataPtr_->cursor_textureDataPtr;
    sharedDataPtr_->cursor_textureDataPtr = nullptr;
    sharedDataPtr_->cursor_textureId = static_cast<unsigned int>(-1);

    delete sharedDataPtr_->cursor_marker_meshDataPtr;
    sharedDataPtr_->cursor_marker_meshDataPtr = nullptr;
    delete sharedDataPtr_->cursor_aimRay1_meshDataPtr;
    sharedDataPtr_->cursor_aimRay1_meshDataPtr = nullptr;
    delete sharedDataPtr_->cursor_aimRay2_meshDataPtr;
    sharedDataPtr_->cursor_aimRay2_meshDataPtr = nullptr;
    delete sharedDataPtr_->cursor_aimRay3_meshDataPtr;
    sharedDataPtr_->cursor_aimRay3_meshDataPtr = nullptr;

    // Impact Resources
    for (size_t j = 0; j < 4; ++j)
    {
        for (size_t i = 0; i < 4; ++i)
        {
            delete sharedDataPtr_->impactFrame_meshDataPtrs[i][j];
            sharedDataPtr_->impactFrame_meshDataPtrs[i][j] = nullptr;
        }
    }

    // Stage Resources
    delete sharedDataPtr_->ground_meshDataPtr;
    sharedDataPtr_->ground_meshDataPtr = nullptr;

    delete sharedDataPtr_->water_meshDataPtr;
    sharedDataPtr_->water_meshDataPtr = nullptr;

    delete sharedDataPtr_->deadZone_meshDataPtr;
    sharedDataPtr_->deadZone_meshDataPtr = nullptr;

    delete sharedDataPtr_->hazard_textureDataPtr;
    sharedDataPtr_->hazard_textureDataPtr = nullptr;
    sharedDataPtr_->hazard_textureId = static_cast<unsigned int>(-1);

    // Stage Grid Resources
    delete sharedDataPtr_->stageGrid_meshDataPtr;
    sharedDataPtr_->stageGrid_meshDataPtr = nullptr;
    sharedDataPtr_->stageGrid_meshId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->stageGridLabels_meshDataPtr;
    sharedDataPtr_->stageGridLabels_meshDataPtr = nullptr;
    sharedDataPtr_->stageGridLabels_meshId = static_cast<unsigned int>(-1);

    delete sharedDataPtr_->stageCollisionQuadTree_meshDataPtr;
    sharedDataPtr_->stageCollisionQuadTree_meshDataPtr = nullptr;

    // Actor Resources
    delete sharedDataPtr_->penguin_body_meshDataPtr;
    sharedDataPtr_->penguin_body_meshDataPtr = nullptr;
    sharedDataPtr_->penguin_body_meshId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->penguin_flipper_right_meshDataPtr;
    sharedDataPtr_->penguin_flipper_right_meshDataPtr = nullptr;
    sharedDataPtr_->penguin_flipper_right_meshId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->penguin_flipper_left_meshDataPtr;
    sharedDataPtr_->penguin_flipper_left_meshDataPtr = nullptr;
    sharedDataPtr_->penguin_flipper_left_meshId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->penguin_foot_right_meshDataPtr;
    sharedDataPtr_->penguin_foot_right_meshDataPtr = nullptr;
    sharedDataPtr_->penguin_foot_right_meshId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->penguin_foot_left_meshDataPtr;
    sharedDataPtr_->penguin_foot_left_meshDataPtr = nullptr;
    sharedDataPtr_->penguin_foot_left_meshId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->penguin_head_meshDataPtr;
    sharedDataPtr_->penguin_head_meshDataPtr = nullptr;
    sharedDataPtr_->penguin_head_meshId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->penguin_eyes_meshDataPtr;
    sharedDataPtr_->penguin_eyes_meshDataPtr = nullptr;
    sharedDataPtr_->penguin_eyes_meshId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->penguin_eyes_v2_meshDataPtr;
    sharedDataPtr_->penguin_eyes_v2_meshDataPtr = nullptr;
    sharedDataPtr_->penguin_eyes_v2_meshId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->penguin_beak_meshDataPtr;
    sharedDataPtr_->penguin_beak_meshDataPtr = nullptr;
    sharedDataPtr_->penguin_beak_meshId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->penguin_glasses_meshDataPtr;
    sharedDataPtr_->penguin_glasses_meshDataPtr = nullptr;
    sharedDataPtr_->penguin_glasses_meshId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->penguin_glasses_v2_meshDataPtr;
    sharedDataPtr_->penguin_glasses_v2_meshDataPtr = nullptr;
    sharedDataPtr_->penguin_glasses_v2_meshId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->penguin_glasses_v3_meshDataPtr;
    sharedDataPtr_->penguin_glasses_v3_meshDataPtr = nullptr;
    sharedDataPtr_->penguin_glasses_v3_meshId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->penguin_textureDataPtr;
    sharedDataPtr_->penguin_textureDataPtr = nullptr;
    sharedDataPtr_->penguin_textureId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->penguin_v2_textureDataPtr;
    sharedDataPtr_->penguin_v2_textureDataPtr = nullptr;
    sharedDataPtr_->penguin_v2_textureId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->penguin_v3_textureDataPtr;
    sharedDataPtr_->penguin_v3_textureDataPtr = nullptr;
    sharedDataPtr_->penguin_v3_textureId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->penguin_v4_textureDataPtr;
    sharedDataPtr_->penguin_v4_textureDataPtr = nullptr;
    sharedDataPtr_->penguin_v4_textureId = static_cast<unsigned int>(-1);

    delete sharedDataPtr_->snowball_meshDataPtr;
    sharedDataPtr_->snowball_meshDataPtr = nullptr;
    sharedDataPtr_->snowball_meshId = static_cast<unsigned int>(-1);

    delete sharedDataPtr_->shark_back_meshDataPtr;
    sharedDataPtr_->shark_back_meshDataPtr = nullptr;
    sharedDataPtr_->shark_back_meshId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->shark_front_meshDataPtr;
    sharedDataPtr_->shark_front_meshDataPtr = nullptr;
    sharedDataPtr_->shark_front_meshId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->shark_jaw_meshDataPtr;
    sharedDataPtr_->shark_jaw_meshDataPtr = nullptr;
    sharedDataPtr_->shark_jaw_meshId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->shark_textureDataPtr;
    sharedDataPtr_->shark_textureDataPtr = nullptr;
    sharedDataPtr_->shark_textureId = static_cast<unsigned int>(-1);

    delete sharedDataPtr_->shark_back_collision_meshDataPtr;
    sharedDataPtr_->shark_back_collision_meshDataPtr = nullptr;
    delete sharedDataPtr_->shark_front_collision_meshDataPtr;
    sharedDataPtr_->shark_front_collision_meshDataPtr = nullptr;
    delete sharedDataPtr_->shark_jaw_collision_meshDataPtr;
    sharedDataPtr_->shark_jaw_collision_meshDataPtr = nullptr;
    delete sharedDataPtr_->shark_attackRay1_meshDataPtr;
    sharedDataPtr_->shark_attackRay1_meshDataPtr = nullptr;
    delete sharedDataPtr_->shark_attackRay2_meshDataPtr;
    sharedDataPtr_->shark_attackRay2_meshDataPtr = nullptr;
    delete sharedDataPtr_->shark_attackRay3_meshDataPtr;
    sharedDataPtr_->shark_attackRay3_meshDataPtr = nullptr;
    delete sharedDataPtr_->shark_attackRay4_meshDataPtr;
    sharedDataPtr_->shark_attackRay4_meshDataPtr = nullptr;

    // Ui Resources
    delete sharedDataPtr_->uiPauseBackground_meshDataPtr;
    sharedDataPtr_->uiPauseBackground_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiPauseTitleText_meshDataPtr;
    sharedDataPtr_->uiPauseTitleText_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiPauseText01_meshDataPtr;
    sharedDataPtr_->uiPauseText01_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiPauseText02_meshDataPtr;
    sharedDataPtr_->uiPauseText02_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiGameOverTitleText_meshDataPtr;
    sharedDataPtr_->uiGameOverTitleText_meshDataPtr = nullptr;
}

void Scene001::ReadConfigFile()
{
    std::ifstream inputStream("CoolPenguinSnowball.ini");

    sharedDataPtr_->configFileFound_ = false;
    if (inputStream.is_open())
    {
        sharedDataPtr_->configFileFound_ = true;

        std::map<std::string, std::map<std::string, std::string>> sections;
        Project001::ReadIniStream(sections, inputStream);

        std::map<std::string, std::map<std::string, std::string>>::const_iterator iter = sections.find("Player_1");
        if (iter != sections.end())
        {
            std::map<std::string, std::string>::const_iterator iter2 = iter->second.find("turnedOn");
            if (iter2 != iter->second.end())
            {
                bool result = false;
                if (Project001::String::StringToBool(iter2->second, result))
                {
                    sharedDataPtr_->playerCreationInfos[0].turnedOn = result;
                }
            }

            iter2 = iter->second.find("controlScheme");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerCreationInfos[0].controlSchemeIndex = ControlSchemeInfo::StringToControlSchemeIndex(iter2->second);
            }

            iter2 = iter->second.find("spawnPositionX");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerCreationInfos[0].spawnPositionX = std::stof(iter2->second);
            }

            iter2 = iter->second.find("spawnPositionY");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerCreationInfos[0].spawnPositionY = std::stof(iter2->second);
            }

            iter2 = iter->second.find("spawnRotation");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerCreationInfos[0].spawnRotation = std::stof(iter2->second);
            }
        }

        iter = sections.find("Player_2");
        if (iter != sections.end())
        {
            std::map<std::string, std::string>::const_iterator iter2 = iter->second.find("turnedOn");
            if (iter2 != iter->second.end())
            {
                bool result = false;
                if (Project001::String::StringToBool(iter2->second, result))
                {
                    sharedDataPtr_->playerCreationInfos[1].turnedOn = result;
                }
            }

            iter2 = iter->second.find("controlScheme");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerCreationInfos[1].controlSchemeIndex = ControlSchemeInfo::StringToControlSchemeIndex(iter2->second);
            }

            iter2 = iter->second.find("spawnPositionX");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerCreationInfos[1].spawnPositionX = std::stof(iter2->second);
            }

            iter2 = iter->second.find("spawnPositionY");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerCreationInfos[1].spawnPositionY = std::stof(iter2->second);
            }

            iter2 = iter->second.find("spawnRotation");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerCreationInfos[1].spawnRotation = std::stof(iter2->second);
            }
        }

        iter = sections.find("Player_3");
        if (iter != sections.end())
        {
            std::map<std::string, std::string>::const_iterator iter2 = iter->second.find("turnedOn");
            if (iter2 != iter->second.end())
            {
                bool result = false;
                if (Project001::String::StringToBool(iter2->second, result))
                {
                    sharedDataPtr_->playerCreationInfos[2].turnedOn = result;
                }
            }

            iter2 = iter->second.find("controlScheme");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerCreationInfos[2].controlSchemeIndex = ControlSchemeInfo::StringToControlSchemeIndex(iter2->second);
            }

            iter2 = iter->second.find("spawnPositionX");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerCreationInfos[2].spawnPositionX = std::stof(iter2->second);
            }

            iter2 = iter->second.find("spawnPositionY");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerCreationInfos[2].spawnPositionY = std::stof(iter2->second);
            }

            iter2 = iter->second.find("spawnRotation");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerCreationInfos[2].spawnRotation = std::stof(iter2->second);
            }
        }

        iter = sections.find("Player_4");
        if (iter != sections.end())
        {
            std::map<std::string, std::string>::const_iterator iter2 = iter->second.find("turnedOn");
            if (iter2 != iter->second.end())
            {
                bool result = false;
                if (Project001::String::StringToBool(iter2->second, result))
                {
                    sharedDataPtr_->playerCreationInfos[3].turnedOn = result;
                }
            }

            iter2 = iter->second.find("controlScheme");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerCreationInfos[3].controlSchemeIndex = ControlSchemeInfo::StringToControlSchemeIndex(iter2->second);
            }

            iter2 = iter->second.find("spawnPositionX");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerCreationInfos[3].spawnPositionX = std::stof(iter2->second);
            }

            iter2 = iter->second.find("spawnPositionY");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerCreationInfos[3].spawnPositionY = std::stof(iter2->second);
            }

            iter2 = iter->second.find("spawnRotation");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerCreationInfos[3].spawnRotation = std::stof(iter2->second);
            }
        }

        iter = sections.find("Keyboard_1");
        if (iter != sections.end())
        {
            std::map<std::string, std::string>::const_iterator iter2 = iter->second.find("pause");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_1_pause_keyCode = Project001::StringToKeyCode(iter2->second);
            }

            iter2 = iter->second.find("left");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_1_left_keyCode = Project001::StringToKeyCode(iter2->second);
            }

            iter2 = iter->second.find("right");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_1_right_keyCode = Project001::StringToKeyCode(iter2->second);
            }

            iter2 = iter->second.find("up");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_1_up_keyCode = Project001::StringToKeyCode(iter2->second);
            }

            iter2 = iter->second.find("down");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_1_down_keyCode = Project001::StringToKeyCode(iter2->second);
            }

            iter2 = iter->second.find("grab");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_1_grab_keyCode = Project001::StringToKeyCode(iter2->second);
            }

            iter2 = iter->second.find("drop");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_1_drop_keyCode = Project001::StringToKeyCode(iter2->second);
            }
        }

        iter = sections.find("Keyboard_2");
        if (iter != sections.end())
        {
            std::map<std::string, std::string>::const_iterator iter2 = iter->second.find("pause");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_2_pause_keyCode = Project001::StringToKeyCode(iter2->second);
            }

            iter2 = iter->second.find("left");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_2_left_keyCode = Project001::StringToKeyCode(iter2->second);
            }

            iter2 = iter->second.find("right");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_2_right_keyCode = Project001::StringToKeyCode(iter2->second);
            }

            iter2 = iter->second.find("up");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_2_up_keyCode = Project001::StringToKeyCode(iter2->second);
            }

            iter2 = iter->second.find("down");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_2_down_keyCode = Project001::StringToKeyCode(iter2->second);
            }

            iter2 = iter->second.find("grab");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_2_grab_keyCode = Project001::StringToKeyCode(iter2->second);
            }

            iter2 = iter->second.find("drop");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_2_drop_keyCode = Project001::StringToKeyCode(iter2->second);
            }
        }

        iter = sections.find("Controller_1");
        if (iter != sections.end())
        {
            std::map<std::string, std::string>::const_iterator iter2 = iter->second.find("pause");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_1_pause_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("left");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_1_left_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("right");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_1_right_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("up");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_1_up_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("down");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_1_down_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("grab");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_1_grab_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("drop");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_1_drop_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("moveRightLeftAxis");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_1_moveRightLeft_axisIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("moveDownUpAxis");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_1_moveDownUp_axisIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("axisDeadzone");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_1_axisDeadzone = std::stof(iter2->second);
            }
        }

        iter = sections.find("Controller_2");
        if (iter != sections.end())
        {
            std::map<std::string, std::string>::const_iterator iter2 = iter->second.find("pause");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_2_pause_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("left");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_2_left_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("right");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_2_right_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("up");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_2_up_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("down");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_2_down_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("grab");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_2_grab_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("drop");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_2_drop_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("moveRightLeftAxis");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_2_moveRightLeft_axisIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("moveDownUpAxis");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_2_moveDownUp_axisIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("axisDeadzone");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_2_axisDeadzone = std::stof(iter2->second);
            }
        }

        iter = sections.find("Controller_3");
        if (iter != sections.end())
        {
            std::map<std::string, std::string>::const_iterator iter2 = iter->second.find("pause");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_3_pause_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("left");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_3_left_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("right");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_3_right_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("up");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_3_up_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("down");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_3_down_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("grab");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_3_grab_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("drop");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_3_drop_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("moveRightLeftAxis");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_3_moveRightLeft_axisIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("moveDownUpAxis");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_3_moveDownUp_axisIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("axisDeadzone");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_3_axisDeadzone = std::stof(iter2->second);
            }
        }

        iter = sections.find("Controller_4");
        if (iter != sections.end())
        {
            std::map<std::string, std::string>::const_iterator iter2 = iter->second.find("pause");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_4_pause_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("left");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_4_left_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("right");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_4_right_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("up");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_4_up_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("down");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_4_down_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("grab");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_4_grab_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("drop");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_4_drop_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("moveRightLeftAxis");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_4_moveRightLeft_axisIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("moveDownUpAxis");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_4_moveDownUp_axisIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("axisDeadzone");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_4_axisDeadzone = std::stof(iter2->second);
            }
        }

        iter = sections.find("Game_Constants");
        if (iter != sections.end())
        {
            std::map<std::string, std::string>::const_iterator iter2 = iter->second.find("cursorEnabled");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->cursorEnabled = static_cast<bool>(std::stoi(iter2->second));
            }

            iter2 = iter->second.find("invisiblePauseScreen");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->invisiblePauseScreen = static_cast<bool>(std::stoi(iter2->second));
            }

            iter2 = iter->second.find("groundApothem");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->groundApothem = std::stof(iter2->second);
            }

            iter2 = iter->second.find("groundApothemShrinkRate_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->groundApothemShrinkRate_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("sharkPathOffset");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->sharkPathOffset = std::stof(iter2->second);
            }

            iter2 = iter->second.find("killzoneApothem");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->killzoneApothem = std::stof(iter2->second);
            }

            iter2 = iter->second.find("coolGlassesPlayerIndex");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->coolGlassesPlayerIndex = static_cast<size_t>(std::stoi(iter2->second));
            }

            iter2 = iter->second.find("mainCameraPitch");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->mainCameraPitch = std::stof(iter2->second);
            }

            iter2 = iter->second.find("mainCameraInitialDistanceAway");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->mainCameraInitialDistanceAway = std::stof(iter2->second);
            }

            iter2 = iter->second.find("mainCameraMinimumPlayerSpread");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->mainCameraMinimumPlayerSpread = std::stof(iter2->second);
            }

            iter2 = iter->second.find("mainCameraPlayerToEdgeSpacing");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->mainCameraPlayerToEdgeSpacing = std::stof(iter2->second);
            }

            iter2 = iter->second.find("mainCameraMoveSpeed");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->mainCameraMoveSpeed = std::stof(iter2->second);
            }

            iter2 = iter->second.find("mainCameraZoomSpeed");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->mainCameraZoomSpeed = std::stof(iter2->second);
            }

            iter2 = iter->second.find("mainCameraSharkInclusionMode");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->mainCameraSharkInclusionMode = static_cast<size_t>(std::stoi(iter2->second));
            }

            iter2 = iter->second.find("cursorSnowballCreationDelay_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->cursorSnowballCreationDelay_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("cursorSnowballInitialRadius");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->cursorSnowballInitialRadius = std::stof(iter2->second);
            }

            iter2 = iter->second.find("cursorSnowballGrowthRate_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->cursorSnowballGrowthRate_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("cursorSnowballThrowSpeed_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->cursorSnowballThrowSpeed_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("penguinGlassesAlpha");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->penguinGlassesAlpha = std::stof(iter2->second);
            }

            iter2 = iter->second.find("penguinDensity");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->penguinDensity = std::stof(iter2->second);
            }

            iter2 = iter->second.find("penguinHitHardThreshold");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->penguinHitHardThreshold = std::stof(iter2->second);
            }

            iter2 = iter->second.find("penguinHitstunCooldownDivisor");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->penguinHitstunCooldownDivisor = std::stof(iter2->second);
            }

            iter2 = iter->second.find("penguinSnowballCreationDelay_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->penguinSnowballCreationDelay_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("penguinSnowballInitialRadius");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->penguinSnowballInitialRadius = std::stof(iter2->second);
            }

            iter2 = iter->second.find("penguinSnowballGrowthRate_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->penguinSnowballGrowthRate_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("penguinSnowballThrowSpeed_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->penguinSnowballThrowSpeed_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("penguinRegrabDelay_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->penguinRegrabDelay_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("penguinMaxSpeed_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->penguinMaxSpeed_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("penguinAcceleration_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->penguinAcceleration_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("penguinFriction_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->penguinFriction_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("penguinMaxAngularSpeed_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->penguinMaxAngularSpeed_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("penguinAngularAcceleration_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->penguinAngularAcceleration_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("penguinAngularFriction_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->penguinAngularFriction_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("penguinHitstunFriction_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->penguinHitstunFriction_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("penguinHitstunAngularFriction_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->penguinHitstunAngularFriction_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("sharkDensity");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->sharkDensity = std::stof(iter2->second);
            }

            iter2 = iter->second.find("sharkHitHardThreshold");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->sharkHitHardThreshold = std::stof(iter2->second);
            }

            iter2 = iter->second.find("sharkHitstunCooldownDivisor");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->sharkHitstunCooldownDivisor = std::stof(iter2->second);
            }

            iter2 = iter->second.find("sharkMaxSpeed_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->sharkMaxSpeed_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("sharkAcceleration_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->sharkAcceleration_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("sharkFriction_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->sharkFriction_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("sharkMaxAngularSpeed_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->sharkMaxAngularSpeed_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("sharkAngularAcceleration_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->sharkAngularAcceleration_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("sharkAngularFriction_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->sharkAngularFriction_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("sharkChasingMaxSpeed_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->sharkChasingMaxSpeed_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("sharkChasingAcceleration_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->sharkChasingAcceleration_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("sharkHitstunFriction_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->sharkHitstunFriction_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("sharkHitstunAngularFriction_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->sharkHitstunAngularFriction_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("snowballDensity");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->snowballDensity = std::stof(iter2->second);
            }

            iter2 = iter->second.find("snowballFriction_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->snowballFriction_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("snowballAngularFriction_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->snowballAngularFriction_s = std::stof(iter2->second);
            }
        }
    }
}

void Scene001::CreateKillZoneMesh()
{
    sharedDataPtr_->deadZone_meshDataPtr->Clear();

    constexpr float rectThickness = SharedApplicationData::s_quadtreeOffset;

    Project001::MeshData tempMeshData0;
    Project001::Mesh::Generate2DRectangle(tempMeshData0, rectThickness, sharedDataPtr_->killzoneApothem * 2.0f);
    Project001::Mesh::TranslateMesh(tempMeshData0, glm::vec3(-(sharedDataPtr_->killzoneApothem + rectThickness * 0.5f), 0.0f, 0.0f));

    Project001::MeshData tempMeshData1;
    Project001::Mesh::Generate2DRectangle(tempMeshData1, rectThickness, sharedDataPtr_->killzoneApothem * 2.0f);
    Project001::Mesh::TranslateMesh(tempMeshData1, glm::vec3(sharedDataPtr_->killzoneApothem + rectThickness * 0.5f, 0.0f, 0.0f));

    Project001::MeshData tempMeshData2;
    Project001::Mesh::Generate2DRectangle(tempMeshData2, (sharedDataPtr_->killzoneApothem + rectThickness) * 2.0f, rectThickness);
    Project001::Mesh::TranslateMesh(tempMeshData2, glm::vec3(0.0f, -(sharedDataPtr_->killzoneApothem + rectThickness * 0.5f), 0.0f));

    Project001::MeshData tempMeshData3;
    Project001::Mesh::Generate2DRectangle(tempMeshData3, (sharedDataPtr_->killzoneApothem + rectThickness) * 2.0f, rectThickness);
    Project001::Mesh::TranslateMesh(tempMeshData3, glm::vec3(0.0f, sharedDataPtr_->killzoneApothem + rectThickness * 0.5f, 0.0f));

    Project001::Mesh::CopyMesh(*sharedDataPtr_->deadZone_meshDataPtr, tempMeshData0);
    Project001::Mesh::CopyMesh(*sharedDataPtr_->deadZone_meshDataPtr, tempMeshData1);
    Project001::Mesh::CopyMesh(*sharedDataPtr_->deadZone_meshDataPtr, tempMeshData2);
    Project001::Mesh::CopyMesh(*sharedDataPtr_->deadZone_meshDataPtr, tempMeshData3);
    Project001::Mesh::ApplyPositionalTextureCoordinates(*sharedDataPtr_->deadZone_meshDataPtr);
    Project001::Mesh::ScaleTextureCoordinates(*sharedDataPtr_->deadZone_meshDataPtr, glm::vec2(1.0f / 32.0f, 1.0f / 32.0f));
}

void Scene001::CreateLoadingTextEntity()
{
    constexpr float loadingPixelSize = 2.0f;

    sharedDataPtr_->loadingText_meshDataPtr->Clear();

    std::string loadingString = "LOADING...";

    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->loadingText_meshDataPtr,
        *sharedDataPtr_->pixelFont_fontDataPtr,
        loadingString,
        loadingPixelSize,
        1
    ));

    GetComponentStoresPtr()->CreateEntity(loadingText_entityId_);
    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(loadingText_entityId_));
    Project001::RenderedMesh* renderedMeshPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, loadingText_entityId_));
    if (renderedMeshPtr != nullptr)
    {
        renderedMeshPtr->SetCameraMask(s_uiCamera_cameraMask_);
        renderedMeshPtr->SetMeshDataPtr(sharedDataPtr_->loadingText_meshDataPtr);
        renderedMeshPtr->SetTextureId(sharedDataPtr_->pixelFont_textureId);
        renderedMeshPtr->SetColor(0.6f, 0.6f, 0.6f, 1.0f);
        renderedMeshPtr->SetUseLighting(false);
    }
}