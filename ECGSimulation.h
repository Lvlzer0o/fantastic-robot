#ifndef ECG_SIMULATION_H
#define ECG_SIMULATION_H

#include <array>
#include <vector>
#include "ECGMorphology.h"

enum Lead_index : std::size_t
{
    lead_i_index = 0,
    lead_ii_index,
    lead_iii_index,
    lead_avr_index,
    lead_avl_index,
    lead_avf_index,
    lead_v1_index,
    lead_v2_index,
    lead_v3_index,
    lead_v4_index,
    lead_v5_index,
    lead_v6_index
};

struct Lead_sample
{
    float64 time_s;
    std::array<float64, 12> leads;
};

std::vector<Lead_sample> generate_ecg_timeseries(
    const Ecg_morphology& morphology,
    float64 heart_rate_bpm,
    float64 sampling_rate_hz,
    float64 duration_seconds);

#endif // ECG_SIMULATION_H
