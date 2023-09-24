#pragma once

#include "TestSceneBase001.h"

#include "Engine/SoundData.h"



namespace Project001
{
    struct KeyEvent;
}

class TestScene050 : public TestSceneBase001
{
public:
    TestScene050();
    ~TestScene050();

    TestScene050(TestScene050& other) = delete;
    void operator=(const TestScene050&) = delete;

    const char* Name() override;

protected:
    bool OnInitialize() override;

    bool OnDeinitialize() override;

    void OnHandleEvent(Project001::Event& event) override;

    void ClearResources();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void UpdateCameraListenerPosition();
    void UpdateShape01EntityPosition(unsigned long long timestep_ns);

    // SoundBuffer & SoundSource Ids -------------------------------------------

    Project001::SoundData soundData01;
    unsigned int soundBufferId01;
    unsigned int soundSourceId01;

    Project001::SoundData soundData02;
    unsigned int soundBufferId02;
    unsigned int soundSourceId02;

private:
    // Tests Play, Pause, Stop, SoundSource Position, and Listener Position
    void TestSoundPlayer();
};