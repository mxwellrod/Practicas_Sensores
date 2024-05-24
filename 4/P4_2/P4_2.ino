#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // UUID del servicio
BLEByteCharacteristic colorCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite); // UUID de la característica

void setup() {
  Serial.begin(9600);
  while (!Serial);
  BLE.setLocalName("Maxwell");
  // set LED pin to output mode
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);         // when the central disconnects, turn off the LED
  digitalWrite(LEDR, HIGH);          // will turn the LED off
  digitalWrite(LEDG, HIGH);        // will turn the LED off
  digitalWrite(LEDB, HIGH);         // will turn the LED off
  
  
  // Inicializar BLE
  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE!");
    while (1);
  }

  // Configurar el servicio BLE
  ledService.addCharacteristic(colorCharacteristic);
  BLE.addService(ledService);

  // Configurar el UUID del servicio que se anunciará
  BLE.setAdvertisedService(ledService);

  // Iniciar la publicidad BLE
  BLE.advertise();

  Serial.println("Esperando conexiones BLE...");
}

void loop() {
  // Esperar a que un dispositivo central se conecte
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Conectado a: ");
    Serial.println(central.address());

    // Mantener la conexión mientras el dispositivo central esté conectado
    while (central.connected()) {
      // Verificar si se ha escrito en la característica
      if (colorCharacteristic.written()) {
        switch (colorCharacteristic.value()) {   // any value other than 0
          case 01:
            Serial.println("Red LED on");
            digitalWrite(LEDR, LOW);            // will turn the LED on
            digitalWrite(LEDG, HIGH);         // will turn the LED off
            digitalWrite(LEDB, HIGH);         // will turn the LED off
            break;
          case 02:
            Serial.println("Green LED on");
            digitalWrite(LEDR, HIGH);         // will turn the LED off
            digitalWrite(LEDG, LOW);        // will turn the LED on
            digitalWrite(LEDB, HIGH);        // will turn the LED off
            break;
          case 03:
            Serial.println("Blue LED on");
            digitalWrite(LEDR, HIGH);         // will turn the LED off
            digitalWrite(LEDG, HIGH);       // will turn the LED off
            digitalWrite(LEDB, LOW);         // will turn the LED on
            break;
          default:
            Serial.println(F("LEDs off"));
            digitalWrite(LEDR, HIGH);          // will turn the LED off
            digitalWrite(LEDG, HIGH);        // will turn the LED off
            digitalWrite(LEDB, HIGH);         // will turn the LED off
            break;
          }
        } 
      if (colorCharacteristic.written()) {
        // Leer el valor escrito en la característica
        uint8_t color = colorCharacteristic.value();
        
        // Cambiar intensidad LED según el valor recibido
        analogWrite(LED_BUILTIN, color);

        Serial.print("Color cambiado a: ");
        Serial.println(color);
      }
      delay(100);
    }

    // Mostrar que el dispositivo central se ha desconectado
    Serial.print("Desconectado de: ");
    Serial.println(central.address());
    
    // Apagar el LED al desconectarse
    analogWrite(LED_BUILTIN, 0);
  }
}




