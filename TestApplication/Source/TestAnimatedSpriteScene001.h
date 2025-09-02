// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-09-01

#pragma once

#include "Scene.h"

#include "glm/glm.hpp"



namespace Project001
{
    struct MeshData;
    struct TextureData;
}

class TestAnimatedSpriteScene001 : public Project001::Scene
{
public:
    struct InitializationInfo
    {
        size_t frameCount = 0;
        Project001::MeshData** frameMeshDataPtrsPtr = nullptr;
        unsigned long long* frameDurations_ns_ptr = nullptr;

        const uint32_t* cameraMaskPtr = nullptr;
        const glm::vec3* positionPtr = nullptr;
        const unsigned int* textureIdPtr = nullptr;
    };

    TestAnimatedSpriteScene001(Project001::Application* applicationPtr);
    ~TestAnimatedSpriteScene001();

    TestAnimatedSpriteScene001(TestAnimatedSpriteScene001& other) = delete;
    void operator=(const TestAnimatedSpriteScene001&) = delete;

    void HandleEvent(Project001::Event& event) override;

    void Initialize(const InitializationInfo& initializationInfo);
    void Deinitialize();

protected:
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    // Mesh Data ---------------------------------------------------------------

    size_t frameCount_;
    Project001::MeshData** frameMeshDataPtrsPtr_;
    unsigned long long* frameDurations_ns_ptr_;

    // Entity Ids --------------------------------------------------------------

    unsigned int spriteEntityId_;

    // -------------------------------------------------------------------------

    size_t currentFrame_;
    unsigned long long animationTimeDebt_ns_;
};