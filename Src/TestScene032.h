#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase001.h"



class TestScene032 : public TestSceneBase001
{
public:
    TestScene032(Project001::Application* applicationPtr);
    ~TestScene032();

    TestScene032(TestScene032& other) = delete;
    void operator=(const TestScene032&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

    // Texture Data ------------------------------------------------------------

    unsigned int earth001_TextureId_;
    unsigned int specular001_TextureId_;
    std::vector<unsigned int> _32x32_TextureIds_;

    // Mesh Data ---------------------------------------------------------------

    Project001::MeshData* icosphere001_MeshDataPtr_;
    unsigned int icosphere001_MeshId_;
    float icosphere001_MaxBoundingRadius_;

    Project001::MeshData* arc001_MeshDataPtr_;
    unsigned int arc001_MeshId_;
    float arc001_MaxBoundingRadius_;

    Project001::MeshData* line001_MeshDataPtr_;
    unsigned int line001_MeshId_;
    float line001_MaxBoundingRadius_;

    Project001::MeshData* cone001_MeshDataPtr_;
    unsigned int cone001_MeshId_;
    float cone001_MaxBoundingRadius_;

    // Entity Ids --------------------------------------------------------------

    unsigned int centerIcosphereEntityId_;
    unsigned int centerStar001_EntityId_;
    unsigned int centerStar002_EntityId_;
    unsigned int psudoStencil001_EntityId_;
    std::vector<unsigned int> icosphereEntityIds_;
    std::vector<unsigned int> arcEntityIds_;
    std::vector<unsigned int> coneEntityIds_;
    std::vector<unsigned int> starEntityIds_;
};