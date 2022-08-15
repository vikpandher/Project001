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
    ClearIndiciesAndEntityIds();

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

    _FAIL_CHECK(Project001::MeshLoader::LoadMeshOBJ(cubeMesh_, "../Models/Cube.obj"));

    {
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
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(shape01Mesh_, positions));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape01Mesh_);
        Project001::MeshLoader::RecenterMesh(shape01Mesh_);
    }

    {
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
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleStrip(shape02Mesh_, positions));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape02Mesh_);
        Project001::MeshLoader::RecenterMesh(shape02Mesh_);
    }

    {
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
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangles(shape03Mesh_, positions));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape03Mesh_);
        Project001::MeshLoader::RecenterMesh(shape03Mesh_);
    }

    {
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
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(shape04Mesh_, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape04Mesh_);
        Project001::MeshLoader::RecenterMesh(shape04Mesh_);
    }

    {
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
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(shape05Mesh_, positions, 0.04f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape05Mesh_);
        Project001::MeshLoader::RecenterMesh(shape05Mesh_);
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.24f, 0.24f);
        positions.emplace_back(0.24f, -0.24f);
        positions.emplace_back(-0.24f, -0.24f);
        positions.emplace_back(0.24f, 0.24f);
        for (size_t i = 0; i < positions.size(); ++i)
        {
            positions[i] += glm::vec2(0.5f, 0.5f);
        }
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(shape06Mesh_, positions, 0.12f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape06Mesh_);
        Project001::MeshLoader::RecenterMesh(shape06Mesh_);
    }

    {
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
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(shape07Mesh_, positions, 0.04f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape07Mesh_);
        Project001::MeshLoader::RecenterMesh(shape07Mesh_);
    }

    _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(shape08Mesh_, 0.32f, 3));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape08Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape08Mesh_, glm::vec2(0.5f, 0.5f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(shape09Mesh_, 0.32f, 4));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape09Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape09Mesh_, glm::vec2(0.5f, 0.5f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(shape10Mesh_, 0.32f, 5));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape10Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape10Mesh_, glm::vec2(0.5f, 0.5f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(shape11Mesh_, 0.32f, 6));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape11Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape11Mesh_, glm::vec2(0.5f, 0.5f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(shape12Mesh_, 0.32f, 7));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape12Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape12Mesh_, glm::vec2(0.5f, 0.5f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(shape13Mesh_, 0.32f, 8));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape13Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape13Mesh_, glm::vec2(0.5f, 0.5f));
    
    _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(shape14Mesh_, 0.32f, 24));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape14Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape14Mesh_, glm::vec2(0.5f, 0.5f));
    Project001::MeshLoader::ScaleMesh(shape14Mesh_, glm::vec3(2.0f, 1.5f, 1.0f));
    Project001::MeshLoader::RotateMesh(shape14Mesh_, glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() / -4.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
    Project001::MeshLoader::RotateMesh(shape14Mesh_, glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() / -4.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    Project001::MeshLoader::RotateMesh(shape14Mesh_, glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() / -4.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
    Project001::MeshLoader::TranslateMesh(shape14Mesh_, glm::vec3(0.16f, 0.0f, 0.0f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(shape15Mesh_, 0.08f, 0.32f, 1, 0.0f, 0.5f * glm::pi<float>()));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape15Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape15Mesh_, glm::vec2(0.5f, 0.5f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(shape16Mesh_, 0.08f, 0.32f, 2, 0.0f, 1.5f * glm::pi<float>()));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape16Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape16Mesh_, glm::vec2(0.5f, 0.5f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(shape17Mesh_, 0.08f, 0.32f, 3, 0.0f, 2.0f * glm::pi<float>()));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape17Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape17Mesh_, glm::vec2(0.5f, 0.5f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(shape18Mesh_, 0.08f, 0.32f, 4, 0.0f, 2.0f * glm::pi<float>()));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape18Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape18Mesh_, glm::vec2(0.5f, 0.5f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(shape19Mesh_, 0.08f, 0.32f, 4, 1.5f * glm::pi<float>(), 1.0f * glm::pi<float>()));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape19Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape19Mesh_, glm::vec2(0.5f, 0.5f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(shape20Mesh_, 0.08f, 0.32f, 24, 1.25f * glm::pi<float>(), 0.75f * glm::pi<float>()));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape20Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape20Mesh_, glm::vec2(0.5f, 0.5f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(shape21Mesh_, 0.16f, 0.32f, 32, 0.0f, 2.0f * glm::pi<float>()));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape21Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape21Mesh_, glm::vec2(0.5f, 0.5f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(shape22Mesh_, 0.32f, 0.32f, 2));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape22Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape22Mesh_, glm::vec2(0.5f, 0.5f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(shape23Mesh_, 0.32f, 0.32f, 4));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape23Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape23Mesh_, glm::vec2(0.5f, 0.5f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(shape24Mesh_, 0.32f, 0.32f, 8));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape24Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape24Mesh_, glm::vec2(0.5f, 0.5f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DBezeledRectangle(shape25Mesh_, 0.64f, 0.64f, 0.16f, 1));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape25Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape25Mesh_, glm::vec2(0.5f, 0.5f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DBezeledRectangle(shape26Mesh_, 0.64f, 0.64f, 0.16f, 2));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape26Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape26Mesh_, glm::vec2(0.5f, 0.5f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DBezeledRectangle(shape27Mesh_, 0.64f, 0.64f, 0.16f, 3));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape27Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape27Mesh_, glm::vec2(0.5f, 0.5f));

    _FAIL_CHECK(Project001::MeshLoader::Generate2DBezeledRectangle(shape28Mesh_, 0.64f, 0.48f, 0.16f, 4));
    Project001::MeshLoader::ApplyPositionalTextureCoordinates(shape28Mesh_);
    Project001::MeshLoader::TranslateTextureCoordinates(shape28Mesh_, glm::vec2(0.5f, 0.5f));

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
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(cubeEntity01Id_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(cubeEntity01Id_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(cubeEntity01Id_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&cubeMesh_);
        renderedModelPtr->SetTextureId(thonkTextureId_);
        renderedModelPtr->SetSpecularId(thonkSpecularTextureId_);
        renderedModelPtr->SetShininess(32.0f);
    }

    // cube entity 02
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(cubeEntity02Id_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(cubeEntity02Id_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(cubeEntity02Id_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&cubeMesh_);
        renderedModelPtr->SetTextureId(dice01TextureId_);
        renderedModelPtr->SetShininess(32.0f);
    }

    // cube entity 03
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(cubeEntity03Id_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(cubeEntity03Id_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(cubeEntity03Id_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&cubeMesh_);
        renderedModelPtr->SetTextureId(dice02TextureId_);
        renderedModelPtr->SetColorRGB(0.8f, 0.2f, 0.2f);
        renderedModelPtr->SetShininess(32.0f);
    }

    // cube entity 04
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(cubeEntity04Id_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(cubeEntity04Id_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(cubeEntity04Id_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&cubeMesh_);
        renderedModelPtr->SetColor(1.0f, 1.0f, 1.0f, 0.25f);
        renderedModelPtr->SetTranslucent(true);
    }

    // cube entity 05
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(cubeEntity05Id_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(cubeEntity05Id_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(cubeEntity05Id_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&cubeMesh_);
        renderedModelPtr->SetColor(1.0f, 1.0f, 1.0f, 0.25f);
        renderedModelPtr->SetTranslucent(true);
    }

    // cube entity 06
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(cubeEntity06Id_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(cubeEntity06Id_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(cubeEntity06Id_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&cubeMesh_);
        renderedModelPtr->SetSpecularId(patternSpecular02TextureId_);
        renderedModelPtr->SetScale(0.5f, 0.75f, 1.0f);
        renderedModelPtr->SetColorRGB(0.2f, 0.8f, 0.2f);
        renderedModelPtr->SetShininess(24.0f);
    }

    // cube entity 07
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(cubeEntity07Id_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(cubeEntity07Id_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(cubeEntity07Id_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&cubeMesh_);
        renderedModelPtr->SetSpecularId(patternSpecular02TextureId_);
        renderedModelPtr->AddPitch(glm::pi<float>() / 4.0f);
        renderedModelPtr->AddYaw(glm::pi<float>() / 4.0f);
        renderedModelPtr->SetColorRGB(0.2f, 0.2f, 0.8f);
        renderedModelPtr->SetShininess(4.0f);
    }

    // generated shape entity 01
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape01EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape01EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape01EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape01Mesh_);
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(0.8f, 0.6f, 0.2f);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(0.8f, 0.2f, 0.6f);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(0.6f, 0.2f, 0.8f);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(0.2f, 0.6f, 0.8f);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(0.2f, 0.8f, 0.6f);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(0.6f, 0.8f, 0.2f);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(1.0f, 1.0f, 1.0f);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(1.0f, 0.0f, 0.0f);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(0.0f, 1.0f, 0.0f);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(0.0f, 0.0f, 1.0f);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(0.0f, 1.0f, 1.0f);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(1.0f, 0.0f, 1.0f);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetColorRGB(1.0f, 1.0f, 0.0f);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
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
        renderedModelPtr->SetTextureId(_100x100TextureId_);
        renderedModelPtr->SetTranslucent(true);
    }
}

void TestScene002::Deinitialize()
{
    TestSceneBase001::Deinitialize();

    ClearIndiciesAndEntityIds();
}

void TestScene002::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene002::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneBase001::OnEvent(event);
}

// protected: ------------------------------------------------------------------

void TestScene002::ClearIndiciesAndEntityIds()
{
    cubeMesh_.Clear();
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

    dice01TextureId_ = (unsigned int)-1;
    dice02TextureId_ = (unsigned int)-1;
    thonkTextureId_ = (unsigned int)-1;
    _100x100TextureId_ = (unsigned int)-1;
    patternSpecular01TextureId_ = (unsigned int)-1;
    patternSpecular02TextureId_ = (unsigned int)-1;
    thonkSpecularTextureId_ = (unsigned int)-1;

    mainCameraEntityId_ = (unsigned int)-1;
    lightSourceEntityId_ = (unsigned int)-1;
    cubeEntity01Id_ = (unsigned int)-1;
    cubeEntity02Id_ = (unsigned int)-1;
    cubeEntity03Id_ = (unsigned int)-1;
    cubeEntity04Id_ = (unsigned int)-1;
    cubeEntity05Id_ = (unsigned int)-1;
    cubeEntity06Id_ = (unsigned int)-1;
    cubeEntity07Id_ = (unsigned int)-1;
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