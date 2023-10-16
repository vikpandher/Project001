#pragma once

#include "Engine/Scene.h"

#include "glm/glm.hpp"

#include <map>



namespace Project001
{
    struct FontData;
    struct MeshData;
    struct TextureData;

    struct CursorPositionEvent;
    struct FrameBufferSizeEvent;
    struct MouseButtonEvent;
    struct RenderEvent;
    struct UpdateEvent;
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
    void ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

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

    Project001::MeshData* circleMeshDataPtr_;

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

    // -------------------------------------------------------------------------

    glm::vec2 previousWorldCursorPosition_;
    glm::vec2 previousWorldCursorPress_;
    glm::vec2 previousWorldCursorRelease_;

    struct Button001
    {
        std::string textString_;
        Project001::MeshData* textMeshDataPtr_ = nullptr;
    };

    std::map<unsigned int, Button001> buttons_;

    unsigned int buttonCollisionShapeId_ = 200;

private:
};