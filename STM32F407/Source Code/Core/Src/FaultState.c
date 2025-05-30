#include "FaultState.h"
#include "Interrupt.h"
#include "Commands.h"
#include "Error.h"
#include "DataType.h"
#include "Utility.h"
#include "Packet.h"

// Điều khiển mô hình 
static void SystemControl(void);

// Giao tiếp với LCD
static void LCDCommunication(void);



//=================================================================================================
void FaultState(StateMachine CurrentStateMachine)
{
    while (1)
    {
    	// Tắt điều khiển hệ thống
        SystemControl();

        // Giao tiếp với LCD
        LCDCommunication();

		// Bật đèn led đỏ để cảnh báo
        AlarmLed(LED_YELLOW);

		// Kiểm tra hệ thống theo chu kì, cập nhật biến "FaultMask"
        CheckOverrall(); 

        TryDecodePacket();

    }
    
}

// Điều khiển mô hình
static void SystemControl(void)
{

	SetMotor(MIN_DAC_MOTOR);	// Tắt mô tơ
	SetLoad(LOAD_0);			// Tắt tải

}


// Giao tiếp với LCD
static void LCDCommunication(void)
{
    if (Timer100msFlag)
    {
        CommLCDConnectAlive();
        CommLCDFaultCode(FaultMask);
        CommLCDStateMachine(FAULT_STATE);

        Timer100msFlag = 0;
    }
}

//=======================================================================================


