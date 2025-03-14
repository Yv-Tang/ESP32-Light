#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "OLED_Power.h"
#include "menuCtrler.h"
#include "menuSpawner.h"
#include "interruptHandler.h"
#include "mods/Manager.h" // 网络模块管理

// 前向声明
class OLEDStatus;

// OLED 对象
Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire);

// 菜单控制器
OLEDStatus oledStatus(oled);

// 初始化函数声明
void pinInit();
void interruptInit();
void oledInit(OLEDStatus *Status);
void interruptHandler();

#endif
