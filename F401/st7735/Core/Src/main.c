/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "st7735.h"
#include "fonts.h"
#include "ov2640.h"

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

const uint8_t OV_WIDTH = 160;
const uint8_t OV_HEIGHT = 80;
uint8_t image[OV_HEIGHT][OV_WIDTH*2] = {0};

uint16_t ov_id;
uint32_t fps = 0;

char info[5] = "-----";

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
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	
	ST7735_Init();
  ST7735_FillScreen(ST7735_BLACK);
  
  ov_id = ov2640_ReadID(CAMERA_I2C_ADDRESS);
  
  if(ov_id != OV2640_ID) {
    ST7735_WriteString(8, 27, "No OV2640", Font_16x26, ST7735_RED, ST7735_BLACK);
    while(1);
  }
  
  // initial 0v2640
  HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);

  ov2640_Init(CAMERA_I2C_ADDRESS, CAMERA_R160x120);
//  ov2640_Config(CAMERA_I2C_ADDRESS, CAMERA_BLACK_WHITE, CAMERA_BLACK_WHITE_BW, CAMERA_BLACK_WHITE_BW);
  
  CAMERA_IO_Write(CAMERA_I2C_ADDRESS, 0XFF, 0X00);
  CAMERA_IO_Write(CAMERA_I2C_ADDRESS, 0XD3, 48);
  CAMERA_IO_Write(CAMERA_I2C_ADDRESS, 0XFF, 0X01);
  CAMERA_IO_Write(CAMERA_I2C_ADDRESS, 0x11, 1);
	
  HAL_Delay(1000);
  HAL_TIM_Base_Start_IT(&htim1);
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    
    HAL_Delay(100);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void get_image() {
  while(HAL_GPIO_ReadPin(VSYNC_GPIO_Port, VSYNC_Pin));
  
  for(uint8_t index_row=0;index_row<OV_HEIGHT;index_row++) {
    while(!HAL_GPIO_ReadPin(HREF_GPIO_Port, HREF_Pin));
    
    uint16_t index_col = 0;
    while(HAL_GPIO_ReadPin(HREF_GPIO_Port, HREF_Pin)){
      while(!HAL_GPIO_ReadPin(PCLK_GPIO_Port, PCLK_Pin));
      
      uint8_t data = 0x00;
      
      data |= HAL_GPIO_ReadPin(Y0_GPIO_Port, Y0_Pin);
      data |= HAL_GPIO_ReadPin(Y1_GPIO_Port, Y1_Pin)<<1;
      data |= HAL_GPIO_ReadPin(Y2_GPIO_Port, Y2_Pin)<<2;
      data |= HAL_GPIO_ReadPin(Y3_GPIO_Port, Y3_Pin)<<3;
      data |= HAL_GPIO_ReadPin(Y4_GPIO_Port, Y4_Pin)<<4;
      data |= HAL_GPIO_ReadPin(Y5_GPIO_Port, Y5_Pin)<<5;
      data |= HAL_GPIO_ReadPin(Y6_GPIO_Port, Y6_Pin)<<6;
      data |= HAL_GPIO_ReadPin(Y7_GPIO_Port, Y7_Pin)<<7;
      
      image[index_row][index_col] = data;
      
      while(HAL_GPIO_ReadPin(PCLK_GPIO_Port, PCLK_Pin));
      fps++;
      
      index_col++;
      
      if(index_col >= OV_WIDTH*2) {
        break;
      }
    }
  }
  
  // display
  uint16_t i=0,j=0;
  for(j=0;j<OV_HEIGHT;j++){
    for(i=0;i<OV_WIDTH;i++){
      if(j<10 &&i < 35){
//        ST7735_DrawPixel(i,j,(image[j][i*2]<<8) | image[j][i*2+1]);
      }else {
        ST7735_DrawPixel(i,j,(image[j][i*2]<<8) | image[j][i*2+1]);
      }
    }
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
//  // row num
//  while(!HAL_GPIO_ReadPin(HREF_GPIO_Port, HREF_Pin));
//  while(HAL_GPIO_ReadPin(HREF_GPIO_Port, HREF_Pin)){
//    while(!HAL_GPIO_ReadPin(PCLK_GPIO_Port, PCLK_Pin));
//    while(HAL_GPIO_ReadPin(PCLK_GPIO_Port, PCLK_Pin));
//    fps++;
//  }
  
  info[4] = fps%10 + '0';
  info[3] = fps/10%10 + '0';
  info[2] = fps/100%10 + '0';
  info[1] = fps/1000%10 + '0';
  info[0] = fps/10000%10 + '0';
  
  ST7735_WriteString(0, 0, info, Font_7x10, ST7735_RED, ST7735_BLACK);
  
  fps = 0;
  
  get_image();
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
