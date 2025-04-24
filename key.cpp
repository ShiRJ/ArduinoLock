#include "Key.h"
const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {13, 12, 14, 27};                                              // connect to the row pinouts of the keypad
byte colPins[COLS] = {26, 25, 33, 32};                                              // connect to the column pinouts of the keypad
int pw_checker[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // 用于存储密码的数组
// Create an object of keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
String pswd0 = "";

int flag = 0; // 0:输入密码 1:输入管理员密码 2:输入新密码 3:再次输入密码
void Key_init()
{
    Serial.print("Read key_sign["); // 打印密码标识
    // EEPROM.begin(4096);
    // for (int i = 0; i < 20; i++)
    // {
    //     EEPROM.write(3500 + i, pw_checker[i]);
    // }
    // EEPROM.commit(); // 执行保存EEPROM
    EEPROM.begin(4096);
    for (int i = 0; i < 20; i++)
    {
        pw_checker[i] = EEPROM.read(3500 + i); // 获取密码标识
        Serial.print(int(pw_checker[i]));
        Serial.print(",");
    }
    Serial.println("]");
}
char get_key()
{
    char key = keypad.getKey();
    return key;
}
void screen(int a)
{
    if (a == 0)
    { // 请输入密码
        u8g2.drawXBMP(25, 18, 16, 16, str7);
        u8g2.drawXBMP(41, 18, 16, 16, str37);
        u8g2.drawXBMP(57, 18, 16, 16, str38);
        u8g2.drawXBMP(73, 18, 16, 16, str39);
        u8g2.drawXBMP(89, 18, 16, 16, str40);
    }
    else if (a == 1)
    { // 请输入管理员密码
        u8g2.drawXBMP(41, 0, 16, 16, str7);
        u8g2.drawXBMP(57, 0, 16, 16, str37);
        u8g2.drawXBMP(73, 0, 16, 16, str38);
        u8g2.drawXBMP(25, 18, 16, 16, str43);
        u8g2.drawXBMP(41, 18, 16, 16, str44);
        u8g2.drawXBMP(57, 18, 16, 16, str45);
        u8g2.drawXBMP(73, 18, 16, 16, str39);
        u8g2.drawXBMP(89, 18, 16, 16, str40);
    }
    else if (a == 2)
    { // 请输入新密码
        u8g2.drawXBMP(17, 18, 16, 16, str7);
        u8g2.drawXBMP(33, 18, 16, 16, str37);
        u8g2.drawXBMP(49, 18, 16, 16, str38);
        u8g2.drawXBMP(65, 18, 16, 16, str56);
        u8g2.drawXBMP(81, 18, 16, 16, str39);
        u8g2.drawXBMP(97, 18, 16, 16, str40);
    }
    else if (a == 3)
    { // 请再次输入
        u8g2.drawXBMP(25, 18, 16, 16, str7);
        u8g2.drawXBMP(41, 18, 16, 16, str46);
        u8g2.drawXBMP(57, 18, 16, 16, str37);
        u8g2.drawXBMP(73, 18, 16, 16, str47);
        u8g2.drawXBMP(89, 18, 16, 16, str48);
    }
}
String get_pswd()
{
    u8g2.clearBuffer();
    screen(flag);
    for (int j = 0; j < 6; j++)
    {
        u8g2.setCursor(20 + j * 16, 48);
        u8g2.print('_');
    }
    u8g2.sendBuffer(); // 开显示
    int c = 0;
    String pw = "";
    while (c < 6)
    {
        char key = keypad.getKey();
        if (key != NO_KEY && key != '#' && key != '*')
        {
            pw += key; // 将按下的键添加到密码字符串中
            beep(1);   // 蜂鸣器响
            u8g2.clearBuffer();
            screen(flag);
            for (int j = 0; j < c; j++)
            {
                u8g2.setCursor(20 + j * 16, 48);
                u8g2.print('*');
            }
            for (int j = c + 1; j < 6; j++)
            {
                u8g2.setCursor(20 + j * 16, 48);
                u8g2.print('_');
            }
            u8g2.setCursor(20 + c * 16, 48);
            u8g2.print(key);   // 显示按下的键
            u8g2.sendBuffer(); // 开显示
            c++;
            continue; // 继续循环
        }
        if (key == '*')
        {
            // pw[c] = '';
            //  删除密码字符串的最后一个字符
            if (c > 0)
            {
                c--;          // 密码长度减1
                pw.remove(c); // 删除密码字符串的最后一个字符
            }
            beep(1); // 蜂鸣器响
            if (c == 0)
            {
                pw = ""; // 清空密码字符串
                u8g2.clearBuffer();
                screen(flag);
                for (int j = 0; j < 6; j++)
                {
                    u8g2.setCursor(20 + j * 16, 48);
                    u8g2.print('_');
                }
                u8g2.sendBuffer(); // 开显示
                continue;
            }
            u8g2.clearBuffer();
            screen(flag);
            for (int j = 0; j < c; j++)
            {
                u8g2.setCursor(20 + j * 16, 48);
                u8g2.print('*');
            }
            for (int j = c; j < 6; j++)
            {
                u8g2.setCursor(20 + j * 16, 48);
                u8g2.print('_');
            }
            u8g2.sendBuffer(); // 开显示
        }
        if (key == '#')
        {
            return "cancel";
        }
    }
    Serial.print("get password: ");
    Serial.println(pw); // 打印密码
    return pw;
}
int Key_verify(String str) // 密码验证
{
    if(str == "cancel")
    {
        return -1; // 取消操作
    }
    flag = 0; // 重置标志位
    for (int i = 0; i < 20; i++)
    {
        pswd0 = "";
        EEPROM.begin(4096);
        if (pw_checker[i] == 1)
        {
            pswd0 = get_string(2700 + 10 * i); // 获取密码
            delay(50);
            Serial.print("get Key from EEPROM: ");
            Serial.println(pswd0); // 打印密码
            if (str == pswd0)
            {
                Serial.print("Key is correct!  ID = ");
                Serial.println(i); // 打印密码标识
                return 1;          //! 密码正确
            } 
        }
    }
    Serial.println("Key is incorrect!"); //! 密码错误
    u8g2.clearBuffer();
    u8g2.drawXBMP(33, 20, 16, 16, str39); // 密
    u8g2.drawXBMP(49, 20, 16, 16, str40); // 码
    u8g2.drawXBMP(65, 20, 16, 16, str5); // 错
    u8g2.drawXBMP(81, 20, 16, 16, str6); // 误
    u8g2.sendBuffer(); // 开显示
    beep(3);                             // 蜂鸣器响
    err++; // 错误次数加1
    return 0;
}
int get_id(int checker[20])
{
    u8g2.clearBuffer(); //! 请输入编号
    u8g2.drawXBMP(25, 18, 16, 16, str7);
    u8g2.drawXBMP(41, 18, 16, 16, str37);
    u8g2.drawXBMP(57, 18, 16, 16, str38);
    u8g2.drawXBMP(73, 18, 16, 16, str54);
    u8g2.drawXBMP(89, 18, 16, 16, str55);
    u8g2.setCursor(52, 49);               // 设置显示位置
    u8g2.print("_");                   // 输出字符
    u8g2.setCursor(68, 49);               // 设置显示位置
    u8g2.print("_");                   // 输出字符
    int j = 0;
    for(int i=0;i<10;i++){
        if(checker[i]==1){
            u8g2.setCursor(j * 6, 61); // 设置显示位置
            u8g2.print(i);
            j++;
        }
    }
    for(int i = 10; i < 20; i++)
    {
        if (checker[i] == 1)
        {
            u8g2.setCursor(j * 6, 62); // 设置显示位置
            u8g2.print("_");
            u8g2.setCursor(j * 6, 61); // 设置显示位置
            u8g2.print(i-10);
            j++;
        }
    }
    u8g2.sendBuffer(); // 开显示

    int c = 0, id = 0;
    while (c < 2)
    {
        char key = keypad.getKey();
        if (c == 0 && key <= '1' && key >= '0')
        {
            id = (int)key - '0';
            u8g2.setCursor(52, 48);
            u8g2.print(key);
            u8g2.sendBuffer(); // 开显示
            beep(1);           // 蜂鸣器响
            c++;
        }
        else if (c == 1 && key <= '9' && key >= '0')
        {
            id = id * 10 + (int)key - '0';
            u8g2.setCursor(68, 48);
            u8g2.print(key);
            u8g2.sendBuffer(); // 开显示
            beep(1);           // 蜂鸣器响
            c++;
            delay(1000); // 延时1秒
            beep(2); // 蜂鸣器响
        }
    }
    return id;
}
void Key_set() // 密码设置
{
    //! 请输入管理员密码
    flag = 1;
    if (1)
    {
        u8 a = 0;
        while (1)
        {
            switch (a)
            {
            case 0:
                flag = 2; //! 请输入新密码
                pswd0 = get_pswd();
                if(pswd0 == "cancel")
                {
                    return; // 取消操作
                }
                Serial.print("New password: ");
                Serial.println(pswd0); // 打印密码
                a = 1;
                break;
            case 1:
                flag = 3; //! 请再次输入
                if (get_pswd() == pswd0)
                {
                    Serial.println("New password is correct!");
                    delay(1000);
                    beep(2); // 蜂鸣器响
                    a = 2;
                    break;
                }
                else
                {
                    Serial.println("New password is incorrect!"); //! 输入错误
                    u8g2.clearBuffer();
                    u8g2.drawXBMP(33, 20, 16, 16, str37); // 输
                    u8g2.drawXBMP(49, 20, 16, 16, str38); // 入
                    u8g2.drawXBMP(65, 20, 16, 16, str5); // 错
                    u8g2.drawXBMP(81, 20, 16, 16, str6); // 误
                    u8g2.sendBuffer();                   // 开显示
                    delay(1000);
                    beep(3);
                    a = 0;
                    break;
                }
            case 2: //! 密码设置成功
                int id = get_id(pw_checker); // 获取ID
                EEPROM.begin(4096);
                set_string(2700 + 10 * id, pswd0); // 保存密码到EEPROM
                pw_checker[id] = 1;
                EEPROM.write(3500 + id, pw_checker[id]); // 保存密码标识到EEPROM
                EEPROM.commit(); // 执行保存EEPROM
                Serial.print("Password ID: ");
                Serial.println(id); // 打印密码标识

                u8g2.clearBuffer(); //! 添加成功
                u8g2.drawXBMP(33, 20, 16, 16, str52); // 添加成功
                u8g2.drawXBMP(49, 20, 16, 16, str53); 
                u8g2.drawXBMP(65, 20, 16, 16, str13);  
                u8g2.drawXBMP(81, 20, 16, 16, str14);  
                u8g2.sendBuffer();                   // 开显示
                beep(2);                             // 蜂鸣器响
                delay(1000);
                flag = 0; //! 请输入密码
                return; // 退出函数
            }
        }
    }

}
void Key_clear()
{
    EEPROM.begin(4096);
    for (int i = 0; i < 20; i++)
    {
        pw_checker[i] = EEPROM.read(3500 + i); // 获取ID标识
        Serial.print(int(pw_checker[i]));
        Serial.print(",");
    }
    Serial.println("]");
    int id = get_id(pw_checker); // 获取ID

    pw_checker[id] = 0; // 清除密码标识
    EEPROM.begin(4096);
    EEPROM.write(3500 + id, pw_checker[id]); // 保存密码标识
    EEPROM.commit();                           // 执行保存EEPROM
    Serial.println("key cleared!  ID = ");
    Serial.print(id); // 打印密码标识
    u8g2.clearBuffer(); //! 删除成功
    u8g2.drawXBMP(33, 20, 16, 16, str50); // 删除成功
    u8g2.drawXBMP(49, 20, 16, 16, str51);
    u8g2.drawXBMP(65, 20, 16, 16, str13);
    u8g2.drawXBMP(81, 20, 16, 16, str14);
    u8g2.sendBuffer(); // 开显示
    beep(2);         // 蜂鸣器响

    delay(1000);
}
