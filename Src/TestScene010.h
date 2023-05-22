#pragma once

#include "TestSceneBase002.h"



class TestScene010 : public TestSceneBase002
{
public:
    TestScene010();
    ~TestScene010();

    TestScene010(TestScene010& other) = delete;
    void operator=(const TestScene010&) = delete;

    const char* Name() override;

protected:
    bool OnInitialize() override;

    bool OnDeinitialize() override;

    void OnHandleEvent(Project001::Event& event) override;

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

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