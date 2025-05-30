#include "Initialization.h"
#include "stm32f4xx_hal.h"
#include "DataType.h"
#include "Commands.h"
#include "Interrupt.h"
#include "Error.h"
#include "Sensor.h"
#include "Utility.h"




// Khởi tạo các biến toàn cục
//========================================================================================
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;
CAN_HandleTypeDef hcan1;
DAC_HandleTypeDef hdac;
TIM_HandleTypeDef htim6;
UART_HandleTypeDef huart2;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim2;
//========================================================================================


// Các hàm thư viện HAL 
//========================================================================================
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM6_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_DAC_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM2_Init(void);
void Error_Handler(void);
//========================================================================================

// Các hàm user
//========================================================================================
static void SystemInitialization(void);
static void LCDCommunication(void);
static void RunSystemForTest(void);

//========================================================================================






// Hàm main
//========================================================================================
//========================================================================================
void InitializationState(StateMachine *CurrentStateMachine)
{

  // Khởi tạo cho hệ thống (khởi tạo các module, peripheral)
  SystemInitialization();


  // Lấy giá trị offset của các cảm biến
  GetOffsetSensor();

  // set thời gian để timeout chuyển state
  TimeoutSwapState = TIMEOUT_SWAP_STATE;

  // Tắt động cơ
  SetMotor(MIN_DAC_MOTOR); 


  // Khởi tạo filter
  InitFilter();

  while (1)
  {


    // Giao tiếp với LCD
    LCDCommunication();

    // Lấy giá trị offset của các cảm biến
    GetOffsetSensor();

    // Đọc cảm biến
    ReadSensor();

    // Chạy hệ thống để test và lấy offset
	  RunSystemForTest();


    CheckOverrall(); // Kiểm tra hệ thống theo chu kì, cập nhật biến "FaultMask"
    // Kiểm tra lỗi hệ thống, nếu có lỗi thì chuyển trạng thái 
    if (FaultMask != FAULT_NONE)
    {
      TimeoutSwapState = TIMEOUT_SWAP_STATE;
      AlarmLed(LED_YELLOW);
    }
    else
    {
      AlarmLed(LED_GREEN);
    }

    // Time out để chuyển trạng thái
    if (!TimeoutSwapState)
    {
      *CurrentStateMachine = STANDBY_STATE;
      return;
    }
  }

}
//========================================================================================
//========================================================================================







// BODY FUNCTION USER 
//========================================================================================
static void LCDCommunication(void)
{
  if (Timer100msFlag)
  {
    CommLCDConnectAlive();
    CommLCDStateMachine(INITIALIZATION_STATE);
    CommLCDFaultCode(FaultMask);

    Timer100msFlag = 0;
  }
}

// Khoi tao toan bo he thong
static void SystemInitialization(void)
{

  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN1_Init();
  MX_TIM6_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_DAC_Init();
  MX_TIM7_Init();
  MX_TIM2_Init();


  HAL_ADC_Start_DMA(&hadc1, adc1_buffer, 3);
  HAL_ADC_Start_DMA(&hadc2, adc2_buffer, 3);

  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);

  // Khởi tạo CAN
  HAL_CAN_Start(&hcan1); // Bắt đầu mô-đun CAN để sẵn sàng gửi và nhận dữ liệu
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING); // Kích hoạt ngắt khi có tin nhắn chờ trong FIFO0 (CAN_IT_RX_FIFO0_MSG_PENDING)

  // Khởi tạo UART
	HAL_UART_Receive_IT(&huart2, &RxData, 1); // Ngắt nhận 1 byte data từ UART

	// Khởi tạo timer
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
}


// Chạy mô hình để test và lấy thông số
static void RunSystemForTest(void)
{
  switch (StepRunSystem)
  {
    case 1:
    // Tăng tốc động cơ tăng dần để kiểm tra speed sensor 
    SetLoad(LOAD_0); // Chạy động cơ không tải 
    RunMotor(&DACSpeedSensorReference1, &ADCCurrentVotolReference1);
    TimeoutSwapState = TIMEOUT_SWAP_STATE;
    break;

    case 2:
    // Tăng tốc động cơ tăng dần để kiểm tra speed sensor 
    // Chạy động cơ full tải 
    SetLoad(LOAD_4);
    RunMotor(&DACSpeedSensorReference2, &ADCCurrentVotolReference2);
    TimeoutSwapState = TIMEOUT_SWAP_STATE;
    break;

    case 3:
    // Tắt động cơ, bật tải, để lấy torque offset lúc không tải
    SetMotor(MIN_DAC_MOTOR);
    SetLoad(LOAD_1);
    TimeoutSwapState = TIMEOUT_SWAP_STATE;
    break;

    default:
    // Tắt động cơ, tắt tải, để lấy torque offset lúc không tải
    SetMotor(MIN_DAC_MOTOR);
    SetLoad(LOAD_0);
    break;

  }




  // Time out để chuyển step tiếp theo
  if (!TimeoutRunSystem)
  {
    StepRunSystem ++;
    TimeoutRunSystem = TIMEOUT_RUN_SYSTEM;
  }

}


//========================================================================================


































































// BODY FUNCTION HAL 
//========================================================================================
static void SystemClock_Config(void)
{
	  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	  /** Configure the main internal regulator output voltage
	  */
	  __HAL_RCC_PWR_CLK_ENABLE();
	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	  /** Initializes the RCC Oscillators according to the specified parameters
	  * in the RCC_OscInitTypeDef structure.
	  */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	  RCC_OscInitStruct.PLL.PLLM = 4;
	  RCC_OscInitStruct.PLL.PLLN = 50;
	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	  RCC_OscInitStruct.PLL.PLLQ = 7;
	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /** Initializes the CPU, AHB and APB buses clocks
	  */
	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	  {
	    Error_Handler();
	  }
}


static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
 static void MX_DAC_Init(void)
 {
 
   /* USER CODE BEGIN DAC_Init 0 */
 
   /* USER CODE END DAC_Init 0 */
 
   DAC_ChannelConfTypeDef sConfig = {0};
 
   /* USER CODE BEGIN DAC_Init 1 */
 
   /* USER CODE END DAC_Init 1 */
 
   /** DAC Initialization
   */
   hdac.Instance = DAC;
   if (HAL_DAC_Init(&hdac) != HAL_OK)
   {
     Error_Handler();
   }
 
   /** DAC channel OUT2 config
   */
   sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
   sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
   if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
   {
     Error_Handler();
   }
   /* USER CODE BEGIN DAC_Init 2 */
 
   /* USER CODE END DAC_Init 2 */
 
 }

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
 static void MX_ADC2_Init(void)
 {
 
   /* USER CODE BEGIN ADC2_Init 0 */
 
   /* USER CODE END ADC2_Init 0 */
 
   ADC_ChannelConfTypeDef sConfig = {0};
 
   /* USER CODE BEGIN ADC2_Init 1 */
 
   /* USER CODE END ADC2_Init 1 */
 
   /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
   */
   hadc2.Instance = ADC2;
   hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
   hadc2.Init.Resolution = ADC_RESOLUTION_12B;
   hadc2.Init.ScanConvMode = ENABLE;
   hadc2.Init.ContinuousConvMode = ENABLE;
   hadc2.Init.DiscontinuousConvMode = DISABLE;
   hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
   hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
   hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
   hadc2.Init.NbrOfConversion = 3;
   hadc2.Init.DMAContinuousRequests = ENABLE;
   hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
   if (HAL_ADC_Init(&hadc2) != HAL_OK)
   {
     Error_Handler();
   }
 
   /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
   */
   sConfig.Channel = ADC_CHANNEL_14;
   sConfig.Rank = 1;
   sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
   if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
   {
     Error_Handler();
   }
 
   /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
   */
   sConfig.Channel = ADC_CHANNEL_15;
   sConfig.Rank = 2;
   if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
   {
     Error_Handler();
   }
 
   /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
   */
   sConfig.Channel = ADC_CHANNEL_8;
   sConfig.Rank = 3;
   if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
   {
     Error_Handler();
   }
   /* USER CODE BEGIN ADC2_Init 2 */
 
   /* USER CODE END ADC2_Init 2 */
 
 }

 /**
   * @brief ADC1 Initialization Function
   * @param None
   * @retval None
   */
 static void MX_ADC1_Init(void)
 {

   /* USER CODE BEGIN ADC1_Init 0 */

   /* USER CODE END ADC1_Init 0 */

   ADC_ChannelConfTypeDef sConfig = {0};

   /* USER CODE BEGIN ADC1_Init 1 */

   /* USER CODE END ADC1_Init 1 */

   /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
   */
   hadc1.Instance = ADC1;
   hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
   hadc1.Init.Resolution = ADC_RESOLUTION_12B;
   hadc1.Init.ScanConvMode = ENABLE;
   hadc1.Init.ContinuousConvMode = ENABLE;
   hadc1.Init.DiscontinuousConvMode = DISABLE;
   hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
   hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
   hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
   hadc1.Init.NbrOfConversion = 3;
   hadc1.Init.DMAContinuousRequests = ENABLE;
   hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
   if (HAL_ADC_Init(&hadc1) != HAL_OK)
   {
     Error_Handler();
   }

   /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
   */
   sConfig.Channel = ADC_CHANNEL_0;
   sConfig.Rank = 1;
   sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
   if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
   {
     Error_Handler();
   }

   /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
   */
   sConfig.Channel = ADC_CHANNEL_1;
   sConfig.Rank = 2;
   if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
   {
     Error_Handler();
   }

   /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
   */
   sConfig.Channel = ADC_CHANNEL_2;
   sConfig.Rank = 3;
   if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
   {
     Error_Handler();
   }
   /* USER CODE BEGIN ADC1_Init 2 */

   /* USER CODE END ADC1_Init 2 */

 }


/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{


    hcan1.Instance = CAN1;
    hcan1.Init.Prescaler = 5;
    hcan1.Init.Mode = CAN_MODE_NORMAL;
//    hcan1.Init.Mode = CAN_MODE_LOOPBACK; //test debug
//    hcan1.Init.Mode = CAN_MODE_SILENT_LOOPBACK; //test debug
    hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
    hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
    hcan1.Init.TimeTriggeredMode = DISABLE;
    hcan1.Init.AutoBusOff = ENABLE;	// Tự động phục hồi bus
    hcan1.Init.AutoWakeUp = DISABLE;
    hcan1.Init.AutoRetransmission = ENABLE; // tránh lỗi khi gửi liên tục
    hcan1.Init.ReceiveFifoLocked = DISABLE;
    hcan1.Init.TransmitFifoPriority = DISABLE;
    if (HAL_CAN_Init(&hcan1) != HAL_OK)
    {
      Error_Handler();
    }

    CAN_FilterTypeDef sFilterConfig;

    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) {
        Error_Handler();
    }

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */

static void MX_USART2_UART_Init(void)
{

	  /* USER CODE BEGIN USART2_Init 0 */

	  /* USER CODE END USART2_Init 0 */

	  /* USER CODE BEGIN USART2_Init 1 */

	  /* USER CODE END USART2_Init 1 */
	  huart2.Instance = USART2;
	  huart2.Init.BaudRate = 115200;
	  huart2.Init.WordLength = UART_WORDLENGTH_8B;
	  huart2.Init.StopBits = UART_STOPBITS_1;
	  huart2.Init.Parity = UART_PARITY_NONE;
	  huart2.Init.Mode = UART_MODE_TX_RX;
	  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	  if (HAL_UART_Init(&huart2) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  /* USER CODE BEGIN USART2_Init 2 */

	  /* USER CODE END USART2_Init 2 */

}

static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;

  htim6.Init.Period = 4999;       // PSC = 4999 (50MHz -> 10kHz)
  htim6.Init.Prescaler = 99;     // ARR = 999 (100ms)
  
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */



}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();


  /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);

    /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
    GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


//      /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);
//
//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);
//
//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);
//
//  /*Configure GPIO pin : PE13 */
//  GPIO_InitStruct.Pin = GPIO_PIN_13;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
//
//  /*Configure GPIO pins : PB10 PB11 PB14 PB15 */
//  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_15;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//  /*Configure GPIO pin : PB14 */
//  GPIO_InitStruct.Pin = GPIO_PIN_14;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
//  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//  /*Configure GPIO pins : PC6 PC7 */
//  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//
//  /*Configure GPIO pins : PA8 PA9 */
//  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//  /*Configure GPIO pin : PA15 */
//  GPIO_InitStruct.Pin = GPIO_PIN_15;
//  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//  /*Configure GPIO pin : PD0 */
//  GPIO_InitStruct.Pin = GPIO_PIN_0;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
//
//
//  /* EXTI interrupt init*/
//  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);











  /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);

    /*Configure GPIO pin : PE13 */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pin : PB13 */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : PD9 PD10 PD11 */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : PC6 PC7 */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : PA8 PA9 */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : PD0 */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);




    /* Configure GPIO Output Level */
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10|GPIO_PIN_12, GPIO_PIN_RESET);

    /* Configure GPIO pin : PD1 */
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* Configure GPIO pins : PC10 PC12 */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}



static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 50 -1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;

  sConfigIC.ICFilter = 4; // Lọc input capture 

  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}


static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */
  


  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 999;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 2499;


  htim6.Init.Period = 4999;       // PSC = 4999 (50MHz -> 10kHz)
  htim6.Init.Prescaler = 1999;     // ARR = 999 (200ms)


  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}
/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
