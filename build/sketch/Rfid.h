#line 1 "c:\\Users\\shike\\Desktop\\客户资料\\4-14lumen 智能门锁\\ArduinoLock\\Rfid.h"
#ifndef RFID_H
#define RFID_H

#include "include.h"
void RFID_init();
String RFID_scan();
char RFID_verify(String uid);
void RFID_add();
void RFID_clear();
void dump_byte_array(byte *buffer, byte bufferSize);
void dump_byte_array_dec(byte *buffer, byte bufferSize);
#endif // RFID_H