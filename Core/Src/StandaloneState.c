#include "StandaloneState.h"
#include "Interrupt.h"
#include "Commands.h"
#include "Error.h"
#include "Sensor.h"
#include "Packet.h"


// FUNCTION PROTOTYPE 
// CÁC FUNCTION NÀY LÀ FUNCTION CỤC BỘ CỦA RIÊNG STATE NÀY 
//========================================================================================
// Giao tiếp với LCD 
static void LCDCommunication(void);

// Giao tiếp với UI
static void UICommunication(void);
static void UIReceive(void);
// Xử lý data nhận về từ UART
static void ProcessPacket(void);

// Điều khiển mô hình 
static void SystemControl(void);
//========================================================================================




// Hàm main 
//========================================================================================
//========================================================================================
void StandaloneState(StateMachine *CurrentStateMachine)
{

    while(1)
    {
        // Đọc cảm biến
        ReadSensor();
        
        // Điều khiển mô hình
        SystemControl();

        // Giao tiếp với LCD
        LCDCommunication();

        // Giao tiếp với UI
        UICommunication();

        // Reset cờ 100ms 
        if (Timer100msFlag == 1)
        {
            Timer100msFlag = 0;
        }

        // Nếu nhận được tín hiệu kết nối từ UI thì chuyển về trạng thái UI 
        if (UIConnectionStatus == CONNECTED)
        {
            *CurrentStateMachine = STANDBY_STATE;
            return;
        }

        CheckOverrall(); // Kiểm tra hệ thống theo chu kì, cập nhật biến "FaultMask"
        // Kiểm tra lỗi hệ thống, nếu có lỗi thì chuyển trạng thái 
        if (FaultMask != FAULT_NONE)
        {
          *CurrentStateMachine = FAULT_STATE;
          return;
        }

    }
    
}
//========================================================================================
//========================================================================================










//========================================================================================
static void LCDCommunication(void)
{
    if (Timer100msFlag)
    {
        CommLCDConnectAlive();                              // Gửi tín hiệu alive to LCD để xác nhận kết nối
        CommLCDStateMachine(STANDALONE_STATE);              // Gửi state machine 
        CommLCDRPMAndTorque(Speed_RPM, Torque_ADC * (float)3000 / 4096 / Torque_Sensitive);         // Gửi giá trị tốc độ bánh xe và mômen
        CommLCDCurrent(Cur_Votol_ADC * (float)3000 / 4096 / Cur_Sensitive, Cur_Generator_ADC * (float)3000 / 4096 / Cur_Sensitive);   // Gửi giá trị dòng điện votol và dòng điện máy phát
        CommLCDVoltage((float)Vol_Votol_ADC * 3 / 4095 * 209.9/9.9, (float)Vol_Generator_ADC * 3 / 4095 * 209.9/9.9);   // Gửi giá trị điện áp votol và điện áp máy phát


        // CommLCDLoad(Load_On_Off_Signal, Load_Level_Signal); // Gửi giá trị on/off tải và mức tải
    }

}

static void UICommunication(void)
{
    UIReceive();
}


static void UIReceive(void)
{
    if (Count > 0)
    {
        if (TryDecodePacket() == 1)
        {
            ProcessPacket();
        }
    }
}

static void ProcessPacket(void)
{
  CommUI id = (CommUI)DataFrame[2];  // Lấy giá trị ID Byte từ frame nhận được

  // Dựa vào ID để thực hiện tác vụ
  switch (id)
  {
    case COMM_UI_CONNECT:
    Timeout5sUI = 50;
    UIConnectionStatus = CONNECTED;
    // gửi lại connect
    CommUIConnect();
    break;
  }
}



// Điều khiển mô hình 
static void SystemControl(void)
{

    // Điều khiển tay ga 
    if (Throttle_ADC <= 2978)
    {
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, adc1_buffer[0] + 620);
    }

    // Công tắt bật tải 
    if (Load_On_Off_Signal == SWITCH_ON)
    {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
    }
    else
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);
	}


    // Công tắt mức tải 
    switch (Load_Level_Signal)
    {
        case LOAD_1:    // Mức tải 1 
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
        break;
    
        case LOAD_2:    // Mức tải 2
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
        break;

        case LOAD_3:    // Mức tải 3 
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
        break;

        case LOAD_4:    // Mức tải 4 
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
        break;
    }

}
//========================================================================================
