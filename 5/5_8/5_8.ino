#include "ArduinoJson.h"
#include <SPIFFS.h>
#include <time.h>

const char* fileName = "/sensor_data.json"; // Nombre del archivo JSON
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 0; //hasta fin de mes
int cont = 0;
int des = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Inicio del todo ");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}

void loop() {

  if(cont < 10){
    // Obtener la marca de tiempo actual en formato ISO 8601
    char currentTime[30];
    getCurrentTime().toCharArray(currentTime, 30);
    // Crear objeto JSON usando ArduinoJson
    DynamicJsonDocument doc(512);
    doc["time"] = currentTime; // Marca de tiempo
    doc["e"] = random(10, 40); // Valor de temperatura
    doc["u"] = "Cel"; // Unidad de temperatura (Celsius)

    // Serializar el objeto JSON en una cadena
    String jsonStr;
    serializeJson(doc, jsonStr);
    Serial.println("Serializar okay okay ");
    // Guardar la cadena JSON en el archivo
    File file = SPIFFS.open(fileName, FILE_WRITE);
    if (!file) {
      Serial.println("Failed to open file for writing");
      return;
    }
    file.println(jsonStr);
    file.close();

    Serial.println("JSON file saved");

    delay(10000); // Esperar 10 segundos antes de generar el próximo JSON
    cont++;

    if(cont == 10){des = 1;}
    
  }

  if(des == 1){
    File file = SPIFFS.open("/sensor_data.json", "r");
    if (!file) {
        Serial.println("Error al abrir el archivo sensor_data.json");
        return;
    }

    String jsonContent = "";
    while (file.available()) {
        char c = file.read();
        jsonContent += c;
    }

    // deserealizacion a ver que ha sacado xd
    DynamicJsonDocument doc(1024); // Tamaño máximo esperado del documento JSON
    DeserializationError error = deserializeJson(doc, jsonContent);
    if (error) {
        Serial.println("Error al parsear JSON");
        return;
    }

    // Acceder a los valores del documento JSON
    const char* timeValue = doc["time"];
    Serial.print("Tiempo: ");
    Serial.println(timeValue);
    int temperature = doc["temperature"];
    Serial.print("Temperatura: ");
    Serial.println(temperature);
    const char* unit = doc["unit"];
    Serial.print("Unidad temp: ");
    Serial.println(unit);
    des = 0;
  }
}

String getCurrentTime() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  char timeString[30];
  strftime(timeString, sizeof(timeString), "%Y-%m-%dT%H:%M:%SZ", timeinfo);
  return String(timeString);
}