#include "TestScene004.h"

#include "Engine/Components/RenderedMesh.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/FreetypeTextLoader.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/Renderer.h"
#include "Engine/TextureLoader.h"
#include "Engine/Window.h"



// public ----------------------------------------------------------------------

TestScene004::TestScene004(Project001::Application* applicationPtr)
    : TestSceneBase001(applicationPtr, "TestScene004")
    , instructionScene_(applicationPtr, "TestInstructionScene001_004")
{}

TestScene004::~TestScene004()
{}

void TestScene004::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene004::ProcessDeinitializeEvent, this, std::placeholders::_1));

    TestSceneBase001::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene004::ProcessInitializeEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene004::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    // Mesh Data ---------------------------------------------------------------

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

    for(size_t i = 0; i < 44; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*newMeshDataPtr, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));
    }

    // Texture Data ------------------------------------------------------------

    std::vector<unsigned int> _32x32_TextureIds_;

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
        rendererPtr_->CreateTexture(tempTextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
        _32x32_TextureIds_.push_back(tempTextureId);
    }

    for (size_t i = 35; i < 70; ++i)
    {
        std::string filePath("../Textures/48x48_");
        if (i / 10 == 0)
        {
            filePath += '0';
        }
        filePath += std::to_string(i);
        filePath += ".png";

        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, filePath));
        unsigned int tempTextureId = (unsigned int)-1;
        rendererPtr_->CreateTexture(tempTextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
        _32x32_TextureIds_.push_back(tempTextureId);
    }

    // Creating Entities
    // -------------------------------------------------------------------------

    // Calculating positions ---------------------------------------------------

    std::vector<glm::vec3> meshEntityPositions;
    for (int i = 2; i >= -2; --i)
    {
        for (int j = -3; j <= 3; ++j)
        {
            meshEntityPositions.emplace_back((float)j, (float)i, 0.0f);
        }
    }

    // back
    for (int i = 2; i >= -2; --i)
    {
        for (int j = -3; j <= 3; ++j)
        {
            meshEntityPositions.emplace_back((float)j, (float)i, -1.0f);
        }
    }

    // Generate Entitys
    // -------------------------------------------------------------------------
    for (size_t i = 0; i < 70; ++i)
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));

        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(meshEntityPositions[i]);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[i]);
        renderedMeshPtr->SetTextureId(_32x32_TextureIds_[i]);
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
        "This Scene tests loading many textures at once,\n"
        "as well as some more Mesh Generation.\n"
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

void TestScene004::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    // _LOG_MESSAGE("DEINITIALIZING: %s", GetName().c_str());

    // -------------------------------------------------------------------------

    instructionScene_.Deinitialize();
}