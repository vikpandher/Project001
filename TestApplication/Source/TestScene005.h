// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-11-06

#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase001.h"



class TestScene005 : public TestSceneBase001
{
public:
    TestScene005(Project001::Application* applicationPtr);
    ~TestScene005();

    TestScene005(TestScene005& other) = delete;
    void operator=(const TestScene005&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

private:
    void TestOrderPointsCCW();
    void TestRemoveDuplicates();
};