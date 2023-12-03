#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase001.h"



class TestScene004 : public TestSceneBase001
{
public:
    TestScene004(Project001::Application* applicationPtr);
    ~TestScene004();

    TestScene004(TestScene004& other) = delete;
    void operator=(const TestScene004&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;
};