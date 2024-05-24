#include "BBTimer.hpp"  // Interrupts
#include "mbed.h"
#include <Streaming.h>        // Serial
#include <ArduinoBLE.h>       // BLE
#include <Arduino_LSM9DS1.h>  // Acc

#define BUFF_LENGTH 128
#define characteristic_length 64

BBTimer my_t0(BB_TIMER3);  // timer envio datos
bool myISR_EN = false;
bool ble_EN = false;
float acc[3] = { 0.0, 0.0, 0.0 };
float gyro[3] = { 0.0, 0.0, 0.0 };
float buff_accX[BUFF_LENGTH], buff_accZ[BUFF_LENGTH], buff_gyrY[BUFF_LENGTH];
int buff_index = 0;
int tsample = 5000; // en us
int a;


enum state { Idle,
             Rise,
             Up,
             Fall };
state currentState = Idle;
unsigned long tinit_rise = 0, tend_rise = 0;
unsigned long tinit_fall = 0, tend_fall = 0;
unsigned long tinit_high = 0, tend_high = 0;
unsigned long trise = 0, tfall = 0, thigh = 0;
float angle_mean = 0.0, cont_mean_angle = 0.0, angle_max = 0.0, angle_min = 0.0, rise_angle = 0.0, high_angle = 0.0, init_angle = 0.0;
// thresholds a mano
float accZ_rise_th = 0.71;
float accZ_fall_th = 0.84;
float accZ_up_th = 0.68;
float accZ_idle_th = 0.87;

float accX_rise_th = 50;
float accX_fall_th = 100;
float accX_up_th = 50;
float accX_idle_th = 100;

float gyrY_rise_high_th = -15;
float gyrY_rise_low_th = -8; //mov. mas lentos
float gyrY_fall_th = 10;
float gyrY_up_high_th = 1.4;
float gyrY_up_low_th = -0.4;
float gyrY_idle_th = 1.8;



BLEService puntiService("731d5ab7-21e4-4a7c-9c3d-49a765e8b2f4");  // 19B10000-E8F2-537E-4F6C-D104768A1214
BLEStringCharacteristic max_angleCharacteristic("e9c42d89-4c22-45c5-889e-db42f8cd7ffc", BLERead | BLENotify, characteristic_length);
BLEStringCharacteristic min_angleCharacteristic("f93c8a5c-f096-4f2d-a6a9-869ac2c7f6d7", BLERead | BLENotify, characteristic_length);  // 19B10001-E8F2-537E-4F6C-D104768A1214
BLEStringCharacteristic mean_angleCharacteristic("efac6cbc-4ed3-4783-a0d2-e7fc13044bd0", BLERead | BLENotify, characteristic_length);
BLEStringCharacteristic t_riseCharacteristic("358afbad-338d-4a24-9625-edef0022c700", BLERead | BLENotify, characteristic_length);
BLEStringCharacteristic t_fallCharacteristic("4405cf1a-ddd8-4d5b-969b-c3ea574c3691", BLERead | BLENotify, characteristic_length);
BLEStringCharacteristic t_highCharacteristic("7e8150cf-c6d5-4f2c-bae3-584a166822a4", BLERead | BLENotify, characteristic_length);
BLEBoolCharacteristic ENcharacteristic("b4dfe45e-8040-4b8d-b778-01b4daed6e7a", BLERead | BLEWrite);

void myISR_t0() {
  myISR_EN = true;  // Con booleanos no va ns porque
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial)
    ;

  // Inicializar BLE
  if (!BLE.begin()) {
    Serial << "Failed to initialize BLE!" << endl;
    while (1)
      ;
  }

  // Inicializar sensores
  if (!IMU.begin()) {
    Serial << "Failed to initialize IMU!" << endl;
    while (1)
      ;
  }

  BLE.setLocalName("mierda");
  BLE.setAdvertisedService(puntiService);
  puntiService.addCharacteristic(max_angleCharacteristic);
  puntiService.addCharacteristic(min_angleCharacteristic);
  puntiService.addCharacteristic(mean_angleCharacteristic);
  puntiService.addCharacteristic(t_riseCharacteristic);
  puntiService.addCharacteristic(t_fallCharacteristic);
  puntiService.addCharacteristic(t_highCharacteristic);
  puntiService.addCharacteristic(ENcharacteristic);
  BLE.addService(puntiService);
  BLE.advertise();

  Serial.println("Esperando conexiones BLE...");

  my_t0.setupTimer(tsample, myISR_t0);  // en us
  my_t0.timerStart();
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Conectado a: ");
    Serial.println(central.address());

    while (central.connected()) {

      if (ENcharacteristic.written()) {
        if (ENcharacteristic.value() == true) {
          Serial.println("Enable detector");
          ble_EN = true;
        } else {
          Serial.println("Disable detector");
          ble_EN = false;
        }
      }

      if (ble_EN & myISR_EN) {
        //Serial.println("Dentro del ISR EN y ble_EN");

        // 1: Lectura
        if (IMU.accelerationAvailable() & IMU.gyroscopeAvailable()) {
          // ACC: ard_y = -proj_z ; ard_z = proj_y ; ard_x = proj_x
          // GYR: ard_y = proj_z ; ard_z = proj_y ; ard_x = -proj_x
          IMU.readAcceleration(acc[0], acc[1], acc[2]);
          buff_accX[buff_index] = acc[0];
          buff_accZ[buff_index] = -acc[1];
          IMU.readGyroscope(gyro[0], gyro[1], gyro[2]);
          buff_gyrY[buff_index] = gyro[2];
          // Serial << "accX: " << acc[0] << endl;
          // Serial << "gyrY: " << gyro[2] << endl; // si imprimo el buffer va mal
          buff_index = (buff_index + 1) % BUFF_LENGTH; // avoid buff overflow
          myISR_EN = false;
        }  // fin lectura

        switch(currentState){
            case Idle:
              if(buff_gyrY[buff_index] <= gyrY_rise_low_th & buff_accZ[buff_index] <= accZ_rise_th){ // done -acc[1] <= accZ_rise_th & 
                //init_angle =
                tinit_rise = millis();
                currentState = Rise;
                Serial.println("Rise");
              }
              break;

            case Rise:
              rise_angle += buff_gyrY[buff_index]*(tsample/1000000.0);
              angle_max = (angle_max < rise_angle) ? rise_angle:angle_max; // high angle is new maximum
              if( (buff_gyrY[buff_index] > gyrY_up_low_th) & (buff_gyrY[buff_index] < gyrY_up_high_th) & (buff_accZ[buff_index] > accZ_up_th) & (buff_accZ[buff_index] < accZ_rise_th) ){ // done -11.5 -acc[1] <= accZ_up_th &
                tend_rise = millis();
                trise = tend_rise - tinit_rise;
                Serial << "Angulo en la subida: " << rise_angle << '\n';
                max_angleCharacteristic.writeValue(String(angle_max));
                Serial << "Rise time: " << trise << '\n'; // print on serial
                t_riseCharacteristic.writeValue(String(trise)); // send to ble
                currentState = Up;
                Serial.println("Up");
              }
              break;

            case Up:
              high_angle += buff_gyrY[buff_index]*(tsample/1000000.0);
              cont_mean_angle+=1.0;
              // vector para acumular angulo medio?
              angle_min = (angle_min > high_angle) ? high_angle:angle_min; // high angle is new minimum

              if( (buff_gyrY[buff_index] > gyrY_fall_th) & (buff_accZ[buff_index] > accZ_fall_th)){ // done 2 -acc[1] > accZ_fall_th &
                tend_high = millis();
                thigh = tend_high - tend_rise;
                angle_mean =  high_angle/cont_mean_angle;
                Serial << "Tiempo puntillas: " << thigh << '\n';
                min_angleCharacteristic.writeValue(String(angle_min));
                mean_angleCharacteristic.writeValue(String(angle_mean));
                t_highCharacteristic.writeValue(String(thigh)); // send to ble
                currentState = Fall;
                Serial.println("Fall");
              }
              break;

            case Fall:
              if( (gyro[2] > gyrY_up_low_th) & (gyro[2] < gyrY_idle_th) & (buff_accZ[buff_index] > accZ_idle_th)){ // done 13 -acc[1] > accZ_idle_th &
                tend_fall = millis();
                tfall = tend_fall - tend_high;
                Serial << "Tiempo bajada: " << tfall << '\n';
                t_fallCharacteristic.writeValue(String(tfall)); // send to ble
                currentState = Idle;
                Serial.println("Idle");
              }
              break;

        } // fin MEF
      } // fin condicional

    }  // fin while connected

    Serial.print("Desconectado de: ");
    Serial.println(central.address());



  }  // fin loop
}
