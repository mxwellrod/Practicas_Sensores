#include <WiFi.h>
#include <WiFiClient.h>

const char* ssid = "Livebox6-42D0";
const char* password = "W39espWXiWvQ";
const char* serverAddress = "192.168.1.111"; // Dirección IP del servidor TCP: habia .16 y con ipconfig he visto que esa no era la ip del pc.
const int serverPort = 2000;

// Simular valores de acelerómetro (por ejemplo, valores en los ejes X, Y, Z)
float accelerometerX = 0.0;
float accelerometerY = 0.0;
float accelerometerZ = 0.0;

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.println("Conectando al WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Conexión WiFi establecida");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client;

  if (!client.connect(serverAddress, serverPort)) {
    Serial.println("Error al conectar al servidor TCP");
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

  Serial.println("Conexión establecida con el servidor TCP");

  while (client.connected()) {
    // Simular lectura de valores de acelerómetro (valores aleatorios para demostración)
    accelerometerX = random(-1000, 1000) / 1000.0;  // Valores en el rango -1.0 a 1.0
    accelerometerY = random(-1000, 1000) / 1000.0;
    accelerometerZ = random(-1000, 1000) / 1000.0;

    // Crear un mensaje con los valores del acelerómetro
    String message = "Acelerómetro - X: " + String(accelerometerX, 4) +
                     ", Y: " + String(accelerometerY, 4) +
                     ", Z: " + String(accelerometerZ, 4);

    // Enviar el mensaje al servidor
    client.println(message);

    Serial.println("Datos de acelerómetro enviados al servidor");

    // Esperar antes de enviar el próximo conjunto de datos
    delay(1000); // Intervalo de 1 segundo entre envíos
  }

  Serial.println("Conexión perdida con el servidor TCP");
  client.stop();

  // Esperar antes de intentar reconectarse
  delay(5000); // Esperar 5 segundos antes de intentar reconectar
}

