#pragma once

#include "Scene.h"

#include "glm/glm.hpp"

#include <vector>



namespace Project001
{
    struct MeshData;

    struct CursorPositionEvent;
    struct DeinitializeEvent;
    struct InitializeEvent;
    struct KeyEvent;
    struct MouseButtonEvent;
    struct RenderEvent;
    struct ScrollEvent;
    struct UpdateEvent;
}

class TestSceneBase001 : public Project001::Scene
{
public:
    TestSceneBase001(Project001::Application* applicationPtr);
    ~TestSceneBase001();

    TestSceneBase001(TestSceneBase001& other) = delete;
    void operator=(const TestSceneBase001&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent);
    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessScrollEvent(Project001::ScrollEvent& scrollEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void UpdateMainCameraEntityPositionAndRoll(unsigned long long timestep_ns);

    void SyncLightSourcePosition();

    // -------------------------------------------------------------------------

    Project001::Window* windowPtr_;

    Project001::Renderer* rendererPtr_;

    Project001::SoundPlayer* soundPlayerPtr_;

    Project001::ComponentStores* componentStoresPtr_;

    // Mesh Data ---------------------------------------------------------------

    std::vector<Project001::MeshData*> meshDataPtrArray_;

    // Entity Ids --------------------------------------------------------------

    unsigned int mainCameraEntityId_;
    static const uint32_t s_uiCameraMask_ = 0b10000000000000000000000000000000;
    unsigned int uiCameraEntityId_;

    unsigned int lightSourceEntityId_;

    std::vector<unsigned int> entityIds_;

    // -------------------------------------------------------------------------

    glm::vec2 previousCursorDownPosition_;
};