/*
  Temperature Sensor (ESP8266 + OneWire Digital Sensor)

  ESP8266 uses Dallas OneWire library to determine the temperature above a stove

  Created 31 August 2018
  By Ivan Sy
  Modified 3 September 2018
  By Ivan Sy

  https://github.com/isyca/esp8266-temperature-sensor
*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

ESP8266WiFiMulti WiFiMulti;

const int sensorPIN = 0;
 
const char* ssid = "SSID";
const char* password = "password";

String destSchema = "http://";
String destHostName = "192.168.0.10:8085";
String destURL = "/?accessoryId=kitchentemp&value=";

int loopTime = 30000;    // milliseconds before each loop
float temperature = 0;

OneWire oneWirePin(sensorPIN);
DallasTemperature sensors(&oneWirePin);

void setup() {
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid,password);
  sensors.begin();
}

void loop() {
  sensors.requestTemperatures(); 
  temperature = sensors.getTempCByIndex(0);
  char buff[10];
  String temperatureConverted = dtostrf(temperature,2,0,buff);
  String urlTemperature = (String)destSchema + destHostName + destURL + temperatureConverted;

  if ((WiFiMulti.run() == WL_CONNECTED)) {
    HTTPClient httpClient;
    httpClient.begin(urlTemperature);
    httpClient.GET();
    httpClient.end();

    delay(loopTime);
  }
}
