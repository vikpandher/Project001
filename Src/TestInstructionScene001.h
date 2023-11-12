#pragma once

#include "Engine/Scene.h"

#include "Engine/Event.h"



namespace Project001
{
    struct FontData;
    struct MeshData;
    struct TextureData;
}

class TestInstructionScene001 : public Project001::Scene
{
public:
    struct InitializationInfo
    {
        std::string hiddenInstructionString;
        std::string instructionString;

        const Project001::FontData* fontDataPtr = nullptr;
        const unsigned int* fontTextureIdPtr = nullptr;

        const unsigned int* cameraEntityIdPtr = nullptr;
        const uint32_t* cameraMaskPtr = nullptr;

        const Project001::KeyCode* keyCode_toggleInstructionsPtr = nullptr;
    };

    TestInstructionScene001(Project001::Application* applicationPtr, const std::string& name);
    ~TestInstructionScene001();

    TestInstructionScene001(TestInstructionScene001& other) = delete;
    void operator=(const TestInstructionScene001&) = delete;

    void HandleEvent(Project001::Event& event) override;

    void Initialize(const InitializationInfo& initializationInfo);
    void Deinitialize();

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

    // -------------------------------------------------------------------------

    Project001::ComponentStores* componentStoresPtr_;

    // Mesh Data ---------------------------------------------------------------

    const float instructionFontPixelSize_ = 0.0048f;
    Project001::MeshData* hiddenInstructionMeshDataPtr_;
    Project001::MeshData* instructionMeshDataPtr_;

    Project001::MeshData* hiddenInstructionBackgroundMeshDataPtr_;
    Project001::MeshData* instructionBackgroundMeshDataPtr_;

    // Entity Ids --------------------------------------------------------------

    unsigned int instructionsEntityId_;
    unsigned int hiddenInstructionMeshIndex_;
    unsigned int instructionMeshIndex_;
    unsigned int hiddenInstructionBackgroundMeshIndex_;
    unsigned int instructionBackgroundMeshIndex_;

    // -------------------------------------------------------------------------

    Project001::KeyCode keyCode_toggleInstructions_;

private:
};