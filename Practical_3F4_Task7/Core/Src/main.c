/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * @brief          : CODE MODIFIED FOR EEE3096S PRACTICAL 3, TASK 7 (FIXED-POINT SCALING - ALL DIMS)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_ITER 100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// --- ADDED FOR TASK 7 ---
const int image_sizes[5] = {128, 160, 192, 224, 256};
const int64_t scaling_factors[3] = {1000, 1000000, 65536}; // 10^3, 10^6, 2^16

// 2D arrays to store results. Structure: results[size_index][scale_index]
uint32_t execution_times[5][3];
uint64_t checksums[5][3];

volatile int test_complete = 0;
// --- END OF ADDED CODE ---
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
// --- ADDED FOR TASK 7 ---
uint64_t calculate_mandelbrot_fixed_point(int width, int height, int max_iterations, int64_t scale);
// --- END OF ADDED CODE ---
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
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  // --- ADDED FOR TASK 7 ---
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // LED on: Start

  // Outer loop: iterate through image sizes
  for (int i = 0; i < 5; i++) {
	  int current_size = image_sizes[i];

	  // Inner loop: iterate through scaling factors
	  for (int j = 0; j < 3; j++) {
		  int64_t current_scale = scaling_factors[j];

		  uint32_t start_time = HAL_GetTick();
		  checksums[i][j] = calculate_mandelbrot_fixed_point(current_size, current_size, MAX_ITER, current_scale);
		  execution_times[i][j] = HAL_GetTick() - start_time;
	  }
  }

  test_complete = 1;
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // LED on: Finish
  // --- END OF ADDED CODE ---
  /* USER CODE END 2 */

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief System Clock Configuration
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 144;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */
// --- ADDED FOR TASK 7 ---
uint64_t calculate_mandelbrot_fixed_point(int width, int height, int max_iterations, int64_t scale) {
    uint64_t mandelbrot_sum = 0;
    const int64_t LIMIT = 4 * scale * scale;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int64_t c_real = (int64_t)((((double)x / width) * 3.5 - 2.5) * scale);
            int64_t c_imag = (int64_t)((((double)y / height) * 2.0 - 1.0) * scale);
            int64_t z_real = 0;
            int64_t z_imag = 0;
            int iteration = 0;
            while (iteration < max_iterations) {
                int64_t z_real_sq = z_real * z_real;
                int64_t z_imag_sq = z_imag * z_imag;
                if ((z_real_sq + z_imag_sq) > LIMIT) {
                    break;
                }
                int64_t z_imag_new = (2 * z_real * z_imag) / scale + c_imag;
                int64_t z_real_new = (z_real_sq - z_imag_sq) / scale + c_real;
                z_real = z_real_new;
                z_imag = z_imag_new;
                iteration++;
            }
            mandelbrot_sum += iteration;
        }
    }
    return mandelbrot_sum;
}
// --- END OF ADDED CODE ---
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */