#pragma once


#include "TestInstructionScene001.h"
#include "TestSceneBase001.h"



class TestScene006 : public TestSceneBase001
{
public:
    TestScene006(Project001::Application* applicationPtr);
    ~TestScene006();

    TestScene006(TestScene006& other) = delete;
    void operator=(const TestScene006&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

private:
};