/* USER CODE BEGIN Header */
/**
 ****************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ****************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ****************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "semphr.h"
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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId_t defaultTaskHandle;
osThreadId_t Blink1TaskHandle;
osThreadId_t TriggTaskHandle;
osThreadId_t UserbuttonTaskHandle;
osMutexId_t myMutexHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void Blink1(void *argument);
void Trigg(void *argument);
void Userbutton(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
	/* USER CODE BEGIN Init */

	/* USER CODE END Init */
	osKernelInitialize();

	/* Create the mutex(es) */
	/* definition and creation of myMutex */
	const osMutexAttr_t myMutex_attributes = { .name = "myMutex" };
	myMutexHandle = osMutexNew(&myMutex_attributes);

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* definition and creation of defaultTask */
	const osThreadAttr_t defaultTask_attributes = { .name = "defaultTask",
			.priority = (osPriority_t) osPriorityLow, .stack_size = 128 };
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL,
			&defaultTask_attributes);

	/* definition and creation of Blink1Task */
	const osThreadAttr_t Blink1Task_attributes = { .name = "Blink1Task",
			.priority = (osPriority_t) osPriorityNormal, .stack_size = 128 };
	Blink1TaskHandle = osThreadNew(Blink1, NULL, &Blink1Task_attributes);

	/* definition and creation of TriggTask */
	const osThreadAttr_t TriggTask_attributes = { .name = "TriggTask",
			.priority = (osPriority_t) osPriorityHigh, .stack_size = 128 };
	TriggTaskHandle = osThreadNew(Trigg, NULL, &TriggTask_attributes);

	/* definition and creation of UserbuttonTask */
	const osThreadAttr_t UserbuttonTask_attributes = { .name = "UserbuttonTask",
			.priority = (osPriority_t) osPriorityHigh, .stack_size = 128 };
	UserbuttonTaskHandle = osThreadNew(Userbutton, NULL,
			&UserbuttonTask_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument) {
	/* USER CODE BEGIN StartDefaultTask */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
	/* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Blink1 */
/**
 * @brief Function implementing the Blink1Task thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Blink1 */
void Blink1(void *argument) {
	/* USER CODE BEGIN Blink1 */
	//uint16_t varv = 0, antalvarv = 1;
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	//const TickType_t xPeriod = pdMS_TO_TICKS(100); // ms to ticks
	// Initialise the xLastWakeTime variable with the current time.
	//xLastWakeTime = xTaskGetTickCount();
	/* Infinite loop */
	for (;;) {

		if ((xSemaphoreTake(myMutexHandle,20) == pdTRUE)) {
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); // Blink PC10
			xSemaphoreGive(myMutexHandle);
			vTaskDelayUntil(&xLastWakeTime,100);
		}
	}

	/* USER CODE END Blink1 */
}

/* USER CODE BEGIN Header_Trigg */
/**
 * @brief Function implementing the TriggTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Trigg */
void Trigg(void *argument) {
	/* USER CODE BEGIN Trigg */
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(10); // ms to ticks
// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	/* Infinite loop */
	for (;;) {
		//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_2); // Blink PD2
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
	}
	/* USER CODE END Trigg */
}

/* USER CODE BEGIN Header_Userbutton */
/**
 * @brief Function implementing the UserbuttonTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Userbutton */
void Userbutton(void *argument) {
	/* USER CODE BEGIN Userbutton */
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(20);
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	/* Infinite loop */
	for (;;) {
		if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET) {
			if (xSemaphoreTake(myMutexHandle,0) == pdTRUE) {
			}
		}else
			xSemaphoreGive(myMutexHandle);
			vTaskDelayUntil(&xLastWakeTime, xPeriod);
	}
	/* USER CODE END Userbutton */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
