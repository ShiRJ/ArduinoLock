#include "finger.h"
HardwareSerial mySerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
u8 ID_NUM = 0; // 存储添加的指纹ID
void init_FR()
{
    finger.begin(57600);
    Serial.println("AS608 is Ready!");
    Serial.print("Read ID_NUM: ");
    EEPROM.begin(4096);
    Serial.println(EEPROM.read(4000));
    ID_NUM = EEPROM.read(4000); // 获取指纹ID
}
void Add_FR()
{
    u8 i = 0, ensure, processnum = 0;
    Serial.println("---------------------------");
    while (1)
    {
        switch (processnum)
        {
        case 0:
            i++;
            //! 请按手指
            u8g2.clearBuffer();
            u8g2.drawXBMP(25, 24, 16, 16, str7);  // 请
            u8g2.drawXBMP(41, 24, 16, 16, str15); // 按
            u8g2.drawXBMP(57, 24, 16, 16, str10); // 手
            u8g2.drawXBMP(73, 24, 16, 16, str3);  // 指
            u8g2.sendBuffer();                    // 开显示
            beep(1);                              // 蜂鸣器响
            Serial.println("Please Press...");
            while(digitalRead(34) != 1)
                ; // 等待指纹按下
            ensure = finger.getImage();
            if (ensure == FINGERPRINT_OK)
            {
                ensure = finger.image2Tz(1); // 生成特征
                if (ensure == FINGERPRINT_OK)
                {
                    // 指纹正常
                    Serial.println("Press OK");
                    i = 0;
                    processnum = 1; // 跳到第二步
                    u8g2.clearBuffer();
                    u8g2.drawXBMP(25, 18, 16, 16, str7); // 请移开手指
                    u8g2.drawXBMP(41, 18, 16, 16, str9);
                    u8g2.drawXBMP(57, 18, 16, 16, str0);
                    u8g2.drawXBMP(73, 18, 16, 16, str10);
                    u8g2.drawXBMP(89, 18, 16, 16, str3);
                    u8g2.sendBuffer(); // 开显示
                    beep(1);           // 蜂鸣器响
                    delay(1000);
                }
                else
                {
                };
            }
            else
            {
            };
            break;

        case 1:
            i++;
            // !再按一次
            Serial.println("Please Press Again...");
            while (digitalRead(34) != 1)
                ; // 等待指纹按下
            u8g2.clearBuffer();
            u8g2.drawXBMP(25, 18, 16, 16, str7); // 请再按一次
            u8g2.drawXBMP(41, 18, 16, 16, str46);
            u8g2.drawXBMP(57, 18, 16, 16, str47);
            u8g2.drawXBMP(73, 18, 16, 16, str37);
            u8g2.drawXBMP(89, 18, 16, 16, str48);
            u8g2.sendBuffer(); // 开显示
            beep(1);           // 蜂鸣器响
            ensure = finger.getImage();
            if (ensure == FINGERPRINT_OK)
            {
                ensure = finger.image2Tz(2); // 生成特征
                if (ensure == FINGERPRINT_OK)
                {
                    // 指纹正常
                    Serial.println("Press Again OK");
                    u8g2.clearBuffer();
                    u8g2.drawXBMP(25, 18, 16, 16, str7); // 请移开手指
                    u8g2.drawXBMP(41, 18, 16, 16, str9);
                    u8g2.drawXBMP(57, 18, 16, 16, str0);
                    u8g2.drawXBMP(73, 18, 16, 16, str10);
                    u8g2.drawXBMP(89, 18, 16, 16, str3);
                    u8g2.sendBuffer(); // 开显示
                    i = 0;
                    processnum = 2; // 跳到第三步
                    beep(1);        // 蜂鸣器响
                    delay(1000);
                }
                else
                {
                };
            }
            else
            {
            };
            break;

        case 2:
            // 创建模板
            Serial.print("Finger Create...");
            ensure = finger.createModel();
            if (ensure == FINGERPRINT_OK)
            {
                Serial.println("Success");
                processnum = 3; // 跳到第四步
            }
            else
            {
                // 创建模板失败
                Serial.println("Fail");
                u8g2.clearBuffer();                   //! 添加失败
                u8g2.drawXBMP(25, 20, 16, 16, str52); // 添加失败
                u8g2.drawXBMP(41, 20, 16, 16, str53);
                u8g2.drawXBMP(57, 20, 16, 16, str16);
                u8g2.drawXBMP(73, 20, 16, 16, str17);
                u8g2.sendBuffer(); // 开显示
                delay(1000);
                i = 0;
                processnum = 0; // 跳回第一步
            }
            delay(500);
            break;

        case 3:
            // 写入指纹ID
            ID_NUM++;
            EEPROM.begin(4096);
            EEPROM.write(4000, ID_NUM);         // 保存指纹ID
            EEPROM.commit();                    // 执行保存EEPROM
            ensure = finger.storeModel(ID_NUM); // 储存模板
            if (ensure == 0x00)
            {
                // 录入指纹成功 ID打印
                Serial.print("Add Fingerprint Success --- ");
                if ((ID_NUM > 0) && (ID_NUM < 10))
                    Serial.print("ID = 0");
                if (ID_NUM >= 10)
                    Serial.print("ID = ");
                Serial.println(ID_NUM);
                Serial.println(" ");
                u8g2.clearBuffer();                   //! 添加成功
                u8g2.drawXBMP(25, 20, 16, 16, str52); // 添加成功
                u8g2.drawXBMP(41, 20, 16, 16, str53);
                u8g2.drawXBMP(57, 20, 16, 16, str13);
                u8g2.drawXBMP(73, 20, 16, 16, str14);
                u8g2.sendBuffer(); // 开显示
                beep(2);           // 蜂鸣器响
                delay(1000);
                return;
            }
            else
            {
                processnum = 0;
            }
            break;
        }
        delay(400);
        if (i == 10) // 超过5次没有按手指则退出
        {
            Serial.println("Timeout !!!");
            u8g2.clearBuffer();
            u8g2.drawXBMP(25, 24, 16, 16, str11); // 识
            u8g2.drawXBMP(41, 24, 16, 16, str12); // 别
            u8g2.drawXBMP(57, 24, 16, 16, str20); // 超
            u8g2.drawXBMP(73, 24, 16, 16, str21); // 时
            u8g2.sendBuffer();                    // 开显示
            lbeep(1000);
            break;
        }
    }
}

int Press_FR()
{
    u8 ensure, i, j = 0;
    // 请按手指
    // Serial.println("---------------------------");
    // Serial.print("Please Press Fingerprint...");
    u8g2.clearBuffer();
    u8g2.drawXBMP(25, 24, 16, 16, str7);  // 请
    u8g2.drawXBMP(41, 24, 16, 16, str8);  // 勿
    u8g2.drawXBMP(57, 24, 16, 16, str9);  // 移
    u8g2.drawXBMP(73, 24, 16, 16, str10); // 手
    u8g2.drawXBMP(89, 24, 16, 16, str3);  // 指
    u8g2.sendBuffer();                    // 开显示
    for (i = 0; i < 15; i++)
    {
        ensure = finger.getImage();
        if (ensure == 0x00) // 获取图像成功
        {
            ensure = finger.image2Tz();
            if (ensure == 0x00) // 生成特征成功
            {
                ensure = finger.fingerFastSearch();
                if (ensure == 0x00) // 搜索成功
                {
                    // 指纹验证成功
                    Serial.println(" ");

                    //                    door(500);
                    Serial.println("Fingerprint verification successful !!!");
                    if ((0 < finger.fingerID) && (finger.fingerID < 10))
                        Serial.print("Welcome ID : 0");
                    if (finger.fingerID >= 10)
                        Serial.print("Welcome ID : ");
                    Serial.println(finger.fingerID);
                    Serial.println(" ");
                    j = 0;
                    delay(2000);
                    return 1;
                }
                else
                {
                    // 未搜索到指纹
                    Serial.println(" ");
                    Serial.println("!!! Fingerprint not found !!!");
                    Serial.println(" ");

                    u8g2.clearBuffer();
                    u8g2.drawXBMP(25, 20, 16, 16, str3); // 指
                    u8g2.drawXBMP(41, 20, 16, 16, str4); // 纹
                    u8g2.drawXBMP(57, 20, 16, 16, str5); // 错
                    u8g2.drawXBMP(73, 20, 16, 16, str6); // 误
                    u8g2.sendBuffer();                   // 开显示
                    beep(3);
                    delay(5000);
                    return 0;
                }
            }
            else
            {
            }
        }
        j++;
        delay(200);

        if (j >= 15)
        {
            // 等待超时
            Serial.println("Timeout !!!");
            u8g2.clearBuffer();
            u8g2.drawXBMP(25, 24, 16, 16, str11); // 识
            u8g2.drawXBMP(41, 24, 16, 16, str12); // 别
            u8g2.drawXBMP(57, 24, 16, 16, str20); // 超
            u8g2.drawXBMP(73, 24, 16, 16, str21); // 时
            u8g2.sendBuffer();                    // 开显示
            lbeep(1000);
            Serial.println(" ");
            return -1;
        }
    }
}

void Delete_FR()
{
    u8g2.clearBuffer();
    while (finger.emptyDatabase())
        ;
    u8g2.clearBuffer(); //! 删除成功
    beep(2);            // 蜂鸣器响
    ID_NUM = 0;
    EEPROM.begin(4096);
    EEPROM.write(4000, ID_NUM);           // 保存指纹ID
    EEPROM.commit();                      // 执行保存EEPROM
    u8g2.drawXBMP(25, 20, 16, 16, str50); // 删除成功
    u8g2.drawXBMP(41, 20, 16, 16, str51);
    u8g2.drawXBMP(57, 20, 16, 16, str13);
    u8g2.drawXBMP(73, 20, 16, 16, str14);
    u8g2.sendBuffer(); // 开显示
    delay(1000);
}

int verify()
{
    if (Press_FR() == 1)
    {
        Serial.println("Fingerprint verification successful !!!");
        return 1;
    }
    else if (Press_FR() == 0)
    {
        Serial.println("Fingerprint not found !!!");
        return 0;
    }
    else if (Press_FR() == -1)
    {
        Serial.println("Timeout !!!");
        return -1;
    }
}