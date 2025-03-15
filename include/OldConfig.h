#ifndef CONFIG_H
#define CONFIG_H

int sampleWindow = 50; // 50ms采样一次。频率20Hz

// 按钮引脚
#define btnPin1 5  // 按钮引脚1
#define btnPin2 18 // 按钮引脚2
#define btnPin3 19 // 按钮引脚3

// 灯引脚
#define ledPin 14 // 指示灯引脚
#define Yellow 27 // 引脚1
#define Green 26  // 引脚2
#define Red 25    // 引脚3

#define OLED_WIDTH 128 // OLED显示屏宽度
#define OLED_HEIGHT 64 // OLED显示屏高度

#define OLED_SDA 21
#define OLED_SCL 22

#define MAX_4466_IN 35  // MAX_4466_IN 13  原先的引脚
#define DHT11_IN 12

#define WIFI_SSID "light_4G"
#define WIFI_PASSWORD "147258369."

#endif