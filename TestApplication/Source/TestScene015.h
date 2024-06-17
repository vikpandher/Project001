#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase002.h"



class TestScene015 : public TestSceneBase002
{
public:
    TestScene015(Project001::Application* applicationPtr);
    ~TestScene015();

    TestScene015(TestScene015& other) = delete;
    void operator=(const TestScene015&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void CreateEntitiesForNewtonsCradle01();
    void CreateEntitiesForNewtonsCradle02();
    void CreateEntitiesForNewtonsCradle03();

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;
};