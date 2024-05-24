#include "WiFi.h"
#include <WiFiClient.h>
#include <time.h> // Incluir la librería time.h para manejar el tiempo

const char* ssid = "Redmi9T";       // Nombre de la red WiFi
const char* password = "12345678";   // Contraseña de la red WiFi
const char* serverAddress = "10.197.74.52"; //10.1.65.125
const int serverPort = 1500;         // Puerto en el que está escuchando el servidor

void setup() {
  Serial.begin(115200);
  delay(10);
  
  // Conexión a la red WiFi
  Serial.println();
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conexión WiFi establecida");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Inicializar la configuración de tiempo
  configTime(0, 0, "pool.ntp.org"); // Configurar el servidor NTP para obtener la hora actual
}

void loop() {
  // Obtener la hora actual
  time_t now = time(nullptr); // Obtener la hora actual en formato de tiempo UNIX

  struct tm* timeinfo;
  timeinfo = localtime(&now); // Convertir el tiempo UNIX a estructura de tiempo local

  // Imprimir la fecha y hora actual formateada
  char strftime_buf[64];
  strftime(strftime_buf, sizeof(strftime_buf), "%A, %B %d %Y %H:%M:%S", timeinfo);
  Serial.println(strftime_buf);

  // Establecer la conexión al servidor
  WiFiClient client;
  if (!client.connect(serverAddress, serverPort)) {
    Serial.println("Error al conectar al servidor");
    return;
  }

  // Hacer la solicitud GET
  client.print("GET /ruta-del-endpoint HTTP/1.1\r\n");
  client.print("Host: ");
  client.print(serverAddress);
  client.print("\r\n");
  client.print("Connection: close\r\n\r\n");
  
  Serial.println("Solicitud enviada al servidor");

  // Leer la respuesta del servidor
  while (client.connected()) {
    String line = client.readStringUntil('\n'); // si esta vacio el string que ni printee PONER
    Serial.println(line);
  }

  // Cerrar la conexión
  client.stop();
  Serial.println("Conexión cerrada");

  delay(1000); // Esperar antes de enviar la próxima solicitud
}

