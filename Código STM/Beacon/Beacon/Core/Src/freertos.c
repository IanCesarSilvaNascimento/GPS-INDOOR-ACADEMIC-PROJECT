/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include <math.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "usart.h"
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
#define power_1m -32
#define prop_meio_x10 24.6

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
uint32_t defaultTaskBuffer[ 128 ];
osStaticThreadDef_t defaultTaskControlBlock;
osThreadId ProcessaDadosHandle;
uint32_t ProcessaDadosBuffer[ 1024 ];
osStaticThreadDef_t ProcessaDadosControlBlock;
osMessageQId DataRxHandle;
uint8_t DataRxBuffer[ 16 * sizeof( Beacon_t ) ];
osStaticMessageQDef_t DataRxControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

float_t rssi_to_meter(int8_t rssi);

   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void funcProcessaDados(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

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

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of DataRx */
  osMessageQStaticDef(DataRx, 16, Beacon_t, DataRxBuffer, &DataRxControlBlock);
  DataRxHandle = osMessageCreate(osMessageQ(DataRx), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadStaticDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128, defaultTaskBuffer, &defaultTaskControlBlock);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of ProcessaDados */
  osThreadStaticDef(ProcessaDados, funcProcessaDados, osPriorityNormal, 0, 1024, ProcessaDadosBuffer, &ProcessaDadosControlBlock);
  ProcessaDadosHandle = osThreadCreate(osThread(ProcessaDados), NULL);

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
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	uint8_t aTxHdlc[]= {0x7E,0x00,0x04,0x08,0x01,0x41,0x53,0x62};



  /* Infinite loop */
  for(;;)
  {
	  if(UartReady == IDLE)
	  {
		  HAL_UART_Transmit_DMA(&huart1, aTxHdlc, sizeof(aTxHdlc));
		  UartReady = BUSY;
	  }
	  else
	  {
		  osDelay(100);
	  }

	  osDelay(5000);

  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_funcProcessaDados */
/**
* @brief Function implementing the ProcessaDados thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_funcProcessaDados */
void funcProcessaDados(void const * argument)
{
  /* USER CODE BEGIN funcProcessaDados */
	float_t dist_B1_B2_Fixa = 1.1;
	float_t distance[64];
	float_t ang_alfa;
	float_t ang_beta;
	float_t d1y1;
	float_t d1y2;
	uint32_t dist_y;
	uint32_t dist_x;
	float_t d1x1;
	float_t d1x2;

	uint16_t panID[64];
	//uint8_t aTxHdlc[127];
	static Beacon_t Device;

	portBASE_TYPE xHigherPriorityTaskWoken = pdTRUE;

	uint8_t cnt = 0;
	uint8_t indice = 0;
  /* Infinite loop */
  for(;;)
  {
	  while(xQueueReceiveFromISR(DataRxHandle, &Device, &xHigherPriorityTaskWoken) == pdTRUE)
	  {
		  if((Device.panId[0]==0x8A&&Device.panId[1]==0x8E)||(Device.panId[0]==0xC9&&Device.panId[1]==0x6F))
		  {
			  cnt++;
			  distance[indice] = rssi_to_meter(Device.rssi);
			  panID[indice++] = ((Device.panId[0]<<8)|Device.panId[1]);
		  }

		  if(indice>1 && indice%2==0)
		  {

			  if((panID[indice-2]==0x8A8E && panID[indice-1]==0xC96F)||(panID[indice-2]==0xC96F && panID[indice-1]==0x8A8E))
			  {
				  if(panID[indice-2]!=panID[indice-1])
				  {
					  ang_alfa = acos((pow(distance[indice-2],2)- pow(dist_B1_B2_Fixa,2) - pow(distance[indice-1],2))/(-2*dist_B1_B2_Fixa*distance[indice-1]));
					  ang_beta = acos((pow(distance[indice-1],2)- pow(dist_B1_B2_Fixa,2) - pow(distance[indice-2],2))/(-2*dist_B1_B2_Fixa*distance[indice-2]));

					  if(ang_alfa>=ang_beta&&distance[indice-2]<=distance[indice-1])
					  {
					  	d1y1 = 1000*(sin(ang_alfa)*distance[indice-2]);
					  	d1y2 = 1000*(sin(ang_beta)*distance[indice-1]);

					  	d1x1 = 1000*(cos(ang_alfa)*distance[indice-2]);
					  	d1x2 = 1000*(cos(ang_beta)*distance[indice-1]);

					  	dist_x = ((d1x1+((1000*dist_B1_B2_Fixa)-d1x2))/2);

					  	dist_y = ((d1y1+d1y2)/2);
					  }

					  if(ang_alfa<=ang_beta&&distance[indice-2]<=distance[indice-1])
					  {
					   	d1y1 = 1000*(sin(ang_beta)*distance[indice-2]);
					  	d1y2 = 1000*(sin(ang_alfa)*distance[indice-1]);

					  	d1x1 = 1000*(cos(ang_beta)*distance[indice-2]);
					  	d1x2 = 1000*(cos(ang_alfa)*distance[indice-1]);

					  	dist_x = ((d1x1+((1000*dist_B1_B2_Fixa)-d1x2))/2);

					  	dist_y = ((d1y1+d1y2)/2);
					  }

					  if(ang_alfa>=ang_beta&&distance[indice-2]>=distance[indice-1])
					  {
					   	d1y1 = 1000*(sin(ang_alfa)*distance[indice-1]);
					   	d1y2 = 1000*(sin(ang_beta)*distance[indice-2]);

					   	d1x1 = 1000*(cos(ang_alfa)*distance[indice-1]);
					   	d1x2 = 1000*(cos(ang_beta)*distance[indice-2]);

					   	dist_x = ((d1x1+((1000*dist_B1_B2_Fixa)-d1x2))/2);

					   	dist_y = ((d1y1+d1y2)/2);
					  }

					  if(ang_alfa<=ang_beta&&distance[indice-2]>=distance[indice-1])
					  {
						d1y1 = 1000*(sin(ang_beta)*distance[indice-1]);
					  	d1y2 = 1000*(sin(ang_alfa)*distance[indice-2]);

					  	d1x2 = 1000*(cos(ang_beta)*distance[indice-1]);
					  	d1x1 = 1000*(cos(ang_alfa)*distance[indice-2]);

					  	dist_x = ((d1x1+((1000*dist_B1_B2_Fixa)-d1x2))/2);

					  	dist_y = ((d1y1+d1y2)/2);
					  }

					  send_x_distance(dist_x);

					  send_y_distance(dist_y);
				  }
			  }
		  }
		  if(indice>63)
		  {
			  indice = 0;
		  }


	  }


//	  if(UartReady == IDLE)
//	 	  {
//	 		  HAL_UART_Transmit_DMA(&huart1, aTxHdlc, tamanho de sua msg);
//	 		  UartReady = BUSY;
//	 	  }
//	 	  else
//	 	  {
//	 		  osDelay(10);
//	 	  }

    osDelay(100);
  }
  /* USER CODE END funcProcessaDados */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
float_t rssi_to_meter(int8_t rssi){

    float_t meters = 0;

    meters = pow(10,((power_1m-rssi)/prop_meio_x10));
    return meters;
  }

void send_x_distance(uint32_t value_x){
	uint8_t send_msg[]= {0x7E,0x00,0x1B,0x10,0x01,0x00,0x13,0xA2,0x00,0x40,0xB0,0x98,0x96,0xFF,0xFE,0x00,0x00,0x76,0x61,0x6C,0x6F,0x72,0x78,0x20,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xE8};
	uint8_t divider_x[6];
	long pot_1;
	long pot_2;
	uint8_t count=0;
	uint8_t cont = 0;
	long sum = 0;

	for(count=0;count<6;count++)
	{
		pot_1 = pow(10,6-count);
		pot_2 = pow(10,5-count);

		divider_x[count] = (value_x%(pot_1)/pot_2);

		send_msg[24+count] = 0x30+divider_x[count];
	}

	for(cont=3;cont<30;cont++)
		{
			sum = sum+send_msg[cont];
		}

	send_msg[30] = (0xFF-(sum&0xFF));

	if(UartReady == IDLE)
		  {
			  HAL_UART_Transmit_DMA(&huart1, send_msg, sizeof(send_msg));
			  UartReady = BUSY;
			  osDelay(500);
		  }
		  else
		  {
			  osDelay(100);
		  }


  }

void send_y_distance(uint32_t value_y){
	uint8_t send_msg[]= {0x7E,0x00,0x1B,0x10,0x01,0x00,0x13,0xA2,0x00,0x40,0xB0,0x98,0x96,0xFF,0xFE,0x00,0x00,0x76,0x61,0x6C,0x6F,0x72,0x79,0x20,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xE7};
	uint8_t divider_y[6];
	uint8_t count=0;
	uint8_t cont = 0;
	long pot_1;
	long pot_2;

	long sum = 0;

	for(count=0;count<6;count++)
	{
		pot_1 = pow(10,6-count);
		pot_2 = pow(10,5-count);

		divider_y[count] = (value_y%(pot_1)/pot_2);

		send_msg[24+count] = 0x30+divider_y[count];
	}

	for(cont=3;cont<30;cont++)
	{
		sum = sum+send_msg[cont];
	}
	send_msg[30] = (0xFF-(sum&0xFF));

	if(UartReady == IDLE)
			  {
				  HAL_UART_Transmit_DMA(&huart1, send_msg, sizeof(send_msg));
				  UartReady = BUSY;
				  osDelay(500);
			  }
			  else
			  {
				  osDelay(100);
			  }


  }
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
