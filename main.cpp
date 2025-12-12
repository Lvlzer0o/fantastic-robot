#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>


#include "ECGMorphology.h"
#include "ECGSimulation.h"
#include "NoiseGenerator.h"

void print_usage(const char *prog_name) {
  std::cout
      << "Usage: " << prog_name << " [options]\n"
      << "Options:\n"
      << "  --hr <bpm>        Heart rate in BPM (default: 72.0)\n"
      << "  --duration <sec>  Duration in seconds (default: 10.0)\n"
      << "  --rate <hz>       Sampling rate in Hz (default: 500.0)\n"
      << "  --noise <amp>     Add white noise with amplitude (default: 0.0)\n"
      << "  --wander <amp>    Add baseline wander with amplitude (default: "
         "0.0)\n"
      << "  --mains <amp>     Add 60Hz mains hum with amplitude (default: "
         "0.0)\n"
      << "  --out <file>      Output CSV file (default: ecg.csv)\n"
      << "  --help            Show this help\n";
}

int main(int argc, char *argv[]) {
  // Defaults
  float64 heart_rate_bpm = 72.0;
  float64 duration_seconds = 10.0;
  float64 sampling_rate_hz = 500.0;
  float64 white_noise_amp = 0.0;
  float64 wander_amp = 0.0;
  float64 mains_amp = 0.0;
  std::string output_file = "ecg.csv";

  // Parse arguments
  for (int i = 1; i < argc; ++i) {
    if (std::strcmp(argv[i], "--help") == 0) {
      print_usage(argv[0]);
      return 0;
    } else if (std::strcmp(argv[i], "--hr") == 0 && i + 1 < argc) {
      heart_rate_bpm = std::stod(argv[++i]);
    } else if (std::strcmp(argv[i], "--duration") == 0 && i + 1 < argc) {
      duration_seconds = std::stod(argv[++i]);
    } else if (std::strcmp(argv[i], "--rate") == 0 && i + 1 < argc) {
      sampling_rate_hz = std::stod(argv[++i]);
    } else if (std::strcmp(argv[i], "--noise") == 0 && i + 1 < argc) {
      white_noise_amp = std::stod(argv[++i]);
    } else if (std::strcmp(argv[i], "--wander") == 0 && i + 1 < argc) {
      wander_amp = std::stod(argv[++i]);
    } else if (std::strcmp(argv[i], "--mains") == 0 && i + 1 < argc) {
      mains_amp = std::stod(argv[++i]);
    } else if (std::strcmp(argv[i], "--out") == 0 && i + 1 < argc) {
      output_file = argv[++i];
    } else {
      std::cerr << "Unknown or incomplete option: " << argv[i] << "\n";
      print_usage(argv[0]);
      return 1;
    }
  }

  std::cout << "Starting Simulation:\n"
            << "  HR: " << heart_rate_bpm << " BPM\n"
            << "  Duration: " << duration_seconds << " s\n"
            << "  Rate: " << sampling_rate_hz << " Hz\n";

  // 1. Create Morphology
  // For now we stick to normal sinus, but we could parameterize this too
  const float64 pr_interval_val = 0.16;
  const float64 qrs_duration_val = 0.10;
  const float64 qrs_axis_val = 60.0;

  // We can add logic here to stretch QRS or modify axis based on args if needed
  Ecg_morphology morphology = create_normal_sinus_morphology(
      pr_interval_val, qrs_duration_val, qrs_axis_val);

  // Example: If user wanted some simple asymmetry (e.g. ST deviation), we could
  // modify it here. morphology.t_wave.shape_params.asymmetry = 0.5; // Example
  // usage

  // 2. Setup Engine
  ECGSimulationEngine engine(morphology, heart_rate_bpm, sampling_rate_hz);

  // 3. Add Noise
  if (std::abs(white_noise_amp) > 1e-9) {
    std::cout << "  Adding White Noise (amp=" << white_noise_amp << ")\n";
    engine.add_noise_source(
        std::make_shared<WhiteNoiseGenerator>(white_noise_amp));
  }
  if (std::abs(wander_amp) > 1e-9) {
    std::cout << "  Adding Baseline Wander (amp=" << wander_amp << ")\n";
    engine.add_noise_source(
        std::make_shared<BaselineWanderGenerator>(wander_amp));
  }
  if (std::abs(mains_amp) > 1e-9) {
    std::cout << "  Adding Mains Hum (amp=" << mains_amp << ")\n";
    engine.add_noise_source(std::make_shared<MainsHumGenerator>(mains_amp));
  }

  // 4. Generate
  const std::vector<Lead_sample> samples = engine.generate(duration_seconds);

  // 5. Output
  std::ofstream csv_output(output_file);
  if (!csv_output.is_open()) {
    std::cerr << "Failed to open output file: " << output_file << "\n";
    return 1;
  }

  csv_output << "time,lead_I,lead_II,lead_III,aVR,aVL,aVF,V1,V2,V3,V4,V5,V6\n";
  csv_output << std::fixed << std::setprecision(6);

  for (const auto &sample : samples) {
    csv_output << sample.time_s << ',' << sample.leads[lead_i_index] << ','
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

  std::cout << "Simulation complete. Data written to " << output_file << "\n";
  return 0;
}
