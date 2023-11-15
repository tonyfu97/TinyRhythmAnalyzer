#pragma once

#include "MovingAverage.h"
#include "ElasticEnvelope.h"

template<int MovingAvergeSize>
class PeakDetector {
private:
  MovingAverage<float, float, MovingAvergeSize> filter;
  bool peakDetected;
  ElasticEnvelope envelope;
  float upperThreshold;
  float lowerThreshold;
  float hysteresis;
  float minThresholdDifference;
  bool crossedUpper;

public:
  PeakDetector(float decayRate, float hysteresisRatio = 0.2f, float minThresholdDiff = 0.05f)
    : peakDetected(false), 
      envelope(decayRate),
      hysteresis(hysteresisRatio),
      minThresholdDifference(minThresholdDiff),
      crossedUpper(false) {}

  void addSample(float sample) {
    // Step 1: Filter the sample with the moving average
    filter.addValue(sample);
    float filteredSample = filter.getAverage();

    // Step 2: Update the envelope with the filtered sample
    envelope.addValue(filteredSample);

    // Step 3: Calculate dynamic thresholds based on the envelope
    upperThreshold = envelope.getAverage() + 3 * hysteresis * (envelope.getMax() - envelope.getAverage());
    lowerThreshold = envelope.getAverage() + 1 * hysteresis * (envelope.getAverage() - envelope.getMin());
    
    // Step 4: If the two thresholds are too close, skip this sample;
    if (upperThreshold - lowerThreshold < minThresholdDifference)
    {
        return;
    }

    // Step 5: Simple peak detection logic with hysteresis
    if (!crossedUpper && filteredSample > upperThreshold) {
      // Signal crosses above the upper threshold
      crossedUpper = true;
    } else if (crossedUpper && filteredSample < lowerThreshold) {
      // Signal crosses below the lower threshold after crossing the upper threshold
      peakDetected = true;
      crossedUpper = false;
    }

    // If signal is between thresholds, keep the state
  }

  bool isPeakDetected() const {
    return peakDetected;
  }

  void clearPeakFlag() {
    peakDetected = false;
  }

  float getUpperThreshold() const {
    return upperThreshold;
  }

  float getLowerThreshold() const {
    return lowerThreshold;
  }

  // Optional: reset function to clear the state of the peak detector
  void reset() {
    filter = MovingAverage<float, float, MovingAvergeSize>(); // Reconstruct the filter
    envelope.reset();
    peakDetected = false;
    crossedUpper = false;
  }
};
