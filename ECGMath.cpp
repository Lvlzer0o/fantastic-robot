#include "ECGMath.h"
#include <cmath>

// --- Heart_vector function implementations ---

Heart_vector add(const Heart_vector& a, const Heart_vector& b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

Heart_vector scale(const Heart_vector& v, float64 k)
{
    return {v.x * k, v.y * k, v.z * k};
}

Heart_vector normalize(const Heart_vector& v)
{
    const float64 mag_squared = (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
    const float64 magnitude = std::sqrt(mag_squared);

    // Rule 151: Avoid magic numbers. Use a named constant for tolerance.
    const float64 zero_tolerance = 1e-9;

    if (magnitude <= zero_tolerance)
    {
        return v;
    }

    const float64 inv_magnitude = 1.0 / magnitude;
    return {v.x * inv_magnitude, v.y * inv_magnitude, v.z * inv_magnitude};
}

float64 dot_product(const Heart_vector& a, const Heart_vector& b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

float64 project_to_lead(const Heart_vector& heart_vector, const Heart_vector& lead_vector)
{
    return dot_product(heart_vector, lead_vector);
}
