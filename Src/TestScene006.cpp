#include "TestScene006.h"

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

TestScene006::TestScene006(Project001::Application* applicationPtr)
    : TestSceneBase001(applicationPtr, "TestScene006")
    , instructionScene_(applicationPtr, "TestInstructionScene001_006")
{}

TestScene006::~TestScene006()
{}

void TestScene006::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene006::ProcessDeinitializeEvent, this, std::placeholders::_1));

    TestSceneBase001::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene006::ProcessInitializeEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene006::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    // Load Font ---------------------------------------------------------------

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

    // Generated Shape Entity 01
    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        float width = 0.005f * font01_TextureData.width;
        float height = 0.005f * font01_TextureData.height;
        // positions.emplace_back(0.5f * width, 0.5f * height);
        // positions.emplace_back(-0.5f * width, 0.5f * height);
        // positions.emplace_back(-0.5f * width, -0.5f * height);
        // positions.emplace_back(0.5f * width, -0.5f * height);
        // _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, positions));
        // std::vector<glm::vec2> textureCoordinates;
        // textureCoordinates.emplace_back(1.0f, 1.0f);
        // textureCoordinates.emplace_back(0.0f, 1.0f);
        // textureCoordinates.emplace_back(0.0f, 0.0f);
        // textureCoordinates.emplace_back(1.0f, 0.0f);
        // Project001::MeshLoader::ApplyTextureCoordinates(*newMeshDataPtr, textureCoordinates);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*newMeshDataPtr, width, height, 0.0f, 1.0f, 0.0f, 1.0f));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(0.0f, 2.0f, 0.0f);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetTextureId(font01_TextureId);
    }

    // Generated Shape Entity 02
    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        const Project001::GlyphMetrics& currentGlyph = font01_FontData.glyphMetricsMap['A'];
        float width = 0.005f * currentGlyph.width_px;
        float height = 0.005f * currentGlyph.height_px;
        const float& textureBottom = currentGlyph.textureBottomLeft.y;
        const float& textureLeft = currentGlyph.textureBottomLeft.x;
        const float& textureTop = currentGlyph.textureTopRight.y;
        const float& textureRight = currentGlyph.textureTopRight.x;
        // std::vector<glm::vec2> positions;
        // positions.emplace_back(0.5f * width, 0.5f * height);
        // positions.emplace_back(-0.5f * width, 0.5f * height);
        // positions.emplace_back(-0.5f * width, -0.5f * height);
        // positions.emplace_back(0.5f * width, -0.5f * height);
        // _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, positions));
        // std::vector<glm::vec2> textureCoordinates;
        // textureCoordinates.emplace_back(textureRight, textureTop);
        // textureCoordinates.emplace_back(textureLeft, textureTop);
        // textureCoordinates.emplace_back(textureLeft, textureBottom);
        // textureCoordinates.emplace_back(textureRight, textureBottom);
        // Project001::MeshLoader::ApplyTextureCoordinates(*newMeshDataPtr, textureCoordinates);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*newMeshDataPtr, width, height, textureLeft, textureRight, textureBottom, textureTop));


        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(-2.0f, 1.0f, 0.0f);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetTextureId(font01_TextureId);
    }

    // Generated Shape Entity 03
    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::FreetypeTextLoader::LoadMesh(*newMeshDataPtr, font01_FontData, "Sphinx of black quartz, judge my vow!"));
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(0.0f, 0.0f, 0.0f);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetTextureId(font01_TextureId);
    }

    // Generated Shape Entity 04
    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::FreetypeTextLoader::LoadMesh(*newMeshDataPtr, font01_FontData, "LINE 001\nAND LINE 002\nAND ALSO LINE 003", 0.005f));
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(0.0f, 1.0f, 0.0f);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetTextureId(font01_TextureId);
        renderedMeshPtr->SetColorRGB(1.0f, 0.6f, 0.6f);
    }

    // Generated Shape Entity 05
    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::FreetypeTextLoader::LoadMesh(*newMeshDataPtr, font01_FontData, "LINE 001\nAND LINE 002\nAND ALSO LINE 003", 0.005f, true));
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(2.0f, 1.0f, 0.0f);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetTextureId(font01_TextureId);
        renderedMeshPtr->SetColorRGB(0.6f, 0.6f, 1.0f);
    }

    // Member Scenes -----------------------------------------------------------

    const Project001::KeyCode keyCode_toggleInstructions = Project001::KeyCode::KEY_CODE_TAB;

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "This Scene tests Text Generation.\n"
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

void TestScene006::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    // _LOG_MESSAGE("DEINITIALIZING: %s", GetName().c_str());

    // -------------------------------------------------------------------------

    instructionScene_.Deinitialize();
}