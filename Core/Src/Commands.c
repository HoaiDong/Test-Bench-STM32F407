#include "Commands.h"
#include "DataType.h"
#include "Packet.h"
#include "Sensor.h"
#include <stdlib.h>
#include <string.h>
#include "Interrupt.h"


// FUNCTION PROTOTYPE
//========================================================================================
static void PacketCANFrame(CommLCD Command, uint8_t DataLength, CAN_TxHeaderTypeDef *TxHeader);

static uint32_t BigToLittleEdian(uint32_t value);
//========================================================================================




// Các hàm send lên UI
//========================================================================================
void CommUIConnect(void)
{
    //uint8_t data[] = {0x02, 0x01, 0x01, 0x21, 0x10, 0x03}; // COMM_UI_CONNECT frame
    //HAL_UART_Transmit(&huart2, data, sizeof(data), HAL_MAX_DELAY); // Blocking mode


    uint8_t data = COMM_UI_CONNECT;
    uint8_t* dataframe = PacketData(&data, sizeof(data));
    HAL_UART_Transmit(&huart2, dataframe, strlen(dataframe), HAL_MAX_DELAY);
}
void CommUIConnectAlive(void)
{
     //uint8_t data[] = {0x02, 0x01, 0x02, 0x42, 0x20, 0x03}; // COMM_UI_CONNECT_ALIVE frame
     //HAL_UART_Transmit(&huart2, data, sizeof(data), HAL_MAX_DELAY); // Blocking mode


    uint8_t data = COMM_UI_CONNECT_ALIVE;
    uint8_t* dataframe = PacketData(&data, sizeof(data));
    HAL_UART_Transmit(&huart2, dataframe, strlen(dataframe), HAL_MAX_DELAY);
}
void CommUIDisconnect(void)
{
    // uint8_t data[] = {0x02, 0x01, 0x03, 0x00, 0x00, 0x03}; // COMM_UI_DISCONNECT frame
    // HAL_UART_Transmit(&huart2, data, sizeof(data), HAL_MAX_DELAY); // Blocking mode

    uint8_t data = COMM_UI_CONNECT;
    uint8_t* dataframe = PacketData(&data, sizeof(data));
    HAL_UART_Transmit(&huart2, dataframe, strlen(dataframe), HAL_MAX_DELAY);
}

void CommUIRealtimeData(void)
{
    //Biến đổi các giá trị cảm biến thành số thực
    uint32_t RPM  = BigToLittleEdian((float)Speed_RPM * 1000);
    uint32_t Torque = BigToLittleEdian((float)Torque_Value * 1000);                        // 2.34 mV/Nm is the sensitivity of the sensor
    uint32_t Vol_Votol = BigToLittleEdian((float)Votol_Voltage * 1000);               // 200k/10k voltage divider;
    uint32_t Vol_Generator = BigToLittleEdian((float)Generator_Voltage * 1000);       // 200k/10k voltage divider;
    uint32_t Cur_Votol = BigToLittleEdian((float)Votol_Current * 1000);         //50.3mV/A is the sensitivity of the sensor, 2358 is the offset
    uint32_t Cur_Generator = BigToLittleEdian((float)Generator_Current* 1000); //50.3mV/A is the sensitivity of the sensor, 2358 is the offset


    //cấp phát động
    uint8_t* data = (uint8_t*) malloc(25);
    data[0] = COMM_UI_REALTIME_DATA;
    memcpy(&data[1], &RPM, sizeof(float));
    memcpy(&data[5], &Torque, sizeof(float));
    memcpy(&data[9], &Vol_Votol, sizeof(float));
    memcpy(&data[13], &Vol_Generator, sizeof(float));
    memcpy(&data[17], &Cur_Votol, sizeof(float));
    memcpy(&data[21], &Cur_Generator, sizeof(float));

    uint8_t* dataframe = PacketData(data, 25);

    HAL_UART_Transmit(&huart2, dataframe, 30, HAL_MAX_DELAY);

    //Giải phóng cấp phát động
    free(data);
}

void CommUIConfirmWriteSetupData(void)
{
	uint8_t data = COMM_UI_WRITE_SETUP;
	uint8_t* dataframe = PacketData(&data, sizeof(data));
	HAL_UART_Transmit(&huart2, dataframe, 6, HAL_MAX_DELAY);
}

void CommUIReadSetupData(void)
{
    //Biến đổi các giá trị cảm biến thành số thực
    uint32_t Speed_Disc_Tooth_UI  = (uint32_t)BigToLittleEdian(Speed_Disc_Tooth*1000);    //Đang bug, gửi lên UI phải *1000 UI mới đọc được
    uint32_t Cur_Sensitive_UI = (uint32_t)BigToLittleEdian((float)Cur_Sensitive * 1000);
    uint32_t Torque_Sensitive_UI = (uint32_t)BigToLittleEdian((float)Torque_Sensitive * 1000);

    //cấp phát động
    uint8_t* data = (uint8_t*) malloc(13);
    data[0] = COMM_UI_READ_SETUP;
    memcpy(&data[1], &Speed_Disc_Tooth_UI, sizeof(uint32_t));
    memcpy(&data[5], &Cur_Sensitive_UI, sizeof(float));
    memcpy(&data[9], &Torque_Sensitive_UI, sizeof(float));

    uint8_t* dataframe = PacketData(data, 13);

    HAL_UART_Transmit(&huart2, dataframe, 18, HAL_MAX_DELAY);

    //Giải phóng cấp phát động
    free(data);
}

static uint32_t BigToLittleEdian(uint32_t value)
{
  return  ((value >> 24) & 0x000000FF) |
            ((value >> 8)  & 0x0000FF00) |
            ((value << 8)  & 0x00FF0000) |
            ((value << 24) & 0xFF000000);
}
//========================================================================================















// Các hàm send lên LCD
//========================================================================================
static void PacketCANFrame(CommLCD Command, uint8_t DataLength, CAN_TxHeaderTypeDef *TxHeader)
{
    // Cấu hình header CAN
    TxHeader->StdId = Command;          // CAN ID 11-bit (0x0 đến 0x7FF)
    TxHeader->ExtId = 0;                               // Không dùng Extended ID
    TxHeader->IDE = CAN_ID_STD;                        // Chuẩn ID (11-bit)
    TxHeader->RTR = CAN_RTR_DATA;                      // Khung dữ liệu (không phải Remote Frame)
    TxHeader->DLC = DataLength;                                 // Độ dài dữ liệu (0-8 byte)
    TxHeader->TransmitGlobalTime = DISABLE;
}

// Truyền giá trị state machine theo kiểu bigendian qua CAN  
void CommLCDStateMachine(uint8_t CurrentStateMachine)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;  // Mailbox để theo dõi trạng thái gửi
    uint8_t data = CurrentStateMachine;
    

    PacketCANFrame(COMM_LCD_STATE_MACHINE, 1, &TxHeader);

    // Đợi đến khi ít nhất 1 mailbox trống
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0)
    {
        // có thể thêm timeout hoặc watchdog ở đây
        // Nếu quá 5s không có tín hiệu duy trì kết nối LCD thì là lỗi
        if(!Timeout5sLCD)
        {
            return;
        }
    }

    // Gửi dữ liệu qua CAN1
//    if ((HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)) > 0)  // Kiểm tra mailbox trôngs để gửi data
//    {
        if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &data, &TxMailbox) != HAL_OK)
        {
            // Xử lý lỗi nếu gửi thất bại
            Error_Handler();
        }
//    }
}
void CommLCDConnect(void)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;  // Mailbox để theo dõi trạng thái gửi
    uint8_t data = 0;


    PacketCANFrame(COMM_LCD_CONNECT, 0, &TxHeader);
    // Gửi dữ liệu qua CAN1
    if ((HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)) > 0)  // Kiểm tra mailbox trôngs để gửi data 
    {
        if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &data, &TxMailbox) != HAL_OK)
        {
            // Xử lý lỗi nếu gửi thất bại
            Error_Handler();
        }
    }
}
void CommLCDConnectAlive(void)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;  // Mailbox để theo dõi trạng thái gửi
    uint8_t data = 0;


    PacketCANFrame(COMM_LCD_CONNECT_ALIVE, 0, &TxHeader);

    // Đợi đến khi ít nhất 1 mailbox trống
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0)
    {
        // có thể thêm timeout hoặc watchdog ở đây
//         Nếu quá 5s không có tín hiệu duy trì kết nối LCD thì là lỗi
        if(!Timeout5sLCD)
        {
            return;
        }
    }

    // Gửi dữ liệu qua CAN1
//    if ((HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)) > 0)  // Kiểm tra mailbox trôngs để gửi data
//    {
        if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &data, &TxMailbox) != HAL_OK)
        {
            // Xử lý lỗi nếu gửi thất bại
//            Error_Handler();
        }
//    }
}

// Truyền giá trị dòng tốc độ bánh xe và momen theo kiểu bigendian qua CAN  
void CommLCDRPMAndTorque(uint16_t SpeedRPM, uint32_t TorqueValue)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;  // Mailbox để theo dõi trạng thái gửi

    // Truyền data theo kiểu big endian 
    uint8_t data[6];
    data[0] = SpeedRPM >> 8;    // Byte cao nhất (MSB)
    data[1] = SpeedRPM >> 0;    // Byte thấp nhất (LSB)
    data[2] = TorqueValue >> 24;  // Byte cao nhất (MSB) 
    data[3] = TorqueValue >> 16;  // Byte thứ hai
    data[4] = TorqueValue >> 8;   // Byte thứ ba
    data[5] = TorqueValue >> 0;   // Byte thấp nhất (LSB)

    PacketCANFrame(COMM_LCD_RPM_TORQUE, 6, &TxHeader);


    // Đợi đến khi ít nhất 1 mailbox trống
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0)
    {
        // có thể thêm timeout hoặc watchdog ở đây
        // Nếu quá 5s không có tín hiệu duy trì kết nối LCD thì là lỗi
        if(!Timeout5sLCD)
        {
            return;
        }
    }


    // Gửi dữ liệu qua CAN1
//    if ((HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)) > 0)  // Kiểm tra mailbox trôngs để gửi data
//    {
        if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, data, &TxMailbox) != HAL_OK) 
        {
            // Xử lý lỗi nếu gửi thất bại
            Error_Handler();
        }
//    }
}

// Truyền giá trị điện áp votol và điện áp máy phát theo kiểu bigendian qua CAN
void CommLCDVoltage(uint32_t VolVotolValue, uint32_t VolGeneratorValue)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;  // Mailbox để theo dõi trạng thái gửi

    // Truyền data theo kiểu big endian 
    uint8_t data[8];
    data[0] = VolVotolValue >> 24;  // Byte cao nhất (MSB) 
    data[1] = VolVotolValue >> 16;  // Byte thứ hai
    data[2] = VolVotolValue >> 8;   // Byte thứ ba
    data[3] = VolVotolValue >> 0;   // Byte thấp nhất (LSB)
    data[4] = VolGeneratorValue >> 24; // Byte cao nhất (MSB) 
    data[5] = VolGeneratorValue >> 16; // Byte thứ hai 
    data[6] = VolGeneratorValue >> 8;  // Byte thứ ba
    data[7] = VolGeneratorValue >> 0;  // Byte thấp nhất (LSB)


    PacketCANFrame(COMM_LCD_VOLTAGE, 8, &TxHeader);

    // Đợi đến khi ít nhất 1 mailbox trống
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0)
    {
        // có thể thêm timeout hoặc watchdog ở đây
        // Nếu quá 5s không có tín hiệu duy trì kết nối LCD thì là lỗi
        if(!Timeout5sLCD)
        {
            return;
        }
    }


    // Gửi dữ liệu qua CAN1
//    if ((HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)) > 0)  // Kiểm tra mailbox trôngs để gửi data
//    {
        if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, data, &TxMailbox) != HAL_OK)
        {
            // Xử lý lỗi nếu gửi thất bại
            Error_Handler();
        }
//    }
}

// Truyền giá trị dòng điện votol và dòng điện máy phát theo kiểu bigendian qua CAN 
void CommLCDCurrent(uint32_t CurrentVotolValue, uint32_t CurrentGenneratorValue)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;  // Mailbox để theo dõi trạng thái gửi

    // Truyền data theo kiểu big endian 
    uint8_t data[8];
    data[0] = CurrentVotolValue >> 24;  // Byte cao nhất (MSB) 
    data[1] = CurrentVotolValue >> 16;  // Byte thứ hai
    data[2] = CurrentVotolValue >> 8;   // Byte thứ ba
    data[3] = CurrentVotolValue >> 0;   // Byte thấp nhất (LSB)
    data[4] = CurrentGenneratorValue >> 24; // Byte cao nhất (MSB) 
    data[5] = CurrentGenneratorValue >> 16; // Byte thứ hai 
    data[6] = CurrentGenneratorValue >> 8;  // Byte thứ ba
    data[7] = CurrentGenneratorValue >> 0;  // Byte thấp nhất (LSB)


    PacketCANFrame(COMM_LCD_CURRENT, 8, &TxHeader);

    // Đợi đến khi ít nhất 1 mailbox trống
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0)
    {
        // có thể thêm timeout hoặc watchdog ở đây
        // Nếu quá 5s không có tín hiệu duy trì kết nối LCD thì là lỗi
        if(!Timeout5sLCD)
        {
            return;
        }
    }


    // Gửi dữ liệu qua CAN1
//    if ((HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)) > 0)  // Kiểm tra mailbox trôngs để gửi data
//    {
        if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, data, &TxMailbox) != HAL_OK)
        {
            // Xử lý lỗi nếu gửi thất bại
            Error_Handler();
        }
//    }
}
void CommLCDLoad(uint8_t LoadOnOff, uint8_t LoadLevel)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;  // Mailbox để theo dõi trạng thái gửi

    uint8_t data[2];
    data[0] = LoadOnOff; 
    data[1] = LoadLevel;

    PacketCANFrame(COMM_LCD_LOAD, 2, &TxHeader);

    // Đợi đến khi ít nhất 1 mailbox trống
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0)
    {
        // có thể thêm timeout hoặc watchdog ở đây
        // Nếu quá 5s không có tín hiệu duy trì kết nối LCD thì là lỗi
        if(!Timeout5sLCD)
        {
            return;
        }
    }

    // Gửi dữ liệu qua CAN1
//    if ((HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)) > 0)  // Kiểm tra mailbox trôngs để gửi data
//    {
        if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, data, &TxMailbox) != HAL_OK)
        {
            // Xử lý lỗi nếu gửi thất bại
            Error_Handler();
        }
//    }
}
void CommLCDDetectSensor(void)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;  // Mailbox để theo dõi trạng thái gửi
    uint8_t data = 0;


    PacketCANFrame(COMM_LCD_DETECT_SENSOR, 0, &TxHeader);
    // Gửi dữ liệu qua CAN1
    if ((HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)) > 0)  // Kiểm tra mailbox trôngs để gửi data 
    {
        if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &data, &TxMailbox) != HAL_OK)
        {
            // Xử lý lỗi nếu gửi thất bại
            Error_Handler();
        }
    }
}
void CommLCDFaultCode(uint16_t FaultMask)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;  // Mailbox để theo dõi trạng thái gửi

    uint8_t data[2];
    // Truyền data theo kiểu big endian; 
    data[0] = FaultMask >> 8;   // Byte cao nhất (MSB)
    data[1] = FaultMask >> 0;   // Byte thấp nhất (LSB)


    PacketCANFrame(COMM_LCD_FAULT_CODE, 2, &TxHeader);

    // Đợi đến khi ít nhất 1 mailbox trống
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0)
    {
        // có thể thêm timeout hoặc watchdog ở đây
        // Nếu quá 5s không có tín hiệu duy trì kết nối LCD thì là lỗi
        if(!Timeout5sLCD)
        {
            return;
        }
    }

    // Gửi dữ liệu qua CAN1
    if ((HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)) > 0)  // Kiểm tra mailbox trôngs để gửi data
    {
        if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, data, &TxMailbox) != HAL_OK)
        {
            // Xử lý lỗi nếu gửi thất bại
            Error_Handler();
        }
    }
}

// Gửi giá trị tay ga qua CAN 
void CommLCDThrottle(uint32_t ThrottleValue)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;  // Mailbox để theo dõi trạng thái gửi
    
    // Truyền data theo kiểu big endian 
    uint8_t data[4];
    data[0] = ThrottleValue >> 24;  // Byte cao nhất (MSB) 
    data[1] = ThrottleValue >> 16;  // Byte thứ hai
    data[2] = ThrottleValue >> 8;   // Byte thứ ba
    data[3] = ThrottleValue >> 0;   // Byte thấp nhất (LSB)

    PacketCANFrame(COMM_LCD_THROTTLE, 4, &TxHeader);

    // Đợi đến khi ít nhất 1 mailbox trống
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0)
    {
        // có thể thêm timeout hoặc watchdog ở đây
        // Nếu quá 5s không có tín hiệu duy trì kết nối LCD thì là lỗi
        if(!Timeout5sLCD)
        {
            return;
        }
    }

    // Gửi dữ liệu qua CAN1
//    if ((HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)) > 0)  // Kiểm tra mailbox trôngs để gửi data
//    {
        if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, data, &TxMailbox) != HAL_OK)
        {
            // Xử lý lỗi nếu gửi thất bại
            Error_Handler();
        }
//    }
}

void CommLCDUIConnectionStatus(uint8_t Status)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;  // Mailbox để theo dõi trạng thái gửi
    
    // Truyền data theo kiểu big endian 
    uint8_t data = Status;


    PacketCANFrame(COMM_LCD_UI_CONNECTION_STATUS, 1, &TxHeader);

    // Đợi đến khi ít nhất 1 mailbox trống
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0)
    {
        // có thể thêm timeout hoặc watchdog ở đây
        // Nếu quá 5s không có tín hiệu duy trì kết nối LCD thì là lỗi
        if(!Timeout5sLCD)
        {
            return;
        }
    }

    // Gửi dữ liệu qua CAN1
    if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, data, &TxMailbox) != HAL_OK)
    {
        // Xử lý lỗi nếu gửi thất bại
        Error_Handler();
    }

}
//========================================================================================
