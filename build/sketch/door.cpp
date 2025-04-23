#line 1 "c:\\Users\\shike\\Desktop\\客户资料\\4-14lumen 智能门锁\\ArduinoLock\\door.cpp"
#include "Door.h"

void beep(char a)
{
    for (int i = 0; i < a; i++)
    {
        digitalWrite(15, HIGH);
        delay(100);
        digitalWrite(15, LOW);
        delay(100);
    }
}
void lbeep(char a)
{
    digitalWrite(15, HIGH);
    delay(a);
    digitalWrite(15, LOW);
}

void Open_door(int a, int t)
{
    if (a)
    {
        u8g2.clearBuffer();
        u8g2.drawXBMP(41, 24, 16, 16, str0); // 开
        u8g2.drawXBMP(57, 24, 16, 16, str1); // 门
        u8g2.drawXBMP(73, 24, 16, 16, str2); // 中
        u8g2.sendBuffer();                   // 开显示
        digitalWrite(4, HIGH);
        beep(2);
        delay(t);
        digitalWrite(4, LOW);
    }
}