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

};