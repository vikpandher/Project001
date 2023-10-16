#pragma once

#include "TestSceneBase001.h"



class TestScene013 : public TestSceneBase001
{
public:
    TestScene013();
    ~TestScene013();

    TestScene013(TestScene013& other) = delete;
    void operator=(const TestScene013&) = delete;

    const char* Name() override;

protected:
    bool OnInitialize() override;

    bool OnDeinitialize() override;

    void OnHandleEvent(Project001::Event& event) override;

    void ClearResources();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

private:
    void Run_UnitTests() const;
};