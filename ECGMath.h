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

// Rule 51: Function names will be composed entirely of lowercase letters.
Heart_vector add(const Heart_vector& a, const Heart_vector& b);
Heart_vector scale(const Heart_vector& v, float64 k);
Heart_vector normalize(const Heart_vector& v);
float64 dot_product(const Heart_vector& a, const Heart_vector& b);

// Defines the viewing angle for the standard 12 leads.
struct Standard_leads
{
    // Rule 52: Constant names are lowercase.
    // Rule 45: Underscore separation.
    static const Heart_vector lead_i;
    static const Heart_vector lead_ii;
    static const Heart_vector lead_iii;
    static const Heart_vector lead_avr;
    static const Heart_vector lead_avl;
    static const Heart_vector lead_avf;
    static const Heart_vector lead_v1;
    static const Heart_vector lead_v2;
    static const Heart_vector lead_v3;
    static const Heart_vector lead_v4;
    static const Heart_vector lead_v5;
    static const Heart_vector lead_v6;
};

#endif // ECG_MATH_H