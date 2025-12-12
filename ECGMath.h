#ifndef ECG_MATH_H
#define ECG_MATH_H

#include <cmath>
#include "Types.h"

// Rule 30: The #define pre-processor directive shall not be used to define constant values.
// Rule 52: The name of a constant will be composed of lowercase letters.
const float64 pi_value = 3.14159265358979323846;

// Rule 50: The first word of the name of a class/struct will begin with an uppercase letter.
// Rule 45: All words in an identifier will be separated by the '_' character.
struct Heart_vector
{
    // Rule 209: Use of specific-length types.
    float64 x;
    float64 y;
    float64 z;
};

// Compile-time helpers to support deterministic, normalized lead definitions.
constexpr float64 square_value(float64 v)
{
    return v * v;
}

constexpr float64 sqrt_newton(float64 x, float64 current, float64 previous)
{
    return (current == previous) ? current : sqrt_newton(x, 0.5 * (current + (x / current)), current);
}

constexpr float64 sqrt_constexpr(float64 x)
{
    return (x <= 0.0) ? 0.0 : sqrt_newton(x, (x > 1.0) ? x : 1.0, 0.0);
}

constexpr Heart_vector normalize_constexpr(float64 x, float64 y, float64 z)
{
    const float64 magnitude = sqrt_constexpr(square_value(x) + square_value(y) + square_value(z));
    return (magnitude <= 0.0)
        ? Heart_vector{0.0, 0.0, 0.0}
        : Heart_vector{x / magnitude, y / magnitude, z / magnitude};
}

// Rule 51: Function names will be composed entirely of lowercase letters.
Heart_vector add(const Heart_vector& a, const Heart_vector& b);
Heart_vector scale(const Heart_vector& v, float64 k);
Heart_vector normalize(const Heart_vector& v);
float64 dot_product(const Heart_vector& a, const Heart_vector& b);
float64 project_to_lead(const Heart_vector& heart_vector, const Heart_vector& lead_vector);

// Defines the viewing angle for the standard 12 leads.
struct Standard_leads
{
    // Rule 52: Constant names are lowercase.
    // Rule 45: Underscore separation.
    inline static constexpr Heart_vector lead_i   = normalize_constexpr(1.0, 0.0, 0.0);
    inline static constexpr Heart_vector lead_ii  = normalize_constexpr(0.5, 0.86602540378443864676, 0.0);
    inline static constexpr Heart_vector lead_iii = normalize_constexpr(-0.5, 0.86602540378443864676, 0.0);
    inline static constexpr Heart_vector lead_avr = normalize_constexpr(-0.86602540378443864676, -0.5, 0.0);
    inline static constexpr Heart_vector lead_avl = normalize_constexpr(0.86602540378443864676, -0.5, 0.0);
    inline static constexpr Heart_vector lead_avf = normalize_constexpr(0.0, 1.0, 0.0);
    inline static constexpr Heart_vector lead_v1 = normalize_constexpr(-0.6, 0.0,  0.8);
    inline static constexpr Heart_vector lead_v2 = normalize_constexpr(-0.3, 0.0,  0.95);
    inline static constexpr Heart_vector lead_v3 = normalize_constexpr(0.0, 0.0,  1.0);
    inline static constexpr Heart_vector lead_v4 = normalize_constexpr(0.3, 0.0,  0.95);
    inline static constexpr Heart_vector lead_v5 = normalize_constexpr(0.6, 0.0,  0.8);
    inline static constexpr Heart_vector lead_v6 = normalize_constexpr(0.9, 0.0,  0.4);
};

#endif // ECG_MATH_H
