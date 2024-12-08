// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#include "TestScene003.h"

#include "TestResource_AntonioRegular_png.h"
#include "TestResource_AntonioRegular_ssf.h"

#include "Components/RenderedMesh.h"
#include "Application.h"
#include "ComponentStores.h"
#include "FontLoader.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "Renderer.h"
#include "TextureLoader.h"
#include "Window.h"



// public ----------------------------------------------------------------------

TestScene003::TestScene003(Project001::Application* applicationPtr)
    : TestSceneBase001(applicationPtr)
    , instructionScene_(applicationPtr)
{}

TestScene003::~TestScene003()
{}

void TestScene003::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene003::ProcessDeinitializeEvent, this, std::placeholders::_1));

    TestSceneBase001::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene003::ProcessInitializeEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene003::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene003:            " << GetId());

    // Texture Data ------------------------------------------------------------

    unsigned int earthTextureId = (unsigned int)-1;
    unsigned int rgb120x60TextureId = (unsigned int)-1;

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/earth.png"));
        rendererPtr_->CreateTexture(earthTextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/120_60_rgb.png"));
        rendererPtr_->CreateTexture(rgb120x60TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    // Creating Entities
    // -------------------------------------------------------------------------

    // Calculating positions ---------------------------------------------------

    std::vector<glm::vec3> meshEntityPositions;
    for (int j = 3; j >= -4; --j)
    {
        for (int i = -3; i <= 2; ++i)
        {
            meshEntityPositions.emplace_back((float)i, (float)j, 0.0f);
        }
    }

    size_t positionIndex = 0;

    // row 1 -------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, 0.64f, 0.48f, 0.32f, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, 0.64f, 0.64f, 0.64f, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, 0.64f, 0.48f, 0.32f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, 0.64f, 0.64f, 0.64f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, 0.32f, 0.32f, 0.32f, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, 0.32f, 0.32f, 0.32f, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // row 2 -------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateSphere(*newMeshDataPtr, 0.32f, 3, 2));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(earthTextureId);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateSphere(*newMeshDataPtr, 0.32f, 4, 4));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(earthTextureId);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateSphere(*newMeshDataPtr, 0.32f, 8, 8));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(earthTextureId);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateSphere(*newMeshDataPtr, 0.32f, 3, 2, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(earthTextureId);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateSphere(*newMeshDataPtr, 0.32f, 4, 4, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(earthTextureId);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateSphere(*newMeshDataPtr, 0.32f, 8, 8, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(earthTextureId);
        }
    }

    // row 3 -------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(*newMeshDataPtr, 0.32f, 0));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(rgb120x60TextureId);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(*newMeshDataPtr, 0.32f, 1));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(rgb120x60TextureId);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(*newMeshDataPtr, 0.32f, 2));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(rgb120x60TextureId);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(*newMeshDataPtr, 0.32f, 0, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(rgb120x60TextureId);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(*newMeshDataPtr, 0.32f, 1, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(rgb120x60TextureId);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(*newMeshDataPtr, 0.32f, 2, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(rgb120x60TextureId);
        }
    }

    // row 4 -------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere_v2(*newMeshDataPtr, 0.32f, 0));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(rgb120x60TextureId);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere_v2(*newMeshDataPtr, 0.32f, 1));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(rgb120x60TextureId);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere_v2(*newMeshDataPtr, 0.32f, 2));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(rgb120x60TextureId);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere_v2(*newMeshDataPtr, 0.32f, 0, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(rgb120x60TextureId);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere_v2(*newMeshDataPtr, 0.32f, 1, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(rgb120x60TextureId);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere_v2(*newMeshDataPtr, 0.32f, 2, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(rgb120x60TextureId);
        }
    }

    // row 5 -------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateCylinder(*newMeshDataPtr, 0.64f, 0.32f, 3));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateCylinder(*newMeshDataPtr, 0.64f, 0.32f, 4));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateCylinder(*newMeshDataPtr, 0.64f, 0.32f, 8));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateCylinder(*newMeshDataPtr, 0.64f, 0.32f, 3, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateCylinder(*newMeshDataPtr, 0.64f, 0.32f, 4, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateCylinder(*newMeshDataPtr, 0.64f, 0.32f, 8, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // row 6 -------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateCapsule(*newMeshDataPtr, 0.32f, 0.16f, 3, 1));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateCapsule(*newMeshDataPtr, 0.32f, 0.16f, 4, 2));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateCapsule(*newMeshDataPtr, 0.32f, 0.16f, 8, 4));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateCapsule(*newMeshDataPtr, 0.32f, 0.16f, 3, 1, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateCapsule(*newMeshDataPtr, 0.32f, 0.16f, 4, 2, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateCapsule(*newMeshDataPtr, 0.32f, 0.16f, 8, 4, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // row 7 -------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateCone(*newMeshDataPtr, 0.64f, 0.32f, 3));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateCone(*newMeshDataPtr, 0.64f, 0.32f, 4));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateCone(*newMeshDataPtr, 0.64f, 0.32f, 8));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateCone(*newMeshDataPtr, 0.64f, 0.32f, 3, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateCone(*newMeshDataPtr, 0.64f, 0.32f, 4, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateCone(*newMeshDataPtr, 0.64f, 0.32f, 8, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // row 8 -------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateTube(*newMeshDataPtr, glm::vec3(-0.64f, -0.64f, -0.64f), glm::vec3(0.64f, 0.64f, 0.64f), 0.12f, 3));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateTube(*newMeshDataPtr, glm::vec3(-0.64f, -0.64f, -0.64f), glm::vec3(0.64f, 0.64f, 0.64f), 0.12f, 4));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateTube(*newMeshDataPtr, glm::vec3(-0.64f, -0.64f, -0.64f), glm::vec3(0.64f, 0.64f, 0.64f), 0.12f, 8));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateTube(*newMeshDataPtr, glm::vec3(-0.64f, -0.64f, -0.64f), glm::vec3(0.64f, 0.64f, 0.64f), 0.12f, 3, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateTube(*newMeshDataPtr, glm::vec3(-0.64f, -0.64f, -0.64f), glm::vec3(0.64f, 0.64f, 0.64f), 0.12f, 4, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateTube(*newMeshDataPtr, glm::vec3(-0.64f, -0.64f, -0.64f), glm::vec3(0.64f, 0.64f, 0.64f), 0.12f, 8, false));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        }
    }

    // test TurnInsideOut
    if (false)
    {
        for (size_t i = 0; i < meshDataPtrArray_.size(); ++i)
        {
            Project001::MeshLoader::TurnInsideOut(*meshDataPtrArray_[i]);
        }
    }

    // test SizeMeshAlongNormals
    if (false)
    {
        for (size_t i = 0; i < meshDataPtrArray_.size(); ++i)
        {
            Project001::MeshLoader::SizeMeshAlongNormals(*meshDataPtrArray_[i], 0.16f);
        }
    }

    // Member Scenes -----------------------------------------------------------

    Project001::FontData font01_FontData;
    FAIL_CHECK(Project001::FontLoader::LoadFontDataFromMemory(
        font01_FontData,
        g_AntonioRegular_ssf,
        sizeof(g_AntonioRegular_ssf)
    ));

    Project001::TextureData font01_TextureData;
    FAIL_CHECK(Project001::TextureLoader::LoadTextureFromMemory(
        font01_TextureData,
        g_AntonioRegular_png,
        sizeof(g_AntonioRegular_png)
    ));
    unsigned int font01_TextureId = (unsigned int)-1;
    rendererPtr_->CreateTexture(
        font01_TextureId,
        font01_TextureData.data,
        font01_TextureData.width,
        font01_TextureData.height,
        font01_TextureData.bytesPerPixel,
        true,
        false
    );

    const Project001::KeyCode keyCode_toggleInstructions = Project001::KeyCode::KEY_CODE_TAB;

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "This Scene also tests Mesh Generation.\n"
        "Use <WASD> to move the camera up, left, down, and right.\n"
        "Use <Q> to roll left and <E> to roll right.\n"
        "Use <Scroll> to move forward and back.\n"
        "<Left-Click> and drag the <Mouse> to move camera.\n"
        "Press <Esc> to return to Main Menu.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = &font01_FontData;
    instructionSceneInfo.fontTextureIdPtr = &font01_TextureId;
    instructionSceneInfo.cameraEntityIdPtr = &uiCameraEntityId_;
    instructionSceneInfo.cameraMaskPtr = &s_uiCameraMask_;
    instructionSceneInfo.keyCode_toggleInstructionsPtr = &keyCode_toggleInstructions;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene003::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene003:            " << GetId());
}