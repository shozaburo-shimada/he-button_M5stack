#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ArduinoWebsockets.h>
#include "config.h"

const int delay_mill = 50;
using namespace websockets;
WebsocketsClient client;

bool connected = false;

const int PIN_BTN_A = 7;
const int PIN_BTN_Y = 5;
const int PIN_LED = 0;

void setup() {
  //M5Stackを初期化
  Serial.begin(115200);

  //Wi-Fi接続
  WiFi.begin(ssid,pass);
  Serial.printf("Waiting connect to WiFi: %s ", ssid);
  while(WiFi.status() != WL_CONNECTED) {
    //接続完了まで待つ
    delay(1000);
    Serial.print(".");
  }
  //接続完了したらIPアドレスとMacアドレスを表示する
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address:");
  Serial.println(WiFi.macAddress());
  delay(500);

  client.onEvent(onEventCallback);

  pinMode(PIN_BTN_A, INPUT);
  pinMode(PIN_BTN_Y, INPUT);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
}


bool btn_a = true;
bool btn_y = true;
bool pre_btn_a = true;
bool pre_btn_y = true;

void loop() {
  pre_btn_a = btn_a;
  pre_btn_y = btn_y;
  btn_a = digitalRead(PIN_BTN_A);
  btn_y = digitalRead(PIN_BTN_Y);
  if(btn_a == false && pre_btn_a == true){
    Serial.println("push A");
    if(connected){
      client.send("a");
      Serial.println("Send text");
    }else{
      Serial.println("Web Socket Not Connected");
    }
  } 

  if(btn_y == false && pre_btn_y == true){
    Serial.println("push Y");
    if(!connected){
    connected = client.connect(server_host, server_port, "/");
      if(connected){
        Serial.println("Web Socket Connected");
        digitalWrite(PIN_LED, HIGH);
      }else{
        Serial.println("Web Socket Not Connected");
      }
    }
  }

  client.poll();
  delay(delay_mill);
}



void onEventCallback(WebsocketsEvent event, String data){
  if(event == WebsocketsEvent::ConnectionOpened){
    Serial.println("Websocket Opened");
  }else if(event == WebsocketsEvent::ConnectionClosed){
    Serial.println("Websocket Closed");
    connected = false;
    digitalWrite(PIN_LED, LOW);
  }else if(event == WebsocketsEvent::GotPing){
    Serial.println("Websocket Ping");
  }else if(event == WebsocketsEvent::GotPong){
    Serial.println("Websocket Pong");
  }

}
