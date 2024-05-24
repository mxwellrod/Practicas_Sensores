#include "BBTimer.hpp"
#include "mbed.h"
#include <Streaming.h> // Serial como en C
#include <Wire.h> // I2C
#include <Arduino_LSM9DS1.h> // IMU
#include <avr/dtostrf.h>

float x[15]; //  acc
float y[15]; //  acc
float z[15]; //  acc

int posAcc = 0;
int posGiro = 4;
int posMag = 9;


void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // function that executes whenever data is received from writer
  Wire.onRequest(requestEvent); // register an event handler for data requests

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Init();
}

void loop() {}


void sendFloat(float value) {
  byte *data = (byte *)&value; // Convierte el flotante en un arreglo de bytes
  for (int i = 0; i < sizeof(float); i++) {
    Wire.write(data[i]); // Envía cada byte por separado
  }
}

void sendFloatArray(float* array, int size) {
  Wire.write((byte*)array, size * sizeof(float));
}

void receiveEvent(int howMany) {
   char c = Wire.read(); // receive a character
   
   if(c == '1'){
     Serial.println("Comunicación OK");
   }
}

void Init(){
  for(int i = 0; i < 15; i++){
    x[i] = 0.0;
    y[i] = 0.0;
    z[i] = 0.0;
  }
}

// requests data handler function
void requestEvent(){
  int cont = 0;

  while(cont < 5){

    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x[posAcc + cont], y[posAcc + cont], z[posAcc + cont]);
      Serial << "ACC: " << x[posAcc + cont] << ", " << y[posAcc + cont] << ", " << z[posAcc + cont] << '\n';
    }

    if (IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(x[posGiro + cont], y[posGiro + cont], z[posGiro + cont]);
      Serial << "GYR: " << x[posGiro + cont] << ", " << y[posGiro + cont] << ", " << z[posGiro + cont] << '\n';
    }

    if(IMU.magneticFieldAvailable()){
      IMU.readMagneticField(x[posMag + cont], y[posMag + cont], z[posMag + cont]);
      Serial << "MAG: " << x[posMag + cont] << ", " << y[posMag + cont] << ", " << z[posMag + cont] << '\n';
    }

    cont++;
    delay(200); // cada 200ms
  }

  Serial.println("***** FIN LECTURA, AHORA ENVIO DE DATOS *******");
  //Wire.write((byte*)x, 15*4);
  sendFloatArray(x, 15);
  sendFloatArray(y, 15);
  sendFloatArray(z, 15);

}