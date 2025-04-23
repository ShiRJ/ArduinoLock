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