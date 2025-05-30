#include "Error.h"
#include "DataType.h"
#include "Interrupt.h"
#include "Sensor.h"
#include "Utility.h"

#define ADC_MIN_THRESH_HOLD 125  // Giá trị ADC tối thiểu để xác định có bị thiếu sensor hay không   
#define SPEED_THRESHOLD 30 // Giá trị tốc độ tối thiểu để xác định có bị thiếu sensor hay không
#define DAC_STEP_RUN_MOTOR 30 // Step để tăng tốc motor từ từ


// Giá trị DAC mà động cơ quay được, để làm mốc kiểm tra speed sensor
uint16_t DACSpeedSensorReference1 = INVALID_REFERENCE; // lúc tắt tải
uint16_t DACSpeedSensorReference2 = INVALID_REFERENCE; // lúc bật tải 

// Giá trị ADC tối thiểu của votol mà động cơ quay được, để làm tham chiếu kiểm tra 
uint16_t ADCCurrentVotolReference1 = INVALID_REFERENCE; // lúc tắt tải
uint16_t ADCCurrentVotolReference2 = INVALID_REFERENCE; // lúc bật tải

//static uint16_t ADCCurrentGeneratorReference1 = INVALID_REFERENCE;


// Trạng thái kết nối với LCD 
CONNECTION_STATUS LCDConnectionStatus = DISCONNECT;

// Trạng thái kết nối với UI 
CONNECTION_STATUS UIConnectionStatus = DISCONNECT;

// Mask thể hiện lỗi của hệ thống 16 bit 
FaultCode FaultMask = FAULT_NONE;


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
     if (Torque_ADC <= ADC_MIN_THRESH_HOLD)
     {
         FaultMask |= FAULT_TORQUE_SENSOR_MISSING;
     }

    // if (Throttle_ADC < ADC_MIN_THRESH_HOLD)
    // {
    //     FaultMask |= FAULT_THROTTLE_MISSING;

    // }

    // Kiểm tra lỗi mất cảm biến tốc độ khi tắt tải 
    if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0) == GPIO_PIN_RESET)
    {
        if (DAC_Motor_Value > DACSpeedSensorReference1 && Speed_RPM == 0)
        {
            FaultMask |= FAULT_SPEED_SENSOR_MISSING;
        }
    }
    else    // Kiểm tra lỗi mất cảm biến tốc độ khi bật tải
    {
        if (DAC_Motor_Value > DACSpeedSensorReference2 && Speed_RPM == 0)
        {
            FaultMask |= FAULT_SPEED_SENSOR_MISSING;
        }
    }

    if (Cur_Votol_ADC <= ADC_MIN_THRESH_HOLD)
    {
        FaultMask |= FAULT_CURRENT_VOTOL_SENSOR_MISSING;
    }

    if (Cur_Generator_ADC <= ADC_MIN_THRESH_HOLD)
    {
        FaultMask |= FAULT_CURRENT_GENERATOR_SENSOR_MISSING;
    }

    if (Vol_Votol_ADC <= ADC_MIN_THRESH_HOLD)
    {
        FaultMask |= FAULT_VOLTAGE_VOTOL_SENSOR_MISSING;
    }

    // Khi bật tải
    if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0) == GPIO_PIN_SET)
    {
    	if (Generator_Current >= 0.5f && Generator_Voltage < 0.5f)
    	{
                FaultMask |= FAULT_VOLTAGE_GENERATOR_SENSOR_MISSING;
    	}
    }

    // Khi tắt tải 
    if ((HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0) == GPIO_PIN_RESET))
    {
        if (ADCCurrentVotolReference1 != INVALID_REFERENCE)
        {
            if (Speed_RPM >= SPEED_THRESHOLD && Cur_Votol_ADC < ADCCurrentVotolReference1)
            {
                FaultMask |= FAULT_CURRENT_VOTOL_SENSOR_MISSING;
            }
        }
        
    }
    else
    {
        if (ADCCurrentVotolReference1 != INVALID_REFERENCE)
        {
            if (Speed_RPM >= SPEED_THRESHOLD && Cur_Votol_ADC < ADCCurrentVotolReference2)
            {
                FaultMask |= FAULT_CURRENT_VOTOL_SENSOR_MISSING;
            }
        }
        
    }




    // //====================== Phần thêm =================//
    // if (CheckStuckTorque(Torque_Value)) {
    //     FaultMask |= FAULT_TORQUE_SENSOR_STUCK;
    // }

    // if (CheckStuckSpeed(Speed_RPM)) {
    //     FaultMask |= FAULT_SPEED_SENSOR_STUCK;
    // }

    // if (CheckStuckCurrentVotol(Votol_Current)) {
    //     FaultMask |= FAULT_CURRENT_VOTOL_SENSOR_STUCK;
    // }

    // if (CheckStuckCurrentGenerator(Generator_Current)) {
    //     FaultMask |= FAULT_CURRENT_GENERATOR_SENSOR_STUCK;
    // }
    // //====================== Phần thêm =================//




    // //====================== Phần thêm =================//
    // // Motor đang cấp dòng mà speed = 0 hoặc stuck
    // if ((Votol_Current > 2.0f) && Speed_RPM == 0) 
    // {
    //     FaultMask |= FAULT_SPEED_SENSOR_MISSING;
    // }

    // // Torque lớn mà speed không thay đổi
    // if ((Torque_Value > 5.0f) && Speed_RPM == 0) 
    // {
    //     FaultMask |= FAULT_SPEED_SENSOR_STUCK;
    // }

    // // Speed tăng nhưng dòng gần 0
    // if ((Speed_RPM > 200) && (Votol_Current < 1.0f)) {
    //     WarningMask |= FAULT_CURRENT_VOTOL_SENSOR_MISSING;
    // }
    // //====================== Phần thêm =================//
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

// Chạy motor để lấy giá trị DACDACSpeedSensorReference
void RunMotor(uint16_t *DACSpeedSensorReference, uint16_t *ADCCurrentVotolReference)
{

	if (*DACSpeedSensorReference == INVALID_REFERENCE)
	{
		if (Speed_RPM > SPEED_THRESHOLD)
		{
			*DACSpeedSensorReference = DAC_Motor_Value + 50;
//            *ADCCurrentVotolReference = Cur_Votol_ADC - 50;
		}

		TimeoutRunSystem = TIMEOUT_RUN_SYSTEM;
	}


//    if (Speed_RPM > SPEED_THRESHOLD)
//    {
//        if (*DACSpeedSensorReference == INVALID_REFERENCE)
//        {
//            *DACSpeedSensorReference = DAC_Motor_Value + 100;
//        }
//    }

    // Nếu dòng điện cấp cho driver đạt 2A mà chưa có speed thì là lỗi
    if (Votol_Current >= 1.0f && Speed_RPM == 0)
    {
        SetMotor(MIN_DAC_MOTOR);
        FaultMask |= FAULT_SPEED_SENSOR_MISSING;
        return;
    }

    // Nếu giá trị DAC đạt max mà vẫn chưa có speed thì speed sensor bị lỗi
    if (DAC_Motor_Value >= MAX_DAC_MOTOR)
    {
        SetMotor(MIN_DAC_MOTOR);
        FaultMask |= FAULT_SPEED_SENSOR_MISSING;
        return;
    }


    // Timeout để tăng step DAC lên từ từ để tăng tốc động cơ 
	if (!TimeoutIncreaseStepRunMotor)
	{
		if (*DACSpeedSensorReference ==  INVALID_REFERENCE)
		{
            DAC_Motor_Value += DAC_STEP_RUN_MOTOR;
        }
        else
        {

        	DAC_Motor_Value = MIN_DAC_MOTOR;
        }

        SetMotor(DAC_Motor_Value);
        TimeoutIncreaseStepRunMotor = TIMEOUT_INCREASE_STEP_RUN_MOTOR;
	}




}


//=======================================================================================
// Hàm kiểm tra Torque Sensor bị stuck/frozen value
// static uint8_t CheckStuckTorque(float value)
// {
//     static float last_value = 0;
//     static uint8_t stuck_count = 0;

//     if (fabsf(value - last_value) <= STUCK_DELTA_TORQUE) {
//         stuck_count++;
//     } else {
//         stuck_count = 0;
//     }
//     last_value = value;

//     if (stuck_count > STUCK_THRESHOLD_TORQUE) {
//         return 1;   // Bị stuck
//     }
//     return 0;
// }

// // Hàm kiểm tra Current Sensor (Votol) bị stuck
// static uint8_t CheckStuckCurrentVotol(float value)
// {
//     static float last_value = 0;
//     static uint8_t stuck_count = 0;

//     if (fabsf(value - last_value) <= STUCK_DELTA_CURRENT) {
//         stuck_count++;
//     } else {
//         stuck_count = 0;
//     }
//     last_value = value;

//     if (stuck_count > STUCK_THRESHOLD_CURRENT) {
//         return 1;
//     }
//     return 0;
// }

// // Hàm kiểm tra Current Sensor (Generator) bị stuck
// static uint8_t CheckStuckCurrentGenerator(float value)
// {
//     static float last_value = 0;
//     static uint8_t stuck_count = 0;

//     if (fabsf(value - last_value) <= STUCK_DELTA_CURRENT) {
//         stuck_count++;
//     } else {
//         stuck_count = 0;
//     }
//     last_value = value;

//     if (stuck_count > STUCK_THRESHOLD_CURRENT) {
//         return 1;
//     }
//     return 0;
// }
