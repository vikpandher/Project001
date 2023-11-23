#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase001.h"



namespace Project001
{
    struct SoundData;
}

class TestScene050 : public TestSceneBase001
{
public:
    TestScene050(Project001::Application* applicationPtr);
    ~TestScene050();

    TestScene050(TestScene050& other) = delete;
    void operator=(const TestScene050&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void UpdateCameraListenerPosition();
    void UpdateShape01EntityPosition(unsigned long long timestep_ns);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

    // SoundData ---------------------------------------------------------------

    Project001::SoundData* sound01_SoundDataPtr_;
    unsigned int soundBufferId01_;
    unsigned int soundSourceId01_;

    Project001::SoundData* sound02_SoundDataPtr_;
    unsigned int soundBufferId02_;
    unsigned int soundSourceId02_;

    // -------------------------------------------------------------------------

    bool playingSound_;

private:
    // Tests Play, Pause, Stop, SoundSource Position, and Listener Position
    void TestSoundPlayer();

    // Tests playing the same sound from two sources at the same time
    void TestSoundPlayer2();
};