/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
TaskHandle_t myTask1Handle = NULL;
TaskHandle_t myTask2Handle = NULL;
TaskHandle_t myTask3Handle = NULL;
TaskHandle_t myIntTaskHandle = NULL;
BaseType_t xReturn = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void myTask1(void *);
void myTask2(void *);
void myTask3(void *);
void myIntTask(void *);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*
 * @brief : A FreeRTOS Task Handler example.
 * @param : A void argument pointer.
 * @return: None.
 */
void myTask1(void *p)
{
	int count = 0;
	TickType_t myLastUnblock;

	while(1){
		printf("count is %d\r\n", count++);
		//vTaskDelay(1000);  // This is not accurate, its based on the no. of systicks not seconds or milliseconds.
		//vTaskDelay(1000 * configTICK_RATE_HZ / 1000); //This is in milliseconds.
		//vTaskDelay(pdMS_TO_TICKS(3000)); //This function i.e. pdMS_TO_TICKS can be used instead of the above equation, it also gives delay in milliseconds.
		/*Pls note that vTaskDelay is not very accurate,
		 * for accurate delays in ms you can use vTaskDelayUntil
		 * as shown below */
		myLastUnblock = xTaskGetTickCount();
		vTaskDelayUntil(&myLastUnblock, pdMS_TO_TICKS(1000));
		if (count >= 30){
			vTaskDelete(myTask1Handle);
		}
	}

}
/*
 * @brief : A FreeRTOS Task Handler example with a single argument handled.
 * @param : A void argument pointer.
 * @return: None.
 */
void myTask2(void *p)
{
	int count = (int *)p;
	TickType_t myLastUnblock;

	while(1){
		printf("count is %d\r\n", count++);
		//vTaskDelay(1000);
		//vTaskDelay(pdMS_TO_TICKS(1000));
		myLastUnblock = xTaskGetTickCount();
		vTaskDelayUntil(&myLastUnblock, pdMS_TO_TICKS(1000));
		if (count >= 30){
			vTaskDelete(myTask2Handle);
		}
	}

}

void myTask3(void *p)
{

	while(1){
		vTaskDelay(5000 * configTICK_RATE_HZ / 1000);
		vTaskSuspend(myTask2Handle);
		vTaskDelay(5000 * configTICK_RATE_HZ / 1000);
		vTaskResume(myTask2Handle);
	}

}

void myIntTask(void *p)
{
	while(1) {
		vTaskSuspend(NULL); //Suspend Itself.
		vTaskDelay(1000 * configTICK_RATE_HZ / 1000);

	}

}

/*
 * @brief : Configure on discovery board Button for interrupt.
 * @param : None.
 * @return: None.
 * */
void button_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	HAL_NVIC_SetPriority(EXTI0_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}
/*
 * @brief : Interrupt Service Routine for User Button on Discovery Board.
 * @param : None.
 * */
void EXTI0_IRQHandler()
{
	BaseType_t checkIfYieldIsRequired;

	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
	checkIfYieldIsRequired = xTaskResumeFromISR(myIntTaskHandle);
	portYIELD_FROM_ISR(checkIfYieldIsRequired);

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  int pass = 1;
  printf("Hello RTOS\r\n");
  button_init();
  /*
   * @param1: Task Handler name.
   * @param2: "Unique String for the new task".
   * @param3: Stack Size in words,not in bytes.
   * @param4: Argument(s) to be passed to the task handler.
   * @param5: Priority of the task can be 0,1,2,3,4 here a macro is used for zero.
   * @param6: Pointer to a handle for the task.
   * */
#if 0
  xReturn = xTaskCreate(myTask1, "Task1", 200, (void *) 0, tskIDLE_PRIORITY, &myTask1Handle);
  if (xReturn != pdPASS) {
	  printf("Error creating task1!!!");
  }
  xReturn = xTaskCreate(myTask2, "Task2", 200, (void *) pass, tskIDLE_PRIORITY, &myTask2Handle);
    if (xReturn != pdPASS) {
  	  printf("Error creating task2!!!");
    }
   xReturn = xTaskCreate(myTask3, "Task3", 200, (void *) 0, tskIDLE_PRIORITY, &myTask3Handle);
   if (xReturn != pdPASS) {
      	  printf("Error creating task2!!!");
   }
#endif
   xReturn = xTaskCreate(myTask2, "Task2", 200, (void *) pass, tskIDLE_PRIORITY, &myTask2Handle);
       if (xReturn != pdPASS) {
     	  printf("Error creating task2!!!");
       }
   xReturn = xTaskCreate(myIntTask, "IntTask", 200, (void *) 0, tskIDLE_PRIORITY, &myIntTaskHandle);
   if (xReturn != pdPASS) {
      printf("Error creating IntTask!!!");
   }
  vTaskStartScheduler();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
