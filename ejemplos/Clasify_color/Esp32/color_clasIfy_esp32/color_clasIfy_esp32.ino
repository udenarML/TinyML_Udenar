// Arduino_TensorFlowLite - Version: 0.alpha.precompiled

#include <TensorFlowLite_ESP32.h>

#include <tensorflow/lite/experimental/micro/kernels/all_ops_resolver.h>
#include <tensorflow/lite/experimental/micro/micro_error_reporter.h>
#include <tensorflow/lite/experimental/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include <tensorflow/lite/version.h>
#include "Adafruit_APDS9960.h"
#include "model.h"
Adafruit_APDS9960 apds;
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
const char* CLASSES[] = {
   
  "Azul",
  "Rojo", 
  "Verde"
};

#define NUM_CLASSES (sizeof(CLASSES) / sizeof(CLASSES[0]))

int led_azul = 7;
int led_rojo = 5;
int led_verde = 6;
void setup() {
  
  pinMode(led_azul,OUTPUT);
  pinMode(led_rojo,OUTPUT);
  pinMode(led_verde,OUTPUT);
  
  digitalWrite(led_azul,LOW);
  digitalWrite(led_rojo,LOW);
  digitalWrite(led_verde,LOW);
  Serial.begin(115200);
  while (!Serial) {};

  Serial.println("Object classification using RGB color sensor");
  Serial.println("--------------------------------------------");
  Serial.println("Esp32 s3 running TensorFlow Lite Micro");
  Serial.println("");

  if(!apds.begin()){
    Serial.println("failed to initialize device! Please check your wiring.");
  }
   //enable color sensign mode
  apds.enableColor(true);
   apds.enableProximity(true);
 
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
   uint16_t r, g, b, c, p;
  float sum;

  // check if both color and proximity data is available to sample
    while (!apds.colorDataReady()) {
    delay(5);
    }
  // read the color and proximity sensor
  apds.getColorData(&r, &g, &b, &c);
  sum = r + g + b;
  p = apds.readProximity();
  

  // check if there's an object close and well illuminated enough
   if(p > 10 && c < 10 && sum > 0) {

    float redRatio = r / sum;
    float greenRatio = g / sum;
    float blueRatio = b / sum;

   

    input->data.f[0] = redRatio;
    input->data.f[1] = greenRatio;
    input->data.f[2] = blueRatio;

    // Run inferencing
  
      TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    error_reporter->Report("Invoke failed: %f\n");
    return;
  }

    // Output results
    for (int i = 0; i < NUM_CLASSES; i++) {
      Serial.print(CLASSES[i]);
      Serial.print(" ");
      Serial.print(int(output->data.f[i] * 100));
      Serial.print("%\n");
    }
    Serial.println();
    if(output->data.f[0]>= 0.9){
        
        digitalWrite(led_azul,HIGH);
        digitalWrite(led_rojo,LOW);
        digitalWrite(led_verde,LOW);
        
      }
       if(output->data.f[1]>= 0.9){
       
        digitalWrite(led_azul,LOW);
        digitalWrite(led_rojo,HIGH);
        digitalWrite(led_verde,LOW);
        
      }
      if(output->data.f[2]>= 0.9){
        
        digitalWrite(led_azul,LOW);
        digitalWrite(led_rojo,LOW);
        digitalWrite(led_verde,HIGH);
        
      }
      
    // Wait for the object to be moved away
    while ((apds.readProximity() <= 5)) {}
  }

}
