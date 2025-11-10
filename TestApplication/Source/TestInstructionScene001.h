// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-11-10

#pragma once

#include "Scene.h"



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
        unsigned int fontTextureId = (unsigned int)-1;

        unsigned int cameraEntityId = (unsigned int)-1;
        uint32_t cameraMask = 0b00000000000000000000000000000000;

        Project001::KeyCode keyCode_toggleInstructions = Project001::KeyCode::KEY_CODE_UNKNOWN;
    };

    TestInstructionScene001(Project001::Application* applicationPtr);
    ~TestInstructionScene001();

    TestInstructionScene001(TestInstructionScene001& other) = delete;
    void operator=(const TestInstructionScene001&) = delete;

    void HandleEvent(Project001::Event& event) override;

    void Initialize(const InitializationInfo& initializationInfo);
    void Deinitialize();

protected:
    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

    // Mesh Data ---------------------------------------------------------------

    Project001::MeshData* hiddenInstructionTextMeshDataPtr_;
    Project001::MeshData* instructionTextMeshDataPtr_;

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
};