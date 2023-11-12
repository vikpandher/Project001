#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase001.h"



class TestScene013 : public TestSceneBase001
{
public:
    TestScene013(Project001::Application* applicationPtr);
    ~TestScene013();

    TestScene013(TestScene013& other) = delete;
    void operator=(const TestScene013&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

private:
    void Run_UnitTests() const;
};