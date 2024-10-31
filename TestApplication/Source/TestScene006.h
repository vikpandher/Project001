// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase001.h"



class TestScene006 : public TestSceneBase001
{
public:
    TestScene006(Project001::Application* applicationPtr);
    ~TestScene006();

    TestScene006(TestScene006& other) = delete;
    void operator=(const TestScene006&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

    // Entity Ids --------------------------------------------------------------

    unsigned int zzzEntityId_;
    unsigned int printableEntityId_;
    unsigned int sphinxEntityId_;

    // -------------------------------------------------------------------------

    unsigned long long timestamp_ns_;
};