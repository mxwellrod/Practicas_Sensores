// #include "BBTimer.hpp"
// #include "mbed.h"
// #include <Arduino_LSM9DS1.h> // Acc
// #include <avr/dtostrf.h>

#include <Streaming.h>
#include <Wire.h>


float x[15];
float y[15];
float z[15];

int posAcc = 0;
int posGiro = 4;
int posMag = 9;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Wire.begin(); // Unirse al bus I2C
}

void loop() {
  Serial.println("1 for start transmission ");

  char ledVal[0];
  readSerial(ledVal); // Leer el valor de entrada
  Serial.println(ledVal);

  Wire.beginTransmission(8); // Transmitir al dispositivo #8 (esclavo)
  Wire.write(ledVal);        // Enviar el valor de entrada
  Wire.endTransmission();    // Finalizar la transmisión

  Wire.requestFrom(8, sizeof(float) * 45); // Solicitar datos al esclavo
  //delay(10000);
  Serial.println(Wire.available());
  // Esperar hasta que se reciba todos los bytes
  while (Wire.available() > 0 and Wire.available() <= sizeof(float) * 45) {

    for (int i = 0; i < 15; i++) {
      Wire.readBytes((byte*)&x[i], sizeof(float));
    }
    for (int i = 0; i < 15; i++) {
      Wire.readBytes((byte*)&y[i], sizeof(float));
    }
    for (int i = 0; i < 15; i++) {
      Wire.readBytes((byte*)&z[i], sizeof(float));
    }
  }

  // Imprimir los valores recibidos
  Serial.println("*************************************************");
  for (int cont = 0; cont < 5; cont++) {
    Serial << "ACC: " << x[posAcc + cont] << ", " << y[posAcc + cont] << ", " << z[posAcc + cont] << '\n';
    Serial << "GYR: " << x[posGiro + cont] << ", " << y[posGiro + cont] << ", " << z[posGiro + cont] << '\n';
    Serial << "MAG: " << x[posMag + cont] << ", " << y[posMag + cont] << ", " << z[posMag + cont] << '\n';
  }

  // Encender LED durante 1 Seg.
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);

}

void Init(){
  for(int i = 0; i < 15; i++){
    x[i] = 0.0;
    y[i] = 0.0;
    z[i] = 0.0;
  }
}

/* Leer datos de entrada desde el puerto serial */
int readSerial(char result[]) {
  int i = 0;
  while (1) {
    while (Serial.available() > 0) {
      char inChar = Serial.read();
      if (inChar == '\n') {
        result[i] = '\0';
        Serial.flush();
        return 0;
      }
      if (inChar != '\r') {
        result[i] = inChar;
        i++;
      }
    }
  }
}


