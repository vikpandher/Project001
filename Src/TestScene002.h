#pragma once

#include "TestSceneBase001.h"

#include "Engine/MeshData.h"



namespace Project001
{
    struct KeyEvent;
}

class TestScene002 : public TestSceneBase001
{
public:
    TestScene002();
    ~TestScene002();

    TestScene002(TestScene002& other) = delete;
    void operator=(const TestScene002&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ClearIndiciesAndEntityIds();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

    // Mesh Data: --------------------------------------------------------------

    Project001::MeshData cubeMesh_;
    Project001::MeshData shape01Mesh_;
    Project001::MeshData shape02Mesh_;
    Project001::MeshData shape03Mesh_;
    Project001::MeshData shape04Mesh_;
    Project001::MeshData shape05Mesh_;
    Project001::MeshData shape06Mesh_;
    Project001::MeshData shape07Mesh_;
    Project001::MeshData shape08Mesh_;
    Project001::MeshData shape09Mesh_;
    Project001::MeshData shape10Mesh_;
    Project001::MeshData shape11Mesh_;
    Project001::MeshData shape12Mesh_;
    Project001::MeshData shape13Mesh_;
    Project001::MeshData shape14Mesh_;
    Project001::MeshData shape15Mesh_;
    Project001::MeshData shape16Mesh_;
    Project001::MeshData shape17Mesh_;
    Project001::MeshData shape18Mesh_;
    Project001::MeshData shape19Mesh_;
    Project001::MeshData shape20Mesh_;
    Project001::MeshData shape21Mesh_;
    Project001::MeshData shape22Mesh_;
    Project001::MeshData shape23Mesh_;
    Project001::MeshData shape24Mesh_;
    Project001::MeshData shape25Mesh_;
    Project001::MeshData shape26Mesh_;
    Project001::MeshData shape27Mesh_;
    Project001::MeshData shape28Mesh_;

    // Texture Ids: ------------------------------------------------------------

    unsigned int dice01TextureId_;
    unsigned int dice02TextureId_;
    unsigned int thonkTextureId_;
    unsigned int _100x100TextureId_;
    unsigned int patternSpecular01TextureId_;
    unsigned int patternSpecular02TextureId_;
    unsigned int thonkSpecularTextureId_;

    // Entity Ids: -------------------------------------------------------------

    unsigned int cubeEntity01Id_;
    unsigned int cubeEntity02Id_;
    unsigned int cubeEntity03Id_;
    unsigned int cubeEntity04Id_;
    unsigned int cubeEntity05Id_;
    unsigned int cubeEntity06Id_;
    unsigned int cubeEntity07Id_;
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
    unsigned int shape17EntityId_;
    unsigned int shape18EntityId_;
    unsigned int shape19EntityId_;
    unsigned int shape20EntityId_;
    unsigned int shape21EntityId_;
    unsigned int shape22EntityId_;
    unsigned int shape23EntityId_;
    unsigned int shape24EntityId_;
    unsigned int shape25EntityId_;
    unsigned int shape26EntityId_;
    unsigned int shape27EntityId_;
    unsigned int shape28EntityId_;

private:
    void BiMapTest() const;
    void ComponentContainerTest() const;
    void ComponentStoresTest() const;
    void MeshLoaderTest() const;
    void TextureLoaderTest() const;
};