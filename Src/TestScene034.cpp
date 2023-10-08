#include "TestScene034.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/RenderedModel.h"
#include "Engine/Math/VectorUtilities.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/Renderer.h"
#include "Engine/TextureLoader.h"
#include "Engine/Window.h"



// public ----------------------------------------------------------------------

TestScene034::TestScene034()
{
    ClearResources();
}

TestScene034::~TestScene034()
{}

const char* TestScene034::Name()
{
    return "TestScene034";
}

// protected -------------------------------------------------------------------

bool TestScene034::OnInitialize()
{
    bool success = TestSceneBase001::OnInitialize();

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

        cube001_MaxRadius_ = cube001_MeshDataPtr_->maxRadius;
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

        line001_MaxRadius_ = line001_MeshDataPtr_->maxRadius;
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

        cone001_MaxRadius_ = cone001_MeshDataPtr_->maxRadius;
    }
 
    // Generate entities
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
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr);
        renderedModelPtr->SetPosition(positions[i]);
        renderedModelPtr->SetOrientation(orientations[i]);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        renderedMeshes.emplace_back();
        Project001::RenderedMesh& cubeMesh = renderedMeshes.back();
        cubeMesh.SetMeshId(cube001_MeshId_);
        cubeMesh.SetMaxRadius(cube001_MeshDataPtr_->maxRadius);
        cubeMesh.SetTextureId(thonk001_TextureId_);
        cubeMesh.SetSpecularId(thonkSpecular001_TextureId_);
        cubeMesh.SetShininess(32.0f);

        renderedMeshes.emplace_back();
        Project001::RenderedMesh& redConeMesh = renderedMeshes.back();
        redConeMesh.SetMeshId(cone001_MeshId_);
        redConeMesh.SetMaxRadius(cone001_MeshDataPtr_->maxRadius);
        redConeMesh.SetColor(0.8f, 0.2f, 0.2f, 0.4f);
        redConeMesh.SetTranslucent(true);
        redConeMesh.SetPositionX(0.64f);
        redConeMesh.AddRelativeRotationZ(-glm::half_pi<float>());

        renderedMeshes.emplace_back();
        Project001::RenderedMesh& redStarMesh = renderedMeshes.back();
        redStarMesh.SetMeshId(line001_MeshId_);
        redStarMesh.SetMaxRadius(line001_MeshDataPtr_->maxRadius);
        redStarMesh.SetColor(0.8f, 0.2f, 0.2f, 1.0f);
        redStarMesh.SetTextureId(_32x32_TextureIds_[0]);
        redStarMesh.SetPositionX(-0.36f);
        redStarMesh.AddRelativeRotationY(-glm::half_pi<float>());

        renderedMeshes.emplace_back();
        Project001::RenderedMesh& greenConeMesh = renderedMeshes.back();
        greenConeMesh.SetMeshId(cone001_MeshId_);
        greenConeMesh.SetMaxRadius(cone001_MeshDataPtr_->maxRadius);
        greenConeMesh.SetColor(0.2f, 0.8f, 0.2f, 0.4f);
        greenConeMesh.SetTranslucent(true);
        greenConeMesh.SetPositionY(0.64f);

        renderedMeshes.emplace_back();
        Project001::RenderedMesh& greenStarMesh = renderedMeshes.back();
        greenStarMesh.SetMeshId(line001_MeshId_);
        greenStarMesh.SetMaxRadius(line001_MeshDataPtr_->maxRadius);
        greenStarMesh.SetColor(0.2f, 0.8f, 0.2f, 1.0f);
        greenStarMesh.SetTextureId(_32x32_TextureIds_[1]);
        greenStarMesh.SetPositionY(-0.36f);
        greenStarMesh.AddRelativeRotationX(glm::half_pi<float>());

        renderedMeshes.emplace_back();
        Project001::RenderedMesh& blueConeMesh = renderedMeshes.back();
        blueConeMesh.SetMeshId(cone001_MeshId_);
        blueConeMesh.SetMaxRadius(cone001_MeshDataPtr_->maxRadius);
        blueConeMesh.SetColor(0.2f, 0.2f, 0.8f, 0.4f);
        blueConeMesh.SetTranslucent(true);
        blueConeMesh.SetPositionZ(0.64f);
        blueConeMesh.AddRelativeRotationX(glm::half_pi<float>());

        renderedMeshes.emplace_back();
        Project001::RenderedMesh& blueStarMesh = renderedMeshes.back();
        blueStarMesh.SetMeshId(line001_MeshId_);
        blueStarMesh.SetMaxRadius(line001_MeshDataPtr_->maxRadius);
        blueStarMesh.SetColor(0.2f, 0.2f, 0.8f, 1.0f);
        blueStarMesh.SetTextureId(_32x32_TextureIds_[2]);
        blueStarMesh.SetPositionZ(-0.36f);
        blueStarMesh.AddRelativeRotationY(-glm::pi<float>());
    }

    return success && true;
}

bool TestScene034::OnDeinitialize()
{
    bool success = TestSceneBase001::OnDeinitialize();

    ClearResources();

    return success && true;
}

void TestScene034::OnHandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene034::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneBase001::OnHandleEvent(event);
}

void TestScene034::ClearResources()
{
    // Texture Data ------------------------------------------------------------

    thonk001_TextureId_ = (unsigned int)-1;

    thonkSpecular001_TextureId_ = (unsigned int)-1;

    _32x32_TextureIds_.clear();

    // Mesh Data ---------------------------------------------------------------

    cube001_MeshDataPtr_ = nullptr;
    cube001_MeshId_ = (unsigned int)-1;
    cube001_MaxRadius_ = 0.0f;

    line001_MeshDataPtr_ = nullptr;
    line001_MeshId_ = (unsigned int)-1;
    line001_MaxRadius_ = 0.0f;

    cone001_MeshDataPtr_ = nullptr;
    cone001_MeshId_ = (unsigned int)-1;
    cone001_MaxRadius_ = 0.0f;
}

void TestScene034::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene050"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene050"));
            }
        }
    }
}
