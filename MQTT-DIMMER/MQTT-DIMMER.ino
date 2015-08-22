#define MQTT_DEBUG_MODE
#define WIFI_DEBUG_MODE

#include <WiFiConnector.h>
#include <MqttConnector.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

MqttConnector *mqtt;
WiFiConnector *wifi;

// MQTT_HOSTT
#define MQTT_HOST        "192.168.15.148"
#define MQTT_PORT        1883
#define PUBLISH_EVERY    15 *1000 // every 15 seconds

#define DEVICE_NAME "MQTT-DIMMER"
#define AUTHOR      "Compiler Exe"
#define BOARD       "ESP8266"
#define PROJECT     "DIMMER"


const byte switchPin = 14;
const byte zcPin = 5;
const byte outPin = 4;


byte fade = 0;
byte state = 0;
byte tarBrightness = 255;
byte curBrightness = 0;
byte zcState = 0; // 0 = ready; 1 = processing; 2 = double trigger fix


#include "init_wifi.h"
#include "_publish.h"
#include "_receive.h"
#include "init_mqtt.h"

void init_hardware()
{
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(zcPin, INPUT_PULLUP);
  pinMode(outPin, OUTPUT);

  digitalWrite(outPin, 0);
  Serial.begin(115200);
  delay(10);


  digitalWrite(outPin, 0);
  Serial.println();
  Serial.println("BEGIN");


}

void setup()
{
  init_hardware();
  init_wifi();
  init_mqtt();
}

void loop()
{
  mqtt->loop(wifi);
}


void zcDetect()
{
  if (tarBrightness < 128 && tarBrightness > 1) {
    int x = map(tarBrightness, 100, 0, 1, 128);
    int dimtime = (75 * x);  // For 60Hz =>65
    Serial.println(x);
    delayMicroseconds(dimtime);    // Wait till firing the TRIAC
    digitalWrite(outPin, HIGH);   // Fire the TRIAC
    delayMicroseconds(5);         // triac On propogation delay (for 60Hz use 8.33)
    digitalWrite(outPin, LOW);
  }
   
}

