#include "ECGSimulation.h"

#include <cmath>

namespace
{
    constexpr float64 seconds_per_minute = 60.0;
    constexpr float64 zero_tolerance = 1e-9;
}

std::vector<Lead_sample> generate_ecg_timeseries(
    const Ecg_morphology& morphology,
    float64 heart_rate_bpm,
    float64 sampling_rate_hz,
    float64 duration_seconds)
{
    if (heart_rate_bpm <= zero_tolerance || sampling_rate_hz <= zero_tolerance || duration_seconds <= zero_tolerance)
    {
        return {};
    }

    const float64 cycle_duration_s = seconds_per_minute / heart_rate_bpm;
    const float64 dt = 1.0 / sampling_rate_hz;
    const int32 total_samples = static_cast<int32>(duration_seconds * sampling_rate_hz);

    std::vector<Lead_sample> samples;
    samples.reserve(static_cast<std::size_t>(total_samples) + 1U);

    for (int32 sample_index = 0; sample_index <= total_samples; ++sample_index)
    {
        const float64 time_s = static_cast<float64>(sample_index) * dt;
        const float64 local_time = std::fmod(time_s, cycle_duration_s);
        const Heart_vector heart_vector = calculate_heart_vector(&morphology, local_time);

        Lead_sample sample{};
        sample.time_s = time_s;
        sample.leads[lead_i_index] = project_to_lead(heart_vector, Standard_leads::lead_i);
        sample.leads[lead_ii_index] = project_to_lead(heart_vector, Standard_leads::lead_ii);
        sample.leads[lead_iii_index] = project_to_lead(heart_vector, Standard_leads::lead_iii);
        sample.leads[lead_avr_index] = project_to_lead(heart_vector, Standard_leads::lead_avr);
        sample.leads[lead_avl_index] = project_to_lead(heart_vector, Standard_leads::lead_avl);
        sample.leads[lead_avf_index] = project_to_lead(heart_vector, Standard_leads::lead_avf);
        sample.leads[lead_v1_index] = project_to_lead(heart_vector, Standard_leads::lead_v1);
        sample.leads[lead_v2_index] = project_to_lead(heart_vector, Standard_leads::lead_v2);
        sample.leads[lead_v3_index] = project_to_lead(heart_vector, Standard_leads::lead_v3);
        sample.leads[lead_v4_index] = project_to_lead(heart_vector, Standard_leads::lead_v4);
        sample.leads[lead_v5_index] = project_to_lead(heart_vector, Standard_leads::lead_v5);
        sample.leads[lead_v6_index] = project_to_lead(heart_vector, Standard_leads::lead_v6);

        samples.push_back(sample);
    }

    return samples;
}
