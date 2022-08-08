#pragma once

#include "TestSceneBase002.h"



class TestScene011 : public TestSceneBase002
{
public:
    TestScene011();
    ~TestScene011();

    TestScene011(TestScene011& other) = delete;
    void operator=(const TestScene011&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

private:
};