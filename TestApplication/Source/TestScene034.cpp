#include "TestScene034.h"

#include "Components/Camera.h"
#include "Components/RenderedModel.h"
#include "Math/VectorUtilities.h"
#include "Application.h"
#include "ComponentStores.h"
#include "FreetypeTextLoader.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "Renderer.h"
#include "TextureLoader.h"
#include "Window.h"



// public ----------------------------------------------------------------------

TestScene034::TestScene034(Project001::Application* applicationPtr)
    : TestSceneBase001(applicationPtr)
    , instructionScene_(applicationPtr)
    , thonk001_TextureId_((unsigned int)-1)
    , thonkSpecular001_TextureId_((unsigned int)-1)
    , _32x32_TextureIds_()
    , cube001_MeshDataPtr_(nullptr)
    , cube001_MeshId_((unsigned int)-1)
    , cube001_MaxBoundingRadius_(0.0f)
    , line001_MeshDataPtr_(nullptr)
    , line001_MeshId_((unsigned int)-1)
    , line001_MaxBoundingRadius_(0.0f)
    , cone001_MeshDataPtr_(nullptr)
    , cone001_MeshId_((unsigned int)-1)
    , cone001_MaxBoundingRadius_(0.0f)
{}

TestScene034::~TestScene034()
{}

void TestScene034::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene034::ProcessDeinitializeEvent, this, std::placeholders::_1));

    TestSceneBase001::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene034::ProcessInitializeEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene034::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    _LOG_MESSAGE("INITIALIZING:   TestScene034:            %u", GetId());

    // Load Textures
    // -------------------------------------------------------------------------

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/Thonk.png"));
        rendererPtr_->CreateTexture(thonk001_TextureId_, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/ThonkSpecular.png"));
        rendererPtr_->CreateTexture(thonkSpecular001_TextureId_, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

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

    // Generate meshes
    // -------------------------------------------------------------------------

    { // cube
        cube001_MeshDataPtr_ = new Project001::MeshData();
        meshDataPtrArray_.push_back(cube001_MeshDataPtr_);
        // _FAIL_CHECK(Project001::MeshLoader::GenerateBox(*cube001_MeshDataPtr_, glm::vec3(-0.34f), glm::vec3(0.34f), false));
        _FAIL_CHECK(Project001::MeshLoader::LoadMeshOBJ(*cube001_MeshDataPtr_, "../Models/Cube.obj"));

        rendererPtr_->CreateMesh(
            cube001_MeshId_,
            cube001_MeshDataPtr_->meshVertexArray.data(),
            (unsigned int)cube001_MeshDataPtr_->meshVertexArray.size(),
            cube001_MeshDataPtr_->meshIndexArray.data(),
            (unsigned int)cube001_MeshDataPtr_->meshIndexArray.size()
        );

        cube001_MaxBoundingRadius_ = cube001_MeshDataPtr_->maxBoundingRadius;
    }

    { // line star
        glm::vec2 topPosition(0.0f, 0.32f);
        glm::vec2 rightPosition = Project001::Rotate2DVector(topPosition, glm::pi<float>() * 2.0f / 5.0f);
        glm::vec2 bottomRightPosition = Project001::Rotate2DVector(rightPosition, glm::pi<float>() * 2.0f / 5.0f);
        glm::vec2 bottomLeftPosition = Project001::Rotate2DVector(bottomRightPosition, glm::pi<float>() * 2.0f / 5.0f);
        glm::vec2 leftPosition = Project001::Rotate2DVector(bottomLeftPosition, glm::pi<float>() * 2.0f / 5.0f);

        std::vector<glm::vec2> positions;
        positions.emplace_back(bottomLeftPosition);
        positions.emplace_back(topPosition);
        positions.emplace_back(bottomRightPosition);
        positions.emplace_back(leftPosition);
        positions.emplace_back(rightPosition);

        for (size_t i = 0; i < positions.size(); ++i)
        {
            positions[i] += glm::vec2(0.5f, 0.5f);
        }

        line001_MeshDataPtr_ = new Project001::MeshData();
        meshDataPtrArray_.push_back(line001_MeshDataPtr_);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(*line001_MeshDataPtr_, positions, 0.16f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*line001_MeshDataPtr_);
        Project001::MeshLoader::RecenterMesh(*line001_MeshDataPtr_);

        rendererPtr_->CreateMesh(
            line001_MeshId_,
            line001_MeshDataPtr_->meshVertexArray.data(),
            (unsigned int)line001_MeshDataPtr_->meshVertexArray.size(),
            line001_MeshDataPtr_->meshIndexArray.data(),
            (unsigned int)line001_MeshDataPtr_->meshIndexArray.size()
        );

        line001_MaxBoundingRadius_ = line001_MeshDataPtr_->maxBoundingRadius;
    }

    { // cone
        cone001_MeshDataPtr_ = new Project001::MeshData();
        meshDataPtrArray_.push_back(cone001_MeshDataPtr_);
        _FAIL_CHECK(Project001::MeshLoader::GenerateCone(*cone001_MeshDataPtr_, 0.64f, 0.32f, 8, false));

        rendererPtr_->CreateMesh(
            cone001_MeshId_,
            cone001_MeshDataPtr_->meshVertexArray.data(),
            (unsigned int)cone001_MeshDataPtr_->meshVertexArray.size(),
            cone001_MeshDataPtr_->meshIndexArray.data(),
            (unsigned int)cone001_MeshDataPtr_->meshIndexArray.size()
        );

        cone001_MaxBoundingRadius_ = cone001_MeshDataPtr_->maxBoundingRadius;
    }
 
    // Create Entities
    // -------------------------------------------------------------------------

    std::vector<glm::vec3> positions;
    for (int i = 1; i >= -1; --i)
    {
        for (int j = -2; j <= 2; ++j)
        {
            positions.emplace_back(2.0f * (float)j, 2.0f * (float)i, 0.0f);
        }
    }

    std::vector<glm::quat> orientations;
    for(size_t i = 0; i < 5; ++i)
    {
        orientations.emplace_back(glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::quarter_pi<float>() * i, glm::vec3(1.0f, 0.0f, 0.0f)));
    }
    for (size_t i = 0; i < 5; ++i)
    {
        orientations.emplace_back(glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::quarter_pi<float>() * i, glm::vec3(0.0f, 1.0f, 0.0f)));
    }
    for (size_t i = 0; i < 5; ++i)
    {
        orientations.emplace_back(glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::quarter_pi<float>() * i, glm::vec3(0.0f, 0.0f, 1.0f)));
    }

    for (size_t i = 0; i < 15; ++i)
    {
        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(renderedModelPtr, tempEntityId));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetPosition(positions[i]);
            renderedModelPtr->SetOrientation(orientations[i]);
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            renderedMeshes.emplace_back();
            Project001::RenderedMesh& cubeMesh = renderedMeshes.back();
            cubeMesh.SetMeshId(cube001_MeshId_);
            cubeMesh.SetMaxBoundingRadius(cube001_MeshDataPtr_->maxBoundingRadius);
            cubeMesh.SetTextureId(thonk001_TextureId_);
            cubeMesh.SetSpecularId(thonkSpecular001_TextureId_);
            cubeMesh.SetShininess(32.0f);

            renderedMeshes.emplace_back();
            Project001::RenderedMesh& redConeMesh = renderedMeshes.back();
            redConeMesh.SetMeshId(cone001_MeshId_);
            redConeMesh.SetMaxBoundingRadius(cone001_MeshDataPtr_->maxBoundingRadius);
            redConeMesh.SetColor(0.8f, 0.2f, 0.2f, 0.4f);
            redConeMesh.SetTranslucent(true);
            redConeMesh.SetPositionX(0.64f);
            redConeMesh.AddRelativeRotationZ(-glm::half_pi<float>());

            renderedMeshes.emplace_back();
            Project001::RenderedMesh& redStarMesh = renderedMeshes.back();
            redStarMesh.SetMeshId(line001_MeshId_);
            redStarMesh.SetMaxBoundingRadius(line001_MeshDataPtr_->maxBoundingRadius);
            redStarMesh.SetColor(0.8f, 0.2f, 0.2f, 1.0f);
            redStarMesh.SetTextureId(_32x32_TextureIds_[0]);
            redStarMesh.SetPositionX(-0.36f);
            redStarMesh.AddRelativeRotationY(-glm::half_pi<float>());

            renderedMeshes.emplace_back();
            Project001::RenderedMesh& greenConeMesh = renderedMeshes.back();
            greenConeMesh.SetMeshId(cone001_MeshId_);
            greenConeMesh.SetMaxBoundingRadius(cone001_MeshDataPtr_->maxBoundingRadius);
            greenConeMesh.SetColor(0.2f, 0.8f, 0.2f, 0.4f);
            greenConeMesh.SetTranslucent(true);
            greenConeMesh.SetPositionY(0.64f);

            renderedMeshes.emplace_back();
            Project001::RenderedMesh& greenStarMesh = renderedMeshes.back();
            greenStarMesh.SetMeshId(line001_MeshId_);
            greenStarMesh.SetMaxBoundingRadius(line001_MeshDataPtr_->maxBoundingRadius);
            greenStarMesh.SetColor(0.2f, 0.8f, 0.2f, 1.0f);
            greenStarMesh.SetTextureId(_32x32_TextureIds_[1]);
            greenStarMesh.SetPositionY(-0.36f);
            greenStarMesh.AddRelativeRotationX(glm::half_pi<float>());

            renderedMeshes.emplace_back();
            Project001::RenderedMesh& blueConeMesh = renderedMeshes.back();
            blueConeMesh.SetMeshId(cone001_MeshId_);
            blueConeMesh.SetMaxBoundingRadius(cone001_MeshDataPtr_->maxBoundingRadius);
            blueConeMesh.SetColor(0.2f, 0.2f, 0.8f, 0.4f);
            blueConeMesh.SetTranslucent(true);
            blueConeMesh.SetPositionZ(0.64f);
            blueConeMesh.AddRelativeRotationX(glm::half_pi<float>());

            renderedMeshes.emplace_back();
            Project001::RenderedMesh& blueStarMesh = renderedMeshes.back();
            blueStarMesh.SetMeshId(line001_MeshId_);
            blueStarMesh.SetMaxBoundingRadius(line001_MeshDataPtr_->maxBoundingRadius);
            blueStarMesh.SetColor(0.2f, 0.2f, 0.8f, 1.0f);
            blueStarMesh.SetTextureId(_32x32_TextureIds_[2]);
            blueStarMesh.SetPositionZ(-0.36f);
            blueStarMesh.AddRelativeRotationY(-glm::pi<float>());
        }
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
    _FAIL_CHECK(Project001::FreetypeTextLoader::LoadTextureDataAndFontData(
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
        "This Scene tests performing transformations on Models.\n"
        "Models are composed of many Meshes.\n"
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

void TestScene034::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    _LOG_MESSAGE("DEINITIALIZING: TestScene034:            %u", GetId());

    // Texture Data ------------------------------------------------------------

    thonk001_TextureId_ = (unsigned int)-1;

    thonkSpecular001_TextureId_ = (unsigned int)-1;

    _32x32_TextureIds_.clear();

    // Mesh Data ---------------------------------------------------------------

    // dont need to delete these here since they are added to meshDataPtrArray_

    cube001_MeshDataPtr_ = nullptr;
    cube001_MeshId_ = (unsigned int)-1;
    cube001_MaxBoundingRadius_ = 0.0f;

    line001_MeshDataPtr_ = nullptr;
    line001_MeshId_ = (unsigned int)-1;
    line001_MaxBoundingRadius_ = 0.0f;

    cone001_MeshDataPtr_ = nullptr;
    cone001_MeshId_ = (unsigned int)-1;
    cone001_MaxBoundingRadius_ = 0.0f;
}