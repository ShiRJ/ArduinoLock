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