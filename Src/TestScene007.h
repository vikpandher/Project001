#pragma once

#include "TestSceneBase001.h"



namespace Project001
{
    struct MeshData;

    struct KeyEvent;
}

class TestScene007 : public TestSceneBase001
{
public:
    TestScene007();
    ~TestScene007();

    TestScene007(TestScene007& other) = delete;
    void operator=(const TestScene007&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ClearResources();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

    // Texture Ids: ------------------------------------------------------------

    unsigned int _32x32_123abc_TextureId_;

private:
};