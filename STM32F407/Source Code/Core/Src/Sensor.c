#include "Sensor.h"
#include "DataType.h"
#include "Flash.h"
#include "Filter.h"

#define V_REF_ADC 3

// Buffer adc lưu trữ giá trị đọc từ cảm biến
uint32_t adc1_buffer[3] = {0, 0, 0};
uint32_t adc2_buffer[3] = {0, 0, 0};


static uint16_t TorqueOffset = 0;
static uint16_t	TorqueOffsetForLoad = 0;
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
uint16_t Throttle_ADC = 0;       // ADC giá trị đọc từ cảm biến tay gaga
uint16_t Torque_ADC = 0;         // ADC giá trị đọc từ cảm biến mô men xoắn
uint16_t Cur_Votol_ADC = 0;      // ADC giá trị đọc từ cảm biến dòng điện Votol
uint16_t Cur_Generator_ADC  = 0; // ADC giá trị đọc từ cảm biến dòng điện Generator
uint16_t Vol_Votol_ADC = 0;      // ADC giá trị đọc từ cảm biến điện áp Votol
uint16_t Vol_Generator_ADC = 0;  // ADC giá trị đọc từ cảm biến điện áp Generator


uint16_t Torque_Value = 0;
float	Votol_Voltage = 0;
float	Generator_Voltage = 0;
float 	Votol_Current = 0;
float	Generator_Current = 0;


uint16_t DAC_Motor_Value = 620; 		// Giá trị DAC để điều khiển động cơ

static KalmanFilter Torque_kf, Current_Votol_kf, Current_Generator_kf, Voltage_Votol_kf, Voltage_Generator_kf, Speed_kf;


// Lọc Median 
static MedianFilter_t Torque_mf;
static MedianFilter_t Current_Votol_mf;
static MedianFilter_t Current_Generator_mf;
static MedianFilter_t Voltage_Votol_mf;
static MedianFilter_t Voltage_Generator_mf;

// Bộ lọc EMA cho tốc độ
static EMAFilter_t Speed_ema_f;

// Bộ lọc EMA cho mỗi sensor:
static EMAFilter_t Torque_ema_f;
static EMAFilter_t Current_Votol_ema_f;
static EMAFilter_t Current_Generator_ema_f;
static EMAFilter_t Voltage_Votol_ema_f;
static EMAFilter_t Voltage_Generator_ema_f;   
//============================================================================================

// Lọc tín hiệu 
static void UpdateFilter(void);

static void UpdateTorqueFilter(void);




// ===================== Lọc chống spike riêng cho Torque Sensor =====================
#define TORQUE_SPIKE_THRESHOLD 20        // Ngưỡng phát hiện spike (đơn vị ADC, chỉnh tuỳ thực tế)
static uint16_t last_good_torque = 0;    // Lưu giá trị Torque hợp lệ gần nhất
static uint8_t torque_first_update = 0;

// ===================================================================



static void UpdateTorqueFilter(void)
{
	uint16_t raw = Torque_Raw_ADC;

	// ===================== Xử lý lần đầu tiên =====================
	// Nếu lần đầu chạy, gán luôn giá trị sensor hiện tại vào biến kiểm tra spike,
	// tránh việc so sánh với 0 dẫn tới bị giữ mãi giá trị 0.
	static uint8_t torque_first_update = 0;
	if (!torque_first_update) {
		last_good_torque = raw;         // Gán giá trị thực tế đầu tiên
		torque_first_update = 1;
	}

	// ===================== Lọc chống spike =====================
	// Nếu giá trị mới lệch quá ngưỡng so với giá trị hợp lệ gần nhất => coi là spike, bỏ qua
	if (abs((int32_t)raw - (int32_t)last_good_torque) > TORQUE_SPIKE_THRESHOLD) {
		raw = last_good_torque;         // Không update, giữ lại giá trị hợp lệ trước đó
	} else {
		last_good_torque = raw;         // Nếu giá trị hợp lý, cập nhật lại biến kiểm tra
	}

	// ===================== Lọc median + EMA =====================
	// Tiếp tục đưa giá trị qua median filter rồi đến EMA để loại nhiễu ngắn và làm mượt tín hiệu
	MedianFilter_Put(&Torque_mf, raw);
	Torque_ADC = (uint16_t)EMAFilter_Update(&Torque_ema_f, (float)MedianFilter_Get(&Torque_mf));
}


//============================================================================================
void InitFilter(void)
{
    MedianFilter_Init(&Torque_mf, 15);
    EMAFilter_Init(&Torque_ema_f, 0.2f);

    MedianFilter_Init(&Current_Votol_mf, 7);
    EMAFilter_Init(&Current_Votol_ema_f, 0.2f);

    MedianFilter_Init(&Current_Generator_mf, 7);
    EMAFilter_Init(&Current_Generator_ema_f, 0.2f);

    MedianFilter_Init(&Voltage_Votol_mf, 7);
    EMAFilter_Init(&Voltage_Votol_ema_f, 0.2f);

    MedianFilter_Init(&Voltage_Generator_mf, 7);
    EMAFilter_Init(&Voltage_Generator_ema_f, 0.2f);
    
    
    // Bộ lọc EMA cho Speed/RPM. Alpha 0.2 là phù hợp (có thể thử 0.15–0.3)
    EMAFilter_Init(&Speed_ema_f, 0.2f);
}

static void UpdateFilter(void)
{
    // // Đưa giá trị mới vào filter và lấy giá trị trung vị đã lọc
    // Median + EMA cho Torque
    // MedianFilter_Put(&Torque_mf, Torque_Raw_ADC);
    // Torque_ADC = (uint16_t)EMAFilter_Update(&Torque_ema_f, (float)MedianFilter_Get(&Torque_mf));

    // --- Lọc riêng cho Torque ---
    UpdateTorqueFilter();


    // Median + EMA cho Current Votol
    MedianFilter_Put(&Current_Votol_mf, Cur_Votol_Raw_ADC);
    Cur_Votol_ADC = (uint16_t)EMAFilter_Update(&Current_Votol_ema_f, (float)MedianFilter_Get(&Current_Votol_mf));

    // Median + EMA cho Current Generator
    MedianFilter_Put(&Current_Generator_mf, Cur_Generator_Raw_ADC);
    Cur_Generator_ADC = (uint16_t)EMAFilter_Update(&Current_Generator_ema_f, (float)MedianFilter_Get(&Current_Generator_mf));

    // Median + EMA cho Voltage Votol
    MedianFilter_Put(&Voltage_Votol_mf, Vol_Votol_Raw_ADC);
    Vol_Votol_ADC = (uint16_t)EMAFilter_Update(&Voltage_Votol_ema_f, (float)MedianFilter_Get(&Voltage_Votol_mf));

    // Median + EMA cho Voltage Generator
    MedianFilter_Put(&Voltage_Generator_mf, Vol_Generator_Raw_ADC);
    Vol_Generator_ADC = (uint16_t)EMAFilter_Update(&Voltage_Generator_ema_f, (float)MedianFilter_Get(&Voltage_Generator_mf));


    // Lọc Speed_RPM bằng EMA filter
    Speed_RPM = EMAFilter_Update(&Speed_ema_f, Speed_RPM); // Giá trị Speed_RPM gốc là raw
}


// Lấy giá trị offset của các cảm biến 
void GetOffsetSensor(void)
{


    // Công tắt bật tải
    if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0) == GPIO_PIN_SET)
    {
    	TorqueOffsetForLoad = Torque_ADC;
    	
    }
    else
	{
    	TorqueOffset = Torque_ADC;
	}


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
		Torque_Sensitive = 2.34;

		FLASH_Write(FLASH_SECTOR11_ADDR, Speed_Disc_Tooth, (uint32_t)(Cur_Sensitive*1000), (uint32_t)(Torque_Sensitive*1000));
	}


}



// Hàm đọc giá trị cảm biến
void ReadSensor(void)
{
 
    // Throttle_Raw_ADC = (uint16_t)adc1_buffer[0];
    Torque_Raw_ADC = (uint16_t)adc1_buffer[1];
    Cur_Votol_Raw_ADC = (uint16_t)adc1_buffer[2];
    Cur_Generator_Raw_ADC  = (uint16_t)adc2_buffer[0];
    Vol_Votol_Raw_ADC = (uint16_t)adc2_buffer[1];
    Vol_Generator_Raw_ADC = (uint16_t)adc2_buffer[2];

    check_motor_timeout();  // Kiểm tra thời gian hoạt động của motor để câpj nhật speed 


    extern StateMachine CurrentStateMachine;
    if (CurrentStateMachine == STANDALONE_STATE)
    {
        if (adc1_buffer[0] >= MIN_DAC_MOTOR)
        {
            DAC_Motor_Value = adc1_buffer[0];
        }
    }
    

    Load_On_Off_Signal = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13);      // Digital giá trị đọc từ công tắt tải tín hiệu ON/OFF



    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_SET)
    {
        Load_Level_Signal = LOAD_1;  // Tải mức 1 
    }
    else if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9) == GPIO_PIN_SET)
    {
        Load_Level_Signal = LOAD_2;  // Tải mức 2 
    }
    else if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10) == GPIO_PIN_SET)
    {
        Load_Level_Signal = LOAD_3;  // Tải mức 3 
    }
    else if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_11) == GPIO_PIN_SET)
    {
        Load_Level_Signal = LOAD_4;  // Tải mức 4 
    }

    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13) == GPIO_PIN_RESET)
    {
        Load_Level_Signal = LOAD_0;  // Tải mức 0 
    }



    Throttle_ADC = Throttle_Raw_ADC;                         // ADC giá trị đọc từ cảm biến tay gaga

    // Xóa cập nhật Torque vì đã có update cùng bộ lọc riêng rồi  
    // Torque_ADC = Torque_Raw_ADC;                             // ADC giá trị đọc từ cảm biến mô men xoắn
    Cur_Votol_ADC = Cur_Votol_Raw_ADC;                       // ADC giá trị đọc từ cảm biến dòng điện Votol
    Cur_Generator_ADC  = Cur_Generator_Raw_ADC;              // ADC giá trị đọc từ cảm biến dòng điện Generator
    Vol_Votol_ADC = Vol_Votol_Raw_ADC;                       // ADC giá trị đọc từ cảm biến điện áp Votol
    Vol_Generator_ADC = Vol_Generator_Raw_ADC;               // ADC giá trị đọc từ cảm biến điện áp Generator

    UpdateFilter(); // Lọc tín hiệu ADC

    // Công tắt bật tải
    if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0) == GPIO_PIN_SET)
    {
    	Torque_Value = (((float)(TorqueOffsetForLoad -Torque_ADC) / 4095) * V_REF_ADC ) / 0.00234;
    }
    else
	{
    	Torque_Value = (((float)(TorqueOffset - Torque_ADC) / 4095) * V_REF_ADC ) / 0.00234;
	}

    Votol_Current = (((float)(Cur_Votol_ADC - CurrentVoltolOffset) / 4095) * V_REF_ADC) / 0.060;
	Generator_Current = (((float)(Cur_Generator_ADC - CurrentGeneratorOffset) / 4095) * V_REF_ADC) / 0.060;
	Votol_Voltage = ((float)Vol_Votol_ADC / 4095) * V_REF_ADC * 21;
	Generator_Voltage = ((float)Vol_Generator_ADC / 4095) * V_REF_ADC * 21;

}  


//Ham check toc do bang 0
void check_motor_timeout()
{

    // Kiểm tra nếu quá 50ms không có xung mới
    if (HAL_GetTick() - last_capture_tick > 125) // 500ms timeout
    {
    	Speed_RPM = 0;
    }
}



