#include "TestScene033.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/RenderedMesh.h"
#include "Engine/Math/VectorUtilities.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/FreetypeTextLoader.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/Renderer.h"
#include "Engine/TextureLoader.h"
#include "Engine/Window.h"



// public ----------------------------------------------------------------------

TestScene033::TestScene033(Project001::Application* applicationPtr)
    : TestSceneBase001(applicationPtr, "TestScene033")
    , instructionScene_(applicationPtr, "TestInstructionScene001_033")
    , _32x32_TextureIds_()
    , _48x48_TextureIds_()
    , font01_FontDataPtr_(nullptr)
    , font01_TextureDataPtr_(nullptr)
    , font01_TextureId_((unsigned int)-1)
    , square_MeshDataPtr_(nullptr)
    , square_MeshId_((unsigned int)-1)
    , square_MaxBoundingRadius_(0.0f)
    , ui_fps_MeshDataPtr_(nullptr)
    , counter_(0)
    , ui_counter_MeshDataPtr_(nullptr)
    , ui_largeText_MeshDataPtr_(nullptr)
    , secondaryCameraEntityId_((unsigned int)-1)
    , tertiaryCameraEntityId_((unsigned int)-1)
    , square_EntityIds_()
    , ui_fps_EntityId_((unsigned int)-1)
    , ui_counter_EntityId_((unsigned int)-1)
    , ui_largeText_EntityId_((unsigned int)-1)
{}

TestScene033::~TestScene033()
{}

void TestScene033::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene033::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene033::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene033::ProcessUpdateEvent, this, std::placeholders::_1));

    TestSceneBase001::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene033::ProcessInitializeEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene033::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    // Secindary camera
    // -------------------------------------------------------------------------

    float secondaryCameraHalfWidth = 0.0f;
    float secondaryCameraHalfHeight = 0.0f;

    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(secondaryCameraEntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::Camera>(secondaryCameraEntityId_));

        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(secondaryCameraEntityId_, cameraPtr));
        int aspectRatioNumerator;
        int aspectRatioDenominator;
        windowPtr_->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
        if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
        {
            float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;
            secondaryCameraHalfHeight = 3.5f;
            secondaryCameraHalfWidth = aspectRatio * secondaryCameraHalfHeight;

            cameraPtr->SetAspectRatio(aspectRatio);
            cameraPtr->SetTopCutoff(secondaryCameraHalfHeight);
            cameraPtr->SetBottomCutoff(-secondaryCameraHalfHeight);
            cameraPtr->SetLeftCutoff(-secondaryCameraHalfWidth);
            cameraPtr->SetRightCutoff(secondaryCameraHalfWidth);
            cameraPtr->SetNearCutoff(-1.0f);
            cameraPtr->SetFarCutoff(1.0f);
        }
        cameraPtr->AddYaw(glm::pi<float>());
        cameraPtr->SetProjection(Project001::Camera::CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC);
        cameraPtr->TurnOn();
        cameraPtr->SetCameraMask(s_secondaryCameraMask_);
        cameraPtr->SetPriorityValue(100);
    }

    // Tertiary camera
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tertiaryCameraEntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::Camera>(tertiaryCameraEntityId_));

        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(tertiaryCameraEntityId_, cameraPtr));
        int aspectRatioNumerator;
        int aspectRatioDenominator;
        windowPtr_->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
        if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
        {
            float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;
            cameraPtr->SetAspectRatio(aspectRatio);
        }
        // cameraPtr->SetCameraViewport(0.5f, 0.25f, 0.25f, 0.25f);
        cameraPtr->SetCameraViewport(0.75f, 0.0f, 0.25f, 0.25f);
        cameraPtr->SetPosition(0.0f, 0.0f, 7.5f);
        cameraPtr->AddYaw(glm::pi<float>());
        cameraPtr->TurnOn();
        cameraPtr->SetPriorityValue(101);
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
        _48x48_TextureIds_.push_back(tempTextureId);
    }

    {
        font01_FontDataPtr_ = new Project001::FontData();
        font01_TextureDataPtr_ = new Project001::TextureData();
        std::vector<unsigned char> characterList;
        for (unsigned char c = 32; c < 127; ++c) // ASCII characters
        {
            characterList.push_back(c);
        }
        _FAIL_CHECK(Project001::FreetypeTextLoader::LoadTextureDataAndFontData(
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
            false,
            false
        );
    }

    // Generate meshes
    // -------------------------------------------------------------------------

    {
        square_MeshDataPtr_ = new Project001::MeshData();
        meshDataPtrArray_.push_back(square_MeshDataPtr_);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*square_MeshDataPtr_, 0.64f, 0.64f, 0.0f, 1.0f, 0.0f, 1.0f));

        rendererPtr_->CreateMesh(
            square_MeshId_,
            square_MeshDataPtr_->meshVertexArray.data(),
            (unsigned int)square_MeshDataPtr_->meshVertexArray.size(),
            square_MeshDataPtr_->meshIndexArray.data(),
            (unsigned int)square_MeshDataPtr_->meshIndexArray.size()
        );

        square_MaxBoundingRadius_ = square_MeshDataPtr_->maxBoundingRadius;
    }

    {
        ui_fps_MeshDataPtr_ = new Project001::MeshData();
        meshDataPtrArray_.push_back(ui_fps_MeshDataPtr_);
    }

    {
        ui_counter_MeshDataPtr_ = new Project001::MeshData();
        meshDataPtrArray_.push_back(ui_counter_MeshDataPtr_);
    }

    {
        ui_largeText_MeshDataPtr_ = new Project001::MeshData();
        meshDataPtrArray_.push_back(ui_largeText_MeshDataPtr_);
        _FAIL_CHECK(Project001::FreetypeTextLoader::LoadMeshData(*ui_largeText_MeshDataPtr_, *font01_FontDataPtr_, "SSS", fontPixelSize_));
        Project001::MeshLoader::RecenterMesh(*ui_largeText_MeshDataPtr_);
    }

    // Generate entities
    // -------------------------------------------------------------------------

    std::vector<glm::vec3> meshEntityPositions;
    int columns = 9;
    int rows = 5;
    int depthRows = 1;

    for (int i = columns / -2; i < (columns + 1) / 2; ++i)
    {
        for (int j = rows / -2; j < (rows + 1) / 2; ++j)
        {
            for (int k = depthRows / -2; k < (depthRows + 1) / 2; ++k)
            {
                meshEntityPositions.emplace_back(1.0f * i, 1.0f * j, 1.0f * k);
            }
        }
    }

    for (size_t i = 0; i < meshEntityPositions.size(); i++)
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        square_EntityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[i];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetMeshId(square_MeshId_);
        renderedMeshPtr->SetMaxBoundingRadius(square_MaxBoundingRadius_);

        if (i < _32x32_TextureIds_.size())
        {
            renderedMeshPtr->SetTextureId(_32x32_TextureIds_[i]);
        }
        else if (i < _32x32_TextureIds_.size() + _48x48_TextureIds_.size())
        {
            renderedMeshPtr->SetTextureId(_48x48_TextureIds_[i - _32x32_TextureIds_.size()]);
        }
    }

    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(ui_fps_EntityId_));

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(ui_fps_EntityId_));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(ui_fps_EntityId_, renderedMeshPtr));
        renderedMeshPtr->SetCameraMask(s_secondaryCameraMask_);
        renderedMeshPtr->SetLit(false);
        renderedMeshPtr->SetMeshDataPtr(ui_fps_MeshDataPtr_);
        renderedMeshPtr->SetTextureId(font01_TextureId_);
        renderedMeshPtr->SetTranslucent(true);
        renderedMeshPtr->SetPositionX(secondaryCameraHalfWidth - 0.2f);
        renderedMeshPtr->SetPositionY(secondaryCameraHalfHeight - 0.2f);
    }

    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(ui_counter_EntityId_));

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(ui_counter_EntityId_));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(ui_counter_EntityId_, renderedMeshPtr));
        renderedMeshPtr->SetCameraMask(s_secondaryCameraMask_);
        renderedMeshPtr->SetLit(false);
        renderedMeshPtr->SetMeshDataPtr(ui_counter_MeshDataPtr_);
        renderedMeshPtr->SetTextureId(font01_TextureId_);
        renderedMeshPtr->SetTranslucent(true);
        renderedMeshPtr->SetPositionX(0.2f - secondaryCameraHalfWidth);
        renderedMeshPtr->SetPositionY(0.2f - secondaryCameraHalfHeight);
    }

    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(ui_largeText_EntityId_));

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(ui_largeText_EntityId_));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(ui_largeText_EntityId_, renderedMeshPtr));
        renderedMeshPtr->SetCameraMask(s_secondaryCameraMask_);
        renderedMeshPtr->SetLit(false);
        renderedMeshPtr->SetMeshDataPtr(ui_largeText_MeshDataPtr_);
        renderedMeshPtr->SetTextureId(font01_TextureId_);
        renderedMeshPtr->SetTranslucent(true);
        renderedMeshPtr->SetScale(5.0f, 5.0f, 5.0f);
        renderedMeshPtr->SetColorRGB(0.0f, 0.0f, 0.0f);
    }

    // Member Scenes -----------------------------------------------------------

    const Project001::KeyCode keyCode_toggleInstructions = Project001::KeyCode::KEY_CODE_TAB;

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "This Scene tests rendering with multiple Cameras.\n"
        "Use <WASD> to move the camera up, left, down, and right.\n"
        "Use <Q> to roll left and <E> to roll right.\n"
        "Use <Scroll> to move forward and back.\n"
        "<Left-Click> and drag the <Mouse> to move camera.\n"
        "Press <Esc> to return to Main Menu.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = font01_FontDataPtr_;
    instructionSceneInfo.fontTextureIdPtr = &font01_TextureId_;
    instructionSceneInfo.cameraEntityIdPtr = &uiCameraEntityId_;
    instructionSceneInfo.cameraMaskPtr = &s_uiCameraMask_;
    instructionSceneInfo.keyCode_toggleInstructionsPtr = &keyCode_toggleInstructions;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene033::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    // _LOG_MESSAGE("DEINITIALIZING: %s", GetName().c_str());

    // -------------------------------------------------------------------------

    instructionScene_.Deinitialize();

    // Texture Data: -----------------------------------------------------------

    _32x32_TextureIds_.clear();
    _48x48_TextureIds_.clear();

    delete font01_FontDataPtr_;
    font01_FontDataPtr_ = nullptr;

    delete font01_TextureDataPtr_;
    font01_TextureDataPtr_ = nullptr;

    font01_TextureId_ = (unsigned int)-1;

    // Mesh Data: --------------------------------------------------------------

    // dont need to delete these here since they are added to meshDataPtrArray_

    square_MeshDataPtr_ = nullptr;
    square_MeshId_ = (unsigned int)-1;
    square_MaxBoundingRadius_ = 0.0f;

    ui_fps_MeshDataPtr_ = nullptr;

    counter_ = 0;
    ui_counter_MeshDataPtr_ = nullptr;

    ui_largeText_MeshDataPtr_ = nullptr;

    // Entity Ids: -------------------------------------------------------------

    secondaryCameraEntityId_ = (unsigned int)-1;

    tertiaryCameraEntityId_ = (unsigned int)-1;

    square_EntityIds_.clear();

    ui_fps_EntityId_ = (unsigned int)-1;

    ui_counter_EntityId_ = (unsigned int)-1;

    ui_largeText_EntityId_ = (unsigned int)-1;
}

void TestScene033::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    float fps = 1000000000.0f / (float)renderEvent.timestep_ns;
    std::string fps_string = std::to_string(fps);
    ui_fps_MeshDataPtr_->Clear();
    _FAIL_CHECK(Project001::FreetypeTextLoader::LoadMeshData(*ui_fps_MeshDataPtr_, *font01_FontDataPtr_, fps_string, fontPixelSize_));
    Project001::MeshLoader::RecenterMesh(*ui_fps_MeshDataPtr_);
    Project001::MeshLoader::TranslateMesh(*ui_fps_MeshDataPtr_, -0.5f * ui_fps_MeshDataPtr_->GetSize());
}

void TestScene033::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    std::string counter_string = std::to_string(++counter_);
    ui_counter_MeshDataPtr_->Clear();
    _FAIL_CHECK(Project001::FreetypeTextLoader::LoadMeshData(*ui_counter_MeshDataPtr_, *font01_FontDataPtr_, counter_string, fontPixelSize_));
    Project001::MeshLoader::RecenterMesh(*ui_counter_MeshDataPtr_);
    Project001::MeshLoader::TranslateMesh(*ui_counter_MeshDataPtr_, 0.5f * ui_counter_MeshDataPtr_->GetSize());
}