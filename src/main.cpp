// import libraries
#include <Arduino.h>
#include <WiFi.h>
#include "SPIFFS.h"
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "Sensor.hpp"
#include "DHT.h"
#include <ArduinoJson.h>

#define DHTTYPE DHT22
#define NOTE_B0  31

//define variables
const char* ssid = "daniel";
const char* password = "allo1234";
Sensor dht22Temp("Temperature", 27);
Sensor dht22Hum("Humidite", 27);
Sensor gas("Monoxyde de carbonne", 32);
Sensor ledAlarm("Led Alarme", 4);
DHT dht(dht22Temp.getPin(), DHTTYPE);
int melody[] = {NOTE_B0};
int noteDurations[] = {8};
StaticJsonDocument<100> jsonData;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
String getContentType(String path);

//function for initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else {
  Serial.println("SPIFFS mounted successfully");
  }
}

//function for initialize WIFI
void initWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  initWiFi();
  initSPIFFS();
  server.begin();
  dht.begin();
  pinMode(4, OUTPUT);
  // collect data
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gasVal = analogRead(gas.getPin());
  dht22Temp.setData(t);
  dht22Hum.setData(h);
  gas.setData(gasVal/10);
  // implement routes server
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    //create json struct in string
    String buffer;
    jsonData["temperature"]["value"] = dht22Temp.getData();
    jsonData["humidity"]["value"] = dht22Hum.getData();
    jsonData["gas"]["value"] = gas.getData();
    if (!gas.verificationNormalRange(400, 450)) // verify condition range value
      jsonData["gas"]["message"] = "Anormal";
    else
      jsonData["gas"]["message"] = "Normal";

    if (!dht22Temp.verificationNormalRange(-25, 30))
      jsonData["temperature"]["message"] = "Anormal";
    else
      jsonData["temperature"]["message"] = "Normal";

    if (!dht22Hum.verificationNormalRange(30, 80))
      jsonData["humidity"]["message"] = "Anormal";
    else
      jsonData["humidity"]["message"] = "Normal";
    serializeJson(jsonData, buffer);
    request->send(200, "text/plain", buffer);
    if (!gas.verificationNormalRange(400, 450) || !dht22Temp.verificationNormalRange(-25, 30) || !dht22Temp.verificationNormalRange(30, 80))
    {
      digitalWrite(4, HIGH);
      for (int thisNote = 0; thisNote < 1; thisNote  ) {
        int noteDuration = 800/noteDurations[thisNote];
        tone(33, melody[thisNote],noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
      }
    }else{
      noTone(33);
      digitalWrite(4, LOW);
    }
  });
  server.onNotFound([](AsyncWebServerRequest *request){
    String path = request->url();
    if(path.endsWith("/")){
      path += "index.html";
    }
    request->send(SPIFFS, path);
  });
}

void loop() {}

String getContentType(String path) { // function of detect type file
  if (path.endsWith(".css")) {
    return "text/css";
  } else if (path.endsWith(".js")) {
    return "application/javascript";
  } else if (path.endsWith(".jpg") || path.endsWith(".jpeg")) {
    return "image/jpeg";
  } else if (path.endsWith(".png")) {
    return "image/png";
  } else if (path.endsWith(".html")) {
    return "text/html";
  } else if (path.endsWith(".svg")) {
    return "image/svg";
  } else if (path.endsWith(".gif")) {
    return "image/gif";
  } else {
    return "text/plain";
  }
}