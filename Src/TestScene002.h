#pragma once

#include "Engine/Scene.h"


namespace Project001
{
    struct CursorPositionEvent;
    struct FrameBufferSizeEvent;
    struct KeyEvent;
    struct MouseButtonEvent;
    struct ScrollEvent;
    struct UpdateEvent;
}

class TestScene002 : public Project001::Scene
{
public:
    TestScene002();
    ~TestScene002();

    TestScene002(TestScene002& other) = delete;
    void operator=(const TestScene002&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ClearIndiciesAndEntityIds();

    void ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent);
    void ProcessFrameBufferSizeEvent(Project001::FrameBufferSizeEvent& frameBufferSizeEvent);
    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent);
    void ProcessScrollEvent(Project001::ScrollEvent& scrollEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void UpdateMainCameraEntityPositionAndRoll(double timestep);

    void UpdateLightEntityPosition();

    void RenderRenderableEntities();

    // Pointers from Application: ----------------------------------------------

    Project001::Window* windowPtr_;

    Project001::ComponentStores* componentStoresPtr_;
    Project001::MeshStores* meshStoresPtr_;
    Project001::TextureStores* textureStoresPtr_;

    Project001::Renderer* rendererPtr_;

    // Mesh Indicies: ----------------------------------------------------------

    unsigned int shape01MeshIndex_;
    unsigned int shape02MeshIndex_;
    unsigned int shape03MeshIndex_;
    unsigned int shape04MeshIndex_;
    unsigned int shape05MeshIndex_;
    unsigned int shape06MeshIndex_;
    unsigned int shape07MeshIndex_;
    unsigned int shape08MeshIndex_;

    // Texture Indicies: -------------------------------------------------------

    unsigned int earthTextureIndex_;

    // Entity Ids: -------------------------------------------------------------

    unsigned int sceneDataEntityId_;
    unsigned int mainCameraEntityId_;
    unsigned int lightSourceEntityId_;
    unsigned int shape01EntityId_;
    unsigned int shape02EntityId_;
    unsigned int shape03EntityId_;
    unsigned int shape04EntityId_;
    unsigned int shape05EntityId_;
    unsigned int shape06EntityId_;
    unsigned int shape07EntityId_;
    unsigned int shape08EntityId_;

private:
};