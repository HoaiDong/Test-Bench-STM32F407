#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>

#define INVALID_REFERENCE 0xFFFF

typedef enum _CONNECTION_STATUS
{
    DISCONNECT = 0,
    CONNECTED = 1,
}CONNECTION_STATUS;

// Trạng thái kết nối với UI 
extern CONNECTION_STATUS UIConnectionStatus;

// Trạng thái kết nối với LCD 
extern CONNECTION_STATUS LCDConnectionStatus;

// Mask thể hiện lỗi của hệ thống 16 bit 
extern uint16_t FaultMask;

// Giá trị DAC mà động cơ quay được, để làm mốc kiểm tra speed sensor
extern uint16_t DACSpeedSensorReference1;
extern uint16_t DACSpeedSensorReference2;

extern uint16_t ADCCurrentVotolReference1;
extern uint16_t ADCCurrentVotolReference2;

void CheckOverrall(void);
void CheckSensor(void);
void CheckLCD(void);
void CheckLoadWire(void);
void CheckOpenCircuit(void);
void CheckUI(void);

// Chạy motor để kiểm tra speed sensor
void RunMotor(uint16_t *DACSpeedSensorReference, uint16_t *ADCCurrentVotolReference);
#endif
