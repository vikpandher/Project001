#pragma once

#include "TestSceneBase001.h"


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
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void UpdateShape01EntityPosition(unsigned long timestep_ns);

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