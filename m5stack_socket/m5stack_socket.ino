#include <M5Stack.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ArduinoWebsockets.h>
#include "config.h"

const int delay_mill = 100;
using namespace websockets;
WebsocketsClient client;

bool connected = false;

void setup() {
  //M5Stackを初期化
  M5.begin();
  M5.Power.begin();

  //Wi-Fi接続
  WiFi.begin(ssid,pass);
  M5.Lcd.printf("Waiting connect to WiFi: %s ", ssid);
  while(WiFi.status() != WL_CONNECTED) {
    //接続完了まで待つ
    delay(1000);
    M5.Lcd.print(".");
  }
  //接続完了したらIPアドレスとMacアドレスを表示する
  M5.Lcd.println("\nWiFi connected");
  M5.Lcd.print("IP address: ");
  M5.Lcd.println(WiFi.localIP());
  M5.Lcd.print("MAC address:");
  M5.Lcd.println(WiFi.macAddress());
  delay(500);

  client.onEvent(onEventCallback);
}


void loop() {

  M5.update();
  if(M5.BtnA.wasPressed()){
    if(connected){
      client.send("a");
      M5.Lcd.println("Send text");
    }else{
      M5.Lcd.println("Web Socket Not Connected");
    }
  } 

  if(M5.BtnC.wasPressed()){
    if(!connected){
    connected = client.connect(server_host, server_port, "/");
      if(connected){
        M5.Lcd.println("Web Socket Connected");
      }else{
        M5.Lcd.println("Web Socket Not Connected");
      }
    }
  }

  client.poll();
  delay(delay_mill);
}



void onEventCallback(WebsocketsEvent event, String data){
  if(event == WebsocketsEvent::ConnectionOpened){
    M5.Lcd.println("Websocket Opened");
  }else if(event == WebsocketsEvent::ConnectionClosed){
    M5.Lcd.println("Websocket Closed");
    connected = false;
  }else if(event == WebsocketsEvent::GotPing){
    M5.Lcd.println("Websocket Ping");
  }else if(event == WebsocketsEvent::GotPong){
    M5.Lcd.println("Websocket Pong");
  }

}
