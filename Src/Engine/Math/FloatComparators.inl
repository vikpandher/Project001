


namespace Project001
{
    inline bool FloatEqualToFloat(float a, float b, float epsilon)
    {
        return glm::abs(a - b) < epsilon;
    }

    inline bool FloatGreaterThanOrEqualToFloat(float a, float b, float epsilon)
    {
        return (a + epsilon) >= b;
    }

    inline bool FloatLessThanOrEqualToFloat(float a, float b, float epsilon)
    {
        return (a - epsilon) <= b;
    }

    // Unused
    inline float GetMax(float a, float b)
    {
        if (a > b)
        {
            return a;
        }
        else
        {
            return b;
        }
    }

    // Unused
    inline float GetMin(float a, float b)
    {
        if (a < b)
        {
            return a;
        }
        else
        {
            return b;
        }
    }

    inline void GetMinMax(float a, float b, float& min, float& max)
    {
        if (a > b)
        {
            min = b;
            max = a;
        }
        else
        {
            min = a;
            max = b;
        }
    }

    inline void Swap(float& a, float& b)
    {
        float c = a;
        a = b;
        b = c;
    }
}