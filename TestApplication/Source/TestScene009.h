// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-30

#pragma once

#include "TestAnimatedSpriteScene001.h"
#include "TestInstructionScene001.h"
#include "TestSceneBase001.h"



class TestScene009 : public TestSceneBase001
{
public:
    TestScene009(Project001::Application* applicationPtr);
    ~TestScene009();

    TestScene009(TestScene009& other) = delete;
    void operator=(const TestScene009&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void Create2DStarMeshes_1();
    void Create2DStarMeshes_2();
    void CreateCrownMeshes_1();

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

    unsigned int gradiant_TextureId_;
    unsigned int numbers12x6_TextureId_;

    std::vector<std::vector<Project001::MeshData*>> animatedMeshDataPtrArrayArray_;
    std::vector<std::vector<unsigned long long>> animatedMeshFrameDurationArrayArray_;
    std::vector<TestAnimatedSpriteScene001*> animatedSpriteSceneArray_;
};