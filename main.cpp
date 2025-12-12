#include <cmath>
#include <fstream>
#include <iomanip>
#include <vector>
#include "ECGMorphology.h"
#include "ECGSimulation.h"

int main()
{
    // Rule 151: Use named constants for parameters
    const float64 pr_interval_val = 0.16;
    const float64 qrs_duration_val = 0.10;
    const float64 qrs_axis_val = 60.0;
    const float64 sampling_rate_hz = 500.0;
    const float64 heart_rate_bpm = 72.0;
    const float64 seconds_per_minute = 60.0;

    // Create morphology struct
    const Ecg_morphology normal_rhythm = create_normal_sinus_morphology(
        pr_interval_val,
        qrs_duration_val,
        qrs_axis_val
    );

    const float64 cycle_duration_s = seconds_per_minute / heart_rate_bpm;
    const std::vector<Lead_sample> samples = generate_ecg_timeseries(
        normal_rhythm,
        heart_rate_bpm,
        sampling_rate_hz,
        cycle_duration_s
    );

    std::ofstream csv_output("ecg.csv");
    csv_output << "time,lead_I,lead_II,lead_III,aVR,aVL,aVF,V1,V2,V3,V4,V5,V6\n";
    csv_output << std::fixed << std::setprecision(6);

    for (const auto& sample : samples)
    {
        csv_output
            << sample.time_s << ','
            << sample.leads[lead_i_index] << ','
            << sample.leads[lead_ii_index] << ','
            << sample.leads[lead_iii_index] << ','
            << sample.leads[lead_avr_index] << ','
            << sample.leads[lead_avl_index] << ','
            << sample.leads[lead_avf_index] << ','
            << sample.leads[lead_v1_index] << ','
            << sample.leads[lead_v2_index] << ','
            << sample.leads[lead_v3_index] << ','
            << sample.leads[lead_v4_index] << ','
            << sample.leads[lead_v5_index] << ','
            << sample.leads[lead_v6_index] << '\n';
    }

    return 0;
}
