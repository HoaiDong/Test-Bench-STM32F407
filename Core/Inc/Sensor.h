#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

// Định nghĩa trạng thái công tắc
typedef enum  _SWITCH_STATUS
{
    SWITCH_OFF = 0, 
    SWITCH_ON
}SWITCH_STATUS;

// Định nghĩa mức tải 
typedef enum _LOAD_LEVEL 
{
    LOAD_1 = 1,
    LOAD_2,
    LOAD_3,
    LOAD_4
}LOAD_LEVEL;

// Buffer adc lưu trữ giá trị đọc từ cảm biến
extern uint32_t adc1_buffer[3];
extern uint32_t adc2_buffer[3];

// Giá trị cảm biến tinh 
extern uint16_t Throttle_ADC;       // ADC giá trị đọc từ cảm biến tay gaga
extern uint16_t Torque_ADC;         // ADC giá trị đọc từ cảm biến mô men xoắn
extern uint16_t Cur_Votol_ADC;      // ADC giá trị đọc từ cảm biến dòng điện Votol
extern uint16_t Cur_Generator_ADC; // ADC giá trị đọc từ cảm biến dòng điện Generator
extern uint16_t Vol_Votol_ADC;      // ADC giá trị đọc từ cảm biến điện áp Votol
extern uint16_t Vol_Generator_ADC;  // ADC giá trị đọc từ cảm biến điện áp Generator

extern uint8_t Load_On_Off_Signal;      // Digital giá trị đọc từ công tắt tải tín hiệu ON/OFF
extern uint8_t Load_Level_Signal;     // Digital giá trị mức tải

extern uint16_t Speed_Disc_Tooth;    // Số răng đĩa cảm biến tốc độ
extern float Cur_Sensitive;  		//Độ nhạy cảm biến dòng
extern float Torque_Sensitive;  		//Độ nhạy cảm biến momen

extern float Speed_RPM;  // Giá trị đọc từ cảm biến tốc độ vòng/phút
extern uint32_t last_capture_tick;  // Thoi gian timeout de check toc do bang 0








// Lấy giá trị offset của các cảm biến 
void GetOffsetSensor(void);

// Hàm đọc giá trị cảm biến
void ReadSensor(void);  

//Ham check toc do bang 0
void check_motor_timeout();






#endif
//EOF
