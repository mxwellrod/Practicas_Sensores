#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h> // Biblioteca para el acelerómetro LSM9DS1 en Arduino Nano 33 BLE Sense

BLEService accelerometerService("19B10000-E8F2-537E-4F6C-D104768A1214"); // UUID del servicio
BLECharacteristic accelerometerCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 6); // UUID de la característica para 3 ejes (cada eje es un int16_t)

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Inicializar BLE
  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE!");
    while (1);
  }

  // Inicializar el acelerómetro
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // Configurar el servicio BLE
  BLE.setLocalName("Acelerómetro");
  BLE.setAdvertisedService(accelerometerService);
  accelerometerService.addCharacteristic(accelerometerCharacteristic);
  BLE.addService(accelerometerService);

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
      // Leer los valores del acelerómetro
      if (IMU.accelerationAvailable()) {
        // Leer la aceleración en los tres ejes
        float x, y, z;
        IMU.readAcceleration(x, y, z);

        // Convertir los valores float a int16_t
        int16_t accelX = static_cast<int16_t>(x * 100); // Multiplicar por 100 para conservar dos decimales
        int16_t accelY = static_cast<int16_t>(y * 100);
        int16_t accelZ = static_cast<int16_t>(z * 100);

        // Imprimir los valores convertidos por Serial
        Serial.print("Valor de accelX (int16_t): ");
        Serial.println(accelX);
        Serial.print("Valor de accelY (int16_t): ");
        Serial.println(accelY);
        Serial.print("Valor de accelZ (int16_t): ");
        Serial.println(accelZ);

        // Crear un buffer para almacenar los valores de los tres ejes
        uint8_t buffer[6];
        memcpy(&buffer[0], &accelX, sizeof(accelX));
        memcpy(&buffer[2], &accelY, sizeof(accelY));
        memcpy(&buffer[4], &accelZ, sizeof(accelZ));

        // Escribir los datos en la característica BLE
        accelerometerCharacteristic.writeValue(buffer, sizeof(buffer));

        // Esperar un breve periodo antes de la próxima lectura
        delay(100);
      }
    }

    // Mostrar que el dispositivo central se ha desconectado
    Serial.print("Desconectado de: ");
    Serial.println(central.address());
  }
}




