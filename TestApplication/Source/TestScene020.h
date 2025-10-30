// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-30

#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase002.h"



class TestScene020 : public TestSceneBase002
{
public:
    TestScene020(Project001::Application* applicationPtr);
    ~TestScene020();

    TestScene020(TestScene020& other) = delete;
    void operator=(const TestScene020&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

private:
    void Test_GetCameraRollPitchYaw() const;

    void Test_CartesianToPolar() const;
    void Test_PolarToCartesian() const;
    void Test_Get2DVectorAngle() const;

    void TestCollision2D() const;
    void Test_Get2D_Point_Line_Distance() const;
    void Test_Get2D_Point_LineSegment_Distance() const;
    void Test_RotateSlope() const;

    void Test_Misc() const;
};