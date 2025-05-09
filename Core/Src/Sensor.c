#include "Sensor.h"
#include "DataType.h"
#include "Flash.h"

// Buffer adc lưu trữ giá trị đọc từ cảm biến
uint32_t adc1_buffer[3] = {0, 0, 0};
uint32_t adc2_buffer[3] = {0, 0, 0};


static uint16_t TorqueOffset = 0;
static uint16_t CurrentVoltolOffset = 0;
static uint16_t CurrentGeneratorOffset = 0;

// Giá trị cảm biến thô
static uint16_t Throttle_Raw_ADC = 0;       // ADC giá trị đọc từ cảm biến tay gaga
static uint16_t Torque_Raw_ADC = 0;         // ADC giá trị đọc từ cảm biến mô men xoắn
static uint16_t Cur_Votol_Raw_ADC = 0;      // ADC giá trị đọc từ cảm biến dòng điện Votol
static uint16_t Cur_Generator_Raw_ADC  = 0; // ADC giá trị đọc từ cảm biến dòng điện Generator
static uint16_t Vol_Votol_Raw_ADC = 0;      // ADC giá trị đọc từ cảm biến điện áp Votol
static uint16_t Vol_Generator_Raw_ADC = 0;  // ADC giá trị đọc từ cảm biến điện áp Generator

// Tín hiệu digital 
uint8_t Load_On_Off_Signal = 0;      // Digital giá trị đọc từ công tắt tải tín hiệu ON/OFF
uint8_t Load_Level_Signal = 0;       // Digital giá trị mức tải


uint16_t Speed_Disc_Tooth = 45 ;    // Số răng đĩa cảm biến tốc độ
float Cur_Sensitive = 0;  		//Độ nhạy cảm biến dòng
float Torque_Sensitive = 0;  		//Độ nhạy cảm biến momen


float Speed_RPM = 0;  // Giá trị đọc từ cảm biến tốc độ vòng/phút
uint32_t last_capture_tick = 0; // Thoi gian timeout de check toc do bang 0



// Giá trị cảm biến tinh 
extern uint16_t Throttle_ADC = 0;       // ADC giá trị đọc từ cảm biến tay gaga
extern uint16_t Torque_ADC = 0;         // ADC giá trị đọc từ cảm biến mô men xoắn
extern uint16_t Cur_Votol_ADC = 0;      // ADC giá trị đọc từ cảm biến dòng điện Votol
extern uint16_t Cur_Generator_ADC  = 0; // ADC giá trị đọc từ cảm biến dòng điện Generator
extern uint16_t Vol_Votol_ADC = 0;      // ADC giá trị đọc từ cảm biến điện áp Votol
extern uint16_t Vol_Generator_ADC = 0;  // ADC giá trị đọc từ cảm biến điện áp Generator


// Kalman Filter
//============================================================================================
typedef struct 
{
    float x_est;    // Giá trị trạng thái ước lượng
    float P;        // Phương sai ước lượng
    float Q;        // Phương sai quá trình
    float R;        // Phương sai đo đạc
    float K;        // Hệ số Kalman
} KalmanFilter; 


void Kalman_Init(KalmanFilter *kf,
                    float init_x,
                    float init_P,
                    float Q_val,
                    float R_val)
{
    kf->x_est = init_x;
    kf->P     = init_P;
    kf->Q     = Q_val;
    kf->R     = R_val;
    kf->K     = 0.0f;
}

float Kalman_Update(KalmanFilter *kf, float z)
{
    // 1. Predict
    kf->P += kf->Q;

    // 2. Compute Kalman Gain
    kf->K = kf->P / (kf->P + kf->R);

    // 3. Update estimate with measurement z
    kf->x_est += kf->K * (z - kf->x_est);

    // 4. Update error covariance
    kf->P *= (1.0f - kf->K);

    return kf->x_est;
}
//============================================================================================

// Median Filter
//============================================================================================
//static const uint8_t MEDIAN_FILTER_SIZE = 5;
//uint16_t median_buffer[MEDIAN_SIZE] = {0};
//uint8_t median_index = 0;
//
//uint16_t Median_Filter(uint16_t new_sample)
//{
//   median_buffer[median_index++] = new_sample;
//   if (median_index >= MEDIAN_SIZE) median_index = 0;
//
//   // Tạo bản sao để sắp xếp
//   uint16_t sorted[MEDIAN_SIZE];
//   memcpy(sorted, median_buffer, sizeof(sorted));
//
//   // Sắp xếp nổi bọt đơn giản
//   for (int i = 0; i < MEDIAN_SIZE - 1; i++)
//   {
//       for (int j = i + 1; j < MEDIAN_SIZE; j++)
//       {
//           if (sorted[i] > sorted[j])
//           {
//               uint16_t temp = sorted[i];
//               sorted[i] = sorted[j];
//               sorted[j] = temp;
//           }
//       }
//   }
//
//   return sorted[MEDIAN_SIZE / 2]; // Trả về giá trị ở giữa
//}

//============================================================================================


// Lấy giá trị offset của các cảm biến 
void GetOffsetSensor(void)
{
    TorqueOffset = adc1_buffer[1];              // Lấy giá trị offset từ cảm biến Torque
    CurrentVoltolOffset = adc1_buffer[2];       // Lấy giá trị offset từ cảm biến Current Voltol
    CurrentGeneratorOffset = adc2_buffer[0];    // Lấy giá trị offset từ cảm biến Current Generator

    //Cập nhật các giá trị Setup
	if(FLASH_Read(FLASH_SECTOR11_ADDR) != 0xFFFFFFFF && FLASH_Read(FLASH_SECTOR11_ADDR + 4) != 0xFFFFFFFF && FLASH_Read(FLASH_SECTOR11_ADDR + 8) != 0xFFFFFFFF)
	{
		Speed_Disc_Tooth = FLASH_Read(FLASH_SECTOR11_ADDR);
		Cur_Sensitive = (float)FLASH_Read(FLASH_SECTOR11_ADDR + 4) / 1000;
		Torque_Sensitive = (float)FLASH_Read(FLASH_SECTOR11_ADDR + 8) / 1000;
	}
	else
	{
		Speed_Disc_Tooth = 45;
		Cur_Sensitive = 0.60;
		Torque_Sensitive = 2.43;

		FLASH_Write(FLASH_SECTOR11_ADDR, Speed_Disc_Tooth, (uint32_t)(Cur_Sensitive*1000), (uint32_t)(Torque_Sensitive*1000));
	}
}



// Hàm đọc giá trị cảm biến
void ReadSensor(void)
{
    Throttle_Raw_ADC = (uint16_t)adc1_buffer[0];
    Torque_Raw_ADC = (uint16_t)adc1_buffer[1];
    Cur_Votol_Raw_ADC = (uint16_t)adc1_buffer[2];
    Cur_Generator_Raw_ADC  = (uint16_t)adc2_buffer[0];
    Vol_Votol_Raw_ADC = (uint16_t)adc2_buffer[1];
    Vol_Generator_Raw_ADC = (uint16_t)adc2_buffer[2];
    check_motor_timeout();

    Load_On_Off_Signal = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13);      // Digital giá trị đọc từ công tắt tải tín hiệu ON/OFF

    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_SET)
    {
        Load_Level_Signal = 1;  // Tải mức 1 
    }
    else if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9) == GPIO_PIN_SET)
    {
        Load_Level_Signal = 2;  // Tải mức 2 
    }
    else if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10) == GPIO_PIN_SET)
    {
        Load_Level_Signal = 3;  // Tải mức 3 
    }
    else if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_11) == GPIO_PIN_SET)
    {
        Load_Level_Signal = 4;  // Tải mức 4 
    }

    Throttle_ADC = Throttle_Raw_ADC;                         // ADC giá trị đọc từ cảm biến tay gaga
    Torque_ADC = Torque_Raw_ADC;                             // ADC giá trị đọc từ cảm biến mô men xoắn
    Cur_Votol_ADC = Cur_Votol_Raw_ADC;                       // ADC giá trị đọc từ cảm biến dòng điện Votol
    Cur_Generator_ADC  = Cur_Generator_Raw_ADC;              // ADC giá trị đọc từ cảm biến dòng điện Generator
    Vol_Votol_ADC = Vol_Votol_Raw_ADC;                       // ADC giá trị đọc từ cảm biến điện áp Votol
    Vol_Generator_ADC = Vol_Generator_Raw_ADC;               // ADC giá trị đọc từ cảm biến điện áp Generator

}  

//Ham check toc do bang 0
void check_motor_timeout()
{
    // Kiểm tra nếu quá 50ms không có xung mới
    if (HAL_GetTick() - last_capture_tick > 250) // 500ms timeout
    {
    	Speed_RPM = 0;
    }
}



