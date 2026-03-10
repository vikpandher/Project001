// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-03-10

#include "Scene001.h"

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
    : Scene(applicationPtr)
{
    sharedDataPtr_ = GetSharedDataPtr<SharedApplicationData>();
    sharedDataPtr_->scene001Id = GetId();

    LoadPixelFontResources();
    LoadGeneralResources();
    LoadMainMenuResources();
    LoadStageResources();
    LoadStageGridResources();
    LoadActorResources();
    LoadUiResources();
}

Scene001::~Scene001()
{
    FreeResources();
}

void Scene001::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&Scene001::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&Scene001::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&Scene001::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&Scene001::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&Scene001::ProcessUpdateEvent, this, std::placeholders::_1));
}

// protected -------------------------------------------------------------------

void Scene001::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   Scene001:            " << GetId());

    // -------------------------------------------------------------------------

    ReadConfigFile();

    CreateUiCameraEntity();
    CreateIntroTextEntity();
}

void Scene001::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    LOG_INFO("DEINITIALIZING: Scene001:            " << GetId());

    // -------------------------------------------------------------------------

    configFileFound_ = false;

    GetComponentStoresPtr()->DeleteEntity(uiCamera_entityId_);
    uiCamera_entityId_ = static_cast<unsigned int>(-1);

    GetComponentStoresPtr()->DeleteEntity(autorText_entityId_);
    autorText_entityId_ = static_cast<unsigned int>(-1);

    GetComponentStoresPtr()->DeleteEntity(introText_entityId_);
    introText_entityId_ = static_cast<unsigned int>(-1);

    GetComponentStoresPtr()->DeleteEntity(startText_entityId_);
    startText_entityId_ = static_cast<unsigned int>(-1);
}

void Scene001::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    sharedDataPtr_->UpdateKeyboardButtonPresses(keyEvent);
}

void Scene001::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    GetRenderSystemPtr()->Render();
}

void Scene001::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    sharedDataPtr_->UpdateButtonPressCounts(GetWindowPtr());

    bool startPressed = false;
    for (size_t i = 0; i < sharedDataPtr_->s_player_count; ++i)
    {
        startPressed |= sharedDataPtr_->playerInfos[i].turnedOn && (sharedDataPtr_->playerInfos[i].start_pressCount > 0);
    }
    if (startPressed)
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
    sharedDataPtr_->authorText_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->introText_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->startText_meshDataPtr = new Project001::MeshData();
    sharedDataPtr_->titleText_meshDataPtr = new Project001::MeshData();
}

void Scene001::LoadStageResources()
{
    constexpr float height = 256.0f;
    const float radius = SharedApplicationData::s_ground_size * 1.0829f;
    const float groundCorner = SharedApplicationData::s_ground_size * 0.41421357f; // sqrt(2) - 1

    sharedDataPtr_->ground_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::GenerateCylinder(
        *sharedDataPtr_->ground_meshDataPtr, height, radius, 8, false
    ));
    Project001::Mesh::RotateMeshX(*sharedDataPtr_->ground_meshDataPtr, glm::half_pi<float>());
    Project001::Mesh::RotateMeshZ(*sharedDataPtr_->ground_meshDataPtr, glm::pi<float>() / 8.0f);
    Project001::Mesh::TranslateMesh(*sharedDataPtr_->ground_meshDataPtr, glm::vec3(0.0f, 0.0f, -0.5f * height));

    sharedDataPtr_->water_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::Generate2DSprite(
        *sharedDataPtr_->water_meshDataPtr,
        SharedApplicationData::s_ground_size * 24.0f,
        SharedApplicationData::s_ground_size * 24.0f,
        0.0f, 1.0f, 0.0f, 1.0f
    ));

    std::vector<glm::vec2> groundCollisionCorners;
    groundCollisionCorners.reserve(8);
    groundCollisionCorners.emplace_back(SharedApplicationData::s_ground_size, groundCorner);
    groundCollisionCorners.emplace_back(groundCorner, SharedApplicationData::s_ground_size);
    groundCollisionCorners.emplace_back(-groundCorner, SharedApplicationData::s_ground_size);
    groundCollisionCorners.emplace_back(-SharedApplicationData::s_ground_size, groundCorner);
    groundCollisionCorners.emplace_back(-SharedApplicationData::s_ground_size, -groundCorner);
    groundCollisionCorners.emplace_back(-groundCorner, -SharedApplicationData::s_ground_size);
    groundCollisionCorners.emplace_back(groundCorner, -SharedApplicationData::s_ground_size);
    groundCollisionCorners.emplace_back(SharedApplicationData::s_ground_size, -groundCorner);
    sharedDataPtr_->groundCollision_meshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::Mesh::Generate2DTriangleFan(
        *sharedDataPtr_->groundCollision_meshDataPtr,
        groundCollisionCorners
    ));

    {
        float rectThickness = (SharedApplicationData::s_maxStage_size - SharedApplicationData::s_deadzone_size) * 2.0f;
    
        Project001::MeshData tempMeshData0;
        Project001::Mesh::Generate2DRectangle(tempMeshData0, rectThickness, SharedApplicationData::s_deadzone_size * 2.0f);
        Project001::Mesh::TranslateMesh(tempMeshData0, glm::vec3(-(SharedApplicationData::s_deadzone_size + rectThickness * 0.5f), 0.0f, 0.0f));
    
        Project001::MeshData tempMeshData1;
        Project001::Mesh::Generate2DRectangle(tempMeshData1, rectThickness, SharedApplicationData::s_deadzone_size * 2.0f);
        Project001::Mesh::TranslateMesh(tempMeshData1, glm::vec3(SharedApplicationData::s_deadzone_size + rectThickness * 0.5f, 0.0f, 0.0f));
    
        Project001::MeshData tempMeshData2;
        Project001::Mesh::Generate2DRectangle(tempMeshData2, (SharedApplicationData::s_deadzone_size + rectThickness) * 2.0f, rectThickness);
        Project001::Mesh::TranslateMesh(tempMeshData2, glm::vec3(0.0f, -(SharedApplicationData::s_deadzone_size + rectThickness * 0.5f), 0.0f));
    
        Project001::MeshData tempMeshData3;
        Project001::Mesh::Generate2DRectangle(tempMeshData3, (SharedApplicationData::s_deadzone_size + rectThickness) * 2.0f, rectThickness);
        Project001::Mesh::TranslateMesh(tempMeshData3, glm::vec3(0.0f, SharedApplicationData::s_deadzone_size + rectThickness * 0.5f, 0.0f));
    
        sharedDataPtr_->deadZone_meshDataPtr = new Project001::MeshData();
        Project001::Mesh::CopyMesh(*sharedDataPtr_->deadZone_meshDataPtr, tempMeshData0);
        Project001::Mesh::CopyMesh(*sharedDataPtr_->deadZone_meshDataPtr, tempMeshData1);
        Project001::Mesh::CopyMesh(*sharedDataPtr_->deadZone_meshDataPtr, tempMeshData2);
        Project001::Mesh::CopyMesh(*sharedDataPtr_->deadZone_meshDataPtr, tempMeshData3);
        Project001::Mesh::ApplyPositionalTextureCoordinates(*sharedDataPtr_->deadZone_meshDataPtr);
        Project001::Mesh::ScaleTextureCoordinates(*sharedDataPtr_->deadZone_meshDataPtr, glm::vec2(1.0f / 32.0f, 1.0f / 32.0f));
    }

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
    float gridLabel_offsetY = pixelFont_pixelSize * 7.0f + 0.5f * lineWidth;
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

    gridLabel_offsetY = pixelFont_pixelSize * 7.0f + 0.5f * lineWidth + gridSpacing;
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

    gridLabel_offsetY = pixelFont_pixelSize * -1.0f - 0.5f * lineWidth;
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

    gridLabel_offsetY = pixelFont_pixelSize * -1.0f - 0.5f * lineWidth - gridSpacing;
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
}

void Scene001::LoadUiResources()
{
    // TODO:
}

void Scene001::FreeResources()
{
    GetRendererPtr()->DeleteAllTextures();
    GetRendererPtr()->DeleteAllMeshes();
    GetSoundPlayerPtr()->DeleteAllSoundBuffers();
    GetSoundPlayerPtr()->DeleteAllSoundSources();

    // General Resources
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
    delete sharedDataPtr_->authorText_meshDataPtr;
    sharedDataPtr_->authorText_meshDataPtr = nullptr;
    delete sharedDataPtr_->introText_meshDataPtr;
    sharedDataPtr_->introText_meshDataPtr = nullptr;
    delete sharedDataPtr_->startText_meshDataPtr;
    sharedDataPtr_->startText_meshDataPtr = nullptr;
    delete sharedDataPtr_->titleText_meshDataPtr;
    sharedDataPtr_->titleText_meshDataPtr = nullptr;

    // Stage Resources
    delete sharedDataPtr_->ground_meshDataPtr;
    sharedDataPtr_->ground_meshDataPtr = nullptr;
    delete sharedDataPtr_->groundCollision_meshDataPtr;
    sharedDataPtr_->groundCollision_meshDataPtr = nullptr;

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

    // Ui Resources
    delete sharedDataPtr_->uiLeftBackground_meshDataPtr;
    sharedDataPtr_->uiLeftBackground_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiLeftText01_meshDataPtr;
    sharedDataPtr_->uiLeftText01_meshDataPtr = nullptr;

    delete sharedDataPtr_->uiMiddleBackground_meshDataPtr;
    sharedDataPtr_->uiMiddleBackground_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiMiddleText01_meshDataPtr;
    sharedDataPtr_->uiMiddleText01_meshDataPtr = nullptr;

    delete sharedDataPtr_->uiRightBackground_meshDataPtr;
    sharedDataPtr_->uiRightBackground_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiRightText01_meshDataPtr;
    sharedDataPtr_->uiRightText01_meshDataPtr = nullptr;

    delete sharedDataPtr_->uiPauseBackground_meshDataPtr;
    sharedDataPtr_->uiPauseBackground_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiPauseText01_meshDataPtr;
    sharedDataPtr_->uiPauseText01_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiPauseText02_meshDataPtr;
    sharedDataPtr_->uiPauseText02_meshDataPtr = nullptr;
    delete sharedDataPtr_->uiPauseText03_meshDataPtr;
    sharedDataPtr_->uiPauseText03_meshDataPtr = nullptr;
}

void Scene001::ReadConfigFile()
{
    std::ifstream inputStream("CoolPenguinSnowball.ini");

    if (inputStream.is_open())
    {
        configFileFound_ = true;

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
                    sharedDataPtr_->playerInfos[0].turnedOn = result;
                }
            }

            iter2 = iter->second.find("controlScheme");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerInfos[0].controlScheme = PlayerInfo::StringToControlScheme(iter2->second);
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
                    sharedDataPtr_->playerInfos[1].turnedOn = result;
                }
            }

            iter2 = iter->second.find("controlScheme");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerInfos[1].controlScheme = PlayerInfo::StringToControlScheme(iter2->second);
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
                    sharedDataPtr_->playerInfos[2].turnedOn = result;
                }
            }

            iter2 = iter->second.find("controlScheme");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerInfos[2].controlScheme = PlayerInfo::StringToControlScheme(iter2->second);
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
                    sharedDataPtr_->playerInfos[3].turnedOn = result;
                }
            }

            iter2 = iter->second.find("controlScheme");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->playerInfos[3].controlScheme = PlayerInfo::StringToControlScheme(iter2->second);
            }
        }

        iter = sections.find("Keyboard_1");
        if (iter != sections.end())
        {
            std::map<std::string, std::string>::const_iterator iter2 = iter->second.find("start");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_1_start_keyCode = Project001::StringToKeyCode(iter2->second);
            }

            iter2 = iter->second.find("pause");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_1_pause_keyCode = Project001::StringToKeyCode(iter2->second);
            }

            iter2 = iter->second.find("quit");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_1_quit_keyCode = Project001::StringToKeyCode(iter2->second);
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

            iter2 = iter->second.find("throw");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_1_throw_keyCode = Project001::StringToKeyCode(iter2->second);
            }
        }

        iter = sections.find("Keyboard_2");
        if (iter != sections.end())
        {
            std::map<std::string, std::string>::const_iterator iter2 = iter->second.find("start");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_2_start_keyCode = Project001::StringToKeyCode(iter2->second);
            }

            iter2 = iter->second.find("pause");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_2_pause_keyCode = Project001::StringToKeyCode(iter2->second);
            }

            iter2 = iter->second.find("quit");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_2_quit_keyCode = Project001::StringToKeyCode(iter2->second);
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

            iter2 = iter->second.find("throw");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->keyboard_2_throw_keyCode = Project001::StringToKeyCode(iter2->second);
            }
        }

        iter = sections.find("Controller_1");
        if (iter != sections.end())
        {
            std::map<std::string, std::string>::const_iterator iter2 = iter->second.find("start");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_1_start_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("pause");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_1_pause_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("quit");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_1_quit_buttonIndex = std::stoi(iter2->second);
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

            iter2 = iter->second.find("throw");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_1_throw_buttonIndex = std::stoi(iter2->second);
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
            std::map<std::string, std::string>::const_iterator iter2 = iter->second.find("start");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_2_start_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("pause");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_2_pause_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("quit");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_2_quit_buttonIndex = std::stoi(iter2->second);
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

            iter2 = iter->second.find("throw");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_2_throw_buttonIndex = std::stoi(iter2->second);
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
            std::map<std::string, std::string>::const_iterator iter2 = iter->second.find("start");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_3_start_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("pause");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_3_pause_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("quit");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_3_quit_buttonIndex = std::stoi(iter2->second);
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

            iter2 = iter->second.find("throw");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_3_throw_buttonIndex = std::stoi(iter2->second);
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
            std::map<std::string, std::string>::const_iterator iter2 = iter->second.find("start");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_4_start_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("pause");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_4_pause_buttonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("quit");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_4_quit_buttonIndex = std::stoi(iter2->second);
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

            iter2 = iter->second.find("throw");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->controller_4_throw_buttonIndex = std::stoi(iter2->second);
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
            std::map<std::string, std::string>::const_iterator iter2 = iter->second.find("randomNumberSeed");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->randomNumberSeed = std::stoi(iter2->second);
            }
        }
    }
}

void Scene001::CreateUiCameraEntity()
{
    GetComponentStoresPtr()->CreateEntity(uiCamera_entityId_);
    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::Camera>(uiCamera_entityId_));

    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, uiCamera_entityId_));
    if (cameraPtr != nullptr)
    {
        int aspectRatioNumerator;
        int aspectRatioDenominator;
        GetWindowPtr()->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
        if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
        {
            float aspectRatio = static_cast<float>(aspectRatioNumerator) / static_cast<float>(aspectRatioDenominator);
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
        cameraPtr->SetCameraMask(s_uiCamera_cameraMask_);
        cameraPtr->SetPriorityValue(1000);
    }
}

void Scene001::CreateIntroTextEntity()
{
    // -------------------------------------------------------------------------

    constexpr float introPixelSize = 2.0f;

    sharedDataPtr_->introText_meshDataPtr->Clear();

    std::string introString = "PLACEHOLDER INTRO STRING"; // TODO:

    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->introText_meshDataPtr,
        *sharedDataPtr_->pixelFont_fontDataPtr,
        introString,
        introPixelSize
    ));

    GetComponentStoresPtr()->CreateEntity(introText_entityId_);
    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(introText_entityId_));
    Project001::RenderedMesh* renderedMeshPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, introText_entityId_));
    if (renderedMeshPtr != nullptr)
    {
        renderedMeshPtr->SetCameraMask(s_uiCamera_cameraMask_);
        renderedMeshPtr->SetMeshDataPtr(sharedDataPtr_->introText_meshDataPtr);
        renderedMeshPtr->SetTextureId(sharedDataPtr_->pixelFont_textureId);
        renderedMeshPtr->SetColor(0.6f, 0.6f, 0.6f, 1.0f);
        renderedMeshPtr->SetUseLighting(false);
    }

    // -------------------------------------------------------------------------

    constexpr float authorPixelSize = 2.0f;

    sharedDataPtr_->authorText_meshDataPtr->Clear();

    std::string authorString = "PLACEHOLDER AUTHOR STRING"; // TODO:

    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->authorText_meshDataPtr,
        *sharedDataPtr_->pixelFont_fontDataPtr,
        authorString,
        authorPixelSize
    ));

    GetComponentStoresPtr()->CreateEntity(autorText_entityId_);
    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(autorText_entityId_));
    renderedMeshPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, autorText_entityId_));
    if (renderedMeshPtr != nullptr)
    {
        renderedMeshPtr->SetCameraMask(s_uiCamera_cameraMask_);
        renderedMeshPtr->SetMeshDataPtr(sharedDataPtr_->authorText_meshDataPtr);
        renderedMeshPtr->SetTextureId(sharedDataPtr_->pixelFont_textureId);
        renderedMeshPtr->SetColor(0.2f, 0.2f, 0.2f, 1.0f);
        renderedMeshPtr->SetUseLighting(false);
    }

    // -------------------------------------------------------------------------

    constexpr float startPixelSize = 2.0f;

    sharedDataPtr_->startText_meshDataPtr->Clear();

    std::string startString = "PLACEHOLDER INTRO STRING"; // TODO:

    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->startText_meshDataPtr,
        *sharedDataPtr_->pixelFont_fontDataPtr,
        startString,
        startPixelSize
    ));

    GetComponentStoresPtr()->CreateEntity(startText_entityId_);
    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(startText_entityId_));
    renderedMeshPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, startText_entityId_));
    if (renderedMeshPtr != nullptr)
    {
        renderedMeshPtr->SetCameraMask(s_uiCamera_cameraMask_);
        renderedMeshPtr->SetMeshDataPtr(sharedDataPtr_->startText_meshDataPtr);
        renderedMeshPtr->SetTextureId(sharedDataPtr_->pixelFont_textureId);
        renderedMeshPtr->SetColor(0.6f, 0.6f, 0.6f, 1.0f);
        renderedMeshPtr->SetUseLighting(false);
    }
}