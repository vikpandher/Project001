#pragma once

#include "TestSceneFramework.h"

#include <map>
#include <vector>


namespace Project001
{
    struct KeyEvent;
}

class TestScene004 : public TestSceneFramework
{
public:
    TestScene004();
    ~TestScene004();

    TestScene004(TestScene004& other) = delete;
    void operator=(const TestScene004&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ClearIndiciesAndEntityIds();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent);
    void ProcessScrollEvent(Project001::ScrollEvent& scrollEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void UpdatedSelectedEntityPosition(unsigned long timestep_ns);

    void DetectCollisions();

    void SyncCollisionComponents();

    // Mesh Indicies: ----------------------------------------------------------
    std::vector<unsigned int> meshIndicies_;

    // Texture Indicies: -------------------------------------------------------

    // Entity Ids: -------------------------------------------------------------
    std::vector<unsigned int> entityIds_;

    unsigned int selectedEntityIdIndex_;

private:
    void Test_CartesianToPolar() const;
    void Test_PolarToCartesian() const;
    void Test_Get2DVectorAngle() const;

    void TestCollision2D() const;
    void Test_Get2D_Point_Line_Distance() const;
    void Test_Get2D_Point_LineSegment_Distance() const;
    void Test_RotateSlope() const;

    void Test_Misc() const;
};