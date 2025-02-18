#ifndef MENU_SPAWNER_H
#define MENU_SPAWNER_H

#include "menuCtrler.h"
#include "config.h"
#include "main.h"
#include "Ticker.h"
#include "interruptHandler.h"
#include "DHT.h"
#include "DHT_U.h"

extern OLEDStatus oledStatus;
extern Adafruit_SSD1306 oled;

Ticker ticker1;

hw_timer_t *timer_Blink = NULL;
hw_timer_t *timer_Breath = NULL;
DHT dht(DHT11_IN, DHT11); // 设置传感器类型及引脚

int opPinColor = Yellow;    // 颜色
int opPinBlink = 0;         // 闪烁时间
int opPinIntensity = 0;     // 亮度
bool opPinStatus = false;   // 灯亮与否
int opPinBreathStatus = 0;  // 渐变强度
bool opPinBreathUp = false; // 渐亮

void btnTrue();
// 一级菜单功能
static void MainMenu_DengCtrl();
static void MainMenu_DengSet();
static void MainMenu_Detect();

// 二级菜单功能
// 灯控模式
static void DengCtrl_Y_Mode();
static void DengCtrl_N_Mode();
static void DengCtrl_T_Mode();

// 亮度调节
static void DengSet_Color();
static void DengSet_Blink();
static void DengSet_Intensity();
static void DengSet_Run();

// 数据测量
static void Detect_Humiture();
static void Detect_VOL_Voltage();

// 创建菜单项

// 主菜单
MENU MainMenu("MainMenu");

// 二级菜单
MENU DengCtrl("DengCtrl", &MainMenu, MainMenu_DengCtrl);
MENU DengSet("DengSet", &MainMenu, MainMenu_DengSet);
MENU Detect("Detect", &MainMenu, MainMenu_Detect);

// 灯控模式菜单
MENU Y_Mode("Y_Mode", &DengCtrl, DengCtrl_Y_Mode);
MENU N_Mode("N_Mode", &DengCtrl, DengCtrl_N_Mode);
MENU T_Mode("T_Mode", &DengCtrl, DengCtrl_T_Mode);

// 亮度调节菜单
MENU Color("Color", &DengSet, DengSet_Color);
MENU Blink("Blink", &DengSet, DengSet_Blink);
MENU Intensity("Intensity", &DengSet, DengSet_Intensity);
MENU Run("Run", &DengSet, DengSet_Run);

// 数据测量菜单
MENU Humiture("Humiture", &Detect, Detect_Humiture);
MENU VOL_Voltage("VOL_Voltage", &Detect, Detect_VOL_Voltage);

void btnTrue()
{
    if (flagBack || flagNext)
    {
        ticker1.detach();
        btn = true;
        flagBack = true;
        flagSelect = false;
        flagNext = false;
        interruptInit();
    }
}

void menuSpawner(OLEDStatus *Status)
{
    // 注册菜单项
    MainMenu.addSubMenu(&DengCtrl);
    MainMenu.addSubMenu(&DengSet);
    MainMenu.addSubMenu(&Detect);

    DengCtrl.addSubMenu(&Y_Mode);
    DengCtrl.addSubMenu(&N_Mode);
    DengCtrl.addSubMenu(&T_Mode);

    DengSet.addSubMenu(&Color);
    DengSet.addSubMenu(&Blink);
    DengSet.addSubMenu(&Intensity);
    DengSet.addSubMenu(&Run);

    Detect.addSubMenu(&Humiture);
    Detect.addSubMenu(&VOL_Voltage);

    // 绑定菜单项事件
    Y_Mode.setEvent(DengCtrl_Y_Mode);
    N_Mode.setEvent(DengCtrl_N_Mode);
    T_Mode.setEvent(DengCtrl_T_Mode);

    Color.setEvent(DengSet_Color);
    Blink.setEvent(DengSet_Blink);
    Intensity.setEvent(DengSet_Intensity);
    Run.setEvent(DengSet_Run);

    Humiture.setEvent(Detect_Humiture);
    VOL_Voltage.setEvent(Detect_VOL_Voltage);
}

static void MainMenu_DengCtrl()
{
    oledStatus.updateMenu(&DengCtrl);
    oledStatus.updateScreen();
}
static void MainMenu_DengSet()
{
    oledStatus.updateMenu(&DengSet);
    oledStatus.updateScreen();
}
static void MainMenu_Detect()
{
    oledStatus.updateMenu(&Detect);
    oledStatus.updateScreen();
}

void Y_Mode_ISR()
{
    digitalWrite(Red, flagSelect);
    flagSelect = digitalRead(btnPin2);
    btnTrue();
}
static void DengCtrl_Y_Mode()
{
    if (oledStatus.Screen)
    {
        // Serial.printf("Y_Mode\n");
        oled.clearDisplay();
        oled.setCursor(1, 0);
        oled.setTextColor(SSD1306_WHITE);
        oled.setTextSize(2);
        oled.println("*Y_Mode");
        oled.display();

        btn = false;
        ticker1.attach_ms(100, Y_Mode_ISR);
    }
}

void N_Mode_ISR()
{
    digitalWrite(Green, !flagSelect);
    flagSelect = digitalRead(btnPin2);
    btnTrue();
}
static void DengCtrl_N_Mode()
{
    if (oledStatus.Screen)
    {
        // Serial.printf("N_Mode\n");
        oled.clearDisplay();
        oled.setCursor(1, 0);
        oled.setTextColor(SSD1306_WHITE);
        oled.setTextSize(2);
        oled.println("*N_Mode");
        oled.display();

        btn = false;
        ticker1.attach_ms(100, N_Mode_ISR);
    }
}

void T_Mode_ISR()
{
    if (flagSelect)
    {
        digitalWrite(Yellow, !digitalRead(Yellow));
    }
    flagSelect = digitalRead(btnPin2);
    btnTrue();
}
static void DengCtrl_T_Mode()
{
    if (oledStatus.Screen)
    {
        // Serial.printf("T_Mode\n");
        oled.clearDisplay();
        oled.setCursor(1, 0);
        oled.setTextColor(SSD1306_WHITE);
        oled.setTextSize(2);
        oled.println("*T_Mode");
        oled.display();

        btn = false;
        ticker1.attach_ms(200, T_Mode_ISR);
    }
}

static void DengSet_Color()
{
    switch (opPinColor)
    {
    case Yellow:
        opPinColor = Green;
        break;
    case Green:
        opPinColor = Red;
        break;
    case Red:
        opPinColor = Yellow;
        break;
    }
    Color.setMenuName(opPinColor == Yellow ? "Yellow" : opPinColor == Green ? "Green"
                                                                            : "Red");
    oledStatus.updateMenu(&DengSet, false, true);
}

static void DengSet_Blink()
{
    opPinBlink = opPinBlink + 500;
    if (opPinBlink > 2000)
    {
        opPinBlink = 0;
    }
    Blink.setMenuName(opPinBlink == 0 ? "No Blink" : opPinBlink == 500 ? "500ms"
                                                 : opPinBlink == 1000  ? "1000ms"
                                                 : opPinBlink == 1500  ? "1500ms"
                                                                       : "2000ms");
    oledStatus.updateMenu(&DengSet, false, true);
}

static void DengSet_Intensity()
{
    opPinIntensity = opPinIntensity + 50;
    if (opPinIntensity > 250)
    {
        opPinIntensity = 0;
    }
    Intensity.setMenuName(opPinIntensity == 0 ? "Breath" : opPinIntensity == 50 ? "50"
                                                       : opPinIntensity == 100  ? "100"
                                                       : opPinIntensity == 150  ? "150"
                                                       : opPinIntensity == 200  ? "200"
                                                                                : "250");
    oledStatus.updateMenu(&DengSet, false, true);
}

void IRAM_ATTR toggle()
{
    analogWrite(opPinColor, opPinStatus ? opPinBreathStatus : 0);
    opPinStatus = !opPinStatus;
}
void IRAM_ATTR breath()
{
    analogWrite(opPinColor, opPinBreathStatus);
    (opPinBreathUp) ? opPinBreathStatus++ : opPinBreathStatus--;
    if (opPinBreathStatus < 0)
    {
        opPinBreathStatus = 0;
        opPinBreathUp = true;
    }
    if (opPinBreathStatus > 250)
    {
        opPinBreathStatus = 250;
        opPinBreathUp = false;
    }
}
static void DengSet_Run()
{
    // 初始关闭所有灯
    digitalWrite(Yellow, LOW);
    digitalWrite(Green, LOW);
    digitalWrite(Red, LOW);

    // 调整亮度颜色
    analogWrite(opPinColor, opPinIntensity);
    opPinBreathStatus = opPinIntensity;

    // 处理闪烁计时器
    if (opPinBlink > 0)
    {
        if (!timer_Blink)
        {
            timer_Blink = timerBegin(0, 80, true); // 初始化(单位:us)
            // Serial.printf("1.Blink:初始化\n");
            timerAttachInterrupt(timer_Blink, &toggle, true); // 绑定中断函数
            // Serial.printf("2.Blink:绑定\n");
        }
        timerAlarmWrite(timer_Blink, opPinBlink * 1000, true); // 设置闪烁时间(单位:ms)
        // Serial.printf("Blink:设置\n");
        timerAlarmEnable(timer_Blink); // 启动闪烁计时器
    }
    else
    {
        if (timer_Blink)
            timerAlarmDisable(timer_Blink); // 关闭闪烁计时器
        // Serial.printf("Blink:关闭\n");
    }

    // 处理呼吸计时器
    if (opPinIntensity == 0)
    {
        if (!timer_Breath)
        {
            timer_Breath = timerBegin(1, 80, true);
            // Serial.printf("1.Breath:初始化\n");
            timerAttachInterrupt(timer_Breath, &breath, true); // 绑定中断函数(单位:us)
            // Serial.printf("2.Breath:绑定\n");
        }
        timerAlarmWrite(timer_Breath, 10000, true); // 设置呼吸步长(时长:10ms)
        // Serial.printf("Breath:设置\n");
        timerAlarmEnable(timer_Breath); // 启动呼吸计时器
    }
    else
    {
        if (timer_Breath)
            timerAlarmDisable(timer_Breath); // 关闭呼吸计时器
        // Serial.printf("Breath:关闭\n");
    }
    // Serial.printf("Color:%d,Blink:%d,Intensity:%d\n", opPinColor, opPinBlink, opPinIntensity);
    oledStatus.updateMenu(&DengSet, true, true);
}

void Humiture_ISR()
{
    // 输出温度/湿度
    oled.clearDisplay();
    oled.setCursor(1, 0);
    oled.setTextColor(SSD1306_WHITE);
    oled.setTextSize(2);
    oled.printf("Humidity:\n%.2f%%\n", dht.readHumidity());
    oled.printf("Temp:\n%.2fC", dht.readTemperature());
    oled.display();
    btnTrue();
}
static void Detect_Humiture()
{
    if (isnan(dht.readHumidity()) || isnan(dht.readTemperature()))
        dht.begin(); // 初始化DHT11传感器
    if (oledStatus.Screen)
    {
        // Serial.printf("N_Mode\n");
        oled.clearDisplay();
        oled.setCursor(1, 0);
        oled.setTextColor(SSD1306_WHITE);
        oled.setTextSize(2);
        oled.println("*Humiture");
        oled.display();
        btn = false;
        ticker1.attach(2, Humiture_ISR);
    }
}

double sampleSoundPeak()
{
    double startMillis = millis(); // 记录采样开始的时间
    int signalMax = 0;
    int signalMin = 1024;
    int sample;

    while ((millis() - startMillis) < sampleWindow) // 当时间小于采样窗口时继续采样
    {
        sample = analogRead(MAX_4466_IN); // 读取输入口的采样值
        if (sample < 1024)                // 采集声音的最高值与最低值
        {
            if (sample > signalMax) // 用这个语法采到一个周期50ms内最高的声音
                signalMax = sample;
            else if (sample < signalMin) // 用这个语法采到一个周期50ms内最低的声音
                signalMin = sample;
        }
    }
    int peakDifference = signalMax - signalMin; // 得到幅值，返回幅值
    return peakDifference;
}
void VOL_Voltage_ISR()
{
    double peakDifference = sampleSoundPeak(); // 采集声音的最大幅值
    peakDifference = sampleSoundPeak();        // 采集声音的最大幅值
    oled.clearDisplay();
    oled.setCursor(1, 0);
    oled.printf("VOL:%s\n", (peakDifference < 340) ? "Low" : (peakDifference < 680) ? "Mid"
                                                                                    : "High");
    oled.printf("VIN:%.2fV", (peakDifference < 29) ? 0 : (peakDifference * 3.3) / 1024);
    oled.display();
    analogWrite(Red, (peakDifference < 29) ? 0 : peakDifference / 5);
    btnTrue();
}
static void Detect_VOL_Voltage()
{
    if (oledStatus.Screen)
    {
        oled.clearDisplay();
        oled.setCursor(1, 0);
        oled.setTextColor(SSD1306_WHITE);
        oled.setTextSize(2);
        oled.println("*VOL_Voltage");
        oled.display();
        btn = false;
        ticker1.attach_ms(sampleWindow, VOL_Voltage_ISR);
    }
}

#endif
