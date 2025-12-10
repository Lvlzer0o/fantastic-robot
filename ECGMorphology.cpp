#include "ECGMorphology.h"
#include <cmath>

// --- Constants for Morphology (Rule 151) ---
// P-Wave
const float64 p_wave_duration_s = 0.10;
const float64 p_wave_axis_degrees = 60.0;
const float64 p_wave_amplitude_scale = 0.15;

// T-Wave
const float64 t_wave_duration_s = 0.20;
const float64 t_wave_axis_offset_degrees = 20.0;
const float64 t_wave_amplitude_scale = 0.35;

// QRS Complex
const float64 qrs_amplitude_scale = 1.2;

// Intervals
const float64 st_segment_gap_s = 0.04;

// --- Function Implementations ---

Heart_vector calculate_component_vector(const Ecg_component* component, float64 time)
{
    if (!component->is_active)
    {
        return {0.0, 0.0, 0.0};
    }

    const float64 local_time = time - component->start_time_s;

    if (local_time < 0.0 || local_time > component->duration_s)
    {
        return {0.0, 0.0, 0.0};
    }

    const float64 zero_tolerance = 1e-9;
    if (component->duration_s <= zero_tolerance)
    {
        return {0.0, 0.0, 0.0};
    }

    const float64 u = local_time / component->duration_s;
    const float64 diff = (u - component->shape_params.center) / component->shape_params.width;
    const float64 mag = std::exp(-(diff * diff));

    return scale(component->shape_params.direction, component->shape_params.scale * mag);
}

Heart_vector calculate_heart_vector(const Ecg_morphology* morphology, float64 local_time)
{
    Heart_vector v = {0.0, 0.0, 0.0};
    v = add(v, calculate_component_vector(&morphology->p_wave, local_time));
    v = add(v, calculate_component_vector(&morphology->qrs_complex, local_time));
    v = add(v, calculate_component_vector(&morphology->t_wave, local_time));
    return v;
}

// Helper for creating a component, local to this file.
static Ecg_component make_directional_component(
    float64 start,
    float64 duration,
    float64 axis_degrees,
    float64 scale_val,
    float64 center = 0.5,
    float64 width = 0.25)
{
    const float64 r = axis_degrees * pi_value / 180.0;
    const Heart_vector dir = normalize({std::cos(r), std::sin(r), 0.0});

    Ecg_component comp;
    comp.start_time_s = start;
    comp.duration_s = duration;
    comp.is_active = true;
    comp.shape_params.direction = dir;
    comp.shape_params.scale = scale_val;
    comp.shape_params.center = center;
    comp.shape_params.width = width;
    return comp;
}

Ecg_morphology create_normal_sinus_morphology(
    float64 pr_interval,
    float64 qrs_duration,
    float64 qrs_axis_degrees)
{
    const float64 p_start = 0.0;
    const float64 qrs_start = pr_interval;
    const float64 t_start = qrs_start + qrs_duration + st_segment_gap_s;

    Ecg_morphology morph;
    morph.p_wave = make_directional_component(p_start, p_wave_duration_s, p_wave_axis_degrees, p_wave_amplitude_scale);
    morph.qrs_complex = make_directional_component(qrs_start, qrs_duration, qrs_axis_degrees, qrs_amplitude_scale);
    morph.t_wave = make_directional_component(t_start, t_wave_duration_s, qrs_axis_degrees + t_wave_axis_offset_degrees, t_wave_amplitude_scale);

    return morph;
}