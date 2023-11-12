#include <Arduino.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include "DHT.h"
const char *ssid = "esp8266";
const char *pass = "123456789";
WebSocketsServer webSocket = WebSocketsServer(81);
unsigned long t_tick = 0;
StaticJsonDocument<500> TempDoc;

#define DHTPIN D1
#define rds D0
#define ssp A0
#define IN1 D5
#define IN2 D6
#define IN3 D7
#define IN4 D8
#define li D2
const int DHTTYPE = DHT11;  
DHT dht(DHTPIN, DHTTYPE);


void setup()
{
  Serial.begin(9600);
  dht.begin();
  pinMode(rds, INPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(li, OUTPUT);
  pinMode(rds, INPUT);
  TempDoc["rain"] = 1;
  TempDoc["tempC"] = 0;
  TempDoc["humi"] = 0;
  Serial.println("Connecting to wifi");
  IPAddress apIP(192, 168, 99, 100);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, pass);
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("Websocket is started");
  randomSeed(analogRead(0));
}
bool mode = 0;
bool onOff = 0;
double humi;
double tempC;
int rain;
// int pp = 100;
// int ps = -100;
//void motor_1_Dung() {
//  digitalWrite(IN1, LOW);
//  digitalWrite(IN2, LOW);
//}
//
//void motor_2_Dung() {
//  digitalWrite(IN3, LOW);
//  digitalWrite(IN4, LOW);
//}
//
//void motor_1_Tien(int speed) {                     //speed: từ 0 - MAX_SPEED
//  speed = constrain(speed, MIN_SPEED, MAX_SPEED);  //đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
//  digitalWrite(IN1, HIGH);                         // chân này không có PWM
//  analogWrite(IN2, 255 - speed);
//}
//
//void motor_1_Lui(int speed) {
//  speed = constrain(speed, MIN_SPEED, MAX_SPEED);  //đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
//  digitalWrite(IN1, LOW);                          // chân này không có PWM
//  analogWrite(IN2, speed);
//}
//
//void motor_2_Tien(int speed) {                     //speed: từ 0 - MAX_SPEED
//  speed = constrain(speed, MIN_SPEED, MAX_SPEED);  //đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
//  digitalWrite(IN3, HIGH);                         // chân này không có PWM
//  analogWrite(IN4, 255 - speed);
//}
//
//void motor_2_Lui(int speed) {
//  speed = constrain(speed, MIN_SPEED, MAX_SPEED);  //đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
//  digitalWrite(IN3, LOW);                          // chân này không có PWM
//  analogWrite(IN4, speed);
//}

void tien(int s){
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(s);
    digitalWrite(IN3, LOW);
}

void lui(int s){
    digitalWrite(IN4, HIGH);
    digitalWrite(IN3, LOW);
    delay(s);
    digitalWrite(IN4, LOW);
}

void loop()
{
  webSocket.loop();
  if (millis() - t_tick > 1000)
  {
    rain = digitalRead(rds); tempC = dht.readTemperature();  humi = dht.readHumidity() ;
    if (onOff){ if (rain == 1){ if (mode == 0){
//          motor_2_Dung();
//          motor_1_Lui(MAX_SPEED);delay(5000);
//          motor_1_Dung();
//          for (int i=0; i<2500; i++){OneStep(false);delay(2);}
          lui(4500);
          digitalWrite(li, LOW);
          mode = 1;
        }
      }
      else{ if (mode == 1){
//          for (int i=0; i<2500; i++){OneStep(true);delay(2);}
//          motor_1_Tien(MAX_SPEED);
//          delay(3500);
//          motor_2_Lui(MAX_SPEED);
//          motor_1_Dung();
            tien(3800);
            digitalWrite(li, HIGH);
            mode = 0;
        }
      }
    }
    else{ if (mode){
        mode = 0;   
//        for (int i=0; i<2500; i++){OneStep(1);delay(2);}
//        motor_1_Tien(MAX_SPEED);delay(3500);
//        motor_2_Dung();motor_1_Dung();
        tien(3800);
        digitalWrite(li, LOW);
        mode = 0;
      }
    }
    Serial.print("Temperature: ");Serial.print(tempC);
    Serial.print("°C Humidity: ");Serial.print(humi);Serial.println("%");
    Serial.print("rain: ");Serial.print(rain);
    rainEvent();
    t_tick = millis();
  }
}
//void OneStep(bool dir)
//{
//  if (dir)
//  {
//    switch (step_number)
//    {
//    case 0:
//      digitalWrite(STEPPER_PIN_1, HIGH);
//      digitalWrite(STEPPER_PIN_2, LOW);
//      digitalWrite(STEPPER_PIN_3, LOW);
//      digitalWrite(STEPPER_PIN_4, LOW);
//      break;
//    case 1:
//      digitalWrite(STEPPER_PIN_1, LOW);
//      digitalWrite(STEPPER_PIN_2, HIGH);
//      digitalWrite(STEPPER_PIN_3, LOW);
//      digitalWrite(STEPPER_PIN_4, LOW);
//      break;
//    case 2:
//      digitalWrite(STEPPER_PIN_1, LOW);
//      digitalWrite(STEPPER_PIN_2, LOW);
//      digitalWrite(STEPPER_PIN_3, HIGH);
//      digitalWrite(STEPPER_PIN_4, LOW);
//      break;
//    case 3:
//      digitalWrite(STEPPER_PIN_1, LOW);
//      digitalWrite(STEPPER_PIN_2, LOW);
//      digitalWrite(STEPPER_PIN_3, LOW);
//      digitalWrite(STEPPER_PIN_4, HIGH);
//      break;
//    }
//  }
//  else
//  {
//    switch (step_number)
//    {
//    case 0:
//      digitalWrite(STEPPER_PIN_1, LOW);
//      digitalWrite(STEPPER_PIN_2, LOW);
//      digitalWrite(STEPPER_PIN_3, LOW);
//      digitalWrite(STEPPER_PIN_4, HIGH);
//      break;
//    case 1:
//      digitalWrite(STEPPER_PIN_1, LOW);
//      digitalWrite(STEPPER_PIN_2, LOW);
//      digitalWrite(STEPPER_PIN_3, HIGH);
//      digitalWrite(STEPPER_PIN_4, LOW);
//      break;
//    case 2:
//      digitalWrite(STEPPER_PIN_1, LOW);
//      digitalWrite(STEPPER_PIN_2, HIGH);
//      digitalWrite(STEPPER_PIN_3, LOW);
//      digitalWrite(STEPPER_PIN_4, LOW);
//      break;
//    case 3:
//      digitalWrite(STEPPER_PIN_1, HIGH);
//      digitalWrite(STEPPER_PIN_2, LOW);
//      digitalWrite(STEPPER_PIN_3, LOW);
//      digitalWrite(STEPPER_PIN_4, LOW);
//    }
//  }
//  step_number++;
//  if (step_number > 3)
//  {
//    step_number = 0;
//  }
//}
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
void rainEvent()
{
  TempDoc["rain"] = rain;
  TempDoc["humi"] = humi;
  TempDoc["tempC"] = tempC;
  char msg[256];
  serializeJson(TempDoc, msg);
  webSocket.broadcastTXT(msg);
}