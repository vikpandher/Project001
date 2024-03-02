#pragma once

#include "Scene.h"

#include "glm/glm.hpp"

#include <vector>



namespace Project001
{
    struct MeshData;
}

class TestSceneBase002 : public Project001::Scene
{
public:
    TestSceneBase002(Project001::Application* applicationPtr);
    ~TestSceneBase002();

    TestSceneBase002(TestSceneBase002& other) = delete;
    void operator=(const TestSceneBase002&) = delete;

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

    void UpdatedSelectedEntityPosition(unsigned long long timestep_ns);

    void Sync_RenderedMesh_CollisionBody_Components();

    void ColorCollisions();

    // -------------------------------------------------------------------------

    Project001::Window* windowPtr_;
    Project001::Renderer* rendererPtr_;
    Project001::ComponentStores* componentStoresPtr_;

    // Mesh Data ---------------------------------------------------------------

    std::vector<Project001::MeshData*> meshDataPtrArray_;

    // Entity Ids --------------------------------------------------------------

    unsigned int mainCameraEntityId_;
    static const uint32_t s_uiCameraMask_ = 0b10000000000000000000000000000000;
    unsigned int uiCameraEntityId_;

    unsigned int cursorEntityId_;

    std::vector<unsigned int> entityIds_;

    // -------------------------------------------------------------------------

    bool cursorGrabbingEntity_;
    glm::vec2 previousWorldCursorPosition_;
    unsigned int selectedEntityIdIndex_;
};