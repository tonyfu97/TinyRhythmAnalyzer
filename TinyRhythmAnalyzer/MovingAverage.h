#pragma once

#include "CircularBuffer.h"

template<typename T, typename S, int bufferSize>
class MovingAverage {
private:
  CircularBuffer<T, bufferSize> buffer;
  S sum;

public:
  MovingAverage() : sum(0) {}

  void addValue(T value) {
    if (buffer.isFilled()) {
        sum -= buffer[0];
    }
    sum += value;
    buffer.push(value);
  }

  float getAverage() const {
    return static_cast<float>(sum) / buffer.size();
  }

  bool isBufferFilled() const {
    return buffer.isFilled();
  }
};
