#pragma once

#include "TestSceneBase001.h"

#include "Engine/MeshData.h"



namespace Project001
{
    struct KeyEvent;
}

class TestScene004 : public TestSceneBase001
{
public:
    TestScene004();
    ~TestScene004();

    TestScene004(TestScene004& other) = delete;
    void operator=(const TestScene004&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ClearIndiciesAndEntityIds();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

    // Mesh Data: --------------------------------------------------------------

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
    Project001::MeshData shape29Mesh_;
    Project001::MeshData shape30Mesh_;
    Project001::MeshData shape31Mesh_;
    Project001::MeshData shape32Mesh_;
    Project001::MeshData shape33Mesh_;
    Project001::MeshData shape34Mesh_;
    Project001::MeshData shape35Mesh_;

    // Texture Ids: ------------------------------------------------------------

    unsigned int _32x32_00_TextureId_;
    unsigned int _32x32_01_TextureId_;
    unsigned int _32x32_02_TextureId_;
    unsigned int _32x32_03_TextureId_;
    unsigned int _32x32_04_TextureId_;
    unsigned int _32x32_05_TextureId_;
    unsigned int _32x32_06_TextureId_;
    unsigned int _32x32_07_TextureId_;
    unsigned int _32x32_08_TextureId_;
    unsigned int _32x32_09_TextureId_;
    unsigned int _32x32_10_TextureId_;
    unsigned int _32x32_11_TextureId_;
    unsigned int _32x32_12_TextureId_;
    unsigned int _32x32_13_TextureId_;
    unsigned int _32x32_14_TextureId_;
    unsigned int _32x32_15_TextureId_;
    unsigned int _32x32_16_TextureId_;
    unsigned int _32x32_17_TextureId_;
    unsigned int _32x32_18_TextureId_;
    unsigned int _32x32_19_TextureId_;
    unsigned int _32x32_20_TextureId_;
    unsigned int _32x32_21_TextureId_;
    unsigned int _32x32_22_TextureId_;
    unsigned int _32x32_23_TextureId_;
    unsigned int _32x32_24_TextureId_;
    unsigned int _32x32_25_TextureId_;
    unsigned int _32x32_26_TextureId_;
    unsigned int _32x32_27_TextureId_;
    unsigned int _32x32_28_TextureId_;
    unsigned int _32x32_29_TextureId_;
    unsigned int _32x32_30_TextureId_;
    unsigned int _32x32_31_TextureId_;
    unsigned int _32x32_32_TextureId_;
    unsigned int _32x32_33_TextureId_;
    unsigned int _32x32_34_TextureId_;

    // Entity Ids: -------------------------------------------------------------

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
    unsigned int shape29EntityId_;
    unsigned int shape30EntityId_;
    unsigned int shape31EntityId_;
    unsigned int shape32EntityId_;
    unsigned int shape33EntityId_;
    unsigned int shape34EntityId_;
    unsigned int shape35EntityId_;

private:
};