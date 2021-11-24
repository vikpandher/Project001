#pragma once

#include "Engine/Scene.h"


namespace Project001
{
    struct CursorPositionEvent;
    struct FrameBufferSizeEvent;
    struct MouseButtonEvent;
    struct RenderEvent;
    struct ScrollEvent;
    struct UpdateEvent;
}

class TestSceneFramework : public Project001::Scene
{
public:
    TestSceneFramework();
    ~TestSceneFramework();

    TestSceneFramework(TestSceneFramework& other) = delete;
    void operator=(const TestSceneFramework&) = delete;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ClearIndiciesAndEntityIds();

    void ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent);
    void ProcessFrameBufferSizeEvent(Project001::FrameBufferSizeEvent& frameBufferSizeEvent);
    void ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessScrollEvent(Project001::ScrollEvent& scrollEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void UpdateMainCameraEntityPositionAndRoll(double timestep);

    void UpdateLightEntityPosition();

    void DeleteDeadEntities();

    // Pointers from Application: ----------------------------------------------

    Project001::Window* windowPtr_;

    Project001::ComponentStores* componentStoresPtr_;
    Project001::MeshStores* meshStoresPtr_;
    Project001::TextureStores* textureStoresPtr_;

    Project001::Renderer* rendererPtr_;

    // Entity Ids: -------------------------------------------------------------

    unsigned int sceneDataEntityId_;
    unsigned int mainCameraEntityId_;
    unsigned int lightSourceEntityId_;

private:
};