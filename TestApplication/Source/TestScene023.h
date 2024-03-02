#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase001.h"



class TestScene023 : public TestSceneBase001
{
public:
    TestScene023(Project001::Application* applicationPtr);
    ~TestScene023();

    TestScene023(TestScene023& other) = delete;
    void operator=(const TestScene023&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

private:
    void Run_UnitTests() const;
};