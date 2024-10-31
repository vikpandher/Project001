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

class TestScene016 : public TestSceneBase002
{
public:
    TestScene016(Project001::Application* applicationPtr);
    ~TestScene016();

    TestScene016(TestScene016& other) = delete;
    void operator=(const TestScene016&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void CreateEntitiesForGravityTest01();

    void GenerateCombinationShape(Project001::CollisionBody2DCreationInfo& collisionBody2DCreationInfo, glm::vec3& position);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;
};