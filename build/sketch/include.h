#line 1 "c:\\Users\\shike\\Desktop\\客户资料\\4-14lumen 智能门锁\\ArduinoLock\\include.h"
#ifndef INCLUDE_H
#define INCLUDE_H

#include <Arduino.h>
#define u8 signed char
#include <WiFi.h>
#include <EEPROM.h>

#include <U8g2lib.h>
#include "font.h"
#include <MFRC522.h>
#include <SPI.h>
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <Keypad.h>
#include "Finger.h"
//#include "RFID.h"
#include "Key.h"
#include "Door.h"
#include "E2prom.h"
#include "Web.h"
#include "Rfid.h"

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2; // 0.96寸oled屏幕参数(SSD1306)
extern char err;
extern char user_order; // 存储用户输入的串口指令


#endif // INCLUDE_H