#include <Arduino.h>
#include <WiFi.h>
// #include <WiFiUdp.h>
#include <HTTPClient.h>
#include "Web.h"
#include "Websettings.h"  //Web配网
char setWiFi_Flag; // 配网标志位

// const char* ssid = "";
// const char* password = "";
// 强制门户Web配网

void setWiFi()
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 15, "WiFi Configuration");
    u8g2.drawStr(0, 30, "Press [#] to cancel");
    u8g2.drawStr(0, 45, "SSID: ");
    u8g2.setCursor(35, 45); // 设置显示位置
    u8g2.print(AP_NAME);
    u8g2.drawStr(0, 60, "CONNECT 192.168.4.1");
    u8g2.sendBuffer();
    setWiFi_Flag = 1;
    initSoftAP();
    initWebServer();
    initDNS();
    while (setWiFi_Flag)
    {
        server.handleClient();
        dnsServer.processNextRequest();
        char key = get_key(); // 获取按键
        if (key == '#')
        {
            setWiFi_Flag = 0;
            server.stop();
            Serial.println("WiFi configuration cancelled.");
            ESP.restart();
        }
    }
}
// 删除保存的wifi信息


