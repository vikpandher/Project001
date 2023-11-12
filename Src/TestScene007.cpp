#include "TestScene007.h"

#include "Engine/Components/RenderedMesh.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/Renderer.h"
#include "Engine/TextureLoader.h"
#include "Engine/Window.h"



// public ----------------------------------------------------------------------

TestScene007::TestScene007(Project001::Application* applicationPtr)
    : TestSceneBase001(applicationPtr, "TestScene007")
{}

TestScene007::~TestScene007()
{}

void TestScene007::HandleEvent(Project001::Event& event)
{
    TestSceneBase001::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene007::ProcessInitializeEvent, this, std::placeholders::_1));
}

// protected -------------------------------------------------------------------

void TestScene007::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    // Load textures -----------------------------------------------------------

    unsigned int _32x32_123abc_TextureId;

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/123456789abcdefghij.png"));
        // _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/viking_room.png"));
        rendererPtr_->CreateTexture(_32x32_123abc_TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    // Creating Entities
    // -------------------------------------------------------------------------

    // Calculating positions ---------------------------------------------------

    std::vector<glm::vec3> meshEntityPositions;
    meshEntityPositions.emplace_back(0.0f, 0.0f, 0.0f);
    // for (int i = 2; i >= -2; --i)
    // {
    //     for (int j = -3; j <= 3; ++j)
    //     {
    //         meshEntityPositions.emplace_back((float)j, (float)i, 0.0f);
    //     }
    // }
    size_t positionPosition = 0;

    // Generated Shape Entity 01
    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        // _FAIL_CHECK(Project001::MeshLoader::LoadMeshOBJ(*newMeshDataPtr, "../Models/viking_room.obj"));
        // Project001::MeshLoader::RotateMeshX(*newMeshDataPtr, -glm::half_pi<float>() - glm::quarter_pi<float>());
        // Project001::MeshLoader::RotateMeshY(*newMeshDataPtr, -glm::half_pi<float>() - glm::quarter_pi<float>());
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.32f, 0.0f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine_v2(*newMeshDataPtr, positions, 0.08f, 4));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);

        // _LOG_MESSAGE("VertexCount = %i, IndexCount = %i", newMeshData.vertexCount, newMeshData.indexCount);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetTextureId(_32x32_123abc_TextureId);
    }
}