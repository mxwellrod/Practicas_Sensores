#include <Arduino.h>

// Definir constantes de pines
#define LED_PIN 2  // Pin del LED (cambiar según la placa)

// Declaración de funciones de las tareas
void sensorTask(void *parameter);
void uartTask(void *parameter);

// Variables globales
volatile int sensorValue = 0;
volatile bool newDataAvailable = false;

void setup() {
  Serial.begin(115200); // Iniciar la comunicación UART
  pinMode(LED_PIN, OUTPUT); // Configurar el pin del LED como salida

  // Crear la tarea para la simulación del sensor (lectura cada 100 ms)
  xTaskCreate(sensorTask, "SensorTask", 1000, NULL, 1, NULL);

  // Crear la tarea para enviar datos vía UART
  xTaskCreate(uartTask, "UARTTask", 1000, NULL, 2, NULL);
}

void loop() {
  // No es necesario utilizar el loop en FreeRTOS
}

// Tarea para simular la lectura de un sensor con valores aleatorios cada 100 ms
void sensorTask(void *parameter) {
  (void)parameter;
  TickType_t lastWakeTime = xTaskGetTickCount();

  while (1) {
    // Simular la lectura de un sensor con un valor aleatorio entre 0 y 1023
    sensorValue = random(0, 1024);
    
    // Indicar que hay nuevos datos disponibles para enviar por UART
    newDataAvailable = true;

    // Esperar 100 ms antes de la próxima lectura
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(100));
  }
}

// Tarea para enviar datos vía UART cada segundo
void uartTask(void *parameter) {
  (void)parameter;

  while (1) {
    // Esperar hasta que haya nuevos datos disponibles para enviar por UART
    while (!newDataAvailable) {
      vTaskDelay(pdMS_TO_TICKS(10)); // Pequeño retraso para no bloquear la tarea
    }

    // Enviar el valor del sensor por UART cada segundo
    Serial.print("Sensor Value: ");
    Serial.println(sensorValue);

    // Activar el LED durante 200 ms
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);

    // Reiniciar la bandera de nuevos datos disponibles
    newDataAvailable = false;

    // Esperar 1 segundo antes de la próxima transmisión
    vTaskDelay(pdMS_TO_TICKS(800));
  }
}



