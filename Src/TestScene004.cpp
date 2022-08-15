#include "TestScene004.h"

#include "Engine/Components/RenderedModel.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/Renderer.h"
#include "Engine/TextureLoader.h"
#include "Engine/Window.h"



// public: ---------------------------------------------------------------------

TestScene004::TestScene004()
{
    ClearIndiciesAndEntityIds();
}

TestScene004::~TestScene004()
{}

const char* TestScene004::Name()
{
    return "TestScene004";
}

void TestScene004::Initialize()
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
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(shape01Mesh_, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape01Mesh_);
        Project001::MeshLoader::RecenterMesh(shape01Mesh_);
    }

    {
        // duplicate position at end
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.16f, 0.0f);
        positions.emplace_back(0.16f, 0.16f);
        positions.emplace_back(0.16f, 0.32f);
        positions.emplace_back(0.16f, 0.64f);
        positions.emplace_back(0.16f, 0.64f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(shape02Mesh_, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape02Mesh_);
        Project001::MeshLoader::RecenterMesh(shape02Mesh_);
    }

    {
        // duplicate position in middle
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.32f, 0.16f);
        positions.emplace_back(0.32f, 0.32f);
        positions.emplace_back(0.32f, 0.32f);
        positions.emplace_back(0.32f, 0.64f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(shape03Mesh_, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape03Mesh_);
        Project001::MeshLoader::RecenterMesh(shape03Mesh_);
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.32f, -0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.32f, 0.32f); // same slope
        positions.emplace_back(-0.32f, -0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(shape04Mesh_, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape04Mesh_);
        Project001::MeshLoader::RecenterMesh(shape04Mesh_);
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, 0.32f);
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.32f, -0.32f); // same slope
        positions.emplace_back(-0.32f, -0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(shape05Mesh_, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape05Mesh_);
        Project001::MeshLoader::RecenterMesh(shape05Mesh_);
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, -0.32f);
        positions.emplace_back(0.0f, -0.32f);
        positions.emplace_back(0.32f, -0.32f); // same slope
        positions.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(shape06Mesh_, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape06Mesh_);
        Project001::MeshLoader::RecenterMesh(shape06Mesh_);
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, 0.0f);
        positions.emplace_back(0.0f, -0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(shape07Mesh_, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape07Mesh_);
        Project001::MeshLoader::RecenterMesh(shape07Mesh_);
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.32f, -0.32f);
        positions.emplace_back(0.32f, 0.32f);
        positions.emplace_back(-0.32f, -0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(shape08Mesh_, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape08Mesh_);
        Project001::MeshLoader::RecenterMesh(shape08Mesh_);
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, 0.32f);
        positions.emplace_back(0.32f, -0.32f);
        positions.emplace_back(-0.32f, -0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(shape09Mesh_, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape09Mesh_);
        Project001::MeshLoader::RecenterMesh(shape09Mesh_);
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, -0.32f);
        positions.emplace_back(0.32f, -0.32f);
        positions.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(shape10Mesh_, positions, 0.24f));
        (Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape10Mesh_));
        (Project001::MeshLoader::RecenterMesh(shape10Mesh_));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, 0.0f);
        positions.emplace_back(0.0f, -0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(shape11Mesh_, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape11Mesh_);
        Project001::MeshLoader::RecenterMesh(shape11Mesh_);
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
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(shape12Mesh_, positions, 0.04f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape12Mesh_);
        Project001::MeshLoader::RecenterMesh(shape12Mesh_);
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32, -0.04f);
        positions.emplace_back(0.08, -0.04f);
        positions.emplace_back(-0.08, 0.04f);
        positions.emplace_back(0.32, 0.04f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(shape13Mesh_, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape13Mesh_);
        Project001::MeshLoader::RecenterMesh(shape13Mesh_);
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32, -0.04f);
        positions.emplace_back(0.08, -0.04f);
        positions.emplace_back(-0.08, 0.04f);
        positions.emplace_back(0.32, 0.04f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(shape14Mesh_, positions, 0.12f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape14Mesh_);
        Project001::MeshLoader::RecenterMesh(shape14Mesh_);
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.16f, 0.16f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(shape15Mesh_, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape15Mesh_);
        Project001::MeshLoader::RecenterMesh(shape15Mesh_);
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(shape16Mesh_, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape16Mesh_);
        Project001::MeshLoader::RecenterMesh(shape16Mesh_);
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(-0.16f, 0.48f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(shape17Mesh_, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape17Mesh_);
        Project001::MeshLoader::RecenterMesh(shape17Mesh_);
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(-0.16f, 0.48f);
        positions.emplace_back(0.16f, 0.16f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(shape18Mesh_, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape18Mesh_);
        Project001::MeshLoader::RecenterMesh(shape18Mesh_);
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.16f, 0.16f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(shape19Mesh_, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape19Mesh_);
        Project001::MeshLoader::RecenterMesh(shape19Mesh_);
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(0.32f, 0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.16f, 0.16f);
        positions.emplace_back(0.48f, -0.16f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(shape20Mesh_, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape20Mesh_);
        Project001::MeshLoader::RecenterMesh(shape20Mesh_);
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.16f, 0.16f);
        positions.emplace_back(0.48f, -0.16f);
        positions.emplace_back(-0.16f, 0.48f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(shape21Mesh_, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape21Mesh_);
        Project001::MeshLoader::RecenterMesh(shape21Mesh_);
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(0.32f, 0.32f);
        positions.emplace_back(0.32f, 0.0f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(shape22Mesh_, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape22Mesh_);
        Project001::MeshLoader::RecenterMesh(shape22Mesh_);
    }

    _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(shape23Mesh_, 0.48f, 0.64f, 0.0f, 13.0f / 32.0f, 1.0f - 16.0f / 32.0f, 1.0f));

    Project001::MeshLoader::CopyMesh(shape24Mesh_, shape23Mesh_);
    Project001::MeshLoader::RotateTextureCoordinates(shape24Mesh_, -glm::quarter_pi<float>());

    Project001::MeshLoader::CopyMesh(shape25Mesh_, shape23Mesh_);
    Project001::MeshLoader::ScaleTextureCoordinates(shape25Mesh_, glm::vec2(0.5f, 0.5f));

    Project001::MeshLoader::CopyMesh(shape26Mesh_, shape23Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape26Mesh_, glm::vec2(0.06f / 0.32f, -0.08f / 0.32f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(shape27Mesh_, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));
    _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(shape28Mesh_, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));
    _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(shape29Mesh_, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));
    _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(shape30Mesh_, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));
    _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(shape31Mesh_, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));
    _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(shape32Mesh_, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));
    _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(shape33Mesh_, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));
    _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(shape34Mesh_, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));
    _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(shape35Mesh_, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));

    // Load textures
    // -------------------------------------------------------------------------

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_00.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_00_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_01.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_01_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_02.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_02_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_03.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_03_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_04.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_04_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_05.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_05_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_06.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_06_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_07.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_07_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_08.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_08_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_09.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_09_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_10.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_10_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_11.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_11_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_12.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_12_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_13.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_13_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_14.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_14_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_15.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_15_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_16.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_16_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_17.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_17_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_18.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_18_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_19.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_19_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_20.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_20_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_21.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_21_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_22.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_22_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_23.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_23_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_24.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_24_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_25.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_25_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_26.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_26_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_27.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_27_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_28.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_28_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_29.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_29_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_30.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_30_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_31.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_31_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_32.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_32_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_33.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_33_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_34.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_34_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
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
        renderedModelPtr->SetMeshDataPtr(&shape01Mesh_);
        renderedModelPtr->SetTextureId(_32x32_00_TextureId_);
    }

    // generated shape entity 02
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape02EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape02EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape02EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape02Mesh_);
        renderedModelPtr->SetTextureId(_32x32_01_TextureId_);
    }

    // generated shape entity 03
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape03EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape03EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape03EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape03Mesh_);
        renderedModelPtr->SetTextureId(_32x32_02_TextureId_);
    }

    // generated shape entity 04
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape04EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape04EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape04EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape04Mesh_);
        renderedModelPtr->SetTextureId(_32x32_03_TextureId_);
    }

    // generated shape entity 05
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape05EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape05EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape05EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape05Mesh_);
        renderedModelPtr->SetTextureId(_32x32_04_TextureId_);
    }

    // generated shape entity 06
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape06EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape06EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape06EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape06Mesh_);
        renderedModelPtr->SetTextureId(_32x32_05_TextureId_);
    }

    // generated shape entity 07
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape07EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape07EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape07EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape07Mesh_);
        renderedModelPtr->SetTextureId(_32x32_06_TextureId_);
    }

    // generated shape entity 08
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape08EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape08EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape08EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape08Mesh_);
        renderedModelPtr->SetTextureId(_32x32_07_TextureId_);
    }

    // generated shape entity 09
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape09EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape09EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape09EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape09Mesh_);
        renderedModelPtr->SetTextureId(_32x32_08_TextureId_);
    }

    // generated shape entity 10
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape10EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape10EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape10EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape10Mesh_);
        renderedModelPtr->SetTextureId(_32x32_09_TextureId_);
    }

    // generated shape entity 11
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape11EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape11EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape11EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape11Mesh_);
        renderedModelPtr->SetTextureId(_32x32_10_TextureId_);
    }

    // generated shape entity 12
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape12EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape12EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape12EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape12Mesh_);
        renderedModelPtr->SetTextureId(_32x32_11_TextureId_);
    }

    // generated shape entity 13
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape13EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape13EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape13EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape13Mesh_);
        renderedModelPtr->SetTextureId(_32x32_12_TextureId_);
    }

    // generated shape entity 14
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape14EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape14EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape14EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape14Mesh_);
        renderedModelPtr->SetTextureId(_32x32_13_TextureId_);
    }

    // generated shape entity 15
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape15EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape15EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape15EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape15Mesh_);
        renderedModelPtr->SetTextureId(_32x32_14_TextureId_);
    }

    // generated shape entity 16
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape16EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape16EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape16EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape16Mesh_);
        renderedModelPtr->SetTextureId(_32x32_15_TextureId_);
    }

    // generated shape entity 17
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape17EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape17EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape17EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape17Mesh_);
        renderedModelPtr->SetTextureId(_32x32_16_TextureId_);
    }

    // generated shape entity 18
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape18EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape18EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape18EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape18Mesh_);
        renderedModelPtr->SetTextureId(_32x32_17_TextureId_);
    }

    // generated shape entity 19
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape19EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape19EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape19EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape19Mesh_);
        renderedModelPtr->SetTextureId(_32x32_18_TextureId_);
    }

    // generated shape entity 20
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape20EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape20EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape20EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape20Mesh_);
        renderedModelPtr->SetTextureId(_32x32_19_TextureId_);
    }

    // generated shape entity 21
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape21EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape21EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape21EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape21Mesh_);
        renderedModelPtr->SetTextureId(_32x32_20_TextureId_);
    }

    // generated shape entity 22
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape22EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape22EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape22EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape22Mesh_);
        renderedModelPtr->SetTextureId(_32x32_21_TextureId_);
    }

    // generated shape entity 23
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape23EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape23EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape23EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape23Mesh_);
        renderedModelPtr->SetTextureId(_32x32_22_TextureId_);
    }

    // generated shape entity 24
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape24EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape24EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape24EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape24Mesh_);
        renderedModelPtr->SetTextureId(_32x32_23_TextureId_);
    }

    // generated shape entity 25
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape25EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape25EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape25EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape25Mesh_);
        renderedModelPtr->SetTextureId(_32x32_24_TextureId_);
    }

    // generated shape entity 26
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape26EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape26EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape26EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape26Mesh_);
        renderedModelPtr->SetTextureId(_32x32_25_TextureId_);
    }

    // generated shape entity 27
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape27EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape27EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape27EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape27Mesh_);
        renderedModelPtr->SetTextureId(_32x32_26_TextureId_);
    }

    // generated shape entity 28
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape28EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape28EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape28EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape28Mesh_);
        renderedModelPtr->SetTextureId(_32x32_27_TextureId_);
    }

    // generated shape entity 29
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape29EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape29EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape29EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape29Mesh_);
        renderedModelPtr->SetTextureId(_32x32_28_TextureId_);
    }

    // generated shape entity 30
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape30EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape30EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape30EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape30Mesh_);
        renderedModelPtr->SetTextureId(_32x32_29_TextureId_);
    }

    // generated shape entity 31
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape31EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape31EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape31EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape31Mesh_);
        renderedModelPtr->SetTextureId(_32x32_30_TextureId_);
    }

    // generated shape entity 32
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape32EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape32EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape32EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape32Mesh_);
        renderedModelPtr->SetTextureId(_32x32_31_TextureId_);
    }

    // generated shape entity 33
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape33EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape33EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape33EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape33Mesh_);
        renderedModelPtr->SetTextureId(_32x32_32_TextureId_);
    }

    // generated shape entity 34
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape34EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape34EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape34EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape34Mesh_);
        renderedModelPtr->SetTextureId(_32x32_33_TextureId_);
    }

    // generated shape entity 35
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape35EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape35EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape35EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape35Mesh_);
        renderedModelPtr->SetTextureId(_32x32_34_TextureId_);
    }
}

void TestScene004::Deinitialize()
{
    TestSceneBase001::Deinitialize();

    ClearIndiciesAndEntityIds();
}

void TestScene004::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene004::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneBase001::OnEvent(event);
}

// protected: ------------------------------------------------------------------

void TestScene004::ClearIndiciesAndEntityIds()
{
    shape01Mesh_.Clear();
    shape02Mesh_.Clear();
    shape03Mesh_.Clear();
    shape04Mesh_.Clear();
    shape05Mesh_.Clear();
    shape06Mesh_.Clear();
    shape07Mesh_.Clear();
    shape08Mesh_.Clear();
    shape09Mesh_.Clear();
    shape10Mesh_.Clear();
    shape11Mesh_.Clear();
    shape12Mesh_.Clear();
    shape13Mesh_.Clear();
    shape14Mesh_.Clear();
    shape15Mesh_.Clear();
    shape16Mesh_.Clear();
    shape17Mesh_.Clear();
    shape18Mesh_.Clear();
    shape19Mesh_.Clear();
    shape20Mesh_.Clear();
    shape21Mesh_.Clear();
    shape22Mesh_.Clear();
    shape23Mesh_.Clear();
    shape24Mesh_.Clear();
    shape25Mesh_.Clear();
    shape26Mesh_.Clear();
    shape27Mesh_.Clear();
    shape28Mesh_.Clear();
    shape29Mesh_.Clear();
    shape30Mesh_.Clear();
    shape31Mesh_.Clear();
    shape32Mesh_.Clear();
    shape33Mesh_.Clear();
    shape34Mesh_.Clear();
    shape35Mesh_.Clear();

    _32x32_00_TextureId_ = (unsigned int)-1;
    _32x32_01_TextureId_ = (unsigned int)-1;
    _32x32_02_TextureId_ = (unsigned int)-1;
    _32x32_03_TextureId_ = (unsigned int)-1;
    _32x32_04_TextureId_ = (unsigned int)-1;
    _32x32_05_TextureId_ = (unsigned int)-1;
    _32x32_06_TextureId_ = (unsigned int)-1;
    _32x32_07_TextureId_ = (unsigned int)-1;
    _32x32_08_TextureId_ = (unsigned int)-1;
    _32x32_09_TextureId_ = (unsigned int)-1;
    _32x32_10_TextureId_ = (unsigned int)-1;
    _32x32_11_TextureId_ = (unsigned int)-1;
    _32x32_12_TextureId_ = (unsigned int)-1;
    _32x32_13_TextureId_ = (unsigned int)-1;
    _32x32_14_TextureId_ = (unsigned int)-1;
    _32x32_15_TextureId_ = (unsigned int)-1;
    _32x32_16_TextureId_ = (unsigned int)-1;
    _32x32_17_TextureId_ = (unsigned int)-1;
    _32x32_18_TextureId_ = (unsigned int)-1;
    _32x32_19_TextureId_ = (unsigned int)-1;
    _32x32_20_TextureId_ = (unsigned int)-1;
    _32x32_21_TextureId_ = (unsigned int)-1;
    _32x32_22_TextureId_ = (unsigned int)-1;
    _32x32_23_TextureId_ = (unsigned int)-1;
    _32x32_24_TextureId_ = (unsigned int)-1;
    _32x32_25_TextureId_ = (unsigned int)-1;
    _32x32_26_TextureId_ = (unsigned int)-1;
    _32x32_27_TextureId_ = (unsigned int)-1;
    _32x32_28_TextureId_ = (unsigned int)-1;
    _32x32_29_TextureId_ = (unsigned int)-1;
    _32x32_30_TextureId_ = (unsigned int)-1;
    _32x32_31_TextureId_ = (unsigned int)-1;
    _32x32_32_TextureId_ = (unsigned int)-1;
    _32x32_33_TextureId_ = (unsigned int)-1;
    _32x32_34_TextureId_ = (unsigned int)-1;

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

void TestScene004::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene005"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene005"));
            }
        }
    }
}
