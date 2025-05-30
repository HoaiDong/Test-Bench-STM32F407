#include "UIState.h"
#include "Interrupt.h"
#include "Error.h"
#include "Commands.h"
#include "Sensor.h"
#include "DataType.h"
#include "Packet.h"
#include "Error.h"
#include "Packet.h"
#include "Commands.h"
#include "Interrupt.h"
#include "Sensor.h"
#include "Utility.h"

// Global variable
//========================================================================================
static uint16_t Throttle_ADC_UI;
static uint16_t Load_On_Off_Signal_UI;
static uint16_t Load_Level_Signal_UI;
//========================================================================================


// FUNCTION PROTOTYPE 
// CÁC FUNCTION NÀY LÀ FUNCTION CỤC BỘ CỦA RIÊNG STATE NÀY 
//========================================================================================
// Xử lý data nhận về từ UART
static void ProcessPacket(void);

// Giao tiếp với UI 
static void UICommunication(void);
static void UISend(void);
static void UIReceive(void);

// Giao tiếp với LCD
static void LCDCommunication(void);

// Điều khiển mô hình 
static void SystemControl(void);
//========================================================================================





// Hàm Main 
//========================================================================================
//========================================================================================
void UIState(StateMachine *CurrentStateMachine)
{
    Throttle_ADC_UI = 620;
    Load_On_Off_Signal_UI = 0;
    Load_Level_Signal_UI = 1;

    HAL_UART_Receive_IT(&huart2, &RxData, 1); // Ngắt nhận 1 byte data từ UART
//    Count = 0;
//    HeadBuffer = 0;  // Thể hiện vị trí tiếp theo trong buffer để nhận data
//    TailBuffer = 0;  // Thể hiện vị trí data cũ nhất trong buffer
//    Count = 0; // Số lượng byte có trong buffer
    while (1)
    {




        // Đọc cảm biến
        ReadSensor();

        // Giao tiếp với LCD 
        LCDCommunication();

        // Giao tiếp với UI 
        UICommunication();

        // Điều khiển mô hình 
        SystemControl();

        if (Timer100msFlag)
        {
            Timer100msFlag = 0;
        }


        // Nếu có ngắt kết nối UI, thì sẽ chuyển trạng thái về trạng thái chờ
        if (UIConnectionStatus == DISCONNECT)
        {
            *CurrentStateMachine = STANDBY_STATE;
            return;
        }
        

        CheckOverrall(); // Kiểm tra hệ thống theo chu kì 
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




// BODY FUNCTION
//========================================================================================
// Xử lý 1 data frame nhận về 
static void ProcessPacket(void) // StateMachine CurrentStateMachine
{
    CommUI id = (CommUI)DataFrame[2];  // Lấy giá trị ID Byte từ frame nhận được

    // Dựa vào ID để thực hiện tác vụ
    switch (id)
    {       

        // Kết nối UI 
        case COMM_UI_CONNECT:
        UIConnectionStatus = CONNECTED;  // Cập nhật trạng thái kết nối UI
        Timeout5sUI = TIMEOUT_UI;        // Cập nhật thời gian chờ
        CommUIConnect();                 // gửi lại connect
        break;

        // Tín hiệu alive duy trì kết nối 
        case COMM_UI_CONNECT_ALIVE:
        Timeout5sUI = TIMEOUT_UI;        // Cập nhật thời gian chờ
        break;

        // Ngắt kết nối 
        case COMM_UI_DISCONNECT:
        UIConnectionStatus = DISCONNECT; // Cập nhật trạng thái kết nối UI
        CommUIDisconnect();              // gửi lại disconnect
        break;

        // Giá trị tay ga 
        case COMM_UI_SET_THROTTLE:
        Throttle_ADC_UI = DataFrame[3]<<24 | DataFrame[4]<<16 | DataFrame[5]<<8 | DataFrame[6]<<0;
        break;
    
        // Giá trị bật/tắt tải và thay đổi mức tải 
        case COMM_UI_SET_LOAD:
//        Load_On_Off_Signal_UI = DataFrame[3];
//        Load_Level_Signal_UI = DataFrame[4];
		Load_On_Off_Signal_UI = DataFrame[3];

        break;
    
        // Request realtime data 
        case COMM_UI_REALTIME_DATA:
        // gửi lại dữ liệu thực tế
        CommUIRealtimeData();
        break;

        case COMM_UI_WRITE_SETUP:
		//Cập nhật số răng đĩa cảm biến tốc độ
		Speed_Disc_Tooth = (DataFrame[3]<<8 | DataFrame[4]<<0);

		//Cập nhật độ nhạy cảm biến dòng
		Cur_Sensitive = (float)(DataFrame[5]<<24 | DataFrame[6]<<16 | DataFrame[7]<<8 | DataFrame[8]<<0) / 1000;

		//Cập nhật độ nhạy cảm biến momen
		Torque_Sensitive = (float)(DataFrame[9]<<24 | DataFrame[10]<<16 | DataFrame[11]<<8 | DataFrame[12]<<0) / 1000;

		FLASH_Write(FLASH_SECTOR11_ADDR, Speed_Disc_Tooth, (uint32_t)(Cur_Sensitive*1000), (uint32_t)(Torque_Sensitive*1000));

		//Gửi xác nhận đã ghi thành công các dữ liệu setup xuống STM32
		CommUIConfirmWriteSetupData();
		break;

        case COMM_UI_READ_SETUP:
		//Gửi lại dữ liệu setup hiện tại của các cảm biến lên UI
		CommUIReadSetupData();
		break;

    }
}


// Giao tiếp với UI 
static void UICommunication(void)
{
    UISend();
    UIReceive();
}

static void UISend(void)
{
    if (Timer100msFlag)
    {
        CommUIConnectAlive();
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

static void LCDCommunication(void)
{
    if (Timer100msFlag)
    {

		CommLCDConnectAlive();                              // Gửi tín hiệu alive to LCD để xác nhận kết nối
		CommLCDStateMachine(UI_STATE);              // Gửi state machine


        CommLCDLoad(Load_On_Off_Signal_UI, Load_Level_Signal_UI); // Gửi giá trị on/off tải và mức tải
        CommLCDRPMAndTorque(Speed_RPM, Torque_Value);         // Gửi giá trị tốc độ bánh xe và mômen
        CommLCDCurrent(Votol_Current*100, Generator_Current*100);   // Gửi giá trị dòng điện votol và dòng điện máy phát
        CommLCDVoltage(Votol_Voltage*100, Generator_Voltage*100);   // Gửi giá trị điện áp votol và điện áp máy phát


        CommLCDThrottle(Throttle_ADC_UI * 100 / 4096);                      // Gửi giá trị tay ga

    }
}

// Điều khiển mô hình 
static void SystemControl(void)
{
    // Điều khiển tay ga 
    SetMotor(Throttle_ADC_UI);
    SetLoad(Load_On_Off_Signal_UI);
}
//========================================================================================
