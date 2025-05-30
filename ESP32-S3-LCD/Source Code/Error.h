#ifndef ERROR_H
#define ERROR_H

#include <Arduino.h>

// return 0 : error
// return 1: none
unsigned char CheckSTMConnection(void);

void STM32ConnectAlive(void);

#endif