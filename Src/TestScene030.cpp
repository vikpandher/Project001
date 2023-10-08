#include "TestScene030.h"

#include "Engine/Components/RenderedMesh.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/Renderer.h"
#include "Engine/TextureLoader.h"
#include "Engine/Window.h"



// public ----------------------------------------------------------------------

TestScene030::TestScene030()
{
    ClearResources();
}

TestScene030::~TestScene030()
{}

const char* TestScene030::Name()
{
    return "TestScene030";
}

// protected -------------------------------------------------------------------

bool TestScene030::OnInitialize()
{
    bool success = TestSceneBase001::OnInitialize();

    // Generate mesh
    // -------------------------------------------------------------------------
    Project001::MeshData* meshDataPtr00_ = new Project001::MeshData();
    meshDataPtrArray_.push_back(meshDataPtr00_);
    glm::vec3 min(-0.04f, -0.04f, -0.04f);
    glm::vec3 max(0.04f, 0.04f, 0.04f);
    _FAIL_CHECK(Project001::MeshLoader::GenerateBox(*meshDataPtr00_, min, max, false));

    // Calculating positions
    // -------------------------------------------------------------------------

    std::vector<glm::vec3> meshEntityPositions;
    int columns = 50;
    int rows = 30;
    int depthRows = 10;

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

    // Generate entities
    // -------------------------------------------------------------------------
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
        renderedMeshPtr->SetMeshDataPtr(meshDataPtr00_);
        renderedMeshPtr->SetTranslucent(true);
    }

    return success && true;
}

bool TestScene030::OnDeinitialize()
{
    bool success = TestSceneBase001::OnDeinitialize();

    ClearResources();

    return success && true;
}

void TestScene030::OnHandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene030::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneBase001::OnHandleEvent(event);
}

void TestScene030::ClearResources()
{}

void TestScene030::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene031"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene031"));
            }
        }
    }
}
