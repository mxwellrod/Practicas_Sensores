#include "BBTimer.hpp"
#include "mbed.h"
#include <Streaming.h>
#include <Wire.h>
#include <Arduino_LSM9DS1.h> // Acc
#include <avr/dtostrf.h>
BBTimer my_t0(BB_TIMER0); // tarea 7


bool sendEN = false;
bool readEN = false;

int cont = 0;
int contSend = 0;
float x = 0.0, y = 0.0, z = 0.0; // variables auxiliares para leer de las funciones y luego guardarlo en el acc.
float xacc[15]; //  acc,  giro,  magneto
float yacc[15]; //  acc,  giro,  magneto
float zacc[15]; //  acc,  giro,  magneto

char xBuff[15]; //  acc,  giro,  magneto
char yBuff[15]; //  acc,  giro,  magneto
char zBuff[15]; //  acc,  giro,  magneto

int posAcc = 0;
int posGiro = 4;
int posMag = 9;

void t0Callback()
{
  readEN = true;
  cont++;

}

void t1Callback()
{
  // Tareas 2 y 3
  sendEN = ~sendEN; // not. Con booleanos no va ns porque
}


void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // function that executes whenever data is received from writer
  my_t0.setupTimer(200000, t0Callback); // muestreo cada 200ms
	//my_t0.timerStart();
  //my_t1.setupTimer(1000000, t1Callback); // enviarlos tras el segundo
	//my_t1.timerStart();

  Init();

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

}

void loop() {


  if(readEN == true){
  
    if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    xacc[posAcc + cont] = x;
    yacc[posAcc + cont] = y;
    zacc[posAcc + cont] = z;
    Serial.print("Valor x ACC #");
    Serial.println(x);
    }

    if (IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(x, y, z);
      xacc[posGiro + cont] = x;
      yacc[posGiro + cont] = y;
      zacc[posGiro + cont] = z;
      Serial.print("Valor x Giros #");
      Serial.println(x);   
    }

    if(IMU.magneticFieldAvailable()){
      IMU.readMagneticField(x, y, z);
      xacc[posMag + cont] = x;
      yacc[posMag + cont] = y;
      zacc[posMag + cont] = z;
      Serial.print("Valor x Mag #");
      Serial.println(x);
    }

    if(cont == 5){
      Serial.println("fin read");
      sendEN = true;
      cont = 0;
      my_t0.timerStop(); // stop sampling
    }

    readEN = false;

  }

  if(sendEN == true){ // enviar valores
    Serial.println("Send");
    //guardar en string
    sendSamples();
    sendEN = false;

  }
}

void Init(){
  for(int i = 0; i < 15; i++){
    xacc[i] = 0.0;
    yacc[i] = 0.0;
    zacc[i] = 0.0;
    xBuff[i] = 0.0;
    yBuff[i] = 0.0;
    zBuff[i] = 0.0;
  }
}

void sendSamples() {
  for (int i = 0; i < 15; i++) {
    char buffer[32];
    Serial.println(xacc[i]);
    int val = snprintf(buffer, sizeof buffer, "%f", xacc[i]);
    Serial.println(buffer);
    Wire.write(buffer);
    // Wire.write((byte*)&xacc[i], sizeof(float));
    // Wire.write((byte*)&yacc[i], sizeof(float));
    // Wire.write((byte*)&zacc[i], sizeof(float));
  }
}

void receiveEvent(int howMany) {
   char c = Wire.read(); // receive a character
   Serial.println("received");
   if(c == '1'){
     my_t0.timerStart();  // start sampling
   }
}

 