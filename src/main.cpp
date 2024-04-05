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
const char* ssid = "UNIFI_IDO1";
const char* password = "41Bidules!";
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
String loadData(const String& data){
  if (data == "TEMPERATURE_VALUE")
    return String(dht22Temp.getData());
  if (data == "TEMPERATURE_MESSAGE"){
    if (!dht22Temp.verificationNormalRange(-25, 30))
      return"Anormal";
    else
      return"Normal";
  }
  if (data == "HUMIDITY_VALUE")
    return String(dht22Hum.getData());
  if (data == "HUMIDITY_MESSAGE"){
    if (!dht22Hum.verificationNormalRange(25, 80))
      return"Anormal";
    else
      return"Normal";
  }
  if (data == "GAS_VALUE")
    return String(gas.getData());
  if (data == "GAS_MESSAGE"){
    if (!gas.verificationNormalRange(250, 450))
      return"Anormal";
    else
      return"Normal";
  }
  if (data == "ALARM_VALUE")
    if (!gas.verificationNormalRange(400, 450) || !dht22Temp.verificationNormalRange(-25, 30) || !dht22Hum.verificationNormalRange(30, 80))
      return "Desactiver";
    else
      return "Activer";
  return String();
}
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
  dht.begin();
  server.begin();
  pinMode(4, OUTPUT);
  // collect data
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gasVal = analogRead(gas.getPin());
  dht22Temp.setData(t);
  dht22Hum.setData(h);
  gas.setData(gasVal/10);
  // implement routes server
  server.onNotFound([](AsyncWebServerRequest *request){
    String path = request->url();
    if(path.endsWith("/")){
      path += "index.html";
    }
    request->send(SPIFFS, path, getContentType(path), false, loadData);
    if (!gas.verificationNormalRange(250, 450) || !dht22Temp.verificationNormalRange(-25, 30) || !dht22Hum.verificationNormalRange(30, 80))
    {
      digitalWrite(4, HIGH);
      // for (int thisNote = 0; thisNote < 1; thisNote  ) {
      //   int noteDuration = 800/noteDurations[thisNote];
      tone(33, 1000);
      //   int pauseBetweenNotes = noteDuration * 1.30;
      //   delay(pauseBetweenNotes);
      // }
    }else{
      noTone(33);
      digitalWrite(4, LOW);
    }
    
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