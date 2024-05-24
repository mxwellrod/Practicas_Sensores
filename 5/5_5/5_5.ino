#include "WiFi.h"
#include <WiFiClient.h>
#include <time.h>

const char* ssid = "Livebox6-42D0"; //"Redmi9T"
const char* password = "W39espWXiWvQ"; //"12345678"
const char* serverAddress = "192.168.1.16";//"10.197.74.52"
const int serverPort = 1500;

bool sendTime = false;

void setup() {
  Serial.begin(115200);
  delay(10);
  
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

  configTime(3600, 3600, "pool.ntp.org");
}

void loop() {
  WiFiClient client;

  if (!client.connect(serverAddress, serverPort)) {
    Serial.println("Error al conectar al servidor");
    delay(1000);
    return;
  }

  // Hacer la solicitud GET
  client.print("GET /ruta-del-endpoint HTTP/1.1\r\n");
  client.print("Host: ");
  client.print(serverAddress);
  client.print("\r\n");
  client.print("Connection: close\r\n\r\n");
  Serial.println("Solicitud enviada al servidor");

  while (client.connected()) {
    if (client.available()) {
      String line = client.readStringUntil('\n');

      if (line.startsWith("Start")) {
        sendTime = true;
        Serial.println("Comenzar envío de hora");
      } else if (line.startsWith("Stop")) {
        sendTime = false;
        Serial.println("Detener envío de hora");
      }
    }

    if (sendTime) {
      // Obtener la hora actual
      time_t now = time(nullptr);
      struct tm* timeinfo = localtime(&now);

      char strftime_buf[64];
      strftime(strftime_buf, sizeof(strftime_buf), "%A, %B %d %Y %H:%M:%S", timeinfo);
      Serial.println(strftime_buf);

      // Enviar la hora al servidor cada segundo
      client.print(strftime_buf);
      client.print("\r\n");
      delay(1000);
    }
  }

  client.stop();
  Serial.println("Conexión cerrada");

  delay(1000); // Esperar antes de enviar la próxima solicitud
}

