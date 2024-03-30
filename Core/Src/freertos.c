/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "scpi/scpi.h"
#include "scpi-def.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticQueue_t osStaticMessageQDef_t;
typedef StaticTimer_t osStaticTimerDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for taskParseCmd */
osThreadId_t taskParseCmdHandle;
const osThreadAttr_t taskParseCmd_attributes = {
  .name = "taskParseCmd",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for taskFeedWD */
osThreadId_t taskFeedWDHandle;
const osThreadAttr_t taskFeedWD_attributes = {
  .name = "taskFeedWD",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for cmdRXQueue */
osMessageQueueId_t cmdRXQueueHandle;
uint8_t cmdRXQueueBuffer[ 512 * sizeof( uint8_t ) ];
osStaticMessageQDef_t cmdRXQueueControlBlock;
const osMessageQueueAttr_t cmdRXQueue_attributes = {
  .name = "cmdRXQueue",
  .cb_mem = &cmdRXQueueControlBlock,
  .cb_size = sizeof(cmdRXQueueControlBlock),
  .mq_mem = &cmdRXQueueBuffer,
  .mq_size = sizeof(cmdRXQueueBuffer)
};
/* Definitions for cmdTxQueue */
osMessageQueueId_t cmdTxQueueHandle;
uint8_t cmdTxQueueBuffer[ 512 * sizeof( uint8_t ) ];
osStaticMessageQDef_t cmdTxQueueControlBlock;
const osMessageQueueAttr_t cmdTxQueue_attributes = {
  .name = "cmdTxQueue",
  .cb_mem = &cmdTxQueueControlBlock,
  .cb_size = sizeof(cmdTxQueueControlBlock),
  .mq_mem = &cmdTxQueueBuffer,
  .mq_size = sizeof(cmdTxQueueBuffer)
};
/* Definitions for cmdReceiveTimeout */
osTimerId_t cmdReceiveTimeoutHandle;
osStaticTimerDef_t cmdReceiveTimeoutControlBlock;
const osTimerAttr_t cmdReceiveTimeout_attributes = {
  .name = "cmdReceiveTimeout",
  .cb_mem = &cmdReceiveTimeoutControlBlock,
  .cb_size = sizeof(cmdReceiveTimeoutControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTaskParseCmd(void *argument);
void StartTaskFeedWD(void *argument);
void cmdReceiveTimeoutCallback(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of cmdReceiveTimeout */
  cmdReceiveTimeoutHandle = osTimerNew(cmdReceiveTimeoutCallback, osTimerOnce, NULL, &cmdReceiveTimeout_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of cmdRXQueue */
  cmdRXQueueHandle = osMessageQueueNew (512, sizeof(uint8_t), &cmdRXQueue_attributes);

  /* creation of cmdTxQueue */
  cmdTxQueueHandle = osMessageQueueNew (512, sizeof(uint8_t), &cmdTxQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of taskParseCmd */
  taskParseCmdHandle = osThreadNew(StartTaskParseCmd, NULL, &taskParseCmd_attributes);

  /* creation of taskFeedWD */
  taskFeedWDHandle = osThreadNew(StartTaskFeedWD, NULL, &taskFeedWD_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
	uint8_t data;
	osStatus_t status;
	SCPI_Init(&scpi_context,
					scpi_commands,
					&scpi_interface,
					scpi_units_def,
					SCPI_IDN1, SCPI_IDN2, SCPI_IDN3, SCPI_IDN4,
					scpi_input_buffer, SCPI_INPUT_BUFFER_LENGTH,
					scpi_error_queue_data, SCPI_ERROR_QUEUE_SIZE);	
	for(;;)
  {
		do
		{
			status = osMessageQueueGet(cmdRxQueueHandle, &data, NULL, 0U);
			if (status == osOK)
			{
				SCPI_Input(&scpi_context, &data, 1);
			}
		}while(status == osOK);
    osDelay(1);
  }
  //process_command_task();
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTaskParseCmd */
/**
* @brief Function implementing the taskParseCmd thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskParseCmd */
void StartTaskParseCmd(void *argument)
{
  /* USER CODE BEGIN StartTaskParseCmd */
  /* Infinite loop */
  for(;;)
  {
    FeedWDG();
    osDelay(1);
  }
  /* USER CODE END StartTaskParseCmd */
}

/* USER CODE BEGIN Header_StartTaskFeedWD */
/**
* @brief Function implementing the taskFeedWD thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskFeedWD */
void StartTaskFeedWD(void *argument)
{
  /* USER CODE BEGIN StartTaskFeedWD */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTaskFeedWD */
}

/* cmdReceiveTimeoutCallback function */
void cmdReceiveTimeoutCallback(void *argument)
{
  /* USER CODE BEGIN cmdReceiveTimeoutCallback */

  /* USER CODE END cmdReceiveTimeoutCallback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

