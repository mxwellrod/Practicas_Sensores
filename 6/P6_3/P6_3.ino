#include <Arduino.h>
#include <esp_sleep.h>
#include <esp_freertos_hooks.h>
// Definir constantes de pines
#define LED_PIN 2  // Pin del LED (cambiar según la placa)

// Declaración de funciones de las tareas
void sensorTask(void *parameter);
void uartTask(void *parameter);

// Variables globales
volatile int sensorValue = 0;
volatile bool newDataAvailable = false;

bool myIdleHook(void) {
  // Poner el microcontrolador en modo de bajo consumo (sleep)
  esp_sleep_enable_timer_wakeup(1000 * 1000); // Configurar el temporizador para despertar después de 1 segundo
  esp_light_sleep_start(); // Entrar en modo de bajo consumo (light sleep)
  return true;
}

void setup() {
  Serial.begin(115200); // Iniciar la comunicación UART
  pinMode(LED_PIN, OUTPUT); // Configurar el pin del LED como salida

  //esp_register_freertos_idle_hook(&vApplicationIdleHook)
  esp_register_freertos_idle_hook(reinterpret_cast<esp_freertos_idle_cb_t>(&myIdleHook));
  // Crear la tarea para la simulación del sensor (lectura cada 100 ms)
  xTaskCreate(sensorTask, "SensorTask", 1000, NULL, 1, NULL);

  // Crear la tarea para enviar datos vía UART
  xTaskCreate(uartTask, "UARTTask", 1000, NULL, 2, NULL);
}

void loop() {

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
