#include "StandaloneState.h"
#include "Interrupt.h"
#include "Commands.h"
#include "Error.h"
#include "Sensor.h"
#include "Packet.h"
#include "Utility.h"




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

        // Bật đèn báo màu xanh lá 
        AlarmLed(LED_GREEN);

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
        CommLCDRPMAndTorque(Speed_RPM, Torque_Value);         // Gửi giá trị tốc độ bánh xe và mômen
        CommLCDCurrent(Votol_Current*100, Generator_Current*100);   // Gửi giá trị dòng điện votol và dòng điện máy phát
        CommLCDVoltage(Votol_Voltage*100, Generator_Voltage*100);   // Gửi giá trị điện áp votol và điện áp máy phát
    }

}

static void UICommunication(void)
{
    UIReceive();
}


static void UIReceive(void)
{
    if (TryDecodePacket() == 1)
    {
        ProcessPacket();
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
    SetMotor(DAC_Motor_Value);  // Điều khiển motor 
    SetLoad(Load_Level_Signal); // Điều khiển tải 
}
//========================================================================================
