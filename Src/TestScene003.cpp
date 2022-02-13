#include "TestScene003.h"

#include "Engine/Components/RenderedModel.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshStores.h"
#include "Engine/Renderer.h"
#include "Engine/TextureStores.h"
#include "Engine/Window.h"



// public: ---------------------------------------------------------------------

TestScene003::TestScene003()
{
    ClearIndiciesAndEntityIds();
}

TestScene003::~TestScene003()
{}

const char* TestScene003::Name()
{
    return "TestScene003";
}

void TestScene003::Initialize()
{
    TestSceneBase001::Initialize();

    // Load meshes
    // -------------------------------------------------------------------------

    {
        // duplicate position at start
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        positions.emplace_back(0.0f, 0.48f);
        positions.emplace_back(0.0f, 0.64f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape01MeshIndex_, positions, 0.24f));
    }

    {
        // duplicate position at end
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.16f, 0.0f);
        positions.emplace_back(0.16f, 0.16f);
        positions.emplace_back(0.16f, 0.32f);
        positions.emplace_back(0.16f, 0.64f);
        positions.emplace_back(0.16f, 0.64f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape02MeshIndex_, positions, 0.24f));
    }

    {
        // duplicate position in middle
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.32f, 0.16f);
        positions.emplace_back(0.32f, 0.32f);
        positions.emplace_back(0.32f, 0.32f);
        positions.emplace_back(0.32f, 0.64f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape03MeshIndex_, positions, 0.24f));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.32f, -0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.32f, 0.32f); // same slope
        positions.emplace_back(-0.32f, -0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape04MeshIndex_, positions, 0.24f));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, 0.32f);
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.32f, -0.32f); // same slope
        positions.emplace_back(-0.32f, -0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape05MeshIndex_, positions, 0.24f));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, -0.32f);
        positions.emplace_back(0.0f, -0.32f);
        positions.emplace_back(0.32f, -0.32f); // same slope
        positions.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape06MeshIndex_, positions, 0.24f));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, 0.0f);
        positions.emplace_back(0.0f, -0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(shape07MeshIndex_, positions, 0.24f));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.32f, -0.32f);
        positions.emplace_back(0.32f, 0.32f);
        positions.emplace_back(-0.32f, -0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLineLoop(shape08MeshIndex_, positions, 0.24f));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, 0.32f);
        positions.emplace_back(0.32f, -0.32f);
        positions.emplace_back(-0.32f, -0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLineLoop(shape09MeshIndex_, positions, 0.24f));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, -0.32f);
        positions.emplace_back(0.32f, -0.32f);
        positions.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLineLoop(shape10MeshIndex_, positions, 0.24f));
    }

    {
        std::vector<glm::vec2> positions;
        positions.emplace_back(-0.32f, 0.0f);
        positions.emplace_back(0.0f, -0.32f);
        positions.emplace_back(0.32f, 0.0f);
        positions.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLineLoop(shape11MeshIndex_, positions, 0.24f));
    }

    {
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
        _FAIL_CHECK(meshStoresPtr_->Generate2DLineLoop(shape12MeshIndex_, positions, 0.04f));
    }

    // Load textures
    // -------------------------------------------------------------------------

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(_100x100TextureIndex_, "../Textures/100x100_2.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(_100x100TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(_100x100TextureIndex_, 4, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(earthTextureIndex_, "../Textures/earth.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(earthTextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(earthTextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(rgb120x60TextureIndex_, "../Textures/120_60_rgb.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(rgb120x60TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(rgb120x60TextureIndex_, 2, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    // Calculating positions
    // -------------------------------------------------------------------------

    std::vector<glm::vec3> modelEntityPositions;
    for (int i = 2; i >= -2; --i)
    {
        for (int j = -3; j <= 3; ++j)
        {
            modelEntityPositions.emplace_back((float)j, (float)i, 0.0f);
        }
    }
    size_t positionPosition = 0;

    // generated shape entity 01
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape01EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape01EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape01EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape01MeshIndex_);
    }

    // generated shape entity 02
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape02EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape02EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape02EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape02MeshIndex_);
    }

    // generated shape entity 03
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape03EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape03EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape03EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape03MeshIndex_);
    }

    // generated shape entity 04
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape04EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape04EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape04EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape04MeshIndex_);
    }

    // generated shape entity 05
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape05EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape05EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape05EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape05MeshIndex_);
    }

    // generated shape entity 06
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape06EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape06EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape06EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape06MeshIndex_);
    }

    // generated shape entity 07
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape07EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape07EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape07EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape07MeshIndex_);
    }

    // generated shape entity 08
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape08EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape08EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape08EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape08MeshIndex_);
    }

    // generated shape entity 09
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape09EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape09EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape09EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape09MeshIndex_);
    }

    // generated shape entity 10
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape10EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape10EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape10EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape10MeshIndex_);
    }

    // generated shape entity 11
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape11EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape11EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape11EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape11MeshIndex_);
    }

    // generated shape entity 12
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape12EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape12EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape12EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape12MeshIndex_);
    }
}

void TestScene003::Deinitialize()
{
    TestSceneBase001::Deinitialize();

    ClearIndiciesAndEntityIds();
}

void TestScene003::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene003::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneBase001::OnEvent(event);
}

// protected: ------------------------------------------------------------------

void TestScene003::ClearIndiciesAndEntityIds()
{
    shape01MeshIndex_ = (unsigned int)-1;
    shape02MeshIndex_ = (unsigned int)-1;
    shape03MeshIndex_ = (unsigned int)-1;
    shape04MeshIndex_ = (unsigned int)-1;
    shape05MeshIndex_ = (unsigned int)-1;
    shape06MeshIndex_ = (unsigned int)-1;
    shape07MeshIndex_ = (unsigned int)-1;
    shape08MeshIndex_ = (unsigned int)-1;
    shape09MeshIndex_ = (unsigned int)-1;
    shape10MeshIndex_ = (unsigned int)-1;
    shape11MeshIndex_ = (unsigned int)-1;
    shape12MeshIndex_ = (unsigned int)-1;
    shape13MeshIndex_ = (unsigned int)-1;
    shape14MeshIndex_ = (unsigned int)-1;
    shape15MeshIndex_ = (unsigned int)-1;
    shape16MeshIndex_ = (unsigned int)-1;
    shape17MeshIndex_ = (unsigned int)-1;
    shape18MeshIndex_ = (unsigned int)-1;
    shape19MeshIndex_ = (unsigned int)-1;
    shape20MeshIndex_ = (unsigned int)-1;
    shape21MeshIndex_ = (unsigned int)-1;
    shape22MeshIndex_ = (unsigned int)-1;
    shape23MeshIndex_ = (unsigned int)-1;
    shape24MeshIndex_ = (unsigned int)-1;
    shape25MeshIndex_ = (unsigned int)-1;
    shape26MeshIndex_ = (unsigned int)-1;
    shape27MeshIndex_ = (unsigned int)-1;
    shape28MeshIndex_ = (unsigned int)-1;
    shape29MeshIndex_ = (unsigned int)-1;
    shape30MeshIndex_ = (unsigned int)-1;
    shape31MeshIndex_ = (unsigned int)-1;
    shape32MeshIndex_ = (unsigned int)-1;
    shape33MeshIndex_ = (unsigned int)-1;
    shape34MeshIndex_ = (unsigned int)-1;
    shape35MeshIndex_ = (unsigned int)-1;

    _100x100TextureIndex_ = (unsigned int)-1;
    earthTextureIndex_ = (unsigned int)-1;
    rgb120x60TextureIndex_ = (unsigned int)-1;

    shape01EntityId_ = (unsigned int)-1;
    shape02EntityId_ = (unsigned int)-1;
    shape03EntityId_ = (unsigned int)-1;
    shape04EntityId_ = (unsigned int)-1;
    shape05EntityId_ = (unsigned int)-1;
    shape06EntityId_ = (unsigned int)-1;
    shape07EntityId_ = (unsigned int)-1;
    shape08EntityId_ = (unsigned int)-1;
    shape09EntityId_ = (unsigned int)-1;
    shape10EntityId_ = (unsigned int)-1;
    shape11EntityId_ = (unsigned int)-1;
    shape12EntityId_ = (unsigned int)-1;
    shape13EntityId_ = (unsigned int)-1;
    shape14EntityId_ = (unsigned int)-1;
    shape15EntityId_ = (unsigned int)-1;
    shape16EntityId_ = (unsigned int)-1;
    shape17EntityId_ = (unsigned int)-1;
    shape18EntityId_ = (unsigned int)-1;
    shape19EntityId_ = (unsigned int)-1;
    shape20EntityId_ = (unsigned int)-1;
    shape21EntityId_ = (unsigned int)-1;
    shape22EntityId_ = (unsigned int)-1;
    shape23EntityId_ = (unsigned int)-1;
    shape24EntityId_ = (unsigned int)-1;
    shape25EntityId_ = (unsigned int)-1;
    shape26EntityId_ = (unsigned int)-1;
    shape27EntityId_ = (unsigned int)-1;
    shape28EntityId_ = (unsigned int)-1;
    shape29EntityId_ = (unsigned int)-1;
    shape30EntityId_ = (unsigned int)-1;
    shape31EntityId_ = (unsigned int)-1;
    shape32EntityId_ = (unsigned int)-1;
    shape33EntityId_ = (unsigned int)-1;
    shape34EntityId_ = (unsigned int)-1;
    shape35EntityId_ = (unsigned int)-1;
}

void TestScene003::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene004"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene004"));
            }
        }
    }
}
