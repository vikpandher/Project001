// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-26

#include "TestScene002.h"

#include "TestApplicationData.h"
#include "TestComponents.h"
#include "TestResource_AntonioRegular_png.h"
#include "TestResource_AntonioRegular_ssf.h"
#include "TestResource_CounterclockwiseDie_png.h"
#include "TestResource_Cube_obj.h"

#include "Components/RenderedMesh.h"
#include "Utilities/FontUtility.h"
#include "Utilities/MathUtility.h"
#include "Utilities/MeshUtility.h"
#include "Utilities/TextureUtility.h"
#include "Application.h"
#include "AutoIdMap.h"
#include "ComponentStores.h"
#include "Logger.h"
#include "Renderer.h"
#include "ResourceStores.h"
#include "UniqueBiMap.h"
#include "Window.h"

#include <algorithm>



// public ----------------------------------------------------------------------

TestScene002::TestScene002(Project001::Application* applicationPtr)
    : TestSceneBase001(applicationPtr)
    , instructionScene_(applicationPtr)
{
    GetSharedDataPtr<TestApplicationData>()->testScene002Id = GetId();

    AutoIdMapTest();
    UniqueBiMapTest();
    ComponentContainerTest();
    ComponentStoresTest();
    ResourceStoresTest();
    ResourceStoresTest2();
    MeshLoaderTest();
    TextureLoaderTest();
}

TestScene002::~TestScene002()
{}

void TestScene002::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene002::ProcessDeinitializeEvent, this, std::placeholders::_1));

    TestSceneBase001::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene002::ProcessInitializeEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene002::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene002:            " << GetId());

    // Texture Data ------------------------------------------------------------

    unsigned int dice01_TextureId = static_cast<unsigned int>(-1);
    unsigned int dice02_TextureId = static_cast<unsigned int>(-1);
    unsigned int dice03_TextureId = static_cast<unsigned int>(-1);
    unsigned int thonk_TextureId = static_cast<unsigned int>(-1);
    unsigned int _100x100_TextureId = static_cast<unsigned int>(-1);
    unsigned int patternSpecular01_TextureId = static_cast<unsigned int>(-1);
    unsigned int patternSpecular02_TextureId = static_cast<unsigned int>(-1);
    unsigned int thonkSpecular_TextureId = static_cast<unsigned int>(-1);
    unsigned int numbers16x4_TextureId = static_cast<unsigned int>(-1);
    unsigned int _0123456789abcdefj_TextureId = static_cast<unsigned int>(-1);

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::Texture::LoadTexture(textureData, "../Textures/CounterclockwiseDie.png"));
        GetRendererPtr()->CreateTexture(dice01_TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::Texture::LoadTextureFromMemory(textureData, g_CounterclockwiseDie_png, sizeof(g_CounterclockwiseDie_png) / sizeof(unsigned char)));
        GetRendererPtr()->CreateTexture(dice02_TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, true, true);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::Texture::LoadTexture(textureData, "../Textures/HallowDie.png"));
        GetRendererPtr()->CreateTexture(dice03_TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::Texture::LoadTexture(textureData, "../Textures/Thonk.png"));
        GetRendererPtr()->CreateTexture(thonk_TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::Texture::LoadTexture(textureData, "../Textures/100x100_2.png"));
        GetRendererPtr()->CreateTexture(_100x100_TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::Texture::LoadTexture(textureData, "../Textures/Specular1.png"));
        GetRendererPtr()->CreateTexture(patternSpecular01_TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::Texture::LoadTexture(textureData, "../Textures/Specular2.png"));
        GetRendererPtr()->CreateTexture(patternSpecular02_TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::Texture::LoadTexture(textureData, "../Textures/ThonkSpecular.png"));
        GetRendererPtr()->CreateTexture(thonkSpecular_TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::Texture::LoadTexture(textureData, "../Textures/16_4_numbers.png"));
        GetRendererPtr()->CreateTexture(numbers16x4_TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::Texture::LoadTexture(textureData, "../Textures/0123456789abcdefghij_long.png"));
        GetRendererPtr()->CreateTexture(_0123456789abcdefj_TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    // Creating Entities and Meshes
    // -------------------------------------------------------------------------

    // Calculating positions ---------------------------------------------------

    std::vector<glm::vec3> meshEntityPositions;
    for (int i = 4; i >= -4; --i)
    {
        for (int j = -4; j <= 5; ++j)
        {
            meshEntityPositions.emplace_back(static_cast<float>(j), static_cast<float>(i), 0.0f);
        }
    }
    size_t positionIndex = 0;

    // row 1 -------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::LoadMeshOBJ(*newMeshDataPtr, "../Models/Cube.obj"));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(thonk_TextureId);
            renderedMeshPtr->SetSpecularId(thonkSpecular_TextureId);
            renderedMeshPtr->SetShininess(32.0f);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::LoadMeshOBJ(*newMeshDataPtr, "../Models/Cube.obj"));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(dice01_TextureId);
            renderedMeshPtr->SetShininess(32.0f);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::LoadMeshOBJ(*newMeshDataPtr, "../Models/Cube.obj"));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(dice02_TextureId);
            renderedMeshPtr->SetShininess(32.0f);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::LoadMeshOBJ(*newMeshDataPtr, "../Models/Cube.obj"));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(dice03_TextureId);
            renderedMeshPtr->SetColorRGB(0.8f, 0.2f, 0.2f);
            renderedMeshPtr->SetShininess(32.0f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::LoadMeshOBJFromMemory(*newMeshDataPtr, g_Cube_obj));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetColor(1.0f, 1.0f, 1.0f, 0.25f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::LoadMeshOBJFromMemory(*newMeshDataPtr, g_Cube_obj));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetColor(1.0f, 1.0f, 1.0f, 0.25f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::LoadMeshOBJFromMemory(*newMeshDataPtr, g_Cube_obj));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetSpecularId(patternSpecular01_TextureId);
            renderedMeshPtr->SetScale(0.5f, 0.75f, 1.0f);
            renderedMeshPtr->SetColorRGB(0.2f, 0.8f, 0.2f);
            renderedMeshPtr->SetShininess(24.0f);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::LoadMeshOBJFromMemory(*newMeshDataPtr, g_Cube_obj));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetSpecularId(patternSpecular02_TextureId);
            renderedMeshPtr->AddPitch(glm::pi<float>() / 4.0f);
            renderedMeshPtr->AddYaw(glm::pi<float>() / 4.0f);
            renderedMeshPtr->SetColorRGB(0.2f, 0.2f, 0.8f);
            renderedMeshPtr->SetShininess(4.0f);
        }
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // row 2 -------------------------------------------------------------------
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
        FAIL_CHECK(Project001::Mesh::Generate2DTriangleFan(*newMeshDataPtr, positions));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::RecenterMesh(*newMeshDataPtr);

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetColorRGB(0.8f, 0.6f, 0.2f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
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
        FAIL_CHECK(Project001::Mesh::Generate2DTriangleStrip(*newMeshDataPtr, positions));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::RecenterMesh(*newMeshDataPtr);

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetColorRGB(0.8f, 0.2f, 0.6f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
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
        FAIL_CHECK(Project001::Mesh::Generate2DTriangles(*newMeshDataPtr, positions));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::RecenterMesh(*newMeshDataPtr);

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetColorRGB(0.6f, 0.2f, 0.8f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, 0.32f);
        positions.emplace_back(-0.32f, 0.08f);
        positions.emplace_back(-0.08f, 0.32f);
        positions.emplace_back(-0.08f, 0.08f);
        positions.emplace_back(0.08f, 0.32f);
        positions.emplace_back(0.08f, 0.08f);
        positions.emplace_back(0.32f, 0.32f);
        positions.emplace_back(0.32f, 0.08f);
        positions.emplace_back(-0.32f, -0.08f);
        positions.emplace_back(-0.32f, -0.32f);
        positions.emplace_back(-0.08f, -0.08f);
        positions.emplace_back(-0.08f, -0.32f);
        positions.emplace_back(0.08f, -0.08f);
        positions.emplace_back(0.08f, -0.32f);
        positions.emplace_back(0.32f, -0.08f);
        positions.emplace_back(0.32f, -0.32f);
        for (size_t i = 0; i < positions.size(); ++i)
        {
            positions[i] += glm::vec2(0.5f, 0.5f);
        }
        FAIL_CHECK(Project001::Mesh::Generate2DQuads(*newMeshDataPtr, positions));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::RecenterMesh(*newMeshDataPtr);

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetColorRGB(0.6f, 0.2f, 0.8f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
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
        FAIL_CHECK(Project001::Mesh::Generate2DLine(*newMeshDataPtr, positions, 0.08f));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::RecenterMesh(*newMeshDataPtr);

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetColorRGB(0.2f, 0.6f, 0.8f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
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
        FAIL_CHECK(Project001::Mesh::Generate2DLine(*newMeshDataPtr, positions, 0.04f));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::RecenterMesh(*newMeshDataPtr);

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetColorRGB(0.2f, 0.8f, 0.6f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
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
        FAIL_CHECK(Project001::Mesh::Generate2DLine(*newMeshDataPtr, positions, 0.12f));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::RecenterMesh(*newMeshDataPtr);

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetColorRGB(0.6f, 0.8f, 0.2f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
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
        FAIL_CHECK(Project001::Mesh::Generate2DLine(*newMeshDataPtr, positions, 0.04f));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::RecenterMesh(*newMeshDataPtr);

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetColorRGB(1.0f, 1.0f, 1.0f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DLine(*newMeshDataPtr, glm::vec2(-0.32f, -0.32f), glm::vec2(0.32f, 0.32f), 0.12f));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetColorRGB(1.0f, 1.0f, 1.0f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DLine(*newMeshDataPtr, glm::vec2(0.0f, 0.32f), glm::vec2(0.32f, 0.0f), 0.12f));
        FAIL_CHECK(Project001::Mesh::Generate2DLine(*newMeshDataPtr, glm::vec2(-0.32f, 0.32f), glm::vec2(0.32f, -0.32f), 0.12f));
        FAIL_CHECK(Project001::Mesh::Generate2DLine(*newMeshDataPtr, glm::vec2(-0.32f, 0.0f), glm::vec2(0.0f, -0.32f), 0.12f));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetColorRGB(1.0f, 1.0f, 1.0f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // row 3 -------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 3));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetColorRGB(1.0f, 0.0f, 0.0f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 4));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetColorRGB(0.0f, 1.0f, 0.0f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 5));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetColorRGB(0.0f, 0.0f, 1.0f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 6));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetColorRGB(0.0f, 1.0f, 1.0f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 7));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetColorRGB(1.0f, 0.0f, 1.0f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 8));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetColorRGB(1.0f, 1.0f, 0.0f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 24));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
        Project001::Mesh::ScaleMesh(*newMeshDataPtr, glm::vec3(2.0f, 1.5f, 1.0f));
        Project001::Mesh::RotateMesh(*newMeshDataPtr, glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() / -4.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
        Project001::Mesh::RotateMesh(*newMeshDataPtr, glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() / -4.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
        Project001::Mesh::RotateMesh(*newMeshDataPtr, glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() / -4.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
        Project001::Mesh::TranslateMesh(*newMeshDataPtr, glm::vec3(0.16f, 0.0f, 0.0f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // row 4 -------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DArc(*newMeshDataPtr, 0.08f, 0.32f, 1, 0.0f, 0.5f * glm::pi<float>()));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DArc(*newMeshDataPtr, 0.08f, 0.32f, 2, 0.0f, 1.5f * glm::pi<float>()));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DArc(*newMeshDataPtr, 0.08f, 0.32f, 3, 0.0f, 2.0f * glm::pi<float>()));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DArc(*newMeshDataPtr, 0.08f, 0.32f, 4, 0.0f, 2.0f * glm::pi<float>()));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DArc(*newMeshDataPtr, 0.08f, 0.32f, 4, 1.5f * glm::pi<float>(), 1.0f * glm::pi<float>()));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DArc(*newMeshDataPtr, 0.08f, 0.32f, 24, 1.25f * glm::pi<float>(), 0.75f * glm::pi<float>()));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DArc(*newMeshDataPtr, 0.16f, 0.32f, 32, 0.0f, 2.0f * glm::pi<float>()));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // row 5 -------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DArc(*newMeshDataPtr, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.16f), 0.48f, 1, 0.16f));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DArc(*newMeshDataPtr, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.16f), 0.48f, 2, 0.16f));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DArc(*newMeshDataPtr, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.16f), 0.48f, 3, 0.16f));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DArc(*newMeshDataPtr, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.16f), 0.48f, 6, 0.16f));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        Project001::MeshData meshDataA;
        FAIL_CHECK(Project001::Mesh::Generate2DArc(meshDataA, 0.08f, 0.32f, 2, 0.0f, glm::pi<float>()));
        Project001::Mesh::ApplyPositionalTextureCoordinates(meshDataA);
        Project001::Mesh::TranslateTextureCoordinates(meshDataA, glm::vec2(0.5f, 0.5f));
        Project001::Mesh::TranslateMesh(meshDataA, glm::vec3(-0.016f, 0.0f, 0.0f));
        Project001::MeshData meshDataB;
        FAIL_CHECK(Project001::Mesh::Generate2DArc(meshDataB, 0.08f, 0.32f, 6, glm::pi<float>(), 0.0f));
        Project001::Mesh::ApplyPositionalTextureCoordinates(meshDataB);
        Project001::Mesh::TranslateTextureCoordinates(meshDataB, glm::vec2(0.5f, 0.5f));
        Project001::Mesh::TranslateMesh(meshDataB, glm::vec3(0.016f, 0.0f, 0.0f));
        Project001::Mesh::CopyMesh(*newMeshDataPtr, meshDataA);
        Project001::Mesh::CopyMesh(*newMeshDataPtr, meshDataB);

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DRectangleFrame(*newMeshDataPtr, glm::vec2(-0.24f, -0.32f), glm::vec2(0.24f, 0.32f), 0.16f));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DRectangleFrame(*newMeshDataPtr, glm::vec2(-0.32f, -0.24f), glm::vec2(0.32f, 0.24f), 0.08f));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // row 6 -------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DCapsule(*newMeshDataPtr, 0.32f, 0.32f, 2));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DCapsule(*newMeshDataPtr, 0.32f, 0.32f, 4));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DCapsule(*newMeshDataPtr, 0.32f, 0.32f, 8));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DCapsule(*newMeshDataPtr, glm::vec2(0.16f, 0.16f), glm::vec2(-0.16f, -0.16f), 0.12f, 2));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DCapsule(*newMeshDataPtr, glm::vec2(-0.16f, 0.16f), glm::vec2(0.16f, -0.16f), 0.12f, 3));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DCapsule(*newMeshDataPtr, glm::vec2(0.08f, 0.08f), glm::vec2(0.08f, 0.08f), 0.12f, 4));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // row 7 -------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DBezeledRectangle(*newMeshDataPtr, 0.64f, 0.64f, 0.16f, 1));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DBezeledRectangle(*newMeshDataPtr, 0.64f, 0.64f, 0.16f, 2));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DBezeledRectangle(*newMeshDataPtr, 0.64f, 0.64f, 0.16f, 3));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DBezeledRectangle(*newMeshDataPtr, 0.64f, 0.48f, 0.16f, 4));
        Project001::Mesh::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::Mesh::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_100x100_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }


    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::LoadMeshOBJ(*newMeshDataPtr, "../Models/Ship.obj"));
        Project001::Mesh::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(numbers16x4_TextureId);
            renderedMeshPtr->SetShininess(32.0f);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::LoadMeshOBJ(*newMeshDataPtr, "../Models/Tank_Head.obj"));
        Project001::Mesh::TranslateMesh(*newMeshDataPtr, glm::vec3(0.0f, 0.5f, 0.0f));
        FAIL_CHECK(Project001::Mesh::LoadMeshOBJ(*newMeshDataPtr, "../Models/Tank_Base.obj"));
        Project001::Mesh::ScaleMesh(*newMeshDataPtr, glm::vec3(0.48f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(numbers16x4_TextureId);
            renderedMeshPtr->SetShininess(32.0f);
        }
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // row 7 -------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DDashedLine(
            *newMeshDataPtr,
            glm::vec2(0.0f, 0.0f),
            Project001::Math::Rotate2DVector(glm::vec2(1.0f, 0.0f), glm::quarter_pi<float>()),
            0.08f, 0.04f, 0.08f, 1
        ));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_0123456789abcdefj_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DDashedLine(
            *newMeshDataPtr,
            glm::vec2(0.0f, 0.0f),
            Project001::Math::Rotate2DVector(glm::vec2(1.0f, 0.0f), glm::quarter_pi<float>()),
            0.08f, 0.04f, 0.08f, 2
        ));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_0123456789abcdefj_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DDashedLine(
            *newMeshDataPtr,
            glm::vec2(0.0f, 0.0f),
            Project001::Math::Rotate2DVector(glm::vec2(1.0f, 0.0f), glm::quarter_pi<float>()),
            0.08f, 0.04f, 0.08f, 3
        ));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_0123456789abcdefj_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DDashedLine(
            *newMeshDataPtr,
            glm::vec2(0.0f, 0.0f),
            Project001::Math::Rotate2DVector(glm::vec2(1.0f, 0.0f), glm::quarter_pi<float>()),
            0.08f, 0.04f, 0.08f, 4
        ));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_0123456789abcdefj_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DDashedLine(
            *newMeshDataPtr,
            glm::vec2(0.0f, 0.0f),
            Project001::Math::Rotate2DVector(glm::vec2(1.0f, 0.0f), glm::quarter_pi<float>()),
            0.08f, 0.04f, 0.08f, 5
        ));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_0123456789abcdefj_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }


    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DDashedLine(
            *newMeshDataPtr,
            glm::vec2(0.0f, 0.0f),
            Project001::Math::Rotate2DVector(glm::vec2(1.0f, 0.0f), glm::quarter_pi<float>()),
            0.08f, 0.04f, 0.08f, 6
        ));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_0123456789abcdefj_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // row 8 -------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DDashedLine(
            *newMeshDataPtr,
            glm::vec2(0.0f, 0.0f),
            Project001::Math::Rotate2DVector(glm::vec2(1.0f, 0.0f), glm::quarter_pi<float>()),
            0.08f, 0.04f, 0.08f, 1, true
        ));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_0123456789abcdefj_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DDashedLine(
            *newMeshDataPtr,
            glm::vec2(0.0f, 0.0f),
            Project001::Math::Rotate2DVector(glm::vec2(1.0f, 0.0f), glm::quarter_pi<float>()),
            0.08f, 0.04f, 0.08f, 2, true
        ));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_0123456789abcdefj_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DDashedLine(
            *newMeshDataPtr,
            glm::vec2(0.0f, 0.0f),
            Project001::Math::Rotate2DVector(glm::vec2(1.0f, 0.0f), glm::quarter_pi<float>()),
            0.08f, 0.04f, 0.08f, 3, true
        ));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_0123456789abcdefj_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DDashedLine(
            *newMeshDataPtr,
            glm::vec2(0.0f, 0.0f),
            Project001::Math::Rotate2DVector(glm::vec2(1.0f, 0.0f), glm::quarter_pi<float>()),
            0.08f, 0.04f, 0.08f, 4, true
        ));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_0123456789abcdefj_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DDashedLine(
            *newMeshDataPtr,
            glm::vec2(0.0f, 0.0f),
            Project001::Math::Rotate2DVector(glm::vec2(1.0f, 0.0f), glm::quarter_pi<float>()),
            0.08f, 0.04f, 0.08f, 5, true
        ));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_0123456789abcdefj_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }


    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::Mesh::Generate2DDashedLine(
            *newMeshDataPtr,
            glm::vec2(0.0f, 0.0f),
            Project001::Math::Rotate2DVector(glm::vec2(1.0f, 0.0f), glm::quarter_pi<float>()),
            0.08f, 0.04f, 0.08f, 6, true
        ));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(_0123456789abcdefj_TextureId);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // -------------------------------------------------------------------------
    {
        positionIndex++;
    }

    // Member Scenes -----------------------------------------------------------

    Project001::FontData font01_FontData;
    FAIL_CHECK(Project001::Font::LoadFontDataFromMemory(
        font01_FontData,
        g_AntonioRegular_ssf,
        sizeof(g_AntonioRegular_ssf)
    ));

    Project001::TextureData font01_TextureData;
    FAIL_CHECK(Project001::Texture::LoadTextureFromMemory(
        font01_TextureData,
        g_AntonioRegular_png,
        sizeof(g_AntonioRegular_png)
    ));
    unsigned int font01_TextureId = static_cast<unsigned int>(-1);
    GetRendererPtr()->CreateTexture(
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
        "This Scene tests Mesh Generation.\n"
        "Use <WASD> to move the camera up, left, down, and right.\n"
        "Use <Q> to roll left and <E> to roll right.\n"
        "Use <Scroll> to move forward and back.\n"
        "<Left-Click> and drag the <Mouse> to move camera.\n"
        "Press <Esc> to return to Main Menu.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = &font01_FontData;
    instructionSceneInfo.fontTextureId = font01_TextureId;
    instructionSceneInfo.cameraEntityId = uiCameraEntityId_;
    instructionSceneInfo.cameraMask = s_uiCameraMask_;
    instructionSceneInfo.keyCode_toggleInstructions = keyCode_toggleInstructions;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene002::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene002:            " << GetId());
}

// private ---------------------------------------------------------------------

void TestScene002::AutoIdMapTest() const
{
    Project001::AutoIdMap<std::string> testAutoIdMap;

    unsigned int id00 = static_cast<unsigned int>(-1);
    unsigned int id01 = static_cast<unsigned int>(-1);
    unsigned int id02 = static_cast<unsigned int>(-1);
    unsigned int id03 = static_cast<unsigned int>(-1);
    unsigned int id04 = static_cast<unsigned int>(-1);
    unsigned int id05 = static_cast<unsigned int>(-1);
    unsigned int id06 = static_cast<unsigned int>(-1);
    unsigned int id07 = static_cast<unsigned int>(-1);

    Project001::AutoIdMap<std::string>::iterator iter00 = testAutoIdMap.Add(id00, "value00");
    Project001::AutoIdMap<std::string>::iterator iter01 = testAutoIdMap.Add(id01, "value01");
    Project001::AutoIdMap<std::string>::iterator iter02 = testAutoIdMap.Add(id02, "value02");
    Project001::AutoIdMap<std::string>::iterator iter03 = testAutoIdMap.Add(id03, "value03");
    Project001::AutoIdMap<std::string>::iterator iter04 = testAutoIdMap.Add(id04, "value04");
    Project001::AutoIdMap<std::string>::iterator iter05 = testAutoIdMap.Add(id05, "value05");
    Project001::AutoIdMap<std::string>::iterator iter06 = testAutoIdMap.Add(id06, "value06");
    Project001::AutoIdMap<std::string>::iterator iter07 = testAutoIdMap.Add(id07, "value07");

    Project001::AutoIdMap<std::string>::iterator iterA = testAutoIdMap.IteratorAtBeginning();
    Project001::AutoIdMap<std::string>::iterator iterB = testAutoIdMap.IteratorPastTheEnd();

    iter00 = testAutoIdMap.Find(static_cast<unsigned int>(-1)); // fail
    iter01 = testAutoIdMap.Find(1000); // fail
    iter02 = testAutoIdMap.Find(2000); // fail
    iter03 = testAutoIdMap.Find(3000); // fail

    iter03 = testAutoIdMap.Find(id03);
    iter02 = testAutoIdMap.Find(id02);
    iter01 = testAutoIdMap.Find(id01);
    iter00 = testAutoIdMap.Find(id00);

    iter07 = testAutoIdMap.Erase(iter07);
    iter06 = testAutoIdMap.Erase(iter06);
    iter05 = testAutoIdMap.Erase(iter05);
    iter04 = testAutoIdMap.Erase(iter04);
    iter03 = testAutoIdMap.Erase(iter03);
    iter02 = testAutoIdMap.Erase(iter02);
    iter01 = testAutoIdMap.Erase(iter01);
    iter00 = testAutoIdMap.Erase(iter00);

    iter00 = testAutoIdMap.Add(id00, "value08");
    iter01 = testAutoIdMap.Add(id01, "value09");
    iter02 = testAutoIdMap.Add(id02, "value10");
    iter03 = testAutoIdMap.Add(id03, "value11");

    testAutoIdMap.Clear();

    iter04 = testAutoIdMap.Add(id04, "value12");
    iter05 = testAutoIdMap.Add(id05, "value13");
    iter06 = testAutoIdMap.Add(id06, "value14");
    iter07 = testAutoIdMap.Add(id07, "value15");

    iter04 = testAutoIdMap.Erase(iter04);
    iter05 = testAutoIdMap.Erase(iter05);
    iter06 = testAutoIdMap.Erase(iter06);
    iter07 = testAutoIdMap.Erase(iter07);

    // iter00 = testAutoIdMap.Erase(iter00); // error
    // iter01 = testAutoIdMap.Erase(iter01); // error
    // iter02 = testAutoIdMap.Erase(iter02); // error
    // iter03 = testAutoIdMap.Erase(iter03); // error

    iter00 = testAutoIdMap.Add(id00, "value12");
    iter01 = testAutoIdMap.Add(id01, "value13");
    iter02 = testAutoIdMap.Add(id02, "value14");
    iter03 = testAutoIdMap.Add(id03, "value15");

    iter01 = testAutoIdMap.Erase(iter01);
    iter02 = testAutoIdMap.Erase(iter02);

    iter02 = testAutoIdMap.Add(id02, "value14-0");
    iter01 = testAutoIdMap.Add(id01, "value13-0");

    iter03 = testAutoIdMap.Erase(id03);
    iter02 = testAutoIdMap.Erase(id02);
    iter01 = testAutoIdMap.Erase(id01);
    iter00 = testAutoIdMap.Erase(id00);

    testAutoIdMap.Clear();
}

void TestScene002::UniqueBiMapTest() const
{
    Project001::UniqueBiMap<unsigned int, std::string> testUniqueBiMap;

    Project001::UniqueBiMap<unsigned int, std::string>::iterator iter = testUniqueBiMap.Add(0, "ZERO");
    iter = testUniqueBiMap.Add(1, "ONE");
    iter = testUniqueBiMap.Add(2, "TWO");
    iter = testUniqueBiMap.Add(3, "THREE");
    // iter = testUniqueBiMap.Add(4, "FOUR");

    size_t size = testUniqueBiMap.Size();

    Project001::UniqueBiMap<unsigned int, std::string>::iterator iter00 = testUniqueBiMap.FindKey(0);
    Project001::UniqueBiMap<unsigned int, std::string>::iterator iter01 = testUniqueBiMap.FindValue("ZERO");

    Project001::UniqueBiMap<unsigned int, std::string>::iterator iter10 = testUniqueBiMap.FindKey(1);
    Project001::UniqueBiMap<unsigned int, std::string>::iterator iter11 = testUniqueBiMap.FindValue("ONE");

    Project001::UniqueBiMap<unsigned int, std::string>::iterator iter20 = testUniqueBiMap.FindKey(2);
    Project001::UniqueBiMap<unsigned int, std::string>::iterator iter21 = testUniqueBiMap.FindValue("TWO");

    Project001::UniqueBiMap<unsigned int, std::string>::iterator iter30 = testUniqueBiMap.FindKey(3);
    Project001::UniqueBiMap<unsigned int, std::string>::iterator iter31 = testUniqueBiMap.FindValue("THREE");

    Project001::UniqueBiMap<unsigned int, std::string>::iterator iter40 = testUniqueBiMap.FindKey(4);
    Project001::UniqueBiMap<unsigned int, std::string>::iterator iter41 = testUniqueBiMap.FindValue("FOUR");

    Project001::UniqueBiMap<unsigned int, std::string>::iterator iterA = testUniqueBiMap.IteratorAtBeginning();
    Project001::UniqueBiMap<unsigned int, std::string>::iterator iterB = testUniqueBiMap.IteratorPastTheEnd();

    Project001::UniqueBiMap<unsigned int, std::string>::iterator iterC = testUniqueBiMap.EraseKey(0);
    Project001::UniqueBiMap<unsigned int, std::string>::iterator iterD = testUniqueBiMap.EraseValue("ZERO");

    Project001::UniqueBiMap<unsigned int, std::string>::iterator iterE = testUniqueBiMap.EraseValue("ONE");
    Project001::UniqueBiMap<unsigned int, std::string>::iterator iterF = testUniqueBiMap.EraseKey(1);

    iter = testUniqueBiMap.Add(2, "TWENTY");
    iter = testUniqueBiMap.Add(30, "THREE");

    iter = testUniqueBiMap.Add(20, "TWENTY");
    iter = testUniqueBiMap.Add(30, "THIRTY");

    Project001::UniqueBiMap<unsigned int, std::string>::iterator iterG = testUniqueBiMap.FindKey(20);
    Project001::UniqueBiMap<unsigned int, std::string>::iterator iterH = testUniqueBiMap.Erase(iterG);

    testUniqueBiMap.Clear();
}

void TestScene002::ComponentContainerTest() const
{
    size_t sizeSize = sizeof(size_t);
    size_t unsignedIntSize = sizeof(unsigned int);
    size_t sizeOf_testComponent00 = sizeof(TestComponent00);
    size_t sizeOf_testComponent01 = sizeof(TestComponent01);
    size_t componentContainerSize = sizeof(Project001::ComponentContainer);

    Project001::ComponentContainer componentContainer00;

    TestComponent00* testComponent00ptr = nullptr;
    TestComponent00* testComponent01ptr = nullptr;
    TestComponent00* testComponent02ptr = nullptr;
    TestComponent00* testComponent03ptr = nullptr;
    TestComponent00* testComponent04ptr = nullptr;
    TestComponent00* testComponent05ptr = nullptr;
    TestComponent00* testComponent06ptr = nullptr;
    TestComponent00* testComponent07ptr = nullptr;
    TestComponent00* testComponent08ptr = nullptr;

    unsigned int entityId00 = static_cast<unsigned int>(-1);
    unsigned int entityId01 = static_cast<unsigned int>(-1);
    unsigned int entityId02 = static_cast<unsigned int>(-1);
    unsigned int entityId03 = static_cast<unsigned int>(-1);
    unsigned int entityId04 = static_cast<unsigned int>(-1);
    unsigned int entityId05 = static_cast<unsigned int>(-1);
    unsigned int entityId06 = static_cast<unsigned int>(-1);
    unsigned int entityId07 = static_cast<unsigned int>(-1);
    unsigned int entityId08 = static_cast<unsigned int>(-1);

    TestComponent00* allTestComponents = nullptr;
    size_t testComponentCount = static_cast<size_t>(-1);

    const unsigned int* allTestComponentEntityIds = nullptr;
    size_t testComponentEntityIdCount = static_cast<size_t>(-1);

    bool functionSuccess = true;

    functionSuccess = componentContainer00.Initialize<TestComponent00>(2, 2, 7);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(0, 1, 2, 3);
    functionSuccess = componentContainer00.GetComponent(testComponent00ptr, 0);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId00, testComponent00ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(1, 2, 3, 4);
    functionSuccess = componentContainer00.GetComponent(testComponent01ptr, 1);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId01, testComponent01ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(2, 3, 4, 5);
    functionSuccess = componentContainer00.GetComponent(testComponent02ptr, 2);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId02, testComponent02ptr);

    functionSuccess = componentContainer00.GetComponent(testComponent00ptr, 0);
    functionSuccess = componentContainer00.GetComponent(testComponent01ptr, 1);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(3, 4, 5, 6);
    functionSuccess = componentContainer00.GetComponent(testComponent03ptr, 3);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId03, testComponent03ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(4, 5, 6, 7);
    functionSuccess = componentContainer00.GetComponent(testComponent04ptr, 4);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId04, testComponent04ptr);

    functionSuccess = componentContainer00.GetComponent(testComponent00ptr, 0);
    functionSuccess = componentContainer00.GetComponent(testComponent01ptr, 1);
    functionSuccess = componentContainer00.GetComponent(testComponent02ptr, 2);
    functionSuccess = componentContainer00.GetComponent(testComponent03ptr, 3);

    functionSuccess = componentContainer00.DeleteComponent(1);
    functionSuccess = componentContainer00.DeleteComponent(3);
    functionSuccess = componentContainer00.GetComponent(testComponent01ptr, 1);
    functionSuccess = componentContainer00.GetComponent(testComponent03ptr, 3);
    functionSuccess = componentContainer00.GetComponent(testComponent04ptr, 4);

    functionSuccess = componentContainer00.GetComponentEntityId(entityId00, testComponent00ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId01, testComponent01ptr); //
    functionSuccess = componentContainer00.GetComponentEntityId(entityId02, testComponent02ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId03, testComponent03ptr); //
    functionSuccess = componentContainer00.GetComponentEntityId(entityId04, testComponent04ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(5, 6, 7, 8);
    functionSuccess = componentContainer00.GetComponent(testComponent05ptr, 5);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId05, testComponent05ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(6, 7, 8, 9);
    functionSuccess = componentContainer00.GetComponent(testComponent06ptr, 6);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId06, testComponent06ptr);

    functionSuccess = componentContainer00.GetAllComponents(allTestComponents, testComponentCount);
    functionSuccess = componentContainer00.GetAllComponentEntityIds<TestComponent00>(allTestComponentEntityIds, testComponentEntityIdCount);

    functionSuccess = componentContainer00.DeleteAllComponents();

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(0, 1, 1, 1);
    functionSuccess = componentContainer00.GetComponent(testComponent00ptr, 0);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId00, testComponent00ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(1, 2, 2, 2);
    functionSuccess = componentContainer00.GetComponent(testComponent01ptr, 1);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId01, testComponent01ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(2, 3, 3, 3);
    functionSuccess = componentContainer00.GetComponent(testComponent02ptr, 2);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId02, testComponent02ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(3, 4, 4, 4);
    functionSuccess = componentContainer00.GetComponent(testComponent03ptr, 3);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId03, testComponent03ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(4, 5, 5, 5);
    functionSuccess = componentContainer00.GetComponent(testComponent04ptr, 4);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId04, testComponent04ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(5, 6, 6, 6);
    functionSuccess = componentContainer00.GetComponent(testComponent05ptr, 5);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId05, testComponent05ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(6, 7, 7, 7);
    functionSuccess = componentContainer00.GetComponent(testComponent06ptr, 6);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId06, testComponent06ptr);

    functionSuccess = componentContainer00.DeleteComponent(0);
    functionSuccess = componentContainer00.DeleteComponent(1);
    functionSuccess = componentContainer00.DeleteComponent(2);
    functionSuccess = componentContainer00.DeleteComponent(3);
    functionSuccess = componentContainer00.DeleteComponent(4);
    functionSuccess = componentContainer00.DeleteComponent(5);
    functionSuccess = componentContainer00.DeleteComponent(6);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(8, 88, 888, 8888);
    functionSuccess = componentContainer00.GetComponent(testComponent08ptr, 8);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId08, testComponent08ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(7, 77, 777, 7777);
    functionSuccess = componentContainer00.GetComponent(testComponent07ptr, 7);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId07, testComponent07ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(6, 66, 666, 6666);
    functionSuccess = componentContainer00.GetComponent(testComponent06ptr, 6);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId06, testComponent06ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(5, 55, 555, 5555);
    functionSuccess = componentContainer00.GetComponent(testComponent05ptr, 5);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId05, testComponent05ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(4, 44, 444, 4444);
    functionSuccess = componentContainer00.GetComponent(testComponent04ptr, 4);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId04, testComponent04ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(3, 33, 333, 3333);
    functionSuccess = componentContainer00.GetComponent(testComponent03ptr, 3);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId03, testComponent03ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(2, 22, 222, 2222);
    functionSuccess = componentContainer00.GetComponent(testComponent02ptr, 2);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId02, testComponent02ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(1, 11, 111, 1111);
    functionSuccess = componentContainer00.GetComponent(testComponent01ptr, 1);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId01, testComponent01ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(0, -1, -2, -3);
    functionSuccess = componentContainer00.GetComponent(testComponent00ptr, 0);
    functionSuccess = componentContainer00.GetComponentEntityId(entityId00, testComponent00ptr);

    functionSuccess = componentContainer00.GetAllComponents(allTestComponents, testComponentCount);
    functionSuccess = componentContainer00.GetAllComponentEntityIds<TestComponent00>(allTestComponentEntityIds, testComponentEntityIdCount);

    std::vector<TestComponent00*> allTestComponentPtrs;
    for (size_t i = 0; i < testComponentCount; ++i)
    {
        allTestComponentPtrs.emplace_back(&allTestComponents[i]);
    }
    std::sort(allTestComponentPtrs.begin(), allTestComponentPtrs.end(),
        [](TestComponent00* a, TestComponent00* b)->bool
        {
            return a->GetSum() < b->GetSum();
        });

    functionSuccess = componentContainer00.DeleteComponent(6);
    functionSuccess = componentContainer00.DeleteComponent(5);
    functionSuccess = componentContainer00.DeleteComponent(4);
    functionSuccess = componentContainer00.DeleteComponent(3);
    functionSuccess = componentContainer00.DeleteComponent(2);
    functionSuccess = componentContainer00.DeleteComponent(1);
    functionSuccess = componentContainer00.DeleteComponent(0);

    functionSuccess = componentContainer00.Initialize<TestComponent01>(1, 3, 8);
}

void TestScene002::ComponentStoresTest() const
{
    Project001::ComponentStores testComponentStores;

    unsigned int entityId01 = static_cast<unsigned int>(-1);
    unsigned int entityId02 = static_cast<unsigned int>(-1);
    unsigned int entityId03 = static_cast<unsigned int>(-1);
    unsigned int entityId04 = static_cast<unsigned int>(-1);
    unsigned int entityId05 = static_cast<unsigned int>(-1);
    unsigned int entityId06 = static_cast<unsigned int>(-1);

    unsigned int entityId01_returned = static_cast<unsigned int>(-1);
    unsigned int entityId02_returned = static_cast<unsigned int>(-1);
    unsigned int entityId03_returned = static_cast<unsigned int>(-1);
    unsigned int entityId04_returned = static_cast<unsigned int>(-1);
    unsigned int entityId05_returned = static_cast<unsigned int>(-1);
    unsigned int entityId06_returned = static_cast<unsigned int>(-1);

    TestComponent00* testComponent0001ptr = nullptr;
    TestComponent00* testComponent0002ptr = nullptr;
    TestComponent00* testComponent0003ptr = nullptr;
    TestComponent00* testComponent0004ptr = nullptr;
    TestComponent00* testComponent0005ptr = nullptr;
    TestComponent00* testComponent0006ptr = nullptr;

    TestComponent00* allTestComponent00s = nullptr;
    size_t testComponent00Count = static_cast<size_t>(-1);

    const unsigned int* allTestComponent00EntityIds = nullptr;
    size_t testComponent00EntityIdCount = static_cast<size_t>(-1);

    TestComponent01* testComponent0101ptr = nullptr;
    TestComponent01* testComponent0102ptr = nullptr;
    TestComponent01* testComponent0103ptr = nullptr;
    TestComponent01* testComponent0104ptr = nullptr;
    TestComponent01* testComponent0105ptr = nullptr;
    TestComponent01* testComponent0106ptr = nullptr;

    TestComponent02* testComponent0201ptr = nullptr;
    TestComponent02* testComponent0202ptr = nullptr;
    TestComponent02* testComponent0203ptr = nullptr;

    bool functionSuccess = true;

    testComponentStores.CreateEntity(entityId01);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId01, 1, 11, 111);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0001ptr, entityId01);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(entityId01_returned, testComponent0001ptr);

    testComponentStores.CreateEntity(entityId02);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId02, 2, 22, 222);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0002ptr, entityId02);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(entityId02_returned, testComponent0002ptr);

    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0001ptr, entityId01);

    testComponentStores.CreateEntity(entityId03);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId03, 3, 33, 333);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0003ptr, entityId03);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(entityId03_returned, testComponent0003ptr);

    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0001ptr, entityId01);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0002ptr, entityId02);

    testComponentStores.CreateEntity(entityId04);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId04, 4, 44, 444);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0004ptr, entityId04);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(entityId04_returned, testComponent0004ptr);

    testComponentStores.CreateEntity(entityId05);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId05, 5, 55, 555);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0005ptr, entityId05);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(entityId05_returned, testComponent0005ptr);

    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0001ptr, entityId01);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0002ptr, entityId02);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0003ptr, entityId03);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0004ptr, entityId04);

    testComponentStores.CreateEntity(entityId06);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId06, 6, 66, 666);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0006ptr, entityId06);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(entityId06_returned, testComponent0006ptr);

    functionSuccess = testComponentStores.GetAllComponents(allTestComponent00s, testComponent00Count);
    functionSuccess = testComponentStores.GetAllComponentEntityIds<TestComponent00>(allTestComponent00EntityIds, testComponent00EntityIdCount);

    functionSuccess = testComponentStores.InitializeComponentContainer<TestComponent01>(2, 2, 5);
    functionSuccess = testComponentStores.CreateComponent<TestComponent01>(entityId06, 6.0f, 66.0f, 666.0f);
    functionSuccess = testComponentStores.GetComponent<TestComponent01>(testComponent0106ptr, entityId06);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent01>(entityId06_returned, testComponent0106ptr);

    functionSuccess = testComponentStores.CreateComponent<TestComponent01>(entityId05, 5.0f, 55.0f, 555.0f);
    functionSuccess = testComponentStores.GetComponent<TestComponent01>(testComponent0105ptr, entityId05);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent01>(entityId05_returned, testComponent0105ptr);

    functionSuccess = testComponentStores.CreateComponent<TestComponent01>(entityId04, 4.0f, 44.0f, 444.0f);
    functionSuccess = testComponentStores.GetComponent<TestComponent01>(testComponent0104ptr, entityId04);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent01>(entityId04_returned, testComponent0104ptr);

    functionSuccess = testComponentStores.CreateComponent<TestComponent01>(entityId03, 3.0f, 33.0f, 333.0f);
    functionSuccess = testComponentStores.GetComponent<TestComponent01>(testComponent0103ptr, entityId03);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent01>(entityId03_returned, testComponent0103ptr);

    functionSuccess = testComponentStores.CreateComponent<TestComponent01>(entityId02, 2.0f, 22.0f, 222.0f);
    functionSuccess = testComponentStores.GetComponent<TestComponent01>(testComponent0102ptr, entityId02);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent01>(entityId02_returned, testComponent0102ptr);

    functionSuccess = testComponentStores.CreateComponent<TestComponent01>(entityId01, 1.0f, 11.0f, 111.0f);
    functionSuccess = testComponentStores.GetComponent<TestComponent01>(testComponent0101ptr, entityId01);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent01>(entityId01_returned, testComponent0101ptr);

    functionSuccess = testComponentStores.GetComponent<TestComponent01>(testComponent0106ptr, entityId06);
    functionSuccess = testComponentStores.GetComponent<TestComponent01>(testComponent0105ptr, entityId05);
    functionSuccess = testComponentStores.GetComponent<TestComponent01>(testComponent0104ptr, entityId04);
    functionSuccess = testComponentStores.GetComponent<TestComponent01>(testComponent0103ptr, entityId03);

    functionSuccess = testComponentStores.DeleteComponent<TestComponent00>(entityId01);
    functionSuccess = testComponentStores.DeleteComponent<TestComponent00>(entityId02);

    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId01, 1, 1, 1);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId02, 2, 2, 2);

    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0001ptr, entityId01);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0002ptr, entityId02);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0005ptr, entityId05);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0006ptr, entityId06);

    functionSuccess = testComponentStores.CreateComponent<TestComponent02>(entityId01, 1, 11, 111);
    functionSuccess = testComponentStores.GetComponent<TestComponent02>(testComponent0201ptr, entityId01);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent02>(entityId01_returned, testComponent0201ptr);

    functionSuccess = testComponentStores.CreateComponent<TestComponent02>(entityId02, 2, 22, 222);
    functionSuccess = testComponentStores.GetComponent<TestComponent02>(testComponent0202ptr, entityId02);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent02>(entityId02_returned, testComponent0202ptr);

    functionSuccess = testComponentStores.CreateComponent<TestComponent02>(entityId03, 3, 33, 333);
    functionSuccess = testComponentStores.GetComponent<TestComponent02>(testComponent0203ptr, entityId03);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent02>(entityId03_returned, testComponent0203ptr);

    functionSuccess = testComponentStores.DeleteAllComponents<TestComponent02>();

    functionSuccess = testComponentStores.CreateComponent<TestComponent02>(entityId03, 3, 3, 3);
    functionSuccess = testComponentStores.GetComponent<TestComponent02>(testComponent0203ptr, entityId03);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent02>(entityId03_returned, testComponent0203ptr);

    functionSuccess = testComponentStores.CreateComponent<TestComponent02>(entityId02, 2, 2, 2);
    functionSuccess = testComponentStores.GetComponent<TestComponent02>(testComponent0202ptr, entityId02);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent02>(entityId02_returned, testComponent0202ptr);

    functionSuccess = testComponentStores.CreateComponent<TestComponent02>(entityId01, 1, 1, 1);
    functionSuccess = testComponentStores.GetComponent<TestComponent02>(testComponent0201ptr, entityId01);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent02>(entityId01_returned, testComponent0201ptr);

    functionSuccess = testComponentStores.DeleteEntity(entityId04);
    functionSuccess = testComponentStores.DeleteEntity(entityId01);

    testComponentStores.DeleteAllEntities();

    testComponentStores.CreateEntity(entityId01);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId01, -1, -1, -1);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0001ptr, entityId01);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(entityId01_returned, testComponent0001ptr);

    testComponentStores.CreateEntity(entityId02);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId02, -2, -2, -2);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0002ptr, entityId02);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(entityId02_returned, testComponent0002ptr);

    testComponentStores.CreateEntity(entityId03);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId03, -3, -3, -3);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0003ptr, entityId03);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(entityId03_returned, testComponent0003ptr);

    testComponentStores.CreateEntity(entityId04);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId04, -4, -4, -4);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0004ptr, entityId04);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(entityId04_returned, testComponent0004ptr);

    testComponentStores.DeleteAllEntitiesAndResetCapacities();

    functionSuccess = testComponentStores.InitializeComponentContainer<TestComponent00>(2, 2, 5);

    testComponentStores.CreateEntity(entityId01);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId01, -1, -1, -1);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0001ptr, entityId01);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(entityId01_returned, testComponent0001ptr);

    testComponentStores.CreateEntity(entityId02);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId02, -2, -2, -2);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0002ptr, entityId02);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(entityId02_returned, testComponent0002ptr);

    testComponentStores.CreateEntity(entityId03);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId03, -3, -3, -3);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0003ptr, entityId03);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(entityId03_returned, testComponent0003ptr);

    testComponentStores.CreateEntity(entityId04);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId04, -4, -4, -4);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0004ptr, entityId04);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(entityId04_returned, testComponent0004ptr);

    testComponentStores.CreateEntity(entityId05);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId05, -5, -5, -5);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0005ptr, entityId05);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(entityId05_returned, testComponent0005ptr);

    testComponentStores.CreateEntity(entityId06); // entity created, but not components
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId06, -6, -6, -6);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(testComponent0006ptr, entityId06);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(entityId06_returned, testComponent0006ptr);

    functionSuccess = testComponentStores.DeleteEntity(entityId06);
}

void TestScene002::ResourceStoresTest() const
{
    Project001::ResourceStores testResourceStores;

    // TestComponent00

    unsigned int testComponent00_Id01 = static_cast<unsigned int>(-1);
    unsigned int testComponent00_Id02 = static_cast<unsigned int>(-1);
    unsigned int testComponent00_Id03 = static_cast<unsigned int>(-1);
    unsigned int testComponent00_Id04 = static_cast<unsigned int>(-1);
    unsigned int testComponent00_Id05 = static_cast<unsigned int>(-1);
    unsigned int testComponent00_Id06 = static_cast<unsigned int>(-1);
    unsigned int testComponent00_Id07 = static_cast<unsigned int>(-1);
    unsigned int testComponent00_Id08 = static_cast<unsigned int>(-1);

    unsigned int returned_testComponent00_Id01 = static_cast<unsigned int>(-1);
    unsigned int returned_testComponent00_Id02 = static_cast<unsigned int>(-1);
    unsigned int returned_testComponent00_Id03 = static_cast<unsigned int>(-1);

    const unsigned int* returned_testComponent00_Ids = nullptr;
    size_t returned_testComponent00_Id_count = 0;

    TestComponent00* testComponent00_ptr01 = nullptr;
    TestComponent00* testComponent00_ptr02 = nullptr;
    TestComponent00* testComponent00_ptr03 = nullptr;

    TestComponent00* testComponent00_ptrs = nullptr;
    size_t testComponent00_count = 0;

    // TestComponent01

    unsigned int testComponent01_Id01 = static_cast<unsigned int>(-1);
    unsigned int testComponent01_Id02 = static_cast<unsigned int>(-1);
    unsigned int testComponent01_Id03 = static_cast<unsigned int>(-1);

    unsigned int returned_testComponent01_Id01 = static_cast<unsigned int>(-1);
    unsigned int returned_testComponent01_Id02 = static_cast<unsigned int>(-1);
    unsigned int returned_testComponent01_Id03 = static_cast<unsigned int>(-1);

    const unsigned int* returned_testComponent01_Ids = nullptr;
    size_t returned_testComponent01_Id_count = 0;

    TestComponent01* testComponent01_ptr01 = nullptr;
    TestComponent01* testComponent01_ptr02 = nullptr;
    TestComponent01* testComponent01_ptr03 = nullptr;

    TestComponent01* testComponent01_ptrs = nullptr;
    size_t testComponent01_count = 0;

    // TestComponent02

    unsigned int testComponent02_Id01 = static_cast<unsigned int>(-1);
    unsigned int testComponent02_Id02 = static_cast<unsigned int>(-1);
    unsigned int testComponent02_Id03 = static_cast<unsigned int>(-1);

    unsigned int returned_testComponent02_Id01 = static_cast<unsigned int>(-1);
    unsigned int returned_testComponent02_Id02 = static_cast<unsigned int>(-1);
    unsigned int returned_testComponent02_Id03 = static_cast<unsigned int>(-1);

    const unsigned int* returned_testComponent02_Ids = nullptr;
    size_t returned_testComponent02_Id_count = 0;

    TestComponent02* testComponent02_ptr01 = nullptr;
    TestComponent02* testComponent02_ptr02 = nullptr;
    TestComponent02* testComponent02_ptr03 = nullptr;

    TestComponent02* testComponent02_ptrs = nullptr;
    size_t testComponent02_count = 0;

    // --------

    bool functionSuccess = true;

    functionSuccess = testResourceStores.DeleteResource<TestComponent00>(testComponent00_Id01);
    functionSuccess = testResourceStores.DeleteAllResources<TestComponent00>();
    functionSuccess = testResourceStores.GetResource<TestComponent00>(testComponent00_ptr01, testComponent00_Id01);
    functionSuccess = testResourceStores.GetAllResources<TestComponent00>(testComponent00_ptrs, testComponent00_count);
    functionSuccess = testResourceStores.GetResourceId<TestComponent00>(returned_testComponent00_Id01, testComponent00_ptr01);

    functionSuccess = testResourceStores.Initialize<TestComponent00>(2, 2, 7);

    functionSuccess = testResourceStores.CreateResource<TestComponent00>(testComponent00_Id01, 1, 10, 100);
    functionSuccess = testResourceStores.CreateResource<TestComponent00>(testComponent00_Id02, 2, 20, 200);
    functionSuccess = testResourceStores.CreateResource<TestComponent00>(testComponent00_Id03, 3, 30, 300);
    functionSuccess = testResourceStores.CreateResource<TestComponent00>(testComponent00_Id04, 4, 40, 400);
    functionSuccess = testResourceStores.CreateResource<TestComponent00>(testComponent00_Id05, 5, 50, 500);
    functionSuccess = testResourceStores.CreateResource<TestComponent00>(testComponent00_Id06, 6, 60, 600);
    functionSuccess = testResourceStores.CreateResource<TestComponent00>(testComponent00_Id07, 7, 70, 700);
    functionSuccess = testResourceStores.CreateResource<TestComponent00>(testComponent00_Id08, 8, 80, 800); // fails

    functionSuccess = testResourceStores.GetResource<TestComponent00>(testComponent00_ptr01, testComponent00_Id01);
    functionSuccess = testResourceStores.GetResource<TestComponent00>(testComponent00_ptr02, testComponent00_Id02);
    functionSuccess = testResourceStores.GetResource<TestComponent00>(testComponent00_ptr03, testComponent00_Id03);

    functionSuccess = testResourceStores.GetAllResources<TestComponent00>(testComponent00_ptrs, testComponent00_count);

    functionSuccess = testResourceStores.GetResourceId<TestComponent00>(returned_testComponent00_Id01, testComponent00_ptr01);
    functionSuccess = testResourceStores.GetResourceId<TestComponent00>(returned_testComponent00_Id02, testComponent00_ptr02);
    functionSuccess = testResourceStores.GetResourceId<TestComponent00>(returned_testComponent00_Id03, testComponent00_ptr03);

    functionSuccess = testResourceStores.GetAllResourceIds<TestComponent00>(returned_testComponent00_Ids, returned_testComponent00_Id_count);

    functionSuccess = testResourceStores.DeleteResource<TestComponent00>(testComponent00_Id02);
    functionSuccess = testResourceStores.DeleteResource<TestComponent00>(testComponent00_Id04);
    functionSuccess = testResourceStores.DeleteResource<TestComponent00>(testComponent00_Id06);
    functionSuccess = testResourceStores.CreateResource<TestComponent00>(testComponent00_Id06, 6, 66, 666);
    functionSuccess = testResourceStores.CreateResource<TestComponent00>(testComponent00_Id02, 4, 44, 444);
    functionSuccess = testResourceStores.CreateResource<TestComponent00>(testComponent00_Id02, 2, 22, 222);

    functionSuccess = testResourceStores.DeleteResource<TestComponent00>(testComponent00_Id01);

    functionSuccess = testResourceStores.GetResource<TestComponent00>(testComponent00_ptr01, testComponent00_Id01);
    functionSuccess = testResourceStores.GetResourceId<TestComponent00>(returned_testComponent00_Id01, testComponent00_ptr01); // success, because old pointer points to other component

    functionSuccess = testResourceStores.DeleteAllResources<TestComponent00>();

    // ----

    functionSuccess = testResourceStores.CreateResource<TestComponent00>(testComponent00_Id01, -1, -10, -100);
    functionSuccess = testResourceStores.CreateResource<TestComponent00>(testComponent00_Id02, -2, -20, -200);
    functionSuccess = testResourceStores.CreateResource<TestComponent00>(testComponent00_Id03, -3, -30, -300);

    functionSuccess = testResourceStores.GetResource<TestComponent00>(testComponent00_ptr01, testComponent00_Id01);
    functionSuccess = testResourceStores.GetResource<TestComponent00>(testComponent00_ptr02, testComponent00_Id02);
    functionSuccess = testResourceStores.GetResource<TestComponent00>(testComponent00_ptr03, testComponent00_Id03);

    functionSuccess = testResourceStores.GetAllResources<TestComponent00>(testComponent00_ptrs, testComponent00_count);

    functionSuccess = testResourceStores.GetResourceId<TestComponent00>(returned_testComponent00_Id01, testComponent00_ptr01);
    functionSuccess = testResourceStores.GetResourceId<TestComponent00>(returned_testComponent00_Id02, testComponent00_ptr02);
    functionSuccess = testResourceStores.GetResourceId<TestComponent00>(returned_testComponent00_Id03, testComponent00_ptr03);

    functionSuccess = testResourceStores.GetAllResourceIds<TestComponent00>(returned_testComponent00_Ids, returned_testComponent00_Id_count);

    // ----

    functionSuccess = testResourceStores.CreateResource<TestComponent01>(testComponent01_Id01, -1.0f, -0.1f, -0.01f);
    functionSuccess = testResourceStores.CreateResource<TestComponent01>(testComponent01_Id02, -2.0f, -0.2f, -0.02f);
    functionSuccess = testResourceStores.CreateResource<TestComponent01>(testComponent01_Id03, -3.0f, -0.3f, -0.03f);

    functionSuccess = testResourceStores.GetResource<TestComponent01>(testComponent01_ptr01, testComponent01_Id01);
    functionSuccess = testResourceStores.GetResource<TestComponent01>(testComponent01_ptr02, testComponent01_Id02);
    functionSuccess = testResourceStores.GetResource<TestComponent01>(testComponent01_ptr03, testComponent01_Id03);

    functionSuccess = testResourceStores.GetAllResources<TestComponent01>(testComponent01_ptrs, testComponent01_count);

    functionSuccess = testResourceStores.GetResourceId<TestComponent01>(returned_testComponent01_Id01, testComponent01_ptr01);
    functionSuccess = testResourceStores.GetResourceId<TestComponent01>(returned_testComponent01_Id02, testComponent01_ptr02);
    functionSuccess = testResourceStores.GetResourceId<TestComponent01>(returned_testComponent01_Id03, testComponent01_ptr03);

    functionSuccess = testResourceStores.GetAllResourceIds<TestComponent01>(returned_testComponent01_Ids, returned_testComponent01_Id_count);

    // ----

    functionSuccess = testResourceStores.CreateResource<TestComponent02>(testComponent02_Id01, 1, 11, 111);
    functionSuccess = testResourceStores.CreateResource<TestComponent02>(testComponent02_Id02, 2, 22, 222);
    functionSuccess = testResourceStores.CreateResource<TestComponent02>(testComponent02_Id03, 3, 33, 333);

    functionSuccess = testResourceStores.GetResource<TestComponent02>(testComponent02_ptr01, testComponent02_Id01);
    functionSuccess = testResourceStores.GetResource<TestComponent02>(testComponent02_ptr02, testComponent02_Id02);
    functionSuccess = testResourceStores.GetResource<TestComponent02>(testComponent02_ptr03, testComponent02_Id03);

    functionSuccess = testResourceStores.GetAllResources<TestComponent02>(testComponent02_ptrs, testComponent02_count);

    functionSuccess = testResourceStores.GetResourceId<TestComponent02>(returned_testComponent02_Id01, testComponent02_ptr01);
    functionSuccess = testResourceStores.GetResourceId<TestComponent02>(returned_testComponent02_Id02, testComponent02_ptr02);
    functionSuccess = testResourceStores.GetResourceId<TestComponent02>(returned_testComponent02_Id03, testComponent02_ptr03);

    functionSuccess = testResourceStores.GetAllResourceIds<TestComponent02>(returned_testComponent02_Ids, returned_testComponent02_Id_count);

    testResourceStores.DeleteAllResources();
    testResourceStores.DeleteAllResourcesAndResetCapacities();

    functionSuccess = testResourceStores.CreateResource<TestComponent02>(testComponent02_Id01, 1, 11, 111);
    functionSuccess = testResourceStores.CreateResource<TestComponent02>(testComponent02_Id02, 2, 22, 222);
    functionSuccess = testResourceStores.CreateResource<TestComponent02>(testComponent02_Id03, 3, 33, 333);
}

void TestScene002::ResourceStoresTest2() const
{
    Project001::ResourceStores* testResourceStoresPtr = new Project001::ResourceStores;

    // FontUtils Data
    {
        unsigned int font01_FontDataId = static_cast<unsigned int>(-1);
        unsigned int font02_FontDataId = static_cast<unsigned int>(-1);
        unsigned int font03_FontDataId = static_cast<unsigned int>(-1);
        unsigned int font04_FontDataId = static_cast<unsigned int>(-1);

        Project001::FontData* font01_FontDataPtr = nullptr;
        Project001::FontData* font02_FontDataPtr = nullptr;
        Project001::FontData* font03_FontDataPtr = nullptr;
        Project001::FontData* font04_FontDataPtr = nullptr;

        bool success = false;

        success = testResourceStoresPtr->CreateResource<Project001::FontData>(font01_FontDataId); // true
        success = testResourceStoresPtr->CreateResource<Project001::FontData>(font02_FontDataId); // true
        success = testResourceStoresPtr->CreateResource<Project001::FontData>(font03_FontDataId); // true

        success = testResourceStoresPtr->GetResource<Project001::FontData>(font02_FontDataPtr, font02_FontDataId); // true
        success = testResourceStoresPtr->GetResource<Project001::FontData>(font04_FontDataPtr, font04_FontDataId); // false

        success = testResourceStoresPtr->DeleteResource<Project001::FontData>(font02_FontDataId); // true
        success = testResourceStoresPtr->DeleteResource<Project001::FontData>(font04_FontDataId); // false

        success = testResourceStoresPtr->DeleteAllResources<Project001::FontData>(); // true
        success = testResourceStoresPtr->DeleteAllResources<Project001::FontData>(); // true
        testResourceStoresPtr->DeleteAllResourcesAndResetCapacities();
        success = testResourceStoresPtr->DeleteAllResources<Project001::FontData>(); // false
    }

    delete testResourceStoresPtr;
}

void TestScene002::MeshLoaderTest() const
{
    bool testBool = false;

    Project001::MeshData meshData01;
    testBool = Project001::Mesh::LoadMeshOBJ(meshData01, "");
    testBool = Project001::Mesh::LoadMeshOBJ(meshData01, "../Models/CubeQ.obj");
}

void TestScene002::TextureLoaderTest() const
{
    bool testBool = false;

    Project001::TextureData textureData01;
    testBool = Project001::Texture::LoadTexture(textureData01, "");
    testBool = Project001::Texture::LoadTexture(textureData01, "../Textures/rgb.png");

    Project001::TextureData textureData02;
    testBool = Project001::Texture::LoadTexture(textureData02, "../Textures/Cube.png");

    Project001::TextureData textureData03;
    testBool = Project001::Texture::LoadTexture(textureData03, "../Textures/CubeSpecular.png");
}