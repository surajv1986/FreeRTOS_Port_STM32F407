/*
 * queue.c
 *
 *  Created on: 13-Nov-2023
 *      Author: Dell
 */

#include <stdio.h>
#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"

TaskHandle_t myTk1Handle = NULL;
TaskHandle_t myTk2Handle = NULL;

QueueHandle_t myQueue;

void myTk1()
{
	char myTxBuff[30];

	myQueue = xQueueCreate(5, sizeof(myTxBuff));
	sprintf(myTxBuff, "message 1");
	/* Third parameter for the below API is the blocking time, in this case its zero */
	// xQueueSend(myQueue, (void *)myTxBuff, (TickType_t) 0);
	/* This FreeRTOS Queue API sends the data in the reverse order */
	xQueueSendToFront(myQueue, (void *)myTxBuff, (TickType_t) 0);


	sprintf(myTxBuff, "message 2");
	//xQueueSend(myQueue, (void *)myTxBuff, (TickType_t) 0);
	xQueueSendToFront(myQueue, (void *)myTxBuff, (TickType_t) 0);

	sprintf(myTxBuff, "message 3");
	//xQueueSend(myQueue, (void *)myTxBuff, (TickType_t) 0);
	xQueueSendToFront(myQueue, (void *)myTxBuff, (TickType_t) 0);

	sprintf(myTxBuff, "message 4");
	/* This API overwrites the previous queue data i.e. "message 3"
	 * will be overwritten by "message 4" in this case */
	xQueueOverwrite(myQueue, (void *)myTxBuff);

	xQueueReset(myQueue);

	printf("Data waiting to be read by task2 is: %d\r\n", uxQueueMessagesWaiting(myQueue));
	printf("Available spaces: %d\r\n", uxQueueSpacesAvailable(myQueue));

	while(1){

	}

}

void myTk2()
{
	char myRxBuff[30];

	vTaskDelay(200);

	while(1){
		if (myQueue != 0) {
			/* Print the Buffer value only if there is data read from the queue */
			/* Note: xQueueReceive is destructive
			 * i.e. it removes the data in the queue after reading it */
			if (xQueueReceive(myQueue, (void *)myRxBuff, (TickType_t) 5)) { /* Third parameter is the
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	  	  	  	 blocking time in this case its five
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	   	   	   	 to give some time for the data to be read  */
				printf("data received : %s \r\n", myRxBuff);
			}
			/* Note: xQueueReceive is non-destructive
			 * i.e. it does not remove the data in the queue after reading it */
			if (xQueuePeek(myQueue, (void *)myRxBuff, (TickType_t) 5)) { /* Third parameter is the
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	  	  	  	 blocking time in this case its five
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	   	   	   	 to give some time for the data to be read  */
				printf("data received : %s \r\n", myRxBuff);
			}
		}
	}

}

void init_queue()
{
	BaseType_t xReturn = 0;

	xReturn = xTaskCreate(myTk1, "Task1", 200, (void *) 0, tskIDLE_PRIORITY, &myTk1Handle);
	  if (xReturn != pdPASS) {
		  printf("Error creating task1!!!");
	  }
	  xReturn = xTaskCreate(myTk2, "Task2", 200, (void *) 0, tskIDLE_PRIORITY, &myTk2Handle);
	    if (xReturn != pdPASS) {
	  	  printf("Error creating task2!!!");
	    }

}
