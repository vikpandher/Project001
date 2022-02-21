#pragma once

#include "Engine/Scene.h"

#include "glm/glm.hpp"

#include <map>
#include <vector>


namespace Project001
{
    struct CursorPositionEvent;
    struct FrameBufferSizeEvent;
    struct KeyEvent;
    struct MouseButtonEvent;
    struct RenderEvent;
    struct ScrollEvent;
    struct UpdateEvent;
}

class TestScene006 : public Project001::Scene
{
public:
    TestScene006();
    ~TestScene006();

    TestScene006(TestScene006& other) = delete;
    void operator=(const TestScene006&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ClearIndiciesAndEntityIds();

    void ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorButtonEvent);
    void ProcessFrameBufferSizeEvent(Project001::FrameBufferSizeEvent& frameBufferSizeEvent);
    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessScrollEvent(Project001::ScrollEvent& scrollEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void UpdatedSelectedEntityPosition(unsigned long timestep_ns);

    void DetectCollisions();

    void Sync_RenderedModel_CollisionBody2D_Components();

    // Pointers from Application: ----------------------------------------------

    Project001::Window* windowPtr_;

    Project001::ComponentStores* componentStoresPtr_;
    Project001::MeshStores* meshStoresPtr_;
    Project001::Renderer* rendererPtr_;

    // Mesh Indicies: ----------------------------------------------------------

    unsigned int selectedEntityIdIndex_;

    std::vector<unsigned int> meshIndicies_;

    // Entity Ids: -------------------------------------------------------------

    unsigned int mainCameraEntityId_;

    std::vector<unsigned int> entityIds_;

    // Scene Data: -------------------------------------------------------------

    bool cursorGrabbingEntity_;
    glm::vec2 previousCursorPosition_;

private:
};