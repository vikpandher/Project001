#pragma once

#include "TestSceneBase001.h"


namespace Project001
{
    struct KeyEvent;
}

class TestScene001 : public TestSceneBase001
{
public:
    TestScene001();
    ~TestScene001();

    TestScene001(TestScene001& other) = delete;
    void operator=(const TestScene001&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ClearIndiciesAndEntityIds();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

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

    // Texture Indicies: -------------------------------------------------------

    unsigned int diceTexture01Index_;
    unsigned int diceTexture02Index_;
    unsigned int thonkTextureIndex_;
    unsigned int _100x100TextureIndex_;
    unsigned int patternSpecularTexture01Index_;
    unsigned int patternSpecularTexture02Index_;
    unsigned int thonkSpecularTextureIndex_;

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
    void MeshStoresTest() const;
    void TextureStoresTest() const;
};