#include <Arduino.h>

#define LED_PIN 2            // Pin del LED (cambiar según la placa)
#define UART_NUM UART_NUM_0  // UART a utilizar (UART0 en ESP32)

TaskHandle_t blinkTaskHandle;
TaskHandle_t uartTaskHandle;

// Prototipos de funciones de tarea
void blinkTask(void *parameter);
void uartTask(void *parameter);

void setup() {
  Serial.begin(115200);

  // Crear la tarea para parpadear el LED
  xTaskCreatePinnedToCore(
    blinkTask,
    "BlinkTask",
    1000,  // Tamaño del stack de la tarea en palabras (aprox. 1000 bytes) -> Memoria asignada para la pila de la tarea
    NULL,
    1,
    &blinkTaskHandle,
    APP_CPU_NUM  // Ejecutar en el núcleo de aplicación (core 1)
  );

  // Crear la tarea para enviar "hola mundo" por UART
  xTaskCreatePinnedToCore(
    uartTask,
    "UARTTask",
    1000,  // Tamaño del stack de la tarea en palabras (aprox. 1000 bytes)
    NULL,
    1,
    &uartTaskHandle,
    APP_CPU_NUM  // Ejecutar en el núcleo de aplicación (core 1)
  );
}

void loop() {
 
}

// Tarea para parpadear el LED cada 200 ms
void blinkTask(void *parameter) {
  (void) parameter;
  
  pinMode(LED_PIN, OUTPUT);

  while (1) {
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(100));  // Encender el LED durante 100 ms
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(100));  // Apagar el LED durante 100 ms
  }
}

// Tarea para enviar "hola mundo" por UART cada segundo
void uartTask(void *parameter) {
  (void) parameter;

  Serial.begin(115200);  // Iniciar la comunicación UART
  
  while (1) {
    Serial.println("hola mundo");
    vTaskDelay(pdMS_TO_TICKS(1000));  // Esperar 1 segundo
  }
}

