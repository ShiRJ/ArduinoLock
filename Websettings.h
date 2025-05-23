#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
const char *AP_NAME = "ESP32WiFiAP"; // Web配网模式下的AP-wifi名字
const char *AP_PSWD = "12345678";    // Web配网模式下的AP-wifi密码

// 暂时存储wifi账号密码
char sta_ssid[32] = {0};
char sta_password[64] = {0};
char sta_auth[32] = {0};

// 配网页面代码
String page_html = R"(
<!DOCTYPE html>
<html lang='en'>
  <head>
    <meta charset='UTF-8'>
   
    <meta name="viewport" content="width=device-width,initial-scale=1.0,maximum-scale=1.0,minimum-scale=1.0,user-scalable=no">
    <title>ESP32智能门锁参数配置</title>
    <style type="text/css">
      * { margin: 0; padding: 0; }
       html { height: 100%; }
       h2 {text-align: center;color: #fff;line-height: 2.2;}
       body { height: 100%; background-color:rgb(80, 112, 255); 50% 50% no-repeat; background-size: cover;}
       .dowebok { position: absolute; left: 50%; top: 30%; width: 380px; height: 500px; margin: -200px 0 0 -200px; border: 3px solid #fff; border-radius: 10px; overflow: hidden;}
       
       .form-item { position: relative; width: 360px; margin: 0 auto; padding-bottom: 20px;}
       .form-item input { width: 288px; height: 48px; padding-left: 10px; border: 1px solid #fff; border-radius: 25px; font-size: 18px; color: #fff; background-color: transparent; outline: none;}
       .send_button { width: 360px; height: 50px; border: 0; border-radius: 25px; font-size: 18px; color:rgb(80, 126, 255); outline: none; cursor: pointer; background-color: #fff; }
       
       .tip { display: none; position: absolute; left: 20px; top: 52px; font-size: 14px; color: #f50; }
       .reg-bar { width: 360px; margin: 20px auto 0; font-size: 14px; overflow: hidden;}
       .reg-bar a { color: #fff; text-decoration: none; }
       .reg-bar a:hover { text-decoration: underline; }
       .reg-bar .reg { float: left; }
       .reg-bar .forget { float: right; }
       .dowebok ::-webkit-input-placeholder { font-size: 18px; line-height: 1.4; color: #fff;}
       .dowebok :-moz-placeholder { font-size: 18px; line-height: 1.4; color: #fff;}
       .dowebok ::-moz-placeholder { font-size: 18px; line-height: 1.4; color: #fff;}
       .dowebok :-ms-input-placeholder { font-size: 18px; line-height: 1.4; color: #fff;}
        
       @media screen and (max-width: 500px) {
       * { box-sizing: border-box; }
       .dowebok { position: static; width: auto; height: auto; margin: 0 30px; border: 0; border-radius: 0; }
       .logo { margin: 50px auto; }
       .form-item { width: auto; }
       .form-item input, .form-item button, .reg-bar { width: 100%; }
       }
       
    </style>
  </head>
  
  <body>
    <div class="dowebok">
      <h2>智能门锁参数配置</h2>
      <form style='text-align: center;padding-top: 20px' name='input' action='/' method='POST'>  
         <div class="form-item">
          <input id="username" type="text" name='ssid' autocomplete="off" placeholder="WiFi名称">
         </div>
         <div class="form-item">
          <input id="password" type="password" name='password' autocomplete="off" placeholder="WiFi密码">
         </div>
         <div class="form-item">
          <input id="auth" type="auth" name='auth' autocomplete="off" placeholder="Blinker设备秘钥">
         </div>
         <div class="form-item">
           <div id="">
            <input id="send_button" type='submit' value='保存并连接'>
           </div>
        </div>
        
      </form> 
     </div>
  </body>
</html>
)";

const byte DNS_PORT = 53;       // DNS端口号
IPAddress apIP(192, 168, 4, 1); // esp32-AP-IP地址
DNSServer dnsServer;            // 创建dnsServer实例
WebServer server(80);           // 创建WebServer

void handleRoot()
{ // 访问主页回调函数
    server.send(200, "text/html", page_html);
}
void initSoftAP(void)
{ // 初始化AP模式
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    Serial.println(WiFi.softAPIP());
    //    Serial.print("本地IP： ");
    //  Serial.println(WiFi.localIP());
    if (WiFi.softAP(AP_NAME, AP_PSWD))
    {
        Serial.println("ESP32 SoftAP is right");
    }
}

void initDNS(void)
{ // 初始化DNS服务器
    if (dnsServer.start(DNS_PORT, "*", apIP))
    { // 判断将所有地址映射到esp8266的ip上是否成功
        Serial.println("start dnsserver success.");
    }
    else
        Serial.println("start dnsserver failed.");
}
void handleRootPost()
{ // Post回调函数
    String wifiid = "", wifipass = "", auth = "";
    Serial.println("handleRootPost");
    if (server.hasArg("ssid"))
    { // 判断是否有账号参数
        if(server.arg("ssid") == "")
        {
            Serial.println("error, not found ssid");
            server.send(200, "text/html", "<meta charset='UTF-8'>提示：请输入WiFi账号，若不修改请输入“0000”");
            return;
        }
        Serial.print("got ssid:");
        strcpy(sta_ssid, server.arg("ssid").c_str()); // 将账号参数拷贝到sta_ssid中
        Serial.println(sta_ssid);
    }

    // 密码与账号同理
    if (server.hasArg("password"))
    {
        if(server.arg("password") == "")
        {
            Serial.println("error, not found password");
            server.send(200, "text/html", "<meta charset='UTF-8'>提示：请输入WiFi密码，若不修改请输入“0000”");
            return;
        }
        Serial.print("got password:");
        strcpy(sta_password, server.arg("password").c_str());
        Serial.println(sta_password);
    }


    if (server.hasArg("auth"))
    {
        if(server.arg("auth") == "")
        {
            Serial.println("error, not found auth");
            server.send(200, "text/html", "<meta charset='UTF-8'>提示：请输入Blinker设备秘钥，若不修改请输入“0000”");
            return;
        }
        Serial.print("got Blinker Auth Code:");
        strcpy(sta_auth, server.arg("auth").c_str());
        Serial.println(sta_auth);
    }

    wifiid = server.arg("ssid");
    wifipass = server.arg("password");
    auth = server.arg("auth");
    EEPROM.begin(4096);
    if (wifiid != "0000")
    {
        set_string(2560, wifiid); // 保存账号到EEPROM
    }
    if (wifipass != "0000")
    {
        set_string(2590, wifipass); // 保存密码到EEPROM
    }
    if (auth != "0000")
    {
        set_string(2620, auth); // 保存auth到EEPROM
    }

    server.send(200, "text/html", "<meta charset='UTF-8'><h1>保存成功，ESP32重启中...</h1>"); // 返回保存成功页面
    delay(2000);
    beep(2); // 蜂鸣器响
    // 连接wifi
    // connectNewWifi();

    ESP.restart(); // 重启ESP32
}
void initWebServer(void)
{ // 初始化WebServer
    // server.on("/",handleRoot);
    // 上面那行必须以下面这种格式去写否则无法强制门户
    server.on("/", HTTP_GET, handleRoot);      // 设置主页回调函数
    server.onNotFound(handleRoot);             // 设置无法响应的http请求的回调函数
    server.on("/", HTTP_POST, handleRootPost); // 设置Post请求回调函数
    server.begin();                            // 启动WebServer
    Serial.println("WebServer started!");
}
