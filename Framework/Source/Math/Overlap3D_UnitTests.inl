// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30



namespace Project001
{
    inline int UnitTest_Check3D_Point_Point_Overlap()
    {
        // 01: two points at origin
        {
            bool result = Check3D_Point_Point_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f));

            if (result != true) return 1;
        }

        // 02: two points on top of each other
        {
            bool result = Check3D_Point_Point_Overlap(
                glm::vec3(-1.5f, 0.5f, 2.0f),
                glm::vec3(-1.5f, 0.5f, 2.0f));

            if (result != true) return 2;
        }

        // 03: two different points
        {
            bool result = Check3D_Point_Point_Overlap(
                glm::vec3(-2.5f, 1.5f, 4.0f),
                glm::vec3(-1.0f, 0.0f, -1.0f));

            if (result != false) return 3;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Point_Line_Overlap()
    {
        // 01: origin point and line through origin
        {
            bool result = Check3D_Point_Line_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f));

            if (result != true) return 1;
        }

        // 02: origin point and line through origin v2
        {
            bool result = Check3D_Point_Line_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));

            if (result != true) return 2;
        }

        // 03: point and line intersecting it
        {
            bool result = Check3D_Point_Line_Overlap(
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));

            if (result != true) return 3;
        }

        // 04: point and line not intersecting it
        {
            bool result = Check3D_Point_Line_Overlap(
                glm::vec3(0.0f, -1.0f, -1.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));

            if (result != false) return 4;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Point_Ray_Overlap()
    {
        // 01: origin point and ray through origin, same position
        {
            bool result = Check3D_Point_Ray_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f));

            if (result != true) return 1;
        }

        // 02: origin point and ray through origin
        {
            bool result = Check3D_Point_Ray_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 0.0f));

            if (result != true) return 2;
        }

        // 03: origin point and ray not through origin
        {
            bool result = Check3D_Point_Ray_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));

            if (result != false) return 3;
        }

        // 04: point and ray intersecting it
        {
            bool result = Check3D_Point_Ray_Overlap(
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)));

            if (result != true) return 4;
        }

        // 05: point and ray not intersecting it
        {
            bool result = Check3D_Point_Ray_Overlap(
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));

            if (result != false) return 5;
        }

        // 06: point and ray not intersecting it
        {
            bool result = Check3D_Point_Ray_Overlap(
                glm::vec3(0.0f, -1.0f, -1.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)));

            if (result != false) return 6;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Point_LineSegment_Overlap()
    {
        // 01: origin point and lineSegment through origin
        {
            bool result = Check3D_Point_LineSegment_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f));

            if (result != true) return 1;
        }

        // 02: point and lineSegment collide
        {
            bool result = Check3D_Point_LineSegment_Overlap(
                glm::vec3(10.0f, 10.0f, 10.0f),
                glm::vec3(-100.0f, -100.0f, -100.0f),
                glm::vec3(11.0f, 11.0f, 11.0f));

            if (result != true) return 2;
        }

        // 03: point and lineSegment are aligned, but don't collide
        {
            bool result = Check3D_Point_LineSegment_Overlap(
                glm::vec3(10.0f, 10.0f, 10.0f),
                glm::vec3(-100.0f, -100.0f, -100.0f),
                glm::vec3(9.0f, 9.0f, 9.0f));

            if (result != false) return 3;
        }

        // 04: point and lineSegment collide at end
        {
            bool result = Check3D_Point_LineSegment_Overlap(
                glm::vec3(10.0f, 10.0f, 10.0f),
                glm::vec3(-100.0f, -100.0f, -100.0f),
                glm::vec3(10.0f, 10.0f, 10.0f));

            if (result != true) return 4;
        }

        // 05: point and lineSegment collide at start
        {
            bool result = Check3D_Point_LineSegment_Overlap(
                glm::vec3(-10.0f, -10.0f, -10.0f),
                glm::vec3(-10.0f, -10.0f, -10.0f),
                glm::vec3(10.0f, 10.0f, 10.0f));

            if (result != true) return 5;
        }

        // 06: point and lineSegment don't collide
        {
            bool result = Check3D_Point_LineSegment_Overlap(
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(3.0f, 3.0f, 4.0f));

            if (result != false) return 6;
        }

        // 07: point and lineSegment don't collide
        {
            bool result = Check3D_Point_LineSegment_Overlap(
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(-3.0f, 3.0f, 2.0f),
                glm::vec3(3.0f, -3.0f, 2.0f));

            if (result != false) return 7;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Point_Plane_Overlap()
    {
        // 01: origin point origin XY plane
        {
            bool result = Check3D_Point_Plane_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                0.0f);

            if (result != true) return 1;
        }

        // 02: point not collliding with XY plane
        {
            bool result = Check3D_Point_Plane_Overlap(
                glm::vec3(0.0f, 0.0f, -1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                0.0f);

            if (result != false) return 2;
        }

        // 03: point not collliding with XY plane version 2
        {
            bool result = Check3D_Point_Plane_Overlap(
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                0.0f);

            if (result != false) return 3;
        }

        // 04: point collliding with a plane
        {
            bool result = Check3D_Point_Plane_Overlap(
                glm::vec3(0.0f, glm::sqrt(2.0f), 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                1.0f);

            if (result != true) return 4;
        }

        // 05: point not collliding with a plane
        {
            bool result = Check3D_Point_Plane_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                1.0f);

            if (result != false) return 5;
        }

        // 06: point not collliding with a plane
        {
            bool result = Check3D_Point_Plane_Overlap(
                glm::vec3(0.0f, 2.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                1.0f);

            if (result != false) return 6;
        }

        // 07: origin point origin XY plane v2
        {
            bool result = Check3D_Point_Plane_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, -1.0f),
                0.0f);

            if (result != true) return 7;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Point_Triangle_Overlap()
    {
        // 01: origin point and triangle on origin
        {
            bool result = Check3D_Point_Triangle_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(-1.0f, 0.0f, -1.0f));

            if (result != true) return 1;
        }

        // 02: origin point and triangle thats infinitly small at origin
        {
            bool result = Check3D_Point_Triangle_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f));

            if (result != true) return 2;
        }

        // 03: origin point and triangle corner1 on origin
        {
            bool result = Check3D_Point_Triangle_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(-1.0f, 0.0f, -1.0f));

            if (result != true) return 3;
        }

        // 04: origin point and triangle corner2 on origin
        {
            bool result = Check3D_Point_Triangle_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, 0.0f, -1.0f));

            if (result != true) return 4;
        }

        // 05: origin point and triangle corner3 on origin
        {
            bool result = Check3D_Point_Triangle_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f));

            if (result != true) return 5;
        }

        // 06: point and triangle collision
        {
            bool result = Check3D_Point_Triangle_Overlap(
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 1.0f),
                glm::vec3(-1.0f, 1.0f, -1.0f));

            if (result != true) return 6;
        }

        // 07: point and triangle, no collision
        {
            bool result = Check3D_Point_Triangle_Overlap(
                glm::vec3(0.0f, -1.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 1.0f),
                glm::vec3(-1.0f, 1.0f, -1.0f));

            if (result != false) return 7;
        }

        // 08: point and triangle, no collision
        {
            bool result = Check3D_Point_Triangle_Overlap(
                glm::vec3(0.9f, -1.0f, 0.9f),
                glm::vec3(1.0f, -1.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 1.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f));

            if (result != false) return 8;
        }

        // 09: point and triangle, no collision
        {
            bool result = Check3D_Point_Triangle_Overlap(
                glm::vec3(-0.9f, -1.0f, 0.0f),
                glm::vec3(1.0f, -1.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 1.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f));

            if (result != false) return 9;
        }

        // 10: point and triangle, edge collision
        {
            bool result = Check3D_Point_Triangle_Overlap(
                glm::vec3(0.5f, -1.0f, 0.5f),
                glm::vec3(1.0f, -1.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 1.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f));

            if (result != true) return 10;
        }

        // 11: point and triangle thats infinitly small, no collision
        {
            bool result = Check3D_Point_Triangle_Overlap(
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f));

            if (result != false) return 11;
        }

        // 12: point on triangle corner
        {
            bool result = Check3D_Point_Triangle_Overlap(
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f));

            if (result != true) return 12;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Point_AABB_Overlap()
    {
        // 01: point in AABB
        {
            bool result = Check3D_Point_AABB_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(-0.5f, -0.5f, -0.5f),
                glm::vec3(0.5f, 0.5f, 0.5f));

            if (result != true) return 1;
        }

        // 02: point in AABB, corner
        {
            bool result = Check3D_Point_AABB_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.5f, 0.5f, 0.5f));

            if (result != true) return 2;
        }

        // 03: point in tiny AABB
        {
            bool result = Check3D_Point_AABB_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f));

            if (result != true) return 3;
        }

        // 04: point not in AABB
        {
            bool result = Check3D_Point_AABB_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(-0.5f, -0.5f, -0.5f),
                glm::vec3(0.5f, 0.5f, 0.5f));

            if (result != false) return 4;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Point_OBB_Overlap()
    {
        // 01: point in OBB
        {
            bool result = Check3D_Point_OBB_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.5f, 0.5f, 0.5f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

            if (result != true) return 1;
        }

        // 02: point in rotated OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));

            bool result = Check3D_Point_OBB_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.5f, 1.5f, 1.5f),
                glm::vec3(2.0f, 0.0f, 0.0f),
                rotation);

            if (result != true) return 2;
        }

        // 03: point in rotated OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));

            bool result = Check3D_Point_OBB_Overlap(
                glm::vec3(0.0f, 1.5f, 0.0f),
                glm::vec3(1.0f, 3.0f, 1.0f),
                glm::vec3(2.0f, 0.0f, 0.0f),
                rotation);

            if (result != true) return 3;
        }

        // 04: point not in rotated OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                -glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));

            bool result = Check3D_Point_OBB_Overlap(
                glm::vec3(0.0f, 1.5f, 0.0f),
                glm::vec3(1.0f, 3.0f, 1.0f),
                glm::vec3(2.0f, 0.0f, 0.0f),
                rotation);

            if (result != false) return 4;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Point_Sphere_Overlap()
    {
        // 01: point in sphere at origin
        {
            bool result = Check3D_Point_Sphere_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                1.0f);

            if (result != true) return 1;
        }

        // 02: point and sphere edge collision
        {
            bool result = Check3D_Point_Sphere_Overlap(
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, 2.0f, 1.0f),
                1.0f);

            if (result != true) return 2;
        }

        // 03: point in infinitly small sphere at origin
        {
            bool result = Check3D_Point_Sphere_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                0.0f);

            if (result != true) return 3;
        }

        // 04: point and sphere no collision
        {
            bool result = Check3D_Point_Sphere_Overlap(
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                1.0f);

            if (result != false) return 4;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Point_Capsule_Overlap()
    {
        // 01: point in infinitly small capsule at origin
        {
            bool result = Check3D_Point_Capsule_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                0.0f);

            if (result != true) return 1;
        }

        // 02: point in infinitly thin capsule at origin v2
        {
            bool result = Check3D_Point_Capsule_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                0.0f);

            if (result != true) return 2;
        }

        // 03: point in sphere like capsule
        {
            bool result = Check3D_Point_Capsule_Overlap(
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                1.0f);

            if (result != true) return 3;
        }

        // 04: point on capsule edge
        {
            bool result = Check3D_Point_Capsule_Overlap(
                glm::vec3(2.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                1.0f);

            if (result != true) return 4;
        }

        // 05: point outside capsule end
        {
            bool result = Check3D_Point_Capsule_Overlap(
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                1.0f);

            if (result != false) return 5;
        }

        // 06: point inside capsule
        {
            bool result = Check3D_Point_Capsule_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                2.0f);

            if (result != true) return 6;
        }

        return 0;
    }

    // -------------------------------------------------------------------------

    inline int UnitTest_Check3D_Line_Line_Overlap()
    {
        // 01: 2 lines not on the same plane
        {
            bool result = Check3D_Line_Line_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));

            if (result != false) return 1;
        }

        // 02: 2 lines that are parallel
        {
            bool result = Check3D_Line_Line_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));

            if (result != false) return 2;
        }

        // 03: 2 lines that are on the same plane
        {
            bool result = Check3D_Line_Line_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));

            if (result != true) return 3;
        }

        // 04: 2 lines that share a point
        {
            bool result = Check3D_Line_Line_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 0.5f, 0.1f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));

            if (result != true) return 4;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Line_Ray_Overlap()
    {
        // 01: line and ray not on the same plane
        {
            bool result = Check3D_Line_Ray_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));

            if (result != false) return 1;
        }

        // 02: line and ray share position point
        {
            bool result = Check3D_Line_Ray_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));

            if (result != true) return 2;
        }

        // 03: line and ray are are parallel
        {
            bool result = Check3D_Line_Ray_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));

            if (result != false) return 3;
        }

        // 04: line and ray intersect
        {
            bool result = Check3D_Line_Ray_Overlap(
                glm::vec3(2.0f, 2.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                glm::vec3(-2.0f, 4.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f));

            if (result != true) return 4;
        }

        // 05: line and ray intersect at ray start
        {
            bool result = Check3D_Line_Ray_Overlap(
                glm::vec3(2.0f, -1.0f,0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(2.0f, 2.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));

            if (result != true) return 5;
        }

        // 06: line and ray don't intersect becuause ray direction
        {
            bool result = Check3D_Line_Ray_Overlap(
                glm::vec3(2.0f, 2.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                glm::vec3(2.0f, -1.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 0.0f));

            if (result != false) return 6;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Line_LineSegment_Overlap()
    {
        // 01: line and lineSegment not on the same plane
        {
            bool result = Check3D_Line_LineSegment_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(1.0f, 0.0f, 1.0f));

            if (result != false) return 1;
        }

        // 02: line and lineSegment parallel
        {
            bool result = Check3D_Line_LineSegment_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f));

            if (result != false) return 2;
        }

        // 03: 
        {
            bool result = Check3D_Line_LineSegment_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)),
                glm::vec3(-4.0f, 2.0f, 0.0f),
                glm::vec3(-2.0f, 0.0f, 0.0f));

            if (result != false) return 3;
        }

        // 04: 
        {
            bool result = Check3D_Line_LineSegment_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)),
                glm::vec3(-3.0f, 0.0f, 0.0f),
                glm::vec3(-3.0f, -2.0f, 0.0f));

            if (result != true) return 4;
        }

        // 05: 
        {
            bool result = Check3D_Line_LineSegment_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)),
                glm::vec3(-1.0f, 4.0f, 0.0f),
                glm::vec3(-1.0f, 1.0f, 0.0f));

            if (result != false) return 5;
        }

        // 06: 
        {
            bool result = Check3D_Line_LineSegment_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(-2.0f, -2.0f, 0.0f));

            if (result != false) return 6;
        }

        // 07: 
        {
            bool result = Check3D_Line_LineSegment_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)),
                glm::vec3(1.0f, 3.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 0.0f));

            if (result != true) return 7;
        }

        // 08: 
        {
            bool result = Check3D_Line_LineSegment_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)),
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::vec3(2.0f, 1.0f, 0.0f));

            if (result != true) return 8;
        }

        // 09: 
        {
            bool result = Check3D_Line_LineSegment_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)),
                glm::vec3(3.0f, 6.0f, 0.0f),
                glm::vec3(2.0f, 2.0f, 0.0f));

            if (result != false) return 9;
        }

        // 10: 
        {
            bool result = Check3D_Line_LineSegment_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)),
                glm::vec3(3.0f, 3.0f, 0.0f),
                glm::vec3(3.0f, 1.0f, 0.0f));

            if (result != true) return 10;
        }

        // 11: 
        {
            bool result = Check3D_Line_LineSegment_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)),
                glm::vec3(5.0f, 3.0f, 0.0f),
                glm::vec3(5.0f, 1.0f, 0.0f));

            if (result != true) return 11;
        }

        // 12: Parallel and on top of each other
        {
            bool result = Check3D_Line_LineSegment_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 0.0f, 0.0f));

            if (result != true) return 12;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Line_Plane_Overlap()
    {
        // 01: line is parallel to the plane
        {
            bool result = Check3D_Line_Plane_Overlap(
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                0.0f);

            if (result != false) return 1;
        }

        // 02: line is parallel to the plane and on the plane
        {
            bool result = Check3D_Line_Plane_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                0.0f);

            if (result != true) return 2;
        }

        // 03: line and plane overlap
        {
            bool result = Check3D_Line_Plane_Overlap(
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(0.0f, 1.0f, 0.0f),
                0.0f);

            if (result != true) return 3;
        }

        // 04: line is parallel to the plane v2
        {
            bool result = Check3D_Line_Plane_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                0.0f);

            if (result != false) return 4;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Line_Triangle_Overlap()
    {
        // 01: line is parallel to the triangle
        {
            bool result = Check3D_Line_Triangle_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f)),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f));

            if (result != false) return 1;
        }

        // 02: line misses to the triangle
        {
            bool result = Check3D_Line_Triangle_Overlap(
                glm::vec3(1.1f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f));

            if (result != false) return 2;
        }

        // 03: line overlaps triangle corner
        {
            bool result = Check3D_Line_Triangle_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f));

            if (result != true) return 3;
        }

        // 04: line overlaps triangle center
        {
            bool result = Check3D_Line_Triangle_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f));

            if (result != true) return 4;
        }

        // 05: line overlaps triangle center v2
        {
            bool result = Check3D_Line_Triangle_Overlap(
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f));

            if (result != true) return 5;
        }

        // 06: line is parallel to the triangle
        {
            bool result = Check3D_Line_Triangle_Overlap(
                glm::vec3(-1.0f, 0.0f, 2.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f)),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f));

            if (result != true) return 6;
        }

        // 07: line is not parallel to the triangle
        {
            bool result = Check3D_Line_Triangle_Overlap(
                glm::vec3(-1.0f, 0.0f, 2.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f));

            if (result != false) return 7;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Line_AABB_Overlap()
    {
        // 01: line passing through a point AABB at origin
        {
            bool result = Check3D_Line_AABB_Overlap(
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f));

            if (result != true) return 1;
        }

        // 02: line passing through a point AABB
        {
            bool result = Check3D_Line_AABB_Overlap(
                glm::vec3(-1.0f, 0.0f, 1.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(0.0f, 1.0f, 2.0f),
                glm::vec3(0.0f, 1.0f, 2.0f));

            if (result != true) return 2;
        }

        // 03: line passing through a AABB
        {
            bool result = Check3D_Line_AABB_Overlap(
                glm::vec3(0.2f, 0.2f, 0.2f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != true) return 3;
        }

        // 04: line passing through a AABB
        {
            bool result = Check3D_Line_AABB_Overlap(
                glm::vec3(0.2f, 0.2f, 0.2f),
                glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != true) return 4;
        }

        // 05: line NOT passing through a AABB
        {
            bool result = Check3D_Line_AABB_Overlap(
                glm::vec3(0.2f, 0.2f, 0.2f),
                glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != false) return 5;
        }

        // 06: line NOT passing through a AABB
        {
            bool result = Check3D_Line_AABB_Overlap(
                glm::vec3(0.2f, 0.2f, 0.2f),
                glm::normalize(glm::vec3(1.0f, -1.0f, -1.0f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != false) return 6;
        }

        // 07: line passing through AABB corner
        {
            bool result = Check3D_Line_AABB_Overlap(
                glm::vec3(-0.1f, 0.2f, 0.3f),
                glm::normalize(glm::vec3(0.1f, 0.1f, -0.1f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != true) return 7;
        }

        // 08: line passing through AABB corner
        {
            bool result = Check3D_Line_AABB_Overlap(
                glm::vec3(0.0f, 0.3f, 0.2f),
                glm::normalize(glm::vec3(0.1f, 0.1f, -0.1f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != true) return 8;
        }

        // 09: line NOT passing through AABB
        {
            bool result = Check3D_Line_AABB_Overlap(
                glm::vec3(0.0f, 0.31f, 0.2f),
                glm::normalize(glm::vec3(0.1f, 0.1f, -0.1f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != false) return 9;
        }

        // 10: line passing through AABB edge
        {
            bool result = Check3D_Line_AABB_Overlap(
                glm::vec3(0.3f, 0.2f, 0.3f),
                glm::normalize(glm::vec3(0.1f, 0.0f, 0.0f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != true) return 10;
        }

        // 11: line passing through AABB edge
        {
            bool result = Check3D_Line_AABB_Overlap(
                glm::vec3(0.3f, 0.2f, 0.3f),
                glm::normalize(glm::vec3(-0.1f, 0.0f, 0.0f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));
        
            if (result != true) return 11;
        }

        // 12: line passing on AABB side
        {
            bool result = Check3D_Line_AABB_Overlap(
                glm::vec3(0.3f, 0.2f, 0.2f),
                glm::normalize(glm::vec3(-0.1f, 0.0f, 0.0f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));
        
            if (result != true) return 12;
        }

        // 13: line passing inside AABB side
        {
            bool result = Check3D_Line_AABB_Overlap(
                glm::vec3(0.3f, 0.1999f, 0.2f),
                glm::normalize(glm::vec3(-0.1f, 0.0f, 0.0f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != true) return 13;
        }

        // 14: line passing outside AABB side
        {
            bool result = Check3D_Line_AABB_Overlap(
                glm::vec3(0.3f, 0.2001f, 0.2f),
                glm::normalize(glm::vec3(-0.1f, 0.0f, 0.0f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != false) return 14;
        }

        // 15: line passing through point AABB
        {
            bool result = Check3D_Line_AABB_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(-0.1f, 0.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(10.0f, 0.0f, 0.0f));

            if (result != true) return 15;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Line_OBB_Overlap()
    {
        // 01: line passing through a point OBB at the origin
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_Line_OBB_Overlap(
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                rotation);

            if (result != true) return 1;
        }

        // 02: line passing through a point OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(1.0f, 0.0f, 0.0f));
            bool result = Check3D_Line_OBB_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(-1.0f,0.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(10.0f, 0.0f, 0.0f),
                rotation);

            if (result != true) return 2;
        }

        // 03: line passing NOT through a point OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(1.0f, 0.0f, 0.0f));
            bool result = Check3D_Line_OBB_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(10.0f, 0.001f, 0.0f),
                rotation);

            if (result != false) return 3;
        }

        // 04: line passing through a OBB
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_Line_OBB_Overlap(
                glm::vec3(0.2f, 0.2f, 0.2f),
                glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f)),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != true) return 4;
        }

        // 05: line NOT passing through a OBB
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_Line_OBB_Overlap(
                glm::vec3(0.2f, 0.2f, 0.2f),
                glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f)),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.1f, 0.0f, 0.1f),
                rotation2 * rotation1);

            if (result != false) return 5;
        }

        // 06: line on OBB corner
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_Line_OBB_Overlap(
                glm::vec3(0.562132001f, 0.412132025f, -0.0621320233f),
                glm::normalize(glm::vec3(-1.0f, 1.0f, 1.0f)),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != true) return 6;
        }

        // 07: line going through OBB corner
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_Line_OBB_Overlap(
                glm::vec3(0.0f, 0.412132025f, -0.0621320233f),
                glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f)),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != true) return 7;
        }

        // 08: line going through OBB edge
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_Line_OBB_Overlap(
                glm::vec3(0.0f, 0.412132025f, 0.1f),
                glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f)),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != true) return 8;
        }

        // 09: 
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_Line_OBB_Overlap(
                glm::vec3(0.0, 0.0f, 0.1f),
                glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f)),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != true) return 9;
        }

        // 10: line going along OBB edge
        {
            bool result = Check3D_Line_OBB_Overlap(
                glm::vec3(0.4f, 0.4f, 0.4f),
                glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f)),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

            if (result != true) return 10;
        }

        // 11: line going along OBB edge
        {
            glm::vec3 position00(0.1f, 0.2f, 0.3f);
            glm::vec3 direction00(-1.0f, 0.0f, 0.0f);

            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));

            position00 = rotation1 * position00;
            direction00 = rotation1 * direction00;

            position00 += glm::vec3(0.3f, 0.2f, 0.1f);

            bool result = Check3D_Line_OBB_Overlap(
                position00,
                direction00,
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation1);

            if (result != true) return 11;
        }

        // 12: line going along OBB edge
        {
            glm::vec3 position00(0.1f, 0.2f, 0.3f);
            glm::vec3 direction00(-1.0f, 0.0f, 0.0f);

            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));

            position00 = rotation2 * rotation1 * position00;
            direction00 = rotation2 * rotation1 * direction00;

            position00 += glm::vec3(0.3f, 0.2f, 0.1f);

            bool result = Check3D_Line_OBB_Overlap(
                position00,
                direction00,
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != true) return 12;
        }

        // 13: line going near OBB edge
        {
            glm::vec3 position00(0.1f, 0.2f, 0.3f);
            glm::vec3 direction00(-1.0f, 0.0f, 0.0f);

            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));

            position00 = rotation2 * rotation1 * position00;
            direction00 = rotation2 * rotation1 * direction00;

            position00 += glm::vec3(0.3f, 0.2f, 0.1f);
            position00 += glm::vec3(0.001f, 0.001f, 0.001f);

            bool result = Check3D_Line_OBB_Overlap(
                position00,
                direction00,
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != false) return 13;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Line_Sphere_Overlap()
    {
        // 01: line passing through sphere at origin
        {
            bool result = Check3D_Line_Sphere_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != true) return 1;
        }

        // 02: sphere at origin, line passing through its edge
        {
            bool result = Check3D_Line_Sphere_Overlap(
                glm::vec3(0.0f, 2.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != true) return 2;
        }

        // 03: sphere at origin, line passing not through it
        {
            bool result = Check3D_Line_Sphere_Overlap(
                glm::vec3(0.0f, 3.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != false) return 3;
        }

        // 04: line passing through a sphere
        {
            bool result = Check3D_Line_Sphere_Overlap(
                glm::vec3(0.0f, -1.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f)),
                glm::vec3(2.0f, -2.0f, 0.0f),
                1.0f);

            if (result != true) return 4;
        }

        // 05: line passing through a sphere edge
        {
            bool result = Check3D_Line_Sphere_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)),
                glm::vec3(2.0f, -2.0f, 0.0f),
                1.0f);

            if (result != true) return 5;
        }

        // 06: line not passing through a sphere
        {
            bool result = Check3D_Line_Sphere_Overlap(
                glm::vec3(0.0f, -2.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                glm::vec3(2.0f, -2.0f, 0.0f),
                1.0f);

            if (result != false) return 6;
        }

        return 0;
    }

    // -------------------------------------------------------------------------

    inline int UnitTest_Check3D_Ray_Ray_Overlap()
    {
        // 01: 2 rays not on the same plane
        {
            bool result = Check3D_Ray_Ray_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f)));

            if (result != false) return 1;
        }

        // 02: 2 rays share a position point
        {
            bool result = Check3D_Ray_Ray_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f)));

            if (result != true) return 2;
        }

        // 03: 2 rays are parallel
        {
            bool result = Check3D_Ray_Ray_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f)),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f)));

            if (result != false) return 3;
        }

        // 03: 2 rays are parallel
        {
            bool result = Check3D_Ray_Ray_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f)),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f)));

            if (result != false) return 3;
        }

        // 04: 2 rays not colliding (parallel)
        {
            bool result = Check3D_Ray_Ray_Overlap(
                glm::vec3(2.0f, 2.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                glm::vec3(2.0f, -1.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));

            if (result != false) return 4;
        }

        // 05: 2 rays not colliding
        {
            bool result = Check3D_Ray_Ray_Overlap(
                glm::vec3(2.0f, 2.0f, 0.0f),
                glm::normalize(glm::vec3(-1.0f, 1.0f, 0.0f)),
                glm::vec3(2.0f, -1.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));

            if (result != false) return 5;
        }

        // 06: 2 rays not colliding
        {
            bool result = Check3D_Ray_Ray_Overlap(
                glm::vec3(2.0f, 2.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                glm::vec3(2.0f, -1.0f, 0.0f),
                glm::normalize(glm::vec3(-1.0f, 1.0f, 0.0f)));

            if (result != false) return 6;
        }

        // 07: 2 rays colliding
        {
            bool result = Check3D_Ray_Ray_Overlap(
                glm::vec3(2.0f, 2.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f)),
                glm::vec3(2.0f, -1.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));

            if (result != true) return 7;
        }

        // 08: 2 rays colliding
        {
            bool result = Check3D_Ray_Ray_Overlap(
                glm::vec3(2.0f, 2.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                glm::vec3(2.0f, -1.0f, 0.0f),
                glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));

            if (result != true) return 8;
        }

        // 09: 2 rays colliding
        {
            bool result = Check3D_Ray_Ray_Overlap(
                glm::vec3(2.0f, 2.0f, 0.0f),
                glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f)),
                glm::vec3(2.0f, -1.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));

            if (result != true) return 9;
        }

        // 10: 2 rays parallel and colliding
        {
            bool result = Check3D_Ray_Ray_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.5f, 0.0f, 0.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f));

            if (result != true) return 10;
        }

        // 11: 2 rays parallel and not colliding
        {
            bool result = Check3D_Ray_Ray_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::vec3(0.5f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f));

            if (result != false) return 11;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Ray_LineSegment_Overlap()
    {
        // 01: ray and lineSegment not on the same plane
        {
            bool result = Check3D_Ray_LineSegment_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(1.0f, 0.0f, 1.0f));

            if (result != false) return 1;
        }

        // 02: ray and lineSegment parallel
        {
            bool result = Check3D_Ray_LineSegment_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f));

            if (result != false) return 2;
        }

        // 03: 
        {
            bool result = Check3D_Ray_LineSegment_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)),
                glm::vec3(-4.0f, 2.0f, 0.0f),
                glm::vec3(-2.0f, 0.0f, 0.0f));

            if (result != false) return 3;
        }

        // 04: 
        {
            bool result = Check3D_Ray_LineSegment_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)),
                glm::vec3(-3.0f, 0.0f, 0.0f),
                glm::vec3(-3.0f, -2.0f, 0.0f));

            if (result != false) return 4;
        }

        // 05: 
        {
            bool result = Check3D_Ray_LineSegment_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)),
                glm::vec3(-1.0f, 4.0f, 0.0f),
                glm::vec3(-1.0f, 1.0f, 0.0f));

            if (result != false) return 5;
        }

        // 06: 
        {
            bool result = Check3D_Ray_LineSegment_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(-2.0f, -2.0f, 0.0f));

            if (result != false) return 6;
        }

        // 07: 
        {
            bool result = Check3D_Ray_LineSegment_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)),
                glm::vec3(1.0f, 3.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 0.0f));

            if (result != true) return 7;
        }

        // 08: 
        {
            bool result = Check3D_Ray_LineSegment_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)),
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::vec3(2.0f, 1.0f, 0.0f));

            if (result != true) return 8;
        }

        // 09: 
        {
            bool result = Check3D_Ray_LineSegment_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)),
                glm::vec3(3.0f, 6.0f, 0.0f),
                glm::vec3(2.0f, 2.0f, 0.0f));

            if (result != false) return 9;
        }

        // 10: 
        {
            bool result = Check3D_Ray_LineSegment_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)),
                glm::vec3(3.0f, 3.0f, 0.0f),
                glm::vec3(3.0f, 1.0f, 0.0f));

            if (result != true) return 10;
        }

        // 11: 
        {
            bool result = Check3D_Ray_LineSegment_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)),
                glm::vec3(5.0f, 3.0f, 0.0f),
                glm::vec3(5.0f, 1.0f, 0.0f));

            if (result != true) return 11;
        }

        // 12: Parallel and on top of each other
        {
            bool result = Check3D_Ray_LineSegment_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 0.0f, 0.0f));

            if (result != true) return 12;
        }

        // 13: Parallel and not on top of each other
        {
            bool result = Check3D_Ray_LineSegment_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 0.0f, 0.0f));

            if (result != false) return 13;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Ray_OBB_Overlap()
    {
        // 01: ray passing through a point OBB at the origin
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_Ray_OBB_Overlap(
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                rotation);

            if (result != true) return 1;
        }

        // 02: ray not passing through a point OBB at the origin
        // (opposite direction)
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_Ray_OBB_Overlap(
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                rotation);

            if (result != false) return 2;
        }

        // 03: line passing through a point OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(1.0f, 0.0f, 0.0f));
            bool result = Check3D_Ray_OBB_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(10.0f, 0.0f, 0.0f),
                rotation);

            if (result != true) return 3;
        }

        // 04: line not passing through a point OBB (opposite direction)
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(1.0f, 0.0f, 0.0f));
            bool result = Check3D_Ray_OBB_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(10.0f, 0.0f, 0.0f),
                rotation);

            if (result != false) return 4;
        }

        // 05: ray passing NOT through a point OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(1.0f, 0.0f, 0.0f));
            bool result = Check3D_Ray_OBB_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(10.0f, 0.001f, 0.0f),
                rotation);

            if (result != false) return 5;
        }

        // 06: ray passing through a OBB (inside OBB)
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_Ray_OBB_Overlap(
                glm::vec3(0.2f, 0.2f, 0.2f),
                glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f)),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != true) return 6;
        }

        // 07: ray passing through a OBB (inside OBB)
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_Ray_OBB_Overlap(
                glm::vec3(0.2f, 0.2f, 0.2f),
                glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f)),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != true) return 7;
        }

        // 08: ray passing through a OBB
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_Ray_OBB_Overlap(
                glm::vec3(0.0f, 0.4f, 0.0f),
                glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f)),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != true) return 8;
        }

        // 09: ray not passing through a OBB (opposite direction)
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_Ray_OBB_Overlap(
                glm::vec3(0.0f, 0.4f, 0.0f),
                glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f)),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != false) return 9;
        }

        // 10: ray on OBB corner
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_Ray_OBB_Overlap(
                glm::vec3(0.562132001f, 0.412132025f, -0.0621320233f),
                glm::normalize(glm::vec3(-1.0f, 1.0f, 1.0f)),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != true) return 10;
        }

        // 11: ray going through OBB corner
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_Ray_OBB_Overlap(
                glm::vec3(0.0f, 0.412132025f, -0.0621320233f),
                glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != true) return 11;
        }

        // 12: ray not going through OBB corner (opposite direction)
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_Ray_OBB_Overlap(
                glm::vec3(0.0f, 0.412132025f, -0.0621320233f),
                glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f)),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != false) return 12;
        }

        // 13: ray going through OBB edge
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_Ray_OBB_Overlap(
                glm::vec3(0.0f, 0.412132025f, 0.1f),
                glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != true) return 13;
        }

        // 14: ray not going through OBB edge (opposite direction)
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_Ray_OBB_Overlap(
                glm::vec3(0.0f, 0.412132025f, 0.1f),
                glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f)),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != false) return 14;
        }

        // 15: ray going along OBB edge
        {
            glm::vec3 position00(0.2f, 0.2f, 0.3f);
            glm::vec3 direction00(-1.0f, 0.0f, 0.0f);

            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));

            position00 = rotation2 * rotation1 * position00;
            direction00 = rotation2 * rotation1 * direction00;

            position00 += glm::vec3(0.3f, 0.2f, 0.1f);

            bool result = Check3D_Ray_OBB_Overlap(
                position00,
                direction00,
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != true) return 15;
        }

        // 16: ray going near OBB edge
        {
            glm::vec3 position00(0.2f, 0.201f, 0.3f);
            glm::vec3 direction00(-1.0f, 0.0f, 0.0f);

            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));

            position00 = rotation2 * rotation1 * position00;
            direction00 = rotation2 * rotation1 * direction00;

            position00 += glm::vec3(0.3f, 0.2f, 0.1f);

            bool result = Check3D_Ray_OBB_Overlap(
                position00,
                direction00,
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != false) return 16;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Ray_Plane_Overlap()
    {
        // 01: ray is parallel to the plane
        {
            bool result = Check3D_Ray_Plane_Overlap(
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                0.0f);

            if (result != false) return 1;
        }

        // 02: ray is parallel to the plane and on the plane
        {
            bool result = Check3D_Ray_Plane_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                0.0f);

            if (result != true) return 2;
        }

        // 03: ray and plane overlap
        {
            bool result = Check3D_Ray_Plane_Overlap(
                glm::vec3(0.0f, -1.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(0.0f, 1.0f, 0.0f),
                0.0f);

            if (result != true) return 3;
        }

        // 04: ray is parallel to the plane v2
        {
            bool result = Check3D_Ray_Plane_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                0.0f);

            if (result != false) return 4;
        }

        // 05: ray and plane don't overlap
        {
            bool result = Check3D_Ray_Plane_Overlap(
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(0.0f, 1.0f, 0.0f),
                0.0f);

            if (result != false) return 5;
        }

        // 06: ray is parallel to the plane and on it
        {
            bool result = Check3D_Ray_Plane_Overlap(
                glm::vec3(-5.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                0.0f);

            if (result != true) return 6;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Ray_Triangle_Overlap()
    {
        // 01: ray is parallel to the triangle
        {
            bool result = Check3D_Ray_Triangle_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f)),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f));

            if (result != false) return 1;
        }

        // 02: ray misses to the triangle
        {
            bool result = Check3D_Ray_Triangle_Overlap(
                glm::vec3(1.1f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f));

            if (result != false) return 2;
        }

        // 03: ray overlaps triangle corner
        {
            bool result = Check3D_Ray_Triangle_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f));

            if (result != true) return 3;
        }

        // 04: ray overlaps triangle center
        {
            bool result = Check3D_Ray_Triangle_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f));

            if (result != true) return 4;
        }

        // 05: ray doesn't overlap triangle center
        {
            bool result = Check3D_Ray_Triangle_Overlap(
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f));

            if (result != false) return 5;
        }

        // 06: ray starts at triangle corner
        {
            bool result = Check3D_Ray_Triangle_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f));

            if (result != true) return 6;
        }

        // 07: ray is parallel to the triangle
        {
            bool result = Check3D_Ray_Triangle_Overlap(
                glm::vec3(-1.0f, 0.0f, 2.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f)),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f));

            if (result != true) return 7;
        }

        // 08: ray is parallel to the triangle, but no collision
        {
            bool result = Check3D_Ray_Triangle_Overlap(
                glm::vec3(-1.0f, 0.0f, 2.0f),
                glm::normalize(glm::vec3(-1.0f, 0.0f, 1.0f)),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f));

            if (result != false) return 8;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Ray_AABB_Overlap()
    {
        // 01: ray passing through a point AABB at origin
        {
            bool result = Check3D_Ray_AABB_Overlap(
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f));

            if (result != true) return 1;
        }

        // 02: ray passing through a point AABB
        {
            bool result = Check3D_Ray_AABB_Overlap(
                glm::vec3(-1.0f, 0.0f, 1.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(0.0f, 1.0f, 2.0f),
                glm::vec3(0.0f, 1.0f, 2.0f));

            if (result != true) return 2;
        }

        // 03: ray going in opposite direction to point AABB
        {
            bool result = Check3D_Ray_AABB_Overlap(
                glm::vec3(-1.0f, 0.0f, 1.0f),
                glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)),
                glm::vec3(0.0f, 1.0f, 2.0f),
                glm::vec3(0.0f, 1.0f, 2.0f));

            if (result != false) return 3;
        }

        // 04: ray NOT passing through a AABB (opposite direction)
        {
            bool result = Check3D_Ray_AABB_Overlap(
                glm::vec3(0.2f, 0.2f, 0.2f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != false) return 4;
        }

        // 05: ray passing through a AABB
        {
            bool result = Check3D_Ray_AABB_Overlap(
                glm::vec3(0.2f, 0.2f, 0.2f),
                glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != true) return 5;
        }

        // 06: ray NOT passing through a AABB
        {
            bool result = Check3D_Ray_AABB_Overlap(
                glm::vec3(0.2f, 0.2f, 0.2f),
                glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != false) return 6;
        }

        // 07: ray NOT passing through a AABB
        {
            bool result = Check3D_Ray_AABB_Overlap(
                glm::vec3(0.2f, 0.2f, 0.2f),
                glm::normalize(glm::vec3(1.0f, -1.0f, -1.0f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != false) return 7;
        }

        // 08: ray passing through AABB corner, starting at corner
        {
            bool result = Check3D_Ray_AABB_Overlap(
                glm::vec3(-0.1f, 0.2f, 0.3f),
                glm::normalize(glm::vec3(0.1f, 0.1f, -0.1f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != true) return 8;
        }

        // 09: ray NOT passing through AABB corner (going in opposite direction)
        {
            bool result = Check3D_Ray_AABB_Overlap(
                glm::vec3(0.0f, 0.3f, 0.2f),
                glm::normalize(glm::vec3(0.1f, 0.1f, -0.1f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != false) return 9;
        }

        // 10: ray passing through AABB corner
        {
            bool result = Check3D_Ray_AABB_Overlap(
                glm::vec3(0.0f, 0.3f, 0.2f),
                glm::normalize(glm::vec3(-0.1f, -0.1f, 0.1f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != true) return 10;
        }

        // 11: ray NOT passing through AABB
        {
            bool result = Check3D_Ray_AABB_Overlap(
                glm::vec3(0.0f, 0.31f, 0.2f),
                glm::normalize(glm::vec3(0.1f, 0.1f, -0.1f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != false) return 11;
        }

        // 12: ray passing through AABB edge
        {
            bool result = Check3D_Ray_AABB_Overlap(
                glm::vec3(0.3f, 0.2f, 0.3f),
                glm::normalize(glm::vec3(-0.1f, 0.0f, 0.0f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != true) return 12;
        }

        // 13: ray NOT passing through AABB edge (opposite direction)
        {
            bool result = Check3D_Ray_AABB_Overlap(
                glm::vec3(0.2f, 0.2f, 0.3f),
                glm::normalize(glm::vec3(0.1f, 0.0f, 0.0f)),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != false) return 13;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Ray_Sphere_Overlap()
    {
        // 01: ray passing through sphere at origin
        {
            bool result = Check3D_Ray_Sphere_Overlap(
                glm::vec3(-3.0f, -3.0f, -3.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != true) return 1;
        }

        // 02: ray not passing through sphere at origin
        {
            bool result = Check3D_Ray_Sphere_Overlap(
                glm::vec3(-3.0f, -3.0f, -3.0f),
                glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != false) return 2;
        }

        // 03: ray passing through sphere edge at origin
        {
            bool result = Check3D_Ray_Sphere_Overlap(
                glm::vec3(-3.0f, -2.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != true) return 3;
        }

        // 04: ray not passing through sphere edge at origin
        {
            bool result = Check3D_Ray_Sphere_Overlap(
                glm::vec3(-3.0f, -2.0f, 0.0f),
                glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != false) return 4;
        }

        // 05: ray not passing through sphere at origin
        {
            bool result = Check3D_Ray_Sphere_Overlap(
                glm::vec3(-2.0f, -3.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != false) return 5;
        }

        // 06: ray not passing through sphere at origin
        {
            bool result = Check3D_Ray_Sphere_Overlap(
                glm::vec3(-2.0f, -3.0f, 0.0f),
                glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != false) return 6;
        }

        return 0;
    }

    // -------------------------------------------------------------------------

    inline int UnitTest_Check3D_LineSegment_LineSegment_Overlap()
    {
        // 01: 2 lineSegments not on the same plane
        {
            bool result = Check3D_LineSegment_LineSegment_Overlap(
                glm::vec3(-3.0f, 0.0f, -1.0f),
                glm::vec3(3.0f, 0.0f, 2.0f),
                glm::vec3(0.0f, -1.0f, 0.0f),
                glm::vec3(0.0f, 3.0f, 0.0f));

            if (result != false) return 1;
        }

        // 02: 2 lineSegments parallel
        {
            bool result = Check3D_LineSegment_LineSegment_Overlap(
                glm::vec3(-3.0f, 0.0f, -1.0f),
                glm::vec3(3.0f, 0.0f, 2.0f),
                glm::vec3(2.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, -1.0f));

            if (result != false) return 2;
        }

        // 03: 
        {
            bool result = Check3D_LineSegment_LineSegment_Overlap(
                glm::vec3(-3.0f, 0.0f, -1.0f),
                glm::vec3(3.0f, 0.0f, 2.0f),
                glm::vec3(-3.0f, 0.0f, 1.0f),
                glm::vec3(-2.0f, 0.0f, -1.0f));

            if (result != true) return 3;
        }

        // 04: 
        {
            bool result = Check3D_LineSegment_LineSegment_Overlap(
                glm::vec3(-3.0f, 0.0f, -1.0f),
                glm::vec3(3.0f, 0.0f, 2.0f),
                glm::vec3(1.0f, 0.0f, 3.0f),
                glm::vec3(1.0f, 0.0f, 1.0f));

            if (result != true) return 4;
        }

        // 05: 
        {
            bool result = Check3D_LineSegment_LineSegment_Overlap(
                glm::vec3(-3.0f, 0.0f, -1.0f),
                glm::vec3(3.0f, 0.0f, 2.0f),
                glm::vec3(1.0f, 0.0f, 1.0f),
                glm::vec3(3.0f, 0.0f, 1.0f));

            if (result != true) return 5;
        }

        // 06: 
        {
            bool result = Check3D_LineSegment_LineSegment_Overlap(
                glm::vec3(-3.0f, 0.0f, -1.0f),
                glm::vec3(3.0f, 0.0f, 2.0f),
                glm::vec3(-3.0f, 0.0f, -1.0f),
                glm::vec3(-3.0f, 0.0f, -3.0f));

            if (result != true) return 6;
        }

        // 07: 
        {
            bool result = Check3D_LineSegment_LineSegment_Overlap(
                glm::vec3(-3.0f, 0.0f, -1.0f),
                glm::vec3(3.0f, 0.0f, 2.0f),
                glm::vec3(-2.0f, 0.0f, -2.0f),
                glm::vec3(-3.0f, 0.0f, -1.0f));

            if (result != true) return 7;
        }

        // 08: 
        {
            bool result = Check3D_LineSegment_LineSegment_Overlap(
                glm::vec3(-3.0f, 0.0f, -1.0f),
                glm::vec3(3.0f, 0.0f, 2.0f),
                glm::vec3(3.0f, 0.0f, 4.0f),
                glm::vec3(3.0f, 0.0f, 2.0f));

            if (result != true) return 8;
        }

        // 09: 
        {
            bool result = Check3D_LineSegment_LineSegment_Overlap(
                glm::vec3(-3.0f, 0.0f, -1.0f),
                glm::vec3(3.0f, 0.0f, 2.0f),
                glm::vec3(3.0f, 0.0f, 2.0f),
                glm::vec3(4.0f, 0.0f, 2.0f));

            if (result != true) return 9;
        }

        // 10: 
        {
            bool result = Check3D_LineSegment_LineSegment_Overlap(
                glm::vec3(-3.0f, 0.0f, -1.0f),
                glm::vec3(3.0f, 0.0f, 2.0f),
                glm::vec3(-4.0f, 0.0f, 1.0f),
                glm::vec3(-4.0f, 0.0f, -2.0f));

            if (result != false) return 10;
        }

        // 11: 
        {
            bool result = Check3D_LineSegment_LineSegment_Overlap(
                glm::vec3(-3.0f, 0.0f, -1.0f),
                glm::vec3(3.0f, 0.0f, 2.0f),
                glm::vec3(4.0f, 0.0f, 3.0f),
                glm::vec3(6.0f, 0.0f, 3.0f));

            if (result != false) return 11;
        }

        // 12: lineSegments parallel and overlapping
        {
            bool result = Check3D_LineSegment_LineSegment_Overlap(
                glm::vec3(-2.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f));

            if (result != true) return 12;
        }

        // 13: lineSegments parallel and not overlapping
        {
            bool result = Check3D_LineSegment_LineSegment_Overlap(
                glm::vec3(-2.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 0.0f, 0.0f));

            if (result != false) return 13;
        }

        return 0;
    }

    inline int UnitTest_Check3D_LineSegment_Plane_Overlap()
    {
        // 01: lineSegment parallel to plane
        {
            bool result = Check3D_LineSegment_Plane_Overlap(
                glm::vec3(0.0f, 0.0f, -2.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                1.0f / glm::sqrt(3.0f));

            if (result != false) return 1;
        }

        // 02: lineSegment parallel to plane and on it
        {
            bool result = Check3D_LineSegment_Plane_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                1.0f / glm::sqrt(3.0f));

            if (result != true) return 2;
        }

        // 03: lineSegment start on plane
        {
            bool result = Check3D_LineSegment_Plane_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.5f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                1.0f / glm::sqrt(3.0f));

            if (result != true) return 3;
        }

        // 04: lineSegment end on plane
        {
            bool result = Check3D_LineSegment_Plane_Overlap(
                glm::vec3(1.5f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                1.0f / glm::sqrt(3.0f));

            if (result != true) return 4;
        }

        // 05: lineSegment through plane
        {
            bool result = Check3D_LineSegment_Plane_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                1.0f / glm::sqrt(3.0f));

            if (result != true) return 5;
        }

        return 0;
    }

    inline int UnitTest_Check3D_LineSegment_Triangle_Overlap()
    {
        // 01: lineSegment parallel to tringle
        {
            bool result = Check3D_LineSegment_Triangle_Overlap(
                glm::vec3(0.0f, 0.0f, -2.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f));

            if (result != false) return 1;
        }

        // 02: lineSegment parallel to tringle and on it
        {
            bool result = Check3D_LineSegment_Triangle_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f));

            if (result != true) return 2;
        }

        // 03: lineSegment parallel to tringle and through it
        {
            bool result = Check3D_LineSegment_Triangle_Overlap(
                glm::vec3(2.0f, 0.0f, -1.0f),
                glm::vec3(-1.0f, 0.0f, 2.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f));

            if (result != true) return 3;
        }

        return 0;
    }

    inline int UnitTest_Check3D_LineSegment_AABB_Overlap()
    {
        // 01: point lineSegment on point AABB at origin
        {
            bool result = Check3D_LineSegment_AABB_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f));
            if (result != true) return 1;
        }

        // 02: point lineSegment on Point AABB
        {
            bool result = Check3D_LineSegment_AABB_Overlap(
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f));
            if (result != true) return 2;
        }

        // 03: point lineSegment in AABB
        {
            bool result = Check3D_LineSegment_AABB_Overlap(
                glm::vec3(2.0f, 3.0f, 4.0f),
                glm::vec3(2.0f, 3.0f, 4.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(3.0f, 4.0f, 5.0f));
            if (result != true) return 3;
        }

        // 04: point lineSegment on AABB corner
        {
            bool result = Check3D_LineSegment_AABB_Overlap(
                glm::vec3(3.0f, 4.0f, 5.0f),
                glm::vec3(3.0f, 4.0f, 5.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(3.0f, 4.0f, 5.0f));
            if (result != true) return 4;
        }

        // 05: point lineSegment outside AABB
        {
            bool result = Check3D_LineSegment_AABB_Overlap(
                glm::vec3(3.0f, 4.0f, 5.0f),
                glm::vec3(3.0f, 4.0f, 5.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(2.0f, 3.0f, 4.0f));
            if (result != false) return 5;
        }

        // 06: lineSegment through point AABB
        {
            bool result = Check3D_LineSegment_AABB_Overlap(
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f));
            if (result != true) return 6;
        }

        // 07: lineSegment not through point AABB
        {
            bool result = Check3D_LineSegment_AABB_Overlap(
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(1.1f, 1.0f, 1.0f),
                glm::vec3(1.1f, 1.0f, 1.0f));
            if (result != false) return 7;
        }

        // 08: lineSegment inside AABB
        {
            bool result = Check3D_LineSegment_AABB_Overlap(
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(-2.0f, -3.0f, -4.0f),
                glm::vec3(2.0f, 3.0f, 4.0f));
            if (result != true) return 8;
        }

        // 09: LineSegment start in AABB
        {
            bool result = Check3D_LineSegment_AABB_Overlap(
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(-2.0f, -3.0f, -4.0f),
                glm::vec3(2.0f, 3.0f, 4.0f));
            if (result != true) return 9;
        }

        // 09: LineSegment end in AABB
        {
            bool result = Check3D_LineSegment_AABB_Overlap(
                glm::vec3(0.3f, 0.2f, 0.7f),
                glm::vec3(0.1f, 0.3f, 0.5f),
                glm::vec3(-0.2f, -0.4f, -0.6f),
                glm::vec3(0.2f, 0.4f, 0.6f));
            if (result != true) return 9;
        }

        // 10: LineSegment not overlapping AABB
        {
            bool result = Check3D_LineSegment_AABB_Overlap(
                glm::vec3(-0.1f, 0.5f, 0.3f),
                glm::vec3(-0.4f, 0.3f, 0.3f),
                glm::vec3(-0.2f, -0.4f, -0.6f),
                glm::vec3(0.2f, 0.4f, 0.6f));
            if (result != false) return 10;
        }

        // 11: LineSegment touching corner of AABB
        {
            bool result = Check3D_LineSegment_AABB_Overlap(
                glm::vec3(0.2f, -0.4f, -0.6f),
                glm::vec3(0.0f, -0.2f, -0.7f),
                glm::vec3(-0.2f, -0.4f, -0.6f),
                glm::vec3(0.2f, 0.4f, 0.6f));
            if (result != true) return 11;
        }

        // 12: LineSegment not overlapping AABB
        {
            bool result = Check3D_LineSegment_AABB_Overlap(
                glm::vec3(0.3f, 0.2f, 0.0f),
                glm::vec3(-0.3f, 0.2f, 0.0f),
                glm::vec3(-0.2f, -0.1f, -0.3f),
                glm::vec3(0.2f, 0.1f, 0.3f));
            if (result != false) return 12;
        }

        // 13: LineSegment on AABB edge
        {
            bool result = Check3D_LineSegment_AABB_Overlap(
                glm::vec3(-0.3f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.3f),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));
            if (result != true) return 13;
        }

        // 14: LineSegment parallel to AABB edge, but no overlap
        {
            bool result = Check3D_LineSegment_AABB_Overlap(
                glm::vec3(-0.3f, 0.2f, 0.3f),
                glm::vec3(-0.2f, 0.2f, 0.3f),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));
            if (result != false) return 14;
        }

        // 15: LineSegment near AABB edge
        {
            bool result = Check3D_LineSegment_AABB_Overlap(
                glm::vec3(-0.3f, 0.201f, 0.3f),
                glm::vec3(0.3f, 0.201f, 0.3f),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));
            if (result != false) return 15;
        }

        return 0;
    }

    inline int UnitTest_Check3D_LineSegment_OBB_Overlap()
    {
        // 01: LineSegment passing through a point OBB at the origin
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_LineSegment_OBB_Overlap(
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                rotation);

            if (result != true) return 1;
        }

        // 02: LineSegment not passing through a point OBB at the origin
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_LineSegment_OBB_Overlap(
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(0.001f, 0.001f, 0.001f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                rotation);

            if (result != false) return 2;
        }

        // 03: LineSegment passing through a point OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(1.0f, 0.0f, 0.0f));
            bool result = Check3D_LineSegment_OBB_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(11.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(10.0f, 0.0f, 0.0f),
                rotation);

            if (result != true) return 3;
        }

        // 04: LineSegment not passing through a point OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(1.0f, 0.0f, 0.0f));
            bool result = Check3D_LineSegment_OBB_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(9.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(10.0f, 0.0f, 0.0f),
                rotation);

            if (result != false) return 4;
        }

        // 05: line passing through a OBB
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_LineSegment_OBB_Overlap(
                glm::vec3(-1.2f, -1.0f, -1.0f),
                glm::vec3(0.8f, 1.0f, 1.0f),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != true) return 5;
        }

        // 06: line NOT passing through a OBB
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_LineSegment_OBB_Overlap(
                glm::vec3(-1.2f, -1.0f, -1.2f),
                glm::vec3(0.8f, 1.0f, 0.8f),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != false) return 6;
        }

        // 07: LineSegment start at OBB corner
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_LineSegment_OBB_Overlap(
                glm::vec3(0.562132001f, 0.412132025f, -0.0621320233f),
                glm::vec3(1.0f, 1.0f, -1.0f),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != true) return 7;
        }

        // 08: LineSegment end at OBB corner
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_LineSegment_OBB_Overlap(
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(0.562132001f, 0.412132025f, -0.0621320233f),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != true) return 8;
        }

        // 09: LineSegment going through OBB corner
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_LineSegment_OBB_Overlap(
                glm::vec3(0.0f, 0.412132025f, -0.0621320233f),
                glm::vec3(1.0f, 0.412132025f, -0.0621320233f),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != true) return 9;
        }

        // 10: LineSegment going through OBB edge
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_LineSegment_OBB_Overlap(
                glm::vec3(0.0f, 0.412132025f, 0.1f),
                glm::vec3(1.0f, 0.412132025f, 0.1f),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != true) return 10;
        }

        // 11: line going along OBB edge
        {
            glm::vec3 position00(-0.8f, 0.2f, 0.3f);
            glm::vec3 position01(0.8f, 0.2f, 0.3f);
        
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
        
            position00 = rotation1 * position00;
            position01 = rotation1 * position01;
        
            position00 += glm::vec3(0.3f, 0.2f, 0.1f);
            position01 += glm::vec3(0.3f, 0.2f, 0.1f);
        
            bool result = Check3D_LineSegment_OBB_Overlap(
                position00,
                position01,
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation1);
        
            if (result != true) return 11;
        }

        // 12: line going along OBB edge
        {
            glm::vec3 position00(-0.8f, 0.2f, 0.3f);
            glm::vec3 position01(0.8f, 0.2f, 0.3f);

            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));

            position00 = rotation2 * rotation1 * position00;
            position01 = rotation2 * rotation1 * position01;

            position00 += glm::vec3(0.3f, 0.2f, 0.1f);
            position01 += glm::vec3(0.3f, 0.2f, 0.1f);

            bool result = Check3D_LineSegment_OBB_Overlap(
                position00,
                position01,
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.3f, 0.2f, 0.1f),
                rotation2 * rotation1);

            if (result != true) return 12;
        }

        return 0;
    }

    inline int UnitTest_Check3D_LineSegment_Sphere_Overlap()
    {
        // 01: lineSegment inside sphere at origin
        {
            bool result = Check3D_LineSegment_Sphere_Overlap(
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != true) return 1;
        }

        // 02: lineSegment crossing sphere at origin
        {
            bool result = Check3D_LineSegment_Sphere_Overlap(
                glm::vec3(-2.0f, -2.0f, -2.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != true) return 2;
        }

        // 03: lineSegment crossing sphere at origin
        {
            bool result = Check3D_LineSegment_Sphere_Overlap(
                glm::vec3(-2.0f, -2.0f, -2.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != true) return 3;
        }

        // 04: lineSegment end touching sphere edge at origin
        {
            bool result = Check3D_LineSegment_Sphere_Overlap(
                glm::vec3(-2.0f, 2.0f, 0.0f),
                glm::vec3(0.0f, 2.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != true) return 4;
        }

        // 05: lineSegment start touching sphere edge at origin
        {
            bool result = Check3D_LineSegment_Sphere_Overlap(
                glm::vec3(2.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 2.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != true) return 5;
        }

        // 06: lineSegment touching sphere edge at origin
        {
            bool result = Check3D_LineSegment_Sphere_Overlap(
                glm::vec3(2.0f, -2.0f, 0.0f),
                glm::vec3(2.0f, 2.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != true) return 6;
        }

        // 07: lineSegment not touching sphere at origin
        {
            bool result = Check3D_LineSegment_Sphere_Overlap(
                glm::vec3(-3.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 3.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != false) return 7;
        }

        // 08: lineSegment overlapping infinitly small circle
        {
            bool result = Check3D_LineSegment_Sphere_Overlap(
                glm::vec3(-3.0f, -3.0f, -3.0f),
                glm::vec3(3.0f, 3.0f, 3.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                0.0f);

            if (result != true) return 8;
        }

        // 09: infinitly shortlineSegment overlapping infinitly small circle
        {
            bool result = Check3D_LineSegment_Sphere_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                0.0f);

            if (result != true) return 9;
        }

        return 0;
    }

    // -------------------------------------------------------------------------

    inline int UnitTest_Check3D_Plane_Plane_Overlap()
    {
        // 01: 2 planes parallel
        {
            bool result = Check3D_Plane_Plane_Overlap(
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                0.0f,
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                1.0f / glm::sqrt(3.0f));

            if (result != false) return 1;
        }

        // 02: 2 planes that are the same
        {
            bool result = Check3D_Plane_Plane_Overlap(
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                1.0f / glm::sqrt(3.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                1.0f / glm::sqrt(3.0f));

            if (result != true) return 2;
        }

        // 03: 2 planes that are different
        {
            bool result = Check3D_Plane_Plane_Overlap(
                glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)),
                -1.0f,
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                1.0f / glm::sqrt(3.0f));

            if (result != true) return 3;
        }

        // 04: 2 planes that are different
        {
            bool result = Check3D_Plane_Plane_Overlap(
                glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)),
                -1.0f,
                glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)),
                -1.0f);

            if (result != true) return 4;
        }

        // 05: 2 planes that are different
        {
            bool result = Check3D_Plane_Plane_Overlap(
                glm::normalize(glm::vec3(1.0f, 2.0f, 0.0f)),
                -1.0f,
                glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)),
                -1.0f);

            if (result != true) return 5;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Plane_Triangle_Overlap()
    {
        // 01: plane at origin and point triangle at origin
        {
            bool result = Check3D_Plane_Triangle_Overlap(
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                0.0f,
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f));

            if (result != true) return 1;
        }

        // 02: plane at origin and triangle at origin
        {
            bool result = Check3D_Plane_Triangle_Overlap(
                glm::vec3(0.0f, 0.0f, 1.0f),
                0.0f,
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));

            if (result != true) return 2;
        }

        // 03: plane above origin and triangle at origin
        {
            bool result = Check3D_Plane_Triangle_Overlap(
                glm::vec3(0.0f, 0.0f, 1.0f),
                1.0f,
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));

            if (result != false) return 3;
        }

        // 04: plane at origin and triangle with corner on origin
        {
            bool result = Check3D_Plane_Triangle_Overlap(
                glm::vec3(0.0f, 0.0f, 1.0f),
                1.0f,
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f));

            if (result != true) return 4;
        }

        // 05: plane and triangle overlapping
        {
            bool result = Check3D_Plane_Triangle_Overlap(
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                1.0f,
                glm::vec3(0.0f, 0.0f, 3.0f),
                glm::vec3(0.0f, 3.0f, 0.0f),
                glm::vec3(0.0f, -3.0f, 0.0f));

            if (result != true) return 5;
        }

        // 06: plane and triangle not overlapping
        {
            bool result = Check3D_Plane_Triangle_Overlap(
                glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)),
                1.0f,
                glm::vec3(0.0f, 0.0f, 3.0f),
                glm::vec3(0.0f, 3.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f));

            if (result != false) return 6;
        }

        // 07: plane and triangle overlapping
        {
            bool result = Check3D_Plane_Triangle_Overlap(
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                1.0f,
                glm::vec3(0.0f, 0.0f, 3.0f),
                glm::vec3(0.0f, 3.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f));

            if (result != true) return 7;
        }

        // 08: plane and triangle not overlapping
        {
            bool result = Check3D_Plane_Triangle_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                1.0f,
                glm::vec3(0.0f, 2.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 2.0f),
                glm::vec3(0.0f, 0.0f, -2.0f));

            if (result != false) return 8;
        }

        // 09: plane and triangle overlapping
        {
            bool result = Check3D_Plane_Triangle_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                1.0f,
                glm::vec3(1.0f, 2.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 2.0f),
                glm::vec3(0.0f, 0.0f, -2.0f));

            if (result != true) return 9;
        }

        // 10: plane and triangle overlapping
        {
            bool result = Check3D_Plane_Triangle_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                1.0f,
                glm::vec3(2.0f, 2.0f, 0.0f),
                glm::vec3(2.0f, 0.0f, 2.0f),
                glm::vec3(0.0f, 0.0f, -2.0f));

            if (result != true) return 10;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Plane_AABB_Overlap()
    {
        // 01: plane at origin overlaps tiny AABB at origin
        {
            bool result = Check3D_Plane_AABB_Overlap(
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                0.0f,
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f));

            if (result != true) return 1;
        }

        // 02: plane at origin not overlapping tiny AABB
        {
            bool result = Check3D_Plane_AABB_Overlap(
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                0.0f,
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f));

            if (result != false) return 2;
        }

        // 03: plane not overlapping AABB
        {
            bool result = Check3D_Plane_AABB_Overlap(
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                -1.0f,
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(9.0f, 9.0f, 9.0f));

            if (result != false) return 3;
        }

        // 04: plane not overlapping AABB
        {
            bool result = Check3D_Plane_AABB_Overlap(
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                1.0f,
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f));

            if (result != false) return 4;
        }

        // 05: plane overlapping AABB
        {
            bool result = Check3D_Plane_AABB_Overlap(
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                0.5f,
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3( 1.0f, 1.0f, 1.0f));

            if (result != true) return 5;
        }

        // 06: plane touching AABB corner
        {
            bool result = Check3D_Plane_AABB_Overlap(
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::sqrt(3.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f));

            if (result != true) return 6;
        }

        // 07: plane touching AABB face
        {
            bool result = Check3D_Plane_AABB_Overlap(
                glm::vec3(0.0f, 0.0f, 1.0f),
                -1.0f,
                glm::vec3(-2.0f, -2.0f, -1.0f),
                glm::vec3(2.0f, 2.0f, 1.0f));

            if (result != true) return 7;
        }

        // 08: plane not touching AABB
        {
            bool result = Check3D_Plane_AABB_Overlap(
                glm::vec3(0.0f, 0.0f, 1.0f),
                -2.0f,
                glm::vec3(-2.0f, -2.0f, -1.0f),
                glm::vec3(2.0f, 2.0f, 1.0f));

            if (result != false) return 8;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Plane_OBB_Overlap()
    {
        // 01: plane at origin overlaps tiny OBB at origin
        {
            glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            bool result = Check3D_Plane_OBB_Overlap(
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                0.0f,
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                rotation);

            if (result != true) return 1;
        }

        // 02: plane overlaps tiny OBB
        {
            glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            bool result = Check3D_Plane_OBB_Overlap(
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::sqrt(3.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                rotation);

            if (result != true) return 2;
        }

        // 03: plane not overlapping tiny OBB
        {
            glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            bool result = Check3D_Plane_OBB_Overlap(
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                -glm::sqrt(3.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                rotation);

            if (result != false) return 3;
        }

        // 04: plane overlapping OBB
        {
            glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            bool result = Check3D_Plane_OBB_Overlap(
                glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)),
                0.0f,
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                rotation);

            if (result != true) return 4;
        }

        // 05: plane touching OBB (OBB on top)
        {
            glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            bool result = Check3D_Plane_OBB_Overlap(
                glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)),
                -0.5f,
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                rotation);

            if (result != true) return 5;
        }

        // 06: plane not overlapping OBB
        {
            glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            bool result = Check3D_Plane_OBB_Overlap(
                glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)),
                0.0f,
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                rotation);

            if (result != false) return 6;
        }

        // 07: plane overlapping OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(1.0f, 0.0f, 0.0f));
            bool result = Check3D_Plane_OBB_Overlap(
                glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)),
                0.0f,
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                rotation);

            if (result != true) return 7;
        }

        // 08: plane not overlapping OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(1.0f, 0.0f, 0.0f));
            rotation = glm::rotate(
                rotation,
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            rotation = glm::rotate(
                rotation,
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_Plane_OBB_Overlap(
                glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)),
                0.0f,
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                rotation);

            if (result != false) return 8;
        }

        // 09: plane not overlapping OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(1.0f, 0.0f, 0.0f));
            rotation = glm::rotate(
                rotation,
                -glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            rotation = glm::rotate(
                rotation,
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_Plane_OBB_Overlap(
                glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)),
                0.0f,
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                rotation);

            if (result != false) return 9;
        }

        // 10: plane overlapping OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            rotation = glm::rotate(
                rotation,
                -glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            rotation = glm::rotate(
                rotation,
                glm::quarter_pi<float>(),
                glm::vec3(1.0f, 0.0f, 0.0f));
            bool result = Check3D_Plane_OBB_Overlap(
                glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)),
                0.0f,
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                rotation);

            if (result != true) return 10;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Plane_Sphere_Overlap()
    {
        // 01: plane at origin overlaps sphere at origin
        {
            bool result = Check3D_Plane_Sphere_Overlap(
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                0.0f,
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != true) return 1;
        }

        // 02: plane at origin and doesn't overlap sphere
        {
            bool result = Check3D_Plane_Sphere_Overlap(
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                0.0f,
                glm::vec3(2.0f, 2.0f, 2.0f),
                2.0f);

            if (result != false) return 2;
        }

        // 03: plane at origin touches the edge of the sphere
        {
            bool result = Check3D_Plane_Sphere_Overlap(
                glm::vec3(0.0f, 1.0f, 0.0f),
                0.0f,
                glm::vec3(0.0f, 2.0f, 0.0f),
                2.0f);

            if (result != true) return 3;
        }

        // 04: sphere below plane at origin
        {
            bool result = Check3D_Plane_Sphere_Overlap(
                glm::vec3(0.0f, 1.0f, 0.0f),
                0.0f,
                glm::vec3(0.0f, -3.0f, 0.0f),
                2.0f);

            if (result != false) return 4;
        }

        return 0;
    }

    // -------------------------------------------------------------------------

    inline int UnitTest_Check3D_Triangle_Triangle_Overlap()
    {
        // 01: point triangle and point triangle at origin
        {
            bool result = Check3D_Triangle_Triangle_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f));

            if (result != true) return 1;
        }

        // 02: point triangle and point triangle overlapping
        {
            bool result = Check3D_Triangle_Triangle_Overlap(
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f));

            if (result != true) return 2;
        }

        // 03: point triangle and point triangle NOT overlapping
        {
            bool result = Check3D_Triangle_Triangle_Overlap(
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 0.0f),
                glm::vec3(1.0f, 2.0f, 0.0f),
                glm::vec3(1.0f, 2.0f, 0.0f));

            if (result != false) return 3;
        }

        // 04: triangle's touching corners
        {
            bool result = Check3D_Triangle_Triangle_Overlap(
                glm::vec3(0.1f, 0.2f, 0.0f),
                glm::vec3(0.3f, 0.6f, 0.1f),
                glm::vec3(0.5f, 0.4f, 0.2f),
                glm::vec3(-0.3f, 0.4f, -0.2f),
                glm::vec3(-0.1f, 0.6f, -0.1f),
                glm::vec3(0.1f, 0.2f, 0.0f));

            if (result != true) return 4;
        }

        // 05: triangle's NOT touching corners
        {
            bool result = Check3D_Triangle_Triangle_Overlap(
                glm::vec3(0.0f, 0.2f, 0.0f),
                glm::vec3(0.3f, 0.6f, 0.1f),
                glm::vec3(0.5f, 0.4f, 0.2f),
                glm::vec3(-0.3f, 0.4f, -0.2f),
                glm::vec3(-0.1f, 0.6f, -0.1f),
                glm::vec3(0.2f, 0.2f, 0.0f));

            if (result != false) return 5;
        }

        // 06: triangle corner touching triangle edge
        {
            bool result = Check3D_Triangle_Triangle_Overlap(
                glm::vec3(-0.1f, 0.5f, 0.0f),
                glm::vec3(-0.3f, 0.1f, 0.2f),
                glm::vec3(-0.5f, 0.3f, 0.4f),
                glm::vec3(-0.2f, 0.3f, 0.1f),
                glm::vec3(0.2f, 0.1f, -0.1f),
                glm::vec3(0.4f, 0.5f, 0.0f));

            if (result != true) return 6;
        }

        // 07: triangle edge touching triangle edge
        {
            bool result = Check3D_Triangle_Triangle_Overlap(
                glm::vec3(0.1f, 0.5f, -0.1f),
                glm::vec3(0.1f, 0.0f, 0.1f),
                glm::vec3(-0.5f, 0.3f, 0.0f),
                glm::vec3(0.1f, 0.4f, 0.1f),
                glm::vec3(0.1f, 0.0f, -0.1f),
                glm::vec3(0.5f, 0.2f, 0.0f));

            if (result != true) return 7;
        }

        // 08: triangle corner touching triangle side
        {
            bool result = Check3D_Triangle_Triangle_Overlap(
                glm::vec3(0.1f, 0.5f, -0.1f),
                glm::vec3(-0.1f, 0.3f, 0.1f),
                glm::vec3(0.1f, 0.1f, -0.1f),
                glm::vec3(0.0f, 0.3f, 0.0f),
                glm::vec3(0.4f, 0.1f, -0.1f),
                glm::vec3(0.2f, 0.5f, 0.1f));

            if (result != true) return 8;
        }

        // 08: triangle inside other triangle
        {
            bool result = Check3D_Triangle_Triangle_Overlap(
                glm::vec3(-0.1f, 0.3f, -0.1f),
                glm::vec3(0.3f, -0.1f, -0.1f),
                glm::vec3(0.5f, 0.5f, -0.1f),
                glm::vec3(0.1f, 0.2f, -0.1f),
                glm::vec3(0.3f, 0.1f, -0.1f),
                glm::vec3(0.3f, 0.3f, -0.1f));

            if (result != true) return 8;
        }

        // 09: triangle NOT inside other triangle
        {
            bool result = Check3D_Triangle_Triangle_Overlap(
                glm::vec3(-0.1f, 0.3f, 0.0f),
                glm::vec3(0.3f, -0.1f, 0.0f),
                glm::vec3(0.5f, 0.5f, 0.0f),
                glm::vec3(0.1f, 0.2f, -0.1f),
                glm::vec3(0.3f, 0.1f, -0.1f),
                glm::vec3(0.3f, 0.3f, -0.1f));

            if (result != false) return 9;
        }

        // 10: triangles intersecting
        {
            bool result = Check3D_Triangle_Triangle_Overlap(
                glm::vec3(-0.1f, 0.3f, -0.1f),
                glm::vec3(0.5f, 0.3f, -0.1f),
                glm::vec3(0.3f, 0.5f, 0.1f),
                glm::vec3(0.1f, 0.5f, -0.1f),
                glm::vec3(0.2f, -0.1f, 0.1f),
                glm::vec3(0.5f, 0.1f, 0.1f));

            if (result != true) return 10;
        }

        // 11: triangles NOT overlapping (parallel SAT axis)
        {
            bool result = Check3D_Triangle_Triangle_Overlap(
                glm::vec3(-0.2f, -0.1f, 0.0f),
                glm::vec3(-0.3f, 0.0f, 0.0f),
                glm::vec3(-0.1f, 0.0f, 0.0f),
                glm::vec3(0.2f, 0.1f, 0.0f),
                glm::vec3(0.3f, 0.0f, 0.0f),
                glm::vec3(0.1f, 0.0f, 0.0f));

            if (result != false) return 11;
        }

        // 12: triangles NOT overlapping (parallel SAT axis)
        {
            bool result = Check3D_Triangle_Triangle_Overlap(
                glm::vec3(-0.2f, -0.1f, 0.0f),
                glm::vec3(-0.3f, 0.0f, 0.0f),
                glm::vec3(-0.1f, 0.0f, 0.0f),
                glm::vec3(0.2f, -0.1f, 0.0f),
                glm::vec3(0.3f, 0.0f, 0.0f),
                glm::vec3(0.1f, 0.0f, 0.0f));

            if (result != false) return 12;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Triangle_AABB_Overlap()
    {
        // 01: point triangle and point AABB at origin
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f));

            if (result != true) return 1;
        }

        // 02: point triangle and point AABB
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f));

            if (result != true) return 2;
        }

        // 03: point triangle on AABB corner
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(-1.0f, -2.0f, -3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f));

            if (result != true) return 3;
        }

        // 04: point triangle on AABB edge
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(1.0f, 2.0f, 1.0f),
                glm::vec3(1.0f, 2.0f, 1.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(-1.0f, -2.0f, -3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f));

            if (result != true) return 4;
        }

        // 05: triangle with point AABB on corner
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f));

            if (result != true) return 5;
        }

        // 06: triangle with point AABB on edge
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 2.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f));

            if (result != true) return 6;
        }

        // 07: triangle not overlapping point AABB
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 2.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));

            if (result != false) return 7;
        }

        // 08: triangle on AABB's side
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(0.0f, 0.0f, 3.0f),
                glm::vec3(0.8f, 0.0f, 3.0f),
                glm::vec3(0.8f, 0.8f, 3.0f),
                glm::vec3(-1.0f, -2.0f, -3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f));

            if (result != true) return 8;
        }

        // 09: triangle near AABB's side (not overlapping)
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(0.0f, 0.0f, 3.1f),
                glm::vec3(0.8f, 0.0f, 3.1f),
                glm::vec3(0.8f, 0.8f, 3.1f),
                glm::vec3(-1.0f, -2.0f, -3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f));

            if (result != false) return 9;
        }

        // 10: triangle and AABB corners overlapping
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(1.0f, 2.0f, 3.0f));

            if (result != true) return 10;
        }

        // 11: triangle and AABB edge overlapping
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(1.0f, 0.2f, -1.0f),
                glm::vec3(2.0f, 0.8f, 1.0f));

            if (result != true) return 11;
        }

        // 12: triangle inside AABB
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(0.4f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.4f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.4f),
                glm::vec3(-1.0f, -2.0f, -3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f));

            if (result != true) return 12;
        }


        // 13: triangle not overlapping AABB
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(0.4f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.4f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.4f),
                glm::vec3(-1.0f, -2.0f, -3.0f),
                glm::vec3(0.0f, 0.0f, 0.0f));

            if (result != false) return 13;
        }

        // 14: triangle overlapping AABB
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f) / 3.0f,
                glm::vec3(1.0f, 1.0f, 1.0f));

            if (result != true) return 14;
        }

        // 15: triangle NOT overlapping AABB
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f) / 3.0f + glm::vec3(0.01f, 0.01f, 0.01f),
                glm::vec3(1.0f, 1.0f, 1.0f));

            if (result != false) return 15;
        }

        // 16: triangle NOT overlapping AABB
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(0.4f, 0.0f, 0.0f),
                glm::vec3(0.4f, 0.4f, 0.0f),
                glm::vec3(0.0f, 0.4f, 0.0f),
                glm::vec3(-0.1f, -0.2f, 0.0f),
                glm::vec3(0.1f, 0.2f, 0.0f));

            if (result != false) return 16;
        }

        // 17: triangle NOT overlapping AABB
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(0.4f, 0.2f, 0.0f),
                glm::vec3(0.8f, 0.2f, 0.0f),
                glm::vec3(0.4f, 0.6f, 0.0f),
                glm::vec3(-0.1f, -0.2f, 0.0f),
                glm::vec3(0.1f, 0.2f, 0.0f));

            if (result != false) return 17;
        }

        // 18: triangle corner touching AABB side
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(0.0f, 0.2f, 0.2f),
                glm::vec3(0.6f, 0.4f, 0.2f),
                glm::vec3(0.2f, 0.6f, 0.2f),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != true) return 18;
        }

        // 19: triangle not overlapping AABB
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(-0.3f, 0.1f, 0.3f),
                glm::vec3(-0.3f, 0.2f, 0.3f),
                glm::vec3(-0.2f, 0.2f, 0.3f),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != false) return 19;
        }

        // 20: triangle not overlapping AABB
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(-0.1f, 0.3f, 0.3f),
                glm::vec3(-0.1f, 0.4f, 0.3f),
                glm::vec3(0.0f, 0.3f, 0.3f),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != false) return 20;
        }

        // 21: triangle not overlapping AABB
        {
            bool result = Check3D_Triangle_AABB_Overlap(
                glm::vec3(0.1f, 0.2f, 0.4f),
                glm::vec3(0.1f, 0.3f, 0.5f),
                glm::vec3(0.1f, 0.2f, 0.5f),
                glm::vec3(-0.1f, -0.2f, -0.3f),
                glm::vec3(0.1f, 0.2f, 0.3f));

            if (result != false) return 21;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Triangle_OBB_Overlap()
    {
        // 01: point triangle and point OBB at origin
        {
            bool result = Check3D_Triangle_OBB_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

            if (result != true) return 1;
        }

        // 02: point triangle and point OBB
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));

            bool result = Check3D_Triangle_OBB_Overlap(
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 2.0f, 3.0f),
                rotation2 * rotation1);

            if (result != true) return 2;
        }

        // 03: triangle corner touching OBB corner
        {
            glm::vec3 position00(0.1f, 0.2f, 0.3f);
            glm::vec3 position01(0.5f, 0.2f, 0.3f);
            glm::vec3 position02(0.1f, 0.6f, 0.3f);

            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));

            glm::vec3 translation = glm::vec3(0.3f, 0.2f, 0.1f);

            position00 = rotation2 * rotation1 * position00;
            position01 = rotation2 * rotation1 * position01;
            position02 = rotation2 * rotation1 * position02;

            position00 += translation;
            position01 += translation;
            position02 += translation;

            bool result = Check3D_Triangle_OBB_Overlap(
                position00,
                position01,
                position02,
                glm::vec3(0.1f, 0.2f, 0.3f),
                translation,
                rotation2 * rotation1);

            if (result != true) return 3;
        }

        // 04: triangle NOT touching OBB
        {
            glm::vec3 position00(0.2f, 0.2f, 0.3f);
            glm::vec3 position01(0.5f, 0.2f, 0.3f);
            glm::vec3 position02(0.1f, 0.6f, 0.3f);

            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));

            glm::vec3 translation = glm::vec3(0.3f, 0.2f, 0.1f);

            position00 = rotation2 * rotation1 * position00;
            position01 = rotation2 * rotation1 * position01;
            position02 = rotation2 * rotation1 * position02;

            position00 += translation;
            position01 += translation;
            position02 += translation;

            bool result = Check3D_Triangle_OBB_Overlap(
                position00,
                position01,
                position02,
                glm::vec3(0.1f, 0.2f, 0.3f),
                translation,
                rotation2 * rotation1);

            if (result != false) return 4;
        }

        // 05: triangle corner touching OBB edge
        {
            glm::vec3 position00(0.1f, 0.2f, 0.1f);
            glm::vec3 position01(0.5f, 0.2f, 0.1f);
            glm::vec3 position02(0.1f, 0.6f, 0.1f);

            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));

            glm::vec3 translation = glm::vec3(0.3f, 0.2f, 0.1f);

            position00 = rotation2 * rotation1 * position00;
            position01 = rotation2 * rotation1 * position01;
            position02 = rotation2 * rotation1 * position02;

            position00 += translation;
            position01 += translation;
            position02 += translation;

            bool result = Check3D_Triangle_OBB_Overlap(
                position00,
                position01,
                position02,
                glm::vec3(0.1f, 0.2f, 0.3f),
                translation,
                rotation2 * rotation1);

            if (result != true) return 5;
        }

        // 06: triangle NOT touching OBB
        {
            glm::vec3 position00(0.1001f, 0.2f, 0.1f);
            glm::vec3 position01(0.5f, 0.2f, 0.1f);
            glm::vec3 position02(0.1f, 0.6f, 0.1f);

            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));

            glm::vec3 translation = glm::vec3(0.3f, 0.2f, 0.1f);

            position00 = rotation2 * rotation1 * position00;
            position01 = rotation2 * rotation1 * position01;
            position02 = rotation2 * rotation1 * position02;

            position00 += translation;
            position01 += translation;
            position02 += translation;

            bool result = Check3D_Triangle_OBB_Overlap(
                position00,
                position01,
                position02,
                glm::vec3(0.1f, 0.2f, 0.3f),
                translation,
                rotation2 * rotation1);

            if (result != false) return 6;
        }

        // 07: triangle corner touching OBB side
        {
            glm::vec3 position00(0.0f, 0.2f, 0.2f);
            glm::vec3 position01(0.6f, 0.4f, 0.2f);
            glm::vec3 position02(0.2f, 0.6f, 0.2f);

            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));

            glm::vec3 translation = glm::vec3(0.3f, 0.2f, 0.1f);

            position00 = rotation2 * rotation1 * position00;
            position01 = rotation2 * rotation1 * position01;
            position02 = rotation2 * rotation1 * position02;

            position00 += translation;
            position01 += translation;
            position02 += translation;

            bool result = Check3D_Triangle_OBB_Overlap(
                position00,
                position01,
                position02,
                glm::vec3(0.1f, 0.2f, 0.3f),
                translation,
                rotation2 * rotation1);

            if (result != true) return 7;
        }

        // 08: triangle NOT touching OBB
        {
            glm::vec3 position00(0.0f, 0.2001f, 0.2f);
            glm::vec3 position01(0.6f, 0.4f, 0.2f);
            glm::vec3 position02(0.2f, 0.6f, 0.2f);

            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));

            glm::vec3 translation = glm::vec3(0.3f, 0.2f, 0.1f);

            position00 = rotation2 * rotation1 * position00;
            position01 = rotation2 * rotation1 * position01;
            position02 = rotation2 * rotation1 * position02;

            position00 += translation;
            position01 += translation;
            position02 += translation;

            bool result = Check3D_Triangle_OBB_Overlap(
                position00,
                position01,
                position02,
                glm::vec3(0.1f, 0.2f, 0.3f),
                translation,
                rotation2 * rotation1);

            if (result != false) return 8;
        }

        // 09: triangle inside OBB
        {
            glm::vec3 position00(0.05f, 0.1f, 0.2f);
            glm::vec3 position01(0.0f, 0.0f, 0.1f);
            glm::vec3 position02(-0.05f, -0.1f, -0.2f);

            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));

            glm::vec3 translation = glm::vec3(0.3f, 0.2f, 0.1f);

            position00 = rotation2 * rotation1 * position00;
            position01 = rotation2 * rotation1 * position01;
            position02 = rotation2 * rotation1 * position02;

            position00 += translation;
            position01 += translation;
            position02 += translation;

            bool result = Check3D_Triangle_OBB_Overlap(
                position00,
                position01,
                position02,
                glm::vec3(0.1f, 0.2f, 0.3f),
                translation,
                rotation2 * rotation1);

            if (result != true) return 9;
        }

        // 10: triangle going through OBB
        {
            glm::vec3 position00(0.2f, 0.1f, 0.2f);
            glm::vec3 position01(0.4f, 0.0f, 0.1f);
            glm::vec3 position02(-0.2f, -0.1f, -0.2f);

            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));

            glm::vec3 translation = glm::vec3(0.3f, 0.2f, 0.1f);

            position00 = rotation2 * rotation1 * position00;
            position01 = rotation2 * rotation1 * position01;
            position02 = rotation2 * rotation1 * position02;

            position00 += translation;
            position01 += translation;
            position02 += translation;

            bool result = Check3D_Triangle_OBB_Overlap(
                position00,
                position01,
                position02,
                glm::vec3(0.1f, 0.2f, 0.3f),
                translation,
                rotation2 * rotation1);

            if (result != true) return 10;
        }

        // 11: triangle NOT overlapping OBB
        {
            glm::vec3 position00(0.2f, 0.1f, 0.2f);
            glm::vec3 position01(0.4f, 0.0f, 0.1f);
            glm::vec3 position02(0.0f, 0.4f, -0.2f);

            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));

            glm::vec3 translation = glm::vec3(0.3f, 0.2f, 0.1f);

            position00 = rotation2 * rotation1 * position00;
            position01 = rotation2 * rotation1 * position01;
            position02 = rotation2 * rotation1 * position02;

            position00 += translation;
            position01 += translation;
            position02 += translation;

            bool result = Check3D_Triangle_OBB_Overlap(
                position00,
                position01,
                position02,
                glm::vec3(0.1f, 0.2f, 0.3f),
                translation,
                rotation2 * rotation1);

            if (result != false) return 11;
        }

        return 0;
    }

    inline int UnitTest_Check3D_Triangle_Sphere_Overlap()
    {
        // 01: triangle inside sphere at origin
        {
            bool result = Check3D_Triangle_Sphere_Overlap(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::vec3(-1.0f, 1.0f, 0.0f),
                glm::vec3(-1.0f, -1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != true) return 1;
        }

        // 02: triangle touching sphere at origin's edge
        {
            bool result = Check3D_Triangle_Sphere_Overlap(
                glm::vec3(-1.0f, -4.0f, 0.0f),
                glm::vec3(1.0f, -4.0f, 0.0f),
                glm::vec3(0.0f, -2.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != true) return 2;
        }

        // 03: triangle overlaping sphere at origin's edge
        {
            bool result = Check3D_Triangle_Sphere_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, -2.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != true) return 3;
        }

        // 04: triangle not overlaping sphere at origin
        {
            bool result = Check3D_Triangle_Sphere_Overlap(
                glm::vec3(-3.0f, 0.0f, 0.0f),
                glm::vec3(-3.0f, 3.0f, 0.0f),
                glm::vec3(0.0f, 3.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != false) return 4;
        }

        // 05: triangle not overlaping sphere
        {
            bool result = Check3D_Triangle_Sphere_Overlap(
                glm::vec3(0.0f, 3.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(3.0f, 0.0f, 0.0f),
                glm::vec3(3.0f, 3.0f, 0.0f),
                1.0f);

            if (result != false) return 5;
        }

        // 06: triangle overlaping infinitly small sphere
        {
            bool result = Check3D_Triangle_Sphere_Overlap(
                glm::vec3(0.0f, 3.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(3.0f, 0.0f, 0.0f),
                glm::vec3(3.0f, 0.0f, 0.0f),
                0.0f);

            if (result != true) return 6;
        }

        // 07: point triangle overlaping infinitly small sphere
        {
            bool result = Check3D_Triangle_Sphere_Overlap(
                glm::vec3(3.0f, 0.0f, 0.0f),
                glm::vec3(3.0f, 0.0f, 0.0f),
                glm::vec3(3.0f, 0.0f, 0.0f),
                glm::vec3(3.0f, 0.0f, 0.0f),
                0.0f);

            if (result != true) return 7;
        }

        return 0;
    }

    // -------------------------------------------------------------------------

    inline int UnitTest_Check3D_AABB_AABB_Overlap()
    {
        // 01: AABB inside AABB
        {
            bool result = Check3D_AABB_AABB_Overlap(
                glm::vec3(-2.0f, -2.0f, -2.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f));

            if (result != true) return 1;
        }

        // 02: point AABB inside AABB
        {
            bool result = Check3D_AABB_AABB_Overlap(
                glm::vec3(-2.0f, -2.0f, -2.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f));

            if (result != true) return 2;
        }

        // 03: AABB overlapping AABB
        {
            bool result = Check3D_AABB_AABB_Overlap(
                glm::vec3(-2.0f, -2.0f, -2.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 2.0f, 2.0f));

            if (result != true) return 3;
        }

        // 04: AABB not overlapping AABB
        {
            bool result = Check3D_AABB_AABB_Overlap(
                glm::vec3(-2.0f, -2.0f, -2.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(2.0f, 2.0f, 2.0f));

            if (result != false) return 4;
        }

        // 05: point AABB overlapping point AABB
        {
            bool result = Check3D_AABB_AABB_Overlap(
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(2.0f, 2.0f, 2.0f));

            if (result != true) return 5;
        }

        // 06: AABB touching AABB corner
        {
            bool result = Check3D_AABB_AABB_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(4.0f, 4.0f, 4.0f));

            if (result != true) return 6;
        }

        // 07: AABB touching AABB corner
        {
            bool result = Check3D_AABB_AABB_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(0.0f, 1.0f, 1.0f),
                glm::vec3(2.0f, 3.0f, 3.0f));

            if (result != true) return 7;
        }

        return 0;
    }

    inline int UnitTest_Check3D_AABB_OBB_Overlap()
    {
        // 01: point AABB overlapping point OBB
        {
            bool result = Check3D_AABB_OBB_Overlap(
                glm::vec3(-2.0f, -2.0f, -2.0f),
                glm::vec3(-2.0f, -2.0f, -2.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(-2.0f, -2.0f, -2.0f),
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

            if (result != true) return 1;
        }

        // 02: point AABB overlapping point OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_AABB_OBB_Overlap(
                glm::vec3(-2.0f, -2.0f, -2.0f),
                glm::vec3(-2.0f, -2.0f, -2.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(-2.0f, -2.0f, -2.0f),
                rotation);

            if (result != true) return 2;
        }

        // 03: point AABB NOT overlapping point OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_AABB_OBB_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.1f),
                rotation);

            if (result != false) return 3;
        }

        // 04: AABB inside OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_AABB_OBB_Overlap(
                glm::vec3(0.2f, 0.2f, 0.2f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                rotation);

            if (result != true) return 4;
        }

        // 05: OBB NOT inside AABB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_AABB_OBB_Overlap(
                glm::vec3(3.0f, 3.0f, 3.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                rotation);

            if (result != false) return 5;
        }

        // 06: OBB inside AABB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_AABB_OBB_Overlap(
                glm::vec3(3.0f, 3.0f, 3.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(0.1f, 0.2f, 0.3f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                rotation);

            if (result != true) return 6;
        }

        // 07: AABB NOT overlapping OBB
        {
            glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            bool result = Check3D_AABB_OBB_Overlap(
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(-0.25f, -0.5f, -1.0f),
                glm::vec3(0.5f, 1.0f, 2.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                rotation);

            if (result != false) return 7;
        }

        // 08: AABB NOT overlapping OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>(),
                glm::vec3(1.0f, 0.0f, 0.0f));
            bool result = Check3D_AABB_OBB_Overlap(
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(-0.25f, -0.5f, -1.0f),
                glm::vec3(0.5f, 1.0f, 2.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                rotation);

            if (result != false) return 8;
        }

        // 09: AABB NOT overlapping OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>(),
                glm::vec3(1.0f, 0.0f, 0.0f));
            rotation = glm::rotate(
                rotation,
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_AABB_OBB_Overlap(
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(-0.25f, -0.5f, -1.0f),
                glm::vec3(0.5f, 1.0f, 2.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                rotation);

            if (result != false) return 9;
        }

        // 10: AABB overlapping OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>(),
                glm::vec3(1.0f, 0.0f, 0.0f));
            rotation = glm::rotate(
                rotation,
                glm::half_pi<float>() + glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_AABB_OBB_Overlap(
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(-0.25f, -0.5f, -1.0f),
                glm::vec3(0.5f, 1.0f, 2.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                rotation);

            if (result != true) return 10;
        }

        // 11: AABB overlapping OBB (sides touching)
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_AABB_OBB_Overlap(
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(-0.25f, -0.5f, -1.0f),
                glm::vec3(0.5f, 1.0f, 2.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                rotation);

            if (result != true) return 11;
        }

        // 12: AABB overlapping OBB (corners touching)
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_AABB_OBB_Overlap(
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(-0.25f, -0.5f, -1.0f),
                glm::vec3(0.5f, 1.0f, 2.0f),
                glm::vec3(1.25f, 1.0f, 1.25f),
                rotation);

            if (result != true) return 12;
        }

        // 13: AABB Not overlapping OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_AABB_OBB_Overlap(
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(-0.25f, -0.5f, -1.0f),
                glm::vec3(0.5f, 1.0f, 2.0f),
                glm::vec3(1.25f, 1.25f, 1.25f),
                rotation);

            if (result != true) return 13;
        }

        return 0;
    }

    inline int UnitTest_Check3D_AABB_Sphere_Overlap()
    {
        // 01: point AABB overlapping point Sphere
        {
            bool result = Check3D_AABB_Sphere_Overlap(
                glm::vec3(-2.0f, -2.0f, -2.0f),
                glm::vec3(-2.0f, -2.0f, -2.0f),
                glm::vec3(-2.0f, -2.0f, -2.0f),
                0.0f);

            if (result != true) return 1;
        }

        // 02: AABB corner touching point Sphere
        {
            bool result = Check3D_AABB_Sphere_Overlap(
                glm::vec3(-1.0f, -2.0f, -3.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                0.0f);

            if (result != true) return 2;
        }

        // 03: point AABB touching Sphere edge
        {
            bool result = Check3D_AABB_Sphere_Overlap(
                glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                1.0f);

            if (result != true) return 3;
        }

        // 04: AABB overlapping Sphere
        {
            bool result = Check3D_AABB_Sphere_Overlap(
                glm::vec3(-2.0f, -2.0f, -2.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(3.0f, 0.0f, 0.0f),
                1.0f);

            if (result != true) return 4;
        }

        // 05: AABB not overlapping Sphere
        {
            bool result = Check3D_AABB_Sphere_Overlap(
                glm::vec3(-2.0f, -2.0f, -2.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(4.0f, 0.0f, 0.0f),
                1.0f);

            if (result != false) return 5;
        }

        // 06: AABB inside Sphere
        {
            bool result = Check3D_AABB_Sphere_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                4.0f);

            if (result != true) return 6;
        }

        // 07: Sphere inside AABB
        {
            bool result = Check3D_AABB_Sphere_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(4.0f, 4.0f, 4.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                1.0f);

            if (result != true) return 7;
        }

        // 08: tiny AABB and tiny sphere overlapping
        {
            bool result = Check3D_AABB_Sphere_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                0.0f);

            if (result != true) return 8;
        }

        // 09: tiny AABB and tiny sphere overlapping
        {
            bool result = Check3D_AABB_Sphere_Overlap(
                glm::vec3(-1.0f, -2.0f, -3.0f),
                glm::vec3(-1.0f, -2.0f, -3.0f),
                glm::vec3(-1.0f, -2.0f, -3.0f),
                0.0f);

            if (result != true) return 9;
        }

        return 0;
    }

    // -------------------------------------------------------------------------

    inline int UnitTest_Check3D_OBB_OBB_Overlap()
    {
        // 01: point OBBs at origin
        {
            bool result = Check3D_OBB_OBB_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

            if (result != true) return 1;
        }

        // 02: point OBBs at overlapping
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_OBB_OBB_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, -2.0f, -3.0f),
                rotation1,
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, -2.0f, -3.0f),
                rotation2);

            if (result != true) return 2;
        }

        // 03: OBB touching OBB
        {
            glm::quat rotation1 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_OBB_OBB_Overlap(
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                rotation1,
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                rotation2);

            if (result != true) return 3;
        }

        // 04: OBB not overlapping OBB
        {
            glm::quat rotation1 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            glm::quat rotation2 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            bool result = Check3D_OBB_OBB_Overlap(
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                rotation1,
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                rotation2);

            if (result != false) return 4;
        }

        // 05: OBB overlapping OBB
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                -glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            glm::quat rotation2 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            bool result = Check3D_OBB_OBB_Overlap(
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                rotation1,
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                rotation2);

            if (result != true) return 5;
        }

        // 06: OBB not overlapping OBB
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(1.0f, 0.0f, 0.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            bool result = Check3D_OBB_OBB_Overlap(
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                rotation1,
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                rotation2);

            if (result != false) return 6;
        }

        // 07: OBB overlapping OBB
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(1.0f, 0.0f, 0.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            rotation2 = glm::rotate(
                rotation2,
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_OBB_OBB_Overlap(
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                rotation1,
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                rotation2);

            if (result != false) return 7;
        }

        // 08: OBB overlapping OBB
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(1.0f, 0.0f, 0.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            rotation2 = glm::rotate(
                rotation2,
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_OBB_OBB_Overlap(
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                rotation1,
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(1.0f, 0.0f, 1.0f),
                rotation2);

            if (result != true) return 8;
        }

        // 09: OBB overlapping OBB
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                -glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            rotation1 = glm::rotate(
                rotation1,
                -glm::quarter_pi<float>(),
                glm::vec3(1.0f, 0.0f, 0.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::half_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            rotation2 = glm::rotate(
                rotation2,
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_OBB_OBB_Overlap(
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                rotation1,
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(1.0f, 0.0f, 1.0f),
                rotation2);

            if (result != true) return 9;
        }

        // 10: OBB overlapping OBB
        {
            glm::quat rotation1 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                -glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            rotation1 = glm::rotate(
                rotation1,
                -glm::quarter_pi<float>(),
                glm::vec3(1.0f, 0.0f, 0.0f));
            glm::quat rotation2 = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                -glm::quarter_pi<float>(),
                glm::vec3(0.0f, 1.0f, 0.0f));
            rotation2 = glm::rotate(
                rotation2,
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_OBB_OBB_Overlap(
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                rotation1,
                glm::vec3(0.25f, 0.5f, 1.0f),
                glm::vec3(1.0f, 0.0f, 1.25f),
                rotation2);

            if (result != false) return 10;
        }

        return 0;
    }

    inline int UnitTest_Check3D_OBB_Sphere_Overlap()
    {
        // 01: point OBB overlapping point Sphere
        {
            bool result = Check3D_OBB_Sphere_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                0.0f);

            if (result != true) return 1;
        }

        // 02: point OBB touching Sphere edge
        {
            bool result = Check3D_OBB_Sphere_Overlap(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                1.0f);

            if (result != true) return 2;
        }

        // 03: thin OBB's edge touching point Sphere
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_OBB_Sphere_Overlap(
                glm::vec3(4.0f, 1.0f, 0.0f),
                glm::vec3(4.0f, 4.0f, 0.0f),
                rotation,
                glm::vec3(4.0f * glm::sqrt(2.0f) - 4.0f, 4.0f * glm::sqrt(2.0f) - 4.0f, 0.0f),
                0.0f);

            if (result != true) return 3;
        }

        // 04: thin OBB's edge not touching point Sphere
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_OBB_Sphere_Overlap(
                glm::vec3(4.0f, 1.0f, 0.0f),
                glm::vec3(4.0f, 4.0f, 0.0f),
                rotation,
                glm::vec3(1.0f, 1.0f, 0.0f),
                0.0f);

            if (result != false) return 4;
        }

        // 05: thin OBB's edge touching Sphere at origin
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_OBB_Sphere_Overlap(
                glm::vec3(4.0f, 1.0f, 0.0f),
                glm::vec3(4.0f, 4.0f, 0.0f),
                rotation,
                glm::vec3(0.0f, 0.0f, 0.0f),
                4.0f * glm::sqrt(2.0f) - 4.0f);

            if (result != true) return 5;
        }

        // 06: thin OBB's edge not touching Sphere at origin
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_OBB_Sphere_Overlap(
                glm::vec3(4.0f, 1.0f, 0.0f),
                glm::vec3(4.0f, 4.0f, 0.0f),
                rotation,
                glm::vec3(0.0f, 0.0f, 0.0f),
                1.0f);

            if (result != false) return 6;
        }

        // 07: OBB at origin overlapping Sphere at origin
        {
            bool result = Check3D_OBB_Sphere_Overlap(
                glm::vec3(2.0f, -1.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                2.0f);

            if (result != true) return 7;
        }

        // 08: OBB not overlapping Sphere
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                -glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_OBB_Sphere_Overlap(
                glm::vec3(3.0f, 1.0f, 1.0f),
                glm::vec3(5.0f, 5.0f, 0.0f),
                rotation,
                glm::vec3(3.0f, 1.0f, 0.0f),
                2.0f);

            if (result != false) return 8;
        }

        // 09: OBB overlapping Sphere
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                -glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_OBB_Sphere_Overlap(
                glm::vec3(3.0f, 1.0f, 1.0f),
                glm::vec3(5.0f, 5.0f, 0.0f),
                rotation,
                glm::vec3(3.0f, 9.0f, 0.0f),
                2.0f);

            if (result != true) return 9;
        }

        // 10: OBB not overlapping Sphere
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                -glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));
            bool result = Check3D_OBB_Sphere_Overlap(
                glm::vec3(3.0f, 1.0f, 1.0f),
                glm::vec3(5.0f, 5.0f, 0.0f),
                rotation,
                glm::vec3(9.0f, 6.0f, 0.0f),
                2.0f);

            if (result != false) return 10;
        }

        return 0;
    }

    // -------------------------------------------------------------------------

    inline int UnitTest_Check3D_Sphere_Sphere_Overlap()
    {
        // 01: point Sphere overlapping point Sphere
        {
            bool result = Check3D_Sphere_Sphere_Overlap(
                glm::vec3(-1.0f, -2.0f, -3.0f),
                0.0f,
                glm::vec3(-1.0f, -2.0f, -3.0f),
                0.0f);

            if (result != true) return 1;
        }

        // 02: Sphere overlapping point Sphere
        {
            bool result = Check3D_Sphere_Sphere_Overlap(
                glm::vec3(-1.0f, 0.0f, 0.0f),
                2.0f,
                glm::vec3(1.0f, 0.0f, 0.0f),
                0.0f);

            if (result != true) return 2;
        }

        // 03: Sphere inside Sphere
        {
            bool result = Check3D_Sphere_Sphere_Overlap(
                glm::vec3(0.0f, 1.0f, 0.0f),
                1.0f,
                glm::vec3(1.0f, 0.0f, 0.0f),
                4.0f);

            if (result != true) return 3;
        }

        // 04: Sphere touching Sphere
        {
            bool result = Check3D_Sphere_Sphere_Overlap(
                glm::vec3(-1.0f, 0.0f, 0.0f),
                1.0f,
                glm::vec3(1.0f, 0.0f, 0.0f),
                1.0f);

            if (result != true) return 4;
        }

        // 05: Sphere overlapping Sphere
        {
            bool result = Check3D_Sphere_Sphere_Overlap(
                glm::vec3(-2.0f, 0.0f, 0.0f),
                3.0f,
                glm::vec3(2.0f, 0.0f, 0.0f),
                3.0f);

            if (result != true) return 5;
        }

        // 06: Sphere not overlapping Sphere
        {
            bool result = Check3D_Sphere_Sphere_Overlap(
                glm::vec3(-2.0f, 0.0f, 0.0f),
                1.0f,
                glm::vec3(2.0f, 0.0f, 0.0f),
                1.0f);

            if (result != false) return 6;
        }

        return 0;
    }

    // -------------------------------------------------------------------------

    inline int UnitTest_Get3D_Point_Line_ClosestPoint()
    {
        // 01: origin point and x-axis line through origin
        {
            glm::vec3 closestPoint;
            Get3D_Point_Line_ClosestPoint(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 0.0f) &&
                FloatEqualToFloat(closestPoint.y, 0.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 1;
            }
        }

        // 02: point and line
        {
            glm::vec3 closestPoint;
            Get3D_Point_Line_ClosestPoint(
                glm::vec3(1.0f, 1.0f, -2.0f),
                glm::vec3(-1.0f, 1.0f, 2.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f)),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 2.0f) &&
                FloatEqualToFloat(closestPoint.y, 1.0f) &&
                FloatEqualToFloat(closestPoint.z, -1.0f)))
            {
                return 2;
            }
        }

        // 03: point and line
        {
            glm::vec3 closestPoint;
            Get3D_Point_Line_ClosestPoint(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, 0.0f, 2.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f)),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 1.0f) &&
                FloatEqualToFloat(closestPoint.y, 0.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 3;
            }
        }

        // 04: point and line
        {
            glm::vec3 closestPoint;
            Get3D_Point_Line_ClosestPoint(
                glm::vec3(-1.0f, -2.0f, 4.0f),
                glm::vec3(0.0f, -2.0f, 1.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f)),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, -2.0f) &&
                FloatEqualToFloat(closestPoint.y, -2.0f) &&
                FloatEqualToFloat(closestPoint.z, 3.0f)))
            {
                return 4;
            }
        }

        return 0;
    }

    inline int UnitTest_Get3D_Point_Ray_ClosestPoint()
    {
        // 01: origin point and x-axis ray through origin
        {
            glm::vec3 closestPoint;
            Get3D_Point_Ray_ClosestPoint(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 0.0f) &&
                FloatEqualToFloat(closestPoint.y, 0.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 1;
            }
        }

        // 02: point and ray
        {
            glm::vec3 closestPoint;
            Get3D_Point_Ray_ClosestPoint(
                glm::vec3(1.0f, 1.0f, -2.0f),
                glm::vec3(-1.0f, 1.0f, 2.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f)),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 2.0f) &&
                FloatEqualToFloat(closestPoint.y, 1.0f) &&
                FloatEqualToFloat(closestPoint.z, -1.0f)))
            {
                return 2;
            }
        }

        // 03: point and ray
        {
            glm::vec3 closestPoint;
            Get3D_Point_Ray_ClosestPoint(
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, 0.0f, 2.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f)),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 1.0f) &&
                FloatEqualToFloat(closestPoint.y, 0.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 3;
            }
        }

        // 04: point and ray
        {
            glm::vec3 closestPoint;
            Get3D_Point_Ray_ClosestPoint(
                glm::vec3(-1.0f, -2.0f, 4.0f),
                glm::vec3(0.0f, -2.0f, 1.0f),
                glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f)),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 0.0f) &&
                FloatEqualToFloat(closestPoint.y, -2.0f) &&
                FloatEqualToFloat(closestPoint.z, 1.0f)))
            {
                return 4;
            }
        }

        return 0;
    }

    inline int UnitTest_Get3D_Point_LineSegment_ClosestPoint()
    {
        // 01: origin point and lineSegment through origin
        {
            glm::vec3 closestPoint;
            Get3D_Point_LineSegment_ClosestPoint(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 0.0f) &&
                FloatEqualToFloat(closestPoint.y, 0.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 1;
            }
        }

        // 02:
        {
            glm::vec3 closestPoint;
            Get3D_Point_LineSegment_ClosestPoint(
                glm::vec3(-1.0f, -1.0f, 2.0f),
                glm::vec3(-1.0f, -1.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 0.0f),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, -1.0f) &&
                FloatEqualToFloat(closestPoint.y, -1.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 2;
            }
        }

        // 03:
        {
            glm::vec3 closestPoint;
            Get3D_Point_LineSegment_ClosestPoint(
                glm::vec3(1.0f, 1.0f, 2.0f),
                glm::vec3(-1.0f, -1.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 0.0f),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 1.0f) &&
                FloatEqualToFloat(closestPoint.y, 1.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 3;
            }
        }

        // 04:
        {
            glm::vec3 closestPoint;
            Get3D_Point_LineSegment_ClosestPoint(
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(-1.0f, -1.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 0.0f),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 1.0f) &&
                FloatEqualToFloat(closestPoint.y, 1.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 4;
            }
        }

        return 0;
    }

    inline int UnitTest_Get3D_Point_Plane_ClosestPoint()
    {
        // 01: origin point and xy plane on origin
        {
            glm::vec3 closestPoint;
            Get3D_Point_Plane_ClosestPoint(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                0.0f,
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 0.0f) &&
                FloatEqualToFloat(closestPoint.y, 0.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 1;
            }
        }

        // 02: point and xy plane on origin
        {
            glm::vec3 closestPoint;
            Get3D_Point_Plane_ClosestPoint(
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                0.0f,
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, -1.0f) &&
                FloatEqualToFloat(closestPoint.y, -1.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 2;
            }
        }

        // 03: point and plane
        {
            glm::vec3 closestPoint;
            Get3D_Point_Plane_ClosestPoint(
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
                glm::sqrt(3.0f),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 1.0f) &&
                FloatEqualToFloat(closestPoint.y, 1.0f) &&
                FloatEqualToFloat(closestPoint.z, 1.0f)))
            {
                return 3;
            }
        }

        return 0;
    }

    inline int UnitTest_Get3D_Point_Triangle_ClosestPoint()
    {
        // 01: origin point and triangle around origin
        {
            glm::vec3 closestPoint;
            Get3D_Point_Triangle_ClosestPoint(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(-2.0f, -3.0f, 0.0f),
                glm::vec3(3.0f, 2.0f, 0.0f),
                glm::vec3(-2.0f, 2.0f, 0.0f),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 0.0f) &&
                FloatEqualToFloat(closestPoint.y, 0.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 1;
            }
        }

        // 02: point and triangle
        {
            glm::vec3 closestPoint;
            Get3D_Point_Triangle_ClosestPoint(
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(-2.0f, -3.0f, -1.0f),
                glm::vec3(3.0f, 2.0f, -1.0f),
                glm::vec3(-2.0f, 2.0f, -1.0f),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 1.0f) &&
                FloatEqualToFloat(closestPoint.y, 1.0f) &&
                FloatEqualToFloat(closestPoint.z, -1.0f)))
            {
                return 2;
            }
        }

        // 03: point and triangle
        {
            glm::vec3 closestPoint;
            Get3D_Point_Triangle_ClosestPoint(
                glm::vec3(-2.0f, -3.0f, 1.0f),
                glm::vec3(-2.0f, -3.0f, -1.0f),
                glm::vec3(3.0f, 2.0f, -1.0f),
                glm::vec3(-2.0f, 2.0f, -1.0f),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, -2.0f) &&
                FloatEqualToFloat(closestPoint.y, -3.0f) &&
                FloatEqualToFloat(closestPoint.z, -1.0f)))
            {
                return 3;
            }
        }

        // 04: point and triangle
        {
            glm::vec3 closestPoint;
            Get3D_Point_Triangle_ClosestPoint(
                glm::vec3(-3.0f, -4.0f, 1.0f),
                glm::vec3(-2.0f, -3.0f, -1.0f),
                glm::vec3(3.0f, 2.0f, -1.0f),
                glm::vec3(-2.0f, 2.0f, -1.0f),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, -2.0f) &&
                FloatEqualToFloat(closestPoint.y, -3.0f) &&
                FloatEqualToFloat(closestPoint.z, -1.0f)))
            {
                return 4;
            }
        }

        // 05: point and triangle
        {
            glm::vec3 closestPoint;
            Get3D_Point_Triangle_ClosestPoint(
                glm::vec3(0.0f, 2.0f, 1.0f),
                glm::vec3(-2.0f, -3.0f, -1.0f),
                glm::vec3(3.0f, 2.0f, -1.0f),
                glm::vec3(-2.0f, 2.0f, -1.0f),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 0.0f) &&
                FloatEqualToFloat(closestPoint.y, 2.0f) &&
                FloatEqualToFloat(closestPoint.z, -1.0f)))
            {
                return 5;
            }
        }

        // 06: point and triangle
        {
            glm::vec3 closestPoint;
            Get3D_Point_Triangle_ClosestPoint(
                glm::vec3(0.0f, 3.0f, 1.0f),
                glm::vec3(-2.0f, -3.0f, -1.0f),
                glm::vec3(3.0f, 2.0f, -1.0f),
                glm::vec3(-2.0f, 2.0f, -1.0f),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 0.0f) &&
                FloatEqualToFloat(closestPoint.y, 2.0f) &&
                FloatEqualToFloat(closestPoint.z, -1.0f)))
            {
                return 6;
            }
        }

        // 07: point and infinitly small triangle
        {
            glm::vec3 closestPoint;
            Get3D_Point_Triangle_ClosestPoint(
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 0.0f) &&
                FloatEqualToFloat(closestPoint.y, 0.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 7;
            }
        }

        return 0;
    }

    inline int UnitTest_Get3D_Point_AABB_ClosestPoint()
    {
        // 01: origin point and AABB around origin
        {
            glm::vec3 closestPoint;
            Get3D_Point_AABB_ClosestPoint(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 0.0f) &&
                FloatEqualToFloat(closestPoint.y, 0.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 1;
            }
        }

        // 02: point in AABB
        {
            glm::vec3 closestPoint;
            Get3D_Point_AABB_ClosestPoint(
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 1.0f) &&
                FloatEqualToFloat(closestPoint.y, 1.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 2;
            }
        }

        // 03: point out of AABB
        {
            glm::vec3 closestPoint;
            Get3D_Point_AABB_ClosestPoint(
                glm::vec3(2.0f, 2.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 1.0f) &&
                FloatEqualToFloat(closestPoint.y, 1.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 3;
            }
        }

        return 0;
    }

    inline int UnitTest_Get3D_Point_OBB_ClosestPoint()
    {
        // 01: origin point and OBB around origin
        {
            glm::vec3 closestPoint;
            Get3D_Point_OBB_ClosestPoint(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 0.0f) &&
                FloatEqualToFloat(closestPoint.y, 0.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 1;
            }
        }

        // 02: point in rotated OBB
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));

            glm::vec3 closestPoint;
            Get3D_Point_OBB_ClosestPoint(
                glm::vec3(0.0f, 2.0f, 0.0f),
                glm::vec3(1.0f, 3.0f, 1.0f),
                glm::vec3(2.0f, 0.0f, 0.0f),
                rotation,
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 0.0f) &&
                FloatEqualToFloat(closestPoint.y, 2.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 2;
            }
        }

        // 03: point on rotated OBB edge
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));

            glm::vec3 closestPoint;
            Get3D_Point_OBB_ClosestPoint(
                glm::vec3(2.0f - 3.0f / glm::sqrt(2.0f), 3.0f / glm::sqrt(2.0f), 1.0f),
                glm::vec3(1.0f, 3.0f, 1.0f),
                glm::vec3(2.0f, 0.0f, 0.0f),
                rotation,
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 2.0f - 3.0f / glm::sqrt(2.0f)) &&
                FloatEqualToFloat(closestPoint.y, 3.0f / glm::sqrt(2.0f)) &&
                FloatEqualToFloat(closestPoint.z, 1.0f)))
            {
                return 3;
            }
        }

        // 04: point on rotated OBB corner
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));

            glm::vec3 closestPoint;
            Get3D_Point_OBB_ClosestPoint(
                glm::vec3(2.0f - 2.0f / glm::sqrt(2.0f), 4.0f / glm::sqrt(2.0f), 1.0f),
                glm::vec3(1.0f, 3.0f, 1.0f),
                glm::vec3(2.0f, 0.0f, 0.0f),
                rotation,
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 2.0f - 2.0f / glm::sqrt(2.0f)) &&
                FloatEqualToFloat(closestPoint.y, 4.0f / glm::sqrt(2.0f)) &&
                FloatEqualToFloat(closestPoint.z, 1.0f)))
            {
                return 4;
            }
        }

        // 05: point near rotated OBB corner
        {
            glm::quat rotation = glm::rotate(
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::quarter_pi<float>(),
                glm::vec3(0.0f, 0.0f, 1.0f));

            glm::vec3 closestPoint;
            Get3D_Point_OBB_ClosestPoint(
                glm::vec3(1.0f, 4.0f, 2.0f),
                glm::vec3(1.0f, 3.0f, 1.0f),
                glm::vec3(2.0f, 0.0f, 0.0f),
                rotation,
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 2.0f - 2.0f / glm::sqrt(2.0f)) &&
                FloatEqualToFloat(closestPoint.y, 4.0f / glm::sqrt(2.0f)) &&
                FloatEqualToFloat(closestPoint.z, 1.0f)))
            {
                return 5;
            }
        }

        return 0;
    }

    inline int UnitTest_Get3D_Point_Sphere_ClosestPoint()
    {
        // 01: origin point and sphere at origin
        {
            glm::vec3 closestPoint;
            Get3D_Point_Sphere_ClosestPoint(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                1.0f,
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 0.0f) &&
                FloatEqualToFloat(closestPoint.y, 0.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 1;
            }
        }

        // 02: point and sphere
        {
            glm::vec3 closestPoint;
            Get3D_Point_Sphere_ClosestPoint(
                glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 0.0f, 0.0f),
                1.0f,
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 1.0f) &&
                FloatEqualToFloat(closestPoint.y, 0.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 2;
            }
        }

        // 03: origin point and infinitly small sphere at origin
        {
            glm::vec3 closestPoint;
            Get3D_Point_Sphere_ClosestPoint(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                0.0f,
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 0.0f) &&
                FloatEqualToFloat(closestPoint.y, 0.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 3;
            }
        }

        // 04: point and infinitly small sphere
        {
            glm::vec3 closestPoint;
            Get3D_Point_Sphere_ClosestPoint(
                glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 0.0f, 0.0f),
                0.0f,
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 2.0f) &&
                FloatEqualToFloat(closestPoint.y, 0.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 4;
            }
        }

        // 05: point in infinitly small sphere
        {
            glm::vec3 closestPoint;
            Get3D_Point_Sphere_ClosestPoint(
                glm::vec3(2.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 0.0f, 0.0f),
                0.0f,
                closestPoint);

            if (!(FloatEqualToFloat(closestPoint.x, 2.0f) &&
                FloatEqualToFloat(closestPoint.y, 0.0f) &&
                FloatEqualToFloat(closestPoint.z, 0.0f)))
            {
                return 5;
            }
        }

        return 0;
    }
}