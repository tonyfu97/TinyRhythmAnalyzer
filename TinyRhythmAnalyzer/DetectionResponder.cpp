#include "DetectionResponder.h"
#include <Arduino.h>

namespace DetectionResponder {

void setup() {
  // Initialize the built-in LED pins as outputs
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  // Start with LEDs off (LEDs are active LOW)
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
}

void respondToDetection(float y_pred) {
  // Turn LEDs OFF
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDR, HIGH);

  // Respond based on the y_pred value
  if (y_pred > 0.5f) {
    digitalWrite(LEDR, LOW);
  } else {
    digitalWrite(LEDG, LOW);
  }
}

} // namespace DetectionResponder
