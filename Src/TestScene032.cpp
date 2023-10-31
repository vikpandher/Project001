#include "TestScene032.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/RenderedMesh.h"
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

TestScene032::TestScene032()
{
    ClearResources();
}

TestScene032::~TestScene032()
{}

const char* TestScene032::Name()
{
    return "TestScene032";
}

// protected -------------------------------------------------------------------

bool TestScene032::OnInitialize()
{
    bool success = TestSceneBase001::OnInitialize();

    // Load Textures
    // -------------------------------------------------------------------------

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/earth.png"));
        rendererPtr_->CreateTexture(earth001_TextureId_, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/Specular1.png"));
        rendererPtr_->CreateTexture(specular001_TextureId_, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
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

    { // icosphere
        icosphere001_MeshDataPtr_ = new Project001::MeshData();
        meshDataPtrArray_.push_back(icosphere001_MeshDataPtr_);
        _FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(*icosphere001_MeshDataPtr_, 0.64f, 1, false));

        rendererPtr_->CreateMesh(
            icosphere001_MeshId_,
            icosphere001_MeshDataPtr_->meshVertexArray.data(),
            (unsigned int)icosphere001_MeshDataPtr_->meshVertexArray.size(),
            icosphere001_MeshDataPtr_->meshIndexArray.data(),
            (unsigned int)icosphere001_MeshDataPtr_->meshIndexArray.size()
        );

        icosphere001_MaxBoundingRadius_ = icosphere001_MeshDataPtr_->maxBoundingRadius;
    }

    { // arc
        arc001_MeshDataPtr_ = new Project001::MeshData();
        meshDataPtrArray_.push_back(arc001_MeshDataPtr_);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*arc001_MeshDataPtr_, 0.08f, 0.32f, 24, glm::pi<float>(), 0.5f * glm::pi<float>()));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*arc001_MeshDataPtr_);
        Project001::MeshLoader::TranslateTextureCoordinates(*arc001_MeshDataPtr_, glm::vec2(0.5f, 0.5f));

        rendererPtr_->CreateMesh(
            arc001_MeshId_,
            arc001_MeshDataPtr_->meshVertexArray.data(),
            (unsigned int)arc001_MeshDataPtr_->meshVertexArray.size(),
            arc001_MeshDataPtr_->meshIndexArray.data(),
            (unsigned int)arc001_MeshDataPtr_->meshIndexArray.size()
        );

        arc001_MaxBoundingRadius_ = arc001_MeshDataPtr_->maxBoundingRadius;
    }

    { // line star
        glm::vec2 topPosition(0.0f, 0.96f);
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
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(*line001_MeshDataPtr_, positions, 0.24f));
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

    // Generate entities
    // -------------------------------------------------------------------------

    { // icosphere at the center (GPU side mesh)
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(centerIcosphereEntityId_));

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(centerIcosphereEntityId_));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(centerIcosphereEntityId_, renderedMeshPtr));
        renderedMeshPtr->SetMeshId(icosphere001_MeshId_);
        renderedMeshPtr->SetMaxBoundingRadius(icosphere001_MeshDataPtr_->maxBoundingRadius);
        renderedMeshPtr->SetColor(1.0f, 1.0f, 0.0f, 0.5f);
        renderedMeshPtr->SetTranslucent(true);
    }

    { // star at the center top (CPU side mesh)
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(centerStar001_EntityId_));

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(centerStar001_EntityId_));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(centerStar001_EntityId_, renderedMeshPtr));
        renderedMeshPtr->SetMeshDataPtr(line001_MeshDataPtr_);
        renderedMeshPtr->SetPosition(0.0f, 1.0f, 1.0f);
        renderedMeshPtr->SetColor(0.2f, 0.8f, 0.8f, 0.5f);
        renderedMeshPtr->SetTranslucent(true);
    }

    { // star at the center bottom (CPU side mesh)
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(centerStar002_EntityId_));

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(centerStar002_EntityId_));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(centerStar002_EntityId_, renderedMeshPtr));
        renderedMeshPtr->SetMeshDataPtr(line001_MeshDataPtr_);
        renderedMeshPtr->SetPosition(0.0f, -1.0f, -1.0f);
        renderedMeshPtr->SetColor(0.2f, 0.2f, 0.8f, 1.0f);
    }

    { // stencils out stuff positioned behind it, that is rendered after it (CPU side mesh)
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(stencil001_EntityId_));

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(stencil001_EntityId_));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(stencil001_EntityId_, renderedMeshPtr));
        renderedMeshPtr->SetMeshDataPtr(arc001_MeshDataPtr_);
        renderedMeshPtr->SetPosition(1.0f, 1.0f, 5.0f);
        renderedMeshPtr->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));
        renderedMeshPtr->SetColor(0.8f, 0.6f, 0.2f, 0.2f);
        renderedMeshPtr->SetTranslucent(true);
        renderedMeshPtr->SetRenderPriorityOverride(-100);
    }

    { // icosphere group (GPU side mesh)
        std::vector<glm::vec3> positions;
        int firstX = 1;
        int lastX = 3;
        int firstY = 1;
        int lastY = 3;
        int firstZ = -2;
        int lastZ = 0;

        for (int i = firstX; i <= lastX; ++i)
        {
            for (int j = firstY; j <= lastY; ++j)
            {
                for (int k = firstZ; k <= lastZ; ++k)
                {
                    positions.emplace_back((float)i, (float)j, (float)k);
                }
            }
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            unsigned int newEntityId;
            _FAIL_CHECK(componentStoresPtr_->CreateEntity(newEntityId));
            icosphereEntityIds_.push_back(newEntityId);

            _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(newEntityId));
            Project001::RenderedMesh* renderedMeshPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(newEntityId, renderedMeshPtr));
            renderedMeshPtr->SetMeshId(icosphere001_MeshId_);
            renderedMeshPtr->SetTextureId(earth001_TextureId_);
            renderedMeshPtr->SetSpecularId(specular001_TextureId_);
            renderedMeshPtr->SetShininess(32.0f);
            renderedMeshPtr->SetMaxBoundingRadius(icosphere001_MaxBoundingRadius_);
            renderedMeshPtr->SetScale(0.5f, 0.5f, 0.5f);
            renderedMeshPtr->SetPosition(positions[i]);
        }
    }

    { // red arc group (CPU Side)
        std::vector<glm::vec3> positions;
        int firstX = -3;
        int lastX = -1;
        int firstY = -2;
        int lastY = 0;
        int firstZ = 0;
        int lastZ = 2;

        for (int i = firstX; i <= lastX; ++i)
        {
            for (int j = firstY; j <= lastY; ++j)
            {
                for (int k = firstZ; k <= lastZ; ++k)
                {
                    positions.emplace_back((float)i, (float)j, (float)k);
                }
            }
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            unsigned int newEntityId;
            _FAIL_CHECK(componentStoresPtr_->CreateEntity(newEntityId));
            arcEntityIds_.push_back(newEntityId);

            _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(newEntityId));
            Project001::RenderedMesh* renderedMeshPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(newEntityId, renderedMeshPtr));
            renderedMeshPtr->SetMeshDataPtr(arc001_MeshDataPtr_);
            renderedMeshPtr->SetColor(0.8f, 0.2f, 0.2f, 1.0f);
            renderedMeshPtr->SetPosition(positions[i]);
        }
    }

    { // blue arc group (GPU Side)
        std::vector<glm::vec3> positions;
        int firstX = 1;
        int lastX = 3;
        int firstY = -2;
        int lastY = 0;
        int firstZ = 0;
        int lastZ = 2;

        for (int i = firstX; i <= lastX; ++i)
        {
            for (int j = firstY; j <= lastY; ++j)
            {
                for (int k = firstZ; k <= lastZ; ++k)
                {
                    positions.emplace_back((float)i, (float)j, (float)k);
                }
            }
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            unsigned int newEntityId;
            _FAIL_CHECK(componentStoresPtr_->CreateEntity(newEntityId));
            arcEntityIds_.push_back(newEntityId);

            _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(newEntityId));
            Project001::RenderedMesh* renderedMeshPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(newEntityId, renderedMeshPtr));
            renderedMeshPtr->SetMeshId(arc001_MeshId_);
            renderedMeshPtr->SetMaxBoundingRadius(arc001_MaxBoundingRadius_);
            renderedMeshPtr->SetColor(0.0f, 0.0f, 1.0f, 0.5f);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetPosition(positions[i]);
        }
    }

    { // cone group (GPU side mesh)
        std::vector<glm::vec3> positions;
        int firstX = -3;
        int lastX = -1;
        int firstY = 1;
        int lastY = 3;
        int firstZ = -2;
        int lastZ = 0;

        for (int i = firstX; i <= lastX; ++i)
        {
            for (int j = firstY; j <= lastY; ++j)
            {
                for (int k = firstZ; k <= lastZ; ++k)
                {
                    positions.emplace_back((float)i, (float)j, (float)k);
                }
            }
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            unsigned int newEntityId;
            _FAIL_CHECK(componentStoresPtr_->CreateEntity(newEntityId));
            coneEntityIds_.push_back(newEntityId);

            _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(newEntityId));
            Project001::RenderedMesh* renderedMeshPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(newEntityId, renderedMeshPtr));
            renderedMeshPtr->SetMeshId(cone001_MeshId_);
            renderedMeshPtr->SetMaxBoundingRadius(cone001_MaxBoundingRadius_);
            renderedMeshPtr->SetPosition(positions[i]);
        }
    }

    { // star group in back (GPU side mesh)
        std::vector<glm::vec3> positions;
        int firstX = -2;
        int lastX = 2;
        int firstY = -2;
        int lastY = 2;
        for (int i = firstX; i <= lastX; ++i)
        {
            for (int j = firstY; j <= lastY; ++j)
            {
                positions.emplace_back((float)i, (float)j, -3.0f);
            }
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            unsigned int newEntityId;
            _FAIL_CHECK(componentStoresPtr_->CreateEntity(newEntityId));
            starEntityIds_.push_back(newEntityId);

            _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(newEntityId));
            Project001::RenderedMesh* renderedMeshPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(newEntityId, renderedMeshPtr));
            renderedMeshPtr->SetMeshId(line001_MeshId_);
            renderedMeshPtr->SetTextureId(_32x32_TextureIds_[i]);
            renderedMeshPtr->SetMaxBoundingRadius(line001_MaxBoundingRadius_);
            renderedMeshPtr->SetScale(0.5f, 0.5f, 0.5f);
            renderedMeshPtr->SetPosition(positions[i]);
            renderedMeshPtr->SetRenderPriorityOverride(-100);
        }
    }

    return success && true;
}

bool TestScene032::OnDeinitialize()
{
    bool success = TestSceneBase001::OnDeinitialize();

    ClearResources();

    return success && true;
}

void TestScene032::OnHandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene032::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneBase001::OnHandleEvent(event);
}

void TestScene032::ClearResources()
{
    earth001_TextureId_ = (unsigned int)-1;

    specular001_TextureId_ = (unsigned int)-1;

    _32x32_TextureIds_.clear();

    icosphere001_MeshDataPtr_ = nullptr;
    icosphere001_MeshId_ = (unsigned int)-1;
    icosphere001_MaxBoundingRadius_ = 0.0f;

    arc001_MeshDataPtr_ = nullptr;
    arc001_MeshId_ = (unsigned int)-1;
    arc001_MaxBoundingRadius_ = 0.0f;

    line001_MeshDataPtr_ = nullptr;
    line001_MeshId_ = (unsigned int)-1;
    line001_MaxBoundingRadius_ = 0.0f;

    cone001_MeshDataPtr_ = nullptr;
    cone001_MeshId_ = (unsigned int)-1;
    cone001_MaxBoundingRadius_ = 0.0f;

    centerIcosphereEntityId_ = (unsigned int)-1;

    centerStar001_EntityId_ = (unsigned int)-1;

    centerStar002_EntityId_ = (unsigned int)-1;

    stencil001_EntityId_ = (unsigned int)-1;

    icosphereEntityIds_.clear();

    arcEntityIds_.clear();

    coneEntityIds_.clear();

    starEntityIds_.clear();
}

void TestScene032::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene033"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene033"));
            }
        }
    }
}
