#include "TestScene004.h"

#include "Engine/Math/Collisions.h"
#include "Engine/Math/CoordinateSystems.h"
#include "Engine/Math/VectorAngles.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshStores.h"
#include "Engine/Renderer.h"
#include "Engine/TextureStores.h"
#include "Engine/Window.h"



// public: ---------------------------------------------------------------------

TestScene004::TestScene004()
{
    ClearIndiciesAndEntityIds();

    Project001::TestPointLineIntersection2d();
    Project001::TestPointFixedRectangleIntersection2d();
    Project001::TestPointRectangleIntersection2d();

    Project001::TestCartesianToPolar();
    Project001::TestPolarToCartesian();

    Project001::TestGet2DVectorAngle();
}

TestScene004::~TestScene004()
{}

const char* TestScene004::Name()
{
    return "TestScene004";
}

void TestScene004::Initialize()
{
    TestSceneFramework::Initialize();

    
}

void TestScene004::Deinitialize()
{
    TestSceneFramework::Deinitialize();

    ClearIndiciesAndEntityIds();
}

void TestScene004::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene004::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneFramework::OnEvent(event);
}

// protected: ------------------------------------------------------------------

void TestScene004::ClearIndiciesAndEntityIds()
{

}

void TestScene004::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
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
