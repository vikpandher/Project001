// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-06-29

#pragma once

#include "TestInstructionScene001.h"

#include "glm/glm.hpp"

#include <vector>



class TestScene104 : public Project001::Scene
{
public:
    TestScene104(Project001::Application* applicationPtr);
    ~TestScene104();

    TestScene104(TestScene104& other) = delete;
    void operator=(const TestScene104&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent);
    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void InitializeInstructionScene();

    void LoadPixelFontResources();
    void LoadCursorResources();

    void CreateMainCameraEntity();
    void CreateCursorEntity(unsigned int& entityId);

    void UpdateCursorPositionUsingWindowCoordinates(unsigned int entityId, float xPosition, float yPosition);

    void UpdateCursor(unsigned int entityId);

    void SyncCursorRenderedModels();

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

    // Resources ---------------------------------------------------------------

    const Project001::FontData* pixelFont_FontDataPtr_ = nullptr;
    const Project001::TextureData* pixelFont_TextureDataPtr_ = nullptr;
    unsigned int pixelFont_TextureId_ = static_cast<unsigned int>(-1);

    Project001::MeshData* cursorHandOpen_MeshDataPtr_ = nullptr;
    Project001::MeshData* cursorHandPointer_MeshDataPtr_ = nullptr;
    Project001::MeshData* cursorHandGrab_MeshDataPtr_ = nullptr;
    Project001::TextureData* cursor_TextureDataPtr_ = nullptr;
    unsigned int cursor_TextureId_ = static_cast<unsigned int>(-1);
    Project001::MeshData* cursorCollision_MeshDataPtr_ = nullptr;

    // Entities ----------------------------------------------------------------

    static const uint32_t s_mainCamera_Mask_ = 0b00000000000000000000000000000001;
    unsigned int mainCamera_EntityId_ = static_cast<unsigned int>(-1);

    unsigned int cursor_EntityId_ = static_cast<unsigned int>(-1);

    static const size_t s_cursorPosition_RenderedMeshIndex_ = 0;
    static const size_t s_cursorPress_RenderedMeshIndex_ = 1;
    static const size_t s_cursorRelease_RenderedMeshIndex_ = 2;
    static const size_t s_cursorHandOpen_RenderedMeshIndex_ = 3;
    static const size_t s_cursorHandGrab_RenderedMeshIndex_ = 4;
    static const size_t s_cursor_RenderedMeshIndices_ = 5;

    static const size_t s_cursorPosition_CollisionPointIndex_ = 0;
    static const size_t s_cursorPress_CollisionPointIndex_ = 1;
    static const size_t s_cursorRelease_CollisionPointIndex_ = 2;
    static const size_t s_cursor_CollisionPointIndices_ = 3;

    static const unsigned int s_cursorPosition_CollisionShapeId_ = 100;
    static const unsigned int s_cursorPress_CollisionShapeId_ = 101;
    static const unsigned int s_cursorRelease_CollisionShapeId_ = 102;

private:
    void CompareSortingAlgorithms();
};