#include <cmath>
#include <fstream>
#include <iomanip>
#include "ECGMorphology.h"

int main()
{
    // Rule 151: Use named constants for parameters
    const float64 pr_interval_val = 0.16;
    const float64 qrs_duration_val = 0.10;
    const float64 qrs_axis_val = 60.0;
    const float64 sampling_rate_hz = 500.0;
    const float64 heart_rate_bpm = 72.0;

    // Create morphology struct
    const Ecg_morphology normal_rhythm = create_normal_sinus_morphology(
        pr_interval_val,
        qrs_duration_val,
        qrs_axis_val
    );

    const float64 cycle_duration_s = 60.0 / heart_rate_bpm;
    const float64 dt = 1.0 / sampling_rate_hz;
    const int32 total_samples = static_cast<int32>(cycle_duration_s * sampling_rate_hz);

    std::ofstream csv_output("ecg.csv");
    csv_output << "time,lead_I,lead_II,lead_III,aVR,aVL,aVF,V1,V2,V3,V4,V5,V6\n";
    csv_output << std::fixed << std::setprecision(6);

    for (int32 sample_index = 0; sample_index <= total_samples; ++sample_index)
    {
        const float64 time_s = static_cast<float64>(sample_index) * dt;
        const float64 local_time = std::fmod(time_s, cycle_duration_s);
        const Heart_vector heart_vector = calculate_heart_vector(&normal_rhythm, local_time);

        const float64 lead_i = project_to_lead(heart_vector, Standard_leads::lead_i);
        const float64 lead_ii = project_to_lead(heart_vector, Standard_leads::lead_ii);
        const float64 lead_iii = project_to_lead(heart_vector, Standard_leads::lead_iii);
        const float64 lead_avr = project_to_lead(heart_vector, Standard_leads::lead_avr);
        const float64 lead_avl = project_to_lead(heart_vector, Standard_leads::lead_avl);
        const float64 lead_avf = project_to_lead(heart_vector, Standard_leads::lead_avf);
        const float64 lead_v1 = project_to_lead(heart_vector, Standard_leads::lead_v1);
        const float64 lead_v2 = project_to_lead(heart_vector, Standard_leads::lead_v2);
        const float64 lead_v3 = project_to_lead(heart_vector, Standard_leads::lead_v3);
        const float64 lead_v4 = project_to_lead(heart_vector, Standard_leads::lead_v4);
        const float64 lead_v5 = project_to_lead(heart_vector, Standard_leads::lead_v5);
        const float64 lead_v6 = project_to_lead(heart_vector, Standard_leads::lead_v6);

        csv_output
            << time_s << ','
            << lead_i << ','
            << lead_ii << ','
            << lead_iii << ','
            << lead_avr << ','
            << lead_avl << ','
            << lead_avf << ','
            << lead_v1 << ','
            << lead_v2 << ','
            << lead_v3 << ','
            << lead_v4 << ','
            << lead_v5 << ','
            << lead_v6 << '\n';
    }

    return 0;
}
