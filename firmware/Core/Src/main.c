/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
GPIO_PinState bitstatus;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void GSM_Commands(void);

/* USER CODE END PM */

/* Private variables
 /*---------------------------------------------------------*/
//Protocols Declaration

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;

/* USER CODE BEGIN PV */


char str1[30];
int len;
int count;
int a,r;


//SWITCH INTERRUPT
volatile int switch_flag=0;


//GPS
uint8_t Rxdata[750];
char txdata[750];
char GPS_Payyload[100];
uint8_t Flag=0;
static int Msgindex;
char *ptr;
double time, Latitude, Longitude;


char Check[800];
char data_init[80];
char data_latitude[80];
char data_longitude[80];

float data_lat_int;
float data_long_int;

char data_lat_char;
char data_long_char;

char *lat_token;
char *long_token;
char lat_token_data1[20];
char lat_token_data2[20];

char RxBufer[512];
char TxBuffer[512];
char ATcommand[512];
uint8_t ATisOK = 0;


///check
char char_lat_token[100];


//GPS LOCATION
double float_lat;
double float_long;
float float_lat1;
float float_long1;
double convert_lat;
double convert_long;
char data_LAT_INT[50];
char data_LAT_FLOAT[50];


//GSM Commands
//1. AT+Commands
char GSMTest[30]="AT\r\n";
char TextMode[30]="AT+CMGF=1\r\n";
char Message[50]="HELLO RECEIVE";
char MsgRead[50]="AT+CMGR=1\r\n";
char MsgRead2[50];
char MsgDelete[100] ="AT+CMGDA=\"DEL ALL\"\r\n";
char MsgDelete1[100] = "AT+CMGD=1\r\n";
char AuthorNo[20]="XXXXXXXXX1";   // authorised owner number (placeholder)
char Dis_unsolicited[30] = "AT+CNMI=0,0,0,0,0\r\n";
char ATcommand[512];



//2. Mobile Numbers
char mobileNumber[]  = "XXXXXXXXX1";
char mobileNumber1[] = "XXXXXXXXX2";
char mobileNumber2[] = "XXXXXXXXX3";
char mobileNumber3[] = "XXXXXXXXX4";

//3. GSM reply Buffer
char reply1[20];
char reply2[50];
char reply3[500];
char reply4[200];


//CAR_Mode-- Driver Code
GPIO_PinState SW2;

//PC_Mode-- Panic Button
GPIO_PinState SW1;

char check_lat1[50];
char check_lat2[50];
char check_long1[50];
char check_long2[50];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_CAN1_Init(void);
static void MX_UART4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	 if(Flag==1)
    {
	  // DELETE ALL MSG (GSM-Command)
	  HAL_UART_Transmit(&huart2, (uint8_t*)MsgDelete, strlen(MsgDelete), HAL_MAX_DELAY);
	  HAL_UART_Receive(&huart2,(uint8_t*)reply4,200, 500);
	  HAL_UART_Transmit(&huart3, (uint8_t*)reply4, strlen(reply4), HAL_MAX_DELAY);

	  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
  	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
  	  HAL_Delay(1000);
  	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);

  	  //Function CALL
  	  GSM_Commands();
  	  HAL_Delay(1000);

  	  Flag=0;
    }
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
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
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
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

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 21;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_12TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_4TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  huart2.Init.BaudRate = 9600;
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

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Relay_on_CAR_ON_GPIO_Port, Relay_on_CAR_ON_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : Relay_on_CAR_ON_Pin */
  GPIO_InitStruct.Pin = Relay_on_CAR_ON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(Relay_on_CAR_ON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PC_MODE_SWITCH_Pin CAR_IGNITION_SWITCH_Pin */
  GPIO_InitStruct.Pin = PC_MODE_SWITCH_Pin|CAR_IGNITION_SWITCH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

/* USER CODE BEGIN 4 */
//SWITCH INTERRUPT

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	Flag=1;
}


//GSM

void GSM_Commands(void)
{


	/*
	#Mode1 -> Car Purpose
	#Mode2 -> Parent Child
	*/

///////////////////////////////////////////////////////////////////////////////////////////////////
	//...........CODE START..............//


// COMMON GSM COMMAND for ALL
	//1. Basic Commands (GSM-ON Command)
	  HAL_Delay(1000);
	  HAL_UART_Transmit(&huart2, (uint8_t*)GSMTest, strlen(GSMTest), HAL_MAX_DELAY);
	  HAL_UART_Receive(&huart2,(uint8_t*)reply1,10, 500);
	  HAL_UART_Transmit(&huart3, (uint8_t*)reply1, strlen(reply1), HAL_MAX_DELAY);

	//2. Text-Mode Commands (GSM-ON Command)

	  HAL_UART_Transmit(&huart2, (uint8_t*)TextMode, strlen(TextMode), HAL_MAX_DELAY);
	  HAL_UART_Receive(&huart2,(uint8_t*)reply2,50, 500);
	  HAL_UART_Transmit(&huart3, (uint8_t*)reply2, strlen(reply2), HAL_MAX_DELAY);

	 //4. un solicited disable
	  HAL_UART_Transmit(&huart2, (uint8_t*)Dis_unsolicited, strlen(Dis_unsolicited), HAL_MAX_DELAY);
	  HAL_UART_Receive(&huart2,(uint8_t*)reply4,200, 500);
	  HAL_UART_Transmit(&huart3, (uint8_t*)reply4, strlen(reply4), HAL_MAX_DELAY);

	//3. DELETE ALL MSG
	  HAL_UART_Transmit(&huart2, (uint8_t*)MsgDelete, strlen(MsgDelete), HAL_MAX_DELAY);
	  HAL_UART_Receive(&huart2,(uint8_t*)reply4,200, 500);
	  HAL_UART_Transmit(&huart3, (uint8_t*)reply4, strlen(reply4), HAL_MAX_DELAY);

	 //4. un solicited disable
	  HAL_UART_Transmit(&huart2, (uint8_t*)Dis_unsolicited, strlen(Dis_unsolicited), HAL_MAX_DELAY);
	  HAL_UART_Receive(&huart2,(uint8_t*)reply4,200, 500);
	  HAL_UART_Transmit(&huart3, (uint8_t*)reply4, strlen(reply4), HAL_MAX_DELAY);

//=====================================================================================================================//
//=====================================================================================================================//

//Initial Mode Checking CODE
while(!strstr(reply3, AuthorNo))
	  {
		  HAL_UART_Transmit(&huart2, (uint8_t*)MsgRead, strlen(MsgRead), HAL_MAX_DELAY);
		  HAL_UART_Receive(&huart2,(uint8_t*)reply3,500, 500);
		  HAL_UART_Transmit(&huart3, (uint8_t*)reply3, strlen(reply3), HAL_MAX_DELAY);

		  SW2 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13);
	  //A. CAR Ignition ON (Mode Check)
	  if(SW2 != 0)
	  	  {
		  	  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
		  	  HAL_Delay(1000);
		  	  HAL_Delay(1000);
		  	  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET);
		  	  HAL_Delay(1000);
	  	  }

	  //B. CAR Ignition ON via GSM Command (Mode Check)
	  else if(strstr(reply3, "CAR_ON"))
			  {
				  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
				  HAL_Delay(1000);
				  HAL_Delay(1000);
				  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET);
				  HAL_Delay(1000);
			  }

	  //C. Enter into CAR Mode (Mode Check)
	  else if(strstr(reply3, "CAR_Mode"))
	  	  	  {
			 //5. Message Read Command

			  	  HAL_UART_Transmit(&huart2, (uint8_t*)MsgRead, strlen(MsgRead), HAL_MAX_DELAY);
			  	  HAL_UART_Receive(&huart2,(uint8_t*)reply3,500, 500);
			  	  HAL_UART_Transmit(&huart3, (uint8_t*)reply3, strlen(reply3), HAL_MAX_DELAY);

			  //-----------
				//1. Basic Commands (GSM-ON Command)
				  HAL_Delay(1000);
				  HAL_UART_Transmit(&huart2, (uint8_t*)GSMTest, strlen(GSMTest), HAL_MAX_DELAY);
				  HAL_UART_Receive(&huart2,(uint8_t*)reply1,10, 500);
				  HAL_UART_Transmit(&huart3, (uint8_t*)reply1, strlen(reply1), HAL_MAX_DELAY);

				//2. Text-Mode Commands (GSM-ON Command)

				  HAL_UART_Transmit(&huart2, (uint8_t*)TextMode, strlen(TextMode), HAL_MAX_DELAY);
				  HAL_UART_Receive(&huart2,(uint8_t*)reply2,50, 500);
				  HAL_UART_Transmit(&huart3, (uint8_t*)reply2, strlen(reply2), HAL_MAX_DELAY);

			   //3. Msg to Owner
				  sprintf(ATcommand,"AT+CMGS=\"%s\"\r\n",mobileNumber);
				  HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
				  HAL_Delay(1000);

				  sprintf(ATcommand,"Hello..CAR MODE ACTIVE.. %c", 0x1a);
				  HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
				  HAL_Delay(2000);

			  goto check1;
			  }

	  //D. Enter into PC Mode (Mode Check)
	  else if(strstr(reply3, "PC_Mode"))
			  {

			   //5. Message Read Command

			  	  HAL_UART_Transmit(&huart2, (uint8_t*)MsgRead, strlen(MsgRead), HAL_MAX_DELAY);
			  	  HAL_UART_Receive(&huart2,(uint8_t*)reply3,500, 500);
			  	  HAL_UART_Transmit(&huart3, (uint8_t*)reply3, strlen(reply3), HAL_MAX_DELAY);


			  	//1. Basic Commands (GSM-ON Command)
			  	  HAL_Delay(1000);
			  	  HAL_UART_Transmit(&huart2, (uint8_t*)GSMTest, strlen(GSMTest), HAL_MAX_DELAY);
			  	  HAL_UART_Receive(&huart2,(uint8_t*)reply1,10, 500);
			  	  HAL_UART_Transmit(&huart3, (uint8_t*)reply1, strlen(reply1), HAL_MAX_DELAY);

			  	//2. Text-Mode Commands (GSM-ON Command)

			  	  HAL_UART_Transmit(&huart2, (uint8_t*)TextMode, strlen(TextMode), HAL_MAX_DELAY);
			  	  HAL_UART_Receive(&huart2,(uint8_t*)reply2,50, 500);
			  	  HAL_UART_Transmit(&huart3, (uint8_t*)reply2, strlen(reply2), HAL_MAX_DELAY);

			  	//3.Msg to Owner
			  	  sprintf(ATcommand,"AT+CMGS=\"%s\"\r\n",mobileNumber);
			  	  HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
			  	  HAL_Delay(1000);

			  	  sprintf(ATcommand,"Hello..PC MODE ACTIVE.. %c", 0x1a);
			  	  HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
			  	  HAL_Delay(2000);

			  goto check2;
	  	  	  }

	  //E. keep Deleting Msges
	  else
		  	  {
			 	//DELETE ALL MSG
			  	  HAL_UART_Transmit(&huart2, (uint8_t*)MsgDelete, strlen(MsgDelete), HAL_MAX_DELAY);
			  	  HAL_UART_Receive(&huart2,(uint8_t*)reply4,200, 500);
			  	  HAL_UART_Transmit(&huart3, (uint8_t*)reply4, strlen(reply4), HAL_MAX_DELAY);
		  	  }
	   }
//============================================================================================================//
//============================================================================================================//

//ENTER into Individual Mode

//============================================================================================================//
//============================================================================================================//

//Mode 1-- CAR - MODE

check1:
//-------------------------------------------------------------------------//
	 //Fetch GPS LOCATION
		label1:
				Msgindex=0;
				strcpy(txdata, (char*)Rxdata);
				ptr=strstr(txdata,"PRMC");
				if(*ptr=='P')
				{
					for(int i=0;i<=37;i++)
						{
							GPS_Payyload[Msgindex]= *ptr;
							Msgindex++;
							*ptr=*(ptr+Msgindex);

					//LATITUDE-DATA
							data_latitude[0] = GPS_Payyload[17];
							data_latitude[1] = GPS_Payyload[18];
							data_latitude[2] = GPS_Payyload[19];
							data_latitude[3] = GPS_Payyload[20];
							data_latitude[4] = GPS_Payyload[21];
							data_latitude[5] = GPS_Payyload[22];
							data_latitude[6] = GPS_Payyload[23];
							data_latitude[7] = GPS_Payyload[24];
							data_latitude[8] = GPS_Payyload[25];


					//LONGITUDE-DATA
							data_longitude[0] = GPS_Payyload[30];
							data_longitude[1] = GPS_Payyload[31];
							data_longitude[2] = GPS_Payyload[32];
							data_longitude[3] = GPS_Payyload[33];
							data_longitude[4] = GPS_Payyload[34];
							data_longitude[5] = GPS_Payyload[35];
							data_longitude[6] = GPS_Payyload[36];
							data_longitude[7] = GPS_Payyload[37];
							data_longitude[8] = GPS_Payyload[38];

						if(Msgindex == 40)
							{
							 goto label1;
							}

					//LATITUDE - UART Transmit
						HAL_UART_Transmit(&huart3,(uint8_t *)data_latitude, strlen(data_latitude), HAL_MAX_DELAY);
						HAL_UART_Transmit(&huart3,(uint8_t *)"\r\n", 3, HAL_MAX_DELAY);


					//LONGITUTDE - UART Transmit
						HAL_UART_Transmit(&huart3,(uint8_t *)data_longitude, strlen(data_longitude), HAL_MAX_DELAY);
						HAL_UART_Transmit(&huart3,(uint8_t *)"\r\n", 3, HAL_MAX_DELAY);

						}

			// CONVERSION FORMULA

				convert_lat = atof(data_latitude);
				convert_long = atof(data_longitude);
//				convert_lat = convert_lat + 24.00;
//				convert_long = convert_long + 28.00;
				convert_lat = convert_lat + 24.00;
				convert_long = convert_long + 28.00;
				float_lat = convert_lat*10000;
				float_long = convert_long*10000;

			//Latitude - Conversion 1
					sprintf(data_LAT_INT, "%d.%.4d", (int)float_lat/1000000, (int)float_lat%1000000);

					HAL_UART_Transmit(&huart3,(uint8_t *)data_LAT_INT, strlen(data_LAT_INT), HAL_MAX_DELAY);
					HAL_UART_Transmit(&huart3,(uint8_t *)"\r\n", 3, HAL_MAX_DELAY);

			//Longitude - Conversion 1
					sprintf(data_LAT_FLOAT, "%d.%.4d",(int)float_long/1000000,(int)float_long%1000000);

					HAL_UART_Transmit(&huart3,(uint8_t *)data_LAT_FLOAT, strlen(data_LAT_FLOAT), HAL_MAX_DELAY);
					HAL_UART_Transmit(&huart3,(uint8_t *)"\r\n", 3, HAL_MAX_DELAY);


///////////////-----------CHECK------------/////////
					sprintf(check_lat1, "%d",(int)float_lat/1000000);
					HAL_UART_Transmit(&huart3,(uint8_t *)check_lat1, strlen(check_lat1), HAL_MAX_DELAY);
					HAL_UART_Transmit(&huart3,(uint8_t *)"\r\n", 3, HAL_MAX_DELAY);

					sprintf(check_lat2, "%.4d",(int)float_lat%1000000);
					HAL_UART_Transmit(&huart3,(uint8_t *)check_lat2, strlen(check_lat2), HAL_MAX_DELAY);
					HAL_UART_Transmit(&huart3,(uint8_t *)"\r\n", 3, HAL_MAX_DELAY);
/////-------------------Check--------------/////
					sprintf(check_long1, "%d",(int)float_long/1000000);
					HAL_UART_Transmit(&huart3,(uint8_t *)check_long1, strlen(check_long1), HAL_MAX_DELAY);
					HAL_UART_Transmit(&huart3,(uint8_t *)"\r\n", 3, HAL_MAX_DELAY);

					sprintf(check_long2, "%.4d",(int)float_long%1000000);
					HAL_UART_Transmit(&huart3,(uint8_t *)check_long2, strlen(check_long2), HAL_MAX_DELAY);
					HAL_UART_Transmit(&huart3,(uint8_t *)"\r\n", 3, HAL_MAX_DELAY);



					//////--------OWNER of the CAR-------///

//location@ & Safety_OFF
					while(!((strstr(reply3, "Location@"))||(strstr(reply3, "SAFETY_OFF"))))
						{
						location_mode:
						 //5. Message Read Command
								sprintf(MsgRead2,"AT+CMGR=1\r\n");
								HAL_UART_Transmit(&huart2, (uint8_t*)MsgRead2, strlen(MsgRead2), HAL_MAX_DELAY);
								HAL_UART_Receive(&huart2,(uint8_t*)reply3,500, 500);
								HAL_UART_Transmit(&huart3, (uint8_t*)reply3, strlen(reply3), HAL_MAX_DELAY);

								//CAR Ignition Switch (TAMPERING)
								SW2 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13);
								if(SW2 != 0 )
									{
									//1. Message to Contact 1
										sprintf(ATcommand,"AT+CMGS=\"%s\"\r\n",mobileNumber);
										HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
										HAL_Delay(1000);

										sprintf(ATcommand,"Hello..Tampering Alert %c", 0x1a);
										HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
										HAL_Delay(2000);
									}
								// Get CAR Location
								else if(strstr(reply3, "Location@"))
									{
										sprintf(MsgRead2,"AT+CMGR=1\r\n");
										HAL_UART_Transmit(&huart2, (uint8_t*)MsgRead2, strlen(MsgRead2), HAL_MAX_DELAY);
										HAL_UART_Receive(&huart2,(uint8_t*)reply3,500, 500);
										HAL_UART_Transmit(&huart3, (uint8_t*)reply3, strlen(reply3), HAL_MAX_DELAY);

									  //3. DELETE ALL MESSAGES
										HAL_UART_Transmit(&huart2, (uint8_t*)MsgDelete, strlen(MsgDelete), HAL_MAX_DELAY);
										HAL_UART_Receive(&huart2,(uint8_t*)reply4,200, 500);
										HAL_UART_Transmit(&huart3, (uint8_t*)reply4, strlen(reply4), HAL_MAX_DELAY);

										//1. Message to Contact 1
										sprintf(ATcommand,"AT+CMGS=\"%s\"\r\n",mobileNumber);
										HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
										HAL_Delay(1000);

										sprintf(ATcommand,"Hello..Under CAR MODE.. Your Location @.!!!  https://maps.google.com/?q=%s,%s %c",data_LAT_INT ,data_LAT_FLOAT, 0x1a);
										HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
										HAL_Delay(2000);

										HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
										HAL_Delay(5000);
										HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

										//Send Data to DATABASE via Node MCU
										HAL_UART_Transmit(&huart4, (uint8_t*)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);

										//4. DELETE ALL MESSAGES
										HAL_UART_Transmit(&huart2, (uint8_t*)MsgDelete, strlen(MsgDelete), HAL_MAX_DELAY);
										HAL_UART_Receive(&huart2,(uint8_t*)reply4,200, 500);
										HAL_UART_Transmit(&huart3, (uint8_t*)reply4, strlen(reply4), HAL_MAX_DELAY);

										// Clear Reply Buffer
										memset(reply3, 0, sizeof(reply3));
									goto location_mode;
									}


								//Get Out of CAR Mode
							   else if(strstr(reply3, "SAFETY_OFF"))
									{
									   sprintf(MsgRead2,"AT+CMGR=1\r\n");
									   HAL_UART_Transmit(&huart2, (uint8_t*)MsgRead2, strlen(MsgRead2), HAL_MAX_DELAY);
									   HAL_UART_Receive(&huart2,(uint8_t*)reply3,500, 500);
									   HAL_UART_Transmit(&huart3, (uint8_t*)reply3, strlen(reply3), HAL_MAX_DELAY);

			//---------------------------------------------------//
									   //1. Message to Contact 1
									   sprintf(ATcommand,"AT+CMGS=\"%s\"\r\n",mobileNumber);
									   HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
									   HAL_Delay(1000);

									   sprintf(ATcommand,"Hello..SAFETY Turned off...CAR_Mode Deactivated %c", 0x1a);
									   HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
									   HAL_Delay(2000);
									   HAL_UART_Transmit(&huart3, (uint8_t*)"OUT OF CAR MODE", 15, HAL_MAX_DELAY);

									   //4. DELETE ALL MSG
									   HAL_UART_Transmit(&huart2, (uint8_t*)MsgDelete, strlen(MsgDelete), HAL_MAX_DELAY);
									   HAL_UART_Receive(&huart2,(uint8_t*)reply4,200, 500);
									   HAL_UART_Transmit(&huart3, (uint8_t*)reply4, strlen(reply4), HAL_MAX_DELAY);

									   //Clear Reply Buffer
									   memset(reply3, 0, sizeof(reply3));

									   //Recursion
									   GSM_Commands();


									}
								//Keep Deleting Messages
							   else
									{
									 //4. DELETE ALL MSG
									  HAL_UART_Transmit(&huart2, (uint8_t*)MsgDelete, strlen(MsgDelete), HAL_MAX_DELAY);
									  HAL_UART_Receive(&huart2,(uint8_t*)reply4,200, 500);
									  HAL_UART_Transmit(&huart3, (uint8_t*)reply4, strlen(reply4), HAL_MAX_DELAY);

									 //Clear Reply Buffer
									  memset(reply3, 0, sizeof(reply3));
									}
						}
				}



//============================================================================================================//
//============================================================================================================//

//Mode 1-- PC - MODE
//-------------------------------------------------//
check2:

//-------------------------------------------------//
		 label2:
			Msgindex=0;
			strcpy(txdata, (char*)Rxdata);
			ptr=strstr(txdata,"PRMC");
			if(*ptr=='P')
			{
				for(int i=0;i<=37;i++)
					{
						GPS_Payyload[Msgindex]= *ptr;
						Msgindex++;
						*ptr=*(ptr+Msgindex);

						//LATITUDE - DATA
						data_latitude[0] = GPS_Payyload[17];
						data_latitude[1] = GPS_Payyload[18];
						data_latitude[2] = GPS_Payyload[19];
						data_latitude[3] = GPS_Payyload[20];
						data_latitude[4] = GPS_Payyload[21];
						data_latitude[5] = GPS_Payyload[22];
						data_latitude[6] = GPS_Payyload[23];
						data_latitude[7] = GPS_Payyload[24];
						data_latitude[8] = GPS_Payyload[25];

						//LONGITUDE - DATA
						data_longitude[0] = GPS_Payyload[30];
						data_longitude[1] = GPS_Payyload[31];
						data_longitude[2] = GPS_Payyload[32];
						data_longitude[3] = GPS_Payyload[33];
						data_longitude[4] = GPS_Payyload[34];
						data_longitude[5] = GPS_Payyload[35];
						data_longitude[6] = GPS_Payyload[36];
						data_longitude[7] = GPS_Payyload[37];
						data_longitude[8] = GPS_Payyload[38];

						if(Msgindex == 40)
							{
							goto label2;
							}

						//LATITUDE - UART DATA
						HAL_UART_Transmit(&huart3,(uint8_t *)data_latitude, strlen(data_latitude), HAL_MAX_DELAY);
						HAL_UART_Transmit(&huart3,(uint8_t *)"\r\n", 3, HAL_MAX_DELAY);

						//LONGITUTDE - UART DATA
						HAL_UART_Transmit(&huart3,(uint8_t *)data_longitude, strlen(data_longitude), HAL_MAX_DELAY);
						HAL_UART_Transmit(&huart3,(uint8_t *)"\r\n", 3, HAL_MAX_DELAY);

					}

						// CONVERSION FORMULA

						convert_lat = atof(data_latitude);
						convert_long = atof(data_longitude);
						convert_lat = convert_lat + 24.00;
						convert_long = convert_long + 28.00;
						float_lat = convert_lat*10000;
						float_long = convert_long*10000;

						//Latitude - Conversion-1
						sprintf(data_LAT_INT, "%d.%.4d", (int)float_lat/1000000, (int)float_lat%1000000);

						HAL_UART_Transmit(&huart3,(uint8_t *)data_LAT_INT, strlen(data_LAT_INT), HAL_MAX_DELAY);
						HAL_UART_Transmit(&huart3,(uint8_t *)"\r\n", 3, HAL_MAX_DELAY);

						//Longitude - Conversion-1
						sprintf(data_LAT_FLOAT, "%d.%.4d",(int)float_long/1000000,(int)float_long%1000000);

						HAL_UART_Transmit(&huart3,(uint8_t *)data_LAT_FLOAT, strlen(data_LAT_FLOAT), HAL_MAX_DELAY);
						HAL_UART_Transmit(&huart3,(uint8_t *)"\r\n", 3, HAL_MAX_DELAY);

/////////------- MESSAGE ------///////////////////
				//location@ & Safety_OFF
						while(!((SW1!=0)||(strstr(reply3, "SAFETY_OFF"))))
							{
							pc_mode:
								SW1 =  HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);

								 //5. Message Read Command
								sprintf(MsgRead2,"AT+CMGR=1\r\n");
								HAL_UART_Transmit(&huart2, (uint8_t*)MsgRead2, strlen(MsgRead2), HAL_MAX_DELAY);
								HAL_UART_Receive(&huart2,(uint8_t*)reply3,500, 500);
								HAL_UART_Transmit(&huart3, (uint8_t*)reply3, strlen(reply3), HAL_MAX_DELAY);

								//5. Message Read Command
								sprintf(MsgRead2,"AT+CMGR=1\r\n");
								HAL_UART_Transmit(&huart2, (uint8_t*)MsgRead2, strlen(MsgRead2), HAL_MAX_DELAY);
								HAL_UART_Receive(&huart2,(uint8_t*)reply3,500, 500);
								HAL_UART_Transmit(&huart3, (uint8_t*)reply3, strlen(reply3), HAL_MAX_DELAY);

								//Panic Button
								if(SW1 != 0)
									{
									HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);

									sprintf(MsgRead2,"AT+CMGR=1\r\n");
								    HAL_UART_Transmit(&huart2, (uint8_t*)MsgRead2, strlen(MsgRead2), HAL_MAX_DELAY);
								    HAL_UART_Receive(&huart2,(uint8_t*)reply3,500, 500);
								    HAL_UART_Transmit(&huart3, (uint8_t*)reply3, strlen(reply3), HAL_MAX_DELAY);

								    //4. DELETE ALL MESSAGES
									HAL_UART_Transmit(&huart2, (uint8_t*)MsgDelete, strlen(MsgDelete), HAL_MAX_DELAY);
									HAL_UART_Receive(&huart2,(uint8_t*)reply4,200, 500);
									HAL_UART_Transmit(&huart3, (uint8_t*)reply4, strlen(reply4), HAL_MAX_DELAY);

									// Send data to DATABASE via NODEMCU
									HAL_UART_Transmit(&huart4, (uint8_t*)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);


									//1. Message to Contact 1
									sprintf(ATcommand,"AT+CMGS=\"%s\"\r\n",mobileNumber);
									HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
									HAL_Delay(1000);
									sprintf(ATcommand,"Hello.. Its EMERGENCY.!!!  https://maps.google.com/?q=%s,%s %c",data_LAT_INT ,data_LAT_FLOAT, 0x1a);
									HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
									HAL_Delay(2000);


									// 2. Message to Contact 2
									sprintf(ATcommand,"AT+CMGS=\"%s\"\r\n", mobileNumber1);
									HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
									HAL_Delay(1000);
									sprintf(ATcommand,"Hello.. Its EMERGENCY.!!!  https://maps.google.com/?q=%s,%s %c",data_LAT_INT ,data_LAT_FLOAT, 0x1a);
									HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
									HAL_Delay(2000);

									// 3. Message to Contact 3
									sprintf(ATcommand,"AT+CMGS=\"%s\"\r\n",mobileNumber2);
									HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
									HAL_Delay(1000);
									sprintf(ATcommand,"Hello.. Its EMERGENCY.!!!  https://maps.google.com/?q=%s,%s %c",data_LAT_INT ,data_LAT_FLOAT, 0x1a);
									HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
									HAL_Delay(2000);

									// 4. Message to Contact 4
									sprintf(ATcommand,"AT+CMGS=\"%s\"\r\n",mobileNumber3);
									HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
									HAL_Delay(1000);
									sprintf(ATcommand,"Hello.. Its EMERGENCY.!!! https://maps.google.com/?q=%s,%s %c",data_LAT_INT ,data_LAT_FLOAT, 0x1a);
									HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
									HAL_Delay(6000);

									/////////------- CALL ------///////////////////

									sprintf(ATcommand,"ATD%s;\r\n", mobileNumber);
									HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
									HAL_Delay(1000);
									HAL_Delay(1000);
									HAL_Delay(1000);
									HAL_Delay(1000);


									HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
									HAL_Delay(5000);
									HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

									// 4. DELETE ALL MESSAGE
									HAL_UART_Transmit(&huart2, (uint8_t*)MsgDelete, strlen(MsgDelete), HAL_MAX_DELAY);
									HAL_UART_Receive(&huart2,(uint8_t*)reply4,200, 500);
									HAL_UART_Transmit(&huart3, (uint8_t*)reply4, strlen(reply4), HAL_MAX_DELAY);

									//Clear Reply Buffer
									memset(reply3, 0, sizeof(reply3));

								goto pc_mode;
							  }
							  else if(SW1==0)
							  {
								  //Get Out of PC_Mode
								  if(strstr(reply3, "SAFETY_OFF"))
									 {
									  sprintf(MsgRead2,"AT+CMGR=1\r\n");
									  HAL_UART_Transmit(&huart2, (uint8_t*)MsgRead2, strlen(MsgRead2), HAL_MAX_DELAY);
									  HAL_UART_Receive(&huart2,(uint8_t*)reply3,500, 500);
									  HAL_UART_Transmit(&huart3, (uint8_t*)reply3, strlen(reply3), HAL_MAX_DELAY);

				//---------------------------------------------------//
									  //1. Message to Contact 1
									  sprintf(ATcommand,"AT+CMGS=\"%s\"\r\n",mobileNumber);
									  HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
									  HAL_Delay(1000);

									  sprintf(ATcommand,"Hello..PC_Mode Turned off...PC_Mode Deactivated %c", 0x1a);
									  HAL_UART_Transmit(&huart2, (uint8_t *)ATcommand, strlen(ATcommand), HAL_MAX_DELAY);
									  HAL_Delay(2000);
									  HAL_UART_Transmit(&huart3, (uint8_t*)"OUT OF PC MODE", 15, HAL_MAX_DELAY);

								    //4. DELETE ALL Messages
									  HAL_UART_Transmit(&huart2, (uint8_t*)MsgDelete, strlen(MsgDelete), HAL_MAX_DELAY);
									  HAL_UART_Receive(&huart2,(uint8_t*)reply4,200, 500);
									  HAL_UART_Transmit(&huart3, (uint8_t*)reply4, strlen(reply4), HAL_MAX_DELAY);

									 //Clear Reply Buffer
									  memset(reply3, 0, sizeof(reply3));

									  //Recursion
									  GSM_Commands();
							  }
							  else
							  {
								 //4. DELETE ALL Messages
								  HAL_UART_Transmit(&huart2, (uint8_t*)MsgDelete, strlen(MsgDelete), HAL_MAX_DELAY);
								  HAL_UART_Receive(&huart2,(uint8_t*)reply4,200, 500);
								  HAL_UART_Transmit(&huart3, (uint8_t*)reply4, strlen(reply4), HAL_MAX_DELAY);

								  goto pc_mode;
							  }
						  }
					}
			}
HAL_Delay(100);

}

//Switch Interrupt
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch_flag=1;

	HAL_UART_Receive_DMA(&huart3, (uint8_t *)Rxdata, 700);

}
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
