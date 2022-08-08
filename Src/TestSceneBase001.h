#pragma once

#include "Engine/Scene.h"



namespace Project001
{
    class ComponentStores;

    struct CursorPositionEvent;
    struct FrameBufferSizeEvent;
    struct MouseButtonEvent;
    struct RenderEvent;
    struct ScrollEvent;
    struct UpdateEvent;
}

class TestSceneBase001 : public Project001::Scene
{
public:
    TestSceneBase001();
    ~TestSceneBase001();

    TestSceneBase001(TestSceneBase001& other) = delete;
    void operator=(const TestSceneBase001&) = delete;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ClearResources();

    void ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent);
    void ProcessFrameBufferSizeEvent(Project001::FrameBufferSizeEvent& frameBufferSizeEvent);
    void ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessScrollEvent(Project001::ScrollEvent& scrollEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void UpdateMainCameraEntityPositionAndRoll(unsigned long timestep_ns);

    void SyncComponentPositions();

    void DeleteDeadEntities();

    // Pointers from Application: ----------------------------------------------

    Project001::Window* windowPtr_;

    Project001::ComponentStores* componentStoresPtr_;

    Project001::Renderer* rendererPtr_;
    Project001::SoundPlayer* soundPlayerPtr_;

    // Entity Ids: -------------------------------------------------------------

    unsigned int sceneDataEntityId_;
    unsigned int mainCameraEntityId_;
    unsigned int lightSourceEntityId_;

private:
};