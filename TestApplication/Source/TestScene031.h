// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase001.h"



class TestScene031 : public TestSceneBase001
{
public:
    TestScene031(Project001::Application* applicationPtr);
    ~TestScene031();

    TestScene031(TestScene031& other) = delete;
    void operator=(const TestScene031&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

    // Texture Data ------------------------------------------------------------

    Project001::FontData* font01_FontDataPtr_;
    Project001::TextureData* font01_TextureDataPtr_;
    unsigned int font01_TextureId_;

    // Mesh Data ---------------------------------------------------------------

    const float fontPixelSize_ = 0.01f;
    Project001::MeshData* ui_fps_MeshDataPtr_;

    Project001::MeshData* ui_renderedMeshCount_MeshDataPtr_;

    // Entity Ids --------------------------------------------------------------

    unsigned int ui_fps_EntityId_;

    unsigned int ui_renderedMeshCount_EntityId_;
};