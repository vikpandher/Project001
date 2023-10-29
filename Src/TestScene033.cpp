#include "TestScene033.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/RenderedMesh.h"
#include "Engine/Math/VectorUtilities.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/FreetypeTextLoader.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/Renderer.h"
#include "Engine/TextureLoader.h"
#include "Engine/Window.h"



// public ----------------------------------------------------------------------

TestScene033::TestScene033()
{
    ClearResources();
}

TestScene033::~TestScene033()
{}

const char* TestScene033::Name()
{
    return "TestScene033";
}

// protected -------------------------------------------------------------------

bool TestScene033::OnInitialize()
{
    bool success = TestSceneBase001::OnInitialize();

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
        cameraPtr->SetProjectionToOrthographic();
        cameraPtr->TurnOn();
        cameraPtr->SetCameraMask(secondaryCameraMask_);
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
        std::vector<unsigned char> characterList;
        for (unsigned char c = 32; c < 127; ++c) // ASCII characters
        {
            characterList.push_back(c);
        }
        _FAIL_CHECK(Project001::FreetypeTextLoader::LoadTexture(font01_TextureData_, font01_FontData_, characterList, "../Fonts/Antonio-Regular.ttf", 48));
        rendererPtr_->CreateTexture(font01_TextureId_, font01_TextureData_.data, font01_TextureData_.width, font01_TextureData_.height, font01_TextureData_.bytesPerPixel, false, false);
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
        ui_testText_MeshDataPtr_ = new Project001::MeshData();
        meshDataPtrArray_.push_back(ui_testText_MeshDataPtr_);
        _FAIL_CHECK(Project001::FreetypeTextLoader::LoadMesh(*ui_testText_MeshDataPtr_, font01_FontData_, "TEST LINE 001\nTEST LINE 002\nTEST LINE 003", fontPixelSize_));
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
        _FAIL_CHECK(Project001::FreetypeTextLoader::LoadMesh(*ui_largeText_MeshDataPtr_, font01_FontData_, "SSS", fontPixelSize_));
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
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(ui_testText_EntityId_));

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(ui_testText_EntityId_));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(ui_testText_EntityId_, renderedMeshPtr));
        renderedMeshPtr->SetCameraMask(secondaryCameraMask_);
        renderedMeshPtr->SetLit(false);
        renderedMeshPtr->SetMeshDataPtr(ui_testText_MeshDataPtr_);
        renderedMeshPtr->SetTextureId(font01_TextureId_);
        renderedMeshPtr->SetTranslucent(true);

        float lineOffset = fontPixelSize_ * (float)font01_FontData_.lineSpacing_px;

        renderedMeshPtr->SetPositionX(-secondaryCameraHalfWidth + 0.2f);
        renderedMeshPtr->SetPositionY(secondaryCameraHalfHeight - lineOffset);
    }

    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(ui_fps_EntityId_));

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(ui_fps_EntityId_));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(ui_fps_EntityId_, renderedMeshPtr));
        renderedMeshPtr->SetCameraMask(secondaryCameraMask_);
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
        renderedMeshPtr->SetCameraMask(secondaryCameraMask_);
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
        renderedMeshPtr->SetCameraMask(secondaryCameraMask_);
        renderedMeshPtr->SetLit(false);
        renderedMeshPtr->SetMeshDataPtr(ui_largeText_MeshDataPtr_);
        renderedMeshPtr->SetTextureId(font01_TextureId_);
        renderedMeshPtr->SetTranslucent(true);
        renderedMeshPtr->SetScale(5.0f, 5.0f, 5.0f);
        renderedMeshPtr->SetColorRGB(0.0f, 0.0f, 0.0f);
    }

    return success && true;
}

bool TestScene033::OnDeinitialize()
{
    bool success = TestSceneBase001::OnDeinitialize();

    ClearResources();

    return success && true;
}

void TestScene033::OnHandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene033::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene033::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene033::ProcessUpdateEvent, this, std::placeholders::_1));

    TestSceneBase001::OnHandleEvent(event);
}

void TestScene033::ClearResources()
{
    // Texture Data: -----------------------------------------------------------

    _32x32_TextureIds_.clear();
    _48x48_TextureIds_.clear();

    font01_FontData_.Clear();
    font01_TextureData_.Clear();
    font01_TextureId_ = (unsigned int)-1;

    // Mesh Data: --------------------------------------------------------------

    square_MeshDataPtr_ = nullptr;
    square_MeshId_ = (unsigned int)-1;
    square_MaxBoundingRadius_ = 0.0f;

    ui_testText_MeshDataPtr_ = nullptr;

    ui_fps_MeshDataPtr_ = nullptr;

    counter_ = 0;
    ui_counter_MeshDataPtr_ = nullptr;

    ui_largeText_MeshDataPtr_ = nullptr;

    // Entity Ids: -------------------------------------------------------------

    secondaryCameraEntityId_ = (unsigned int)-1;

    tertiaryCameraEntityId_ = (unsigned int)-1;

    square_EntityIds_.clear();

    ui_testText_EntityId_ = (unsigned int)-1;

    ui_fps_EntityId_ = (unsigned int)-1;

    ui_counter_EntityId_ = (unsigned int)-1;

    ui_largeText_EntityId_ = (unsigned int)-1;
}

void TestScene033::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene034"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene034"));
            }
        }
    }
}

void TestScene033::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    float fps = 1000000000.0f / (float)renderEvent.timestep_ns;
    std::string fps_string = std::to_string(fps);
    ui_fps_MeshDataPtr_->Clear();
    _FAIL_CHECK(Project001::FreetypeTextLoader::LoadMesh(*ui_fps_MeshDataPtr_, font01_FontData_, fps_string, fontPixelSize_));
    Project001::MeshLoader::RecenterMesh(*ui_fps_MeshDataPtr_);
    Project001::MeshLoader::TranslateMesh(*ui_fps_MeshDataPtr_, -0.5f * ui_fps_MeshDataPtr_->GetSize());
}

void TestScene033::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    std::string counter_string = std::to_string(++counter_);
    ui_counter_MeshDataPtr_->Clear();
    _FAIL_CHECK(Project001::FreetypeTextLoader::LoadMesh(*ui_counter_MeshDataPtr_, font01_FontData_, counter_string, fontPixelSize_));
    Project001::MeshLoader::RecenterMesh(*ui_counter_MeshDataPtr_);
    Project001::MeshLoader::TranslateMesh(*ui_counter_MeshDataPtr_, 0.5f * ui_counter_MeshDataPtr_->GetSize());
}