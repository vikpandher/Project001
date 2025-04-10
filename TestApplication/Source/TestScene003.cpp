// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-04-10

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
    unsigned int numbers12x6TextureId = (unsigned int)-1;
    unsigned int box01_textureId = (unsigned int)-1;
    unsigned int box02_textureId = (unsigned int)-1;
    unsigned int box03_textureId = (unsigned int)-1;

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

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/12_6_numbers.png"));
        rendererPtr_->CreateTexture(numbers12x6TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/box_01.png"));
        rendererPtr_->CreateTexture(box01_textureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/box_02.png"));
        rendererPtr_->CreateTexture(box02_textureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/box_03.png"));
        rendererPtr_->CreateTexture(box03_textureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    std::vector<glm::vec2> box01_textureCoordinates;
    box01_textureCoordinates.reserve(24);
    box01_textureCoordinates.emplace_back(0.0f, 1.0f);        // front
    box01_textureCoordinates.emplace_back(0.0f, 0.5f);        //
    box01_textureCoordinates.emplace_back(1.0f / 3.0f, 1.0f); //
    box01_textureCoordinates.emplace_back(1.0f / 3.0f, 0.5f); //
    box01_textureCoordinates.emplace_back(1.0f / 3.0f, 1.0f); // right
    box01_textureCoordinates.emplace_back(1.0f / 3.0f, 0.5f); //
    box01_textureCoordinates.emplace_back(2.0f / 3.0f, 1.0f); //
    box01_textureCoordinates.emplace_back(2.0f / 3.0f, 0.5f); //
    box01_textureCoordinates.emplace_back(2.0f / 3.0f, 1.0f); // back
    box01_textureCoordinates.emplace_back(2.0f / 3.0f, 0.5f); //
    box01_textureCoordinates.emplace_back(1.0f, 1.0f);        //
    box01_textureCoordinates.emplace_back(1.0f, 0.5f);        //
    box01_textureCoordinates.emplace_back(0.0f, 0.5f);        // left
    box01_textureCoordinates.emplace_back(0.0f, 0.0f);        //
    box01_textureCoordinates.emplace_back(1.0f / 3.0f, 0.5f); //
    box01_textureCoordinates.emplace_back(1.0f / 3.0f, 0.0f); //
    box01_textureCoordinates.emplace_back(1.0f / 3.0f, 0.5f); // top
    box01_textureCoordinates.emplace_back(1.0f / 3.0f, 0.0f); //
    box01_textureCoordinates.emplace_back(2.0f / 3.0f, 0.5f); //
    box01_textureCoordinates.emplace_back(2.0f / 3.0f, 0.0f); //
    box01_textureCoordinates.emplace_back(2.0f / 3.0f, 0.5f); // bottom
    box01_textureCoordinates.emplace_back(2.0f / 3.0f, 0.0f); //
    box01_textureCoordinates.emplace_back(1.0f, 0.5f);        //
    box01_textureCoordinates.emplace_back(1.0f, 0.0f);        //

    std::vector<glm::vec2> box02_textureCoordinates;
    box02_textureCoordinates.reserve(24);
    box02_textureCoordinates.emplace_back(0.0f, 1.0f); // front
    box02_textureCoordinates.emplace_back(0.0f, 0.5f); //
    box02_textureCoordinates.emplace_back(0.4f, 1.0f); //
    box02_textureCoordinates.emplace_back(0.4f, 0.5f); //
    box02_textureCoordinates.emplace_back(0.4f, 1.0f); // right
    box02_textureCoordinates.emplace_back(0.4f, 0.5f); //
    box02_textureCoordinates.emplace_back(0.6f, 1.0f); //
    box02_textureCoordinates.emplace_back(0.6f, 0.5f); //
    box02_textureCoordinates.emplace_back(0.6f, 1.0f); // back
    box02_textureCoordinates.emplace_back(0.6f, 0.5f); //
    box02_textureCoordinates.emplace_back(1.0f, 1.0f); //
    box02_textureCoordinates.emplace_back(1.0f, 0.5f); //
    box02_textureCoordinates.emplace_back(0.0f, 0.5f); // left
    box02_textureCoordinates.emplace_back(0.0f, 0.0f); //
    box02_textureCoordinates.emplace_back(0.2f, 0.5f); //
    box02_textureCoordinates.emplace_back(0.2f, 0.0f); //
    box02_textureCoordinates.emplace_back(0.2f, 0.5f); // top
    box02_textureCoordinates.emplace_back(0.2f, 0.0f); //
    box02_textureCoordinates.emplace_back(0.6f, 0.5f); //
    box02_textureCoordinates.emplace_back(0.6f, 0.0f); //
    box02_textureCoordinates.emplace_back(0.6f, 0.5f); // bottom
    box02_textureCoordinates.emplace_back(0.6f, 0.0f); //
    box02_textureCoordinates.emplace_back(1.0f, 0.5f); //
    box02_textureCoordinates.emplace_back(1.0f, 0.0f); //

    std::vector<glm::vec2> box03_textureCoordinates;
    box03_textureCoordinates.reserve(24);
    box03_textureCoordinates.emplace_back(0.0f, 1.0f);  // front
    box03_textureCoordinates.emplace_back(0.0f, 0.5f);  //
    box03_textureCoordinates.emplace_back(0.5f, 1.0f);  //
    box03_textureCoordinates.emplace_back(0.5f, 0.5f);  //
    box03_textureCoordinates.emplace_back(0.5f, 1.0f);  // right
    box03_textureCoordinates.emplace_back(0.5f, 0.5f);  //
    box03_textureCoordinates.emplace_back(0.75f, 1.0f); //
    box03_textureCoordinates.emplace_back(0.75f, 0.5f); //
    box03_textureCoordinates.emplace_back(0.0f, 0.5f);  // back
    box03_textureCoordinates.emplace_back(0.0f, 0.0f);  //
    box03_textureCoordinates.emplace_back(0.5f, 0.5f);  //
    box03_textureCoordinates.emplace_back(0.5f, 0.0f);  //
    box03_textureCoordinates.emplace_back(0.75f, 1.0f); // left
    box03_textureCoordinates.emplace_back(0.75f, 0.5f); //
    box03_textureCoordinates.emplace_back(1.0f, 1.0f);  //
    box03_textureCoordinates.emplace_back(1.0f, 0.5f);  //
    box03_textureCoordinates.emplace_back(0.5f, 0.5f);  // top
    box03_textureCoordinates.emplace_back(0.5f, 0.25f); //
    box03_textureCoordinates.emplace_back(1.0f, 0.5f);  //
    box03_textureCoordinates.emplace_back(1.0f, 0.25f); //
    box03_textureCoordinates.emplace_back(0.5f, 0.25f); // bottom
    box03_textureCoordinates.emplace_back(0.5f, 0.0f);  //
    box03_textureCoordinates.emplace_back(1.0f, 0.25f); //
    box03_textureCoordinates.emplace_back(1.0f, 0.0f);  //

    // Creating Entities
    // -------------------------------------------------------------------------

    // Calculating positions ---------------------------------------------------

    std::vector<glm::vec3> meshEntityPositions;
    for (int j = 2; j >= -3; --j)
    {
        for (int i = -6; i <= 5; ++i)
        {
            meshEntityPositions.emplace_back((float)i + 0.5f, (float)j, 0.0f);
        }
    }

    size_t positionIndex = 0;

    // row 1 ---------------------------------------------------------------- 00
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        const glm::vec3 min(-0.16f, -0.16f, -0.16f);
        const glm::vec3 max(0.16f, 0.16f, 0.16f);
        FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, min, max));

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

    // ---------------------------------------------------------------------- 01
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        const glm::vec3 min(-0.32f, -0.16f, -0.16f);
        const glm::vec3 max(0.32f, 0.16f, 0.16f);
        FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, min, max));

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

    // ---------------------------------------------------------------------- 02
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        const glm::vec3 min(-0.32f, -0.32f, -0.16f);
        const glm::vec3 max(0.32f, 0.32f, 0.16f);
        FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, min, max));

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

    // ---------------------------------------------------------------------- 03
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        const glm::vec3 min(-0.16f, -0.16f, -0.16f);
        const glm::vec3 max(0.16f, 0.16f, 0.16f);
        FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, min, max, false));

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

    // ---------------------------------------------------------------------- 04
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        const glm::vec3 min(-0.32f, -0.16f, -0.16f);
        const glm::vec3 max(0.32f, 0.16f, 0.16f);
        FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, min, max, false));

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

    // ---------------------------------------------------------------------- 05
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        const glm::vec3 min(-0.32f, -0.32f, -0.16f);
        const glm::vec3 max(0.32f, 0.32f, 0.16f);
        FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, min, max, false));

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

    // ---------------------------------------------------------------------- 06
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        const glm::vec3 min(-0.16f, -0.16f, -0.16f);
        const glm::vec3 max(0.16f, 0.16f, 0.16f);
        FAIL_CHECK(Project001::MeshLoader::GenerateBoxWithTexture(*newMeshDataPtr, min, max, box01_textureCoordinates));

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
            renderedMeshPtr->SetTextureId(box01_textureId);
        }
    }

    // ---------------------------------------------------------------------- 07
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        const glm::vec3 min(-0.32f, -0.16f, -0.16f);
        const glm::vec3 max(0.32f, 0.16f, 0.16f);
        FAIL_CHECK(Project001::MeshLoader::GenerateBoxWithTexture(*newMeshDataPtr, min, max, box02_textureCoordinates));

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
            renderedMeshPtr->SetTextureId(box02_textureId);
        }
    }

    // ---------------------------------------------------------------------- 08
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        const glm::vec3 min(-0.32f, -0.32f, -0.16f);
        const glm::vec3 max(0.32f, 0.32f, 0.16f);
        FAIL_CHECK(Project001::MeshLoader::GenerateBoxWithTexture(*newMeshDataPtr, min, max, box03_textureCoordinates));

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
            renderedMeshPtr->SetTextureId(box03_textureId);
        }
    }

    // ---------------------------------------------------------------------- 09
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        const glm::vec3 min(-0.16f, -0.16f, -0.16f);
        const glm::vec3 max(0.16f, 0.16f, 0.16f);
        FAIL_CHECK(Project001::MeshLoader::GenerateBoxWithTexture(*newMeshDataPtr, min, max, box01_textureCoordinates, false));

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
            renderedMeshPtr->SetTextureId(box01_textureId);
        }
    }

    // ---------------------------------------------------------------------- 10
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        const glm::vec3 min(-0.32f, -0.16f, -0.16f);
        const glm::vec3 max(0.32f, 0.16f, 0.16f);
        FAIL_CHECK(Project001::MeshLoader::GenerateBoxWithTexture(*newMeshDataPtr, min, max, box02_textureCoordinates, false));

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
            renderedMeshPtr->SetTextureId(box02_textureId);
        }
    }

    // ---------------------------------------------------------------------- 11
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        const glm::vec3 min(-0.32f, -0.32f, -0.16f);
        const glm::vec3 max(0.32f, 0.32f, 0.16f);
        FAIL_CHECK(Project001::MeshLoader::GenerateBoxWithTexture(*newMeshDataPtr, min, max, box03_textureCoordinates, false));

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
            renderedMeshPtr->SetTextureId(box03_textureId);
        }
    }

    // row 2 ---------------------------------------------------------------- 12
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 13
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 14
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 15
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 16
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 17
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 18
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateSphereSection(*newMeshDataPtr, 0.32f, 4, 4, 0, glm::two_pi<float>(), 0, glm::pi<float>()));

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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 19
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateSphereSection(
            *newMeshDataPtr,
            0.32f,
            4,
            4,
            0,
            glm::two_pi<float>(),
            1.0f / 3.0f * glm::pi<float>(),
            2.0f / 3.0f * glm::pi<float>()
        ));

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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 20
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateSphereSection(
            *newMeshDataPtr,
            0.32f,
            4,
            4,
            0,
            glm::quarter_pi<float>(),
            1.0f / 8.0f * glm::pi<float>(),
            glm::half_pi<float>()
        ));

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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 21
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateSphereSection(*newMeshDataPtr, 0.32f, 4, 4, 0, glm::two_pi<float>(), 0, glm::pi<float>(), false));

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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 22
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateSphereSection(
            *newMeshDataPtr,
            0.32f,
            4,
            4,
            0,
            glm::two_pi<float>(),
            1.0f / 3.0f * glm::pi<float>(),
            2.0f / 3.0f * glm::pi<float>(),
            false
        ));

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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 23
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateSphereSection(
            *newMeshDataPtr,
            0.32f,
            4,
            4,
            0,
            glm::quarter_pi<float>(),
            1.0f / 8.0f * glm::pi<float>(),
            glm::half_pi<float>(),
            false
        ));

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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // row 3 ---------------------------------------------------------------- 24
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 25
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 26
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 27
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 28
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 29
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 30
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 31
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 32
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 33
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 34
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 35
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // row 4 ---------------------------------------------------------------- 36
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 37
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 38
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 39
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 40
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 41
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 42
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateHemisphere(*newMeshDataPtr, 0.32f, 3, 2));

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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 43
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateHemisphere(*newMeshDataPtr, 0.32f, 4, 4));

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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 44
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateHemisphere(*newMeshDataPtr, 0.32f, 8, 8));

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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 45
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateHemisphere(*newMeshDataPtr, 0.32f, 3, 2, false));

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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 46
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateHemisphere(*newMeshDataPtr, 0.32f, 4, 4, false));

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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 47
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateHemisphere(*newMeshDataPtr, 0.32f, 8, 8, false));

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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // row 5 ---------------------------------------------------------------- 48
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 49
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 50
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 51
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 52
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 53
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 54
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 55
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 56
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 57
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 58
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // ---------------------------------------------------------------------- 59
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
            renderedMeshPtr->SetTextureId(numbers12x6TextureId);
        }
    }

    // row 6 ---------------------------------------------------------------- 60
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

    // ---------------------------------------------------------------------- 61
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

    // ---------------------------------------------------------------------- 62
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

    // ---------------------------------------------------------------------- 63
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

    // ---------------------------------------------------------------------- 64
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

    // ---------------------------------------------------------------------- 65
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