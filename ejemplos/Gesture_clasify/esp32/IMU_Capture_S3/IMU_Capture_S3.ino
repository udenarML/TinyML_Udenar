#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;


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
void setup(void) {
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);
  pinMode(led2,OUTPUT);
  digitalWrite(led2,HIGH);
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
  delay(100);
  
  
}
void loop(){
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
      samplesRead++;
//      Serial.print(aSum_g);
//      Serial.print(',');
//      Serial.print(aSum);
//      Serial.println();
      // print the data in CSV format
      if(bandera2==true){
        Serial.println("aX,aY,aZ,gX,gY,gZ");
        bandera2=false;
        }
      Serial.print(Ax, 3);
      Serial.print(',');
      Serial.print(Ay, 3);
      Serial.print(',');
      Serial.print(Az, 3);
      Serial.print(',');
      Serial.print(g.gyro.x, 3);
      Serial.print(',');
      Serial.print(g.gyro.y, 3);
      Serial.print(',');
      Serial.print(g.gyro.z, 3);
      Serial.println();
      bandera=!bandera;
      if(bandera){
        digitalWrite(led2,HIGH);
        }
        else{
          digitalWrite(led2,LOW);
          }
      if (samplesRead == numSamples) {
        // add an empty line if it's the last sample
        Serial.println();
        digitalWrite(led2,LOW);
        contador++;
        delay(100);
      }
    if(contador == muestras){
        digitalWrite(led,HIGH);
        while(1){}
        }
  }

  
  }
