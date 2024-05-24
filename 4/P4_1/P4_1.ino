#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // UUID del servicio
BLEByteCharacteristic colorCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite); // UUID de la característica

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT); // Configurar el pin del LED como salida
  
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

  // Iniciar advertising BLE
  BLE.advertise();

  Serial.println("Esperando conexiones BLE...");
}

void loop() {
  // Esperar a que un dispositivo se conecte
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Conectado a: ");
    Serial.println(central.address());

    // Mantener la conexión mientras el dispositivo central esté conectado
    while (central.connected()) {
      // Verificar si se ha escrito en la característica
      if (colorCharacteristic.written()) {
        // Leer el valor escrito en la característica
        uint8_t color = colorCharacteristic.value();
        
        // Cambiar el color del LED según el valor recibido
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




