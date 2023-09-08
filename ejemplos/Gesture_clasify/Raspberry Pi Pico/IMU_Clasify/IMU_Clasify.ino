#include <TensorFlowLite.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include <tensorflow/lite/version.h>

#include "model.h"
#include <Wire.h>
 
//Direccion I2C de la IMU
#define MPU 0x68
 
//Ratios de
#define G_R 131.0 // 32768/250
 
//Conversion de radianes a grados 180/PI
#define RAD_A_DEG = 57.295779 conversion
#define A_R 16384.0 // 32768/2

int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
 
//Angulos
float Acc[3];
float Gy[3];
const float accelerationThreshold = 1.8; // threshold of significant in G's
const int numSamples = 50;
int contador=0;
int muestras = 50;
int samplesRead = numSamples;
bool bandera=true;
bool bandera2=true;


// global variables used for TensorFlow Lite (Micro)
tflite::MicroErrorReporter tflErrorReporter;

// pull in all the TFLM ops, you can remove this line and
// only pull in the TFLM ops you need, if would like to reduce
// the compiled size of the sketch.
tflite::AllOpsResolver tflOpsResolver;

const tflite::Model* tflModel = nullptr;
tflite::MicroInterpreter* tflInterpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;

// Create a static memory buffer for TFLM, the size may need to
// be adjusted based on the model you are using
constexpr int tensorArenaSize = 8 * 1024;
byte tensorArena[tensorArenaSize] __attribute__((aligned(16)));

// array to map gesture index to a name
const char* GESTURES[] = {
  "adelante",
  "atras",
  "derecha",
  "izquierda"
};

#define NUM_GESTURES (sizeof(GESTURES) / sizeof(GESTURES[0]))

void setup() {
   Wire.begin(); // D2(GPIO4)=SDA / D1(GPIO5)=SCL
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(115200);

  // get the TFL representation of the model byte array
  tflModel = tflite::GetModel(model);
  if (tflModel->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Model schema mismatch!");
    while (1);
  }

  // Create an interpreter to run the model
  tflInterpreter = new tflite::MicroInterpreter(tflModel, tflOpsResolver, tensorArena, tensorArenaSize, &tflErrorReporter);

  // Allocate memory for the model's input and output tensors
  tflInterpreter->AllocateTensors();

  // Get pointers for the model's input and output tensors
  tflInputTensor = tflInterpreter->input(0);
  tflOutputTensor = tflInterpreter->output(0);

}

void leer_acc(){
   Wire.beginTransmission(MPU);
   Wire.write(0x3B); //Pedir el registro 0x3B - corresponde al AcX
   Wire.endTransmission(false);
   Wire.requestFrom(MPU,6,true);   //A partir del 0x3B, se piden 6 registros
   AcX=Wire.read()<<8|Wire.read(); //Cada valor ocupa 2 registros
   AcY=Wire.read()<<8|Wire.read();
   AcZ=Wire.read()<<8|Wire.read();
   Acc[0]= AcX/A_R;
   Acc[1]= AcY/A_R;
   Acc[2]= AcZ/A_R;
   
  }
void leer_gyro(){
   Wire.beginTransmission(MPU);
   Wire.write(0x43);
   Wire.endTransmission(false);
   Wire.requestFrom(MPU,6,true);   //A partir del 0x43, se piden 6 registros
   GyX=Wire.read()<<8|Wire.read(); //Cada valor ocupa 2 registros
   GyY=Wire.read()<<8|Wire.read();
   GyZ=Wire.read()<<8|Wire.read();
 
   //Calculo del angulo del Giroscopio
   Gy[0] = GyX/G_R;
   Gy[1] = GyY/G_R;
   Gy[2] = GyZ/G_R;
  }
void loop() {
   while (samplesRead == numSamples) {
    
      leer_acc();

      // sum up the absolutes
      float aSum = fabs(Acc[0]) + fabs(Acc[1]) + fabs(Acc[2]);

      // check if it's above the threshold
      if (aSum >= accelerationThreshold) {
        // reset the sample read count
        samplesRead = 0;
        break;
      }
    
  }
    while (samplesRead < numSamples) {
    // check if new acceleration AND gyroscope data is available
     leer_acc();
     leer_gyro();

      // normalize the IMU data between 0 to 1 and store in the model's
      // input tensor
      tflInputTensor->data.f[samplesRead * 6 + 0] = (Acc[0] + 4.0) / 8.0;
      tflInputTensor->data.f[samplesRead * 6 + 1] = (Acc[1] + 4.0) / 8.0;
      tflInputTensor->data.f[samplesRead * 6 + 2] = (Acc[2] + 4.0) / 8.0;
      tflInputTensor->data.f[samplesRead * 6 + 3] = (Gy[0] + 2000.0) / 4000.0;
      tflInputTensor->data.f[samplesRead * 6 + 4] = (Gy[1] + 2000.0) / 4000.0;
      tflInputTensor->data.f[samplesRead * 6 + 5] = (Gy[2] + 2000.0) / 4000.0;

      samplesRead++;

      if (samplesRead == numSamples) {
        // Run inferencing
        TfLiteStatus invokeStatus = tflInterpreter->Invoke();
        if (invokeStatus != kTfLiteOk) {
          Serial.println("Invoke failed!");
          while (1);
          return;
        }

        // Loop through the output tensor values from the model
        for (int i = 0; i < NUM_GESTURES; i++) {
          Serial.print(GESTURES[i]);
          Serial.print(": ");
          Serial.println(tflOutputTensor->data.f[i], 4);
        }
        Serial.println();
        delay(200);
      }
    }
    
  }


  
