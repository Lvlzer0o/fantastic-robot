#ifndef ECG_MATH_HPP
#define ECG_MATH_HPP

#include <cmath>
#include <algorithm>

// Standard PI fallback for C++17 compatibility
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// MARK: - Cardiac Vector
// A simple 3D vector representing the heart's electrical axis.
// Designed for stack allocation and deterministic math.
struct HeartVector {
    double x{0.0};
    double y{0.0};
    double z{0.0};

    // Static constant for zero
    static constexpr HeartVector zero() { return {0.0, 0.0, 0.0}; }

    // Dot product
    [[nodiscard]] constexpr double dot(const HeartVector& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Scalar multiplication
    [[nodiscard]] constexpr HeartVector scaled(double k) const {
        return {x * k, y * k, z * k};
    }

    // Normalization (Safe: handles zero magnitude)
    [[nodiscard]] HeartVector normalized() const {
        const double mag = std::sqrt(x * x + y * y + z * z);
        if (mag <= 1e-9) { return *this; }
        return {x / mag, y / mag, z / mag};
    }
};

// Operator overloading for vector addition
[[nodiscard]] constexpr HeartVector operator+(const HeartVector& a, const HeartVector& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

// MARK: - Standard ECG Leads
// Defines the viewing angle for the standard 12 leads.
struct StandardLeads {
    // Helper: Calculates a unit vector in the frontal plane
    static HeartVector frontal(double degrees) {
        const double r = degrees * M_PI / 180.0;
        return HeartVector{std::cos(r), std::sin(r), 0.0}.normalized();
    }

    // Limb Leads (Hexaxial Reference System)
    inline static const HeartVector I   = frontal(0.0);
    inline static const HeartVector II  = frontal(60.0);
    inline static const HeartVector III = frontal(120.0);
    inline static const HeartVector aVR = frontal(-150.0);
    inline static const HeartVector aVL = frontal(-30.0);
    inline static const HeartVector aVF = frontal(90.0);

    // Precordial (Chest) Leads
    // +Z is Anterior, +X is Left Lateral
    inline static const HeartVector V1 = HeartVector{-0.6, 0.0,  0.8}.normalized();
    inline static const HeartVector V2 = HeartVector{-0.3, 0.0,  0.95}.normalized();
    inline static const HeartVector V3 = HeartVector{ 0.0, 0.0,  1.0}.normalized();
    inline static const HeartVector V4 = HeartVector{ 0.3, 0.0,  0.95}.normalized();
    inline static const HeartVector V5 = HeartVector{ 0.6, 0.0,  0.8}.normalized();
    inline static const HeartVector V6 = HeartVector{ 0.9, 0.0,  0.4}.normalized();
};

#endif // ECG_MATH_HPP