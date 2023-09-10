#include <Arduino.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include "DHT.h"
const char *ssid = "esp8266";
const char *pass = "123456789";
WebSocketsServer webSocket = WebSocketsServer(81);
unsigned long t_tick = 0;
StaticJsonDocument<500> TempDoc;
#define DHTPIN D2
#define STEPPER_PIN_1 D5
#define STEPPER_PIN_2 D6
#define STEPPER_PIN_3 D7
#define STEPPER_PIN_4 D8
int step_number = 0;
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
double humi;
double tempC;
void setup()
{
  Serial.begin(9600);
  dht.begin();
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
  TempDoc["tempC"] = 0;
  TempDoc["humi"] = 0;
  Serial.println("Connecting to wifi");
  IPAddress apIP(192, 168, 99, 100);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, pass);
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("Websocket is started");
}
bool mode = 0;
bool onOff = 0;
void loop()
{
  webSocket.loop();
  if (millis() - t_tick > 2000)
  {
    humi = dht.readHumidity();
    tempC = dht.readTemperature();
    if (isnan(tempC) || isnan(humi))
    {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    if (onOff)
    {

      if (humi < 55)
      {
        if (!mode)
        {
          for (int i = 0; i < 2000; i++)
          {
            OneStep(1);
            delay(2);
          }
          mode = 1;
        }
      }
      else
      {
        if (mode)
        {
          for (int i = 0; i < 2000; i++)
          {
            OneStep(0);
            delay(2);
          }
          mode = 0;
        }
      }
    }
    else
    {

      if (mode)
      {
        for (int i = 0; i < 2000; i++)
        {
          OneStep(0);
          delay(2);
        }
        mode = 0;
      }
    }

    // if (onOff){
    //   if (humi > 55){
    //     if (mode == 0){
    //       for (int i=0; i<2000; i++){
    //         OneStep(0);
    //         delay(2);
    //       }
    //       mode = 1;
    //     }
    //   }
    //   else{
    //     if (mode == 1){
    //       for (int i=0; i<2000; i++){
    //         OneStep(1);
    //         delay(2);
    //       }
    //       mode = 0;
    //     }
    //   }
    // }
    // else{
    //   if (mode){
    //     mode = 0;
    // for (int i=0; i<2000; i++){
    //   OneStep(1);
    //   delay(2);
    // }
    //     mode = 0;
    //   }
    // }
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C Humidity: ");
    Serial.print(humi);
    Serial.println("%");
    dhtEvent();
    t_tick = millis();
  }
}
void OneStep(bool dir)
{
  if (dir)
  {
    switch (step_number)
    {
    case 0:
      digitalWrite(STEPPER_PIN_1, HIGH);
      digitalWrite(STEPPER_PIN_2, LOW);
      digitalWrite(STEPPER_PIN_3, LOW);
      digitalWrite(STEPPER_PIN_4, LOW);
      break;
    case 1:
      digitalWrite(STEPPER_PIN_1, LOW);
      digitalWrite(STEPPER_PIN_2, HIGH);
      digitalWrite(STEPPER_PIN_3, LOW);
      digitalWrite(STEPPER_PIN_4, LOW);
      break;
    case 2:
      digitalWrite(STEPPER_PIN_1, LOW);
      digitalWrite(STEPPER_PIN_2, LOW);
      digitalWrite(STEPPER_PIN_3, HIGH);
      digitalWrite(STEPPER_PIN_4, LOW);
      break;
    case 3:
      digitalWrite(STEPPER_PIN_1, LOW);
      digitalWrite(STEPPER_PIN_2, LOW);
      digitalWrite(STEPPER_PIN_3, LOW);
      digitalWrite(STEPPER_PIN_4, HIGH);
      break;
    }
  }
  else
  {
    switch (step_number)
    {
    case 0:
      digitalWrite(STEPPER_PIN_1, LOW);
      digitalWrite(STEPPER_PIN_2, LOW);
      digitalWrite(STEPPER_PIN_3, LOW);
      digitalWrite(STEPPER_PIN_4, HIGH);
      break;
    case 1:
      digitalWrite(STEPPER_PIN_1, LOW);
      digitalWrite(STEPPER_PIN_2, LOW);
      digitalWrite(STEPPER_PIN_3, HIGH);
      digitalWrite(STEPPER_PIN_4, LOW);
      break;
    case 2:
      digitalWrite(STEPPER_PIN_1, LOW);
      digitalWrite(STEPPER_PIN_2, HIGH);
      digitalWrite(STEPPER_PIN_3, LOW);
      digitalWrite(STEPPER_PIN_4, LOW);
      break;
    case 3:
      digitalWrite(STEPPER_PIN_1, HIGH);
      digitalWrite(STEPPER_PIN_2, LOW);
      digitalWrite(STEPPER_PIN_3, LOW);
      digitalWrite(STEPPER_PIN_4, LOW);
    }
  }
  step_number++;
  if (step_number > 3)
  {
    step_number = 0;
  }
}
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  String cmd = "";
  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.println("Websocket is disconnected");
    break;
  case WStype_CONNECTED:
  {
    Serial.println("Websocket is connected");
    Serial.println(webSocket.remoteIP(num).toString());
    webSocket.sendTXT(num, "connected");
  }
  break;
  case WStype_TEXT:
    cmd = "";
    for (int i = 0; i < length; i++)
    {
      cmd = cmd + (char)payload[i];
    }
    Serial.print("Data from flutter:");
    Serial.println(cmd);
    if (cmd == "on")
    {
      onOff = 1;
    }
    else if (cmd == "off")
    {
      onOff = 0;
    }
    break;
  case WStype_FRAGMENT_TEXT_START:
    break;
  case WStype_FRAGMENT_BIN_START:
    break;
  case WStype_BIN:
    hexdump(payload, length);
    break;
  default:
    break;
  }
}
void dhtEvent()
{
  TempDoc["humi"] = humi;
  TempDoc["tempC"] = tempC;
  char msg[256];
  serializeJson(TempDoc, msg);
  webSocket.broadcastTXT(msg);
}