#ifndef DATATYPE_H
#define DATATYPE_H

// Tạm thờithời
//=============================================================
// #include <stm32f407xx.h>
//=============================================================




#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_can.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_hal_dac.h"
#include "stm32f4xx_hal_dma.h"

#define FLASH_SECTOR11_ADDR		0X080E0000

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;
extern CAN_HandleTypeDef hcan1;
extern DAC_HandleTypeDef hdac;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2;

// Định nghĩa kiểu enum để mô tả các trạng thái của state machine (máy trạng thái)
typedef enum _StateMachine
{
    INITIALIZATION_STATE = 0,  // Trạng thái khởi tạo: Hệ thống đang thực hiện quá trình khởi động, thiết lập ban đầu.
    STANDBY_STATE,             // Trạng thái chờ (standby): Hệ thống đã khởi tạo xong và đang ở trạng thái sẵn sàng, chờ nhận lệnh.
    STANDALONE_STATE,          // Trạng thái hoạt động độc lập: Hệ thống chạy riêng rẽ, không phụ thuộc vào các module khác.
    UI_STATE,                  // Trạng thái giao diện người dùng: Hệ thống đang hiển thị giao diện và tương tác với người dùng.
    FAULT_STATE                // Trạng thái lỗi: Hệ thống gặp sự cố hoặc lỗi và cần được xử lý.

} StateMachine;


// Định nghĩa kiểu enum FaultCode để biểu diễn các lỗi phát hiện dưới dạng bit lỗi (mask) 
typedef enum _FaultCode
{
    FAULT_NONE                = 0,         // Không có lỗi
    FAULT_LCD_MISSING         = 1 << 0,    // Bit 0: Thiếu LCD
    FAULT_TORQUE_SENSOR_MISSING     = 1 << 1,    // Bit 1: Thiếu torque sensor 
    FAULT_THROTTLE_MISSING     = 1 << 2,    // Bit 2: Thiếu throttle sensor
    FAULT_SPEED_SENSOR_MISSING     = 1 << 3,    // Bit 3: Thiếu speed sensor
    FAULT_CURRENT_VOTOL_SENSOR_MISSING     = 1 << 4,    // Bit 4: Thiếu current votol sensor
    FAULT_CURRENT_GENERATOR_SENSOR_MISSING     = 1 << 5,    // Bit 5: Thiếu current generator sensor
    FAULT_VOLTAGE_VOTOL_SENSOR_MISSING     = 1 << 6,    // Bit 6: Thiếu voltage votol sensor
    FAULT_VOLTAGE_GENERATOR_SENSOR_MISSING     = 1 << 7,    // Bit 7: Thiếu voltage generator sensor
    FAULT_LOAD_WIRE1_BROKEN   = 1 << 8,    // Bit 8: Đứt dây GPIO số 1
    FAULT_LOAD_WIRE2_BROKEN   = 1 << 9,    // Bit 9: Đứt dây GPIO số 2
    FAULT_LOAD_WIRE3_BROKEN   = 1 << 10,    // Bit 10: Đứt dây GPIO số 3
    FAULT_LOAD_WIRE4_BROKEN   = 1 << 11,   // Bit 11: Đứt dây GPIO số 4
    FAULT_OPEN_CIRCUIT        = 1 << 12,    // Bit 12: Hở mạch (Open circuit)
    FAULT_UI_MISSING          = 1 << 13,    // Bit 13: Thiếu giao diện người dùng
    FAULT_LOAD_LEVEL_SWITCH_MISSING = 1 << 14    // Bit 14: Thiếu công tắc mức tải

} FaultCode;

// Định nghĩa kiểu enum CommLCD để biểu diễn các giá trị ID của data frame CAN 2.0A mà STM32 giao tiếp với LCD
typedef enum _CommLCD
{
    COMM_LCD_STATE_MACHINE = 0,  // ID CAN cho trạng thái máy (state machine) của hệ thống
    COMM_LCD_CONNECT,            // ID CAN dùng khi yêu cầu kết nối hoặc xác nhận kết nối giữa STM32 và LCD
    COMM_LCD_CONNECT_ALIVE,      // ID CAN để báo hiệu rằng kết nối đang hoạt động (giữ kết nối sống)
    COMM_LCD_RPM_TORQUE,         // ID CAN truyền thông tin về tốc độ vòng quay (RPM) và mô-men xoắn (Torque) từ động cơ
    COMM_LCD_VOLTAGE,            // ID CAN truyền thông tin về điện áp
    COMM_LCD_CURRENT,            // ID CAN truyền thông tin về dòng điện
    COMM_LCD_LOAD,               // ID CAN truyền thông tin về tải của hệ thống
    COMM_LCD_DETECT_SENSOR,      // ID CAN truyền tín hiệu khi phát hiện cảm biến hoặc nhận dữ liệu từ cảm biến
    COMM_LCD_FAULT_CODE,         // ID CAN truyền mã lỗi nếu có sự cố xảy ra
    COMM_LCD_THROTTLE            // ID CAN truyền thông tin về giá trị tay ga 

}CommLCD;

// Định nghĩa kiểu enum CommUI để biểu diễn các giá trị ID của data frame mà STM32 giao tiếp với giao diện người dùng (UI) bằng UART 
typedef enum _CommUI
{
    COMM_UI_CONNECT = 1,       // ID UART để yêu cầu kết nối từ UI đến STM32
    COMM_UI_CONNECT_ALIVE,     // ID UART báo hiệu kết nối vẫn còn hoạt động (giữ kết nối sống)
    COMM_UI_DISCONNECT,        // ID UART để yêu cầu ngắt kết nối giữa UI và STM32
    COMM_UI_SET_THROTTLE,      // ID UART để thiết lập giá trị ga (throttle) từ UI
    COMM_UI_SET_LOAD,          // ID UART để thiết lập giá trị tải từ UI
    COMM_UI_REALTIME_DATA,     // ID UART để gửi dữ liệu thời gian thực từ STM32 đến UI
	COMM_UI_WRITE_SETUP,
	COMM_UI_READ_SETUP

}CommUI;

// Định nghĩa kiểu enum LCD Status để biểu thị tình trạng kết nối với LCD
typedef enum _LCDStatus
{
    LCD_STATUS_ALIVE = 0,     // Vẫn đang kết nối với LCD
    LCD_STATUS_DEAD,          // Mất kết nối với LCD
    
}LCDStatus;


// Định nghĩa kiểu enum UI Status để biểu thị tình trạng kết nối với UI
typedef enum _UIStatus
{
    UI_STATUS_ALIVE = 0,     // Vẫn đang kết nối với giao diện người
    UI_STATUS_DEAD,          // Mất kết nối với giao diện người dùng
}UIStatus;

#endif  //DATATYPE_H
