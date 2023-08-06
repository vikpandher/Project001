#pragma once

#include "TestSceneBase001.h"



namespace Project001
{
    struct MeshData;

    struct KeyEvent;
}

class TestScene031 : public TestSceneBase001
{
public:
    TestScene031();
    ~TestScene031();

    TestScene031(TestScene031& other) = delete;
    void operator=(const TestScene031&) = delete;

    const char* Name() override;

protected:
    bool OnInitialize() override;

    bool OnDeinitialize() override;

    void OnHandleEvent(Project001::Event& event) override;

    void ClearResources();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

private:
};