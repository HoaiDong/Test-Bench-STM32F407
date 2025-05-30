#include "Error.h"

#define INTERVAL 500 //5s
static unsigned long previousMillis = 0;
static unsigned long currentMillis = 0 ;              // lấy thời gian hiện tại

// return 0 : error
// return 1: none
unsigned char CheckSTMConnection(void)
{
    unsigned long currentMillis = millis();  // lấy thời gian hiện tại

    if (currentMillis - previousMillis >= INTERVAL)     // nếu đã ≥INTERVAL ms 
    {    
        return 0; // timeout
    }

    return 1; // vẫn còn trong thời gian hợp lệ
}

void STM32ConnectAlive() 
{
    previousMillis = millis();  // reset lại timer khi có tín hiệu
}

