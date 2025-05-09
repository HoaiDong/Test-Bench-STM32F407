#include "Error.h"
#include "DataType.h"
#include "Interrupt.h"
#include "Sensor.h"

#define ADC_MIN_THRESH_HOLD 150  // Giá trị ADC tối thiểu để xác định có bị thiếu sensor hay không 

// Trạng thái kết nối với LCD 
CONNECTION_STATUS LCDConnectionStatus = DISCONNECT;

// Trạng thái kết nối với UI 
CONNECTION_STATUS UIConnectionStatus = DISCONNECT;

// Mask thể hiện lỗi của hệ thống 16 bit 
uint16_t FaultMask = FAULT_NONE;

//========================================================================================

// Kiểm tra tổng thể, xem có lỗi hay không
//  0: không có lỗi
// !0: có lỗi
void CheckOverrall(void)
{
    if (TimerCheckErrorFlag == 1)
    {
        CheckSensor();
        CheckLCD();
        CheckLoadWire();
        CheckOpenCircuit();

        if (UIConnectionStatus == CONNECTED)
        {
            CheckUI();
        }

        TimerCheckErrorFlag = 0;
    }
}

void CheckSensor(void)
{
    // if (Torque_ADC <= ADC_MIN_THRESH_HOLD)
    // {
    //     FaultMask |= FAULT_TORQUE_SENSOR_MISSING;
    // }

    // if (Throttle_ADC < 370)
    // {
    //     FaultMask |= FAULT_THROTTLE_MISSING;

    // }

    if (0)
    {
        FaultMask |= FAULT_SPEED_SENSOR_MISSING;
    }

    if (Cur_Votol_ADC <= ADC_MIN_THRESH_HOLD)
    {
        FaultMask |= FAULT_CURRENT_VOTOL_SENSOR_MISSING;
    }

    if (Cur_Generator_ADC <= ADC_MIN_THRESH_HOLD)
    {
        FaultMask |= FAULT_CURRENT_GENERATOR_SENSOR_MISSING;
    }

//    if (Vol_Votol_ADC <= ADC_MIN_THRESH_HOLD)
//    {
//        FaultMask |= FAULT_VOLTAGE_VOTOL_SENSOR_MISSING;
//    }

    // if (Vol_Generator_ADC <= ADC_MIN_THRESH_HOLD)
    // {
    //     FaultMask |= FAULT_VOLTAGE_GENERATOR_SENSOR_MISSING;
    // }
    
}

// Kiểm tra kết nối LCD
void CheckLCD(void)
{
    // Nếu quá 5s không có tín hiệu duy trì kết nối LCD thì là lỗi 
    if(!Timeout5sLCD)
    {
        FaultMask |= FAULT_LCD_MISSING;
    }
}

void CheckLoadWire(void)
{
    if (0)
    {
        FaultMask |= FAULT_LOAD_WIRE1_BROKEN;
    }

    if (0)
    {
        FaultMask |= FAULT_LOAD_WIRE2_BROKEN;
    }

    if (0)
    {
        FaultMask |= FAULT_LOAD_WIRE3_BROKEN;
    }

    if (0)
    {
        FaultMask |= FAULT_LOAD_WIRE4_BROKEN;
    }
}

void CheckOpenCircuit(void)
{
    if (0)
    {
        FaultMask |= FAULT_OPEN_CIRCUIT;
    }
}


// Kiểm tra kết nối với UI
void CheckUI(void)
{
    // Nếu quá 5s không có tín hiệu duy trì kết nối UI thì là lỗi 
    if (!Timeout5sUI)
    {
        FaultMask |= FAULT_UI_MISSING;
    }
}
