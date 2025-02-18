#ifndef OLED_POWER_H
#define OLED_POWER_H

#include "config.h"
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

// 前向声明
class OLEDStatus;

// OLED 初始化函数声明
void OLED_init(Adafruit_SSD1306 &oled);

void OLED_init(Adafruit_SSD1306 &oled)
{
    Wire.begin(OLED_SDA, OLED_SCL, 100000); // 初始化I2C总线
    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C); // 初始化OLED显示屏
    oled.display();                         // 显示屏幕(内存数据推送到屏幕)
                                            // oled.display(); // 显示屏幕(内存数据推送到屏幕)
                                            // oled.clearDisplay(); //清除屏幕
                                            // oled.setTextcolor(1); // 设置字体颜色(0:WHITE白色,1:BLACK黑色)
                                            // oled.setTextSize(1);  // 设置字体大小(1:6*8 ,2:12*16,3:18*24)
                                            // oled.setCursor(0, 0); // 设置光标位置(左上角)
                                            // oled.print("Powered by ESE001"); // 简单输出
                                            // oled.printf("Powered by %s", "ESE001"); // 复杂输出
}
#endif
