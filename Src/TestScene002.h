#pragma once

#include "TestSceneFramework.h"


namespace Project001
{
    struct KeyEvent;
}

class TestScene002 : public TestSceneFramework
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

    unsigned int earthTextureIndex_;
    unsigned int rgb120x60TextureIndex_;

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