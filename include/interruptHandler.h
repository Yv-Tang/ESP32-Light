#ifndef INTERRUPTHANDLER_H
#define INTERRUPTHANDLER_H

#include "config.h"
#include "main.h"

extern OLEDStatus oledStatus;

// 中断标志位
bool btn = true;
volatile bool flagNext = false;
volatile bool flagSelect = false;
volatile bool flagBack = false;

// 外部中断服务函数声明
void ISRNext();
void ISRSelect();
void ISRBack();

// 中断服务程序
void ISRNext()
{
  static unsigned long lastInterruptTime = 0;
  if (millis() - lastInterruptTime > 200)
  { // 200ms 防抖
    flagNext = true;
    lastInterruptTime = millis();
  }
}

void ISRSelect()
{
  static unsigned long lastInterruptTime = 0;
  if (millis() - lastInterruptTime > 200)
  {
    flagSelect = true;
    lastInterruptTime = millis();
  }
}

void ISRBack()
{
  static unsigned long lastInterruptTime = 0;
  if (millis() - lastInterruptTime > 200)
  {
    flagBack = true;
    lastInterruptTime = millis();
  }
}

void interruptInit()
{
  attachInterrupt(digitalPinToInterrupt(btnPin1), ISRBack, FALLING);
  attachInterrupt(digitalPinToInterrupt(btnPin2), ISRSelect, FALLING);
  attachInterrupt(digitalPinToInterrupt(btnPin3), ISRNext, FALLING);
}

void interruptHandler()
{
  static unsigned long lastHeartbeat = 0;
  static bool lighted = false;

  // 按键处理器
  if (btn)
  {
    // 处理中断标志位
    if (flagNext)
    {
      oledStatus.moveDown();
      flagNext = false;
    }
    if (flagSelect)
    {
      oledStatus.select();
      flagSelect = false;
    }
    if (flagBack)
    {
      oledStatus.back();
      flagBack = false;
    }

    if (millis() - lastHeartbeat > 1000)
    {
      analogWrite(ledPin, (lighted) ? 0 : 100);
      lighted = !lighted;
      lastHeartbeat = millis();
    }
  }
  else
  {
    if (lighted)
    {
      analogWrite(ledPin, 0);
      lighted = false;
    }
  }
}
#endif