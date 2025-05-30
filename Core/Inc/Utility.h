#ifndef ULTILITY_H
#define ULTILITY_H

#include "Sensor.h"

typedef enum _LED
{
    LED_GREEN = 0,
    LED_YELLOW = 1,
    LED_RED = 2
}LED;

// Bật led cảnh báo 
void AlarmLed(LED Led);

// Điều chỉnh tải và mức tải
void SetLoad(LOAD_LEVEL Level);

// Xuất giá trị DAC cho driver Votol
void SetMotor(unsigned short Value);
#endif