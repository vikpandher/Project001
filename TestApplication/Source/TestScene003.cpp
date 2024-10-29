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
    _LOG_MESSAGE("INITIALIZING:   TestScene003:            %u", GetId());

    // Mesh Data ---------------------------------------------------------------

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, 0.64f, 0.48f, 0.32f, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateSphere(*newMeshDataPtr, 0.32f, 3, 2));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateSphere(*newMeshDataPtr, 0.32f, 4, 4));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateSphere(*newMeshDataPtr, 0.32f, 8, 8));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateSphere(*newMeshDataPtr, 0.32f, 3, 2, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateSphere(*newMeshDataPtr, 0.32f, 4, 4, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateSphere(*newMeshDataPtr, 0.32f, 8, 8, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, 0.64f, 0.64f, 0.64f, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(*newMeshDataPtr, 0.32f, 0));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(*newMeshDataPtr, 0.32f, 1));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(*newMeshDataPtr, 0.32f, 2));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(*newMeshDataPtr, 0.32f, 0, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(*newMeshDataPtr, 0.32f, 1, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(*newMeshDataPtr, 0.32f, 2, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, 0.64f, 0.48f, 0.32f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateCylinder(*newMeshDataPtr, 0.64f, 0.32f, 3));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateCylinder(*newMeshDataPtr, 0.64f, 0.32f, 4));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateCylinder(*newMeshDataPtr, 0.64f, 0.32f, 8));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateCylinder(*newMeshDataPtr, 0.64f, 0.32f, 3, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateCylinder(*newMeshDataPtr, 0.64f, 0.32f, 4, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateCylinder(*newMeshDataPtr, 0.64f, 0.32f, 8, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, 0.64f, 0.64f, 0.64f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateCapsule(*newMeshDataPtr, 0.32f, 0.16f, 3, 1));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateCapsule(*newMeshDataPtr, 0.32f, 0.16f, 4, 2));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateCapsule(*newMeshDataPtr, 0.32f, 0.16f, 8, 4));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateCapsule(*newMeshDataPtr, 0.32f, 0.16f, 3, 1, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateCapsule(*newMeshDataPtr, 0.32f, 0.16f, 4, 2, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateCapsule(*newMeshDataPtr, 0.32f, 0.16f, 8, 4, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, 0.32f, 0.32f, 0.32f, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateCone(*newMeshDataPtr, 0.64f, 0.32f, 3));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateCone(*newMeshDataPtr, 0.64f, 0.32f, 4));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateCone(*newMeshDataPtr, 0.64f, 0.32f, 8));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateCone(*newMeshDataPtr, 0.64f, 0.32f, 3, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateCone(*newMeshDataPtr, 0.64f, 0.32f, 4, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateCone(*newMeshDataPtr, 0.64f, 0.32f, 8, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, 0.32f, 0.32f, 0.32f, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateTube(*newMeshDataPtr, glm::vec3(-0.64f, -0.64f, -0.64f), glm::vec3(0.64f, 0.64f, 0.64f), 0.12f, 3));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateTube(*newMeshDataPtr, glm::vec3(-0.64f, -0.64f, -0.64f), glm::vec3(0.64f, 0.64f, 0.64f), 0.12f, 4));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateTube(*newMeshDataPtr, glm::vec3(-0.64f, -0.64f, -0.64f), glm::vec3(0.64f, 0.64f, 0.64f), 0.12f, 8));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateTube(*newMeshDataPtr, glm::vec3(-0.64f, -0.64f, -0.64f), glm::vec3(0.64f, 0.64f, 0.64f), 0.12f, 3, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateTube(*newMeshDataPtr, glm::vec3(-0.64f, -0.64f, -0.64f), glm::vec3(0.64f, 0.64f, 0.64f), 0.12f, 4, false));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateTube(*newMeshDataPtr, glm::vec3(-0.64f, -0.64f, -0.64f), glm::vec3(0.64f, 0.64f, 0.64f), 0.12f, 8, false));
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

    // Texture Data ------------------------------------------------------------

    unsigned int earthTextureId = (unsigned int)-1;
    unsigned int rgb120x60TextureId = (unsigned int)-1;

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/earth.png"));
        rendererPtr_->CreateTexture(earthTextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/120_60_rgb.png"));
        rendererPtr_->CreateTexture(rgb120x60TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    // Creating Entities
    // -------------------------------------------------------------------------

    // Calculating positions ---------------------------------------------------

    std::vector<glm::vec3> meshEntityPositions;
    for (int i = 2; i >= -3; --i)
    {
        for (int j = -3; j <= 3; ++j)
        {
            meshEntityPositions.emplace_back((float)j, (float)i, 0.0f);
        }
    }
    size_t positionPosition = 0;

    // Generated Shape Entity 01
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[0]);
        }
    }

    // Generated Shape Entity 02
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[1]);
            renderedMeshPtr->SetTextureId(earthTextureId);
        }
    }

    // Generated Shape Entity 03
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[2]);
            renderedMeshPtr->SetTextureId(earthTextureId);
        }
    }

    // Generated Shape Entity 04
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[3]);
            renderedMeshPtr->SetTextureId(earthTextureId);
        }
    }

    // Generated Shape Entity 05
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[4]);
            renderedMeshPtr->SetTextureId(earthTextureId);
        }
    }

    // Generated Shape Entity 06
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[5]);
            renderedMeshPtr->SetTextureId(earthTextureId);
        }
    }

    // Generated Shape Entity 07
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[6]);
            renderedMeshPtr->SetTextureId(earthTextureId);
        }
    }

    // Generated Shape Entity 08
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[7]);
        }
    }

    // Generated Shape Entity 09
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[8]);
            renderedMeshPtr->SetTextureId(rgb120x60TextureId);
        }
    }

    // Generated Shape Entity 10
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[9]);
            renderedMeshPtr->SetTextureId(rgb120x60TextureId);
        }
    }

    // Generated Shape Entity 11
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[10]);
            renderedMeshPtr->SetTextureId(rgb120x60TextureId);
        }
    }

    // Generated Shape Entity 12
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[11]);
            renderedMeshPtr->SetTextureId(rgb120x60TextureId);
        }
    }

    // Generated Shape Entity 13
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[12]);
            renderedMeshPtr->SetTextureId(rgb120x60TextureId);
        }
    }

    // Generated Shape Entity 14
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[13]);
            renderedMeshPtr->SetTextureId(rgb120x60TextureId);
        }
    }

    // Generated Shape Entity 15
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[14]);
        }
    }

    // Generated Shape Entity 16
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[15]);
        }
    }

    // Generated Shape Entity 17
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[16]);
        }
    }

    // Generated Shape Entity 18
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[17]);
        }
    }

    // Generated Shape Entity 19
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[18]);
        }
    }

    // Generated Shape Entity 20
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[19]);
        }
    }

    // Generated Shape Entity 21
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[20]);
        }
    }

    // Generated Shape Entity 22
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[21]);
        }
    }

    // Generated Shape Entity 23
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[22]);
        }
    }

    // Generated Shape Entity 24
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[23]);
        }
    }

    // Generated Shape Entity 25
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[24]);
        }
    }

    // Generated Shape Entity 26
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[25]);
        }
    }

    // Generated Shape Entity 27
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[26]);
        }
    }

    // Generated Shape Entity 28
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[27]);
        }
    }

    // Generated Shape Entity 29
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[28]);
        }
    }

    // Generated Shape Entity 30
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[29]);
        }
    }

    // Generated Shape Entity 31
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[30]);
        }
    }

    // Generated Shape Entity 32
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[31]);
        }
    }

    // Generated Shape Entity 33
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[32]);
        }
    }

    // Generated Shape Entity 34
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[33]);
        }
    }

    // Generated Shape Entity 35
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[34]);
        }
    }

    // Generated Shape Entity 36
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[35]);
        }
    }

    // Generated Shape Entity 37
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[36]);
        }
    }

    // Generated Shape Entity 38
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[37]);
        }
    }

    // Generated Shape Entity 39
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[38]);
        }
    }

    // Generated Shape Entity 40
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[39]);
        }
    }

    // Generated Shape Entity 41
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[40]);
        }
    }

    // Generated Shape Entity 42
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
            renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[41]);
        }
    }

    // Member Scenes -----------------------------------------------------------

    Project001::FontData font01_FontData;
    _FAIL_CHECK(Project001::FontLoader::LoadFontDataFromMemory(
        font01_FontData,
        g_AntonioRegular_ssf,
        sizeof(g_AntonioRegular_ssf)
    ));

    Project001::TextureData font01_TextureData;
    _FAIL_CHECK(Project001::TextureLoader::LoadTextureFromMemory(
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

    _LOG_MESSAGE("DEINITIALIZING: TestScene003:            %u", GetId());
}