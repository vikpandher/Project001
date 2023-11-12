#include "TestScene031.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/RenderedMesh.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/FreetypeTextLoader.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/RenderSystem.h"
#include "Engine/TextureLoader.h"
#include "Engine/Window.h"



// public ----------------------------------------------------------------------

TestScene031::TestScene031(Project001::Application* applicationPtr)
    : TestSceneBase001(applicationPtr, "TestScene031")
    , instructionScene_(applicationPtr, "TestInstructionScene001_031")
    , font01_FontDataPtr_(nullptr)
    , font01_TextureDataPtr_(nullptr)
    , font01_TextureId_((unsigned int)-1)
    , ui_fps_MeshDataPtr_(nullptr)
    , ui_renderedMeshCount_MeshDataPtr_(nullptr)
    , ui_fps_EntityId_((unsigned int)-1)
    , ui_renderedMeshCount_EntityId_((unsigned int)-1)
{}

TestScene031::~TestScene031()
{}

void TestScene031::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene031::ProcessDeinitializeEvent, this, std::placeholders::_1));

    TestSceneBase001::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene031::ProcessInitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene031::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene031::ProcessUpdateEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene031::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    // Texture Data ------------------------------------------------------------

    font01_FontDataPtr_ = new Project001::FontData();
    font01_TextureDataPtr_ = new Project001::TextureData();
    font01_TextureId_ = (unsigned int)-1;
    std::vector<unsigned char> characterList;
    for (unsigned char c = 32; c < 127; ++c) // ASCII characters
    {
        characterList.push_back(c);
    }
    _FAIL_CHECK(Project001::FreetypeTextLoader::LoadTexture(
        *font01_TextureDataPtr_,
        *font01_FontDataPtr_,
        characterList,
        "../Fonts/Antonio-Regular.ttf",
        48
    ));
    rendererPtr_->CreateTexture(
        font01_TextureId_,
        font01_TextureDataPtr_->data,
        font01_TextureDataPtr_->width,
        font01_TextureDataPtr_->height,
        font01_TextureDataPtr_->bytesPerPixel,
        true,
        false
    );

    // Generate Meshes ---------------------------------------------------------
    Project001::MeshData* meshDataPtr00 = new Project001::MeshData();
    meshDataPtrArray_.push_back(meshDataPtr00);
    glm::vec3 min(-0.04f, -0.04f, -0.04f);
    glm::vec3 max(0.04f, 0.04f, 0.04f);
    _FAIL_CHECK(Project001::MeshLoader::GenerateBox(*meshDataPtr00, min, max, false));

    unsigned int meshId00;
    rendererPtr_->CreateMesh(
        meshId00,
        meshDataPtr00->meshVertexArray.data(),
        (unsigned int)meshDataPtr00->meshVertexArray.size(),
        meshDataPtr00->meshIndexArray.data(),
        (unsigned int)meshDataPtr00->meshIndexArray.size()
    );

    float maxBoundingRadius00 = meshDataPtr00->maxBoundingRadius;

    ui_fps_MeshDataPtr_ = new Project001::MeshData();
    meshDataPtrArray_.push_back(ui_fps_MeshDataPtr_);

    ui_renderedMeshCount_MeshDataPtr_ = new Project001::MeshData();
    meshDataPtrArray_.push_back(ui_renderedMeshCount_MeshDataPtr_);

    // Calculating Positions ---------------------------------------------------

    std::vector<glm::vec3> meshEntityPositions;
#ifdef NDEBUG
    int columns = 50;
    int rows = 40;
    int depthRows = 50;
#else
    int columns = 50;
    int rows = 40;
    int depthRows = 10;
#endif

    for (int i = columns / -2; i < (columns + 1) / 2; ++i)
    {
        for (int j = rows / -2; j < (rows + 1) / 2; ++j)
        {
            for (int k = depthRows / -2; k < (depthRows + 1) / 2; ++k)
            {
                meshEntityPositions.emplace_back(0.1f * i, 0.1f * j, 0.1f * k);
            }
        }
    }

    // Generate Entities -------------------------------------------------------
    for (size_t i = 0; i < meshEntityPositions.size(); i++)
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[i];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetColor(
            0.5f + currentPosition.x * 0.1f,
            0.5f + currentPosition.y * 0.1f,
            0.5f + currentPosition.z * 0.1f,
            0.5f
        );
        renderedMeshPtr->SetMeshId(meshId00);
        renderedMeshPtr->SetTranslucent(true);
        renderedMeshPtr->SetMaxBoundingRadius(maxBoundingRadius00);
    }

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(uiCameraEntityId_, cameraPtr));
    float uiCameraHalfHeight = cameraPtr->GetTopCutoff();
    float uiCameraHalfWidth = cameraPtr->GetRightCutoff();

    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(ui_fps_EntityId_));

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(ui_fps_EntityId_));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(ui_fps_EntityId_, renderedMeshPtr));
        renderedMeshPtr->SetCameraMask(s_uiCameraMask_);
        renderedMeshPtr->SetLit(false);
        renderedMeshPtr->SetMeshDataPtr(ui_fps_MeshDataPtr_);
        renderedMeshPtr->SetTextureId(font01_TextureId_);
        renderedMeshPtr->SetTranslucent(true);
        renderedMeshPtr->SetPositionX(uiCameraHalfWidth - 0.2f);
        renderedMeshPtr->SetPositionY(uiCameraHalfHeight - 0.2f);
    }

    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(ui_renderedMeshCount_EntityId_));

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(ui_renderedMeshCount_EntityId_));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(ui_renderedMeshCount_EntityId_, renderedMeshPtr));
        renderedMeshPtr->SetCameraMask(s_uiCameraMask_);
        renderedMeshPtr->SetLit(false);
        renderedMeshPtr->SetMeshDataPtr(ui_renderedMeshCount_MeshDataPtr_);
        renderedMeshPtr->SetTextureId(font01_TextureId_);
        renderedMeshPtr->SetTranslucent(true);
        renderedMeshPtr->SetPositionX(0.2f - uiCameraHalfWidth);
        renderedMeshPtr->SetPositionY(0.2f - uiCameraHalfHeight);
    }

    // Member Scenes -----------------------------------------------------------

    const Project001::KeyCode keyCode_toggleInstructions = Project001::KeyCode::KEY_CODE_TAB;

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "This Scene tests the fps when rendering many Instanced Meshes.\n"
        "Instance information is sent to the the Renderer every frame.\n"
        "24 Verticies per Mesh, 36 Indicies per Mesh.\n"
        "Use <WASD> to move the camera up, left, down, and right.\n"
        "Use <Q> to roll left and <E> to roll right.\n"
        "Use <Scroll> to move forward and back.\n"
        "<Left-Click> and drag the <Mouse> to move camera.\n"
        "Press <ESC> to return to Main Menu.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = font01_FontDataPtr_;
    instructionSceneInfo.fontTextureIdPtr = &font01_TextureId_;
    instructionSceneInfo.cameraEntityIdPtr = &uiCameraEntityId_;
    instructionSceneInfo.cameraMaskPtr = &s_uiCameraMask_;
    instructionSceneInfo.keyCode_toggleInstructionsPtr = &keyCode_toggleInstructions;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene031::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    // _LOG_MESSAGE("DEINITIALIZING: %s", GetName().c_str());

    // -------------------------------------------------------------------------

    instructionScene_.Deinitialize();

    // Texture Data ------------------------------------------------------------

    delete font01_FontDataPtr_;
    font01_FontDataPtr_ = nullptr;

    delete font01_TextureDataPtr_;
    font01_TextureDataPtr_ = nullptr;

    font01_TextureId_ = (unsigned int)-1;

    // Mesh Data ---------------------------------------------------------------

    // dont need to delete these here since they are added to meshDataPtrArray_

    ui_fps_MeshDataPtr_ = nullptr;

    ui_renderedMeshCount_MeshDataPtr_ = nullptr;

    // Entity Ids --------------------------------------------------------------

    ui_fps_EntityId_ = (unsigned int)-1;

    ui_renderedMeshCount_EntityId_ = (unsigned int)-1;
}

void TestScene031::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    float fps = 1000000000.0f / (float)renderEvent.timestep_ns;
    std::string fps_string = std::to_string(fps);
    ui_fps_MeshDataPtr_->Clear();
    _FAIL_CHECK(Project001::FreetypeTextLoader::LoadMesh(*ui_fps_MeshDataPtr_, *font01_FontDataPtr_, fps_string, fontPixelSize_));
    Project001::MeshLoader::RecenterMesh(*ui_fps_MeshDataPtr_);
    Project001::MeshLoader::TranslateMesh(*ui_fps_MeshDataPtr_, -0.5f * ui_fps_MeshDataPtr_->GetSize());
}

void TestScene031::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    size_t numberOfMeshesBeingRenderedForMainCamera = 0;
    if (Project001::RenderSystem::GetNumberOfMeshesRendered(mainCameraEntityId_, numberOfMeshesBeingRenderedForMainCamera))
    {
        std::string count_string = std::to_string(numberOfMeshesBeingRenderedForMainCamera);
        ui_renderedMeshCount_MeshDataPtr_->Clear();
        _FAIL_CHECK(Project001::FreetypeTextLoader::LoadMesh(*ui_renderedMeshCount_MeshDataPtr_, *font01_FontDataPtr_, count_string, fontPixelSize_));
        Project001::MeshLoader::RecenterMesh(*ui_renderedMeshCount_MeshDataPtr_);
        Project001::MeshLoader::TranslateMesh(*ui_renderedMeshCount_MeshDataPtr_, 0.5f * ui_renderedMeshCount_MeshDataPtr_->GetSize());
    }
}