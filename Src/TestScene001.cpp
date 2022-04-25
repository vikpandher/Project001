#include "TestScene001.h"

#include "TestComponents.h"

#include "Engine/Components/RenderedModel.h"
#include "Engine/Application.h"
#include "Engine/BiMap.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/LRUArray.h"
#include "Engine/MeshStores.h"
#include "Engine/Renderer.h"
#include "Engine/TextureStores.h"
#include "Engine/Window.h"



// public: ---------------------------------------------------------------------

TestScene001::TestScene001()
{
    ClearIndiciesAndEntityIds();

    BiMapTest();
    ComponentContainerTest();
    ComponentStoresTest();
    LRUArrayTest();
    MeshStoresTest();
    TextureStoresTest();
}

TestScene001::~TestScene001()
{}

const char* TestScene001::Name()
{
    return "TestScene001";
}

void TestScene001::Initialize()
{
    TestSceneBase001::Initialize();

    // Load meshes
    // -------------------------------------------------------------------------

    _FAIL_CHECK(meshStoresPtr_->LoadMeshOBJ(cubeMeshIndex_, "../Models/Cube.obj", false));

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
        _FAIL_CHECK(meshStoresPtr_->Generate2DTriangleFan(shape01MeshIndex_, positions, positions));
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
        _FAIL_CHECK(meshStoresPtr_->Generate2DTriangleStrip(shape02MeshIndex_, positions, positions));
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
        _FAIL_CHECK(meshStoresPtr_->Generate2DTriangles(shape03MeshIndex_, positions, positions));
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
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape04MeshIndex_, positions, 0.08f));
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
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape05MeshIndex_, positions, 0.04f));
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
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape06MeshIndex_, positions, 0.12f));
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
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape07MeshIndex_, positions, 0.04f));
    }

    _FAIL_CHECK(meshStoresPtr_->Generate2DRegularPolygon(shape08MeshIndex_, 0.32f, 3));
    _FAIL_CHECK(meshStoresPtr_->Generate2DRegularPolygon(shape09MeshIndex_, 0.32f, 4));
    _FAIL_CHECK(meshStoresPtr_->Generate2DRegularPolygon(shape10MeshIndex_, 0.32f, 5));
    _FAIL_CHECK(meshStoresPtr_->Generate2DRegularPolygon(shape11MeshIndex_, 0.32f, 6));
    _FAIL_CHECK(meshStoresPtr_->Generate2DRegularPolygon(shape12MeshIndex_, 0.32f, 7));
    _FAIL_CHECK(meshStoresPtr_->Generate2DRegularPolygon(shape13MeshIndex_, 0.32f, 8));
    
    _FAIL_CHECK(meshStoresPtr_->Generate2DRegularPolygon(shape14MeshIndex_, 0.32f, 24));
    _FAIL_CHECK(meshStoresPtr_->ScaleMesh(shape14MeshIndex_, glm::vec3(2.0f, 1.5f, 1.0f)));
    _FAIL_CHECK(meshStoresPtr_->RotateMesh(shape14MeshIndex_, glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() / -4.0f, glm::vec3(0.0f, 0.0f, 1.0f))));
    _FAIL_CHECK(meshStoresPtr_->RotateMesh(shape14MeshIndex_, glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() / -4.0f, glm::vec3(0.0f, 1.0f, 0.0f))));
    _FAIL_CHECK(meshStoresPtr_->RotateMesh(shape14MeshIndex_, glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() / -4.0f, glm::vec3(1.0f, 0.0f, 0.0f))));
    _FAIL_CHECK(meshStoresPtr_->TranslateMesh(shape14MeshIndex_, glm::vec3(0.16f, 0.0f, 0.0f)));

    _FAIL_CHECK(meshStoresPtr_->Generate2DArc(shape15MeshIndex_, 0.08f, 0.32f, 1, 0.0f, 0.5f * glm::pi<float>()));
    _FAIL_CHECK(meshStoresPtr_->Generate2DArc(shape16MeshIndex_, 0.08f, 0.32f, 2, 0.0f, 1.5f * glm::pi<float>()));
    _FAIL_CHECK(meshStoresPtr_->Generate2DArc(shape17MeshIndex_, 0.08f, 0.32f, 3, 0.0f, 2.0f * glm::pi<float>()));
    _FAIL_CHECK(meshStoresPtr_->Generate2DArc(shape18MeshIndex_, 0.08f, 0.32f, 4, 0.0f, 2.0f * glm::pi<float>()));
    _FAIL_CHECK(meshStoresPtr_->Generate2DArc(shape19MeshIndex_, 0.08f, 0.32f, 4, 1.5f * glm::pi<float>(), 1.0f * glm::pi<float>()));
    _FAIL_CHECK(meshStoresPtr_->Generate2DArc(shape20MeshIndex_, 0.08f, 0.32f, 24, 1.25f * glm::pi<float>(), 0.75f * glm::pi<float>()));
    _FAIL_CHECK(meshStoresPtr_->Generate2DArc(shape21MeshIndex_, 0.16f, 0.32f, 32, 0.0f, 2.0f * glm::pi<float>()));

    _FAIL_CHECK(meshStoresPtr_->Generate2DCapsule(shape22MeshIndex_, 0.32f, 0.32f, 2));
    _FAIL_CHECK(meshStoresPtr_->Generate2DCapsule(shape23MeshIndex_, 0.32f, 0.32f, 4));
    _FAIL_CHECK(meshStoresPtr_->Generate2DCapsule(shape24MeshIndex_, 0.32f, 0.32f, 8));

    _FAIL_CHECK(meshStoresPtr_->Generate2DBezeledRectangle(shape25MeshIndex_, 0.64f, 0.64f, 0.16f, 1));
    _FAIL_CHECK(meshStoresPtr_->Generate2DBezeledRectangle(shape26MeshIndex_, 0.64f, 0.64f, 0.16f, 2));
    _FAIL_CHECK(meshStoresPtr_->Generate2DBezeledRectangle(shape27MeshIndex_, 0.64f, 0.64f, 0.16f, 3));
    _FAIL_CHECK(meshStoresPtr_->Generate2DBezeledRectangle(shape28MeshIndex_, 0.64f, 0.48f, 0.16f, 4));

    // Load textures
    // -------------------------------------------------------------------------

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(diceTexture01Index_, "../Textures/CounterclockwiseDie.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(diceTexture01Index_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(diceTexture01Index_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(diceTexture02Index_, "../Textures/HallowDie.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(diceTexture02Index_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(diceTexture02Index_, 2, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(thonkTextureIndex_, "../Textures/Thonk.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(thonkTextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(thonkTextureIndex_, 3, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_100x100TextureIndex_, "../Textures/100x100_2.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_100x100TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_100x100TextureIndex_, 4, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(patternSpecularTexture01Index_, "../Textures/Specular1.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(patternSpecularTexture01Index_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(patternSpecularTexture01Index_, 6, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(patternSpecularTexture02Index_, "../Textures/Specular2.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(patternSpecularTexture02Index_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(patternSpecularTexture02Index_, 7, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(thonkSpecularTextureIndex_, "../Textures/ThonkSpecular.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(thonkSpecularTextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(thonkSpecularTextureIndex_, 8, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
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
        renderedModelPtr->SetMeshIndex(cubeMeshIndex_);
        renderedModelPtr->SetTextureIndex(thonkTextureIndex_);
        renderedModelPtr->SetSpecularIndex(thonkSpecularTextureIndex_);
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
        renderedModelPtr->SetMeshIndex(cubeMeshIndex_);
        renderedModelPtr->SetTextureIndex(diceTexture01Index_);
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
        renderedModelPtr->SetMeshIndex(cubeMeshIndex_);
        renderedModelPtr->SetTextureIndex(diceTexture02Index_);
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
        renderedModelPtr->SetMeshIndex(cubeMeshIndex_);
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
        renderedModelPtr->SetMeshIndex(cubeMeshIndex_);
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
        renderedModelPtr->SetMeshIndex(cubeMeshIndex_);
        renderedModelPtr->SetSpecularIndex(patternSpecularTexture02Index_);
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
        renderedModelPtr->SetMeshIndex(cubeMeshIndex_);
        renderedModelPtr->SetSpecularIndex(patternSpecularTexture01Index_);
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
        renderedModelPtr->SetMeshIndex(shape01MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape02MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape03MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape04MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape05MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape06MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape07MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape08MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape09MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape10MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape11MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape12MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape13MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape14MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape15MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape16MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape17MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape18MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape19MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape20MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape21MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
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
        renderedModelPtr->SetMeshIndex(shape22MeshIndex_);
    }

    // generated shape entity 23
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape23EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape23EntityId_));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape23EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape23MeshIndex_);
    }

    // generated shape entity 24
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape24EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape24EntityId_));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape24EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape24MeshIndex_);
    }

    // generated shape entity 25
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape25EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape25EntityId_));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape25EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape25MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
    }

    // generated shape entity 26
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape26EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape26EntityId_));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape26EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape26MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
    }

    // generated shape entity 27
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape27EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape27EntityId_));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape27EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape27MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
    }

    // generated shape entity 28
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape28EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape28EntityId_));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape28EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape28MeshIndex_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
    }
}

void TestScene001::Deinitialize()
{
    TestSceneBase001::Deinitialize();

    ClearIndiciesAndEntityIds();
}

void TestScene001::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene001::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneBase001::OnEvent(event);
}

// protected: ------------------------------------------------------------------

void TestScene001::ClearIndiciesAndEntityIds()
{
    cubeMeshIndex_ = (unsigned int)-1;
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

    diceTexture01Index_ = (unsigned int)-1;
    diceTexture02Index_ = (unsigned int)-1;
    thonkTextureIndex_ = (unsigned int)-1;
    _100x100TextureIndex_ = (unsigned int)-1;
    patternSpecularTexture01Index_ = (unsigned int)-1;
    patternSpecularTexture02Index_ = (unsigned int)-1;
    thonkSpecularTextureIndex_ = (unsigned int)-1;

    sceneDataEntityId_ = (unsigned int)-1;
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

void TestScene001::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene002"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene002"));
            }
        }
    }
}

// private: --------------------------------------------------------------------

void TestScene001::BiMapTest() const
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

void TestScene001::ComponentContainerTest() const
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
}

void TestScene001::ComponentStoresTest() const
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

void TestScene001::LRUArrayTest() const
{
    Project001::LRUArray<unsigned int> testLRUArray(4);
    testLRUArray.Add(0);
    testLRUArray.Add(1);
    testLRUArray.Add(2);
    testLRUArray.Add(3);
    unsigned int x0 = testLRUArray.Get(0);
    unsigned int x1 = testLRUArray.Get(1);
    unsigned int x2 = testLRUArray.Get(2);
    unsigned int x3 = testLRUArray.Get(3);
    size_t size = testLRUArray.Size();

    unsigned int temp = testLRUArray.Get(0);
    testLRUArray.Add(4);
    temp = testLRUArray.Get(2);
    testLRUArray.Add(5);
    testLRUArray.Add(6);

    x0 = testLRUArray[0];
    x1 = testLRUArray[1];
    x2 = testLRUArray[2];
    x3 = testLRUArray[3];

    testLRUArray[0] = 0;
    testLRUArray[1] = 1;
    testLRUArray[2] = 2;
    testLRUArray[3] = 3;
}

void TestScene001::MeshStoresTest() const
{
    bool testBool = false;

    Project001::MeshStores testMeshStores;

    unsigned int model0;
    testBool = testMeshStores.LoadMeshOBJ(model0, "");
    testBool = testMeshStores.LoadMeshOBJ(model0, "../Models/CubeQ.obj");
}

void TestScene001::TextureStoresTest() const
{
    bool testBool = false;

    Project001::TextureStores testTextureStores;

    unsigned int texture0;
    testBool = testTextureStores.LoadTexture(texture0, "");
    testBool = testTextureStores.LoadTexture(texture0, "../Textures/rgb.png");

    unsigned int texture1;
    testBool = testTextureStores.LoadTexture(texture1, "../Textures/Cube.png");

    unsigned int texture2;
    testBool = testTextureStores.LoadTexture(texture2, "../Textures/CubeSpecular.png");
}