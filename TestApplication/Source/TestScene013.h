#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase002.h"



class TestScene013 : public TestSceneBase002
{
public:
    TestScene013(Project001::Application* applicationPtr);
    ~TestScene013();

    TestScene013(TestScene013& other) = delete;
    void operator=(const TestScene013&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    // Mesh Data ---------------------------------------------------------------

    unsigned int rectangleMeshId_;

    unsigned int circleMeshId_;

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

private:
    void TestCollisionBodyQuadTree2D();
};