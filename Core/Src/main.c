/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#include "main.h"

#include "Initialization.h"
#include "StandbyState.h"
#include "StandaloneState.h"
#include "UIState.h"
#include "DataType.h"

extern StateMachine CurrentStateMachine = INITIALIZATION_STATE;		//extern de debug

int main(void)
{

  while (1)
  {

    switch (CurrentStateMachine)
    {
      case INITIALIZATION_STATE:
      InitializationState(&CurrentStateMachine);
      break;

      case STANDBY_STATE:
      StandbyState(&CurrentStateMachine);
      break;

      case STANDALONE_STATE:
      StandaloneState(&CurrentStateMachine);
      break;

      case UI_STATE:
      UIState(&CurrentStateMachine);
      break;

      case FAULT_STATE:
	  FaultState(FAULT_STATE);
      break;

	  }

  }

}



#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
