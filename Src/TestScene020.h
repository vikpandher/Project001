#pragma once

#include "TestSceneBase001.h"

#include "Engine/MeshData.h"



namespace Project001
{
    struct KeyEvent;
}

class TestScene020 : public TestSceneBase001
{
public:
    TestScene020();
    ~TestScene020();

    TestScene020(TestScene020& other) = delete;
    void operator=(const TestScene020&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ClearResources();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void UpdateShape01EntityPosition(unsigned long timestep_ns);

    // Mesh Data: --------------------------------------------------------------

    Project001::MeshData shape01MeshData_;

    // SoundBuffer Ids: --------------------------------------------------------

    unsigned int  song01SoundBufferId_;

    // SoundSource Ids: --------------------------------------------------------

    unsigned int song01SoundSourceId_;

    // Texture Ids: ------------------------------------------------------------

    // Entity Ids: -------------------------------------------------------------

    unsigned int shape01EntityId_;

private:
    void TestOpenAL() const;
};