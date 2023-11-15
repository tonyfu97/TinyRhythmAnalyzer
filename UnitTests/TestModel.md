# How to test the model on local machine

To test the model:

1. Clone the [TensorFlow Lite for Microcontrollers](https://github.com/tensorflow/tflite-micro)
2. Replace the content of `tflite-micro/tensorflow/lite/micro/examples/hello_world/hello_world_test.cc` with the code below.
3. Download the `.tflite` model you trained on Colab and place it in the `tflite-micro/tensorflow/lite/micro/examples/hello_world/models` folder. Rename this file to `hello_world_int8.tflite` (you will have to remove or rename the existing file of the same name). 
4. Run `make -f tensorflow/lite/micro/tools/make/Makefile test_hello_world_test` to build and run the test. This will take a while, especially if it is the first time you are building the project. 

```cpp
/* Copyright 2023 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <math.h>

#include "tensorflow/lite/core/c/common.h"
#include "tensorflow/lite/micro/examples/hello_world/models/hello_world_int8_model_data.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_profiler.h"
#include "tensorflow/lite/micro/recording_micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

TfLiteStatus TestModel() {
  // Define a tensor arena with an appropriate size
  constexpr int tensor_arena_size = 2 * 1024;  // Adjust size as needed
  alignas(16) uint8_t tensor_arena[tensor_arena_size];

  // Map the model into a usable data structure
  const tflite::Model* model = tflite::GetModel(g_hello_world_int8_model_data);

  // Make sure the schema version of the model matches the one we expect
  TFLITE_CHECK_EQ(model->version(), TFLITE_SCHEMA_VERSION);

  // Create a MicroOpResolver and add the custom operations
  tflite::MicroMutableOpResolver<3> op_resolver;
  TF_LITE_ENSURE_STATUS(op_resolver.AddFullyConnected());
  TF_LITE_ENSURE_STATUS(op_resolver.AddLogistic());
  TF_LITE_ENSURE_STATUS(op_resolver.AddRelu());

  // Build an interpreter to run the model with
  tflite::MicroInterpreter interpreter(model, op_resolver, tensor_arena,
                                       tensor_arena_size);

  // Allocate memory from the tensor_arena for the model's tensors
  TF_LITE_ENSURE_STATUS(interpreter.AllocateTensors());
  MicroPrintf("Model arena size = %u", interpreter.arena_used_bytes());

  // Obtain pointers to the model's input and output tensors
  TfLiteTensor* input = interpreter.input(0);
  TFLITE_CHECK_NE(input, nullptr);

  TfLiteTensor* output = interpreter.output(0);
  TFLITE_CHECK_NE(output, nullptr);

  // Check input tensor dimensions
  TFLITE_CHECK_EQ(input->type, kTfLiteFloat32);
  TFLITE_CHECK_EQ(input->dims->size, 2);
  TFLITE_CHECK_EQ(input->dims->data[0], 1);
  TFLITE_CHECK_EQ(input->dims->data[1], 10);

  // Check output tensor dimensions
  TFLITE_CHECK_EQ(output->type, kTfLiteFloat32);
  TFLITE_CHECK_EQ(output->dims->size, 2);
  TFLITE_CHECK_EQ(output->dims->data[0], 1);
  TFLITE_CHECK_EQ(output->dims->data[1], 1);


  const int num_arrhythmic_trials = 20;
  float arrhythmic_intervals[20][10] = {
      {0.521583, 0.248201, 0.796763, 0.550360, 0.526978, 0.163669, 0.165468, 0.739209, 0.464029, 0.343525},
      {0.416139, 0.356013, 0.363924, 0.397152, 0.352848, 0.362342, 0.381329, 0.275316, 0.545886, 0.409810},
      {0.466387, 0.796218, 0.363445, 0.718487, 0.418067, 0.380252, 0.380252, 0.590336, 0.573529, 0.605042},
      {0.548718, 0.343590, 1.000000, 0.741026, 0.725641, 0.469231, 0.948718, 0.741026, 0.733333, 0.464103},
      {0.544118, 0.481092, 0.397059, 0.575630, 0.222689, 0.224790, 0.911765, 0.418067, 0.714286, 0.443277},
      {0.186424, 0.263425, 0.231003, 0.220871, 0.203647, 0.328267, 0.202634, 0.263425, 0.226950, 0.197568},
      {0.370253, 0.384494, 0.137658, 0.137658, 0.536392, 0.420886, 0.162975, 0.101266, 0.443038, 0.473101},
      {0.376689, 0.687500, 0.368243, 0.638514, 0.476351, 0.439189, 0.479730, 0.589527, 0.548986, 0.555743},
      {0.776163, 0.613372, 0.593023, 0.601744, 0.590116, 0.595930, 0.389535, 0.799419, 0.552326, 0.645349},
      {0.630814, 0.604651, 0.610465, 0.587209, 0.610465, 0.552326, 0.662791, 0.622093, 0.415698, 0.799419},
      {0.763401, 0.360444, 0.334566, 0.425139, 0.386322, 0.552680, 0.569316, 0.641405, 0.384473, 0.900185},
      {0.197635, 0.398649, 0.228041, 0.535473, 0.496622, 0.271959, 0.422297, 0.476351, 0.312500, 0.391892},
      {0.614118, 0.183529, 0.315294, 0.701176, 0.531765, 0.682353, 0.562353, 0.618824, 0.555294, 0.625882},
      {0.336414, 0.341959, 0.759704, 0.292052, 0.728281, 0.325323, 0.726433, 0.373383, 0.693161, 0.347505},
      {0.366038, 0.903774, 0.367925, 0.894340, 0.364151, 0.907547, 0.367925, 0.441509, 0.443396, 0.616981},
      {0.979667, 0.787431, 0.349353, 0.316081, 0.926063, 0.800370, 0.377079, 0.338262, 0.955638, 0.768946},
      {0.888655, 0.485294, 0.722689, 0.460084, 0.569328, 0.378151, 0.672269, 0.453782, 0.615546, 0.451681},
      {0.217905, 0.417230, 0.429054, 0.423986, 0.251689, 0.724662, 0.253378, 0.464527, 0.425676, 0.375000},
      {0.470588, 0.525210, 0.203782, 0.203782, 0.550420, 0.401261, 0.598739, 0.401261, 0.598739, 0.432773},
      {0.229730, 0.212838, 0.456081, 0.260135, 0.327703, 0.369932, 0.366554, 0.418919, 0.280405, 0.266892},
  };

  const int num_normal_trials = 20;
  float normal_intervals[20][10] = {
      {0.555556, 0.462963, 0.410774, 0.427609, 0.335017, 0.575758, 0.331650, 0.523569, 0.449495, 0.427609},
      {0.732187, 0.756757, 0.746929, 0.727273, 0.710074, 0.707617, 0.690418, 0.714988, 0.724816, 0.754300},
      {0.700246, 0.660934, 0.668305, 0.744472, 0.751843, 0.685504, 0.695332, 0.700246, 0.710074, 0.658477},
      {0.783439, 0.783439, 0.690021, 0.685775, 0.743100, 0.751592, 0.734607, 0.668790, 0.747346, 0.787686},
      {0.757764, 0.739130, 0.720497, 0.773292, 0.763975, 0.748447, 0.729814, 0.748447, 0.736025, 0.757764},
      {0.749280, 0.746398, 0.740634, 0.743516, 0.772334, 0.740634, 0.752161, 0.731988, 0.729107, 0.740634},
      {0.616633, 0.626775, 0.618661, 0.626775, 0.622718, 0.624746, 0.649087, 0.555781, 0.679513, 0.655172},
      {0.717391, 0.704969, 0.711180, 0.692547, 0.720497, 0.723602, 0.711180, 0.692547, 0.704969, 0.708075},
      {0.725424, 0.738983, 0.713559, 0.677966, 0.683051, 0.718644, 0.733898, 0.700000, 0.671186, 0.681356},
      {0.348101, 0.400316, 0.373418, 0.356013, 0.368671, 0.460443, 0.420886, 0.526899, 0.340190, 0.352848},
      {0.695332, 0.697789, 0.727273, 0.737101, 0.746929, 0.712531, 0.690418, 0.687961, 0.695332, 0.714988},
      {0.610390, 0.592532, 0.616883, 0.618506, 0.584416, 0.615260, 0.602273, 0.900974, 0.462662, 0.678571},
      {0.687956, 0.684307, 0.802920, 0.852190, 0.826642, 0.894161, 0.852190, 0.726277, 0.795620, 0.793796},
      {0.415825, 0.410774, 0.553872, 0.457912, 0.447811, 0.508418, 0.594276, 0.632997, 0.444444, 0.447811},
      {0.748387, 0.738710, 0.674194, 0.796774, 0.716129, 0.770968, 0.800000, 0.790323, 0.758065, 0.748387},
      {0.760807, 0.755043, 0.755043, 0.778098, 0.769452, 0.760807, 0.766571, 0.789625, 0.783862, 0.746398},
      {0.576271, 0.559322, 0.514124, 0.516949, 0.522599, 0.508475, 0.525424, 0.525424, 0.564972, 0.460452},
      {0.553672, 0.562147, 0.570621, 0.584746, 0.570621, 0.553672, 0.531073, 0.519774, 0.522599, 0.539548},
      {0.332278, 0.471519, 0.384494, 0.485759, 0.337025, 0.332278, 0.314873, 0.335443, 0.392405, 0.433544},
      {0.736025, 0.751553, 0.763975, 0.739130, 0.729814, 0.745342, 0.767081, 0.739130, 0.711180, 0.720497},
  };

  int num_correct = 0;
  int num_correct_threshold = num_normal_trials * 0.8;

  // Loop over the trials
  for (int trial_idx = 0; trial_idx < num_normal_trials; trial_idx++) {
    // Provide an input value
    for (int i = 0; i < 10; i++) {
      input->data.f[i] = normal_intervals[trial_idx][i];
    }

    // Run the model on this input and check that it succeeds
    TfLiteStatus invoke_status = interpreter.Invoke();
    if (invoke_status != kTfLiteOk) {
      MicroPrintf("Invoke failed\n");
      return invoke_status;
    }

    // Read the predicted value
    float y_pred = output->data.f[0];
    // MicroPrintf("y_pred normal = %f\n", static_cast<double>(y_pred));
    
    if (y_pred < 0.7f) {
      num_correct++;
    }

  }

  float accuracy = static_cast<float>(num_correct) / static_cast<float>(num_normal_trials);
  MicroPrintf("accuracy of normal = %f\n", static_cast<double>(accuracy));
  TFLITE_CHECK_GE(num_correct, num_correct_threshold);

  num_correct = 0;
  num_correct_threshold = num_arrhythmic_trials * 0.8;

  // Loop over the trials
  for (int trial_idx = 0; trial_idx < num_arrhythmic_trials; trial_idx++) {
    // Provide an input value
    for (int i = 0; i < 10; i++) {
      input->data.f[i] = arrhythmic_intervals[trial_idx][i];
    }

    // Run the model on this input and check that it succeeds
    TfLiteStatus invoke_status = interpreter.Invoke();
    if (invoke_status != kTfLiteOk) {
      MicroPrintf("Invoke failed\n");
      return invoke_status;
    }

    // Read the predicted value
    float y_pred = output->data.f[0];
    // MicroPrintf("y_pred arrhythmic = %f\n", static_cast<double>(y_pred));
    
    if (y_pred > 0.7f) {
      num_correct++;
    }
  }

  accuracy = static_cast<float>(num_correct) / static_cast<float>(num_arrhythmic_trials);
  MicroPrintf("accuracy of arrhythmic = %f\n", static_cast<double>(accuracy));
  TFLITE_CHECK_GE(num_correct, num_correct_threshold);

  return kTfLiteOk;
}


int main(int argc, char* argv[]) {
  tflite::InitializeTarget();
  TF_LITE_ENSURE_STATUS(TestModel());
  MicroPrintf("~~~ALL TESTS PASSED~~~\n");
  return kTfLiteOk;
}
```