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

    TestCartesianToPolar();
    TestPolarToCartesian();

    TestGet2DVectorAngle();
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

// private: --------------------------------------------------------------------

void TestScene004::TestCartesianToPolar() const
{
    glm::vec2 input00(0.0f, 0.0f);
    glm::vec2 output00 = Project001::CartesianToPolar(input00);

    glm::vec2 input01(1.0f, 0.0f);
    glm::vec2 output01 = Project001::CartesianToPolar(input01);

    glm::vec2 input02(1.0f, 1.0f);
    glm::vec2 output02 = Project001::CartesianToPolar(input02);

    glm::vec2 input03(0.0f, 1.0f);
    glm::vec2 output03 = Project001::CartesianToPolar(input03);

    glm::vec2 input04(-1.0f, 1.0f);
    glm::vec2 output04 = Project001::CartesianToPolar(input04);

    glm::vec2 input05(-1.0f, 0.0f);
    glm::vec2 output05 = Project001::CartesianToPolar(input05);

    glm::vec2 input06(-1.0f, -1.0f);
    glm::vec2 output06 = Project001::CartesianToPolar(input06);

    glm::vec2 input07(0.0f, -1.0f);
    glm::vec2 output07 = Project001::CartesianToPolar(input07);

    glm::vec2 input08(1.0f, -1.0f);
    glm::vec2 output08 = Project001::CartesianToPolar(input08);
}

void TestScene004::TestPolarToCartesian() const
{
    glm::vec2 input00(0.0f, 0.0f);
    glm::vec2 output00 = Project001::PolarToCartesian(input00);

    glm::vec2 input01(1.0f, 0.0f);
    glm::vec2 output01 = Project001::PolarToCartesian(input01);

    glm::vec2 input02(1.0f, glm::quarter_pi<float>());
    glm::vec2 output02 = Project001::PolarToCartesian(input02);

    glm::vec2 input03(1.0f, glm::half_pi<float>());
    glm::vec2 output03 = Project001::PolarToCartesian(input03);

    glm::vec2 input04(1.0f, glm::half_pi<float>() + glm::quarter_pi<float>());
    glm::vec2 output04 = Project001::PolarToCartesian(input04);

    glm::vec2 input05(1.0f, glm::pi<float>());
    glm::vec2 output05 = Project001::PolarToCartesian(input05);

    glm::vec2 input06(1.0f, glm::pi<float>() + glm::quarter_pi<float>());
    glm::vec2 output06 = Project001::PolarToCartesian(input06);

    glm::vec2 input07(1.0f, glm::pi<float>() + glm::half_pi<float>());
    glm::vec2 output07 = Project001::PolarToCartesian(input07);

    glm::vec2 input08(1.0f, glm::pi<float>() + glm::half_pi<float>() + glm::quarter_pi<float>());
    glm::vec2 output08 = Project001::PolarToCartesian(input08);

    glm::vec2 input09(1.0f, glm::two_pi<float>() + glm::pi<float>());
    glm::vec2 output09 = Project001::PolarToCartesian(input09);
}

void TestScene004::TestGet2DVectorAngle() const
{
    glm::vec2 vector00(0.0f, 0.0f);
    glm::vec2 vector01(1.0f, 0.0f);
    glm::vec2 vector02(1.0f, 1.0f);
    glm::vec2 vector03(0.0f, 1.0f);
    glm::vec2 vector04(-1.0f, 1.0f);
    glm::vec2 vector05(-1.0f, 0.0f);
    glm::vec2 vector06(-1.0f, -1.0f);
    glm::vec2 vector07(0.0f, -1.0f);
    glm::vec2 vector08(1.0f, -1.0f);
    float angle0000 = Project001::Get2DVectorAngle(vector00, vector00);
    float angle0001 = Project001::Get2DVectorAngle(vector00, vector01);
    float angle0003 = Project001::Get2DVectorAngle(vector00, vector03);
    float angle0101 = Project001::Get2DVectorAngle(vector01, vector01);
    float angle0102 = Project001::Get2DVectorAngle(vector01, vector02);
    float angle0201 = Project001::Get2DVectorAngle(vector02, vector01);
    float angle0103 = Project001::Get2DVectorAngle(vector01, vector03);
    float angle0104 = Project001::Get2DVectorAngle(vector01, vector04);
    float angle0105 = Project001::Get2DVectorAngle(vector01, vector05);
    float angle0106 = Project001::Get2DVectorAngle(vector01, vector06);
    float angle0107 = Project001::Get2DVectorAngle(vector01, vector07);
    float angle0108 = Project001::Get2DVectorAngle(vector01, vector08);
}
