#pragma once

#include "TestSceneBase001.h"



class TestScene032 : public TestSceneBase001
{
public:
    TestScene032();
    ~TestScene032();

    TestScene032(TestScene032& other) = delete;
    void operator=(const TestScene032&) = delete;

    const char* Name() override;

protected:
    bool OnInitialize() override;

    bool OnDeinitialize() override;

    void OnHandleEvent(Project001::Event& event) override;

    void ClearResources();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

    // Texture Data ------------------------------------------------------------

    unsigned int earth001_TextureId_;

    unsigned int specular001_TextureId_;

    std::vector<unsigned int> _32x32_TextureIds_;

    // Mesh Data ---------------------------------------------------------------

    Project001::MeshData* icosphere001_MeshDataPtr_;
    unsigned int icosphere001_MeshId_;
    float icosphere001_MaxRadius_;

    Project001::MeshData* arc001_MeshDataPtr_;
    unsigned int arc001_MeshId_;
    float arc001_MaxRadius_;

    Project001::MeshData* line001_MeshDataPtr_;
    unsigned int line001_MeshId_;
    float line001_MaxRadius_;

    Project001::MeshData* cone001_MeshDataPtr_;
    unsigned int cone001_MeshId_;
    float cone001_MaxRadius_;

    // Entity Ids --------------------------------------------------------------

    unsigned int centerIcosphereEntityId_;

    unsigned int centerStar001_EntityId_;

    unsigned int centerStar002_EntityId_;

    std::vector<unsigned int> icosphereEntityIds_;

    std::vector<unsigned int> arcEntityIds_;

    std::vector<unsigned int> coneEntityIds_;

    std::vector<unsigned int> starEntityIds_;

private:
};