#include "BBTimer.hpp"
#include "mbed.h"
#include <Streaming.h>
#include <Wire.h>

BBTimer my_t0(BB_TIMER0); // tareas 2 y 3

int sensorValue = 0;
const byte PWMPin = 5; // Arduino pin number
bool readEN = false; // Lectura habilitada o no
byte readEN2 = 0x00;

mbed::PwmOut pwmPin(digitalPinToPinName( D10 )); // usar el namespace que sino no va + mapeo del pin de arduino en mbed

const int max_length = 9; // Como mucho debería haber 8 -> hasta ADC(999) + CR/LF sería válida la entrada
static char msg[max_length];
int pos = 0; // posicion mensaje

void t0Callback()
{
  // Tareas 2 y 3
  readEN2 = ~readEN2; // not. Con booleanos no va ns porque
}

void setup() {

  Serial.begin(9600);
  my_t0.setupTimer(5000000, t0Callback);
	my_t0.timerStart();

  // Tarea 3
  pwmPin.period_us( 200 ); // 5kHz
  pwmPin.write(0.5); // Duty por default

  Serial.setTimeout(10000);

}

void loop() {
  // Tareas 2 y 3: Leer ADC cada 10s + sacar PWM con valor proporcional al leído

  if(readEN2 == 0xFF){
    sensorValue = analogRead(A0);
    Serial.println(sensorValue);
    readEN2 = ~readEN2;
    pwmPin.write((float)sensorValue/ 1023.0); // Duty actualizado
  }

  // Tarea 4: UART
  while(Serial.available() > 0){
    char actualByte = Serial.read();

    if(actualByte != '\n' && (pos < max_length -1)){
      // almacenarlo
      msg[pos] = actualByte;
      pos++;
    } 
    else { // opciones. No las aplico hasta que no haya terminado de leer

      msg[pos] = '\0'; // añadir final de cadena
      Serial << "mensaje: " << msg << '\n';

      // Casuística
      if(strcmp(msg, "ADC") == 0){ // Caso 1; si se hace directamente solo toma el primer valor del array
        Serial << "Valor del sensor " << sensorValue << '\n'; // una linea pero no hace salto
      // Serial.print("Valor del sensor: ");
      // Serial.println(sensorValue);
      }
      else{
        // Obtener dato
        int data = atoi(msg + 4); // Solo guarda el número; aotf(msg + 4) para valores decimales
        Serial << "Cond data: " << data << '\n';

        if(strncmp(msg, "ADC", 3) == 0){
          int t = data*1000000;
          my_t0.setupTimer(t, t0Callback);
	        my_t0.timerStart();
        }
        else if(strncmp(msg, "PWM", 3) == 0){
          // Cambiar duty
          pwmPin.write(1.0); // Duty va de 0 a 9 -> Son enteros o no?? (float)data/ 9.0
        }
      }

      pos = 0; // reinicio
    }
  }
  
}
