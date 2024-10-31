// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase001.h"



class TestScene033 : public TestSceneBase001
{
public:
    TestScene033(Project001::Application* applicationPtr);
    ~TestScene033();

    TestScene033(TestScene033& other) = delete;
    void operator=(const TestScene033&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

    // Texture Data ------------------------------------------------------------

    std::vector<unsigned int> _32x32_TextureIds_;
    std::vector<unsigned int> _48x48_TextureIds_;

    Project001::FontData* font01_FontDataPtr_;
    Project001::TextureData* font01_TextureDataPtr_;
    unsigned int font01_TextureId_;

    // Mesh Data ---------------------------------------------------------------

    Project001::MeshData* square_MeshDataPtr_;
    unsigned int square_MeshId_;
    float square_MaxBoundingRadius_;

    const float fontPixelSize_ = 0.01f;
    Project001::MeshData* ui_fps_MeshDataPtr_;

    unsigned int counter_;
    Project001::MeshData* ui_counter_MeshDataPtr_;

    Project001::MeshData* ui_largeText_MeshDataPtr_;

    // Entity Ids --------------------------------------------------------------

    unsigned int secondaryCameraEntityId_;

    unsigned int tertiaryCameraEntityId_;

    std::vector<unsigned int> square_EntityIds_;

    unsigned int ui_fps_EntityId_;

    unsigned int ui_counter_EntityId_;

    unsigned int ui_largeText_EntityId_;

    // -------------------------------------------------------------------------

    const uint32_t s_secondaryCameraMask_ = 0b00000000000000000000000000000010;
};