#include "TestScene030.h"

#include "Engine/Components/RenderedModel.h"
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
    : meshDataPtr00_(nullptr)
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
    meshDataPtr00_ = new Project001::MeshData();
    meshDataPtrArray_.push_back(meshDataPtr00_);
    glm::vec3 min(-0.04f, -0.04f, -0.04f);
    glm::vec3 max(0.04f, 0.04f, 0.04f);
    _FAIL_CHECK(Project001::MeshLoader::GenerateBox(*meshDataPtr00_, min, max, false));

    // Calculating positions
    // -------------------------------------------------------------------------

    std::vector<glm::vec3> modelEntityPositions;
    int rows = 10;
    int columns = 10;
    int depthRows = 10;

    for (int i = rows / -2; i < (rows + 1) / 2; ++i)
    {
        for (int j = columns / -2; j < (columns + 1) / 2; ++j)
        {
            for (int k = depthRows / -2; k < (depthRows + 1) / 2; ++k)
            {
                modelEntityPositions.emplace_back(0.1f * i, 0.1f * j, 0.1f * k);
            }
        }
    }

    // Generate entities
    // -------------------------------------------------------------------------
    for (size_t i = 0; i < modelEntityPositions.size(); i++)
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[i];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(meshDataPtr00_);
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
{
    delete meshDataPtr00_;
}

void TestScene030::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene002"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene002"));
            }
        }
    }
}
