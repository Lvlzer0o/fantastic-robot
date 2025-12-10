#include <iostream>
#include "ECGMorphology.h"

int main() {
    // Rule 151: Use named constants for parameters
    const float64 pr_interval_val = 0.16;
    const float64 qrs_duration_val = 0.10;
    const float64 qrs_axis_val = 60.0;
    const float64 time_point_s = 0.1;

    // Create morphology struct
    Ecg_morphology normal_rhythm = create_normal_sinus_morphology(
        pr_interval_val,
        qrs_duration_val,
        qrs_axis_val
    );

    // Calculate the heart vector at a specific time
    Heart_vector vector_at_time = calculate_heart_vector(&normal_rhythm, time_point_s);

    // Note: JSF does not allow std::cout. This is for demonstration.
    // A compliant implementation would use a dedicated logging/output mechanism.
    std::cout << "Heart vector at " << time_point_s << "s: ("
              << vector_at_time.x << ", "
              << vector_at_time.y << ", "
              << vector_at_time.z << ")" << std::endl;

    return 0;
}