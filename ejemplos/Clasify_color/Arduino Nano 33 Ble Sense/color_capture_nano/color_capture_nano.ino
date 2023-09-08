#include <Arduino_APDS9960.h>  // libreria del sensor de color
#define muestras 500  //cantidad de muestras a tomar
// variables
boolean bandera=true; 

int  r, g, b, c, p;
float sum;
int contador=0;

void setup(){
  Serial.begin(9600);
   while (!Serial) {};
   
  if(!APDS.begin()){
    Serial.println("Error al inicializar el sensor");
    while(1){}
    }
  
  pinMode(LEDR,OUTPUT);
  pinMode(LEDG,OUTPUT);
  digitalWrite(LEDR,HIGH);
  digitalWrite(LEDG,HIGH);
  Serial.println("Red,Green,Blue");
  
}

void loop(){
  // esperamos hasta que esten disponibles los datos del sensor.
  while(!APDS.colorAvailable() || !APDS.proximityAvailable()){}

  // leemos los datos del sensor.
  APDS.readColor(r, g, b, c);
  sum = r + g + b;
  p= APDS.readProximity();

  // capturamos los datos si el objeto esta lo suficientemente cerca y bien iluminado.
  if(p == 0 && c <= 30 && sum > 0){
   float redRatio = r / sum;
    float greenRatio = g / sum;
    float blueRatio = b / sum;
  
    // enviamos los datos en formato CSV
    Serial.print(redRatio, 3);
    Serial.print(", ");
    Serial.print(greenRatio, 3);
    Serial.print(", ");
    Serial.print(blueRatio, 3);
    
    Serial.println();
    contador++;
     
  }
    if(contador== muestras){
      digitalWrite(LEDR,LOW);
      
      while(1){}
    }
    
    
        
    delay(20);
    
  
}
