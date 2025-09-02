// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-09-01

#pragma once

#include "Scene.h"

#include "glm/glm.hpp"

#include <vector>



struct TestApplicationData;

namespace Project001
{
    struct MeshData;
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

    // Mesh Data ---------------------------------------------------------------

    std::vector<Project001::MeshData*> meshDataPtrArray_;

    // Entity Ids --------------------------------------------------------------

    static const uint32_t s_mainCameraMask_ = 0b00000000000000000000000000000001;
    unsigned int mainCameraEntityId_;
    static const uint32_t s_uiCameraMask_ = 0b10000000000000000000000000000000;
    unsigned int uiCameraEntityId_;

    unsigned int lightSourceEntityId_;

    std::vector<unsigned int> entityIds_;

    // -------------------------------------------------------------------------

    glm::vec2 previousCursorDownPosition_;

    unsigned long long remainingTimeRecordingDuration_ns_;
};