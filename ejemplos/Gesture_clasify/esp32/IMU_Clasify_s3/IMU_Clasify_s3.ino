#include <TensorFlowLite_ESP32.h>

#include <tensorflow/lite/experimental/micro/kernels/all_ops_resolver.h>
#include <tensorflow/lite/experimental/micro/micro_error_reporter.h>
#include <tensorflow/lite/experimental/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include <tensorflow/lite/version.h>
#include "model.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* tfmodel = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;



// Create a static memory buffer for TFLM, the size may need to
// be adjusted based on the model you are using
constexpr int kTensorArenaSize = 8 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
}

// array to map gesture index to a name
const char* GESTURES[] = {
  "adelante",
  "atras",
  "derecha",
  "izquierda"
};

#define NUM_GESTURES (sizeof(GESTURES) / sizeof(GESTURES[0]))
const float accelerationThreshold = 1.9; // threshold of significant in m/s
const int numSamples = 80;
int samplesRead = numSamples;
float aSum, aSum_g;
float Ax, Ay, Az, Gx, Gy, Gz;
int contador=0;
int muestras=50;
int led=5;
int led2=6;
bool bandera=true;
bool bandera2=true;
void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10); 

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  tfmodel = tflite::GetModel(model);
  if (tfmodel->version() != TFLITE_SCHEMA_VERSION) {
    error_reporter->Report(
        "Model provided is schema version %d not equal "
        "to supported version %d.",
        tfmodel->version(), TFLITE_SCHEMA_VERSION);
    return;
  }
  static tflite::ops::micro::AllOpsResolver resolver;

   // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      tfmodel, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    error_reporter->Report("AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);

}

void loop() {
    while (samplesRead == numSamples){
     sensors_event_t a, g, temp;
     mpu.getEvent(&a, &g, &temp);
    
        aSum = fabs(a.acceleration.x) + fabs(a.acceleration.y) + fabs(a.acceleration.z);
        aSum_g=aSum/9.8;
    if (aSum_g >= accelerationThreshold) {
        // reset the sample read count
        samplesRead = 0;
        break;
      }
  
  }

   while (samplesRead < numSamples) {
  
      sensors_event_t a, g, temp;
     mpu.getEvent(&a, &g, &temp);
     Ax=(a.acceleration.x)/9.8;
     Ay=(a.acceleration.y)/9.8;
     Az=(a.acceleration.z)/9.8;
     
//     
     

    input->data.f[samplesRead * 6 + 0] = (Ax + 4.0) / 8.0;
     input->data.f[samplesRead * 6 + 1] = (Ay + 4.0) / 8.0; 
     input->data.f[samplesRead * 6 + 2] = (Az + 4.0) / 8.0;
     input->data.f[samplesRead * 6 + 3] = (g.gyro.x + 2000.0) / 4000.0;
     input->data.f[samplesRead * 6 + 4] = (g.gyro.y + 2000.0) / 4000.0;
     input->data.f[samplesRead * 6 + 5] = (g.gyro.z + 2000.0) / 4000.0;
      samplesRead++;
      if (samplesRead == numSamples) {
        TfLiteStatus invoke_status = interpreter->Invoke();
          if (invoke_status != kTfLiteOk) {
          error_reporter->Report("Invoke failed: %f\n");
          return;
           }

          // Output results
        for (int i = 0; i < NUM_GESTURES; i++) {
      Serial.print(GESTURES[i]);
      Serial.print(" ");
      Serial.print(int(output->data.f[i] * 100));
      Serial.print("%\n");
    }
    Serial.println();
       
        delay(100);
      }
   
  }

}
