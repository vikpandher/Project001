// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-05-04

#pragma once

#include "glm/gtc/quaternion.hpp"



struct TestComponent00
{
    TestComponent00::TestComponent00(int a, int b, int c)
        : a(a)
        , b(b)
        , c(c)
    {}

    ~TestComponent00()
    {
        int x = 0;
        x++;
    }

    int GetSum() const
    {
        return a + b + c;
    }

    int a;
    int b;
    int c;
};

struct TestComponent01
{
    TestComponent01::TestComponent01(float x, float y, float z)
        : x(x)
        , y(y)
        , z(z)
    {}

    float x;
    float y;
    float z;
};

struct TestComponent02
{
    TestComponent02::TestComponent02(unsigned int r, unsigned int g, unsigned int b)
        : r(r)
        , g(g)
        , b(b)
    {}

    unsigned int r;
    unsigned int g;
    unsigned int b;
};