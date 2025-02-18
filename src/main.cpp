#include "main.h"

// 声明全局变量-OLED对象和菜单控制器对象
extern Adafruit_SSD1306 oled;
extern OLEDStatus oledStatus;

void setup()
{
    Serial.begin(115200);
    pinInit();
    interruptInit();
    oledInit(&oledStatus);      // 初始化OLED显示屏
    menuSpawner(&oledStatus);   // 构建菜单二叉树
    oledStatus.start(MainMenu); // 主菜单显示

    initWebServer(); // 初始化 HTTP 服务器
}

void loop()
{
    interruptHandler();
    managerUpdate(); // 处理 HTTP 请求
}

void pinInit()
{
    pinMode(btnPin1, INPUT_PULLDOWN); // 按键1 (下一个)
    pinMode(btnPin2, INPUT_PULLDOWN); // 按键2 (确定/控制灯)
    pinMode(btnPin3, INPUT_PULLDOWN); // 按键3 (返回)
    pinMode(ledPin, OUTPUT);        // 指示灯
    pinMode(Yellow, OUTPUT);        // 黄灯
    pinMode(Green, OUTPUT);         // 绿灯
    pinMode(Red, OUTPUT);           // 红灯
}

void oledInit(OLEDStatus *Status)
{
    if (Status)
    {
        OLED_init(oled); // 初始化OLED显示屏
    }
}

