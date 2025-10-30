// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-30

#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase002.h"



namespace Project001
{
    struct CollisionBody2DCreationInfo;
}

class TestScene026 : public TestSceneBase002
{
public:
    TestScene026(Project001::Application* applicationPtr);
    ~TestScene026();

    TestScene026(TestScene026& other) = delete;
    void operator=(const TestScene026&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void CreateEntitiesForGravityTest01();

    void GenerateCombinationShape(Project001::CollisionBody2DCreationInfo& collisionBody2DCreationInfo, glm::vec3& position);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;
};