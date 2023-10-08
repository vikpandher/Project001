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

    Project001::SoundData soundData01_;
    unsigned int soundBufferId01_;
    unsigned int soundSourceId01_;

    Project001::SoundData soundData02_;
    unsigned int soundBufferId02_;
    unsigned int soundSourceId02_;

private:
    // Tests Play, Pause, Stop, SoundSource Position, and Listener Position
    void TestSoundPlayer();
};