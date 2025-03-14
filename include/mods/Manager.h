#pragma once

#include "main.h"
#include "config.h"
#include <ESPmDNS.h>
#include <WebServer.h>

extern OLEDStatus oledStatus;

WebServer server(80);

void manangerUpdate();
void initWebServer();
void handleRoot();
void handleRun();
void handleKey();

void managerUpdate()
{
    server.handleClient();
}

/// @brief 初始化 Web 服务器, 定义路由, 并启动 mDNS
void initWebServer()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    MDNS.begin("esp32-manager"); // 启动 mDNS
    // 定义路由
    server.on("/", HTTP_GET, handleRoot);
    server.on("/run", HTTP_POST, handleRun); // 修改为POST请求
    server.on("/sim_key", HTTP_POST, handleKey); // 修改为POST请求
    server.begin();
    Serial.println("Web server started");

    oledStatus.Screen->setCursor(2, 56);
    oledStatus.Screen->setTextSize(1);
    oledStatus.Screen->print(WiFi.localIP());
    oledStatus.Screen->display();
}

/// @brief 生成根目录响应
void handleRoot()
{
    String html = "<html>"
                  "  <head>"
                  "    <title>ESP32 Web Control</title>"
                  "    <style>"
                  "      body { font-family: Arial, sans-serif; margin: 20px; }"
                  "      button { display: block; width: 200px; margin: 10px auto; padding: 10px; }"
                  "      .container { max-width: 800px; margin: 0 auto; }"
                  "      .section { margin-bottom: 30px; }"
                  "      .section-title { font-size: 1.2em; font-weight: bold; margin-bottom: 10px; }"
                  "      .button-container { display: flex; justify-content: space-between; flex-wrap: wrap; }"
                  "      .input-group { display: flex; align-items: center; margin-bottom: 10px; }"
                  "      input[type=text] { margin-left: 10px; padding: 5px; }"
                  "    </style>"
                  "  </head>"
                  "  <body>"
                  "    <div class='container'>"
                  "      <div class='section'>"
                  "        <div class='section-title'>功能运行</div>"
                  "        <div class='button-container'>"
                  "          <button onclick='runMode(\"Y_Mode\")'>Y_Mode</button>"
                  "          <button onclick='runMode(\"N_Mode\")'>N_Mode</button>"
                  "          <button onclick='runMode(\"T_Mode\")'>T_Mode</button>"
                  "        </div>"
                  "        <div class='button-container'>"
                  "          <button onclick='runMode(\"Humiture\")'>Detect_Humiture();</button>"
                  "          <button onclick='runMode(\"VOL_Voltage\")'>Detect_VOL_Voltage();</button>"
                  "        </div>"
                  "      </div>"
                  "      <div class='section'>"
                  "        <div class='section-title'>灯光控制</div>"
                  "        <div class='input-group'>"
                  "          <span>颜色(Y;G;R):</span>"
                  "          <input type='text' id='color' placeholder='Yellow;Green;Red'>"
                  "        </div>"
                  "        <div class='input-group'>"
                  "          <span>亮度大小(0-255):</span>"
                  "          <input type='text' id='brightness' placeholder='0-255'>"
                  "        </div>"
                  "        <div class='input-group'>"
                  "          <span>闪烁频率(ms):</span>"
                  "          <input type='text' id='flash' placeholder='闪烁频率(ms)'>"
                  "        </div>"
                  "        <button onclick='runLight()'>Run</button>"
                  "      </div>"
                  "      <div class='section'>"
                  "        <div class='section-title'>模拟按键</div>"
                  "        <div class='button-container'>"
                  "          <button onclick='runSimKey(\"back\")'>Back</button>"
                  "          <button onclick='runSimKey(\"select\")'>Select</button>"
                  "          <button onclick='runSimKey(\"next\")'>Next</button>"
                  "        </div>"
                  "      </div>"
                  "      <div id='response' class='section'>"
                  "        <div class='section-title'>运行的功能</div>"
                  "      </div>"
                  "    </div>"
                  "    <script>"
                  "      function runMode(mode) {"
                  "        var xhr = new XMLHttpRequest();"
                  "        xhr.open('POST', '/run', true);"
                  "        xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');"
                  "        xhr.onreadystatechange = function() {"
                  "          if (xhr.readyState === 4 && xhr.status === 200) {"
                  "            document.getElementById('response').innerHTML += '<p>' + xhr.responseText + '</p>';"
                  "          }"
                  "        };"
                  "        xhr.send('mode=' + encodeURIComponent(mode));"
                  "      }"
                  "      function runLight() {"
                  "        var color = document.getElementById('color').value;"
                  "        var brightness = document.getElementById('brightness').value;"
                  "        var flash = document.getElementById('flash').value;"
                  "        var xhr = new XMLHttpRequest();"
                  "        xhr.open('POST', '/run', true);"
                  "        xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');"
                  "        xhr.onreadystatechange = function() {"
                  "          if (xhr.readyState === 4 && xhr.status === 200) {"
                  "            document.getElementById('response').innerHTML += '<p>' + xhr.responseText + '</p>';"
                  "          }"
                  "        };"
                  "        xhr.send('mode=light&color=' + encodeURIComponent(color) +"
                  "          '&brightness=' + encodeURIComponent(brightness) +"
                  "          '&flash=' + encodeURIComponent(flash));"
                  "      }"
                  "      function runSimKey(key) {"
                  "        var xhr = new XMLHttpRequest();"
                  "        xhr.open('POST', '/sim_key', true);"
                  "        xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');"
                  "        xhr.onreadystatechange = function() {"
                  "          if (xhr.readyState === 4 && xhr.status === 200) {"
                  "            document.getElementById('response').innerHTML += '<p>' + xhr.responseText + '</p>';"
                  "          }"
                  "        };"
                  "        xhr.send('key=' + encodeURIComponent(key));"
                  "      }"
                  "    </script>"
                  "</body>"
                  "</html>";
    server.send(200, "text/html;charset=utf-8", html);
}

/// @brief 处理函数运行请求
void handleRun()
{
    Serial.println("Run Mode");
    String mode = server.arg("mode");
    if (mode == "Y_Mode")
    {
        DengCtrl_Y_Mode();
        server.send(200, "text/plain", "Y_Mode");
    }
    else if (mode == "N_Mode")
    {
        DengCtrl_N_Mode();
        server.send(200, "text/plain", "N_Mode");
    }
    else if (mode == "T_Mode")
    {
        DengCtrl_T_Mode();
        server.send(200, "text/plain", "T_Mode");
    }
    else if (mode == "Humiture")
    {
        Detect_Humiture();
        server.send(200, "text/plain", "Humiture");
    }
    else if (mode == "VOL_Voltage")
    {
        Detect_VOL_Voltage();
        server.send(200, "text/plain", "VOL_Voltage");
    }
    else if (mode == "light")
    {
        String color = server.arg("color");
        int brightness = atoi(server.arg("brightness").c_str());
        int flash = atoi(server.arg("flash").c_str());
        opPinColor = (color == "Y")  ? Yellow
                     : (color == "G") ? Green
                                          : Red;
        opPinIntensity = brightness;
        opPinBlink = flash;
        DengSet_Run();
        server.send(200, "text/plain", "lights");
    }
    else
    {
        server.send(404, "text/plain", "Not found");
    }
}

/// @brief 处理模拟按键请求
void handleKey()
{
    Serial.println("Simulate Key");

    String key = server.arg("key");
    if (key == "next")
    {
        flagNext = true;
        server.send(200, "text/plain", "Next");
    }
    else if (key == "select")
    {
        flagSelect = true;
        server.send(200, "text/plain", "Select");
    }
    else if (key == "back")
    {
        flagBack = true;
        server.send(200, "text/plain", "Back");
    }
    else
    {
        server.send(404, "text/plain", "Not found");
    }
}
