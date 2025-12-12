#ifndef SIGNAL_GENERATOR_H
#define SIGNAL_GENERATOR_H

#include <cmath>

/**
 * @brief Abstract base class for any time-variant signal source.
 *
 * Provides a common interface for ECG signals, noise sources, and biological
 * artifacts.
 */
class SignalGenerator {
public:
  virtual ~SignalGenerator() = default;

  /**
   * @brief Get the signal amplitude at a specific time.
   *
   * @param time_s Absolute simulation time in seconds.
   * @return Amplitude (usually in millivolts).
   */
  virtual double get_value(double time_s) = 0;
};

#endif // SIGNAL_GENERATOR_H
