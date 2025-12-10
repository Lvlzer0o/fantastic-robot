#ifndef ECG_MORPHOLOGY_HPP
#define ECG_MORPHOLOGY_HPP

#include "ECGMath.hpp"
#include <cmath>

// MARK: - Shape Configuration
// Parameters defining a specific wave (e.g., P-wave)
// Uses POD (Plain Old Data) for zero-allocation usage.
struct GaussianShapeParams {
    HeartVector direction{0,0,0}; // The 3D axis of the wave
    double scale{0.0};            // Amplitude
    double center{0.5};           // Normalized time center (0.0 to 1.0)
    double width{0.25};           // Width of the gaussian curve
};

// MARK: - ECG Component
// Represents a single segment (P, QRS, or T)
struct ECGComponent {
    double start{0.0};    // Start time in seconds (relative to beat start)
    double duration{0.0}; // Duration in seconds
    bool active{false};   // Is this component used?

    GaussianShapeParams shapeParams;

    // Calculates the vector at a specific time
    [[nodiscard]] HeartVector vector(double time) const {
        if (!active) return HeartVector::zero();

        const double local = time - start;

        // Bounds check
        if (local < 0.0 || local > duration) {
            return HeartVector::zero();
        }

        // Safety check for divide-by-zero
        if (duration <= 1e-9) return HeartVector::zero();

        const double u = local / duration;

        // Gaussian Formula: exp( -pow( (u - center) / width, 2 ) )
        const double diff = (u - shapeParams.center) / shapeParams.width;
        const double mag = std::exp(- (diff * diff));

        return shapeParams.direction.scaled(shapeParams.scale * mag);
    }
};

// MARK: - ECG Morphology
// A container for a complete heart beat
struct ECGMorphology {
    ECGComponent pWave;
    ECGComponent qrs;
    ECGComponent tWave;

    // Sums the components to get the total heart vector at a moment in time
    [[nodiscard]] HeartVector heartVector(double localTime) const {
        HeartVector v = HeartVector::zero();
        v = v + pWave.vector(localTime);
        v = v + qrs.vector(localTime);
        v = v + tWave.vector(localTime);
        return v;
    }
};

// MARK: - Factory Functions

// Helper to create a component easily
[[nodiscard]] inline ECGComponent makeDirectionalComponent(
    double start,
    double duration,
    double axisDegrees,
    double scale,
    double center = 0.5,
    double width = 0.25
) {
    const double r = axisDegrees * M_PI / 180.0;
    // We create a vector in the frontal plane for simplicity
    const HeartVector dir = HeartVector{std::cos(r), std::sin(r), 0.0}.normalized();

    ECGComponent comp;
    comp.start = start;
    comp.duration = duration;
    comp.active = true;
    comp.shapeParams.direction = dir;
    comp.shapeParams.scale = scale;
    comp.shapeParams.center = center;
    comp.shapeParams.width = width;
    return comp;
}

// Creates a standard Sinus Rhythm morphology
[[nodiscard]] inline ECGMorphology normalSinusMorphology(
    double prInterval = 0.16,
    double qrsDuration = 0.10,
    double qtInterval = 0.40,
    double qrsAxisDegrees = 60.0
) {
    // Timing logic
    const double pStart = 0.0;
    const double qrsStart = prInterval; // QRS starts after PR interval
    const double tStart = qrsStart + qrsDuration + 0.04; // ST segment gap

    return ECGMorphology{
        // P-Wave: 60 degrees axis, small amplitude
        .pWave = makeDirectionalComponent(pStart, 0.10, 60.0, 0.15),

        // QRS: User-defined axis, large amplitude
        .qrs = makeDirectionalComponent(qrsStart, qrsDuration, qrsAxisDegrees, 1.2),

        // T-Wave: Concordant (similar axis to QRS), medium amplitude
        .tWave = makeDirectionalComponent(tStart, 0.20, qrsAxisDegrees + 20.0, 0.35)
    };
}

#endif // ECG_MORPHOLOGY_HPP