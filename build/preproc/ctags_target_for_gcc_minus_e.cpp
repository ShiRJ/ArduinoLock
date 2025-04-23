# 1 "c:\\Users\\shike\\Desktop\\客户资料\\4-14lumen 智能门锁\\ArduinoLock\\ArduinoLock.ino"


# 4 "c:\\Users\\shike\\Desktop\\客户资料\\4-14lumen 智能门锁\\ArduinoLock\\ArduinoLock.ino" 2
# 5 "c:\\Users\\shike\\Desktop\\客户资料\\4-14lumen 智能门锁\\ArduinoLock\\ArduinoLock.ino" 2
String admin = "610AAA";
char auth[30];
char ssid[30];
char pswd[30];
char err = 0;
char page = 1;
char user_order = ' ';
BlinkerButton Button1("btn-abc");
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2((&u8g2_cb_r0), /* reset=*/255); // 0.96寸oled屏幕参数(SSD1306)
// 按下按键即会执行该函数
void button1_callback(const String &state)
{
    // BLINKER_LOG("get button state: ", state);
    BLINKER_LOG("successfully send order!");
    Serial.println("door open successed");
    Open_door(1, 500);
}
void oleddisplay();
void setup()
{
    init_FR(); // 初始化指纹模块
    Serial.begin(115200);
    u8g2.begin();
    u8g2.drawXBMP(25, 24, 16, 16, str32); // 系
    u8g2.drawXBMP(41, 24, 16, 16, str33); // 统
    u8g2.drawXBMP(57, 24, 16, 16, str34); // 初
    u8g2.drawXBMP(73, 24, 16, 16, str35); // 始
    u8g2.drawXBMP(89, 24, 16, 16, str36); // 化
    u8g2.sendBuffer();
    WiFi.mode(WIFI_MODE_STA); // 设置WiFi为STA模式
    EEPROM_Config(); // 初始化EEPROM
    Key_init(); // 初始化密码标识
    RFID_init(); // 初始化RFID模块
    pinMode(34, 0x01);
    pinMode(4, 0x03);
    digitalWrite(4, 0x0);
    pinMode(15, 0x03);
    digitalWrite(15, 0x0);

    BLINKER_DEBUG.stream(Serial);

    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    // Blinker.attachData(dataRead);
    Button1.attach(button1_callback);
    delay(3000);
}

void loop()
{
    if (err >= 5)
    {
        Serial.println("Error times over 5, please reset the system!");
        delay(30000);
        err = 0;
    }
    // 串口指令读取
    if (Serial.available())
        user_order = Serial.read();
    char key = get_key(); // 获取按键
    String uid = RFID_scan(); // 用于存储UID的字符串
    if (key == '#' && page == 1)
    {
        beep(1);
        Open_door(Key_verify(get_pswd()), 500); // 获取并验证密码
        // Serial.print("Key Pressed : ");
        // Serial.println(key);
    }
    else if (key == '*')
    {

        beep(1);
        if (page == 1)
        {
            flag = 1; // 设置标志位
            if (get_pswd() == admin)
            {
                Serial.println("Admin password is correct!");
                page = 2; // 切换到第二个界面
            }
            else
            {
                Serial.println("Admin password is incorrect!"); //! 密码错误
                beep(3); // 蜂鸣器响
                u8g2.clearBuffer();
                u8g2.drawXBMP(73, 18, 16, 16, str39); // 密
                u8g2.drawXBMP(89, 18, 16, 16, str40); // 码
                u8g2.drawXBMP(57, 20, 16, 16, str5); // 错
                u8g2.drawXBMP(73, 20, 16, 16, str6); // 误
                u8g2.sendBuffer(); // 开显示
                delay(1000);
            }
        }
        else
        {
            page = 1; // 切换到第一个界面
        }
    }
    else if (key != NO_KEY && key != '#' && key != '*' && page == 1)
    {
    }
    else if (key == '1' && page == 2)
    {
        beep(1);
        Serial.println("WEB config");
        Serial.println("Please open the browser and enter the IP address: ");
        setWiFi(); // 调用配网函数
    }
    else if (key == '2' && page == 2)
    {
        beep(1);
        Serial.println("Add fingerprint");
        Add_FR();
    }
    else if (key == '3' && page == 2)
    {
        beep(1);
        Serial.println("Add RFID card");
        RFID_add();
    }
    else if (key == '4' && page == 2)
    {
        beep(1);
        Serial.println("Add password");
        Key_set();
    }
    else if (key == '5' && page == 2)
    {
        beep(1);
        Serial.println("Delete fingerprint");
        Delete_FR();
    }
    else if (key == '6' && page == 2)
    {
        beep(1);
        Serial.println("Delete RFID card");
        RFID_clear();
    }
    else if (key == '7' && page == 2)
    {
        beep(1);
        Serial.println("Delete password");
        Key_clear();
    }

    if (digitalRead(34) == 1 && page == 1)
    {
        Open_door(Press_FR(), 500); // 调用Press_FR函数
    }
    if (uid != "" && page == 1)
    {
        Serial.print("get UID: ");
        Serial.println(uid); // 打印UID
        Open_door(RFID_verify(uid), 500); // 调用RFID_verify函数
    }
    if (WiFi.status() == WL_CONNECTED)
        Blinker.run();

    oleddisplay();
}

void oleddisplay()
{
    if (page == 1)
    { // 界面1
        IPAddress APIP = WiFi.softAPIP();
        IPAddress IP = WiFi.localIP();
        IPAddress NAME = WiFi.localIP();
        int16_t year = Blinker.year();
        int8_t month = Blinker.month();
        int8_t mday = Blinker.mday();
        int8_t hour = Blinker.hour();
        int8_t min = Blinker.minute();
        int8_t sec = Blinker.second();
        bool result = Blinker.connect();

        u8g2.clearBuffer(); // 清屏
        u8g2.setFont(u8g2_font_ncenB08_tr); // 字体08
        if (sec == -1)
        { // 如果获取不到时间
            u8g2.setCursor(0, 64); // 设置显示位置
            u8g2.print("----------"); // 输出字符

            u8g2.setCursor(80, 64); // 设置显示位置
            u8g2.print("--:--:--"); // 输出字符
        }
        else
        { // 如果获取到时间了
            u8g2.setCursor(0, 64); // 设置显示位置
            u8g2.print(year); // 年
            u8g2.setCursor(25, 64); // 设置显示位置
            u8g2.print("-"); // 输出字符
            u8g2.setCursor(30, 64); // 设置显示位置

            if (month < 10)
            { // 月
                u8g2.print("0");
                u8g2.setCursor(36, 64); // 设置显示位置
                u8g2.print(month);
            }
            else
                u8g2.print(month);

            u8g2.setCursor(43, 64); // 设置显示位置
            u8g2.print("-"); // 输出字符
            u8g2.setCursor(48, 64); // 设置显示位置

            if (mday < 10)
            { // 日
                u8g2.print("0");
                u8g2.setCursor(54, 64); // 设置显示位置
                u8g2.print(mday);
            }
            else
                u8g2.print(mday);

            u8g2.setCursor(78, 64); // 设置显示位置
            if (hour < 10)
            { // 时
                u8g2.print("0");
                u8g2.setCursor(84, 64); // 设置显示位置
                u8g2.print(hour);
            }
            else
                u8g2.print(hour);

            u8g2.setCursor(91, 64); // 设置显示位置
            u8g2.print(":"); // 输出字符

            u8g2.setCursor(96, 64); // 设置显示位置
            if (min < 10)
            { // 分
                u8g2.print("0");
                u8g2.setCursor(102, 64); // 设置显示位置
                u8g2.print(min);
            }
            else
                u8g2.print(min);

            u8g2.setCursor(109, 64); // 设置显示位置
            u8g2.print(":"); // 输出字符

            u8g2.setCursor(114, 64); // 设置显示位置
            if (sec < 10)
            { // 秒
                u8g2.print("0");
                u8g2.setCursor(120, 64); // 设置显示位置
                u8g2.print(sec);
            }
            else
                u8g2.print(sec);
        }

        u8g2.setCursor(20, 8); // 设置显示位置
        u8g2.print("WiFi connect:"); // 输出字符
        if (WiFi.status() != WL_CONNECTED)
        { // 如果wifi没连接
            u8g2.drawXBMP(0, 0, 16, 16, str23); // wifi连接失败图标
            u8g2.setCursor(20, 17); // 设置显示位置
            u8g2.print("fail"); // 输出字符
        }
        else
        { // 如果wifi有连接
            u8g2.drawXBMP(0, 0, 16, 16, str22); // wifi连接成功图标
            u8g2.setCursor(20, 17); // 设置显示位置
            u8g2.print(ssid); // 输出字符
        }

        if (result == 1)
            u8g2.drawXBMP(0, 17, 16, 16, str24); // 网络连接图标
        else
            u8g2.drawXBMP(0, 17, 16, 16, str25); // 网络未连接图标

        u8g2.setCursor(20, 26); // 设置显示位置
        u8g2.print("WiFi IP Address:"); // 输出字符
        u8g2.setCursor(20, 35); // 设置显示位置
        u8g2.print(IP); // 输出wifi的IP
        u8g2.drawXBMP(0, 36, 16, 16, str26); // AP图标
        u8g2.setCursor(20, 44); // 设置显示位置
        u8g2.print("AP IP Address:"); // 输出AP的IP
        u8g2.setCursor(20, 53); // 设置显示位置
        u8g2.print(APIP); // 输出字符

        u8g2.sendBuffer(); // 开显示
    }
    else if (page == 2)
    { // 界面2
        u8g2.clearBuffer(); // 清屏
        u8g2.setFont(u8g2_font_ncenB08_tr); //
        u8g2.setCursor(0, 8); // 设置显示位置
        u8g2.print("[1] Wifi & Auth"); // 输出字符
        u8g2.setCursor(0, 17); // 设置显示位置
        u8g2.print("[2] Add Finger"); // 输出字符
        u8g2.setCursor(0, 26); // 设置显示位置
        u8g2.print("[3] Add RFID Card"); // 输出字符
        u8g2.setCursor(0, 35); // 设置显示位置
        u8g2.print("[4] Add Password"); // 输出字符
        u8g2.setCursor(0, 44); // 设置显示位置
        u8g2.print("[5] Del Finger"); // 输出字符
        u8g2.setCursor(0, 53); // 设置显示位置
        u8g2.print("[6] Del RFID Card"); // 输出字符
        u8g2.setCursor(0, 62); // 设置显示位置
        u8g2.print("[7] Del Password"); // 输出字符
        u8g2.sendBuffer(); // 开显示
    }
}
