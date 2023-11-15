#pragma once

class ElasticEnvelope {
private:
  float currentMax;
  float currentMin;
  float decayRate;
  float average;
  bool initialized;

public:
  explicit ElasticEnvelope(float rate)
  : currentMax(0.0f),
    currentMin(0.0f),
    decayRate(rate),
    average(0.0f),
    initialized(false) {}

  void addValue(float value) {
    if (!initialized) {
      currentMax = value;
      currentMin = value;
      average = value;
      initialized = true;
      return;
    }
        
    // 1. Update current max and min
    if (value > currentMax) {
      currentMax = value;
    }
    if (value < currentMin) {
      currentMin = value;
    }

    // 2. Calculate the midpoint of the current max and min
    float midpoint = currentMin + (currentMax - currentMin) / 2.0f;

    // 3. Exponentially decay the max and min towards the midpoint
    currentMax -= decayRate * (currentMax - midpoint);
    currentMin += decayRate * (midpoint - currentMin);

    average = midpoint;
  }

  float getMax() const {
    return currentMax;
  }
  
  float getMin() const {
    return currentMin;
  }

  float getAverage() const {
    return average;
  }

  bool isInitialized() const {
    return initialized;
  }

  void reset() {
    currentMax = 0.0f;
    currentMin = 0.0f;
    average = 0.0f;
    initialized = false;
  }
};
