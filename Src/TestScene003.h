#pragma once

#include "TestSceneBase001.h"


namespace Project001
{
    struct KeyEvent;
}

class TestScene003 : public TestSceneBase001
{
public:
    TestScene003();
    ~TestScene003();

    TestScene003(TestScene003& other) = delete;
    void operator=(const TestScene003&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ClearIndiciesAndEntityIds();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

    // Mesh Indicies: ----------------------------------------------------------

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
    unsigned int shape17MeshIndex_;
    unsigned int shape18MeshIndex_;
    unsigned int shape19MeshIndex_;
    unsigned int shape20MeshIndex_;
    unsigned int shape21MeshIndex_;
    unsigned int shape22MeshIndex_;
    unsigned int shape23MeshIndex_;
    unsigned int shape24MeshIndex_;
    unsigned int shape25MeshIndex_;
    unsigned int shape26MeshIndex_;
    unsigned int shape27MeshIndex_;
    unsigned int shape28MeshIndex_;
    unsigned int shape29MeshIndex_;
    unsigned int shape30MeshIndex_;
    unsigned int shape31MeshIndex_;
    unsigned int shape32MeshIndex_;
    unsigned int shape33MeshIndex_;
    unsigned int shape34MeshIndex_;
    unsigned int shape35MeshIndex_;

    // Texture Indicies: -------------------------------------------------------

    unsigned int _32x32_00_TextureIndex_;
    unsigned int _32x32_01_TextureIndex_;
    unsigned int _32x32_02_TextureIndex_;
    unsigned int _32x32_03_TextureIndex_;
    unsigned int _32x32_04_TextureIndex_;
    unsigned int _32x32_05_TextureIndex_;
    unsigned int _32x32_06_TextureIndex_;
    unsigned int _32x32_07_TextureIndex_;
    unsigned int _32x32_08_TextureIndex_;
    unsigned int _32x32_09_TextureIndex_;
    unsigned int _32x32_10_TextureIndex_;
    unsigned int _32x32_11_TextureIndex_;
    unsigned int _32x32_12_TextureIndex_;
    unsigned int _32x32_13_TextureIndex_;
    unsigned int _32x32_14_TextureIndex_;
    unsigned int _32x32_15_TextureIndex_;
    unsigned int _32x32_16_TextureIndex_;
    unsigned int _32x32_17_TextureIndex_;
    unsigned int _32x32_18_TextureIndex_;
    unsigned int _32x32_19_TextureIndex_;
    unsigned int _32x32_20_TextureIndex_;
    unsigned int _32x32_21_TextureIndex_;
    unsigned int _32x32_22_TextureIndex_;
    unsigned int _32x32_23_TextureIndex_;
    unsigned int _32x32_24_TextureIndex_;
    unsigned int _32x32_25_TextureIndex_;
    unsigned int _32x32_26_TextureIndex_;
    unsigned int _32x32_27_TextureIndex_;
    unsigned int _32x32_28_TextureIndex_;
    unsigned int _32x32_29_TextureIndex_;
    unsigned int _32x32_30_TextureIndex_;
    unsigned int _32x32_31_TextureIndex_;
    unsigned int _32x32_32_TextureIndex_;
    unsigned int _32x32_33_TextureIndex_;
    unsigned int _32x32_34_TextureIndex_;

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