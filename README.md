# TinyRhythmAnalyzer
TinyML-powered ECG arrhythmia detection on Arduino Nano 33 BLE Sense

![demo_gif](./figures/demo.gif)

---

## Requirements

### Hardware

- Arduino Nano 33 BLE Sense
- Breadboard
- Jumper wires x 3
- Potentiometer
- (Optional) [SparkFun Single Lead Heart Rate Monitor - AD8232](https://www.sparkfun.com/products/12650)

### Software

- Arduino IDE
- Python 3
- [TensorFlow Lite for Microcontrollers](https://github.com/tensorflow/tflite-micro)
- (Optional) [CppUnitLite](https://github.com/smikes/CppUnitLite)


---


## Disclaimer

This project is a conceptual tool intended for educational and research purposes only. It is not a medical device and should not be used for diagnosing or treating health conditions. The outputs of this project should not be considered accurate or reliable for clinical or diagnostic use. The creators of this project are not liable for any misuse or for any direct or indirect damage that may result from the use of the information provided. Always seek the advice of a qualified healthcare provider with any questions you may have regarding a medical condition or health concerns.


---


## Tutorial

I have recorded a video tutorial for this project:

[![TinyRhythmAnalyzer Tutorial](https://img.youtube.com/vi/0Z5XWqJZa8I/0.jpg)](https://youtu.be/0Z5XWqJZa8I)


And here are the rough steps:

### 1. Download the MIT-BIH Arrhythmia Database

Download the [MIT-BIH Arrhythmia Database](https://www.physionet.org/content/mitdb/1.0.0/) and extract the `mit-bih-arrhythmia-database-1.0.0.zip` file.

### 2. Train a TensorFlow Model on Colab

Start a new notebook on [Google Colab](https://colab.research.google.com/) and upload the MIT-BIH Arrhythmia Database to your Google Drive. Clean the dataset to retain only signals with 'N' and 'V' annotations. Address any class imbalances with SMOTE. Develop a model that processes an array of 10 R-R intervals, predicting if the signal is normal or arrhythmic. Save the trained model as a `.tflite` file, then hexdump it to a `.cpp` file.

The model can be either a fully-connected neural network or a 1D convolutional neural network. I used the former for simplicity, but the latter makes more sense for time series data like ECG signals. LSTM networks are also a good option.

### 3. Write the Arduino Code

Write Arduino Code to extract R-R intervals from the incoming ECG signal.

### 4. (Optional) Test the Code on Local Machine

Test the code for R-R interval extraction on your local machine using `CppUnitLite`. Testing the TensorFlow Lite model is a bit more complicated. I recommend cloning the [TensorFlow Lite for Microcontrollers](https://github.com/tensorflow/tflite-micro) and overwriting the `tensorflow/lite/micro/examples/hello_world` example with your own test code. Then, you can run the test using `make -f tensorflow/lite/micro/tools/make/Makefile test_hello_world_test`.

### 5. Test the Code on Arduino

Put together the Arduino sketch and test it on the Arduino Nano 33 BLE Sense. I first use a potentiometer to simulate the ECG signal, just to make sure the code is working. Then, I connect the SparkFun Single Lead Heart Rate Monitor to the Arduino and test the code with real ECG signals.
