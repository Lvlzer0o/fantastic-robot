#ifndef ECG_MORPHOLOGY_H
#define ECG_MORPHOLOGY_H

#include "ECGMath.h"
#include "Types.h"

// Rule 50, 45: Struct naming convention
struct Gaussian_shape_params
{
    Heart_vector direction;
    float64 scale;
    float64 center;
    float64 width;
};

struct Ecg_component
{
    float64 start_time_s;
    float64 duration_s;
    bool is_active;
    Gaussian_shape_params shape_params;
};

// Rule 51: Function names are lowercase
Heart_vector calculate_component_vector(const Ecg_component* component, float64 time);

struct Ecg_morphology
{
    Ecg_component p_wave;
    Ecg_component qrs_complex;
    Ecg_component t_wave;
};

Heart_vector calculate_heart_vector(const Ecg_morphology* morphology, float64 local_time);

// Factory functions
Ecg_morphology create_normal_sinus_morphology(
    float64 pr_interval,
    float64 qrs_duration,
    float64 qrs_axis_degrees
);

#endif // ECG_MORPHOLOGY_H