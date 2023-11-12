#pragma once

#include "TestSceneBase001.h"



class TestScene007 : public TestSceneBase001
{
public:
    TestScene007(Project001::Application* applicationPtr);
    ~TestScene007();

    TestScene007(TestScene007& other) = delete;
    void operator=(const TestScene007&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);

private:
};