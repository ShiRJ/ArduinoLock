#include "E2prom.h"
String Auth = "5d2befcadda6";
String Ssid = "Pura 70";
String Pswd = "12345678";
// 获取指定EEPROM位置的字符串，从EEPROM的a位开始读取
String get_string(int a)
{
    char b = EEPROM.read(a); // 获取字符串长度
    if (b > 29)
        b = 29; // 如果字符串长度大于29，则将其截断
    else if (b < 0)
        b = 0; // 如果字符串长度小于0，则将其设为0
    else if (b == 0)
        return ""; // 如果字符串长度为0，则返回空字符串
    String data = "";
    for (int i = 1; i <= b; i++)
    {
        data += char(EEPROM.read(a + i));
    }
    Serial.print("Successfully read string: ");
    Serial.println(data);
    return data;
}
// 用EEPROM的a位保存字符串的长度，字符串的从EEPROM的b位开始保存，str为要保存的字符串
void set_string(int a, String str)
{
    int s = str.length(); // 获取字符串长度
    if (s > 29)
        s = 29; // 如果字符串长度大于29，则将其截断
    else if (s > 0)
        EEPROM.write(a, s); // EEPROM第a位，写入str字符串的长度
    for (int i = 0; i < s; i++)
    {
        EEPROM.write(a + 1 + i, str[i]);
    }
    EEPROM.commit(); // 执行保存EEPROM
}
void EEPROM_Config()
{
    /********************************************************
     * EEPROM address 0000-2431 is used for Blinker         *
     * EEPROM address 2560-2650 is used for User Settings   *
     * EEPROM address 2700-3000 is used for User Keys       *
     * EEPROM address 3100-3400 is used for IC Keys         *
     * EEPROM address 3500-4095 is used for Counters        *
     ********************************************************/
    EEPROM.begin(4096); // 初始化EEPROM
    // set_string(2560, Ssid);
    // set_string(2590, Pswd);
    // set_string(2620, Auth);
    strcpy(ssid, get_string(2560).c_str()); // 获取字符串
    strcpy(pswd, get_string(2590).c_str()); // 获取字符串
    strcpy(auth, get_string(2620).c_str()); // 获取字符串
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("PSWD: ");
    Serial.println(pswd);
    Serial.print("AUTH: ");
    Serial.println(auth);
}