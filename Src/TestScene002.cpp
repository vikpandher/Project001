#include "TestScene002.h"

#include "TestComponents.h"

#include "Engine/Components/RenderedMesh.h"
#include "Engine/Application.h"
#include "Engine/BiMap.h"
#include "Engine/ComponentStores.h"
#include "Engine/FreetypeTextLoader.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/Renderer.h"
#include "Engine/TextureLoader.h"
#include "Engine/Window.h"

#include <algorithm>



// public ----------------------------------------------------------------------

TestScene002::TestScene002(Project001::Application* applicationPtr)
    : TestSceneBase001(applicationPtr, "TestScene002")
    , instructionScene_(applicationPtr, "TestInstructionScene001_002")
{
    BiMapTest();
    ComponentContainerTest();
    ComponentStoresTest();
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
    // Mesh Data ---------------------------------------------------------------

    { // 0
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::LoadMeshOBJ(*newMeshDataPtr, "../Models/Cube.obj"));
    }

    { // 1
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

    { // 2
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

    { // 3
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

    { // 4
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

    { // 5
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

    { // 6
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

    { // 7
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

    { // 8
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 3));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 9
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 4));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 10
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 5));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 11
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 6));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 12
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 7));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 13
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 8));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 14
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

    { // 15
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*newMeshDataPtr, 0.08f, 0.32f, 1, 0.0f, 0.5f * glm::pi<float>()));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 16
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*newMeshDataPtr, 0.08f, 0.32f, 2, 0.0f, 1.5f * glm::pi<float>()));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 17
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*newMeshDataPtr, 0.08f, 0.32f, 3, 0.0f, 2.0f * glm::pi<float>()));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 18
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*newMeshDataPtr, 0.08f, 0.32f, 4, 0.0f, 2.0f * glm::pi<float>()));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 19
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*newMeshDataPtr, 0.08f, 0.32f, 4, 1.5f * glm::pi<float>(), 1.0f * glm::pi<float>()));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 20
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*newMeshDataPtr, 0.08f, 0.32f, 24, 1.25f * glm::pi<float>(), 0.75f * glm::pi<float>()));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 21
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*newMeshDataPtr, 0.16f, 0.32f, 32, 0.0f, 2.0f * glm::pi<float>()));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 22
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(*newMeshDataPtr, 0.32f, 0.32f, 2));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 23
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(*newMeshDataPtr, 0.32f, 0.32f, 4));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 24
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(*newMeshDataPtr, 0.32f, 0.32f, 8));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 25
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DBezeledRectangle(*newMeshDataPtr, 0.64f, 0.64f, 0.16f, 1));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 26
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DBezeledRectangle(*newMeshDataPtr, 0.64f, 0.64f, 0.16f, 2));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 27
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DBezeledRectangle(*newMeshDataPtr, 0.64f, 0.64f, 0.16f, 3));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 28
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DBezeledRectangle(*newMeshDataPtr, 0.64f, 0.48f, 0.16f, 4));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 29
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*newMeshDataPtr, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.16f), 0.48f, 1, 0.16f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 30
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*newMeshDataPtr, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.16f), 0.48f, 2, 0.16f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 31
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*newMeshDataPtr, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.16f), 0.48f, 3, 0.16f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    { // 32
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*newMeshDataPtr, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.16f), 0.48f, 6, 0.16f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    // Texture Data ------------------------------------------------------------

    unsigned int dice01TextureId = (unsigned int)-1;
    unsigned int dice02TextureId = (unsigned int)-1;
    unsigned int thonkTextureId = (unsigned int)-1;
    unsigned int _100x100TextureId = (unsigned int)-1;
    unsigned int patternSpecular01TextureId = (unsigned int)-1;
    unsigned int patternSpecular02TextureId = (unsigned int)-1;
    unsigned int thonkSpecularTextureId = (unsigned int)-1;

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/CounterclockwiseDie.png"));
        rendererPtr_->CreateTexture(dice01TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/HallowDie.png"));
        rendererPtr_->CreateTexture(dice02TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/Thonk.png"));
        rendererPtr_->CreateTexture(thonkTextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/100x100_2.png"));
        rendererPtr_->CreateTexture(_100x100TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/Specular1.png"));
        rendererPtr_->CreateTexture(patternSpecular01TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/Specular2.png"));
        rendererPtr_->CreateTexture(patternSpecular02TextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/ThonkSpecular.png"));
        rendererPtr_->CreateTexture(thonkSpecularTextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
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

    // Cube Entity 01
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));

        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[0]);
        renderedMeshPtr->SetTextureId(thonkTextureId);
        renderedMeshPtr->SetSpecularId(thonkSpecularTextureId);
        renderedMeshPtr->SetShininess(32.0f);
    }

    // Cube Entity 02
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));

        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[0]);
        renderedMeshPtr->SetTextureId(dice01TextureId);
        renderedMeshPtr->SetShininess(32.0f);
    }

    // Cube Entity 03
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));

        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[0]);
        renderedMeshPtr->SetTextureId(dice02TextureId);
        renderedMeshPtr->SetColorRGB(0.8f, 0.2f, 0.2f);
        renderedMeshPtr->SetShininess(32.0f);
    }

    // Cube Entity 04
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));

        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[0]);
        renderedMeshPtr->SetColor(1.0f, 1.0f, 1.0f, 0.25f);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Cube Entity 05
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));

        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[0]);
        renderedMeshPtr->SetColor(1.0f, 1.0f, 1.0f, 0.25f);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Cube Entity 06
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));

        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[0]);
        renderedMeshPtr->SetSpecularId(patternSpecular01TextureId);
        renderedMeshPtr->SetScale(0.5f, 0.75f, 1.0f);
        renderedMeshPtr->SetColorRGB(0.2f, 0.8f, 0.2f);
        renderedMeshPtr->SetShininess(24.0f);
    }

    // Cube Entity 07
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));

        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[0]);
        renderedMeshPtr->SetSpecularId(patternSpecular02TextureId);
        renderedMeshPtr->AddPitch(glm::pi<float>() / 4.0f);
        renderedMeshPtr->AddYaw(glm::pi<float>() / 4.0f);
        renderedMeshPtr->SetColorRGB(0.2f, 0.2f, 0.8f);
        renderedMeshPtr->SetShininess(4.0f);
    }

    // Generated Shape Entity 01
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));

        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[1]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetColorRGB(0.8f, 0.6f, 0.2f);
        renderedMeshPtr->SetTranslucent(true);
    }
 
    // Generated Shape Entity 02
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));

        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[2]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetColorRGB(0.8f, 0.2f, 0.6f);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 03
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[3]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetColorRGB(0.6f, 0.2f, 0.8f);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 04
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[4]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetColorRGB(0.2f, 0.6f, 0.8f);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 05
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[5]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetColorRGB(0.2f, 0.8f, 0.6f);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 06
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[6]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetColorRGB(0.6f, 0.8f, 0.2f);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 07
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[7]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetColorRGB(1.0f, 1.0f, 1.0f);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 08
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[8]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetColorRGB(1.0f, 0.0f, 0.0f);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 09
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[9]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetColorRGB(0.0f, 1.0f, 0.0f);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 10
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[10]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetColorRGB(0.0f, 0.0f, 1.0f);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 11
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[11]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetColorRGB(0.0f, 1.0f, 1.0f);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 12
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[12]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetColorRGB(1.0f, 0.0f, 1.0f);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 13
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[13]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetColorRGB(1.0f, 1.0f, 0.0f);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 14
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[14]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 15
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[15]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 16
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[16]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 17
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[17]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 18
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[18]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 19
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[19]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 20
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[20]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 21
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[21]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 22
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[22]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 23
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[23]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 24
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[24]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 25
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[25]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 26
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[26]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 27
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[27]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 28
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[28]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 29
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[29]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 30
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[30]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 31
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[31]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Generated Shape Entity 32
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[positionPosition++]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[32]);
        renderedMeshPtr->SetTextureId(_100x100TextureId);
        renderedMeshPtr->SetTranslucent(true);
    }

    // Member Scenes -----------------------------------------------------------

    Project001::FontData font01_FontData;
    Project001::TextureData font01_TextureData;
    unsigned int font01_TextureId = (unsigned int)-1;
    std::vector<unsigned char> characterList;
    for (unsigned char c = 32; c < 127; ++c) // ASCII characters
    {
        characterList.push_back(c);
    }
    _FAIL_CHECK(Project001::FreetypeTextLoader::LoadTexture(
        font01_TextureData,
        font01_FontData,
        characterList,
        "../Fonts/Antonio-Regular.ttf",
        48
    ));
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
        "This Scene tests Mesh Generation.\n"
        "Use <WASD> to move the camera up, left, down, and right.\n"
        "Use <Q> to roll left and <E> to roll right.\n"
        "Use <Scroll> to move forward and back.\n"
        "<Left-Click> and drag the <Mouse> to move camera.\n"
        "Press <ESC> to return to Main Menu.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = &font01_FontData;
    instructionSceneInfo.fontTextureIdPtr = &font01_TextureId;
    instructionSceneInfo.cameraEntityIdPtr = &uiCameraEntityId_;
    instructionSceneInfo.cameraMaskPtr = &s_uiCameraMask_;
    instructionSceneInfo.keyCode_toggleInstructionsPtr = &keyCode_toggleInstructions;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene002::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    // _LOG_MESSAGE("DEINITIALIZING: %s", GetName().c_str());

    // -------------------------------------------------------------------------

    instructionScene_.Deinitialize();
}

// private ---------------------------------------------------------------------

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

    unsigned int entityId00 = (unsigned int)-1;
    unsigned int entityId01 = (unsigned int)-1;
    unsigned int entityId02 = (unsigned int)-1;
    unsigned int entityId03 = (unsigned int)-1;
    unsigned int entityId04 = (unsigned int)-1;
    unsigned int entityId05 = (unsigned int)-1;
    unsigned int entityId06 = (unsigned int)-1;
    unsigned int entityId07 = (unsigned int)-1;
    unsigned int entityId08 = (unsigned int)-1;

    TestComponent00* allTestComponents = nullptr;
    size_t testComponentCount = (size_t)-1;

    const unsigned int* allTestComponentEntityIds = nullptr;
    size_t testComponentEntityIdCount = (size_t)-1;

    bool functionSuccess = true;

    functionSuccess = componentContainer00.Initialize<TestComponent00>(2, 2, 7);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(0, 1, 2, 3);
    functionSuccess = componentContainer00.GetComponent(0, testComponent00ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent00ptr, entityId00);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(1, 2, 3, 4);
    functionSuccess = componentContainer00.GetComponent(1, testComponent01ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent01ptr, entityId01);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(2, 3, 4, 5);
    functionSuccess = componentContainer00.GetComponent(2, testComponent02ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent02ptr, entityId02);

    functionSuccess = componentContainer00.GetComponent(0, testComponent00ptr);
    functionSuccess = componentContainer00.GetComponent(1, testComponent01ptr);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(3, 4, 5, 6);
    functionSuccess = componentContainer00.GetComponent(3, testComponent03ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent03ptr, entityId03);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(4, 5, 6, 7);
    functionSuccess = componentContainer00.GetComponent(4, testComponent04ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent04ptr, entityId04);

    functionSuccess = componentContainer00.GetComponent(0, testComponent00ptr);
    functionSuccess = componentContainer00.GetComponent(1, testComponent01ptr);
    functionSuccess = componentContainer00.GetComponent(2, testComponent02ptr);
    functionSuccess = componentContainer00.GetComponent(3, testComponent03ptr);

    functionSuccess = componentContainer00.DeleteComponent(1);
    functionSuccess = componentContainer00.DeleteComponent(3);
    functionSuccess = componentContainer00.GetComponent(1, testComponent01ptr);
    functionSuccess = componentContainer00.GetComponent(3, testComponent03ptr);
    functionSuccess = componentContainer00.GetComponent(4, testComponent04ptr);

    functionSuccess = componentContainer00.GetComponentEntityId(testComponent00ptr, entityId00);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent01ptr, entityId01); //
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent02ptr, entityId02);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent03ptr, entityId03); //
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent04ptr, entityId04);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(5, 6, 7, 8);
    functionSuccess = componentContainer00.GetComponent(5, testComponent05ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent05ptr, entityId05);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(6, 7, 8, 9);
    functionSuccess = componentContainer00.GetComponent(6, testComponent06ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent06ptr, entityId06);

    functionSuccess = componentContainer00.GetAllComponents(allTestComponents, testComponentCount);
    functionSuccess = componentContainer00.GetAllComponentEntityIds<TestComponent00>(allTestComponentEntityIds, testComponentEntityIdCount);

    functionSuccess = componentContainer00.DeleteAllComponents();

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(0, 1, 1, 1);
    functionSuccess = componentContainer00.GetComponent(0, testComponent00ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent00ptr, entityId00);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(1, 2, 2, 2);
    functionSuccess = componentContainer00.GetComponent(1, testComponent01ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent01ptr, entityId01);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(2, 3, 3, 3);
    functionSuccess = componentContainer00.GetComponent(2, testComponent02ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent02ptr, entityId02);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(3, 4, 4, 4);
    functionSuccess = componentContainer00.GetComponent(3, testComponent03ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent03ptr, entityId03);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(4, 5, 5, 5);
    functionSuccess = componentContainer00.GetComponent(4, testComponent04ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent04ptr, entityId04);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(5, 6, 6, 6);
    functionSuccess = componentContainer00.GetComponent(5, testComponent05ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent05ptr, entityId05);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(6, 7, 7, 7);
    functionSuccess = componentContainer00.GetComponent(6, testComponent06ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent06ptr, entityId06);

    functionSuccess = componentContainer00.DeleteComponent(0);
    functionSuccess = componentContainer00.DeleteComponent(1);
    functionSuccess = componentContainer00.DeleteComponent(2);
    functionSuccess = componentContainer00.DeleteComponent(3);
    functionSuccess = componentContainer00.DeleteComponent(4);
    functionSuccess = componentContainer00.DeleteComponent(5);
    functionSuccess = componentContainer00.DeleteComponent(6);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(8, 88, 888, 8888);
    functionSuccess = componentContainer00.GetComponent(8, testComponent08ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent08ptr, entityId08);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(7, 77, 777, 7777);
    functionSuccess = componentContainer00.GetComponent(7, testComponent07ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent07ptr, entityId07);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(6, 66, 666, 6666);
    functionSuccess = componentContainer00.GetComponent(6, testComponent06ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent06ptr, entityId06);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(5, 55, 555, 5555);
    functionSuccess = componentContainer00.GetComponent(5, testComponent05ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent05ptr, entityId05);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(4, 44, 444, 4444);
    functionSuccess = componentContainer00.GetComponent(4, testComponent04ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent04ptr, entityId04);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(3, 33, 333, 3333);
    functionSuccess = componentContainer00.GetComponent(3, testComponent03ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent03ptr, entityId03);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(2, 22, 222, 2222);
    functionSuccess = componentContainer00.GetComponent(2, testComponent02ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent02ptr, entityId02);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(1, 11, 111, 1111);
    functionSuccess = componentContainer00.GetComponent(1, testComponent01ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent01ptr, entityId01);

    functionSuccess = componentContainer00.CreateComponent<TestComponent00>(0, -1, -2, -3);
    functionSuccess = componentContainer00.GetComponent(0, testComponent00ptr);
    functionSuccess = componentContainer00.GetComponentEntityId(testComponent00ptr, entityId00);

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

    unsigned int entityId01 = (unsigned int)-1;
    unsigned int entityId02 = (unsigned int)-1;
    unsigned int entityId03 = (unsigned int)-1;
    unsigned int entityId04 = (unsigned int)-1;
    unsigned int entityId05 = (unsigned int)-1;
    unsigned int entityId06 = (unsigned int)-1;

    unsigned int entityId01_returned = (unsigned int)-1;
    unsigned int entityId02_returned = (unsigned int)-1;
    unsigned int entityId03_returned = (unsigned int)-1;
    unsigned int entityId04_returned = (unsigned int)-1;
    unsigned int entityId05_returned = (unsigned int)-1;
    unsigned int entityId06_returned = (unsigned int)-1;

    TestComponent00* testComponent0001ptr = nullptr;
    TestComponent00* testComponent0002ptr = nullptr;
    TestComponent00* testComponent0003ptr = nullptr;
    TestComponent00* testComponent0004ptr = nullptr;
    TestComponent00* testComponent0005ptr = nullptr;
    TestComponent00* testComponent0006ptr = nullptr;

    TestComponent00* allTestComponent00s = nullptr;
    size_t testComponent00Count = (size_t)-1;

    const unsigned int* allTestComponent00EntityIds = nullptr;
    size_t testComponent00EntityIdCount = (size_t)-1;

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

    functionSuccess = testComponentStores.CreateEntity(entityId01);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId01, 1, 11, 111);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId01, testComponent0001ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(testComponent0001ptr, entityId01_returned);

    functionSuccess = testComponentStores.CreateEntity(entityId02);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId02, 2, 22, 222);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId02, testComponent0002ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(testComponent0002ptr, entityId02_returned);

    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId01, testComponent0001ptr);

    functionSuccess = testComponentStores.CreateEntity(entityId03);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId03, 3, 33, 333);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId03, testComponent0003ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(testComponent0003ptr, entityId03_returned);

    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId01, testComponent0001ptr);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId02, testComponent0002ptr);

    functionSuccess = testComponentStores.CreateEntity(entityId04);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId04, 4, 44, 444);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId04, testComponent0004ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(testComponent0004ptr, entityId04_returned);

    functionSuccess = testComponentStores.CreateEntity(entityId05);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId05, 5, 55, 555);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId05, testComponent0005ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(testComponent0005ptr, entityId05_returned);

    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId01, testComponent0001ptr);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId02, testComponent0002ptr);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId03, testComponent0003ptr);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId04, testComponent0004ptr);

    functionSuccess = testComponentStores.CreateEntity(entityId06);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId06, 6, 66, 666);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId06, testComponent0006ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(testComponent0006ptr, entityId06_returned);

    functionSuccess = testComponentStores.GetAllComponents(allTestComponent00s, testComponent00Count);
    functionSuccess = testComponentStores.GetAllComponentEntityIds<TestComponent00>(allTestComponent00EntityIds, testComponent00EntityIdCount);

    functionSuccess = testComponentStores.InitializeComponentContainer<TestComponent01>(2, 2, 5);
    functionSuccess = testComponentStores.CreateComponent<TestComponent01>(entityId06, 6.0f, 66.0f, 666.0f);
    functionSuccess = testComponentStores.GetComponent<TestComponent01>(entityId06, testComponent0106ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent01>(testComponent0106ptr, entityId06_returned);

    functionSuccess = testComponentStores.CreateComponent<TestComponent01>(entityId05, 5.0f, 55.0f, 555.0f);
    functionSuccess = testComponentStores.GetComponent<TestComponent01>(entityId05, testComponent0105ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent01>(testComponent0105ptr, entityId05_returned);

    functionSuccess = testComponentStores.CreateComponent<TestComponent01>(entityId04, 4.0f, 44.0f, 444.0f);
    functionSuccess = testComponentStores.GetComponent<TestComponent01>(entityId04, testComponent0104ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent01>(testComponent0104ptr, entityId04_returned);

    functionSuccess = testComponentStores.CreateComponent<TestComponent01>(entityId03, 3.0f, 33.0f, 333.0f);
    functionSuccess = testComponentStores.GetComponent<TestComponent01>(entityId03, testComponent0103ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent01>(testComponent0103ptr, entityId03_returned);

    functionSuccess = testComponentStores.CreateComponent<TestComponent01>(entityId02, 2.0f, 22.0f, 222.0f);
    functionSuccess = testComponentStores.GetComponent<TestComponent01>(entityId02, testComponent0102ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent01>(testComponent0102ptr, entityId02_returned);

    functionSuccess = testComponentStores.CreateComponent<TestComponent01>(entityId01, 1.0f, 11.0f, 111.0f);
    functionSuccess = testComponentStores.GetComponent<TestComponent01>(entityId01, testComponent0101ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent01>(testComponent0101ptr, entityId01_returned);

    functionSuccess = testComponentStores.GetComponent<TestComponent01>(entityId06, testComponent0106ptr);
    functionSuccess = testComponentStores.GetComponent<TestComponent01>(entityId05, testComponent0105ptr);
    functionSuccess = testComponentStores.GetComponent<TestComponent01>(entityId04, testComponent0104ptr);
    functionSuccess = testComponentStores.GetComponent<TestComponent01>(entityId03, testComponent0103ptr);

    functionSuccess = testComponentStores.DeleteComponent<TestComponent00>(entityId01);
    functionSuccess = testComponentStores.DeleteComponent<TestComponent00>(entityId02);

    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId01, 1, 1, 1);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId02, 2, 2, 2);

    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId01, testComponent0001ptr);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId02, testComponent0002ptr);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId05, testComponent0005ptr);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId06, testComponent0006ptr);

    functionSuccess = testComponentStores.CreateComponent<TestComponent02>(entityId01, 1, 11, 111);
    functionSuccess = testComponentStores.GetComponent<TestComponent02>(entityId01, testComponent0201ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent02>(testComponent0201ptr, entityId01_returned);

    functionSuccess = testComponentStores.CreateComponent<TestComponent02>(entityId02, 2, 22, 222);
    functionSuccess = testComponentStores.GetComponent<TestComponent02>(entityId02, testComponent0202ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent02>(testComponent0202ptr, entityId02_returned);

    functionSuccess = testComponentStores.CreateComponent<TestComponent02>(entityId03, 3, 33, 333);
    functionSuccess = testComponentStores.GetComponent<TestComponent02>(entityId03, testComponent0203ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent02>(testComponent0203ptr, entityId03_returned);

    functionSuccess = testComponentStores.DeleteAllComponents<TestComponent02>();

    functionSuccess = testComponentStores.CreateComponent<TestComponent02>(entityId03, 3, 3, 3);
    functionSuccess = testComponentStores.GetComponent<TestComponent02>(entityId03, testComponent0203ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent02>(testComponent0203ptr, entityId03_returned);

    functionSuccess = testComponentStores.CreateComponent<TestComponent02>(entityId02, 2, 2, 2);
    functionSuccess = testComponentStores.GetComponent<TestComponent02>(entityId02, testComponent0202ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent02>(testComponent0202ptr, entityId02_returned);

    functionSuccess = testComponentStores.CreateComponent<TestComponent02>(entityId01, 1, 1, 1);
    functionSuccess = testComponentStores.GetComponent<TestComponent02>(entityId01, testComponent0201ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent02>(testComponent0201ptr, entityId01_returned);

    functionSuccess = testComponentStores.DeleteEntity(entityId04);
    functionSuccess = testComponentStores.DeleteEntity(entityId01);

    testComponentStores.DeleteAllEntities();

    functionSuccess = testComponentStores.CreateEntity(entityId01);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId01, -1, -1, -1);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId01, testComponent0001ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(testComponent0001ptr, entityId01_returned);

    functionSuccess = testComponentStores.CreateEntity(entityId02);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId02, -2, -2, -2);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId02, testComponent0002ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(testComponent0002ptr, entityId02_returned);

    functionSuccess = testComponentStores.CreateEntity(entityId03);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId03, -3, -3, -3);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId03, testComponent0003ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(testComponent0003ptr, entityId03_returned);

    functionSuccess = testComponentStores.CreateEntity(entityId04);
    functionSuccess = testComponentStores.CreateComponent<TestComponent00>(entityId04, -4, -4, -4);
    functionSuccess = testComponentStores.GetComponent<TestComponent00>(entityId04, testComponent0004ptr);
    functionSuccess = testComponentStores.GetComponentEntityId<TestComponent00>(testComponent0004ptr, entityId04_returned);
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