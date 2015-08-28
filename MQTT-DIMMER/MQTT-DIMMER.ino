#define MQTT_DEBUG_MODE
#define WIFI_DEBUG_MODE

#include <WiFiConnector.h>
#include <MqttConnector.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>


MqttConnector *mqtt;
WiFiConnector *wifi;
bool mqtt_ready = true;

// long counter = 0;
long _prev_main = millis();
int direction = 1;
int target = 0;
int dimtime = 0;
int x = 0;

// MQTT_HOSTT
#define MQTT_HOST        "192.168.15.148" 
#define MQTT_PORT        1883
#define PUBLISH_EVERY    10 *1000 // every 15 seconds
#define DEVICE_NAME "MQTT-DIMMER"
#define AUTHOR      "Nat"
#define BOARD       "ESP8266"
#define PROJECT     "DIMMER"


const byte switchPin = 14;
const byte zcPin = 5;
const byte outPin = 4;


bool en = false;
byte fade = 0;
byte state = 0;
byte tarBrightness = 100;
byte curBrightness = 0;
byte zcState = 0; // 0 = ready; 1 = processing; 2 = double trigger fix

unsigned long _last_message = millis();


#include "init_wifi.h"
#include "_publish.h"
#include "_receive.h"
#include "init_mqtt.h"

void timer2() {
    digitalWrite(outPin, LOW);
}

void timer() {
  digitalWrite(outPin, HIGH); // Fire the TRIAC
  timer1_detachInterrupt();
  timer1_isr_init();
  timer1_attachInterrupt(timer2);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  timer1_write(5*10); // 5 ticks/us 
}

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

// void timer() {
//   if (millis() - _prev_main > 1000) {
//     Serial.println(millis());
//     Serial.println("TIMER 1");

//   }
//   timer1_detachInterrupt();

// }

void setup()
{
  init_hardware();
  init_wifi();
  init_mqtt();

  
}

// int counter = 0;
void loop()
{

  //   if (target >= 98) {
  //     direction = -1;
  //   }
  //   else if (target <= 2) {
  //     direction = 1;
  //   }  
  // if (millis() - _prev_main > 1000) {
  //   // Serial.println("MAIN");
  //   // _prev_main = millis();
  // }

  if (mqtt_ready) {
    mqtt->loop(wifi);
  }

}

int _prev = millis() ;
int last_dim = 0;;

unsigned long _micro = micros();
void zcDetect()
{
jj  static int count = 0;;
  int target = tarBrightness;
  if (millis() - _prev >= 5000) {
    en = true;
    Serial.print("COUNT: ");
    Serial.print(count);
    count =0;
    Serial.print("X: ");
    Serial.print(x);
    Serial.print("T: ");    
    Serial.println(dimtime);
    _prev = millis();
  }
  else {
    en = false;
  }
  count++;

  x = map(target, 100, 0, 1, 128);
  dimtime = (78 * x);  // For 60Hz =>65

  last_dim = dimtime;
    timer1_isr_init();
    timer1_attachInterrupt(timer);
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
    timer1_write( (5* dimtime)); // 5 ticks/us 


}
