#include "ECGMath.h"
#include <cmath>

// --- Heart_vector function implementations ---

Heart_vector add(const Heart_vector &a, const Heart_vector &b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z};
}

Heart_vector scale(const Heart_vector &v, float64 k) {
  return {v.x * k, v.y * k, v.z * k};
}

Heart_vector normalize(const Heart_vector &v) {
  const float64 mag_squared = (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
  const float64 magnitude = std::sqrt(mag_squared);

  // Rule 151: Avoid magic numbers. Use a named constant for tolerance.
  const float64 zero_tolerance = 1e-9;

  if (magnitude <= zero_tolerance) {
    return v;
  }

  const float64 inv_magnitude = 1.0 / magnitude;
  return {v.x * inv_magnitude, v.y * inv_magnitude, v.z * inv_magnitude};
}

float64 dot_product(const Heart_vector &a, const Heart_vector &b) {
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

float64 project_onto_lead(const Heart_vector &heart_vector,
                          const Heart_vector &lead_vector) {
  return dot_product(heart_vector, lead_vector);
}

// --- Standard_leads constant definitions ---

// Helper function to avoid repetition, kept local to this translation unit.
static Heart_vector frontal(float64 degrees) {
  const float64 r = degrees * pi_value / 180.0;
  return normalize({std::cos(r), std::sin(r), 0.0});
}

const Heart_vector Standard_leads::lead_i = frontal(0.0);
const Heart_vector Standard_leads::lead_ii = frontal(60.0);
const Heart_vector Standard_leads::lead_iii = frontal(120.0);
const Heart_vector Standard_leads::lead_avr = frontal(-150.0);
const Heart_vector Standard_leads::lead_avl = frontal(-30.0);
const Heart_vector Standard_leads::lead_avf = frontal(90.0);

const Heart_vector Standard_leads::lead_v1 = normalize({-0.6, 0.0, 0.8});
const Heart_vector Standard_leads::lead_v2 = normalize({-0.3, 0.0, 0.95});
const Heart_vector Standard_leads::lead_v3 = normalize({0.0, 0.0, 1.0});
const Heart_vector Standard_leads::lead_v4 = normalize({0.3, 0.0, 0.95});
const Heart_vector Standard_leads::lead_v5 = normalize({0.6, 0.0, 0.8});
const Heart_vector Standard_leads::lead_v6 = normalize({0.9, 0.0, 0.4});