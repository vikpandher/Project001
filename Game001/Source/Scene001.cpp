// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-13

#include "Scene001.h"

#include "Resources/dropCoin_wav.h"
#include "Resources/gridDark_64x64_png.h"
#include "Resources/gridLit_64x64_png.h"
#include "Resources/hitHurt_wav.h"
#include "Resources/houseDark_384x256_png.h"
#include "Resources/houseLit_384x256_png.h"
#include "Resources/lampDark_32x160_png.h"
#include "Resources/lampLit_32x160_png.h"
#include "Resources/monsterLit_32x56_png.h"
#include "Resources/pickupCoin_wav.h"
#include "Resources/personLit_32x56_png.h"
#include "Resources/unknownDark_32x56_png.h"

#include "Components/Camera.h"
#include "Components/RenderedModel.h"
#include "Math/MathUtilities.h"
#include "Resources/PixelFont5x6.h"
#include "ComponentStores.h"
#include "FontLoader.h"
#include "IniReaderWriter.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "RenderSystem.h"
#include "SoundLoader.h"
#include "SoundPlayer.h"
#include "TextureLoader.h"
#include "Window.h"

#include <fstream>



// public ----------------------------------------------------------------------

Scene001::Scene001(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
{
    sharedDataPtr_ = GetSharedDataPtr<SharedApplicationData>();
    sharedDataPtr_->scene001Id = GetId();

    ReadConfigFile();

    LoadPixelFontResources();

    LoadIntroTextResources();
    LoadCursorResources();
    LoadGroundGridResources();
    LoadGroundResources();
    LoadHouseLightResources();
    LoadHouseResources();
    LoadLampLightResources();
    LoadLampResources();
    LoadPersonResources();
    LoadPlayerLightResources();
    LoadUiTextResources();
    LoadSoundResources();
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
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&Scene001::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&Scene001::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&Scene001::ProcessUpdateEvent, this, std::placeholders::_1));
}

// protected -------------------------------------------------------------------

void Scene001::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   Scene001:            " << GetId());

    // -------------------------------------------------------------------------

    CreateUiCameraEntity();
    CreateIntroTextEntity();
}

void Scene001::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    LOG_INFO("DEINITIALIZING: Scene001:            " << GetId());

    // -------------------------------------------------------------------------

    GetComponentStoresPtr()->DeleteEntity(uiCamera_EntityId_);
    uiCamera_EntityId_ = static_cast<unsigned int>(-1);

    GetComponentStoresPtr()->DeleteEntity(introText_EntityId_);
    introText_EntityId_ = static_cast<unsigned int>(-1);

    GetComponentStoresPtr()->DeleteEntity(autorText_EntityId_);
    autorText_EntityId_ = static_cast<unsigned int>(-1);
}

void Scene001::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (sharedDataPtr_->pause_usesKeyboard && keyCode == sharedDataPtr_->pause_KeyCode)
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
}

void Scene001::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (!sharedDataPtr_->pause_usesKeyboard && mouseButton == sharedDataPtr_->pause_MouseButton)
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
}

void Scene001::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    Project001::RenderSystem::Render(GetComponentStoresPtr(), GetRendererPtr());
}

void Scene001::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    std::vector<bool> buttonValues;
    GetWindowPtr()->GetJoystickButtons(0, buttonValues);

    bool start = false;
    if (buttonValues.size() > sharedDataPtr_->pause_joystickButtonIndex)
    {
        start |= buttonValues[sharedDataPtr_->pause_joystickButtonIndex];
    }

    if (start)
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
    sharedDataPtr_->pixelFont_FontDataPtr = &Project001::Get_PixelFont5x6_FontData();
    sharedDataPtr_->pixelFont_TextureDataPtr = &Project001::Get_PixelFont5x6_TextureData();
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->pixelFont_TextureId,
        sharedDataPtr_->pixelFont_TextureDataPtr->data,
        sharedDataPtr_->pixelFont_TextureDataPtr->width,
        sharedDataPtr_->pixelFont_TextureDataPtr->height,
        sharedDataPtr_->pixelFont_TextureDataPtr->bytesPerPixel,
        false,
        false
    );
}

void Scene001::LoadIntroTextResources()
{
    sharedDataPtr_->introText_MeshDataPtr = new Project001::MeshData();

    sharedDataPtr_->authorText_MeshDataPtr = new Project001::MeshData();
}

void Scene001::LoadCursorResources()
{
    sharedDataPtr_->cursorCircle_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(
        *sharedDataPtr_->cursorCircle_MeshDataPtr, 4.0f, 12
    ));
}

void Scene001::LoadGroundGridResources()
{
    sharedDataPtr_->groundGrid_MeshDataPtr = new Project001::MeshData();

    constexpr float gridSpacing = 64.0f;
    constexpr float gridSize = 16.0f * gridSpacing;
    constexpr float lineWidth = 2.0f;

    for (float f = -gridSize; f < gridSize + 1.0f; f += gridSpacing)
    {
        FAIL_CHECK(Project001::MeshLoader::Generate2DLine(
            *sharedDataPtr_->groundGrid_MeshDataPtr,
            glm::vec2(-gridSize - 0.5f * lineWidth, f),
            glm::vec2(gridSize + 0.5f * lineWidth, f),
            lineWidth
        ));
    }
    for (float f = -gridSize; f < gridSize + 1.0f; f += gridSpacing)
    {
        for (float g = -gridSize; g < gridSize - 1.0f; g += gridSpacing)
        {
            FAIL_CHECK(Project001::MeshLoader::Generate2DLine(
                *sharedDataPtr_->groundGrid_MeshDataPtr,
                glm::vec2(f, g + 0.5f * lineWidth),
                glm::vec2(f, g - 0.5f * lineWidth + gridSpacing),
                lineWidth
            ));
        }
    }

    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->groundGrid_MeshId,
        sharedDataPtr_->groundGrid_MeshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->groundGrid_MeshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->groundGrid_MeshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->groundGrid_MeshDataPtr->meshIndexArray.size())
    );

    sharedDataPtr_->groundGridLabels_MeshDataPtr = new Project001::MeshData();

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
            *sharedDataPtr_->pixelFont_FontDataPtr,
            currentLabel,
            pixelFont_pixelSize
        ));
        Project001::MeshLoader::TranslateMesh(
            currentLabelMeshData,
            glm::vec3(gridLabel_offsetX, gridLabel_offsetY, 0.0f)
        );

        Project001::MeshLoader::CopyMesh(*sharedDataPtr_->groundGridLabels_MeshDataPtr, currentLabelMeshData);

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
            *sharedDataPtr_->pixelFont_FontDataPtr,
            currentLabel,
            pixelFont_pixelSize
        ));
        Project001::MeshLoader::TranslateMesh(
            currentLabelMeshData,
            glm::vec3(gridLabel_offsetX, gridLabel_offsetY, 0.0f)
        );

        Project001::MeshLoader::CopyMesh(*sharedDataPtr_->groundGridLabels_MeshDataPtr, currentLabelMeshData);

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
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            currentLabelMeshData,
            *sharedDataPtr_->pixelFont_FontDataPtr,
            currentLabel,
            pixelFont_pixelSize
        ));
        Project001::MeshLoader::TranslateMesh(
            currentLabelMeshData,
            glm::vec3(gridLabel_offsetX + addition_offsetX, gridLabel_offsetY, 0.0f)
        );

        Project001::MeshLoader::CopyMesh(*sharedDataPtr_->groundGridLabels_MeshDataPtr, currentLabelMeshData);

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
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            currentLabelMeshData,
            *sharedDataPtr_->pixelFont_FontDataPtr,
            currentLabel,
            pixelFont_pixelSize
        ));
        Project001::MeshLoader::TranslateMesh(
            currentLabelMeshData,
            glm::vec3(gridLabel_offsetX + addition_offsetX, gridLabel_offsetY, 0.0f)
        );

        Project001::MeshLoader::CopyMesh(*sharedDataPtr_->groundGridLabels_MeshDataPtr, currentLabelMeshData);

        gridLabel_offsetY -= gridSpacing;
    }

    GetRendererPtr()->CreateMesh(
        sharedDataPtr_->groundGridLabels_MeshId,
        sharedDataPtr_->groundGridLabels_MeshDataPtr->meshVertexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->groundGridLabels_MeshDataPtr->meshVertexArray.size()),
        sharedDataPtr_->groundGridLabels_MeshDataPtr->meshIndexArray.data(),
        static_cast<unsigned int>(sharedDataPtr_->groundGridLabels_MeshDataPtr->meshIndexArray.size())
    );
}

void Scene001::LoadGroundResources()
{
    sharedDataPtr_->groundDark_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
        *sharedDataPtr_->groundDark_MeshDataPtr,
        2048.0f, 2048.0f, 0.0f, 32.0f, 0.0f, 32.0f
    ));

    sharedDataPtr_->groundDark_TextureDataPtr = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTextureFromMemory(
        *sharedDataPtr_->groundDark_TextureDataPtr,
        g_gridDark_64x64_png,
        sizeof(g_gridDark_64x64_png) / sizeof(unsigned char)
    ));
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->groundDark_TextureId,
        sharedDataPtr_->groundDark_TextureDataPtr->data,
        sharedDataPtr_->groundDark_TextureDataPtr->width,
        sharedDataPtr_->groundDark_TextureDataPtr->height,
        sharedDataPtr_->groundDark_TextureDataPtr->bytesPerPixel,
        false, false
    );

    sharedDataPtr_->groundLit_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
        *sharedDataPtr_->groundLit_MeshDataPtr,
        2048.0f, 2048.0f, 0.0f, 32.0f, 0.0f, 32.0f
    ));

    sharedDataPtr_->groundLit_TextureDataPtr = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTextureFromMemory(
        *sharedDataPtr_->groundLit_TextureDataPtr,
        g_gridLit_64x64_png,
        sizeof(g_gridLit_64x64_png) / sizeof(unsigned char)
    ));
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->groundLit_TextureId,
        sharedDataPtr_->groundLit_TextureDataPtr->data,
        sharedDataPtr_->groundLit_TextureDataPtr->width,
        sharedDataPtr_->groundLit_TextureDataPtr->height,
        sharedDataPtr_->groundLit_TextureDataPtr->bytesPerPixel,
        false, false
    );

    sharedDataPtr_->groundFog_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
        *sharedDataPtr_->groundFog_MeshDataPtr,
        20480.0f, 20480.0f, 0.0f, 1.0f, 0.0f, 1.0f
    ));

    {
        Project001::MeshData tempMeshData0;
        Project001::MeshLoader::Generate2DRectangle(tempMeshData0, 128.0f, 2048.0f);
        Project001::MeshLoader::TranslateMesh(tempMeshData0, glm::vec3(-1088.0f, 0.0f, 0.0f));

        Project001::MeshData tempMeshData1;
        Project001::MeshLoader::Generate2DRectangle(tempMeshData1, 128.0f, 2048.0f);
        Project001::MeshLoader::TranslateMesh(tempMeshData1, glm::vec3(1088.0f, 0.0f, 0.0f));

        Project001::MeshData tempMeshData2;
        Project001::MeshLoader::Generate2DRectangle(tempMeshData2, 2304.0f, 128.0f);
        Project001::MeshLoader::TranslateMesh(tempMeshData2, glm::vec3(0.0f, 1088.0f, 0.0f));

        Project001::MeshData tempMeshData3;
        Project001::MeshLoader::Generate2DRectangle(tempMeshData3, 2304.0f, 128.0f);
        Project001::MeshLoader::TranslateMesh(tempMeshData3, glm::vec3(0.0f, -1088.0f, 0.0f));

        sharedDataPtr_->groundCollision_MeshDataPtr = new Project001::MeshData();
        Project001::MeshLoader::CopyMesh(*sharedDataPtr_->groundCollision_MeshDataPtr, tempMeshData0);
        Project001::MeshLoader::CopyMesh(*sharedDataPtr_->groundCollision_MeshDataPtr, tempMeshData1);
        Project001::MeshLoader::CopyMesh(*sharedDataPtr_->groundCollision_MeshDataPtr, tempMeshData2);
        Project001::MeshLoader::CopyMesh(*sharedDataPtr_->groundCollision_MeshDataPtr, tempMeshData3);
    }

    sharedDataPtr_->groundCollisionQuadTree_MeshDataPtr = new Project001::MeshData();
}

void Scene001::LoadHouseLightResources()
{
    const float& radius = sharedDataPtr_->houseLight_radius;

    sharedDataPtr_->houseLightTop_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::GenerateHemisphere(
        *sharedDataPtr_->houseLightTop_MeshDataPtr, radius, 16, 8, false
    ));
    Project001::MeshLoader::RotateMeshX(
        *sharedDataPtr_->houseLightTop_MeshDataPtr, glm::half_pi<float>()
    );
    Project001::MeshLoader::TranslateMesh(
        *sharedDataPtr_->houseLightTop_MeshDataPtr, glm::vec3(0.0f, 0.0f, radius * 0.5f)
    );

    sharedDataPtr_->houseLightBottom_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(
        *sharedDataPtr_->houseLightBottom_MeshDataPtr, radius, 16
    ));
    Project001::MeshLoader::TranslateMesh(
        *sharedDataPtr_->houseLightBottom_MeshDataPtr, glm::vec3(0.0f, 1.0f, 1.0f)
    );

    sharedDataPtr_->houseLightCollision_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(
        *sharedDataPtr_->houseLightCollision_MeshDataPtr, radius, 16
    ));

    sharedDataPtr_->houseDoorCollision_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRectangle(
        *sharedDataPtr_->houseDoorCollision_MeshDataPtr, 48.0f, 32.0f
    ));
    Project001::MeshLoader::TranslateMesh(
        *sharedDataPtr_->houseDoorCollision_MeshDataPtr, glm::vec3(0.0f, -80.0f, 0.0f)
    );
}

void Scene001::LoadHouseResources()
{
    {
        Project001::MeshData tempMeshData0; // front face
        FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
            tempMeshData0, 128.0f, 112.0f,
            128.0f / 384.0f,
            256.0f / 384.0f,
            0.0f,
            112.0f / 256.0f
        ));
        Project001::MeshLoader::TranslateMesh(tempMeshData0, glm::vec3(0.0f, 56.0f, 64.0f));

        Project001::MeshData tempMeshData1; // left face
        FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
            tempMeshData1, 128.0f, 80.0f,
            0.0f,
            128.0f / 384.0f,
            0.0f,
            80.0f / 256.0f
        ));
        Project001::MeshLoader::TranslateMesh(tempMeshData1, glm::vec3(0.0f, 40.0f, 64.0f));
        Project001::MeshLoader::RotateMeshY(tempMeshData1, -glm::half_pi<float>());

        Project001::MeshData tempMeshData2; // right face
        FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
            tempMeshData2, 128.0f, 80.0f,
            256.0f / 384.0f,
            1.0f,
            0.0f,
            80.0f / 256.0f
        ));
        Project001::MeshLoader::TranslateMesh(tempMeshData2, glm::vec3(0.0f, 40.0f, 64.0f));
        Project001::MeshLoader::RotateMeshY(tempMeshData2, glm::half_pi<float>());

        Project001::MeshData tempMeshData3; // left roof
        FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
            tempMeshData3, 80.0f, 144.0f,
            112.0f / 384.0f,
            192.0f / 384.0f,
            112.0f / 256.0f,
            1.0f
        ));
        Project001::MeshLoader::RotateMeshX(tempMeshData3, -glm::half_pi<float>());
        Project001::MeshLoader::TranslateMesh(tempMeshData3, glm::vec3(-40.0f, 0.0f, 0.0f));
        Project001::MeshLoader::RotateMeshZ(tempMeshData3, glm::pi<float>() / 7.0f);
        Project001::MeshLoader::TranslateMesh(tempMeshData3, glm::vec3(0.0f, 114.0f, 0.0f));

        Project001::MeshData tempMeshData4; // right roof
        FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
            tempMeshData4, 80.0f, 144.0f,
            192.0f / 384.0f,
            272.0f / 384.0f,
            112.0f / 256.0f,
            1.0f
        ));
        Project001::MeshLoader::RotateMeshX(tempMeshData4, -glm::half_pi<float>());
        Project001::MeshLoader::TranslateMesh(tempMeshData4, glm::vec3(40.0f, 0.0f, 0.0f));
        Project001::MeshLoader::RotateMeshZ(tempMeshData4, -glm::pi<float>() / 7.0f);
        Project001::MeshLoader::TranslateMesh(tempMeshData4, glm::vec3(0.0f, 114.0f, 0.0f));

        Project001::MeshData tempMeshData5; // door mat
        FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
            tempMeshData5, 48.0f, 32.0f,
            0.0f,
            48.0f / 384.0f,
            224.0f / 256.0f,
            1.0f
        ));
        Project001::MeshLoader::RotateMeshX(tempMeshData5, -glm::half_pi<float>());
        Project001::MeshLoader::TranslateMesh(tempMeshData5, glm::vec3(0.0f, 0.4f, 80.0f));

        sharedDataPtr_->houseLit_MeshDataPtr = new Project001::MeshData();
        Project001::MeshLoader::CopyMesh(*sharedDataPtr_->houseLit_MeshDataPtr, tempMeshData3);
        Project001::MeshLoader::CopyMesh(*sharedDataPtr_->houseLit_MeshDataPtr, tempMeshData4);
        Project001::MeshLoader::CopyMesh(*sharedDataPtr_->houseLit_MeshDataPtr, tempMeshData0);
        Project001::MeshLoader::CopyMesh(*sharedDataPtr_->houseLit_MeshDataPtr, tempMeshData1);
        Project001::MeshLoader::CopyMesh(*sharedDataPtr_->houseLit_MeshDataPtr, tempMeshData2);
        Project001::MeshLoader::CopyMesh(*sharedDataPtr_->houseLit_MeshDataPtr, tempMeshData5);
        Project001::MeshLoader::RotateMeshX(*sharedDataPtr_->houseLit_MeshDataPtr, glm::half_pi<float>());
    }

    sharedDataPtr_->houseLit_TextureDataPtr = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTextureFromMemory(
        *sharedDataPtr_->houseLit_TextureDataPtr,
        g_houseLit_384x256_png,
        sizeof(g_houseLit_384x256_png) / sizeof(unsigned char)
    ));
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->houseLit_TextureId,
        sharedDataPtr_->houseLit_TextureDataPtr->data,
        sharedDataPtr_->houseLit_TextureDataPtr->width,
        sharedDataPtr_->houseLit_TextureDataPtr->height,
        sharedDataPtr_->houseLit_TextureDataPtr->bytesPerPixel,
        false, false
    );

    sharedDataPtr_->houseDark_MeshDataPtr = new Project001::MeshData();
    Project001::MeshLoader::CopyMesh(
        *sharedDataPtr_->houseDark_MeshDataPtr, *sharedDataPtr_->houseLit_MeshDataPtr
    );

    sharedDataPtr_->houseDark_TextureDataPtr = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTextureFromMemory(
        *sharedDataPtr_->houseDark_TextureDataPtr,
        g_houseDark_384x256_png,
        sizeof(g_houseDark_384x256_png) / sizeof(unsigned char)
    ));
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->houseDark_TextureId,
        sharedDataPtr_->houseDark_TextureDataPtr->data,
        sharedDataPtr_->houseDark_TextureDataPtr->width,
        sharedDataPtr_->houseDark_TextureDataPtr->height,
        sharedDataPtr_->houseDark_TextureDataPtr->bytesPerPixel,
        false, false
    );

    sharedDataPtr_->houseText_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->houseText_MeshDataPtr,
        *sharedDataPtr_->pixelFont_FontDataPtr,
        "HOME",
        2.0f
    ));
    Project001::MeshLoader::RecenterMesh(*sharedDataPtr_->houseText_MeshDataPtr);
    Project001::MeshLoader::RotateMeshX(*sharedDataPtr_->houseText_MeshDataPtr, glm::half_pi<float>());

    sharedDataPtr_->houseCollision_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRectangle(
        *sharedDataPtr_->houseCollision_MeshDataPtr,
        128.0f, 128.0f
    ));
}

void Scene001::LoadLampLightResources()
{
    const float& radius = sharedDataPtr_->lampLight_radius;

    sharedDataPtr_->lampLightTop_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::GenerateTruncatedCone(
        *sharedDataPtr_->lampLightTop_MeshDataPtr, 120.0f, 8.0f, radius, 16, false
    ));
    Project001::MeshLoader::RotateMeshX(
        *sharedDataPtr_->lampLightTop_MeshDataPtr, glm::half_pi<float>()
    );
    Project001::MeshLoader::TranslateMesh(
        *sharedDataPtr_->lampLightTop_MeshDataPtr, glm::vec3(0.0f, -16.0f, 60.0f)
    );

    sharedDataPtr_->lampLightBottom_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(
        *sharedDataPtr_->lampLightBottom_MeshDataPtr, radius, 16
    ));
    Project001::MeshLoader::TranslateMesh(
        *sharedDataPtr_->lampLightBottom_MeshDataPtr, glm::vec3(0.0f, -16.0f, 1.0f)
    );

    sharedDataPtr_->lampLightCollision_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(
        *sharedDataPtr_->lampLightCollision_MeshDataPtr, radius, 16
    ));
    Project001::MeshLoader::TranslateMesh(
        *sharedDataPtr_->lampLightCollision_MeshDataPtr, glm::vec3(0.0f, -16.0f, 0.0f)
    );
}

void Scene001::LoadLampResources()
{
    {
        Project001::MeshData tempMeshData0; // lamp neck
        FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
            tempMeshData0, 32.0f, 128.0f, 0.0f, 1.0f, 0.0f, 0.8f
        ));
        Project001::MeshLoader::TranslateMesh(tempMeshData0, glm::vec3(0.0f, 64.0f, 0.0f));

        Project001::MeshData tempMeshData1; // lamp head
        FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
            tempMeshData1, 32.0f, 32.0f, 0.0f, 1.0f, 0.8f, 1.0f
        ));
        Project001::MeshLoader::RotateMeshX(tempMeshData1, -glm::half_pi<float>());
        Project001::MeshLoader::TranslateMesh(tempMeshData1, glm::vec3(0.0f, 128.0f, 16.0f));

        sharedDataPtr_->lampLit_MeshDataPtr = new Project001::MeshData();
        Project001::MeshLoader::CopyMesh(*sharedDataPtr_->lampLit_MeshDataPtr, tempMeshData0);
        Project001::MeshLoader::CopyMesh(*sharedDataPtr_->lampLit_MeshDataPtr, tempMeshData1);
        Project001::MeshLoader::RotateMeshX(*sharedDataPtr_->lampLit_MeshDataPtr, glm::half_pi<float>());
    }

    sharedDataPtr_->lampLit_TextureDataPtr = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTextureFromMemory(
        *sharedDataPtr_->lampLit_TextureDataPtr,
        g_lampLit_32x160_png,
        sizeof(g_lampLit_32x160_png) / sizeof(unsigned char)
    ));
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->lampLit_TextureId,
        sharedDataPtr_->lampLit_TextureDataPtr->data,
        sharedDataPtr_->lampLit_TextureDataPtr->width,
        sharedDataPtr_->lampLit_TextureDataPtr->height,
        sharedDataPtr_->lampLit_TextureDataPtr->bytesPerPixel,
        false, false
    );

    sharedDataPtr_->lampDark_MeshDataPtr = new Project001::MeshData();
    Project001::MeshLoader::CopyMesh(
        *sharedDataPtr_->lampDark_MeshDataPtr, *sharedDataPtr_->lampLit_MeshDataPtr
    );

    sharedDataPtr_->lampDark_TextureDataPtr = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTextureFromMemory(
        *sharedDataPtr_->lampDark_TextureDataPtr,
        g_lampDark_32x160_png,
        sizeof(g_lampDark_32x160_png) / sizeof(unsigned char)
    ));
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->lampDark_TextureId,
        sharedDataPtr_->lampDark_TextureDataPtr->data,
        sharedDataPtr_->lampDark_TextureDataPtr->width,
        sharedDataPtr_->lampDark_TextureDataPtr->height,
        sharedDataPtr_->lampDark_TextureDataPtr->bytesPerPixel,
        false, false
    );

    sharedDataPtr_->lampCollision_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(
        *sharedDataPtr_->lampCollision_MeshDataPtr, 8.0f, 8
    ));
}

void Scene001::LoadPersonResources()
{
    constexpr float spriteWidth = 32.0f;
    constexpr float spriteHeight = 56.0f;

    sharedDataPtr_->monsterLit_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
        *sharedDataPtr_->monsterLit_MeshDataPtr, spriteWidth, spriteHeight, 0.0f, 1.0f, 0.0f, 1.0f
    ));
    Project001::MeshLoader::TranslateMesh(
        *sharedDataPtr_->monsterLit_MeshDataPtr, glm::vec3(0.0f, 0.5 * spriteHeight, 0.0f)
    );
    Project001::MeshLoader::RotateMeshX(*sharedDataPtr_->monsterLit_MeshDataPtr, glm::half_pi<float>());

    sharedDataPtr_->monsterLit_TextureDataPtr = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTextureFromMemory(
        *sharedDataPtr_->monsterLit_TextureDataPtr,
        g_monsterLit_32x56_png,
        sizeof(g_monsterLit_32x56_png) / sizeof(unsigned char)
    ));
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->monsterLit_TextureId,
        sharedDataPtr_->monsterLit_TextureDataPtr->data,
        sharedDataPtr_->monsterLit_TextureDataPtr->width,
        sharedDataPtr_->monsterLit_TextureDataPtr->height,
        sharedDataPtr_->monsterLit_TextureDataPtr->bytesPerPixel,
        false, false
    );

    sharedDataPtr_->monsterCollision_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(
        *sharedDataPtr_->monsterCollision_MeshDataPtr, 8.0f, 8
    ));

    sharedDataPtr_->monsterVisionCollision_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(
        *sharedDataPtr_->monsterVisionCollision_MeshDataPtr, sharedDataPtr_->monsterVision_radius, 16
    ));

    sharedDataPtr_->personLit_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
        *sharedDataPtr_->personLit_MeshDataPtr, spriteWidth, spriteHeight, 0.0f, 1.0f, 0.0f, 1.0f
    ));
    Project001::MeshLoader::TranslateMesh(
        *sharedDataPtr_->personLit_MeshDataPtr, glm::vec3(0.0f, 0.5 * spriteHeight, 0.0f)
    );
    Project001::MeshLoader::RotateMeshX(*sharedDataPtr_->personLit_MeshDataPtr, glm::half_pi<float>());

    sharedDataPtr_->personLit_TextureDataPtr = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTextureFromMemory(
        *sharedDataPtr_->personLit_TextureDataPtr,
        g_personLit_32x56_png,
        sizeof(g_personLit_32x56_png) / sizeof(unsigned char)
    ));
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->personLit_TextureId,
        sharedDataPtr_->personLit_TextureDataPtr->data,
        sharedDataPtr_->personLit_TextureDataPtr->width,
        sharedDataPtr_->personLit_TextureDataPtr->height,
        sharedDataPtr_->personLit_TextureDataPtr->bytesPerPixel,
        false, false
    );

    sharedDataPtr_->personCollision_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(
        *sharedDataPtr_->personCollision_MeshDataPtr, 8.0f, 8
    ));

    sharedDataPtr_->unknownDark_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
        *sharedDataPtr_->unknownDark_MeshDataPtr, spriteWidth, spriteHeight, 0.0f, 1.0f, 0.0f, 1.0f
    ));
    Project001::MeshLoader::TranslateMesh(
        *sharedDataPtr_->unknownDark_MeshDataPtr, glm::vec3(0.0f, 0.5 * spriteHeight, 0.0f)
    );
    Project001::MeshLoader::RotateMeshX(*sharedDataPtr_->unknownDark_MeshDataPtr, glm::half_pi<float>());

    sharedDataPtr_->unknownDark_TextureDataPtr = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTextureFromMemory(
        *sharedDataPtr_->unknownDark_TextureDataPtr,
        g_unknownDark_32x56_png,
        sizeof(g_unknownDark_32x56_png) / sizeof(unsigned char)
    ));
    GetRendererPtr()->CreateTexture(
        sharedDataPtr_->unknownDark_TextureId,
        sharedDataPtr_->unknownDark_TextureDataPtr->data,
        sharedDataPtr_->unknownDark_TextureDataPtr->width,
        sharedDataPtr_->unknownDark_TextureDataPtr->height,
        sharedDataPtr_->unknownDark_TextureDataPtr->bytesPerPixel,
        false, false
    );
}

void Scene001::LoadPlayerLightResources()
{
    {
        Project001::MeshData tempMeshData0;
        Project001::MeshData tempMeshData1;
        sharedDataPtr_->playerLightBottom_MeshDataPtr = new Project001::MeshData();
        sharedDataPtr_->playerLightTop_MeshDataPtr = new Project001::MeshData();
        std::vector<glm::vec3> capCorners;

        FAIL_CHECK(Project001::MeshLoader::GenerateIceCreamCup(
            tempMeshData0, 112.0f, 64.0f, 8.0f, 16, 8, false
        ));
        Project001::MeshLoader::TranslateMesh(
            tempMeshData0, glm::vec3(0.0f, 104.0f, 24.0f)
        );
        Project001::MeshLoader::SliceMeshWithAPlane(
            *sharedDataPtr_->playerLightTop_MeshDataPtr, tempMeshData1, capCorners, tempMeshData0, glm::vec3(0.0f, 0.0f, 1.0f), 0.0f
        );

        std::vector<glm::vec2> corners;
        for (size_t i = 0; i < capCorners.size(); ++i)
        {
            corners.emplace_back(capCorners[i].x, capCorners[i].y);
        }
        std::vector<glm::vec2> noDupCorners;
        Project001::RemoveDuplicates(noDupCorners, corners);
        Project001::OrderPointsCCW(noDupCorners, 0.0f);

        Project001::MeshLoader::Generate2DPolygon(
            *sharedDataPtr_->playerLightBottom_MeshDataPtr, noDupCorners
        );
        Project001::MeshLoader::TranslateMesh(
            *sharedDataPtr_->playerLightBottom_MeshDataPtr, glm::vec3(0.0f, -16.0f, 1.0f)
        );

        sharedDataPtr_->playerLightStrong_MeshDataPtr = new Project001::MeshData();
        FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(
            *sharedDataPtr_->playerLightStrong_MeshDataPtr, sharedDataPtr_->playerVision_radius, 32, false
        ));
        Project001::MeshLoader::TranslateMesh(
            *sharedDataPtr_->playerLightStrong_MeshDataPtr, glm::vec3(0.0f, 0.0f, 0.1f)
        );
    }

    sharedDataPtr_->playerLightCollision_MeshDataPtr = new Project001::MeshData();
    {
        std::vector<glm::vec2> corners;
        constexpr float radius = 60.0f;
        constexpr size_t subdivisions = 8;
        constexpr float angleRotation = glm::pi<float>() / static_cast<float>(subdivisions);
        for (size_t i = 0; i <= subdivisions; ++i)
        {
            glm::vec2 radiusVector(radius, 0.0f);
            radiusVector = Project001::Rotate2DVector(radiusVector, static_cast<float>(i) * angleRotation);
            corners.emplace_back(radiusVector);
        }
        corners.emplace_back(-4.0f, -112.0f);
        corners.emplace_back(4.0f, -112.0f);
        for (size_t i = 0; i < corners.size(); ++i)
        {
            corners[i].y += 128.0f;
        }
        FAIL_CHECK(Project001::MeshLoader::Generate2DPolygon(
            *sharedDataPtr_->playerLightCollision_MeshDataPtr, corners
        ));
    }
}

void Scene001::LoadUiTextResources()
{
    sharedDataPtr_->uiLeftBackground_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRectangle(
        *sharedDataPtr_->uiLeftBackground_MeshDataPtr, 96.0f, 48.0f
    ));

    sharedDataPtr_->uiLeftText01_MeshDataPtr = new Project001::MeshData();
    sharedDataPtr_->uiLeftText02_MeshDataPtr = new Project001::MeshData();
    sharedDataPtr_->uiLeftText03_MeshDataPtr = new Project001::MeshData();

    sharedDataPtr_->uiMiddleBackground_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRectangle(
        *sharedDataPtr_->uiMiddleBackground_MeshDataPtr, 74.0f, 16.0f
    ));

    sharedDataPtr_->uiMiddleText01_MeshDataPtr = new Project001::MeshData();

    sharedDataPtr_->uiPauseBackground_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRectangle(
        *sharedDataPtr_->uiPauseBackground_MeshDataPtr, 134.0f, 50.0f
    ));

    std::string pauseString01 = "* PAUSED *\n";
    std::string pauseString02 = "Press <ESC>\n";
    std::string pauseString03 = "to quit.";

    constexpr float pixelSize = 2.0f;

    sharedDataPtr_->uiPauseText01_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->uiPauseText01_MeshDataPtr,
        *sharedDataPtr_->pixelFont_FontDataPtr,
        pauseString01,
        pixelSize
    ));
    Project001::MeshLoader::TranslateMesh(
        *sharedDataPtr_->uiPauseText01_MeshDataPtr,
        glm::vec3(sharedDataPtr_->uiPauseText01_MeshDataPtr->GetSize().x * -0.5f, 0.0f, 0.0f)
    );

    sharedDataPtr_->uiPauseText02_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->uiPauseText02_MeshDataPtr,
        *sharedDataPtr_->pixelFont_FontDataPtr,
        pauseString02,
        pixelSize
    ));
    Project001::MeshLoader::TranslateMesh(
        *sharedDataPtr_->uiPauseText02_MeshDataPtr,
        glm::vec3(sharedDataPtr_->uiPauseText02_MeshDataPtr->GetSize().x * -0.5f, 0.0f, 0.0f)
    );

    sharedDataPtr_->uiPauseText03_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->uiPauseText03_MeshDataPtr,
        *sharedDataPtr_->pixelFont_FontDataPtr,
        pauseString03,
        pixelSize
    ));
    Project001::MeshLoader::TranslateMesh(
        *sharedDataPtr_->uiPauseText03_MeshDataPtr,
        glm::vec3(sharedDataPtr_->uiPauseText03_MeshDataPtr->GetSize().x * -0.5f, 0.0f, 0.0f)
    );

    sharedDataPtr_->uiMiniMapBackground_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRectangle(
        *sharedDataPtr_->uiMiniMapBackground_MeshDataPtr, 64.0f, 64.0f
    ));

    sharedDataPtr_->uiMiniMapHouse_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRectangle(
        *sharedDataPtr_->uiMiniMapHouse_MeshDataPtr, 4.0f, 4.0f
    ));

    sharedDataPtr_->uiMiniMapPlayer_MeshDataPtr = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRectangle(
        *sharedDataPtr_->uiMiniMapPlayer_MeshDataPtr, 2.0f, 2.0f
    ));
}

void Scene001::LoadSoundResources()
{
    sharedDataPtr_->hitHurt_SoundDataPtr = new Project001::SoundData();
    FAIL_CHECK(Project001::SoundLoader::LoadSoundWAVFromMemory(
        *sharedDataPtr_->hitHurt_SoundDataPtr,
        g_hitHurt_wav,
        sizeof(g_hitHurt_wav) / sizeof(unsigned char)
    ));

    FAIL_CHECK(GetSoundPlayerPtr()->CreateSoundBuffer(
        sharedDataPtr_->hitHurt_SoundBufferId,
        sharedDataPtr_->hitHurt_SoundDataPtr->data,
        sharedDataPtr_->hitHurt_SoundDataPtr->sizeInBytes,
        sharedDataPtr_->hitHurt_SoundDataPtr->numberOfChannels,
        sharedDataPtr_->hitHurt_SoundDataPtr->sampleRate_Hz,
        sharedDataPtr_->hitHurt_SoundDataPtr->bitsPerSample,
        sharedDataPtr_->hitHurt_SoundDataPtr->sizeInFrames
    ));

    FAIL_CHECK(GetSoundPlayerPtr()->CreateSoundSource(
        sharedDataPtr_->hitHurt_SoundSourceId,
        sharedDataPtr_->hitHurt_SoundBufferId
    ));

    sharedDataPtr_->pickupCoin_SoundDataPtr = new Project001::SoundData();
    FAIL_CHECK(Project001::SoundLoader::LoadSoundWAVFromMemory(
        *sharedDataPtr_->pickupCoin_SoundDataPtr,
        g_pickupCoin_wav,
        sizeof(g_pickupCoin_wav) / sizeof(unsigned char)
    ));

    FAIL_CHECK(GetSoundPlayerPtr()->CreateSoundBuffer(
        sharedDataPtr_->pickupCoin_SoundBufferId,
        sharedDataPtr_->pickupCoin_SoundDataPtr->data,
        sharedDataPtr_->pickupCoin_SoundDataPtr->sizeInBytes,
        sharedDataPtr_->pickupCoin_SoundDataPtr->numberOfChannels,
        sharedDataPtr_->pickupCoin_SoundDataPtr->sampleRate_Hz,
        sharedDataPtr_->pickupCoin_SoundDataPtr->bitsPerSample,
        sharedDataPtr_->pickupCoin_SoundDataPtr->sizeInFrames
    ));

    FAIL_CHECK(GetSoundPlayerPtr()->CreateSoundSource(
        sharedDataPtr_->pickupCoin_SoundSourceId,
        sharedDataPtr_->pickupCoin_SoundBufferId
    ));

    sharedDataPtr_->dropCoin_SoundDataPtr = new Project001::SoundData();
    FAIL_CHECK(Project001::SoundLoader::LoadSoundWAVFromMemory(
        *sharedDataPtr_->dropCoin_SoundDataPtr,
        g_dropCoin_wav,
        sizeof(g_dropCoin_wav) / sizeof(unsigned char)
    ));

    FAIL_CHECK(GetSoundPlayerPtr()->CreateSoundBuffer(
        sharedDataPtr_->dropCoin_SoundBufferId,
        sharedDataPtr_->dropCoin_SoundDataPtr->data,
        sharedDataPtr_->dropCoin_SoundDataPtr->sizeInBytes,
        sharedDataPtr_->dropCoin_SoundDataPtr->numberOfChannels,
        sharedDataPtr_->dropCoin_SoundDataPtr->sampleRate_Hz,
        sharedDataPtr_->dropCoin_SoundDataPtr->bitsPerSample,
        sharedDataPtr_->dropCoin_SoundDataPtr->sizeInFrames
    ));

    FAIL_CHECK(GetSoundPlayerPtr()->CreateSoundSource(
        sharedDataPtr_->dropCoin_SoundSourceId,
        sharedDataPtr_->dropCoin_SoundBufferId
    ));
}

void Scene001::FreeResources()
{
    GetRendererPtr()->DeleteAllTextures();
    GetRendererPtr()->DeleteAllMeshes();
    GetSoundPlayerPtr()->DeleteAllSoundBuffers();
    GetSoundPlayerPtr()->DeleteAllSoundSources();

    delete sharedDataPtr_->cursorCircle_MeshDataPtr;
    sharedDataPtr_->cursorCircle_MeshDataPtr = nullptr;

    delete sharedDataPtr_->introText_MeshDataPtr;
    sharedDataPtr_->introText_MeshDataPtr = nullptr;
    delete sharedDataPtr_->authorText_MeshDataPtr;
    sharedDataPtr_->authorText_MeshDataPtr = nullptr;

    delete sharedDataPtr_->groundDark_MeshDataPtr;
    sharedDataPtr_->groundDark_MeshDataPtr = nullptr;
    delete sharedDataPtr_->groundDark_TextureDataPtr;
    sharedDataPtr_->groundDark_TextureDataPtr = nullptr;
    sharedDataPtr_->groundDark_TextureId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->groundLit_MeshDataPtr;
    sharedDataPtr_->groundLit_MeshDataPtr = nullptr;
    delete sharedDataPtr_->groundLit_TextureDataPtr;
    sharedDataPtr_->groundLit_TextureDataPtr = nullptr;
    sharedDataPtr_->groundLit_TextureId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->groundFog_MeshDataPtr;
    sharedDataPtr_->groundFog_MeshDataPtr = nullptr;
    delete sharedDataPtr_->groundCollision_MeshDataPtr;
    sharedDataPtr_->groundCollision_MeshDataPtr = nullptr;
    delete sharedDataPtr_->groundCollisionQuadTree_MeshDataPtr;
    sharedDataPtr_->groundCollisionQuadTree_MeshDataPtr = nullptr;

    delete sharedDataPtr_->groundGrid_MeshDataPtr;
    sharedDataPtr_->groundGrid_MeshDataPtr = nullptr;
    sharedDataPtr_->groundGrid_MeshId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->groundGridLabels_MeshDataPtr;
    sharedDataPtr_->groundGridLabels_MeshDataPtr = nullptr;
    sharedDataPtr_->groundGridLabels_MeshId = static_cast<unsigned int>(-1);

    delete sharedDataPtr_->houseLit_MeshDataPtr;
    sharedDataPtr_->houseLit_MeshDataPtr = nullptr;
    delete sharedDataPtr_->houseLit_TextureDataPtr;
    sharedDataPtr_->houseLit_TextureDataPtr = nullptr;
    sharedDataPtr_->houseLit_TextureId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->houseDark_MeshDataPtr;
    sharedDataPtr_->houseDark_MeshDataPtr = nullptr;
    delete sharedDataPtr_->houseDark_TextureDataPtr;
    sharedDataPtr_->houseDark_TextureDataPtr = nullptr;
    sharedDataPtr_->houseDark_TextureId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->houseText_MeshDataPtr;
    sharedDataPtr_->houseText_MeshDataPtr = nullptr;
    delete sharedDataPtr_->houseCollision_MeshDataPtr;
    sharedDataPtr_->houseCollision_MeshDataPtr = nullptr;

    delete sharedDataPtr_->houseLightBottom_MeshDataPtr;
    sharedDataPtr_->houseLightBottom_MeshDataPtr = nullptr;
    delete sharedDataPtr_->houseLightTop_MeshDataPtr;
    sharedDataPtr_->houseLightTop_MeshDataPtr = nullptr;
    delete sharedDataPtr_->houseLightCollision_MeshDataPtr;
    sharedDataPtr_->houseLightCollision_MeshDataPtr = nullptr;
    delete sharedDataPtr_->houseDoorCollision_MeshDataPtr;
    sharedDataPtr_->houseDoorCollision_MeshDataPtr = nullptr;

    delete sharedDataPtr_->lampLit_MeshDataPtr;
    sharedDataPtr_->lampLit_MeshDataPtr = nullptr;
    delete sharedDataPtr_->lampLit_TextureDataPtr;
    sharedDataPtr_->lampLit_TextureDataPtr = nullptr;
    sharedDataPtr_->lampLit_TextureId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->lampDark_MeshDataPtr;
    sharedDataPtr_->lampDark_MeshDataPtr = nullptr;
    delete sharedDataPtr_->lampDark_TextureDataPtr;
    sharedDataPtr_->lampDark_TextureDataPtr = nullptr;
    sharedDataPtr_->lampDark_TextureId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->lampCollision_MeshDataPtr;
    sharedDataPtr_->lampCollision_MeshDataPtr = nullptr;

    delete sharedDataPtr_->lampLightBottom_MeshDataPtr;
    sharedDataPtr_->lampLightBottom_MeshDataPtr = nullptr;
    delete sharedDataPtr_->lampLightTop_MeshDataPtr;
    sharedDataPtr_->lampLightTop_MeshDataPtr = nullptr;
    delete sharedDataPtr_->lampLightCollision_MeshDataPtr;
    sharedDataPtr_->lampLightCollision_MeshDataPtr = nullptr;

    delete sharedDataPtr_->monsterLit_MeshDataPtr;
    sharedDataPtr_->monsterLit_MeshDataPtr = nullptr;
    delete sharedDataPtr_->monsterLit_TextureDataPtr;
    sharedDataPtr_->monsterLit_TextureDataPtr = nullptr;
    sharedDataPtr_->monsterLit_TextureId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->monsterCollision_MeshDataPtr;
    sharedDataPtr_->monsterCollision_MeshDataPtr = nullptr;
    delete sharedDataPtr_->monsterVisionCollision_MeshDataPtr;
    sharedDataPtr_->monsterVisionCollision_MeshDataPtr = nullptr;

    delete sharedDataPtr_->personLit_MeshDataPtr;
    sharedDataPtr_->personLit_MeshDataPtr = nullptr;
    delete sharedDataPtr_->personLit_TextureDataPtr;
    sharedDataPtr_->personLit_TextureDataPtr = nullptr;
    sharedDataPtr_->personLit_TextureId = static_cast<unsigned int>(-1);
    delete sharedDataPtr_->personCollision_MeshDataPtr;
    sharedDataPtr_->personCollision_MeshDataPtr = nullptr;

    delete sharedDataPtr_->unknownDark_MeshDataPtr;
    sharedDataPtr_->unknownDark_MeshDataPtr = nullptr;
    delete sharedDataPtr_->unknownDark_TextureDataPtr;
    sharedDataPtr_->unknownDark_TextureDataPtr = nullptr;
    sharedDataPtr_->unknownDark_TextureId = static_cast<unsigned int>(-1);

    delete sharedDataPtr_->playerLightBottom_MeshDataPtr;
    sharedDataPtr_->playerLightBottom_MeshDataPtr = nullptr;
    delete sharedDataPtr_->playerLightTop_MeshDataPtr;
    sharedDataPtr_->playerLightTop_MeshDataPtr = nullptr;
    delete sharedDataPtr_->playerLightStrong_MeshDataPtr;
    sharedDataPtr_->playerLightStrong_MeshDataPtr = nullptr;
    delete sharedDataPtr_->playerLightCollision_MeshDataPtr;
    sharedDataPtr_->playerLightCollision_MeshDataPtr = nullptr;

    delete sharedDataPtr_->uiLeftBackground_MeshDataPtr;
    sharedDataPtr_->uiLeftBackground_MeshDataPtr = nullptr;
    delete sharedDataPtr_->uiLeftText01_MeshDataPtr;
    sharedDataPtr_->uiLeftText01_MeshDataPtr = nullptr;
    delete sharedDataPtr_->uiLeftText02_MeshDataPtr;
    sharedDataPtr_->uiLeftText02_MeshDataPtr = nullptr;
    delete sharedDataPtr_->uiLeftText03_MeshDataPtr;
    sharedDataPtr_->uiLeftText03_MeshDataPtr = nullptr;
    delete sharedDataPtr_->uiMiddleBackground_MeshDataPtr;
    sharedDataPtr_->uiMiddleBackground_MeshDataPtr = nullptr;
    delete sharedDataPtr_->uiMiddleText01_MeshDataPtr;
    sharedDataPtr_->uiMiddleText01_MeshDataPtr = nullptr;
    delete sharedDataPtr_->uiMiniMapBackground_MeshDataPtr;
    sharedDataPtr_->uiMiniMapBackground_MeshDataPtr = nullptr;
    delete sharedDataPtr_->uiMiniMapHouse_MeshDataPtr;
    sharedDataPtr_->uiMiniMapHouse_MeshDataPtr = nullptr;
    delete sharedDataPtr_->uiMiniMapPlayer_MeshDataPtr;
    sharedDataPtr_->uiMiniMapPlayer_MeshDataPtr = nullptr;
    delete sharedDataPtr_->uiPauseBackground_MeshDataPtr;
    sharedDataPtr_->uiPauseBackground_MeshDataPtr = nullptr;
    delete sharedDataPtr_->uiPauseText01_MeshDataPtr;
    sharedDataPtr_->uiPauseText01_MeshDataPtr = nullptr;
    delete sharedDataPtr_->uiPauseText02_MeshDataPtr;
    sharedDataPtr_->uiPauseText02_MeshDataPtr = nullptr;
    delete sharedDataPtr_->uiPauseText03_MeshDataPtr;
    sharedDataPtr_->uiPauseText03_MeshDataPtr = nullptr;

    delete sharedDataPtr_->hitHurt_SoundDataPtr;
    sharedDataPtr_->hitHurt_SoundDataPtr = nullptr;
    delete sharedDataPtr_->pickupCoin_SoundDataPtr;
    sharedDataPtr_->pickupCoin_SoundDataPtr = nullptr;
    delete sharedDataPtr_->dropCoin_SoundDataPtr;
    sharedDataPtr_->dropCoin_SoundDataPtr = nullptr;
}

void Scene001::ReadConfigFile()
{
    std::ifstream inputStream("TrickOrTreatOnSpookyStreet.ini");

    if (inputStream.is_open())
    {
        configFileFound_ = true;

        std::map<std::string, std::map<std::string, std::string>> sections;
        Project001::ReadIniStream(sections, inputStream);

        std::map<std::string, std::map<std::string, std::string>>::const_iterator iter = sections.find("Player_Controls");
        if (iter != sections.end())
        {
            std::map<std::string, std::string>::const_iterator iter2 = iter->second.find("sprint");
            if (iter2 != iter->second.end())
            {
                const std::string& value = iter2->second;
                sharedDataPtr_->sprint_KeyCode = Project001::StringToKeyCode(value);
                sharedDataPtr_->sprint_MouseButton = Project001::StringToMouseButton(value);
                sharedDataPtr_->sprint_usesKeyboard = sharedDataPtr_->sprint_MouseButton == Project001::MouseButton::MOUSE_BUTTON_UNKNOWN;
            }

            iter2 = iter->second.find("shine");
            if (iter2 != iter->second.end())
            {
                const std::string& value = iter2->second;
                sharedDataPtr_->shine_KeyCode = Project001::StringToKeyCode(value);
                sharedDataPtr_->shine_MouseButton = Project001::StringToMouseButton(value);
                sharedDataPtr_->shine_usesKeyboard = sharedDataPtr_->shine_MouseButton == Project001::MouseButton::MOUSE_BUTTON_UNKNOWN;
            }

            iter2 = iter->second.find("pause");
            if (iter2 != iter->second.end())
            {
                const std::string& value = iter2->second;
                sharedDataPtr_->pause_KeyCode = Project001::StringToKeyCode(value);
                sharedDataPtr_->pause_MouseButton = Project001::StringToMouseButton(value);
                sharedDataPtr_->pause_usesKeyboard = sharedDataPtr_->pause_MouseButton == Project001::MouseButton::MOUSE_BUTTON_UNKNOWN;
            }

            iter2 = iter->second.find("quit");
            if (iter2 != iter->second.end())
            {
                const std::string& value = iter2->second;
                sharedDataPtr_->quit_KeyCode = Project001::StringToKeyCode(value);
                sharedDataPtr_->quit_MouseButton = Project001::StringToMouseButton(value);
                sharedDataPtr_->quit_usesKeyboard = sharedDataPtr_->quit_MouseButton == Project001::MouseButton::MOUSE_BUTTON_UNKNOWN;
            }

            iter2 = iter->second.find("up");
            if (iter2 != iter->second.end())
            {
                const std::string& value = iter2->second;
                sharedDataPtr_->up_KeyCode = Project001::StringToKeyCode(value);
                sharedDataPtr_->up_MouseButton = Project001::StringToMouseButton(value);
                sharedDataPtr_->up_usesKeyboard = sharedDataPtr_->up_MouseButton == Project001::MouseButton::MOUSE_BUTTON_UNKNOWN;
            }

            iter2 = iter->second.find("left");
            if (iter2 != iter->second.end())
            {
                const std::string& value = iter2->second;
                sharedDataPtr_->left_KeyCode = Project001::StringToKeyCode(value);
                sharedDataPtr_->left_MouseButton = Project001::StringToMouseButton(value);
                sharedDataPtr_->left_usesKeyboard = sharedDataPtr_->left_MouseButton == Project001::MouseButton::MOUSE_BUTTON_UNKNOWN;
            }

            iter2 = iter->second.find("down");
            if (iter2 != iter->second.end())
            {
                const std::string& value = iter2->second;
                sharedDataPtr_->down_KeyCode = Project001::StringToKeyCode(value);
                sharedDataPtr_->down_MouseButton = Project001::StringToMouseButton(value);
                sharedDataPtr_->down_usesKeyboard = sharedDataPtr_->down_MouseButton == Project001::MouseButton::MOUSE_BUTTON_UNKNOWN;
            }

            iter2 = iter->second.find("right");
            if (iter2 != iter->second.end())
            {
                const std::string& value = iter2->second;
                sharedDataPtr_->right_KeyCode = Project001::StringToKeyCode(value);
                sharedDataPtr_->right_MouseButton = Project001::StringToMouseButton(value);
                sharedDataPtr_->right_usesKeyboard = sharedDataPtr_->right_MouseButton == Project001::MouseButton::MOUSE_BUTTON_UNKNOWN;
            }

            iter2 = iter->second.find("sprint_button");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->sprint_joystickButtonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("pause_button");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->pause_joystickButtonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("quit_button");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->quit_joystickButtonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("up_button");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->up_joystickButtonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("left_button");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->left_joystickButtonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("down_button");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->down_joystickButtonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("right_button");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->right_joystickButtonIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("moveRightLeft_axis");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->moveRightLeft_joystickAxisIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("moveDownUp_axis");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->moveDownUp_joystickAxisIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("aimRightLeft_axis");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->aimRightLeft_joystickAxisIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("aimDownUp_axis");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->aimDownUp_joystickAxisIndex = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("moveDeadzone");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->move_joystickAxisDeadzone = std::stof(iter2->second);
            }

            iter2 = iter->second.find("aimDeadzone");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->aim_joystickAxisDeadzone = std::stof(iter2->second);
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

            iter2 = iter->second.find("startingGameTime_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->startingGameTime_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("house_initialOnCount");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->house_initialOnCount = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("person_initalCount");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->person_initalCount = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("monster_initialCount");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->monster_initialCount = std::stoi(iter2->second);
            }

            iter2 = iter->second.find("house_randomTurnOnWait_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->house_randomTurnOnWait_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("house_turnOffWait_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->house_turnOffWait_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("person_walkSpeed");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->person_walkSpeed = std::stof(iter2->second);
            }

            iter2 = iter->second.find("person_walkDurationMin_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->person_walkDurationMin_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("person_walkDurationMax_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->person_walkDurationMax_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("player_initialBattery_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->player_initialBattery_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("player_initialStamina_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->player_initialStamina_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("player_staminaRechargeRate");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->player_staminaRechargeRate = std::stof(iter2->second);
            }

            iter2 = iter->second.find("player_walkSpeed");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->player_walkSpeed = std::stof(iter2->second);
            }

            iter2 = iter->second.find("player_runSpeed");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->player_runSpeed = std::stof(iter2->second);
            }

            iter2 = iter->second.find("monster_walkSpeed");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->monster_walkSpeed = std::stof(iter2->second);
            }

            iter2 = iter->second.find("monster_runSpeed");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->monster_runSpeed = std::stof(iter2->second);
            }

            iter2 = iter->second.find("monster_walkDurationMin_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->monster_walkDurationMin_s = std::stof(iter2->second);
            }

            iter2 = iter->second.find("monster_walkDurationMax_s");
            if (iter2 != iter->second.end())
            {
                sharedDataPtr_->monster_walkDurationMax_s = std::stof(iter2->second);
            }
        }
    }
}

void Scene001::CreateUiCameraEntity()
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
        cameraPtr->SetCameraMask(s_uiCamera_Mask_);
        cameraPtr->SetPriorityValue(1000);
    }
}

void Scene001::CreateIntroTextEntity()
{
    constexpr float infoPixelSize = 2.0f;

    sharedDataPtr_->introText_MeshDataPtr->Clear();

    std::string introString;

    if (sharedDataPtr_->score > 0)
    {
        introString += "Your score was: " + std::to_string(sharedDataPtr_->score) + "\n";
    }
    else
    {
        introString += "\n";
    }

    introString += std::string(
        "\n"
        "Trick or Treat on Spooky Street\n"
        "\n"
        "version 0.2.x\n"
        "\n"
        "\n"
        "Knock on the doors of lit-up houses to collect candy.\n"
        "Return it home to score points and charge your flashlight.\n"
        "Watch out for monsters!\n"
        "\n"
        "\n"
        "Default Controls | Mouse & Keyboard  | Controller\n"
        "-----------------+-------------------+--------------------\n"
        "Movement         | W-A-S-D Keys      | D-Pad or Left Stick\n"
        "Aim              | Mouse Position    | Right Stick\n"
        "Flash Light      | Left Mouse Button | Right Stick\n"
        "Sprint           | Space             | L Bumper Button\n"
        "Pause / Start    | Enter             | Menu Button\n"
        "Quit             | Esc               | View Button\n"
        "\n"
        "\n"
    );

    if (configFileFound_)
    {
        introString += "Config file was found.\n\n\n";
    }
    else
    {
        introString += "\n\n\n\n";
    }

    introString += "Press Enter or Menu Button to start.";

    FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->introText_MeshDataPtr,
        *sharedDataPtr_->pixelFont_FontDataPtr,
        introString,
        infoPixelSize
    ));

    GetComponentStoresPtr()->CreateEntity(introText_EntityId_);
    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(introText_EntityId_));
    Project001::RenderedMesh* renderedMeshPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, introText_EntityId_));
    if (renderedMeshPtr != nullptr)
    {
        renderedMeshPtr->SetCameraMask(s_uiCamera_Mask_);
        renderedMeshPtr->SetMeshDataPtr(sharedDataPtr_->introText_MeshDataPtr);
        renderedMeshPtr->SetTextureId(sharedDataPtr_->pixelFont_TextureId);
        renderedMeshPtr->SetColor(0.6f, 0.6f, 0.6f, 1.0f);
        renderedMeshPtr->SetUseLighting(false);
        renderedMeshPtr->SetPosition(-384.0f, 256.0f, 0.0f);
    }

    constexpr float authorPixelSize = 2.0f;

    sharedDataPtr_->authorText_MeshDataPtr->Clear();

    std::string authorString = "created by VIK PANDHER";

    FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->authorText_MeshDataPtr,
        *sharedDataPtr_->pixelFont_FontDataPtr,
        authorString,
        authorPixelSize
    ));
    Project001::MeshLoader::TranslateMesh(
        *sharedDataPtr_->authorText_MeshDataPtr,
        glm::vec3((authorString.length() - 1) * authorPixelSize * -6.0f, 0.0f, 0.0f)
    );

    GetComponentStoresPtr()->CreateEntity(autorText_EntityId_);
    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(autorText_EntityId_));
    renderedMeshPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, autorText_EntityId_));
    if (renderedMeshPtr != nullptr)
    {
        renderedMeshPtr->SetCameraMask(s_uiCamera_Mask_);
        renderedMeshPtr->SetMeshDataPtr(sharedDataPtr_->authorText_MeshDataPtr);
        renderedMeshPtr->SetTextureId(sharedDataPtr_->pixelFont_TextureId);
        renderedMeshPtr->SetColor(0.2f, 0.2f, 0.2f, 1.0f);
        renderedMeshPtr->SetUseLighting(false);
        renderedMeshPtr->SetPosition(384.0f, -256.0f, 0.0f);
    }
}