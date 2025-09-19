/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * @brief          : CODE FOR EEE3096S PRACTICAL 3, TASK 8 (POWER MEASUREMENT)
  ******************************************************************************
  * @attention
  *
  * This code does not perform a measurement itself. It provides two distinct
  * states of operation (Active and Idle) during which an external power
  * measurement tool would be used.
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
#define IMAGE_SIZE 256
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// This volatile variable is just to ensure the compiler doesn't optimize away the loop
volatile uint64_t checksum_fixed_point;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
uint64_t calculate_mandelbrot_fixed_point(int width, int height, int max_iterations);
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

  // --- STATE 1: ACTIVE COMPUTATION ---
  // An external power analyzer would measure the "Active Current" (I_active)
  // during this phase. The LED indicates this state is running.
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

  while (1) // Loop forever, constantly running the benchmark
  {
    // Run the most computationally intensive benchmark (fixed-point for F0)
    checksum_fixed_point = calculate_mandelbrot_fixed_point(IMAGE_SIZE, IMAGE_SIZE, MAX_ITER);
    
    // For this practical, we break out after one run to demonstrate the idle state.
    // In a real measurement, this break would be removed.
    break; 
  }
  
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);


  // --- STATE 2: IDLE ---
  // An external power analyzer would now measure the "Idle Current" (I_idle)
  // during this phase. A different LED indicates this state.
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);

  while (1)
  {
    // The CPU is now in a tight loop doing nothing. This represents the
    // baseline power consumption of the active MCU without a heavy workload.
  }

  /* USER CODE END 2 */
}

/**
  * @brief System Clock Configuration
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
// Fixed-point implementation
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