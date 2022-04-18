#pragma once

#include "TestSceneBase002.h"



class TestScene006 : public TestSceneBase002
{
public:
    TestScene006();
    ~TestScene006();

    TestScene006(TestScene006& other) = delete;
    void operator=(const TestScene006&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

private:
};