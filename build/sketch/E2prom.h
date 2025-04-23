#line 1 "c:\\Users\\shike\\Desktop\\客户资料\\4-14lumen 智能门锁\\ArduinoLock\\E2prom.h"
#ifndef E2PROM_H
#define E2PROM_H

#include <EEPROM.h>
#include <Arduino.h>
extern String Auth;
extern String Ssid;
extern String Pswd;
extern char auth[30];
extern char ssid[30];
extern char pswd[30];
String get_string(int a);
void set_string(int a, String str);
void EEPROM_Config();

#endif // E2PROM_H