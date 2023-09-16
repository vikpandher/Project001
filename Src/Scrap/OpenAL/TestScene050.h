#pragma once

#include "TestSceneBase001.h"



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

    void UpdateShape01EntityPosition(unsigned long long timestep_ns);

    // SoundBuffer Ids: --------------------------------------------------------

    unsigned int  song01SoundBufferId_;

    // SoundSource Ids: --------------------------------------------------------

    unsigned int song01SoundSourceId_;

private:
    void TestOpenAL() const;
};