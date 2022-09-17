#include "TestScene002.h"

#include "TestComponents.h"

#include "Engine/Components/RenderedModel.h"
#include "Engine/Application.h"
#include "Engine/BiMap.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/Renderer.h"
#include "Engine/TextureLoader.h"
#include "Engine/Window.h"

#include <algorithm>



// public: ---------------------------------------------------------------------

TestScene002::TestScene002()
{
    ClearResources();

    BiMapTest();
    ComponentContainerTest();
    ComponentStoresTest();
    MeshLoaderTest();
    TextureLoaderTest();
}

TestScene002::~TestScene002()
{}

const char* TestScene002::Name()
{
    return "TestScene002";
}

void TestScene002::Initialize()
{
    TestSceneBase001::Initialize();

    // Load meshes
    // -------------------------------------------------------------------------

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::LoadMeshOBJ(*newMeshDataPtr, "../Models/Cube.obj"));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.24f, 0.24f);
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(-0.24f, 0.24f);
        positions.emplace_back(-0.32f, 0.0f);
        positions.emplace_back(-0.24f, -0.24f);
        positions.emplace_back(0.0f, -0.32f);
        positions.emplace_back(0.24f, -0.24f);
        for (size_t i = 0; i < positions.size(); ++i)
        {
            positions[i] += glm::vec2(0.5f, 0.5f);
        }
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, positions));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, -0.32f);
        positions.emplace_back(-0.24f, -0.24f);
        positions.emplace_back(-0.32f, 0.32f);
        positions.emplace_back(-0.24f, 0.24f);
        positions.emplace_back(0.32f, 0.32f);
        positions.emplace_back(0.24f, 0.24f);
        positions.emplace_back(0.32f, -0.32f);
        positions.emplace_back(0.24f, -0.24f);
        positions.emplace_back(-0.16f, -0.32f);
        positions.emplace_back(-0.08f, -0.24f);
        positions.emplace_back(-0.16f, 0.16f);
        positions.emplace_back(-0.08f, 0.08f);
        positions.emplace_back(0.16f, 0.16f);
        positions.emplace_back(0.08f, 0.08f);
        positions.emplace_back(0.16f, -0.16f);
        positions.emplace_back(0.08f, -0.08f);
        for (size_t i = 0; i < positions.size(); ++i)
        {
            positions[i] += glm::vec2(0.5f, 0.5f);
        }
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleStrip(*newMeshDataPtr, positions));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.08f);
        positions.emplace_back(0.24f, 0.32f);
        positions.emplace_back(-0.24f, 0.32f);
        positions.emplace_back(0.08f, 0.0f);
        positions.emplace_back(0.32f, -0.24f);
        positions.emplace_back(0.32f, 0.24f);
        positions.emplace_back(0.0f, -0.08f);
        positions.emplace_back(-0.24f, -0.32f);
        positions.emplace_back(0.24f, -0.32f);
        positions.emplace_back(-0.08f, 0.0f);
        positions.emplace_back(-0.32f, 0.24f);
        positions.emplace_back(-0.32f, -0.24f);
        for (size_t i = 0; i < positions.size(); ++i)
        {
            positions[i] += glm::vec2(0.5f, 0.5f);
        }
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangles(*newMeshDataPtr, positions));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.24f, -0.16f);
        positions.emplace_back(-0.24f, 0.0f);
        positions.emplace_back(-0.24f, 0.16f);
        positions.emplace_back(-0.16f, 0.24f);
        positions.emplace_back(0.0f, 0.24f);
        positions.emplace_back(0.16f, 0.24f);
        positions.emplace_back(0.24f, 0.16f);
        positions.emplace_back(0.24f, 0.0f);
        positions.emplace_back(0.24f, -0.16f);
        positions.emplace_back(0.16f, -0.24f);
        positions.emplace_back(0.0f, -0.24f);
        positions.emplace_back(-0.06f, -0.24f);
        positions.emplace_back(-0.12f, -0.16f);
        positions.emplace_back(-0.12f, 0.0f);
        positions.emplace_back(-0.12f, 0.08f);
        positions.emplace_back(0.0f, 0.08f);
        positions.emplace_back(0.12f, 0.08f);
        positions.emplace_back(0.12f, 0.0f);
        positions.emplace_back(0.12f, -0.08f);
        positions.emplace_back(0.0f, -0.08f);
        positions.emplace_back(0.0f, 0.0f);
        for (size_t i = 0; i < positions.size(); ++i)
        {
            positions[i] += glm::vec2(0.5f, 0.5f);
        }
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.08f, 0.0f);
        positions.emplace_back(0.24f, 0.16f);
        positions.emplace_back(0.08f, 0.08f);
        positions.emplace_back(0.08f, 0.24f);
        positions.emplace_back(0.0f, 0.08f);
        positions.emplace_back(-0.08f, 0.24f);
        positions.emplace_back(-0.08f, 0.08f);
        positions.emplace_back(-0.24f, 0.16f);
        positions.emplace_back(-0.08f, 0.0f);
        positions.emplace_back(-0.24f, -0.16f);
        positions.emplace_back(-0.08f, -0.16f);
        positions.emplace_back(-0.08f, -0.24f);
        positions.emplace_back(0.0f, -0.16f);
        positions.emplace_back(0.08f, -0.24f);
        positions.emplace_back(0.08f, -0.16f);
        positions.emplace_back(0.24f, -0.16f);
        positions.emplace_back(0.16f, -0.08f);
        for (size_t i = 0; i < positions.size(); ++i)
        {
            positions[i] += glm::vec2(0.5f, 0.5f);
        }
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, positions, 0.04f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.24f, 0.24f);
        positions.emplace_back(0.24f, -0.24f);
        positions.emplace_back(-0.24f, -0.24f);
        positions.emplace_back(0.24f, 0.24f);
        for (size_t i = 0; i < positions.size(); ++i)
        {
            positions[i] += glm::vec2(0.5f, 0.5f);
        }
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, positions, 0.12f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.24f, 0.24f);
        positions.emplace_back(0.24f, 0.24f);
        positions.emplace_back(-0.24f, 0.16f);
        positions.emplace_back(0.24f, 0.16f);
        positions.emplace_back(-0.24f, 0.08f);
        positions.emplace_back(0.24f, 0.08f);
        positions.emplace_back(-0.24f, 0.0f);
        positions.emplace_back(0.24f, 0.0f);
        positions.emplace_back(-0.24f, -0.08f);
        positions.emplace_back(0.24f, -0.08f);
        positions.emplace_back(-0.24f, -0.16f);
        positions.emplace_back(0.24f, -0.16f);
        positions.emplace_back(-0.24f, -0.24f);
        positions.emplace_back(0.24f, -0.24f);
        for (size_t i = 0; i < positions.size(); ++i)
        {
            positions[i] += glm::vec2(0.5f, 0.5f);
        }
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, positions, 0.04f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 3));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 4));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 5));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 6));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 7));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 8));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 24));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
        Project001::MeshLoader::ScaleMesh(*newMeshDataPtr, glm::vec3(2.0f, 1.5f, 1.0f));
        Project001::MeshLoader::RotateMesh(*newMeshDataPtr, glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() / -4.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
        Project001::MeshLoader::RotateMesh(*newMeshDataPtr, glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() / -4.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
        Project001::MeshLoader::RotateMesh(*newMeshDataPtr, glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() / -4.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
        Project001::MeshLoader::TranslateMesh(*newMeshDataPtr, glm::vec3(0.16f, 0.0f, 0.0f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*newMeshDataPtr, 0.08f, 0.32f, 1, 0.0f, 0.5f * glm::pi<float>()));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*newMeshDataPtr, 0.08f, 0.32f, 2, 0.0f, 1.5f * glm::pi<float>()));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*newMeshDataPtr, 0.08f, 0.32f, 3, 0.0f, 2.0f * glm::pi<float>()));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*newMeshDataPtr, 0.08f, 0.32f, 4, 0.0f, 2.0f * glm::pi<float>()));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*newMeshDataPtr, 0.08f, 0.32f, 4, 1.5f * glm::pi<float>(), 1.0f * glm::pi<float>()));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*newMeshDataPtr, 0.08f, 0.32f, 24, 1.25f * glm::pi<float>(), 0.75f * glm::pi<float>()));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*newMeshDataPtr, 0.16f, 0.32f, 32, 0.0f, 2.0f * glm::pi<float>()));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(*newMeshDataPtr, 0.32f, 0.32f, 2));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(*newMeshDataPtr, 0.32f, 0.32f, 4));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(*newMeshDataPtr, 0.32f, 0.32f, 8));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DBezeledRectangle(*newMeshDataPtr, 0.64f, 0.64f, 0.16f, 1));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DBezeledRectangle(*newMeshDataPtr, 0.64f, 0.64f, 0.16f, 2));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DBezeledRectangle(*newMeshDataPtr, 0.64f, 0.64f, 0.16f, 3));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DBezeledRectangle(*newMeshDataPtr, 0.64f, 0.48f, 0.16f, 4));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    // Load textures
    // -------------------------------------------------------------------------

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/CounterclockwiseDie.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(dice01TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/HallowDie.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(dice02TextureId_, 2, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/Thonk.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(thonkTextureId_, 3, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/100x100_2.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_100x100TextureId_, 4, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/Specular1.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(patternSpecular01TextureId_, 6, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/Specular2.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(patternSpecular02TextureId_, 7, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/ThonkSpecular.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(thonkSpecularTextureId_, 8, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
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

    // cube entity 01
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[0]);
        renderedModelPtr->SetTextureId(thonkTextureId_);
        renderedModelPtr->SetSpecularId(thonkSpecularTextureId_);
        renderedModelPtr->SetShininess(32.0f);
    }

    // cube entity 02
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[0]);
        renderedModelPtr->SetTextureId(dice01TextureId_);
        renderedModelPtr->SetShininess(32.0f);
    }

    // cube entity 03
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[0]);
        renderedModelPtr->SetTextureId(dice02TextureId_);
        renderedModelPtr->SetColorRGB(0.8f, 0.2f, 0.2f);
        renderedModelPtr->SetShininess(32.0f);
    }

    // cube entity 04
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[0]);
        renderedModelPtr->SetColor(1.0f, 1.0f, 1.0f, 0.25f);
        renderedModelPtr->SetTranslucent(true);
    }

    // cube entity 05
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[0]);
        renderedModelPtr->SetColor(1.0f, 1.0f, 1.0f, 0.25f);
        renderedModelPtr->SetTranslucent(true);
    }

    // cube entity 06
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[0]);
        renderedModelPtr->SetSpecularId(patternSpecular02TextureId_);
        renderedModelPtr->SetScale(0.5f, 0.75f, 1.0f);
        renderedModelPtr->SetColorRGB(0.2f, 0.8f, 0.2f);
        renderedModelPtr->SetShininess(24.0f);
    }

    // cube entity 07
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[0]);
        renderedModelPtr->SetSpecularId(patternSpecular02TextureId_);
        renderedModelPtr->AddPitch(glm::pi<float>() / 4.0f);
        renderedModelPtr->AddYaw(glm::pi<float>() / 4.0f);
        renderedModelPtr->SetColorRGB(0.2f, 0.2f, 0.8f);
        renderedModelPtr->SetShininess(4.0f);
    }

    // generated shape entity 01
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[1]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(0.8f, 0.6f, 0.2f);
        renderedModelPtr->SetTranslucent(true);
    }
 
    // generated shape entity 02
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[2]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(0.8f, 0.2f, 0.6f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 03
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[3]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(0.6f, 0.2f, 0.8f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 04
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[4]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(0.2f, 0.6f, 0.8f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 05
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[5]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(0.2f, 0.8f, 0.6f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 06
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[6]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(0.6f, 0.8f, 0.2f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 07
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[7]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(1.0f, 1.0f, 1.0f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 08
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[8]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(1.0f, 0.0f, 0.0f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 09
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[9]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(0.0f, 1.0f, 0.0f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 10
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[10]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(0.0f, 0.0f, 1.0f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 11
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[11]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(0.0f, 1.0f, 1.0f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 12
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[12]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(1.0f, 0.0f, 1.0f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 13
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[13]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(1.0f, 1.0f, 0.0f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 14
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[14]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 15
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[15]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 16
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[16]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 17
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[17]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 18
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[18]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 19
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[19]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 20
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[20]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 21
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[21]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 22
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[22]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 23
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[23]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 24
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[24]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 25
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[25]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 26
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[26]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 27
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[27]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 28
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[28]);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
    }
}

void TestScene002::Deinitialize()
{
    TestSceneBase001::Deinitialize();

    ClearResources();
}

void TestScene002::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene002::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneBase001::OnEvent(event);
}

// protected: ------------------------------------------------------------------

void TestScene002::ClearResources()
{
    dice01TextureId_ = (unsigned int)-1;
    dice02TextureId_ = (unsigned int)-1;
    thonkTextureId_ = (unsigned int)-1;
    _100x100TextureId_ = (unsigned int)-1;
    patternSpecular01TextureId_ = (unsigned int)-1;
    patternSpecular02TextureId_ = (unsigned int)-1;
    thonkSpecularTextureId_ = (unsigned int)-1;
}

void TestScene002::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene003"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene003"));
            }
        }
    }
}

// private: --------------------------------------------------------------------

void TestScene002::BiMapTest() const
{
    Project001::BiMap<unsigned int, std::string> testBiMap;

    testBiMap.Add(0, "ZERO");
    testBiMap.Add(1, "ONE");
    testBiMap.Add(2, "TWO");
    testBiMap.Add(3, "THREE");

    size_t size = testBiMap.Size();

    unsigned int x00;
    std::string y00;
    if (testBiMap.Find_X(0))
    {
        y00 = testBiMap.Get_Using_X(0);
    }
    if (testBiMap.Find_Y("ZERO"))
    {
        x00 = testBiMap.Get_Using_Y("ZERO");
    }

    unsigned int x01;
    std::string y01;
    if (testBiMap.Find_X(1))
    {
        y01 = testBiMap.Get_Using_X(1);
    }
    if (testBiMap.Find_Y("ONE"))
    {
        x01 = testBiMap.Get_Using_Y("ONE");
    }

    unsigned int x02;
    std::string y02;
    if (testBiMap.Find_X(2))
    {
        y02 = testBiMap.Get_Using_X(2);
    }
    if (testBiMap.Find_Y("TWO"))
    {
        x02 = testBiMap.Get_Using_Y("TWO");
    }

    testBiMap.Remove_Using_X(0);
    testBiMap.Remove_Using_Y("ONE");

    testBiMap.Add(2, "TWENTY");
    testBiMap.Add(30, "THREE");

    testBiMap.Clear();
}

void TestScene002::ComponentContainerTest() const
{
    bool testBool = false;

    Project001::ComponentContainer testComponentContainer;

    testBool = testComponentContainer.CreateComponent<TestComponent00>(0, 10, 10, 10);
    testBool = testComponentContainer.CreateComponent<TestComponent00>(0, 10, 10, 10);
    testBool = testComponentContainer.CreateComponent<TestComponent00>(1, 20, 20, 20);
    testBool = testComponentContainer.CreateComponent<TestComponent00>(2, 30, 30, 30);

    TestComponent00* testComponent0 = nullptr;
    TestComponent00* testComponent1 = nullptr;
    TestComponent00* testComponent2 = nullptr;

    testBool = testComponentContainer.GetComponent<TestComponent00>(0, testComponent0);
    testBool = testComponentContainer.GetComponent<TestComponent00>(1, testComponent1);
    testBool = testComponentContainer.GetComponent<TestComponent00>(2, testComponent2);

    TestComponent00* allTestComponents = nullptr;
    size_t count = 0;

    testBool = testComponentContainer.GetAllComponents<TestComponent00>(allTestComponents, count);

    testBool = testComponentContainer.DeleteComponent(0);
    testBool = testComponentContainer.DeleteComponent(0);
    testBool = testComponentContainer.GetComponent<TestComponent00>(0, testComponent0);

    testBool = testComponentContainer.GetAllComponents<TestComponent00>(allTestComponents, count);

    testComponentContainer.DeleteAllComponents();

    testBool = testComponentContainer.GetAllComponents<TestComponent00>(allTestComponents, count);

    testBool = testComponentContainer.CreateComponent<TestComponent00>(0, 11, 11, 11);
    testBool = testComponentContainer.CreateComponent<TestComponent00>(0, 11, 11, 11);
    testBool = testComponentContainer.CreateComponent<TestComponent00>(1, 22, 22, 22);
    testBool = testComponentContainer.CreateComponent<TestComponent00>(2, 33, 33, 33);
    testBool = testComponentContainer.CreateComponent<TestComponent00>(3, 44, 44, 44);

    testBool = testComponentContainer.GetComponent<TestComponent00>(0, testComponent0);
    testBool = testComponentContainer.GetComponent<TestComponent00>(1, testComponent1);

    unsigned int returnedEntityId0 = (unsigned int)-1;
    unsigned int returnedEntityId1 = (unsigned int)-1;
    unsigned int returnedEntityId2 = (unsigned int)-1;
    testBool = testComponentContainer.GetComponentEntityId<TestComponent00>(testComponent0, returnedEntityId0);
    testBool = testComponentContainer.GetComponentEntityId<TestComponent00>(testComponent1, returnedEntityId1);
    testBool = testComponentContainer.GetComponentEntityId<TestComponent00>(testComponent2, returnedEntityId2);

    testComponentContainer.DeleteAllComponents();

    testBool = testComponentContainer.GetAllComponents<TestComponent00>(allTestComponents, count);

    testBool = testComponentContainer.CreateComponent<TestComponent00>(0, 0, 0, 0);
    testBool = testComponentContainer.CreateComponent<TestComponent00>(1, 1, 1, 1);
    testBool = testComponentContainer.CreateComponent<TestComponent00>(2, 2, 2, 2);
    testBool = testComponentContainer.CreateComponent<TestComponent00>(3, 3, 3, 3);
    testBool = testComponentContainer.CreateComponent<TestComponent00>(4, 4, 4, 4);
    testBool = testComponentContainer.CreateComponent<TestComponent00>(5, 5, 5, 5);

    testBool = testComponentContainer.GetAllComponents<TestComponent00>(allTestComponents, count);

    std::vector<TestComponent00*> testComponentPtrs;
    for (size_t i = 0; i < count; ++i)
    {
        testComponentPtrs.emplace_back(&allTestComponents[i]);
    }
    std::sort(testComponentPtrs.begin(), testComponentPtrs.end(),
        [](TestComponent00* a, TestComponent00* b)->bool
        {
            return a->GetSum() > b->GetSum();
        });
}

void TestScene002::ComponentStoresTest() const
{
    bool testBool = false;

    Project001::ComponentStores testComponentStores;

    testBool = testComponentStores.CreateComponent<TestComponent00>(0, 0, 0, 0);

    unsigned int entity0;
    testBool = testComponentStores.CreateEntity(entity0);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity0, 10, 10, 10);

    unsigned int entity1;
    testBool = testComponentStores.CreateEntity(entity1);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity1, 11, 11, 11);

    unsigned int entity2;
    testBool = testComponentStores.CreateEntity(entity2);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity2, 12, 12, 12);

    testBool = testComponentStores.DeleteEntity(9);
    testBool = testComponentStores.DeleteEntity(entity0);
    testBool = testComponentStores.DeleteEntity(entity0);

    unsigned int entity3;
    testBool = testComponentStores.CreateEntity(entity3);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity3, 13, 13, 13);

    TestComponent00* testComponent0 = nullptr;
    testBool = testComponentStores.GetComponent<TestComponent00>(entity1, testComponent0);
    int sum = testComponent0->GetSum();

    testBool = testComponentStores.DeleteAllComponents<TestComponent00>();
    testBool = testComponentStores.GetComponent<TestComponent00>(entity0, testComponent0);

    testBool = testComponentStores.CreateComponent<TestComponent00>(entity0, 20, 20, 20);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity1, 21, 21, 21);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity2, 22, 22, 22);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity3, 23, 23, 23);

    testBool = testComponentStores.GetComponent<TestComponent00>(entity0, testComponent0);

    testComponentStores.DeleteAllEntities();

    testBool = testComponentStores.GetComponent<TestComponent00>(entity0, testComponent0);

    testBool = testComponentStores.CreateEntity(entity0);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity0, 20, 20, 20);
    testBool = testComponentStores.CreateEntity(entity1);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity1, 21, 21, 21);
    testBool = testComponentStores.CreateEntity(entity2);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity2, 22, 22, 22);
    testBool = testComponentStores.CreateEntity(entity3);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity3, 23, 23, 23);

    testBool = testComponentStores.GetComponent<TestComponent00>(entity0, testComponent0);

    TestComponent00* testComponent1 = nullptr;
    testBool = testComponentStores.GetComponent<TestComponent00>(entity1, testComponent1);

    TestComponent00* testComponent2 = nullptr;

    unsigned int testEntityId0 = (unsigned int)-1;
    testBool = testComponentStores.GetComponentEntityId<TestComponent00>(testComponent0, testEntityId0);
    unsigned int testEntityId1 = (unsigned int)-1;
    testBool = testComponentStores.GetComponentEntityId<TestComponent00>(testComponent1, testEntityId1);
    unsigned int testEntityId2 = (unsigned int)-1;
    testBool = testComponentStores.GetComponentEntityId<TestComponent00>(testComponent2, testEntityId2);

    TestComponent00* allTestComponents = nullptr;
    size_t count = 0;

    testBool = testComponentStores.GetAllComponents<TestComponent00>(allTestComponents, count);

    testBool = testComponentStores.GetComponentEntityId<TestComponent00>(&allTestComponents[2], testEntityId2);
}

void TestScene002::MeshLoaderTest() const
{
    bool testBool = false;

    Project001::MeshData meshData01;
    testBool = Project001::MeshLoader::LoadMeshOBJ(meshData01, "");
    testBool = Project001::MeshLoader::LoadMeshOBJ(meshData01, "../Models/CubeQ.obj");
}

void TestScene002::TextureLoaderTest() const
{
    bool testBool = false;

    Project001::TextureData textureData01;
    testBool = Project001::TextureLoader::LoadTexture(textureData01, "");
    testBool = Project001::TextureLoader::LoadTexture(textureData01, "../Textures/rgb.png");

    Project001::TextureData textureData02;
    testBool = Project001::TextureLoader::LoadTexture(textureData02, "../Textures/Cube.png");

    Project001::TextureData textureData03;
    testBool = Project001::TextureLoader::LoadTexture(textureData03, "../Textures/CubeSpecular.png");
}