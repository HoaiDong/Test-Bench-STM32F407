#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>

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




void CheckOverrall(void);
void CheckSensor(void);
void CheckLCD(void);
void CheckLoadWire(void);
void CheckOpenCircuit(void);
void CheckUI(void);


#endif
