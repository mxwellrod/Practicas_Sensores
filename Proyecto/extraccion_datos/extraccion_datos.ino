#include "BBTimer.hpp"
#include "mbed.h"
#include <Streaming.h>
#include <Arduino_LSM9DS1.h> // Acc

BBTimer my_t0(BB_TIMER0); // timer envio datos
byte EN = 0x00; // con bool no funciona adecuadamente
void myISR_t0(); // prototipo

// arrays inicializados
float acc[3] = {0.0, 0.0, 0.0};
float gyro[3] = {0.0, 0.0, 0.0};
int ts = 5000; // t sampling en us -> 5ms = 200 Hz
int tdebug = 500000; // para debug en puerto serie
unsigned long start, end; // principio y final de lectura sensores

void myISR_t0()
{
  EN = ~EN; // Con booleanos no va ns porque
}

void setup() {
  
  Serial.begin(115200);
  my_t0.setupTimer(tdebug, myISR_t0); // en us
	my_t0.timerStart();

  if (!IMU.begin()) {
    Serial << "Failed to initialize IMU!" << endl;
    while (1);
  }

}

void loop() {
  // Habilitada la lectura, los datos se envian por el puerto serie separados por comas.
  // Los sentidos de los sensores se invierten de acuerdo al convenio establecido para el proyecto.

  if(EN){
    start = millis();
    if (IMU.accelerationAvailable() & IMU.gyroscopeAvailable()) {
      // ACC: ard_y = -proj_z ; ard_z = proj_y ; ard_x = proj_x
      // GYR: ard_y = proj_z ; ard_z = proj_y ; ard_x = -proj_x 
      IMU.readAcceleration(acc[0], acc[1], acc[2]);
      IMU.readGyroscope(gyro[0], gyro[1], gyro[2]);
      end = millis(); 
      //Serial  << acc[0] << ", " << acc[2] << ", " << -acc[1] << ", " << -gyro[0] << ", " << gyro[2] << ", " << gyro[1] << '\n';
      //Serial  << "Ax: " << acc[0] << "Az: " << -acc[1] << "Gy:" << gyro[2] << '\n'; // Ax, Az, Gy
      //Serial  << "Gy:" << gyro[2] << '\n'; // Ax, Az, Gy
      Serial  << "Az:" << -acc[1] << '\n'; // Ax, Az, Gy
      EN = ~EN;
    }
    // reposo: Ax = -0.16 Az = 1 Gy = -3.4
    

  }



}

