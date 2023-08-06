#include "TestScene031.h"

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

TestScene031::TestScene031()
{
    ClearResources();
}

TestScene031::~TestScene031()
{}

const char* TestScene031::Name()
{
    return "TestScene031";
}

// protected -------------------------------------------------------------------

bool TestScene031::OnInitialize()
{
    bool success = TestSceneBase001::OnInitialize();

    // Generate mesh
    // -------------------------------------------------------------------------
    Project001::MeshData* meshDataPtr00 = new Project001::MeshData();
    meshDataPtrArray_.push_back(meshDataPtr00);
    glm::vec3 min(-0.04f, -0.04f, -0.04f);
    glm::vec3 max(0.04f, 0.04f, 0.04f);
    _FAIL_CHECK(Project001::MeshLoader::GenerateBox(*meshDataPtr00, min, max, false));

    unsigned int meshId00;
    rendererPtr_->CreateMesh(
        meshId00,
        meshDataPtr00->meshVertexArray.data(),
        meshDataPtr00->meshVertexArray.size(),
        meshDataPtr00->meshIndexArray.data(),
        meshDataPtr00->meshIndexArray.size()
    );

    float maxRadius00 = meshDataPtr00->maxRadius;

    // Calculating positions
    // -------------------------------------------------------------------------

    std::vector<glm::vec3> modelEntityPositions;
    int columns = 50;
    int rows = 30;
    int depthRows = 10;

    for (int i = columns / -2; i < (columns + 1) / 2; ++i)
    {
        for (int j = rows / -2; j < (rows + 1) / 2; ++j)
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
        renderedModelPtr->SetColor(
            0.5f + currentPosition.x * 0.1f,
            0.5f + currentPosition.y * 0.1f,
            0.5f + currentPosition.z * 0.1f,
            0.5f
        );
        renderedModelPtr->SetMeshId(meshId00);
        renderedModelPtr->SetTranslucent(true);
        renderedModelPtr->SetMaxRadius(maxRadius00);
    }

    return success && true;
}

bool TestScene031::OnDeinitialize()
{
    bool success = TestSceneBase001::OnDeinitialize();

    ClearResources();

    return success && true;
}

void TestScene031::OnHandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene031::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneBase001::OnHandleEvent(event);
}

void TestScene031::ClearResources()
{}

void TestScene031::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene032"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene032"));
            }
        }
    }
}
