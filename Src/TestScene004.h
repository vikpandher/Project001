#pragma once

#include "TestSceneFramework.h"


namespace Project001
{
    struct KeyEvent;
}

class TestScene004 : public TestSceneFramework
{
public:
    TestScene004();
    ~TestScene004();

    TestScene004(TestScene004& other) = delete;
    void operator=(const TestScene004&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ClearIndiciesAndEntityIds();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

    // Mesh Indicies: ----------------------------------------------------------

    // Texture Indicies: -------------------------------------------------------

    // Entity Ids: -------------------------------------------------------------

private:
    void TestCartesianToPolar() const;
    void TestPolarToCartesian() const;
    void TestGet2DVectorAngle() const;
};