#ifndef KEY_H
#define KEY_H

#include "include.h"

char get_key();
void Key_init();
String get_pswd();
int Key_verify(String str);
void Key_set();
int get_id(int checker[20]);
void Key_clear();
extern int flag;

#endif // KEY_H