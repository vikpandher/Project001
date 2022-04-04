#pragma once

#include "Engine/Math/Overlap3D.h"



namespace Project001
{
    // -------------------------------------------------------------------------

    int UnitTest_Check3D_Point_Point_Overlap();

    int UnitTest_Check3D_Point_Line_Overlap();

    int UnitTest_Check3D_Point_Ray_Overlap();

    int UnitTest_Check3D_Point_LineSegment_Overlap();

    int UnitTest_Check3D_Point_Plane_Overlap();

    int UnitTest_Check3D_Point_Triangle_Overlap();

    int UnitTest_Check3D_Point_AABB_Overlap();

    int UnitTest_Check3D_Point_OBB_Overlap();

    int UnitTest_Check3D_Point_Sphere_Overlap();

    // -------------------------------------------------------------------------

    int UnitTest_Check3D_Line_Line_Overlap();

    int UnitTest_Check3D_Line_Ray_Overlap();

    int UnitTest_Check3D_Line_LineSegment_Overlap();

    int UnitTest_Check3D_Line_Plane_Overlap();

    int UnitTest_Check3D_Line_Triangle_Overlap();

    // -------------------------------------------------------------------------

    int UnitTest_Check3D_Ray_Ray_Overlap();

    int UnitTest_Check3D_Ray_LineSegment_Overlap();

    int UnitTest_Check3D_Ray_Plane_Overlap();

    int UnitTest_Check3D_Ray_Triangle_Overlap();

    // -------------------------------------------------------------------------

    int UnitTest_Check3D_LineSegment_LineSegment_Overlap();

    int UnitTest_Check3D_LineSegment_Plane_Overlap();

    int UnitTest_Check3D_LineSegment_Triangle_Overlap();

    // -------------------------------------------------------------------------

    int UnitTest_Check3D_Plane_Plane_Overlap();

    // -------------------------------------------------------------------------

    int UnitTest_Get3D_Point_Line_ClosestPoint();

    int UnitTest_Get3D_Point_Ray_ClosestPoint();

    int UnitTest_Get3D_Point_LineSegment_ClosestPoint();

    int UnitTest_Get3D_Point_Plane_ClosestPoint();

    int UnitTest_Get3D_Point_Triangle_ClosestPoint();

    int UnitTest_Get3D_Point_AABB_ClosestPoint();

    int UnitTest_Get3D_Point_OBB_ClosestPoint();

    int UnitTest_Get3D_Point_Sphere_ClosestPoint();

    // -------------------------------------------------------------------------
}

#include "Overlap3D_UnitTests.inl"