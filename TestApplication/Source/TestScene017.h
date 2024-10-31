// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase002.h"



namespace Project001
{
    struct CollisionBody2DCreationInfo;
}

class TestScene017 : public TestSceneBase002
{
public:
    TestScene017(Project001::Application* applicationPtr);
    ~TestScene017();

    TestScene017(TestScene017& other) = delete;
    void operator=(const TestScene017&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void CreateCollisionBodyEntities();

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;
};