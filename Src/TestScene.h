#pragma once

#include "Engine/Scene.h"


namespace Project001
{
    struct CursorPositionEvent;
    struct FrameBufferSizeEvent;
    struct MouseButtonEvent;
    struct ScrollEvent;
    struct UpdateEvent;
}

class TestScene : public Project001::Scene
{
public:
    TestScene();
    ~TestScene();

    TestScene(TestScene& other) = delete;
    void operator=(const TestScene&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent);
    void ProcessFrameBufferSizeEvent(Project001::FrameBufferSizeEvent& frameBufferSizeEvent);
    void ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent);
    void ProcessScrollEvent(Project001::ScrollEvent& scrollEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void UpdateMainCameraEntityPositionAndRoll(double timestep);

    void UpdateLightEntityPosition();

    void DeleteDeadEntities();

    void RenderRenderableEntities();

    // Pointers from Application: ----------------------------------------------

    Project001::Window* windowPtr_;

    Project001::ComponentStores* componentStoresPtr_;
    Project001::MeshStores* meshStoresPtr_;
    Project001::TextureStores* textureStoresPtr_;

    Project001::Renderer* rendererPtr_;

    // Mesh Indicies: ----------------------------------------------------------

    unsigned int cubeMeshIndex_;
    unsigned int shape01MeshIndex_;
    unsigned int shape02MeshIndex_;
    unsigned int shape03MeshIndex_;
    unsigned int shape04MeshIndex_;
    unsigned int shape05MeshIndex_;
    unsigned int shape06MeshIndex_;
    unsigned int shape07MeshIndex_;
    unsigned int shape08MeshIndex_;
    unsigned int shape09MeshIndex_;
    unsigned int shape10MeshIndex_;
    unsigned int shape11MeshIndex_;
    unsigned int shape12MeshIndex_;
    unsigned int shape13MeshIndex_;
    unsigned int shape14MeshIndex_;
    unsigned int shape15MeshIndex_;
    unsigned int shape16MeshIndex_;

    // Texture Indicies: -------------------------------------------------------

    unsigned int diceTexture01Index_;
    unsigned int diceTexture02Index_;
    unsigned int thonkTextureIndex_;
    unsigned int _100x100TextureIndex_;
    unsigned int patternSpecularTextureIndex_;
    unsigned int thonkSpecularTextureIndex_;

    // Entity Ids: -------------------------------------------------------------

    unsigned int sceneDataEntityId_;
    unsigned int mainCameraEntityId_;
    unsigned int lightSourceEntityId_;
    unsigned int cubeEntity01Id_;
    unsigned int cubeEntity02Id_;
    unsigned int cubeEntity03Id_;
    unsigned int cubeEntity04Id_;
    unsigned int cubeEntity05Id_;
    unsigned int shape01EntityId_;
    unsigned int shape02EntityId_;
    unsigned int shape03EntityId_;
    unsigned int shape04EntityId_;
    unsigned int shape05EntityId_;
    unsigned int shape06EntityId_;
    unsigned int shape07EntityId_;
    unsigned int shape08EntityId_;
    unsigned int shape09EntityId_;
    unsigned int shape10EntityId_;
    unsigned int shape11EntityId_;
    unsigned int shape12EntityId_;
    unsigned int shape13EntityId_;
    unsigned int shape14EntityId_;
    unsigned int shape15EntityId_;
    unsigned int shape16EntityId_;

private:
    void BiMapTest() const;
    void ComponentContainerTest() const;
    void ComponentStoresTest() const;
    void LRUArrayTest() const;
    void MeshStoresTest() const;
    void RendererTest() const;
    void TextureStoresTest() const;
};