#pragma once

#include "TestSceneFramework.h"


namespace Project001
{
    struct KeyEvent;
}

class TestScene003 : public TestSceneFramework
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
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void UpdateShape01EntityPosition(double timestep);

    // Mesh Indicies: ----------------------------------------------------------

    unsigned int shape01MeshIndex_;

    // Sound Indicies: ---------------------------------------------------------

    unsigned int song01SoundIndex_;

    // Texture Indicies: -------------------------------------------------------

    // Entity Ids: -------------------------------------------------------------

    unsigned int shape01EntityId_;

private:
    void TestOpenAL() const;
};