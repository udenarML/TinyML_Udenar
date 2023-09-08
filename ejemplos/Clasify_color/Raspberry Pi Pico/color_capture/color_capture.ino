#include <Wire.h> 
#include <Adafruit_APDS9960.h>

#define muestras 500
// construimos el objeto para nuestro sensor
Adafruit_APDS9960 apds; 

// variables
boolean bandera=true; 
boolean bandera2=true;
uint16_t r, g, b, c, p;
float sum;
int led=25;
int contador=0;


void setup() {
  
  Serial.begin(9600); // iniciamos el puerto serial.
  
  Wire.begin(); // iniciamos la comunicacion I2C.
  
  if(!apds.begin()){ // iniciamos el sensor 
    Serial.println("Ha ocurrido un error");
    while(1){}
  }
  
  //habilitacion de los datos de color y proximidad.
  apds.enableColor(true);
  apds.enableProximity(true);

  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);
  
}
void loop(){
  while (!apds.colorDataReady()) { // estancamos la ejecucion hasta que exista algun dato.
    delay(5);
    }
    
  // Leemos los datos de color y proximidad.
  apds.getColorData(&r, &g, &b, &c);
  sum = r + g + b; 
  p = apds.readProximity();
  
  // verificamos si existe algun objeto cerca y bien iluminado.
  if(p>10 && c < 10 && sum > 0){
    
    float redRatio = r / sum;
    float greenRatio = g / sum;
    float blueRatio = b / sum;

    // Escribimos el encabezado solo una vez
    if(bandera==true){
      Serial.println("Red,Green,Blue");
      bandera=!bandera;
      }
    // Escribimos los datos en formato .CSV  
    Serial.print(redRatio, 3);
    Serial.print(',');
    Serial.print(greenRatio, 3);
    Serial.print(',');
    Serial.print(blueRatio, 3);
    
    Serial.println();
    contador++;
    bandera2=!bandera2;
  }
  // Detenemos la ejecucion del codigo cuando se han tomado todas las muestras
  if(contador==muestras){
    contador=0;
    digitalWrite(led,LOW);
    while(1){}
    }
  
  if(bandera2==true){
    digitalWrite(led,HIGH);
    }
   else{
    digitalWrite(led,LOW);
    }   
   delay(50);
  }
