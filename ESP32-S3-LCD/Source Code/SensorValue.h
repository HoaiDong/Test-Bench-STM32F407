#ifndef SENSORVALUE_H
#define SENSORVALUE_H

#include "DataType.h"

extern unsigned int Throttle;
extern SWITCH_STATUS LoadOnOff;
extern LOAD_LEVEL LoadLevel;

extern unsigned short Speed;
extern unsigned int Torque;
extern float CurrentVotol;
extern float CurrentGenerator;
extern float VoltageVotol;
extern float VoltageGenerator;

extern CONNECTION_STATUS UIConnectStatus;

#endif