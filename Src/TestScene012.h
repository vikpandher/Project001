#pragma once

#include "TestSceneBase002.h"



class TestScene012 : public TestSceneBase002
{
public:
    TestScene012();
    ~TestScene012();

    TestScene012(TestScene012& other) = delete;
    void operator=(const TestScene012&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

private:
};