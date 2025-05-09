#include "StandbyState.h"
#include "Error.h"
#include "Packet.h"
#include "Commands.h"
#include "Interrupt.h"
#include "Sensor.h"

//========================================================================================
// Định nghĩa mô hình đã ở trạng thái an toàn chưa để điều khiển 
typedef enum _SAFE_STATE
{
    UN_SAFE = 0,
    SAFE = 1,
}SAFE_STATE;
//========================================================================================


// FUNCTION PROTOTYPE 
// CÁC FUNCTION NÀY LÀ FUNCTION CỤC BỘ CỦA RIÊNG STATE NÀY 
//========================================================================================
// Xử lý data nhận về từ UART
static void ProcessPacket(void);

// Giao tiếp với LCD 
static void LCDCommunication(void);

// Giao tiếp với UI
static void UICommunication(void);
static void UISend(void);
static void UIReceive(void);

// Kiểm tra trạng thái của máy 
SAFE_STATE CheckSafeState(void);


static void SystemControl(void);
//========================================================================================




// Hàm main 
//========================================================================================
//========================================================================================
void StandbyState(StateMachine *CurrentStateMachine)
{
   *CurrentStateMachine = STANDALONE_STATE;
    TimeoutSwapState = TIMEOUT_SWAP_STATE;  // Timeout 8s

    HAL_UART_Receive_IT(&huart2, &RxData, 1); // Ngắt nhận 1 byte data từ UART

  
    while (1)
    {
        // Đọc giá trị cảm biến 
        ReadSensor();
        
        SystemControl();
        
        // Giao tiếp LCD 
        LCDCommunication();

        // Giao tiếp UI
        UICommunication();

        // Reset flag 100ms
        if (Timer100msFlag == 1)
        {
            Timer100msFlag = 0;
        }


        // Kiểm tra điều kiện an toàn của hệ thống, nếu không an toàn thì không được phép chuyển trạng thái
//        if (CheckSafeState() == UN_SAFE)
//        {
//            TimeoutSwapState = TIMEOUT_SWAP_STATE;
//
//            HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
//            HAL_Delay(100);
//        }
        
        // Kiểm tra UI có kết nối hay không, để thay đổi trạng thái hệ thống 
        if (UIConnectionStatus == DISCONNECT)
        {
            *CurrentStateMachine = STANDALONE_STATE;
        }
        else
        {
            *CurrentStateMachine = UI_STATE;
        }


        CheckOverrall(); // Kiểm tra hệ thống theo chu kì, cập nhật biến "FaultMask"
        // Kiểm tra lỗi hệ thống, nếu có lỗi thì chuyển trạng thái 
        if (FaultMask != FAULT_NONE)
        {
          *CurrentStateMachine = FAULT_STATE;
          return;
        }

         // Timeout SwapState là thời gian chờ chuyển trạng thái
         if (!TimeoutSwapState)
         {
             return;
         }

    }
        
}
//========================================================================================
//========================================================================================









// BODY FUNCTION 
//========================================================================================
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
    
    case COMM_UI_CONNECT_ALIVE:
    Timeout5sUI = 50;
    break;

    case COMM_UI_DISCONNECT:
    UIConnectionStatus = DISCONNECT;
    // gửi lại disconnect
    CommUIDisconnect();
    break;
  }
}

// Kiểm tra trạng thái của máy 
SAFE_STATE CheckSafeState(void)
{
//     Chưa trả tay ga về vị trí ban đầu
//     if (Throttle_ADC >= 100)
//     {
//         return UN_SAFE;
//     }

    // Đang bật công tắt tải 
    if (Load_On_Off_Signal == SWITCH_ON)
    {
        
        return UN_SAFE;
    }

    // nếu công tắt mức tải khác mức 1 thì return UN_SAFE
    if (Load_Level_Signal != LOAD_1)
    {
        return UN_SAFE;
    }
    
    return SAFE;
}


static void LCDCommunication(void)
{
    if (Timer100msFlag)
    {
        CommLCDConnectAlive();
        CommLCDStateMachine(STANDBY_STATE);
        CommLCDLoad(Load_On_Off_Signal, Load_Level_Signal);  // Gửi giá trị on/off tải và mức tải 
        CommLCDThrottle(Throttle_ADC*100/4096);                       // Gửi giá trị tay ga 

    }

}

static void UICommunication(void)
{
    UISend();
    UIReceive();
}

static void UISend(void)
{
    if (UIConnectionStatus == CONNECTED)
    {
        if (Timer100msFlag)
        {
            CommUIConnectAlive();
        }
    }
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
//========================================================================================

