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

protected:
    bool OnInitialize() override;

    bool OnDeinitialize() override;

    void OnHandleEvent(Project001::Event& event) override;

    void ClearResources();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

    // Texture Ids: ------------------------------------------------------------

    unsigned int earthTextureId_;
    unsigned int rgb120x60TextureId_;

private:
};