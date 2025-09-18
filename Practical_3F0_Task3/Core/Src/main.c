/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * @brief          : CODE MODIFIED FOR EEE3096S PRACTICAL 3, TASK 3
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
// --- ADDED FOR BENCHMARKING ---
#include <stdint.h> // Required for specific integer types
// --- END OF ADDED CODE ---
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// --- ADDED FOR BENCHMARKING ---
// Define the fixed MAX_ITER for Task 3 comparison
#define MAX_ITER 100
// --- END OF ADDED CODE ---
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// --- ADDED FOR BENCHMARKING ---
// Array of image sizes to test, same as in Practical 1B
const int image_sizes[5] = {128, 160, 192, 224, 256};

// --- Result arrays for Fixed-Point ---
uint32_t fixed_point_times[5];         // Wall-clock time in ms
uint64_t fixed_point_checksums[5];
float    fixed_point_throughput[5];    // Pixels per second

// --- Result arrays for Double-Precision ---
uint32_t double_times[5];              // Wall-clock time in ms
uint64_t double_checksums[5];
float    double_throughput[5];         // Pixels per second

volatile int all_benchmarks_complete = 0;
// --- END OF ADDED CODE ---
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
// --- ADDED FOR BENCHMARKING ---
// Function prototypes for the Mandelbrot algorithms
uint64_t calculate_mandelbrot_fixed_point(int width, int height, int max_iterations);
uint64_t calculate_mandelbrot_double(int width, int height, int max_iterations);
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
  /* USER CODE BEGIN 2 */

  // --- ADDED FOR BENCHMARKING ---
  // Visual indicator: Turn on an LED to signal processing start
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

  // Loop through the 5 image sizes
  for (int i = 0; i < 5; i++) {
	  int current_size = image_sizes[i];
	  uint32_t start_time, end_time;
	  float exec_time_s, throughput;

	  // --- Run Fixed-Point Test ---
	  start_time = HAL_GetTick();
	  fixed_point_checksums[i] = calculate_mandelbrot_fixed_point(current_size, current_size, MAX_ITER);
	  end_time = HAL_GetTick();

	  // Calculate metrics based on wall-clock time
	  fixed_point_times[i] = end_time - start_time;
	  exec_time_s = (float)fixed_point_times[i] / 1000.0f; // Convert ms to seconds
	  if (exec_time_s > 0) {
		  throughput = (current_size * current_size) / exec_time_s;
	  } else {
		  throughput = 0; // Avoid division by zero if too fast
	  }
	  fixed_point_throughput[i] = throughput;

	  // --- Run Double-Precision Test ---
	  start_time = HAL_GetTick();
	  double_checksums[i] = calculate_mandelbrot_double(current_size, current_size, MAX_ITER);
	  end_time = HAL_GetTick();

	  // Calculate metrics based on wall-clock time
	  double_times[i] = end_time - start_time;
	  exec_time_s = (float)double_times[i] / 1000.0f; // Convert ms to seconds
	  if (exec_time_s > 0) {
		  throughput = (current_size * current_size) / exec_time_s;
	  } else {
		  throughput = 0;
	  }
	  double_throughput[i] = throughput;
  }

  // Visual indicator: Turn on another LED to signal processing end
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);

  all_benchmarks_complete = 1;
  // --- END OF ADDED CODE ---
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
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
// --- Mandelbrot functions remain the same ---
uint64_t calculate_mandelbrot_fixed_point(int width, int height, int max_iterations){
    uint64_t mandelbrot_sum = 0;
    const int64_t SCALE = 1000000;
    const int64_t LIMIT = 4 * SCALE * SCALE;
    for (int y = 0; y < height; y++){
      for (int x = 0; x < width; x++){
            int64_t c_real = ((int64_t)x * 3500000) / width - 2500000;
            int64_t c_imag = ((int64_t)y * 2000000) / height - 1000000;
            int64_t z_real = 0;
            int64_t z_imag = 0;
            int iteration = 0;
            while (iteration < max_iterations) {
                int64_t z_real_sq = z_real * z_real;
                int64_t z_imag_sq = z_imag * z_imag;
                if ((z_real_sq + z_imag_sq) > LIMIT) {
                    break;
                }
                int64_t z_imag_new = (2 * z_real * z_imag) / SCALE + c_imag;
                int64_t z_real_new = (z_real_sq - z_imag_sq) / SCALE + c_real;
                z_real = z_real_new;
                z_imag = z_imag_new;
                iteration++;
            }
            mandelbrot_sum += iteration;
        }
    }
    return mandelbrot_sum;
}
uint64_t calculate_mandelbrot_double(int width, int height, int max_iterations){
    uint64_t mandelbrot_sum = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double c_real = ((double)x / width) * 3.5 - 2.5;
            double c_imag = ((double)y / height) * 2.0 - 1.0;
            double z_real = 0.0;
            double z_imag = 0.0;
            int iteration = 0;
            while (iteration < max_iterations && (z_real * z_real + z_imag * z_imag) <= 4.0) {
                double z_real_new = z_real * z_real - z_imag * z_imag + c_real;
                z_imag = 2 * z_real * z_imag + c_imag;
                z_real = z_real_new;
                iteration++;
            }
            mandelbrot_sum += iteration;
        }
    }
    return mandelbrot_sum;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
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