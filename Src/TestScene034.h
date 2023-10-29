#pragma once

#include "TestSceneBase001.h"



class TestScene034 : public TestSceneBase001
{
public:
    TestScene034();
    ~TestScene034();

    TestScene034(TestScene034& other) = delete;
    void operator=(const TestScene034&) = delete;

    const char* Name() override;

protected:
    bool OnInitialize() override;

    bool OnDeinitialize() override;

    void OnHandleEvent(Project001::Event& event) override;

    void ClearResources();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

    // Texture Data ------------------------------------------------------------

    unsigned int thonk001_TextureId_;

    unsigned int thonkSpecular001_TextureId_;

    std::vector<unsigned int> _32x32_TextureIds_;

    // Mesh Data ---------------------------------------------------------------

    Project001::MeshData* cube001_MeshDataPtr_;
    unsigned int cube001_MeshId_;
    float cube001_MaxBoundingRadius_;

    Project001::MeshData* line001_MeshDataPtr_;
    unsigned int line001_MeshId_;
    float line001_MaxBoundingRadius_;

    Project001::MeshData* cone001_MeshDataPtr_;
    unsigned int cone001_MeshId_;
    float cone001_MaxBoundingRadius_;

private:
};