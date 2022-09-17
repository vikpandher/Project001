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
    ClearResources();
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
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(0.0f, 0.48f);
        positions.emplace_back(0.0f, 0.64f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        // duplicate position at end
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.16f, 0.0f);
        positions.emplace_back(0.16f, 0.16f);
        positions.emplace_back(0.16f, 0.32f);
        positions.emplace_back(0.16f, 0.64f);
        positions.emplace_back(0.16f, 0.64f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        // duplicate position in middle
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.32f, 0.16f);
        positions.emplace_back(0.32f, 0.32f);
        positions.emplace_back(0.32f, 0.32f);
        positions.emplace_back(0.32f, 0.64f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.32f, -0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.32f, 0.32f); // same slope
        positions.emplace_back(-0.32f, -0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, 0.32f);
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.32f, -0.32f); // same slope
        positions.emplace_back(-0.32f, -0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, -0.32f);
        positions.emplace_back(0.0f, -0.32f);
        positions.emplace_back(0.32f, -0.32f); // same slope
        positions.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, 0.0f);
        positions.emplace_back(0.0f, -0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.32f, -0.32f);
        positions.emplace_back(0.32f, 0.32f);
        positions.emplace_back(-0.32f, -0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(*newMeshDataPtr, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, 0.32f);
        positions.emplace_back(0.32f, -0.32f);
        positions.emplace_back(-0.32f, -0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(*newMeshDataPtr, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, -0.32f);
        positions.emplace_back(0.32f, -0.32f);
        positions.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(*newMeshDataPtr, positions, 0.24f));
        (Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr));
        (Project001::MeshLoader::RecenterMesh(*newMeshDataPtr));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, 0.0f);
        positions.emplace_back(0.0f, -0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(*newMeshDataPtr, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
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
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(*newMeshDataPtr, positions, 0.04f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32, -0.04f);
        positions.emplace_back(0.08, -0.04f);
        positions.emplace_back(-0.08, 0.04f);
        positions.emplace_back(0.32, 0.04f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32, -0.04f);
        positions.emplace_back(0.08, -0.04f);
        positions.emplace_back(-0.08, 0.04f);
        positions.emplace_back(0.32, 0.04f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, positions, 0.12f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.16f, 0.16f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(-0.16f, 0.48f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(-0.16f, 0.48f);
        positions.emplace_back(0.16f, 0.16f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.16f, 0.16f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(*newMeshDataPtr, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(0.32f, 0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.16f, 0.16f);
        positions.emplace_back(0.48f, -0.16f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(*newMeshDataPtr, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.16f, 0.16f);
        positions.emplace_back(0.48f, -0.16f);
        positions.emplace_back(-0.16f, 0.48f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(*newMeshDataPtr, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(0.32f, 0.32f);
        positions.emplace_back(0.32f, 0.0f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(*newMeshDataPtr, positions, 0.08f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);
    }

    Project001::MeshData* meshData01Ptr;
    {
        meshData01Ptr = new Project001::MeshData();
        meshDataPtrArray_.push_back(meshData01Ptr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*meshData01Ptr, 0.48f, 0.64f, 0.0f, 13.0f / 32.0f, 1.0f - 16.0f / 32.0f, 1.0f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        Project001::MeshLoader::CopyMesh(*newMeshDataPtr, *meshData01Ptr);
        Project001::MeshLoader::RotateTextureCoordinates(*newMeshDataPtr, -glm::quarter_pi<float>());
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        Project001::MeshLoader::CopyMesh(*newMeshDataPtr, *meshData01Ptr);
        Project001::MeshLoader::ScaleTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        Project001::MeshLoader::CopyMesh(*newMeshDataPtr, *meshData01Ptr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.06f / 0.32f, -0.08f / 0.32f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*newMeshDataPtr, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*newMeshDataPtr, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*newMeshDataPtr, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*newMeshDataPtr, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*newMeshDataPtr, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*newMeshDataPtr, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*newMeshDataPtr, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*newMeshDataPtr, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*newMeshDataPtr, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));
    }

    // Load textures
    // -------------------------------------------------------------------------

    for (size_t i = 0; i < 35; ++i)
    {
        std::string filePath("../Textures/32x32_");
        if (i / 10 == 0)
        {
            filePath += '0';
        }
        filePath += std::to_string(i);
        filePath += ".png";

        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, filePath));
        unsigned int tempTextureId = (unsigned int)-1;
        _FAIL_CHECK(rendererPtr_->CreateTexture(tempTextureId, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
        _32x23_TextureIds_.push_back(tempTextureId);
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
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
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
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
    }

    // generated shape entity 29
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
    }

    // generated shape entity 30
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
    }

    // generated shape entity 31
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
    }

    // generated shape entity 32
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
    }

    // generated shape entity 33
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
    }

    // generated shape entity 34
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
    }

    // generated shape entity 35
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition]);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[positionPosition]);
        renderedModelPtr->SetTextureId(_32x23_TextureIds_[positionPosition++]);
    }
}

void TestScene004::Deinitialize()
{
    TestSceneBase001::Deinitialize();

    ClearResources();
}

void TestScene004::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene004::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneBase001::OnEvent(event);
}

// protected: ------------------------------------------------------------------

void TestScene004::ClearResources()
{
    _32x23_TextureIds_.clear();
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
            SendEvent(Project001::SwitchSceneEvent("TestScene006"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene006"));
            }
        }
    }
}
