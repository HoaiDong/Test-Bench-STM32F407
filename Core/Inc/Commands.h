#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

// Xử lý 1 data frame nhận về
//void ProcessPacket(void);

// Các hàm send lên UI
void CommUIConnect(void);
void CommUIConnectAlive(void);
void CommUIDisconnect(void);
void CommUIRealtimeData(void);
void CommUIConfirmWriteSetupData(void);
void CommUIReadSetupData(void);

// Các hàm send lên LCD
void CommLCDStateMachine(uint8_t CurrentStateMachine);
void CommLCDConnect(void);
void CommLCDConnectAlive(void);
void CommLCDRPMAndTorque(uint16_t SpeedRPM, uint32_t TorqueValue);
void CommLCDVoltage(uint32_t VolVotolValue, uint32_t VolGeneratorValue);
void CommLCDCurrent(uint32_t CurrentVotolValue, uint32_t CurrentGenneratorValue);  // Truyền giá trị dòng điện votol và dòng điện máy phát theo kiểu bigendian qua CAN
void CommLCDLoad(uint8_t LoadOnOff, uint8_t LoadLevel);
void CommLCDDetectSensor(void);
void CommLCDFaultCode(uint16_t FaultMask);
void CommLCDThrottle(uint32_t ThrottleValue);



#endif
