#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

#define TIMEOUT_UI	50
#define TIMEOUT_SWAP_STATE 40

extern uint8_t RxData;

extern uint8_t Timer10MsFlag;
extern uint8_t Timer50MsFlag;
extern uint8_t Timer100msFlag;
extern uint8_t Timer500MsFlag;


extern uint8_t TimerCheckErrorFlag; // Timer kiểm tra lỗi toàn hệ thống theo chu kỳ
extern uint16_t Timeout5s;

extern uint16_t Timeout5sLCD; // Timeout kiểm tra tín hiệu LCD 

extern uint16_t Timeout5sUI; // cho kiểm tra tín hiệu UI
extern uint16_t TimeoutSwapState; // cho để swap state

#endif
