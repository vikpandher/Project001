#include "TestScene003.h"

#include "Engine/Components/RenderedModel.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshStores.h"
#include "Engine/Renderer.h"
#include "Engine/TextureStores.h"
#include "Engine/Window.h"



// public: ---------------------------------------------------------------------

TestScene003::TestScene003()
{
    ClearIndiciesAndEntityIds();
}

TestScene003::~TestScene003()
{}

const char* TestScene003::Name()
{
    return "TestScene003";
}

void TestScene003::Initialize()
{
    TestSceneBase001::Initialize();

    // Load meshes
    // -------------------------------------------------------------------------

    {
        // duplicate position at start
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(0.0f, 0.48f);
        positions.emplace_back(0.0f, 0.64f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape01MeshIndex_, positions, 0.24f));
        _FAIL_CHECK(meshStoresPtr_->ApplyPositionalTextureCoordinates(shape01MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->RecenterMesh(shape01MeshIndex_));
    }

    {
        // duplicate position at end
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.16f, 0.0f);
        positions.emplace_back(0.16f, 0.16f);
        positions.emplace_back(0.16f, 0.32f);
        positions.emplace_back(0.16f, 0.64f);
        positions.emplace_back(0.16f, 0.64f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape02MeshIndex_, positions, 0.24f));
        _FAIL_CHECK(meshStoresPtr_->ApplyPositionalTextureCoordinates(shape02MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->RecenterMesh(shape02MeshIndex_));
    }

    {
        // duplicate position in middle
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.32f, 0.16f);
        positions.emplace_back(0.32f, 0.32f);
        positions.emplace_back(0.32f, 0.32f);
        positions.emplace_back(0.32f, 0.64f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape03MeshIndex_, positions, 0.24f));
        _FAIL_CHECK(meshStoresPtr_->ApplyPositionalTextureCoordinates(shape03MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->RecenterMesh(shape03MeshIndex_));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.32f, -0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.32f, 0.32f); // same slope
        positions.emplace_back(-0.32f, -0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape04MeshIndex_, positions, 0.24f));
        _FAIL_CHECK(meshStoresPtr_->ApplyPositionalTextureCoordinates(shape04MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->RecenterMesh(shape04MeshIndex_));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, 0.32f);
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.32f, -0.32f); // same slope
        positions.emplace_back(-0.32f, -0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape05MeshIndex_, positions, 0.24f));
        _FAIL_CHECK(meshStoresPtr_->ApplyPositionalTextureCoordinates(shape05MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->RecenterMesh(shape05MeshIndex_));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, -0.32f);
        positions.emplace_back(0.0f, -0.32f);
        positions.emplace_back(0.32f, -0.32f); // same slope
        positions.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape06MeshIndex_, positions, 0.24f));
        _FAIL_CHECK(meshStoresPtr_->ApplyPositionalTextureCoordinates(shape06MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->RecenterMesh(shape06MeshIndex_));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, 0.0f);
        positions.emplace_back(0.0f, -0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape07MeshIndex_, positions, 0.24f));
        _FAIL_CHECK(meshStoresPtr_->ApplyPositionalTextureCoordinates(shape07MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->RecenterMesh(shape07MeshIndex_));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.32f, -0.32f);
        positions.emplace_back(0.32f, 0.32f);
        positions.emplace_back(-0.32f, -0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLineLoop(shape08MeshIndex_, positions, 0.24f));
        _FAIL_CHECK(meshStoresPtr_->ApplyPositionalTextureCoordinates(shape08MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->RecenterMesh(shape08MeshIndex_));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, 0.32f);
        positions.emplace_back(0.32f, -0.32f);
        positions.emplace_back(-0.32f, -0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLineLoop(shape09MeshIndex_, positions, 0.24f));
        _FAIL_CHECK(meshStoresPtr_->ApplyPositionalTextureCoordinates(shape09MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->RecenterMesh(shape09MeshIndex_));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, -0.32f);
        positions.emplace_back(0.32f, -0.32f);
        positions.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLineLoop(shape10MeshIndex_, positions, 0.24f));
        _FAIL_CHECK(meshStoresPtr_->ApplyPositionalTextureCoordinates(shape10MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->RecenterMesh(shape10MeshIndex_));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, 0.0f);
        positions.emplace_back(0.0f, -0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLineLoop(shape11MeshIndex_, positions, 0.24f));
        _FAIL_CHECK(meshStoresPtr_->ApplyPositionalTextureCoordinates(shape11MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->RecenterMesh(shape11MeshIndex_));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.08f, 0.16f);
        positions.emplace_back(0.16f, -0.08f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.16f, 0.08f);
        positions.emplace_back(-0.08f, -0.16f);
        positions.emplace_back(0.0f, -0.32f);
        positions.emplace_back(0.08f, -0.16f);
        positions.emplace_back(-0.16f, 0.08f);
        positions.emplace_back(-0.32f, 0.0f);
        positions.emplace_back(-0.16f, -0.08f);
        positions.emplace_back(0.08f, 0.16f);
        positions.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLineLoop(shape12MeshIndex_, positions, 0.04f));
        _FAIL_CHECK(meshStoresPtr_->ApplyPositionalTextureCoordinates(shape12MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->RecenterMesh(shape12MeshIndex_));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32, -0.04f);
        positions.emplace_back(0.08, -0.04f);
        positions.emplace_back(-0.08, 0.04f);
        positions.emplace_back(0.32, 0.04f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape13MeshIndex_, positions, 0.08f));
        _FAIL_CHECK(meshStoresPtr_->ApplyPositionalTextureCoordinates(shape13MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->RecenterMesh(shape13MeshIndex_));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32, -0.04f);
        positions.emplace_back(0.08, -0.04f);
        positions.emplace_back(-0.08, 0.04f);
        positions.emplace_back(0.32, 0.04f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape14MeshIndex_, positions, 0.12f));
        _FAIL_CHECK(meshStoresPtr_->ApplyPositionalTextureCoordinates(shape14MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->RecenterMesh(shape14MeshIndex_));
    }

    _FAIL_CHECK(meshStoresPtr_->Generate2DSprite(shape15MeshIndex_, 0.48f, 0.64f, 0.0f, 13.0f / 32.0f, 1.0f - 16.0f / 32.0f, 1.0f));

    _FAIL_CHECK(meshStoresPtr_->CopyMesh(shape16MeshIndex_, shape15MeshIndex_));
    _FAIL_CHECK(meshStoresPtr_->RotateTextureCoordinates(shape16MeshIndex_, -glm::quarter_pi<float>()));

    _FAIL_CHECK(meshStoresPtr_->CopyMesh(shape17MeshIndex_, shape15MeshIndex_));
    _FAIL_CHECK(meshStoresPtr_->ScaleTextureCoordinates(shape17MeshIndex_, glm::vec2(0.5f, 0.5f)));

    _FAIL_CHECK(meshStoresPtr_->CopyMesh(shape18MeshIndex_, shape15MeshIndex_));
    _FAIL_CHECK(meshStoresPtr_->TranslateTextureCoordinates(shape18MeshIndex_, glm::vec2(0.06f / 0.32f, -0.08f / 0.32f)));

    _FAIL_CHECK(meshStoresPtr_->Generate2DSprite(shape19MeshIndex_, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));

    // Load textures
    // -------------------------------------------------------------------------

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_00_TextureIndex_, "../Textures/32x32_00.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_00_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_00_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_01_TextureIndex_, "../Textures/32x32_01.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_01_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_01_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_02_TextureIndex_, "../Textures/32x32_02.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_02_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_02_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_03_TextureIndex_, "../Textures/32x32_03.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_03_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_03_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_04_TextureIndex_, "../Textures/32x32_04.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_04_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_04_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_05_TextureIndex_, "../Textures/32x32_05.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_05_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_05_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_06_TextureIndex_, "../Textures/32x32_06.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_06_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_06_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_07_TextureIndex_, "../Textures/32x32_07.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_07_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_07_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_08_TextureIndex_, "../Textures/32x32_08.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_08_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_08_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_09_TextureIndex_, "../Textures/32x32_09.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_09_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_09_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_10_TextureIndex_, "../Textures/32x32_10.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_10_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_10_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_11_TextureIndex_, "../Textures/32x32_11.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_11_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_11_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_12_TextureIndex_, "../Textures/32x32_12.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_12_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_12_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_13_TextureIndex_, "../Textures/32x32_13.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_13_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_13_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_14_TextureIndex_, "../Textures/32x32_14.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_14_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_14_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_15_TextureIndex_, "../Textures/32x32_15.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_15_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_15_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_16_TextureIndex_, "../Textures/32x32_16.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_16_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_16_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_17_TextureIndex_, "../Textures/32x32_17.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_17_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_17_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_18_TextureIndex_, "../Textures/32x32_18.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_18_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_18_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_19_TextureIndex_, "../Textures/32x32_19.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_19_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_19_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_20_TextureIndex_, "../Textures/32x32_20.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_20_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_20_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_21_TextureIndex_, "../Textures/32x32_21.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_21_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_21_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_22_TextureIndex_, "../Textures/32x32_22.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_22_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_22_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_23_TextureIndex_, "../Textures/32x32_23.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_23_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_23_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_24_TextureIndex_, "../Textures/32x32_24.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_24_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_24_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_25_TextureIndex_, "../Textures/32x32_25.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_25_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_25_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_26_TextureIndex_, "../Textures/32x32_26.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_26_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_26_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_27_TextureIndex_, "../Textures/32x32_27.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_27_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_27_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_28_TextureIndex_, "../Textures/32x32_28.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_28_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_28_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_29_TextureIndex_, "../Textures/32x32_29.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_29_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_29_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_30_TextureIndex_, "../Textures/32x32_30.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_30_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_30_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_31_TextureIndex_, "../Textures/32x32_31.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_31_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_31_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_32_TextureIndex_, "../Textures/32x32_32.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_32_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_32_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_33_TextureIndex_, "../Textures/32x32_33.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_33_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_33_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_32x32_34_TextureIndex_, "../Textures/32x32_34.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_32x32_34_TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_32x32_34_TextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    // Calculating positions
    // -------------------------------------------------------------------------

    std::vector<glm::vec3> modelEntityPositions;
    for (int i = 2; i >= -2; --i)
    {
        for (int j = -3; j <= 3; ++j)
        {
            modelEntityPositions.emplace_back((float)j, (float)i, 0.0f);
        }
    }
    size_t positionPosition = 0;

    // generated shape entity 01
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape01EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape01EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape01EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape01MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_00_TextureIndex_);
    }

    // generated shape entity 02
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape02EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape02EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape02EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape02MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_01_TextureIndex_);
    }

    // generated shape entity 03
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape03EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape03EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape03EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape03MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_02_TextureIndex_);
    }

    // generated shape entity 04
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape04EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape04EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape04EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape04MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_03_TextureIndex_);
    }

    // generated shape entity 05
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape05EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape05EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape05EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape05MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_04_TextureIndex_);
    }

    // generated shape entity 06
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape06EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape06EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape06EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape06MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_05_TextureIndex_);
    }

    // generated shape entity 07
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape07EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape07EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape07EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape07MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_06_TextureIndex_);
    }

    // generated shape entity 08
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape08EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape08EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape08EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape08MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_07_TextureIndex_);
    }

    // generated shape entity 09
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape09EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape09EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape09EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape09MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_08_TextureIndex_);
    }

    // generated shape entity 10
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape10EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape10EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape10EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape10MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_09_TextureIndex_);
    }

    // generated shape entity 11
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape11EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape11EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape11EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape11MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_10_TextureIndex_);
    }

    // generated shape entity 12
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape12EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape12EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape12EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape12MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_11_TextureIndex_);
    }

    // generated shape entity 13
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape13EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape13EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape13EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape13MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_12_TextureIndex_);
    }

    // generated shape entity 14
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape14EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape14EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape14EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape14MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_13_TextureIndex_);
    }

    // generated shape entity 15
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape15EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape15EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape15EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape15MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_14_TextureIndex_);
    }

    // generated shape entity 16
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape16EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape16EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape16EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape16MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_15_TextureIndex_);
    }

    // generated shape entity 17
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape17EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape17EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape17EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape17MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_16_TextureIndex_);
    }

    // generated shape entity 18
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape18EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape18EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape18EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape18MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_17_TextureIndex_);
    }

    // generated shape entity 19
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape19EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape19EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape19EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape19MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_18_TextureIndex_);
    }

    // generated shape entity 20
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape20EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape20EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape20EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape19MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_19_TextureIndex_);
    }

    // generated shape entity 21
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape21EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape21EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape21EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape19MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_20_TextureIndex_);
    }

    // generated shape entity 22
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape22EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape22EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape22EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape19MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_21_TextureIndex_);
    }

    // generated shape entity 23
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape23EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape23EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape23EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape19MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_22_TextureIndex_);
    }

    // generated shape entity 24
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape24EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape24EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape24EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape19MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_23_TextureIndex_);
    }

    // generated shape entity 25
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape25EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape25EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape25EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape19MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_24_TextureIndex_);
    }

    // generated shape entity 26
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape26EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape26EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape26EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape19MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_25_TextureIndex_);
    }

    // generated shape entity 27
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape27EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape27EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape27EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape19MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_26_TextureIndex_);
    }

    // generated shape entity 28
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape28EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape28EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape28EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape19MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_27_TextureIndex_);
    }

    // generated shape entity 29
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape29EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape29EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape29EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape19MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_28_TextureIndex_);
    }

    // generated shape entity 30
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape30EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape30EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape30EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape19MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_29_TextureIndex_);
    }

    // generated shape entity 31
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape31EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape31EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape31EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape19MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_30_TextureIndex_);
    }

    // generated shape entity 32
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape32EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape32EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape32EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape19MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_31_TextureIndex_);
    }

    // generated shape entity 33
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape33EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape33EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape33EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape19MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_32_TextureIndex_);
    }

    // generated shape entity 34
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape34EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape34EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape34EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape19MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_33_TextureIndex_);
    }

    // generated shape entity 35
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape35EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape35EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape35EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape19MeshIndex_);
        renderedModelPtr->SetTextureIndex(_32x32_34_TextureIndex_);
    }
}

void TestScene003::Deinitialize()
{
    TestSceneBase001::Deinitialize();

    ClearIndiciesAndEntityIds();
}

void TestScene003::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene003::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneBase001::OnEvent(event);
}

// protected: ------------------------------------------------------------------

void TestScene003::ClearIndiciesAndEntityIds()
{
    shape01MeshIndex_ = (unsigned int)-1;
    shape02MeshIndex_ = (unsigned int)-1;
    shape03MeshIndex_ = (unsigned int)-1;
    shape04MeshIndex_ = (unsigned int)-1;
    shape05MeshIndex_ = (unsigned int)-1;
    shape06MeshIndex_ = (unsigned int)-1;
    shape07MeshIndex_ = (unsigned int)-1;
    shape08MeshIndex_ = (unsigned int)-1;
    shape09MeshIndex_ = (unsigned int)-1;
    shape10MeshIndex_ = (unsigned int)-1;
    shape11MeshIndex_ = (unsigned int)-1;
    shape12MeshIndex_ = (unsigned int)-1;
    shape13MeshIndex_ = (unsigned int)-1;
    shape14MeshIndex_ = (unsigned int)-1;
    shape15MeshIndex_ = (unsigned int)-1;
    shape16MeshIndex_ = (unsigned int)-1;
    shape17MeshIndex_ = (unsigned int)-1;
    shape18MeshIndex_ = (unsigned int)-1;
    shape19MeshIndex_ = (unsigned int)-1;
    shape20MeshIndex_ = (unsigned int)-1;
    shape21MeshIndex_ = (unsigned int)-1;
    shape22MeshIndex_ = (unsigned int)-1;
    shape23MeshIndex_ = (unsigned int)-1;
    shape24MeshIndex_ = (unsigned int)-1;
    shape25MeshIndex_ = (unsigned int)-1;
    shape26MeshIndex_ = (unsigned int)-1;
    shape27MeshIndex_ = (unsigned int)-1;
    shape28MeshIndex_ = (unsigned int)-1;
    shape29MeshIndex_ = (unsigned int)-1;
    shape30MeshIndex_ = (unsigned int)-1;
    shape31MeshIndex_ = (unsigned int)-1;
    shape32MeshIndex_ = (unsigned int)-1;
    shape33MeshIndex_ = (unsigned int)-1;
    shape34MeshIndex_ = (unsigned int)-1;
    shape35MeshIndex_ = (unsigned int)-1;

    _32x32_00_TextureIndex_ = (unsigned int)-1;
    _32x32_01_TextureIndex_ = (unsigned int)-1;
    _32x32_02_TextureIndex_ = (unsigned int)-1;
    _32x32_03_TextureIndex_ = (unsigned int)-1;
    _32x32_04_TextureIndex_ = (unsigned int)-1;
    _32x32_05_TextureIndex_ = (unsigned int)-1;
    _32x32_06_TextureIndex_ = (unsigned int)-1;
    _32x32_07_TextureIndex_ = (unsigned int)-1;
    _32x32_08_TextureIndex_ = (unsigned int)-1;
    _32x32_09_TextureIndex_ = (unsigned int)-1;
    _32x32_10_TextureIndex_ = (unsigned int)-1;
    _32x32_11_TextureIndex_ = (unsigned int)-1;
    _32x32_12_TextureIndex_ = (unsigned int)-1;
    _32x32_13_TextureIndex_ = (unsigned int)-1;
    _32x32_14_TextureIndex_ = (unsigned int)-1;
    _32x32_15_TextureIndex_ = (unsigned int)-1;
    _32x32_16_TextureIndex_ = (unsigned int)-1;
    _32x32_17_TextureIndex_ = (unsigned int)-1;
    _32x32_18_TextureIndex_ = (unsigned int)-1;
    _32x32_19_TextureIndex_ = (unsigned int)-1;
    _32x32_20_TextureIndex_ = (unsigned int)-1;
    _32x32_21_TextureIndex_ = (unsigned int)-1;
    _32x32_22_TextureIndex_ = (unsigned int)-1;
    _32x32_23_TextureIndex_ = (unsigned int)-1;
    _32x32_24_TextureIndex_ = (unsigned int)-1;
    _32x32_25_TextureIndex_ = (unsigned int)-1;
    _32x32_26_TextureIndex_ = (unsigned int)-1;
    _32x32_27_TextureIndex_ = (unsigned int)-1;
    _32x32_28_TextureIndex_ = (unsigned int)-1;
    _32x32_29_TextureIndex_ = (unsigned int)-1;
    _32x32_30_TextureIndex_ = (unsigned int)-1;
    _32x32_31_TextureIndex_ = (unsigned int)-1;
    _32x32_32_TextureIndex_ = (unsigned int)-1;
    _32x32_33_TextureIndex_ = (unsigned int)-1;
    _32x32_34_TextureIndex_ = (unsigned int)-1;

    shape01EntityId_ = (unsigned int)-1;
    shape02EntityId_ = (unsigned int)-1;
    shape03EntityId_ = (unsigned int)-1;
    shape04EntityId_ = (unsigned int)-1;
    shape05EntityId_ = (unsigned int)-1;
    shape06EntityId_ = (unsigned int)-1;
    shape07EntityId_ = (unsigned int)-1;
    shape08EntityId_ = (unsigned int)-1;
    shape09EntityId_ = (unsigned int)-1;
    shape10EntityId_ = (unsigned int)-1;
    shape11EntityId_ = (unsigned int)-1;
    shape12EntityId_ = (unsigned int)-1;
    shape13EntityId_ = (unsigned int)-1;
    shape14EntityId_ = (unsigned int)-1;
    shape15EntityId_ = (unsigned int)-1;
    shape16EntityId_ = (unsigned int)-1;
    shape17EntityId_ = (unsigned int)-1;
    shape18EntityId_ = (unsigned int)-1;
    shape19EntityId_ = (unsigned int)-1;
    shape20EntityId_ = (unsigned int)-1;
    shape21EntityId_ = (unsigned int)-1;
    shape22EntityId_ = (unsigned int)-1;
    shape23EntityId_ = (unsigned int)-1;
    shape24EntityId_ = (unsigned int)-1;
    shape25EntityId_ = (unsigned int)-1;
    shape26EntityId_ = (unsigned int)-1;
    shape27EntityId_ = (unsigned int)-1;
    shape28EntityId_ = (unsigned int)-1;
    shape29EntityId_ = (unsigned int)-1;
    shape30EntityId_ = (unsigned int)-1;
    shape31EntityId_ = (unsigned int)-1;
    shape32EntityId_ = (unsigned int)-1;
    shape33EntityId_ = (unsigned int)-1;
    shape34EntityId_ = (unsigned int)-1;
    shape35EntityId_ = (unsigned int)-1;
}

void TestScene003::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene004"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene004"));
            }
        }
    }
}
