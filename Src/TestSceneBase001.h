#pragma once

#include "Engine/Scene.h"

#include "glm/glm.hpp"



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

    const char* Name() override;

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

    void SyncLightSourcePosition();

    // -------------------------------------------------------------------------

    Project001::Window* windowPtr_;

    Project001::Renderer* rendererPtr_;

    Project001::SoundPlayer* soundPlayerPtr_;

    Project001::ComponentStores* componentStoresPtr_;

    // Entity Ids: -------------------------------------------------------------

    unsigned int mainCameraEntityId_;
    unsigned int lightSourceEntityId_;

    // Scene Data: -------------------------------------------------------------

    glm::vec2 previousWorldCursorDownPosition_;

private:
};