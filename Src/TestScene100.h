#pragma once

#include "TestSceneBase001.h"



namespace Project001
{
    struct KeyEvent;
}

class TestScene100 : public TestSceneBase001
{
public:
    TestScene100();
    ~TestScene100();

    TestScene100(TestScene100& other) = delete;
    void operator=(const TestScene100&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ClearResources();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

    // Texture Ids: ------------------------------------------------------------

    unsigned int textureId00_;

private:
};