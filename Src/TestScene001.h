#pragma once

#include "Engine/Scene.h"

#include "Engine/Event.h"

#include "glm/glm.hpp"

#include <vector>


namespace Project001
{
    struct FontData;
    struct MeshData;
    struct TextureData;
}

class TestScene001 : public Project001::Scene
{
public:
    TestScene001();
    ~TestScene001();

    TestScene001(TestScene001& other) = delete;
    void operator=(const TestScene001&) = delete;

    const char* Name() override;

protected:
    bool OnInitialize() override;

    bool OnDeinitialize() override;

    void OnHandleEvent(Project001::Event& event) override;

    void ClearResources();

    void ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent);
    void ProcessFrameBufferSizeEvent(Project001::FrameBufferSizeEvent& frameBufferSizeEvent);
    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    // requires:
    //     mainCameraEntityId_
    // 
    // xPosition and yPosition are window coordinates
    void UpdatePreviousWorldCursorPosition(float xPosition, float yPosition);

    // -------------------------------------------------------------------------

    Project001::Window* windowPtr_;

    Project001::Renderer* rendererPtr_;

    Project001::SoundPlayer* soundPlayerPtr_;

    Project001::ComponentStores* componentStoresPtr_;

    // Font Data ---------------------------------------------------------------

    Project001::FontData* font01_FontDataPtr_;
    Project001::TextureData* font01_TextureDataPtr_;
    unsigned int font01_TextureId_;

    // Mesh Data ---------------------------------------------------------------

    Project001::MeshData* rectangleMeshDataPtr_;
    unsigned int rectangularMeshId_;

    Project001::MeshData* selectorMeshDataPtr_;
    unsigned int selectorMeshId_;

    Project001::MeshData* circleMeshDataPtr_;

    std::vector<Project001::MeshData*> textMeshDataPtrs_;

    float fontPixelSize_ = 0.0032f;

    // Entity Ids --------------------------------------------------------------

    unsigned int mainCameraEntityId_;
    unsigned int cursorEntityId_;

    unsigned int cursorPositionRenderedMeshIndex_;
    unsigned int cursorPressRenderedMeshIndex_;
    unsigned int cursorReleaseRenderedMeshIndex_;

    unsigned int cursorPositionCollisionPointIndex_;
    unsigned int cursorPressCollisionPointIndex_;
    unsigned int cursorReleaseCollisionPointIndex_;

    unsigned int cursorPositionCollisionShapeId_ = 100;
    unsigned int cursorPressCollisionShapeId_ = 101;
    unsigned int cursorReleaseCollisionShapeId_ = 102;

    std::vector<unsigned int> buttonEntityIds_;

    unsigned int selectorEntityId_;

    // -------------------------------------------------------------------------

    glm::vec2 previousWorldCursorPosition_;
    glm::vec2 previousWorldCursorPress_;
    glm::vec2 previousWorldCursorRelease_;

    unsigned int selectedEntityId_;

    static const Project001::KeyCode s_keyCodeMoveUp_ = Project001::KeyCode::KEY_CODE_W;
    static const Project001::KeyCode s_keyCodeMoveLeft_ = Project001::KeyCode::KEY_CODE_A;
    static const Project001::KeyCode s_keyCodeMoveDown_ = Project001::KeyCode::KEY_CODE_S;
    static const Project001::KeyCode s_keyCodeMoveRight_ = Project001::KeyCode::KEY_CODE_D;
    static const Project001::KeyCode s_keyCodeSelect_ = Project001::KeyCode::KEY_CODE_ENTER;

private:
};