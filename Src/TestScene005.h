#pragma once

#include "TestSceneBase002.h"



class TestScene005 : public TestSceneBase002
{
public:
    TestScene005();
    ~TestScene005();

    TestScene005(TestScene005& other) = delete;
    void operator=(const TestScene005&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
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