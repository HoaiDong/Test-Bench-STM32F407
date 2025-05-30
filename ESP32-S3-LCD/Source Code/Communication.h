#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include "driver/twai.h"
#include "Error.h"

extern twai_message_t rx_message;

// Khởi tạo CAN 
void InitCAN(void);

// Các hàm send xuống STM32
void CommLCDConnectAlive(void);
void CommLCDRunSystem(void);


// Hàm đọc CAN 
unsigned char CANReceive(void);


void checkAndRecoverTWAI(void); 

#endif