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
#include "usart.h"
#include "gpio.h"
#include "string.h"
#include "stdio.h"
#include "math.h"
#define TRUE 1
#define FALSE 0
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern UART_HandleTypeDef huart2;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Temp_Inq(uint8_t data[],uint16_t Size);
void led_drive();
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint32_t Baud_Rate = 115200;

uint8_t Rx_data[12];                 						       // Receive data Buffer
uint8_t tmp_data[1];                                                 	               // Receive data temporary Buffer
uint8_t i=0;                        						       // index variable
uint8_t bool;						                               // Start Stop state boolean variable
uint16_t delayoff=700,delayon=300;	  					       // Delay time variable


/* Note: This function is interrupt event. It calls when receive interrupt ongoing.
 *  It sets to RX interrupt again in for permanence.
 *  Temp_Inq an Echo query work in this event because they must go process after getting Rx_data.
 *
 * */

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart->Instance == USART2){  						// is instance(huart) USART2?

		HAL_UARTEx_ReceiveToIdle_IT(&huart2, Rx_data, sizeof(Rx_data));		// Receive interrupt

		for(i=Size;i<sizeof(Rx_data);i++)					// This is to to reset the old words that exceed the size.
		{
			Rx_data[i]=0;
	}}

	Temp_Inq(Rx_data,Size);								//"Temporary Inquary" function to below explained.


	if(bool==TRUE){									// Bool query makes echo if was started.
			HAL_UART_Transmit_IT(&huart2, Rx_data,Size);

		}
}

/*It asks what is in data series. Including Start, Stop, ledon and ledoff queries.
 * It includes also to hand delay times of "ledon" and ledoff" as integer.
 * */

void Temp_Inq(uint8_t data[],uint16_t Size){
	if (strcmp((char*)data,"START")==0) {						// Start query use strcmp()
		bool = TRUE;

	}else if(strcmp((char*)data,"STOP")==0){					// Stop query use strcmp()
		bool = FALSE;

	}else if((strncmp((char*)data,"ledon=",6) == 0) && (bool != FALSE)){		// To find led on delay time,
											// Use strncmp() to not mix other chars.
		delayon=0;
		i=1;

		while(data[i+6] !='\0')							// To hand delay integer, loop go to after '='.
		{

			delayon = delayon + (data[(Size-1)-i]-48)*pow(10,i);		// To convert char to decimal and addition same time.
			i++;
		}
			delayon = delayon + (data[Size-1]-48);				// Addition of ones digit and find delay on time.

	}else if((strncmp((char*)data,"ledoff=",7)==0) && (bool != FALSE) ){		// Led off function is same the "ledon" query.

		delayoff=0;
		i=1;

		while(data[i+7] !='\0')
		{

			delayoff = delayoff + (data[(Size-1)-i]-48)*pow(10,i);
			i++;
		}
		    delayoff = delayoff + (data[Size-1]-48);
	}
}

/* Led drives according to bool state and delay times.
 * */
void led_drive(){
if(bool==TRUE){

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);					// GPIO PIN is HIGH
	HAL_Delay(delayon);								// Delay time to on

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 0);					// GPIO PIN is LOW
	HAL_Delay(delayoff);								// Delay time to off.

}else{

	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);						// GPIO toggle function.
	HAL_Delay(1000);								// Delay 1s.

}
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
  MX_USART2_UART_Init();
  huart2.Init.BaudRate = Baud_Rate;                          				// Init Parameter for Baud Rate.

  /* USER CODE BEGIN 2 */

  HAL_UARTEx_ReceiveToIdle_IT(&huart2, Rx_data, sizeof(Rx_data));			// First start receive function.

  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {



	     led_drive();								// Led drives in the loop.


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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 96;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
