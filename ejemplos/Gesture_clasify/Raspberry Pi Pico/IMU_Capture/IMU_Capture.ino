#include <Wire.h>
 
//Direccion I2C de la IMU
#define MPU 0x68
 
//Ratios de
#define G_R 131.0 // 32768/250
 
//Conversion de radianes a grados 180/PI
#define RAD_A_DEG = 57.295779 conversion
#define A_R 16384.0 // 32768/2
 
//MPU-6050 da los valores en enteros de 16 bits
//Valores RAW
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
 
//Angulos
float Acc[3];
float Gy[3];
const float accelerationThreshold = 1.4; // threshold of significant in G's
const int numSamples = 50;
int contador=0;
int muestras = 50;
int samplesRead = numSamples;
bool bandera=true;
bool bandera2=true;

void setup() {
  Wire.begin(); // D2(GPIO4)=SDA / D1(GPIO5)=SCL
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(115200);
  

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
      
      leer_acc();
      leer_gyro();

      samplesRead++;
      if(bandera2==true){
        Serial.println("aX,aY,aZ,gX,gY,gZ");
        bandera2=false;
        
        }
      // print the data in CSV format
      Serial.print(Acc[0], 3);
      Serial.print(',');
      Serial.print(Acc[1], 3);
      Serial.print(',');
      Serial.print(Acc[2], 3);
      Serial.print(',');
      Serial.print(Gy[0], 3);
      Serial.print(',');
      Serial.print(Gy[1], 3);
      Serial.print(',');
      Serial.print(Gy[2], 3);
      Serial.println();
      delay(10);
      if (samplesRead == numSamples) {
        // add an empty line if it's the last sample
        Serial.println();
        contador++;
        
      }
      if(contador== muestras){
        
        while(1){}
        }
     
  }
  
   
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
