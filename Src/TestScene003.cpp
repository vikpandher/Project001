#include "TestScene003.h"

#include "Engine/Components/Camera.h"
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
    TestSceneFramework::Initialize();

    // Load meshes
    // -------------------------------------------------------------------------

    _FAIL_CHECK(meshStoresPtr_->GenerateBox(shape01MeshIndex_, 0.64f, 0.64f, 0.64f, false));
    // _FAIL_CHECK(meshStoresPtr_->LoadMeshOBJ(shape01MeshIndex_, "../Models/Cube.obj", true, true, false));
    // _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape01MeshIndex_));

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

    // reposition camera
    {
        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
        cameraPtr->SetPosition(0.0f, 0.0f, 2.0f);
    }

    // generated shape entity 01
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape01EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape01EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape01EntityId_, renderedModelPtr));
        // renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape01MeshIndex_);
    }
}

void TestScene003::Deinitialize()
{
    TestSceneFramework::Deinitialize();

    ClearIndiciesAndEntityIds();
}

void TestScene003::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene003::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneFramework::OnEvent(event);
}

// protected: ------------------------------------------------------------------

void TestScene003::ClearIndiciesAndEntityIds()
{
    shape01MeshIndex_ = (unsigned int)-1;

    shape01EntityId_ = (unsigned int)-1;
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
            SendEvent(Project001::SwitchSceneEvent("TestScene001"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene001"));
            }
        }
    }
}

