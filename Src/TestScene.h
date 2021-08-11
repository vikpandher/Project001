#pragma once

#include "Engine/Scene.h"


namespace Project001
{
    struct CursorPositionEvent;
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

    void Initialize(
        Project001::ComponentStores* componentStoresPtr,
        Project001::MeshStores* meshStoresPtr,
        Project001::TextureStores* textureStoresPtr,
        Project001::Renderer* rendererPtr,
        Project001::Window* windowPtr) override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:

    // On CursorPositionEvent: -------------------------------------------------

    void ProcessCursorPosition(Project001::CursorPositionEvent& cursorPositionEvent);

    // On MouseButtonEvent: ----------------------------------------------------

    void ProcessMouseButton(Project001::MouseButtonEvent& mouseButtonEvent);

    // On ScrollEvent: ---------------------------------------------------------

    void ProcessScroll(Project001::ScrollEvent& scrollEvent);

    // On UpdateEvent: ---------------------------------------------------------

    void Update(Project001::UpdateEvent& updateEvent);

    void UpdateMainCameraEntityPositionAndRoll(double timestep);

    void UpdateLightEntityPosition();

    void DeleteDeadEntities();

    void RenderRenderableEntities();

    // Pointers from Application: ----------------------------------------------

    Project001::ComponentStores* componentStoresPtr_;
    Project001::MeshStores* meshStoresPtr_;
    Project001::Renderer* rendererPtr_;
    Project001::TextureStores* textureStoresPtr_;
    Project001::Window* windowPtr_;

    // EntityIds: --------------------------------------------------------------

    unsigned int cubeMeshIndex_;
    unsigned int diceTexture01Index_;
    unsigned int diceTexture02Index_;
    unsigned int thonkTextureIndex_;
    unsigned int patternSpecularIndex_;
    unsigned int thonkSpecularIndex_;

    unsigned int shape01Index_;
    unsigned int shape02Index_;
    unsigned int shape03Index_;
    unsigned int shape04Index_;
    unsigned int shape05Index_;

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

private:
    void ComponentContainerTest() const;
    void ComponentStoresTest() const;
    void LRUArrayTest() const;
    void MeshStoresTest() const;
    void RendererTest() const;
    void TextureStoresTest() const;
};