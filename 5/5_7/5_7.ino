#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <time.h>

const char* ssid = "Livebox6-42D0";
const char* password = "W39espWXiWvQ";

String ledState;

AsyncWebServer server(80);

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    
      ledState = getLocalTime();
    
    return ledState;
  }
  return String();
}

String getLocalTime()
{
  struct tm t;
  if (!getLocalTime(&t))
  {
    Serial.println("ERROR tiempo");
    return "";
  }
  time_t currentTime = mktime(&t);

  String s = ctime(&currentTime);
  return s;
}

String getCurrentTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return "";
  }
  char timeString[30];
  strftime(timeString, sizeof(timeString), "%A, %B %d %Y %H:%M:%S", &timeinfo);
  return String(timeString);
}

void setCustomTime(int year, int month, int day, int hour, int min, int sec) {
  struct tm customTime;
  customTime.tm_year = year - 1900;
  customTime.tm_mon = month - 1;
  customTime.tm_mday = day;
  customTime.tm_hour = hour;
  customTime.tm_min = min;
  customTime.tm_sec = sec;
  customTime.tm_isdst = 0;

  if (customTime.tm_year < (2016 - 1900)) {
    Serial.println("Error: custom year is too early");
    return;
  }

  time_t customTime_t = mktime(&customTime);
  struct timeval customTimeval = { customTime_t, 0 };
  settimeofday(&customTimeval, nullptr);
}

void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  // Configurar la hora utilizando un servidor NTP
  configTime(0, 0, "pool.ntp.org");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String currentTime = getCurrentTime();
    request->send(SPIFFS, "/index.html", "text/html", false, [currentTime](const String& var) {
      if (var == "TIME") {
        return currentTime;
      }
      return String();
    });
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    setCustomTime(2024, 1, 1, 0, 0, 0);
    Serial.println("Reiniciando la hora");
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.begin();
}

void loop() {
  // Manejar otras tareas
}
