#pragma once

#include "TestSceneBase001.h"

#include "Engine/FontData.h"
#include "Engine/TextureData.h"



class TestScene033 : public TestSceneBase001
{
public:
    TestScene033();
    ~TestScene033();

    TestScene033(TestScene033& other) = delete;
    void operator=(const TestScene033&) = delete;

    const char* Name() override;

protected:
    bool OnInitialize() override;

    bool OnDeinitialize() override;

    void OnHandleEvent(Project001::Event& event) override;

    void ClearResources();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    // Texture Data ------------------------------------------------------------

    std::vector<unsigned int> _32x32_TextureIds_;
    std::vector<unsigned int> _48x48_TextureIds_;

    Project001::FontData font01_FontData_;
    Project001::TextureData font01_TextureData_;
    unsigned int font01_TextureId_;

    // Mesh Data ---------------------------------------------------------------

    Project001::MeshData* square_MeshDataPtr_;
    unsigned int square_MeshId_;
    float square_MaxBoundingRadius_;

    float fontPixelSize_ = 0.01f;
    Project001::MeshData* ui_testText_MeshDataPtr_;

    Project001::MeshData* ui_fps_MeshDataPtr_;

    unsigned int counter_;
    Project001::MeshData* ui_counter_MeshDataPtr_;

    Project001::MeshData* ui_largeText_MeshDataPtr_;

    // Entity Ids --------------------------------------------------------------

    uint32_t secondaryCameraMask_ = 0b00000000000000000000000000000010;

    unsigned int secondaryCameraEntityId_;

    unsigned int tertiaryCameraEntityId_;

    std::vector<unsigned int> square_EntityIds_;

    unsigned int ui_testText_EntityId_;

    unsigned int ui_fps_EntityId_;

    unsigned int ui_counter_EntityId_;

    unsigned int ui_largeText_EntityId_;

private:
};