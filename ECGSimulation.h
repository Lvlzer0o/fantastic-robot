#ifndef ECG_SIMULATION_H
#define ECG_SIMULATION_H

#include "ECGMorphology.h"
#include "NoiseGenerator.h"
#include <array>
#include <memory>
#include <vector>


enum Lead_index : std::size_t {
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

struct Lead_sample {
  float64 time_s;
  std::array<float64, 12> leads;
};

class ECGSimulationEngine {
public:
  ECGSimulationEngine(const Ecg_morphology &morphology, float64 heart_rate_bpm,
                      float64 sampling_rate_hz);

  // Add a noise source to the simulation
  void add_noise_source(std::shared_ptr<SignalGenerator> noise);

  // Generate samples for a given duration
  std::vector<Lead_sample> generate(float64 duration_seconds);

private:
  Ecg_morphology morphology_;
  float64 heart_rate_bpm_;
  float64 sampling_rate_hz_;
  double current_time_s_{0.0};

  std::vector<std::shared_ptr<SignalGenerator>> noise_sources_;

  // Helper to calculate one sample at absolute time t
  Lead_sample calculate_sample(float64 t);
};

// Legacy support for existing tests (wraps the engine)
std::vector<Lead_sample>
generate_ecg_timeseries(const Ecg_morphology &morphology,
                        float64 heart_rate_bpm, float64 sampling_rate_hz,
                        float64 duration_seconds);

#endif // ECG_SIMULATION_H
