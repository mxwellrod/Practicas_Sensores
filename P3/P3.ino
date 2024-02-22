#include "BBTimer.hpp"
#include "mbed.h"
#include <Streaming.h>
#include <Wire.h>
#include <Arduino_LSM9DS1.h> // Acc
#include <avr/dtostrf.h>

BBTimer my_t0(BB_TIMER0); // tareas 2 y 3
byte EN = 0x00;
float acc[3] = {0.0, 0.0, 0.0};

void t0Callback()
{
  // Tareas 2 y 3
  EN = ~EN; // not. Con booleanos no va ns porque
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  my_t0.setupTimer(50000, t0Callback);
	my_t0.timerStart();

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(EN){
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(acc[0], acc[1], acc[2]);
      Serial.print("Valor x ACC #");
      Serial.println(acc[0]);
    }
  }
}
