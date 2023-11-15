#include <TensorFlowLite.h>

#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

#include "PeakDetector.h"
#include "CircularBuffer.h"
#include "ElasticEnvelope.h"
#include "DetectionResponder.h"
#include "model.h"

namespace {
  const int WINDOW_SIZE = 10;
  PeakDetector<5> peakDetector(0.01f, 0.2f);
  CircularBuffer<int, WINDOW_SIZE> buffer;
  ElasticEnvelope envelope(0.1f);

  constexpr int kTensorArenaSize = 2 * 1024;
  uint8_t tensor_arena[kTensorArenaSize];

  tflite::ErrorReporter* error_reporter = nullptr;
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;
} // namespace

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);

  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  model = tflite::GetModel(model_tflite);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // Pull in only the operation implementations we need.
  static tflite::MicroMutableOpResolver<3> op_resolver;
  op_resolver.AddFullyConnected();
  op_resolver.AddLogistic();
  op_resolver.AddRelu();

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  interpreter->AllocateTensors();

  // Set model input settings
  TfLiteTensor* model_input = interpreter->input(0);
  if ((model_input->dims->size != 2) ||
      (model_input->dims->data[0] != 1) ||
      (model_input->dims->data[1] != 10) ||
      (model_input->type != kTfLiteFloat32)) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Bad input tensor parameters in model");
    return;
  }

  // Set model output settings
  TfLiteTensor* model_output = interpreter->output(0);
  if ((model_output->dims->size != 2) ||
      (model_output->dims->data[0] != 1) ||
      (model_output->dims->data[1] != 1) ||
      (model_output->type != kTfLiteFloat32)) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Bad output tensor parameters in model");
    return;
  }

  // Setup detection responder
  DetectionResponder::setup();
}

void loop() {
  static long lastPeakTime = millis();
  int sample = analogRead(A0);
  Serial.println(sample);
  delay(10);

  peakDetector.addSample(sample);

  if (peakDetector.isPeakDetected())
  {
    long currTime = millis();
    int peakTimeDiff = currTime - lastPeakTime;
    lastPeakTime = currTime;
    buffer.push(peakTimeDiff);
    envelope.addValue(peakTimeDiff);

    if (buffer.isFilled()) {
      for (int i = 0; i < WINDOW_SIZE; ++i) {
        interpreter->input(0)->data.f[i] = static_cast<float>(buffer[i]) / envelope.getMax();
      }

      // Run the model on this input and check that it succeeds
      TfLiteStatus invoke_status = interpreter->Invoke();
      if (invoke_status != kTfLiteOk) {
        Serial.println("Invoke failed");
        while (true); // Halt execution
      }

      // Read the predicted value
      float y_pred = interpreter->output(0)->data.f[0];
//      Serial.print("y_pred = ");
//      Serial.println(y_pred);
      DetectionResponder::respondToDetection(y_pred);
    }
    peakDetector.clearPeakFlag();
  }
}
