// #include <Streaming.h>
// #include <Wire.h>
#include "BBTimer.hpp"
#include "mbed.h"
#include <Streaming.h>
#include <Wire.h>
#include <Arduino_LSM9DS1.h> // Acc
#include <avr/dtostrf.h>

float x[15]; //  acc,  giro,  magneto
float y[15]; //  acc,  giro,  magneto
float z[15]; //  acc,  giro,  magneto

int contx = 0, conty = 0, contz = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Wire.begin(); // join i2c bus (address optional for writer)
}
 
void loop() {
   
  String dataS = "";
  Serial.println("1 for start transmission ");
  char ledVal[0]; 

  readSerial(ledVal);
  Serial.println(ledVal);

  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(ledVal);        // sends the given value
  Wire.endTransmission();    // stop transmitting

  Serial.println((Wire.read()));
  Serial.println("*******************************************************************************");
  while (Wire.available() > 0) {
    inChar = Wire.read();
    Serial.println("xd");
    
    if (inChar != '\n') {
      dataS = dataS + inChar; 
      Serial.print("char");
      Serial.println(dataS);     
    }

    else if (inChar == '\n') { // contadores (para saber en que dato estoy)
      contx++;

      if(contx == 5){  
        contx--; 
        conty++;
      } else {
        x[contx] = dataS.toFloat();
      }

      if(conty == 5){
        conty--;
        contz++;
      } else {
        y[conty] = dataS.toFloat();
      }

      if(contz == 5){
        contx= 0;
        conty=0;
        contz=0;
      } else {
        z[contz] = dataS.toFloat();
      }
    }
    
  }
  // bucle para printear todos los valores ordenadamente
  for(int k = 0; k < 15; k++){

    Serial.print("Valor #");
    Serial.print(k);
    Serial.print(" x = ");
    Serial.print(x[k]);
    Serial.print(" y = ");
    Serial.print(y[k]);
    Serial.print(" z = ");
    Serial.println(z[k]);

    //Serial << "Valor sensor # " << k << ":" << '\n';
  }

  //valor = dataString.toFloat();
  //Serial.println(dataString);

  delay(500);
}

/* Read input serial */
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

