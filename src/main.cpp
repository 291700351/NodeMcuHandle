/*
 * @Author: Ice Lee 
 * @Date: 2019-01-20 17:07:01 
 * @Last Modified by: Ice Lee
 * @Last Modified time: 2019-01-20 18:24:48
 * @File Desa: 控制器手柄
 */
#include <Arduino.h>
#include <ESP8266WiFi.h>

//定义按键引脚
#define BTN_LEFT D5
#define BTN_UP D6
#define BTN_RIGHT D7
#define BTN_DOWN D8

// 定义小车的WiFi
// #define SSID "ShaFei"
#define SSID "ESP8266-CAR"
#define PASSWORD "99233123"
#define SERVER_IP "192.168.1.1"
#define SERVER_PORT 8888
// 发送的控制命令
unsigned char CMD_STOP[] = {0XFF, 0x01, 0x00, 0xFF};
unsigned char CMD_LEFT[] = {0XFF, 0x01, 0x01, 0xFF};
unsigned char CMD_FORWARD[] = {0XFF, 0x01, 0x02, 0xFF};
unsigned char CMD_RIGHT[] = {0XFF, 0x01, 0x03, 0xFF};
unsigned char CMD_BACKWARD[] = {0XFF, 0x01, 0x04, 0xFF};

//判断是否有按键按下
boolean hasKeyPressed = false;
// 按住的按键
unsigned char pressedCode = 0;

WiFiClient client;
// 按键扫描
unsigned char keyScan();
// TCP服务器连接
void reconnect();
// 发送数据到TCP服务器
void sendData(unsigned char data[4]);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setDebugOutput(true);
  // 初始化按键引脚
  pinMode(BTN_LEFT, INPUT);
  pinMode(BTN_UP, INPUT);
  pinMode(BTN_RIGHT, INPUT);
  pinMode(BTN_DOWN, INPUT);
  digitalWrite(BTN_LEFT, LOW);
  digitalWrite(BTN_UP, LOW);
  digitalWrite(BTN_RIGHT, LOW);
  digitalWrite(BTN_DOWN, LOW);
  //连接小车WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP());
}

void loop()
{
  // put your main code here, to run repeatedly:
  // 连接小车TCP服务器
  reconnect();
  // 键盘扫描
  unsigned char keyCode = keyScan(); 
  if (hasKeyPressed && keyCode != pressedCode)
  {
    pressedCode = keyCode;
    switch (keyCode)
    {
    case 1:
      Serial.print("Pressed Key Is Left...");
      sendData(CMD_LEFT);
      break;
    case 2:
      Serial.print("Pressed Key Is UP...");
      sendData(CMD_FORWARD);
      break;
    case 3:
      Serial.print("Pressed Key Is Right...");
      sendData(CMD_RIGHT);
      break;
    case 4:
      Serial.print("Pressed Key Is Down...");
      sendData(CMD_BACKWARD);
      break;
    case 0:
      Serial.print("No Key Pressed...");
      sendData(CMD_STOP);
      break;
    } 
    Serial.println();
  }
  else
  {
    if (keyCode != pressedCode)
    {
      pressedCode = keyCode;
      Serial.println("No Key Pressed...");
      sendData(CMD_STOP);
    }
  }
}

unsigned char keyScan()
{
  unsigned char result = 0;
  hasKeyPressed = false;

  int leftStatus = digitalRead(BTN_LEFT);
  if (leftStatus == HIGH)
  {
    delay(10);
    if (leftStatus == HIGH)
    {
      hasKeyPressed = true;
      result = 1;
    }
  }

  int upStatus = digitalRead(BTN_UP);
  if (upStatus == HIGH)
  {
    delay(10);
    if (upStatus == HIGH)
    {
      hasKeyPressed = true;
      result = 2;
    }
  }

  int rightStatus = digitalRead(BTN_RIGHT);
  if (rightStatus == HIGH)
  {
    delay(10);
    if (rightStatus == HIGH)
    {
      hasKeyPressed = true;
      result = 3;
    }
  }

  int downStatus = digitalRead(BTN_DOWN);
  if (downStatus == HIGH)
  {
    delay(10);
    if (downStatus == HIGH)
    {
      hasKeyPressed = true;
      result = 4;
    }
  }

  return result;
}

void reconnect()
{
  //连接TCP服务器
  while (!client.connected()) //几个非连接的异常处理
  {

    Serial.print("Try to connect to ");
    Serial.print(SERVER_IP);
    Serial.print(":");
    Serial.print(SERVER_PORT);
    if (!client.connect(SERVER_IP, SERVER_PORT))
    {
      Serial.println("connection Fail");
      //client.stop();
      delay(500);
    }
    Serial.println("\nConnect OK");
  }
}

void sendData(unsigned char data[4])
{
  for (int i = 0; i < 4; i++)
  {
    client.write(data[i]);
  }
}