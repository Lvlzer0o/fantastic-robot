#ifndef NOISE_GENERATOR_H
#define NOISE_GENERATOR_H

#include "SignalGenerator.h"
#include <random>
#include <vector>

/**
 * @brief Generates Gaussian white noise (thermal/electronic noise).
 */
class WhiteNoiseGenerator : public SignalGenerator {
public:
  explicit WhiteNoiseGenerator(double amplitude)
      : amplitude_(amplitude), distribution_(-1.0, 1.0) {}

  double get_value(double time_s) override {
    // We use a simple approximation or standard deviation.
    // For consistent noise across calls, we might want a member RNG,
    // but 'time' dependent deterministic noise is harder for white noise
    // without hashing time. For simulation, random access is fine.
    return distribution_(generator_) * amplitude_;
  }

private:
  double amplitude_;
  std::mt19937 generator_{std::random_device{}()};
  std::uniform_real_distribution<double> distribution_;
};

/**
 * @brief Generates 50/60Hz Mains Hum.
 */
class MainsHumGenerator : public SignalGenerator {
public:
  MainsHumGenerator(double amplitude, double frequency_hz = 60.0,
                    double phase_deg = 0.0)
      : amplitude_(amplitude), frequency_(frequency_hz),
        phase_rad_(phase_deg * 3.14159265359 / 180.0) {}

  double get_value(double time_s) override {
    return amplitude_ *
           std::sin(2.0 * 3.14159265359 * frequency_ * time_s + phase_rad_);
  }

private:
  double amplitude_;
  double frequency_;
  double phase_rad_;
};

/**
 * @brief Generates 1/f Pink Noise (Baseline Wander).
 * Uses the Voss-McCartney algorithm or a simple sum of low-frequency sine
 * waves. Here we implement a Sum-of-Sines approach for deterministic wandering.
 */
class BaselineWanderGenerator : public SignalGenerator {
public:
  BaselineWanderGenerator(double amplitude) : amplitude_(amplitude) {
    // Initialize a few low-frequency oscillators to simulate breathing/movement
    oscillators_.push_back({0.15, 0.0}); // Respiration (approx 9 bpm)
    oscillators_.push_back({0.5, 2.0});  // Body movement
    oscillators_.push_back({0.05, 1.0}); // Very slow drift
  }

  double get_value(double time_s) override {
    double val = 0.0;
    for (const auto &osc : oscillators_) {
      val += std::sin(2.0 * 3.14159265359 * osc.first * time_s + osc.second);
    }
    // Normalize roughly to -1..1 range before scaling
    return (val / oscillators_.size()) * amplitude_;
  }

private:
  std::vector<std::pair<double, double>> oscillators_; // {freq, phase}
  double amplitude_;
};

/**
 * @brief Composite generator that sums multiple sources.
 */
class CompositeGenerator : public SignalGenerator {
public:
  void add(SignalGenerator *generator) { components_.push_back(generator); }

  double get_value(double time_s) override {
    double total = 0.0;
    for (auto *gen : components_) {
      total += gen->get_value(time_s);
    }
    return total;
  }

private:
  std::vector<SignalGenerator *> components_;
};

#endif // NOISE_GENERATOR_H
