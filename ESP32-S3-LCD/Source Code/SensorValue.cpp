#include "SensorValue.h"


unsigned int Throttle = 0;
SWITCH_STATUS LoadOnOff = SWITCH_OFF;
LOAD_LEVEL LoadLevel = LOAD_1;



unsigned short Speed = 0;
unsigned int Torque = 0;
float CurrentVotol = 0.0;
float CurrentGenerator = 0.0;
float VoltageVotol = 0.0;
float VoltageGenerator = 0.0;


CONNECTION_STATUS UIConnectStatus = DISCONNECTED;
