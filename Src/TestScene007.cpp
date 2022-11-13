#include "TestScene007.h"

#include "Engine/Components/RenderedModel.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/Renderer.h"
#include "Engine/TextureLoader.h"
#include "Engine/Window.h"



// public: ---------------------------------------------------------------------

TestScene007::TestScene007()
{
    ClearResources();
}

TestScene007::~TestScene007()
{}

const char* TestScene007::Name()
{
    return "TestScene007";
}

void TestScene007::Initialize()
{
    TestSceneBase001::Initialize();

    // Load textures
    // -------------------------------------------------------------------------

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/123456789abcdefghij.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(_32x32_123abc_TextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false));
    }

    // Calculating positions
    // -------------------------------------------------------------------------

    std::vector<glm::vec3> modelEntityPositions;
    modelEntityPositions.emplace_back(0.0f, 0.0f, 0.0f);
    // for (int i = 2; i >= -2; --i)
    // {
    //     for (int j = -3; j <= 3; ++j)
    //     {
    //         modelEntityPositions.emplace_back((float)j, (float)i, 0.0f);
    //     }
    // }
    size_t positionPosition = 0;

    // generated shape entity 01
    // -------------------------------------------------------------------------
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> positions;
        positions.emplace_back(0.0f, 0.0f);
        positions.emplace_back(0.32f, 0.0f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine_v2(*newMeshDataPtr, positions, 0.08f, 4));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::RecenterMesh(*newMeshDataPtr);

        // _LOG_MESSAGE("VertexCount = %i, IndexCount = %i", newMeshData.vertexCount, newMeshData.indexCount);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetTextureId(_32x32_123abc_TextureId_);
    }
}

void TestScene007::Deinitialize()
{
    TestSceneBase001::Deinitialize();

    ClearResources();
}

void TestScene007::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene007::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneBase001::OnEvent(event);
}

// protected: ------------------------------------------------------------------

void TestScene007::ClearResources()
{
    _32x32_123abc_TextureId_ = (unsigned int)-1;
}

void TestScene007::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene010"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene010"));
            }
        }
    }
}
