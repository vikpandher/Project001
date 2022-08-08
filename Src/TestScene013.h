#pragma once

#include "Engine/Scene.h"

#include "glm/glm.hpp"

#include <map>
#include <vector>


namespace Project001
{
    class ComponentStores;
    struct MeshData;

    struct CursorPositionEvent;
    struct FrameBufferSizeEvent;
    struct KeyEvent;
    struct MouseButtonEvent;
    struct RenderEvent;
    struct ScrollEvent;
    struct UpdateEvent;
}

class TestScene013 : public Project001::Scene
{
public:
    TestScene013();
    ~TestScene013();

    TestScene013(TestScene013& other) = delete;
    void operator=(const TestScene013&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ClearResources();

    void ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorButtonEvent);
    void ProcessFrameBufferSizeEvent(Project001::FrameBufferSizeEvent& frameBufferSizeEvent);
    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessScrollEvent(Project001::ScrollEvent& scrollEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void UpdatedSelectedEntityPosition(unsigned long timestep_ns);

    void DetectCollisions();

    void Sync_RenderedModel_CollisionBody_Components();

    // Pointers from Application: ----------------------------------------------

    Project001::Window* windowPtr_;

    Project001::ComponentStores* componentStoresPtr_;

    Project001::Renderer* rendererPtr_;

    // Mesh Data: --------------------------------------------------------------

    unsigned int selectedEntityIdIndex_;

    std::vector<Project001::MeshData*> meshDataPtrArray_;

    // Entity Ids: -------------------------------------------------------------

    unsigned int mainCameraEntityId_;

    std::vector<unsigned int> entityIds_;

    // Scene Data: -------------------------------------------------------------

    bool cursorGrabbingEntity_;
    glm::vec2 previousWorldCursorDownPosition_;

private:
    void Run_UnitTests() const;
};