#include <Wire.h>
#include <Arduino_LSM9DS1.h> // Acc
BBTimer my_t0(BB_TIMER0); // tareas 2 y 3

void t0Callback()
{
  // Tareas 2 y 3
  readEN2 = ~readEN2; // not. Con booleanos no va ns porque
}


void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // function that executes whenever data is received from writer
  my_t0.setupTimer(5000000, t0Callback);
	my_t0.timerStart();
}

void loop() {
  delay(100);
}

void receiveEvent(int howMany) {
   char c = Wire.read(); // receive a character
   if(c == '0'){
     digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
   }
   if(c == '1'){
     digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
   }
}
 