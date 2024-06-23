#pragma once


#include "TestInstructionScene001.h"
#include "TestSceneBase001.h"



class TestScene007 : public TestSceneBase001
{
public:
    TestScene007(Project001::Application* applicationPtr);
    ~TestScene007();

    TestScene007(TestScene007& other) = delete;
    void operator=(const TestScene007&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

    // Mesh Data ---------------------------------------------------------------

    Project001::MeshData* mesh001_MeshDataPtr_;
    unsigned int mesh001_MeshId_;
    float mesh001_MaxBoundingRadius_;

    // Entity Ids --------------------------------------------------------------

    unsigned int object001_EntityId_;
};