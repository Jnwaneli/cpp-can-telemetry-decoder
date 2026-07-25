/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
    uint16_t ain1_raw;
    uint16_t ain2_raw;
    uint16_t ain3_raw;

    uint16_t battery_mV;
    uint16_t temperature_deciC;

    uint16_t speed_raw;
    uint16_t rpm;

    uint8_t status;
    uint8_t gear;
    uint8_t throttle_percent;
    uint8_t brake_percent;
    uint8_t counter;

    uint32_t timestamp_ms;
} SensorSample;

typedef struct
{
    uint16_t ain1_raw;
    uint16_t ain2_raw;
    uint16_t ain3_raw;

    uint16_t battery_mV;
    uint16_t temperature_deciC;

    uint16_t speed_raw;
    uint16_t rpm;

    uint8_t status;
    uint8_t gear;
    uint8_t throttle_percent;
    uint8_t brake_percent;
    uint8_t counter;

    uint32_t timestamp_ms;
    uint8_t valid;
} ProcessedTelemetry;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

COM_InitTypeDef BspCOMInit;
FDCAN_HandleTypeDef hfdcan1;

/* Definitions for SignalGenerator */
osThreadId_t SignalGeneratorHandle;
const osThreadAttr_t SignalGenerator_attributes = {
  .name = "SignalGenerator",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for ProcessingTask */
osThreadId_t ProcessingTaskHandle;
const osThreadAttr_t ProcessingTask_attributes = {
  .name = "ProcessingTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for CanTxTask */
osThreadId_t CanTxTaskHandle;
const osThreadAttr_t CanTxTask_attributes = {
  .name = "CanTxTask",
  .priority = (osPriority_t) osPriorityAboveNormal,
  .stack_size = 256 * 4
};
/* Definitions for StatusLedTask */
osThreadId_t StatusLedTaskHandle;
const osThreadAttr_t StatusLedTask_attributes = {
  .name = "StatusLedTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* USER CODE BEGIN PV */
FDCAN_TxHeaderTypeDef txHeader;

uint8_t txData[8] = {
    0x00, 0x08,
    0x10, 0x00,
    0xFF, 0x0A,
    0x07,
    0x01
};
osMessageQueueId_t sensorQueueHandle;
osMutexId_t telemetryMutexHandle;

ProcessedTelemetry latestTelemetry = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FDCAN1_Init(void);
void StartSignalGeneratorTask(void *argument);
void StartProcessingTask(void *argument);
void StartCanTxTask(void *argument);
void StartStatusLedTask(void *argument);

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
  MX_FDCAN1_Init();
  /* USER CODE BEGIN 2 */
  txHeader.Identifier = 0x100;
  txHeader.IdType = FDCAN_STANDARD_ID;
  txHeader.TxFrameType = FDCAN_DATA_FRAME;
  txHeader.DataLength = FDCAN_DLC_BYTES_8;
  txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  txHeader.BitRateSwitch = FDCAN_BRS_OFF;
  txHeader.FDFormat = FDCAN_CLASSIC_CAN;
  txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  txHeader.MessageMarker = 0;

  if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
  {
      Error_Handler();
  }

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  telemetryMutexHandle = osMutexNew(NULL);
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  sensorQueueHandle = osMessageQueueNew(8, sizeof(SensorSample), NULL);

  if (sensorQueueHandle == NULL || telemetryMutexHandle == NULL)
  {
      Error_Handler();
  }
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of SignalGenerator */
  SignalGeneratorHandle = osThreadNew(StartSignalGeneratorTask, NULL, &SignalGenerator_attributes);

  /* creation of ProcessingTask */
  ProcessingTaskHandle = osThreadNew(StartProcessingTask, NULL, &ProcessingTask_attributes);

  /* creation of CanTxTask */
  CanTxTaskHandle = osThreadNew(StartCanTxTask, NULL, &CanTxTask_attributes);

  /* creation of StatusLedTask */
  StatusLedTaskHandle = osThreadNew(StartStatusLedTask, NULL, &StatusLedTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Initialize leds */
  BSP_LED_Init(LED_GREEN);

  /* Initialize USER push-button, will be used to trigger an interrupt each time it's pressed.*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Initialize COM1 port (115200, 8 bits (7-bit data + 1 stop bit), no parity */
  BspCOMInit.BaudRate   = 115200;
  BspCOMInit.WordLength = COM_WORDLENGTH_8B;
  BspCOMInit.StopBits   = COM_STOPBITS_1;
  BspCOMInit.Parity     = COM_PARITY_NONE;
  BspCOMInit.HwFlowCtl  = COM_HWCONTROL_NONE;
  if (BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief FDCAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_FDCAN1_Init(void)
{

  /* USER CODE BEGIN FDCAN1_Init 0 */

  /* USER CODE END FDCAN1_Init 0 */

  /* USER CODE BEGIN FDCAN1_Init 1 */

  /* USER CODE END FDCAN1_Init 1 */
  hfdcan1.Instance = FDCAN1;
  hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = DISABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 20;
  hfdcan1.Init.NominalSyncJumpWidth = 1;
  hfdcan1.Init.NominalTimeSeg1 = 13;
  hfdcan1.Init.NominalTimeSeg2 = 3;
  hfdcan1.Init.DataPrescaler = 1;
  hfdcan1.Init.DataSyncJumpWidth = 1;
  hfdcan1.Init.DataTimeSeg1 = 1;
  hfdcan1.Init.DataTimeSeg2 = 1;
  hfdcan1.Init.StdFiltersNbr = 0;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */

  /* USER CODE END FDCAN1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartSignalGeneratorTask */
/**
  * @brief  Function implementing the SignalGenerator thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartSignalGeneratorTask */
void StartSignalGeneratorTask(void *argument)
{
  /* USER CODE BEGIN 5 */

  SensorSample sample;

  uint16_t fake_adc = 1000;
  uint16_t fake_speed = 550;
  uint16_t fake_rpm = 3200;
  uint8_t fake_throttle = 42;
  uint8_t counter = 0;

  for (;;)
  {
    fake_adc += 25;

    if (fake_adc > 3500)
    {
      fake_adc = 1000;
    }

    fake_speed += 2;

    if (fake_speed > 700)
    {
      fake_speed = 550;
    }

    fake_rpm += 25;

    if (fake_rpm > 4200)
    {
      fake_rpm = 3200;
    }

    fake_throttle++;

    if (fake_throttle > 60)
    {
      fake_throttle = 35;
    }

    sample.ain1_raw = fake_adc;
    sample.ain2_raw = fake_adc + 100;
    sample.ain3_raw = fake_adc + 200;

    sample.battery_mV = 12600 + (counter % 20);
    sample.temperature_deciC = 345 + (counter % 10);

    sample.speed_raw = fake_speed;
    sample.rpm = fake_rpm;

    sample.status = 0x07;
    sample.gear = 3;
    sample.throttle_percent = fake_throttle;
    sample.brake_percent = 0;
    sample.counter = counter;
    sample.timestamp_ms = HAL_GetTick();

    osMessageQueuePut(sensorQueueHandle, &sample, 0U, 0U);

    counter++;

    osDelay(10);
  }

  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartProcessingTask */
/**
* @brief Function implementing the ProcessingTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartProcessingTask */
void StartProcessingTask(void *argument)
{
  /* USER CODE BEGIN StartProcessingTask */

  SensorSample sample;

  for (;;)
  {
    if (osMessageQueueGet(sensorQueueHandle, &sample, NULL, osWaitForever) == osOK)
    {
      if (osMutexAcquire(telemetryMutexHandle, osWaitForever) == osOK)
      {
        latestTelemetry.ain1_raw = sample.ain1_raw;
        latestTelemetry.ain2_raw = sample.ain2_raw;
        latestTelemetry.ain3_raw = sample.ain3_raw;

        latestTelemetry.battery_mV = sample.battery_mV;
        latestTelemetry.temperature_deciC = sample.temperature_deciC;

        latestTelemetry.speed_raw = sample.speed_raw;
        latestTelemetry.rpm = sample.rpm;

        latestTelemetry.status = sample.status;
        latestTelemetry.gear = sample.gear;
        latestTelemetry.throttle_percent = sample.throttle_percent;
        latestTelemetry.brake_percent = sample.brake_percent;
        latestTelemetry.counter = sample.counter;

        latestTelemetry.timestamp_ms = sample.timestamp_ms;
        latestTelemetry.valid = 1;

        osMutexRelease(telemetryMutexHandle);
      }
    }
  }

  /* USER CODE END StartProcessingTask */
}

/* USER CODE BEGIN Header_StartCanTxTask */
/**
* @brief Function implementing the CanTxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCanTxTask */
void StartCanTxTask(void *argument)
{
  /* USER CODE BEGIN StartCanTxTask */
	 ProcessedTelemetry localTelemetry = {0};
	  uint8_t localTxData[8];

	  for(;;)
	  {
	    if (osMutexAcquire(telemetryMutexHandle, osWaitForever) == osOK)
	    {
	      localTelemetry = latestTelemetry;
	      osMutexRelease(telemetryMutexHandle);
	    }

	    if (localTelemetry.valid)
	    {
	      localTxData[0] = (uint8_t)(localTelemetry.ain1_raw & 0xFF);
	      localTxData[1] = (uint8_t)((localTelemetry.ain1_raw >> 8) & 0xFF);

	      localTxData[2] = (uint8_t)(localTelemetry.ain2_raw & 0xFF);
	      localTxData[3] = (uint8_t)((localTelemetry.ain2_raw >> 8) & 0xFF);

	      localTxData[4] = (uint8_t)(localTelemetry.ain3_raw & 0xFF);
	      localTxData[5] = (uint8_t)((localTelemetry.ain3_raw >> 8) & 0xFF);

	      localTxData[6] = localTelemetry.status;
	      localTxData[7] = localTelemetry.counter;

	      if (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1) > 0)
	      {
	        HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &txHeader, localTxData);
	      }
	    }

	    osDelay(100);
	  }
  /* USER CODE END StartCanTxTask */
}

/* USER CODE BEGIN Header_StartStatusLedTask */
/**
* @brief Function implementing the StatusLedTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartStatusLedTask */
void StartStatusLedTask(void *argument)
{
  /* USER CODE BEGIN StartStatusLedTask */
	 for(;;)
	  {
	    BSP_LED_Toggle(LED_GREEN);
	    osDelay(500);
	  }
  /* USER CODE END StartStatusLedTask */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
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
#ifdef USE_FULL_ASSERT
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
