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

    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);

    // Mesh Data ---------------------------------------------------------------

    const float fontPixelSize_ = 0.008f;
    Project001::MeshData* ui_fps_MeshDataPtr_;

    unsigned int rectangleMeshId_;

    unsigned int circleMeshId_;

    // Entity Ids --------------------------------------------------------------

    unsigned int ui_fps_EntityId_;

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

private:
    void TestCollisionBodyQuadTree2D();
};