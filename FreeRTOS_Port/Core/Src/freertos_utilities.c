/*
 * freertos_utilities.c
 *
 *  Created on: 05-Nov-2023
 *      Author: Suraj Vanjeeswaran Iyer
 */

#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "FreeRTOS.h"
#include "freertos_utilities.h"
#include "task.h"

/* USER CODE END Includes */

TaskHandle_t myT1Handle = NULL;
TaskHandle_t myT2Handle = NULL;

char myTaskList[300];



void myT1(void *parameters)
{
	int count = 0;

	TaskHandle_t tskHand;


	while(1) {
		printf("Hello World %d \r\n", count++);
		vTaskDelay(1000 * configTICK_RATE_HZ / 1000);

		tskHand = xTaskGetCurrentTaskHandle();
		//tskHand = xTaskGetIdleTaskHandle(); /* Used to get the Task Handle for an Idle Task */
		if (count == 3) {
			vTaskDelete(tskHand);
		}
	}
}

void myT2(void *parameters)
{
	int count = 0;
	//eTaskState myState = 0;

	while(1) {
		count++;

#if 0
		myState = eTaskGetState(myT2Handle);
		printf("The Current Task state is %d\r\n", myState);
		myState = eTaskGetState(myT2Handle);
		printf("The state of Task 1 is %d\r\n", myState);
		printf("Hey There, the count is %d \r\n", count);
#endif



		vTaskList(myTaskList);
		printf("%s\r\n", myTaskList);
		printf("The total number of tasks running is %d\r\n", uxTaskGetNumberOfTasks());
		printf("The total ticks is : %d\r\n", xTaskGetTickCount()); // Not to be used inside an ISR.
		printf("The total ticks is : %d\r\n", xTaskGetTickCountFromISR()); //Use this variant inside an ISR.
		printf("The task name is : %s \r\n", pcTaskGetName(myT2Handle));
		vTaskDelay(1000 * configTICK_RATE_HZ / 1000);
		if (count == 30) {
			vTaskDelete(xTaskGetHandle("Task2")); /* Delete Task using its name to obtain an handle to the task. */
		}
	}
}



void rtos_utility_demo()
{
	BaseType_t xReturn = 0;

	xReturn = xTaskCreate(myT1, "Task1", 200, (void *) 0, tskIDLE_PRIORITY, &myT1Handle);
	  if (xReturn != pdPASS) {
		  printf("Error creating task1!!!");
	  }
	  xReturn = xTaskCreate(myT2, "Task2", 200, (void *) 0, tskIDLE_PRIORITY, &myT2Handle);
	    if (xReturn != pdPASS) {
	  	  printf("Error creating task2!!!");
	  }

}



