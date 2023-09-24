#pragma once

#include "TestSceneBase001.h"

#include "Engine/FontData.h"
#include "Engine/TextureData.h"



namespace Project001
{
    struct KeyEvent;
}

class TestScene006 : public TestSceneBase001
{
public:
    TestScene006();
    ~TestScene006();

    TestScene006(TestScene006& other) = delete;
    void operator=(const TestScene006&) = delete;

    const char* Name() override;

protected:
    bool OnInitialize() override;

    bool OnDeinitialize() override;

    void OnHandleEvent(Project001::Event& event) override;

    void ClearResources();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

    // Font Data ---------------------------------------------------------------

    Project001::FontData fontData_;

    // Texture Data ------------------------------------------------------------

    Project001::TextureData fontTextureData_;

    // Texture Ids -------------------------------------------------------------

    unsigned int fontTextureId_;

private:
};