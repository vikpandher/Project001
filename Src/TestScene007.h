#pragma once

#include "TestSceneBase002.h"



class TestScene007 : public TestSceneBase002
{
public:
    TestScene007();
    ~TestScene007();

    TestScene007(TestScene007& other) = delete;
    void operator=(const TestScene007&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

private:
};