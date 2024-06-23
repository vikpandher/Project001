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

    void ApplyGravity(float timestep_s);
    void CapVelocities();
    void CreateEntityForEnergyText();
    void UpdateEnergyString();

    void CreateEntitiesForNewtonsCradle01(float yPos);
    void CreateEntitiesForNewtonsCradle02(float yPos);
    void CreateEntitiesForNewtonsCradle03(float yPos);
    void CreateEntitiesForNewtonsCradle04(float yPos);
    void CreateEntitiesForNewtonsCradle05(float yPos, float friction);
    void CreateEntitiesForNewtonsCradle06(float yPos, float friction);
    void CreateEntitiesForGravityTest01();

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

    // Texture Data ------------------------------------------------------------

    unsigned int texture001_TextureId_;

    // Mesh Data ---------------------------------------------------------------

    const float fontPixelSize_ = 0.008f;
    Project001::MeshData* ui_energy_MeshDataPtr_;

    // Entity Ids --------------------------------------------------------------

    unsigned int ui_energy_EntityId_;

    // -------------------------------------------------------------------------

    static const uint32_t collisionGroupMask_ = 0b00000000000000000000000000000010;

    glm::vec2 gravity_;
};