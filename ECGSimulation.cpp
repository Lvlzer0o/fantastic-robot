#include "ECGSimulation.h"

#include <cmath>

namespace {
constexpr float64 seconds_per_minute = 60.0;
constexpr float64 zero_tolerance = 1e-9;
} // namespace

ECGSimulationEngine::ECGSimulationEngine(const Ecg_morphology &morphology,
                                         float64 heart_rate_bpm,
                                         float64 sampling_rate_hz)
    : morphology_(morphology), heart_rate_bpm_(heart_rate_bpm),
      sampling_rate_hz_(sampling_rate_hz) {}

void ECGSimulationEngine::add_noise_source(
    std::shared_ptr<SignalGenerator> noise) {
  noise_sources_.push_back(noise);
}

std::vector<Lead_sample>
ECGSimulationEngine::generate(float64 duration_seconds) {
  if (heart_rate_bpm_ <= zero_tolerance ||
      sampling_rate_hz_ <= zero_tolerance ||
      duration_seconds <= zero_tolerance) {
    return {};
  }

  const float64 dt = 1.0 / sampling_rate_hz_;
  const int32 total_samples =
      static_cast<int32>(duration_seconds * sampling_rate_hz_);

  std::vector<Lead_sample> samples;
  samples.reserve(static_cast<std::size_t>(total_samples) + 1U);

  for (int32 sample_index = 0; sample_index <= total_samples; ++sample_index) {
    current_time_s_ = static_cast<float64>(sample_index) * dt;
    samples.push_back(calculate_sample(current_time_s_));
  }

  return samples;
}

Lead_sample ECGSimulationEngine::calculate_sample(float64 t) {
  const float64 cycle_duration_s = seconds_per_minute / heart_rate_bpm_;
  const float64 local_time = std::fmod(t, cycle_duration_s);
  const Heart_vector heart_vector =
      calculate_heart_vector(&morphology_, local_time);

  Lead_sample sample{};
  sample.time_s = t;

  // Project to leads
  // array of pointers to member/static vectors would be cleaner, but existing
  // struct is fine
  sample.leads[lead_i_index] =
      project_to_lead(heart_vector, Standard_leads::lead_i);
  sample.leads[lead_ii_index] =
      project_to_lead(heart_vector, Standard_leads::lead_ii);
  sample.leads[lead_iii_index] =
      project_to_lead(heart_vector, Standard_leads::lead_iii);
  sample.leads[lead_avr_index] =
      project_to_lead(heart_vector, Standard_leads::lead_avr);
  sample.leads[lead_avl_index] =
      project_to_lead(heart_vector, Standard_leads::lead_avl);
  sample.leads[lead_avf_index] =
      project_to_lead(heart_vector, Standard_leads::lead_avf);
  sample.leads[lead_v1_index] =
      project_to_lead(heart_vector, Standard_leads::lead_v1);
  sample.leads[lead_v2_index] =
      project_to_lead(heart_vector, Standard_leads::lead_v2);
  sample.leads[lead_v3_index] =
      project_to_lead(heart_vector, Standard_leads::lead_v3);
  sample.leads[lead_v4_index] =
      project_to_lead(heart_vector, Standard_leads::lead_v4);
  sample.leads[lead_v5_index] =
      project_to_lead(heart_vector, Standard_leads::lead_v5);
  sample.leads[lead_v6_index] =
      project_to_lead(heart_vector, Standard_leads::lead_v6);

  // Apply noise
  for (auto &noise_gen : noise_sources_) {
    // Some noises are characteristic per lead (White), others common (Wander)
    // For now, we apply the generator to each lead.
    // Note: For deterministic noise (Wander), get_value(t) returns same value
    // for all leads (Common Mode). For random noise (White), get_value(t)
    // returns NEW random value for each lead.
    for (double &lead_val : sample.leads) {
      lead_val += noise_gen->get_value(t);
    }
  }

  return sample;
}

std::vector<Lead_sample>
generate_ecg_timeseries(const Ecg_morphology &morphology,
                        float64 heart_rate_bpm, float64 sampling_rate_hz,
                        float64 duration_seconds) {
  ECGSimulationEngine engine(morphology, heart_rate_bpm, sampling_rate_hz);
  return engine.generate(duration_seconds);
}
