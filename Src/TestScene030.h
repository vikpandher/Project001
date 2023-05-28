#pragma once

#include "TestSceneBase001.h"



namespace Project001
{
    struct MeshData;

    struct KeyEvent;
}

class TestScene030 : public TestSceneBase001
{
public:
    TestScene030();
    ~TestScene030();

    TestScene030(TestScene030& other) = delete;
    void operator=(const TestScene030&) = delete;

    const char* Name() override;

protected:
    bool OnInitialize() override;

    bool OnDeinitialize() override;

    void OnHandleEvent(Project001::Event& event) override;

    void ClearResources();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

    // 

    Project001::MeshData* meshDataPtr00_;

private:
};