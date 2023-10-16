#pragma once

#include "TestSceneBase001.h"



class TestScene002 : public TestSceneBase001
{
public:
    TestScene002();
    ~TestScene002();

    TestScene002(TestScene002& other) = delete;
    void operator=(const TestScene002&) = delete;

    const char* Name() override;

protected:
    bool OnInitialize() override;

    bool OnDeinitialize() override;

    void OnHandleEvent(Project001::Event& event) override;

    void ClearResources();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

    // Texture Ids -------------------------------------------------------------

    unsigned int dice01TextureId_;
    unsigned int dice02TextureId_;
    unsigned int thonkTextureId_;
    unsigned int _100x100TextureId_;
    unsigned int patternSpecular01TextureId_;
    unsigned int patternSpecular02TextureId_;
    unsigned int thonkSpecularTextureId_;

private:
    void BiMapTest() const;
    void ComponentContainerTest() const;
    void ComponentStoresTest() const;
    void MeshLoaderTest() const;
    void TextureLoaderTest() const;
};