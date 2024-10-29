#include "TestScene007.h"

#include "TestResource_AntonioRegular_png.h"
#include "TestResource_AntonioRegular_ssf.h"

#include "Components/Camera.h"
#include "Components/RenderedModel.h"
#include "Application.h"
#include "ComponentStores.h"
#include "FontLoader.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "Renderer.h"
#include "TextureLoader.h"
#include "Window.h"



// public ----------------------------------------------------------------------

TestScene007::TestScene007(Project001::Application* applicationPtr)
    : TestSceneBase001(applicationPtr)
    , instructionScene_(applicationPtr)
    , mesh001_MeshDataPtr_(nullptr)
    , mesh001_MeshId_((unsigned int)-1)
    , mesh001_MaxBoundingRadius_(0.0f)
    , object001_EntityId_((unsigned int)-1)
{}

TestScene007::~TestScene007()
{}

void TestScene007::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene007::ProcessDeinitializeEvent, this, std::placeholders::_1));

    TestSceneBase001::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene007::ProcessInitializeEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene007::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    _LOG_MESSAGE("INITIALIZING:   TestScene007:            %u", GetId());

    // Main Camera Entity
    // -------------------------------------------------------------------------
    {
        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
        if (cameraPtr != nullptr)
        {
            cameraPtr->SetDepthTestEnabled(false);
        }
    }

    // Generate Meshes
    // -------------------------------------------------------------------------

    { // mesh001
        mesh001_MeshDataPtr_ = new Project001::MeshData();
        meshDataPtrArray_.push_back(mesh001_MeshDataPtr_);
        _FAIL_CHECK(Project001::MeshLoader::LoadMeshOBJ(*mesh001_MeshDataPtr_, "../Models/Cube.obj"));

        rendererPtr_->CreateMesh(
            mesh001_MeshId_,
            mesh001_MeshDataPtr_->meshVertexArray.data(),
            (unsigned int)mesh001_MeshDataPtr_->meshVertexArray.size(),
            mesh001_MeshDataPtr_->meshIndexArray.data(),
            (unsigned int)mesh001_MeshDataPtr_->meshIndexArray.size()
        );

        mesh001_MaxBoundingRadius_ = mesh001_MeshDataPtr_->maxBoundingRadius;
    }

    // Generate entities
    // -------------------------------------------------------------------------

    { // icosphere at the center (GPU side mesh)
        componentStoresPtr_->CreateEntity(object001_EntityId_);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(object001_EntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, object001_EntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetMeshIdAndMaxBoundingRadius(mesh001_MeshId_, mesh001_MaxBoundingRadius_);
            renderedMeshPtr->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // Load Font ---------------------------------------------------------------

    std::vector<unsigned char> characterList;
    for (unsigned char c = 32; c < 127; ++c) // ASCII characters
    {
        characterList.push_back(c);
    }

    float font01_pixelSize = 0.005f;
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

    // Member Scenes -----------------------------------------------------------

    const Project001::KeyCode keyCode_toggleInstructions = Project001::KeyCode::KEY_CODE_TAB;

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "This Scene tests ...\n"
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

void TestScene007::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    _LOG_MESSAGE("DEINITIALIZING: TestScene007:            %u", GetId());

    // Mesh Data ---------------------------------------------------------------

    // dont need to delete these here since they are added to meshDataPtrArray_

    mesh001_MeshDataPtr_ = nullptr;
    mesh001_MeshId_ = (unsigned int)-1;
    mesh001_MaxBoundingRadius_ = 0.0f;

    // Entity Ids --------------------------------------------------------------

    object001_EntityId_ = (unsigned int)-1;
}