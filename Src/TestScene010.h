#pragma once

#include "TestSceneBase001.h"


namespace Project001
{
    struct KeyEvent;
}

class TestScene010 : public TestSceneBase001
{
public:
    TestScene010();
    ~TestScene010();

    TestScene010(TestScene010& other) = delete;
    void operator=(const TestScene010&) = delete;

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