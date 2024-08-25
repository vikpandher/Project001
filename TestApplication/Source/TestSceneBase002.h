#pragma once

#include "Scene.h"
#include "TimeProfiler.h"

#include "glm/glm.hpp"

#include <vector>



namespace Project001
{
    struct FontData;
    struct MeshData;
    struct TextureData;
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

    void UpdateWorldCursor(float xPosition, float yPosition);

    void UpdateCameraPosition(unsigned long long timestep_ns);

    void UpdateSelectedEntityPosition(unsigned long long timestep_ns);

    void UpdateSelectedEntityVelocity(unsigned long long timestep_ns);

    void ApplyGravity(unsigned long long timestep_ns);

    void CapPositions();

    void CapVelocities();

    void Sync_RenderedMesh_CollisionBody_Components();

    void UpdateCollisionBodyColors();

    void UpdateCursorLineAndDistanceTextMesh();

    void UpdateMassTextMesh();

    void UpdateMomentOfInertiaTextMesh();

    void UpdateEntityIdTextMesh();

    void UpdateFpsTextMesh(unsigned long long timestep_ns);

    void UpdateEnergyTextMesh();

    void UpdateCollisionBodyBorderMesh();

    void UpdateCollisionBodyQuadTreeMesh();

    void UpdateCollisionMarkerCollectionMesh();

    void UpdateCollisionBodyTexture();

    // -------------------------------------------------------------------------

    Project001::Window* windowPtr_;
    Project001::Renderer* rendererPtr_;
    Project001::ComponentStores* componentStoresPtr_;

    // Texture Data ------------------------------------------------------------

    Project001::FontData* font01_FontDataPtr_;
    Project001::TextureData* font01_TextureDataPtr_;
    unsigned int font01_TextureId_;

    unsigned int text_dynamic_TextureId_;
    unsigned int text_overlapOnly_TextureId_;
    unsigned int text_rotationOnly_TextureId_;
    unsigned int text_static_TextureId_;
    unsigned int text_translationOnly_TextureId_;

    // Mesh Data ---------------------------------------------------------------

    std::vector<glm::vec2> cursorLinePositions_;
    Project001::MeshData* cursorLineMeshDataPtr_;

    const float fontPixelSize_ = 0.005f;
    Project001::MeshData* distanceTextMeshDataPtr_;
    Project001::MeshData* massTextMeshDataPtr_;
    Project001::MeshData* momentOfInertiaTextMeshDataPtr_;

    Project001::MeshData* entityIdTextMeshDataPtr_;
    Project001::MeshData* fps_MeshDataPtr_;
    Project001::MeshData* energy_MeshDataPtr_;
    Project001::MeshData* collisionBodyBorderMeshDataPtr_;
    Project001::MeshData* collisionBodyQuadTreeMeshDataPtr_;
    Project001::MeshData* collisionMarkerCollectionMeshDataPtr_;

    std::vector<Project001::MeshData*> meshDataPtrArray_;

    // Entity Ids --------------------------------------------------------------

    unsigned int mainCameraEntityId_;
    static const uint32_t s_uiCameraMask_ = 0b10000000000000000000000000000000;
    unsigned int uiCameraEntityId_;

    unsigned int cursorEntityId_;

    unsigned int distanceEntityId_;
    unsigned int massEntityId_;
    unsigned int momentOfInertiaEntityId_;
    unsigned int entityIdTextEntityId_;
    unsigned int fpsTextEntityId_;
    unsigned int energyTextEntityId_;
    unsigned int collisionBodyBorderEntityId_;
    unsigned int collisionBodyQuadTreeEntityId_;
    unsigned int collisionMarkerCollectionEntityId_;

    std::vector<unsigned int> entityIds_;

    // -------------------------------------------------------------------------

    bool cursorGrabbingEntity_;
    glm::vec2 previousWorldCursorPosition_;
    unsigned int selectedEntityIdIndex_;

    Project001::Timer timer01_;
    unsigned long long remainingTimeRecordingDuration_ns_;

    glm::vec2 positionBorderSize_;

    float maxVelocity_;
    float maxAngularVelocity_;

    bool generateCursorLineAndDistanceTextMesh_;
    bool generateMassTextMesh_;
    bool generateMomentOfInertiaTextMesh_;
    bool generateEntityIdTextMesh_;
    bool generateFpsTextMesh_;
    bool generateEnergyTextMesh_;
    bool generateCollisionBodyQuadTreeMesh_;
    bool generateCollisionMarkerCollectionMesh_;

    bool applyGravity_;
    bool updateCollisionBodyTexture_;
    bool recolorOverlappingCollisionBodies_;
    bool velocityBasedMovement_;

    size_t physicsStepsPerUpdate_;
    bool useCollisionBodyQuadTree_;

    static const uint32_t mainCollisionGroupMask_ = 0b00000000000000000000000000000010;
    glm::vec2 gravity_;
};