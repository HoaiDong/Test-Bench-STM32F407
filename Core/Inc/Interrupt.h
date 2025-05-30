#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

#define TIMEOUT_UI	500
#define TIMEOUT_LCD 500
#define TIMEOUT_SWAP_STATE 400
#define TIMEOUT_RUN_SYSTEM 250
#define TIMEOUT_INCREASE_STEP_RUN_MOTOR 15


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

extern uint16_t TimeoutRunSystem;	// Thời gian để kiểm tra hệ thống
extern uint16_t TimeoutIncreaseStepRunMotor; // Thời gian để chạy động cơ tăng dần
extern uint8_t  StepRunSystem;      // Các bước chạy hệ thống 

#endif
