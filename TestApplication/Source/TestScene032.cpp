// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-05-04

#include "TestScene032.h"

#include "TestApplicationData.h"
#include "TestResource_AntonioRegular_png.h"
#include "TestResource_AntonioRegular_ssf.h"

#include "Components/Camera.h"
#include "Components/RenderedMesh.h"
#include "Math/MathUtilities.h"
#include "Application.h"
#include "ComponentStores.h"
#include "FontLoader.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "Renderer.h"
#include "TextureLoader.h"
#include "Window.h"



// public ----------------------------------------------------------------------

TestScene032::TestScene032(Project001::Application* applicationPtr)
    : TestSceneBase001(applicationPtr)
    , instructionScene_(applicationPtr)
    , earth001_TextureId_((unsigned int)-1)
    , specular001_TextureId_((unsigned int)-1)
    , _32x32_TextureIds_()
    , icosphere001_MeshDataPtr_(nullptr)
    , icosphere001_MeshId_((unsigned int)-1)
    , icosphere001_MaxBoundingRadius_(0.0f)
    , arc001_MeshDataPtr_(nullptr)
    , arc001_MeshId_((unsigned int)-1)
    , arc001_MaxBoundingRadius_(0.0f)
    , line001_MeshDataPtr_(nullptr)
    , line001_MeshId_((unsigned int)-1)
    , line001_MaxBoundingRadius_(0.0f)
    , cone001_MeshDataPtr_(nullptr)
    , cone001_MeshId_((unsigned int)-1)
    , cone001_MaxBoundingRadius_(0.0f)
    , centerIcosphereEntityId_((unsigned int)-1)
    , centerStar001_EntityId_((unsigned int)-1)
    , centerStar002_EntityId_((unsigned int)-1)
    , psudoStencil001_EntityId_((unsigned int)-1)
    , icosphereEntityIds_()
    , arcEntityIds_()
    , coneEntityIds_()
    , starEntityIds_()
{
    GetSharedDataPtr<TestApplicationData>()->testScene032Id = GetId();
}

TestScene032::~TestScene032()
{}

void TestScene032::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene032::ProcessDeinitializeEvent, this, std::placeholders::_1));

    TestSceneBase001::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene032::ProcessInitializeEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene032::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene032:            " << GetId());

    // Load Textures
    // -------------------------------------------------------------------------

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/earth.png"));
        GetRendererPtr()->CreateTexture(earth001_TextureId_, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/Specular1.png"));
        GetRendererPtr()->CreateTexture(specular001_TextureId_, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
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
        FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, filePath));
        unsigned int tempTextureId = (unsigned int)-1;
        GetRendererPtr()->CreateTexture(tempTextureId, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
        _32x32_TextureIds_.push_back(tempTextureId);
    }

    // Generate meshes
    // -------------------------------------------------------------------------

    { // icosphere
        icosphere001_MeshDataPtr_ = new Project001::MeshData();
        meshDataPtrArray_.push_back(icosphere001_MeshDataPtr_);
        FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(*icosphere001_MeshDataPtr_, 0.64f, 1, false));

        GetRendererPtr()->CreateMesh(
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
        FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*arc001_MeshDataPtr_, 0.08f, 0.32f, 24, glm::pi<float>(), 0.5f * glm::pi<float>()));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*arc001_MeshDataPtr_);
        Project001::MeshLoader::TranslateTextureCoordinates(*arc001_MeshDataPtr_, glm::vec2(0.5f, 0.5f));

        GetRendererPtr()->CreateMesh(
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
        FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(*line001_MeshDataPtr_, positions, 0.24f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*line001_MeshDataPtr_);
        Project001::MeshLoader::RecenterMesh(*line001_MeshDataPtr_);

        GetRendererPtr()->CreateMesh(
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
        FAIL_CHECK(Project001::MeshLoader::GenerateCone(*cone001_MeshDataPtr_, 0.64f, 0.32f, 8, false));

        GetRendererPtr()->CreateMesh(
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
        GetComponentStoresPtr()->CreateEntity(centerIcosphereEntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(centerIcosphereEntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, centerIcosphereEntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetMeshIdAndMaxBoundingRadius(icosphere001_MeshId_, icosphere001_MaxBoundingRadius_);
            renderedMeshPtr->SetColor(1.0f, 1.0f, 0.0f, 0.5f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    { // star at the center top (CPU side mesh)
        GetComponentStoresPtr()->CreateEntity(centerStar001_EntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(centerStar001_EntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, centerStar001_EntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetMeshDataPtr(line001_MeshDataPtr_);
            renderedMeshPtr->SetPosition(0.0f, 1.0f, 1.0f);
            renderedMeshPtr->SetColor(0.2f, 0.8f, 0.8f, 0.5f);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    { // star at the center bottom (CPU side mesh)
        GetComponentStoresPtr()->CreateEntity(centerStar002_EntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(centerStar002_EntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, centerStar002_EntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetMeshDataPtr(line001_MeshDataPtr_);
            renderedMeshPtr->SetPosition(0.0f, -1.0f, -1.0f);
            renderedMeshPtr->SetColor(0.2f, 0.2f, 0.8f, 1.0f);
        }
    }

    { // stencils out stuff positioned behind it, that is rendered after it (CPU side mesh)
        GetComponentStoresPtr()->CreateEntity(psudoStencil001_EntityId_);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(psudoStencil001_EntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, psudoStencil001_EntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetMeshDataPtr(arc001_MeshDataPtr_);
            renderedMeshPtr->SetPosition(1.0f, 1.0f, 5.0f);
            renderedMeshPtr->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));
            renderedMeshPtr->SetColor(0.8f, 0.6f, 0.2f, 0.2f);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetRenderPriorityOverride(-100);
        }
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
            GetComponentStoresPtr()->CreateEntity(newEntityId);
            icosphereEntityIds_.push_back(newEntityId);

            FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(newEntityId));
            Project001::RenderedMesh* renderedMeshPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, newEntityId));
            if (renderedMeshPtr != nullptr)
            {
                renderedMeshPtr->SetMeshIdAndMaxBoundingRadius(icosphere001_MeshId_, icosphere001_MaxBoundingRadius_);
                renderedMeshPtr->SetTextureId(earth001_TextureId_);
                renderedMeshPtr->SetSpecularId(specular001_TextureId_);
                renderedMeshPtr->SetShininess(32.0f);
                renderedMeshPtr->SetScale(0.5f, 0.5f, 0.5f);
                renderedMeshPtr->SetPosition(positions[i]);
            }
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
            GetComponentStoresPtr()->CreateEntity(newEntityId);
            arcEntityIds_.push_back(newEntityId);

            FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(newEntityId));
            Project001::RenderedMesh* renderedMeshPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, newEntityId));
            if (renderedMeshPtr != nullptr)
            {
                renderedMeshPtr->SetMeshDataPtr(arc001_MeshDataPtr_);
                renderedMeshPtr->SetColor(0.8f, 0.2f, 0.2f, 1.0f);
                renderedMeshPtr->SetPosition(positions[i]);
            }
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
            GetComponentStoresPtr()->CreateEntity(newEntityId);
            arcEntityIds_.push_back(newEntityId);

            FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(newEntityId));
            Project001::RenderedMesh* renderedMeshPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, newEntityId));
            if (renderedMeshPtr != nullptr)
            {
                renderedMeshPtr->SetMeshIdAndMaxBoundingRadius(arc001_MeshId_, arc001_MaxBoundingRadius_);
                renderedMeshPtr->SetColor(0.0f, 0.0f, 1.0f, 0.5f);
                renderedMeshPtr->SetTranslucent(true);
                renderedMeshPtr->SetPosition(positions[i]);
            }
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
            GetComponentStoresPtr()->CreateEntity(newEntityId);
            coneEntityIds_.push_back(newEntityId);

            FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(newEntityId));
            Project001::RenderedMesh* renderedMeshPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, newEntityId));
            if (renderedMeshPtr != nullptr)
            {
                renderedMeshPtr->SetMeshIdAndMaxBoundingRadius(cone001_MeshId_, cone001_MaxBoundingRadius_);
                renderedMeshPtr->SetPosition(positions[i]);
            }
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
            GetComponentStoresPtr()->CreateEntity(newEntityId);
            starEntityIds_.push_back(newEntityId);

            FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(newEntityId));
            Project001::RenderedMesh* renderedMeshPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, newEntityId));
            if (renderedMeshPtr != nullptr)
            {
                renderedMeshPtr->SetMeshIdAndMaxBoundingRadius(line001_MeshId_, line001_MaxBoundingRadius_);
                renderedMeshPtr->SetTextureId(_32x32_TextureIds_[i]);
                renderedMeshPtr->SetScale(0.5f, 0.5f, 0.5f);
                renderedMeshPtr->SetPosition(positions[i]);
                renderedMeshPtr->SetRenderPriorityOverride(-100);
            }
        }
    }

    // Member Scenes -----------------------------------------------------------

    Project001::FontData font01_FontData;
    FAIL_CHECK(Project001::FontLoader::LoadFontDataFromMemory(
        font01_FontData,
        g_AntonioRegular_ssf,
        sizeof(g_AntonioRegular_ssf)
    ));

    Project001::TextureData font01_TextureData;
    FAIL_CHECK(Project001::TextureLoader::LoadTextureFromMemory(
        font01_TextureData,
        g_AntonioRegular_png,
        sizeof(g_AntonioRegular_png)
    ));
    unsigned int font01_TextureId = (unsigned int)-1;
    GetRendererPtr()->CreateTexture(
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
        "This Scene tests rendering Batched and Instanced Meshes\n"
        "with various rendering priorities. Some are translucent.\n"
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

void TestScene032::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene032:            " << GetId());

    // Texture Data ------------------------------------------------------------

    earth001_TextureId_ = (unsigned int)-1;
    specular001_TextureId_ = (unsigned int)-1;
    _32x32_TextureIds_.clear();

    // Mesh Data ---------------------------------------------------------------

    // dont need to delete these here since they are added to meshDataPtrArray_

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

    // Entity Ids --------------------------------------------------------------

    centerIcosphereEntityId_ = (unsigned int)-1;
    centerStar001_EntityId_ = (unsigned int)-1;
    centerStar002_EntityId_ = (unsigned int)-1;
    psudoStencil001_EntityId_ = (unsigned int)-1;
    icosphereEntityIds_.clear();
    arcEntityIds_.clear();
    coneEntityIds_.clear();
    starEntityIds_.clear();
}