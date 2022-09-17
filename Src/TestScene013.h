#pragma once

#include "TestSceneBase001.h"



namespace Project001
{
    struct KeyEvent;
}

class TestScene013 : public TestSceneBase001
{
public:
    TestScene013();
    ~TestScene013();

    TestScene013(TestScene013& other) = delete;
    void operator=(const TestScene013&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ClearResources();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

private:
    void Run_UnitTests() const;
};