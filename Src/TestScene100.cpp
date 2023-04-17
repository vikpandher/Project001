#include "TestScene100.h"

#include "TestComponents.h"

#include "Engine/Components/LightSource.h"
#include "Engine/Components/RenderedModel.h"
#include "Engine/Application.h"
#include "Engine/BiMap.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/Renderer.h"
#include "Engine/TextureLoader.h"
#include "Engine/Window.h"

#include <algorithm>



// public: ---------------------------------------------------------------------

TestScene100::TestScene100()
{
    ClearResources();
}

TestScene100::~TestScene100()
{}

const char* TestScene100::Name()
{
    return "TestScene100";
}

void TestScene100::Initialize()
{
    TestSceneBase001::Initialize();

    // Load meshes
    // -------------------------------------------------------------------------

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 3));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
    }

    // Load textures
    // -------------------------------------------------------------------------

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/cape-basket-2256492_1920.jpg"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(textureId00_, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false));
    }

    // entity 01
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(0.0f, 0.0f, 0.0f);
        renderedModelPtr->SetMeshDataPtr(meshDataPtrArray_[0]);
        renderedModelPtr->SetTextureId(0);
        renderedModelPtr->SetSpecularId(0);
        renderedModelPtr->SetShininess(0.0f);
        // renderedModelPtr->SetColorRGB(0.8f, 0.2f, 0.2f);

        // _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::LightSource>(tempEntityId));
        // 
        // Project001::LightSource* lightSourcePtr;
        // _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::LightSource>(tempEntityId, lightSourcePtr));
        // lightSourcePtr->SetLightTypeDirectional();
        // lightSourcePtr->SetDirection(-1.0f, -1.0f, -1.0f);
        // lightSourcePtr->SetDiffuseColor(0.1f, 0.0f, 0.0f);
        // lightSourcePtr->TurnOn();
    }
}

void TestScene100::Deinitialize()
{
    TestSceneBase001::Deinitialize();

    ClearResources();
}

void TestScene100::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene100::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneBase001::OnEvent(event);
}

// protected: ------------------------------------------------------------------

void TestScene100::ClearResources()
{
    textureId00_ = (unsigned int)-1;
}

void TestScene100::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
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