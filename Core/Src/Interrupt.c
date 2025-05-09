#include "Interrupt.h"
#include "DataType.h"
#include "Error.h"
#include "Packet.h"
#include "stm32f4xx_hal_uart.h"
#include "Sensor.h"



uint8_t RxData = 0; // Biến để chứa 1 data đọc về từ UART

uint8_t Timer10MsFlag = 0;
uint8_t Timer50MsFlag = 0;
uint8_t Timer100msFlag = 0;
uint8_t Timer500MsFlag = 0;

uint8_t TimerCheckErrorFlag = 0; // Timer kiểm tra lỗi toàn hệ thống theo chu kỳ

uint16_t Timeout5s = 50;
uint16_t Timeout5sUI = 50; // chờ kiểm tra tín hiệu UI
uint16_t TimeoutSwapState = 80; // chờ 5s để Swap State

uint16_t Timeout5sLCD = 50; // Timeout kiểm tra tín hiệu LCD 

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);	// Input capture 

// Hàm ngắt GPIO 
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) 
{


}


// Ngắt nhận data từ UARTUART
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
    {
        // Xử lý dữ liệu từ RxBuffer
        EnqueueBuffer(RxData);

        // Khởi động lại chế độ nhận để nhận byte tiếp theo
        HAL_UART_Receive_IT(huart, &RxData, 1);
    }
}

// Ngắt nhận data từ CAN
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t RxData[8];

	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);

	if (RxHeader.StdId == COMM_LCD_CONNECT_ALIVE)
	{
		// LCDConnectAlive = LCD_STATUS_ALIVE;
		// // OverallCount = 50;

		LCDConnectionStatus = CONNECTED;
		Timeout5sLCD = 50;
	}





	// if (RxHeader.StdId == COMM_LCD_CONNECT)
	// {
    //     LCDConnectAlive = 50;
	// }

}

// Timer 100ms
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) 
{
    if (htim->Instance == TIM6) 
    {


		Timer100msFlag = 1;

		static uint8_t TimerCount = 0;
		TimerCount++;
		if (TimerCount % 30 == 0)
		{
			TimerCheckErrorFlag = 1; // Timer kiểm tra lỗi toàn hệ thống theo chu kỳ
			TimerCount = 0;
		}

		// static uint8_t TimerCount = 0;
		// TimerCount++;

		// 100ms
		// if (TimerCount % 1 == 0)
		// {
		// 	// Timer10MsFlag = 1;
		// 	Timer100msFlag = 1;
		// 	TimerCount = 0;
		// }

		// // 50ms
		// if (TimerCount % 5 == 0)
		// {
		// 	Timer50MsFlag = 1;
		// }

		// // 100ms
		// if (TimerCount % 10 == 0)
		// {
		// 	Timer100msFlag = 1;
		// }

		// // 500ms
		// if (TimerCount % 50 == 0)
		// {
		// 	Timer500MsFlag = 1;
		// 	TimerCount = 0;
		// }

		
		// Timeout 5s cho LCD
		if (Timeout5sLCD)
		{
			Timeout5sLCD--;
		}

		// Timeout 5s cho UI
		if(Timeout5sUI)
		{
			Timeout5sUI--;
		}

		// Timeout 5s cho swap state
		if (TimeoutSwapState)
		{
			TimeoutSwapState--;
		}
    }





	// Timer 200ms
	// Xử lý debounce ở nút bật tắt tải (bật tắt máy phát)
    if (htim->Instance == TIM7) 
	{
    	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);

		extern StateMachine CurrentStateMachine;

		switch (CurrentStateMachine)
		    {
		      case INITIALIZATION_STATE:
			  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		      break;

		      case STANDBY_STATE:
		      HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
			  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		      break;

		      case STANDALONE_STATE:
		      HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
			  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		      break;

		      case UI_STATE:
		    	  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		      break;

		      case FAULT_STATE:
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);		    	  
		      break;

			  }
		
	}
}




void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		static uint32_t IC_Val1 = 0;
		static uint32_t IC_Val2 = 0;
		static uint32_t Difference = 0;
		static uint8_t Is_First_Captured = 0;
		if (Is_First_Captured==0) // if the first rising edge is not captured
		{
			
			IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
			Is_First_Captured = 1;  // set the first captured as true
			
		}
		else   // If the first rising edge is captured, now we will capture the second edge
		{
			IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value

			if (IC_Val2 > IC_Val1)
			{
				Difference = IC_Val2-IC_Val1;
			}
			else if (IC_Val1 > IC_Val2)
			{
				Difference = (0xffffffff - IC_Val1) + IC_Val2;
			}

			uint32_t frequency = 1000000/Difference;
		
			Speed_RPM = ((float)frequency/Speed_Disc_Tooth) * 60;

			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
			Is_First_Captured = 0; // set it back to false

			//Thoi gian timeout de check toc do bang 0
			last_capture_tick = HAL_GetTick();
		}
	}
}
