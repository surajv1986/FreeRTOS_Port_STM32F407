/*
 * freertos_mutex.c
 *
 *  Created on: 14-Nov-2023
 *      Author: Dell
 */


#include "main.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "freertos_utilities.h"
#include "task.h"
#include "semphr.h"

TaskHandle_t myThread1Handle = NULL;
TaskHandle_t myThread2Handle = NULL;

char mySharedResource[10];
xSemaphoreHandle xMutex;

void myThread1(void *p)
{
	char task1Msg[10] = "potatoes";

	while (1) {

		int i;

		/* Check If the Mutex lock is acquired. */
		if (xSemaphoreTake(xMutex, (TickType_t) 0xFFFFFFFF) == 1) {
			/* Enter the Critical Section */
			for (i = 0; i < 9; i++) {
				mySharedResource[i] = task1Msg[i];
				vTaskDelay(50);
			}
			mySharedResource[i] = 0;
			/* Release the Mutex. */
			xSemaphoreGive(xMutex);
			/* Exit the Critical Section */
		}
		/* If the Mutex lock is not acquired then print the
		 * shared variable value and wait for some time.
		 * */
		printf("T1: %s\r\n", mySharedResource);
		vTaskDelay(1000);
	}

}

void myThread2(void *p)
{
	char task2Msg[10] = "potatoes";

		while (1) {
			int i;

			/* Check If the Mutex lock is acquired. */
			if (xSemaphoreTake(xMutex, (TickType_t) 0xFFFFFFFF) == 1) {
				/* Enter the Critical Section */
				for (i = 0; i < 9; i++) {
					mySharedResource[i] = task2Msg[i];
					vTaskDelay(50);
				}
				mySharedResource[i] = 0;
				/* Release the Mutex. */
				xSemaphoreGive(xMutex);
				/* Exit the Critical Section */
			}
			/* If the Mutex lock is not acquired then print the
			 * shared variable value and wait for some time.
			 * */
			printf("T2: %s\r\n", mySharedResource);
			vTaskDelay(1000);
		}
}

void freertos_mutex()
{
	BaseType_t xReturn = 0;

	xReturn = xTaskCreate(myThread1, "Task1", 200, (void *) 0, tskIDLE_PRIORITY, &myThread1Handle);
	if (xReturn != pdPASS) {
		  printf("Error creating task1!!!");
	}
    xReturn = xTaskCreate(myThread2, "Task2", 200, (void *) 0, tskIDLE_PRIORITY, &myThread2Handle);
	if (xReturn != pdPASS) {
	  	  printf("Error creating task2!!!");
	}

	/* Since in FreeRTOS, the same handle type i.e. SemaphoreHandle_t
	 * is used for both Semaphore & Mutex, the mutex is specified using
	 * the below API in the main function for the program. ss*/
	xMutex = xSemaphoreCreateMutex();

}