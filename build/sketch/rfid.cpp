#line 1 "c:\\Users\\shike\\Desktop\\客户资料\\4-14lumen 智能门锁\\ArduinoLock\\rfid.cpp"
#include "include.h"
#include "Rfid.h"

#define RST_PIN 2
#define SS_PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN); // 创建新的RFID实例
MFRC522::MIFARE_Key key0;
int RFID_checker[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // 用于存储密码的数组
String cardUid = "";                                                                 // 用于存储卡片UID的字符串
void RFID_init()
{
    SPI.begin(18, 19, 23, 5); // SPI开始
    mfrc522.PCD_Init();       // 初始化MFRC522卡

    // 初始化密钥
    for (byte i = 0; i < 6; i++)
    {
        key0.keyByte[i] = 0xFF;
    }
    EEPROM.begin(4096);             // 初始化EEPROM
    Serial.print("Read key_sign["); // 打印密码标识
    // EEPROM.begin(4096);
    // for (int i = 0; i < 20; i++)
    // {
    //     EEPROM.write(3520 + i, RFID_checker[i]);
    // }   
    // EEPROM.commit(); // 执行保存EEPROM
    EEPROM.begin(4096);
    for (int i = 0; i < 20; i++)
    {
            RFID_checker[i] = EEPROM.read(3520 + i); // 获取密码标识
        Serial.print(int(RFID_checker[i]));
        Serial.print(",");
    }
    Serial.println("]");
    Serial.println("RFID Reader is ready!");
    // set_string(3100, "72D44D05");
    // cardUid = get_string(3100); // 从EEPROM读取UID
}

String RFID_scan()
{
    // 寻找新卡
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        // Serial.println("");
        return "";
    }

    // 选择一张卡
    if (!mfrc522.PICC_ReadCardSerial())
    {
        Serial.println("card not selected");
        return "";
    }

    // 显示卡片的详细信息
    // Serial.print("UID: ");
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    String uidStr = "";
    Serial.println();
    for (int i = 0; i < 4; i++)
    {
        // 将每个16进制数转换为两位数的字符串
        char hexStr[3];
        sprintf(hexStr, "%02X", mfrc522.uid.uidByte[i]);
        uidStr += hexStr;
    }
    // Serial.println(uidStr);
    // Serial.print("Card type: ");
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    // Serial.println(mfrc522.PICC_GetTypeName(piccType));
    if (uidStr != "")
        return uidStr;
    // 检查兼容性
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K)
    {
        Serial.println("Only Mifare Classic");
        return "";
    }
    {
        Serial.println("Card UID is not registered!");
        return "";
    }
    // 进行身份验证
    byte blockAddr = 1; // 选择要读取的块地址
    MFRC522::StatusCode status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockAddr, &key0, &(mfrc522.uid));

    if (status != MFRC522::STATUS_OK)
    {
        Serial.print("RFID READ ERROR: ");
        Serial.println(mfrc522.GetStatusCodeName(status));
        return "";
    }

    // 读取数据
    byte buffer[18];
    byte size = sizeof(buffer);
    status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);

    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("RFID READ ERROR: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return "";
    }

    // 停止 PICC
    mfrc522.PICC_HaltA();
    // 停止加密 PCD
    mfrc522.PCD_StopCrypto1();
    return uidStr;
}

char RFID_verify(String uid)
{
    for (int i = 0; i < 20; i++)
    {
        EEPROM.begin(4096);
        if (RFID_checker[i] == 1)
        {
            cardUid = get_string(3100 + 10 * i); // 获取UID
            delay(50);
            Serial.print("get UID from EEPROM: ");
            Serial.println(cardUid); // 打印UID
            if (uid == cardUid)
            {
                Serial.print("Card UID is correct!  ID = ");
                Serial.println(i); // 打印密码标识
                return 1;          //! 密码正确
            }
        }
    }
    Serial.println("Card UID verification failed!"); //! 卡片错误
    u8g2.clearBuffer();
    u8g2.drawXBMP(33, 20, 16, 16, str41); // 刷
    u8g2.drawXBMP(49, 20, 16, 16, str42); // 卡
    u8g2.drawXBMP(65, 20, 16, 16, str5); // 错
    u8g2.drawXBMP(81, 20, 16, 16, str6); // 误
    u8g2.sendBuffer();                   // 开显示
    beep(3);
    delay(3000);
    return 0; // 返回0表示验证失败
}
/**
   将字节数组转储为串行的十六进制值
*/
void dump_byte_array(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

/**
   将字节数组转储为串行的十进制值
*/
void dump_byte_array_dec(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 10 ? " 0" : " ");
        Serial.print(buffer[i], DEC);
    }
}
/**
 * 添加卡片
 */
void RFID_add()
{
    String uidStr = "";
    u8g2.clearBuffer();
    u8g2.drawXBMP(33, 20, 16, 16, str7); // 请刷新卡
    u8g2.drawXBMP(49, 20, 16, 16, str41);
    u8g2.drawXBMP(65, 20, 16, 16, str56);
    u8g2.drawXBMP(81, 20, 16, 16, str42);
    u8g2.sendBuffer();                   // 开显示
    while (uidStr == "")
    {
        uidStr = RFID_scan(); // 扫描卡片
        char key = get_key(); // 获取按键
        if (key == '#')
        {
            beep(1);
            Serial.print("add card cancelled!");
            break;
        }
    }
    if (uidStr != "")
    {
        beep(1); // 蜂鸣器响
        EEPROM.begin(4096);
        for (int i = 0; i < 20; i++)
        {
            RFID_checker[i] = EEPROM.read(3520 + i); // 获取ID标识
            Serial.print(int(RFID_checker[i]));
            Serial.print(",");
        }
        Serial.println("]");
        int id = get_id(RFID_checker); // 获取ID
        Serial.print("ID = ");
        Serial.println(id); // 打印密码标识
        EEPROM.begin(4096);
        set_string(3100 + 10 * id, uidStr);        // 保存UID到EEPROM
        RFID_checker[id] = 1;                      // 设置密码标识
        EEPROM.write(3520 + id, RFID_checker[id]); // 保存密码标识
        EEPROM.commit();                           // 执行保存EEPROM
        Serial.print("Card UID added: ");
        Serial.println(uidStr); // 打印UID

        u8g2.clearBuffer();     //! 添加成功
        u8g2.drawXBMP(33, 20, 16, 16, str52); // 添加成功
        u8g2.drawXBMP(49, 20, 16, 16, str53);
        u8g2.drawXBMP(65, 20, 16, 16, str13);
        u8g2.drawXBMP(81, 20, 16, 16, str14);
        u8g2.sendBuffer(); // 开显示
        beep(2);           // 蜂鸣器响
        delay(1000);
    }
}
/**
 * 清除卡片
 */
void RFID_clear()
{
    EEPROM.begin(4096);
    for (int i = 0; i < 20; i++)
    {
        RFID_checker[i] = EEPROM.read(3520 + i); // 获取ID标识
        Serial.print(int(RFID_checker[i]));
        Serial.print(",");
    }
    Serial.println("]");
    int id = get_id(RFID_checker); // 获取ID

    RFID_checker[id] = 0; // 清除密码标识
    EEPROM.begin(4096);
    EEPROM.write(3520 + id, RFID_checker[id]); // 保存密码标识
    EEPROM.commit();                           // 执行保存EEPROM
    Serial.print("Card UID cleared! ID = ");
    Serial.println(id); // 打印密码标识

    u8g2.clearBuffer();                   //! 删除成功
    u8g2.drawXBMP(33, 20, 16, 16, str50); // 删除成功
    u8g2.drawXBMP(49, 20, 16, 16, str51);
    u8g2.drawXBMP(65, 20, 16, 16, str13);
    u8g2.drawXBMP(81, 20, 16, 16, str14);
    u8g2.sendBuffer(); // 开显示
    beep(2);           // 蜂鸣器响
    delay(1000);
}