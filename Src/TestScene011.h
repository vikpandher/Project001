#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase002.h"



class TestScene011 : public TestSceneBase002
{
public:
    TestScene011(Project001::Application* applicationPtr);
    ~TestScene011();

    TestScene011(TestScene011& other) = delete;
    void operator=(const TestScene011&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

private:
};