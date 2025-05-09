#include "FaultState.h"
#include "Interrupt.h"
#include "Commands.h"
#include "Error.h"

// Điều khiển mô hình 
static void SystemControl(void);

// Giao tiếp với LCD
static void LCDCommunication(void);


void FaultState(StateMachine CurrentStateMachine)
{
    while (1)
    {
    	// Tắt điều khiển hệ thống
        SystemControl();

        // Giao tiếp với LCD
        LCDCommunication();

        CheckOverrall(); // Kiểm tra hệ thống theo chu kì, cập nhật biến "FaultMask"

    }
    
}

// Điều khiển mô hình
static void SystemControl(void)
{

    // Điều khiển tay ga
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 620);


    // Công tắt bật tải
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);


    // Công tắt mức tải
   // Mức tải 1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);


}


// Giao tiếp với LCD
static void LCDCommunication(void)
{
    if (Timer100msFlag)
    {
        CommLCDConnectAlive();
        CommLCDStateMachine(FAULT_STATE);
        CommLCDFaultCode(FaultMask);

        Timer100msFlag = 0;
    }
}

