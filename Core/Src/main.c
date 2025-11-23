/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
///* USER CODE BEGIN PD */
// Display resolution defines
#define EPD_WIDTH   250
#define EPD_HEIGHT  122
#define EPD_BUFFER_SIZE 4000

#define	 X_Addr_Start    0x01
#define  X_Addr_End      0x10

#define  Y_Addr_Start_H  0x00
#define  Y_Addr_Start_L  0xF9
#define  Y_Addr_End_H    0x00
#define  Y_Addr_End_L    0x00

// Function Prototypes
void EPD_Delay_ms(uint32_t ms);
void EPD_Read_Busy(void);
void EPD_Write_Command(uint8_t command);
void EPD_Write_Data(uint8_t data);
void EPD_Init(void); // New Initialization function
void EPD_Test_Sequence(void);
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

COM_InitTypeDef BspCOMInit;
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


// ... (Private macros and variables remain the same) ...
// ... (EPD_Delay_ms, EPD_Read_Busy, EPD_Write_Command, EPD_Write_Data functions remain the same) ...

/**
  * @brief  Initializes the EPD driver with the required sequence from the datasheet.
  *         This function follows the flow described in section 10.2 of the specification.
  * @retval None
  */
void EPD_Init(void)
{
    // --- 1. HARDWARE RESET SEQUENCE ---
	EPD_Delay_ms(10); // delay >200us
    HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET); // RES# low
    EPD_Delay_ms(1); // delay >200us
    HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);   // RES# high
    EPD_Delay_ms(1); // delay >200us
    EPD_Read_Busy(); // Wait for busy low

    // --- 2. SOFTWARE RESET ---
    EPD_Write_Command(0x12); // SWRESET command
    EPD_Read_Busy();         // Wait for busy low

    // --- 3. DRIVER OUTPUT CONTROL (Command 0x01) ---
    // Sets the panel resolution: (122-1) for Gate, (250-1) for Source
    // The panel resolution is 122(H)x250(V). This is where the original code was likely incorrect.
    EPD_Write_Command(0x01);
    EPD_Write_Data(Y_Addr_Start_H); // 121 lines (0-121)
    EPD_Write_Data(Y_Addr_Start_L); // Low byte
    EPD_Write_Data(0x00);

    // --- 4. DATA ENTRY MODE SETTING (Command 0x11) ---
    // Sets the address counter auto-increment/decrement direction. Default is 0x01, but 0x03 is common for landscape mode
    EPD_Write_Command(0x11);
    EPD_Write_Data(0x03); // Y increment, X increment

    // --- 5. SET RAM X ADDRESS START/END POSITION (Command 0x44) ---
    // Starts at 0, ends at 250/8 - 1 = 31 (0x1F) bytes
    EPD_Write_Command(0x44);
    EPD_Write_Data(X_Addr_Start);
    EPD_Write_Data(X_Addr_End); // 31

    // --- 6. SET RAM Y ADDRESS START/END POSITION (Command 0x45) ---
    // Starts at 0, ends at 122-1 = 121 (0x79)
    EPD_Write_Command(0x45);
    EPD_Write_Data(Y_Addr_Start_L);
    EPD_Write_Data(Y_Addr_Start_H); // Start Y
    EPD_Write_Data(Y_Addr_End_L); // 121 (0x79)
    EPD_Write_Data(Y_Addr_End_H); // End Y high byte


    EPD_Write_Command(0x4E);        //Set RAM X address counter
    EPD_Write_Data(X_Addr_Start);


    EPD_Write_Command(0x4F);        //Set RAM Y address counter
    EPD_Write_Data(Y_Addr_Start_L);
    EPD_Write_Data(Y_Addr_Start_H);


    // --- 7. BORDER WAVEFORM CONTROL (Command 0x3C) ---
    // Typically set to Hi-Z to prevent artifacts at the edges
    EPD_Write_Command(0x3C);
    EPD_Write_Data(0x05); // C0h sets VBD as HIZ

    EPD_Write_Command(0x21);
    EPD_Write_Data(0x00);
    EPD_Write_Data(0x00);

//    // --- 8. SET RAM X/Y COUNTERS TO START POSITION (Command 0x4E/0x4F) ---
//    // Set the address counter to the start of the RAM
    EPD_Write_Command(0x4E); // Set RAM X address counter to 0
    EPD_Write_Data(0x00);
    EPD_Write_Command(0x4F); // Set RAM Y address counter to 0
    EPD_Write_Data(0x00);
    EPD_Write_Data(0x00);

    EPD_Read_Busy(); // Wait for busy low

    // Note: The datasheet 10.2 reference is minimal. For full compatibility, commands
    // 0x0C (Booster Soft Start) might be needed, though the default 0xFF data for 0x22 should load LUTs from OTP.
}

/**
  * @brief  Provides a delay in milliseconds.
  * @param  ms: delay time in ms.
  * @retval None
  */
void EPD_Delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

/**
  * @brief  Waits for the EPD busy signal to go low.
  * @retval None
  */
void EPD_Read_Busy(void)
{
    // The EPD outputs HIGH when busy, MCU waits for LOW
    while(HAL_GPIO_ReadPin(BUSY_GPIO_Port, BUSY_Pin) == GPIO_PIN_SET)
    {
        EPD_Delay_ms(1);
    }
}

/**
  * @brief  Sends a command to the EPD via SPI.
  * @param  command: The command byte to send.
  * @retval None
  */
void EPD_Write_Command(uint8_t command)
{
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);      // CS low (enable chip)
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);      // D/C# low (command mode)
    HAL_SPI_Transmit(&hspi1, &command, 1, 100);                           // Transmit the command
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);        // CS high (disable chip select after transfer)
}

/**
  * @brief  Sends data to the EPD via SPI.
  * @param  data: The data byte to send.
  * @retval None
  */
void EPD_Write_Data(uint8_t data)
{
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);      // CS low (enable chip)
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);        // D/C# high (data mode)
    HAL_SPI_Transmit(&hspi1, &data, 1, 100);                              // Transmit the data
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);        // CS high
}

/**
  * @brief  Runs a basic test sequence to display an image on the EPD.
  *         This function follows the flow described in section 10.2.
  * @retval None
  */
void EPD_Test_Sequence(void)
{
    // A simple test buffer: 4000 bytes (all black display)
    // 0x00 for black pixel, 0xFF for white pixel in RAM for BW command 0x24
    uint8_t image_buffer[EPD_BUFFER_SIZE];
    for(int i = 0; i < 4000; i++)
    {
        // Example: Fill with a gray pattern (checkerboard of white/black blocks)
    	if (i < 500) {
    		image_buffer[i] = 0x00;
    	} else {
    		image_buffer[i] = 0x00;
    	}
    }

    // Call the new initialization function
    EPD_Init();

    // --- LOAD IMAGE AND UPDATE ---
    EPD_Write_Command(0x24); // Command 0x24: Load image to Black/White RAM
    for(int i = 0; i < EPD_BUFFER_SIZE; i++)
    {
        EPD_Write_Data(image_buffer[i]); // Send 4000 bytes of image data
    }


    EPD_Write_Command(0x26); // Command 0x24: Load image to Black/White RAM
    for(int i = 0; i < EPD_BUFFER_SIZE; i++)
    {
        EPD_Write_Data(image_buffer[i]); // Send 4000 bytes of image data
    }



    EPD_Write_Command(0x20); // Command 0x20: Master Activation (Display Update)
    EPD_Read_Busy();         // Wait for busy low (update time can be several seconds)

    // --- POWER OFF (Enter Deep Sleep) ---
    EPD_Write_Command(0x10); // Command 0x10: Enter deep sleep mode
    EPD_Write_Data(0x01);    // Data 0x01
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

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Initialize leds */
  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);

  /* Initialize USER push-button, will be used to trigger an interrupt each time it's pressed.*/
  BSP_PB_Init(BUTTON_SW1, BUTTON_MODE_EXTI);
  BSP_PB_Init(BUTTON_SW2, BUTTON_MODE_EXTI);
  BSP_PB_Init(BUTTON_SW3, BUTTON_MODE_EXTI);

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

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  EPD_Test_Sequence();
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK4|RCC_CLOCKTYPE_HCLK2
                              |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SMPS;
  PeriphClkInitStruct.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSI;
  PeriphClkInitStruct.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE0;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN Smps */

  /* USER CODE END Smps */
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, CS_Pin|RST_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : CS_Pin DC_Pin */
  GPIO_InitStruct.Pin = CS_Pin|DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : BUSY_Pin */
  GPIO_InitStruct.Pin = BUSY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUSY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_DM_Pin USB_DP_Pin */
  GPIO_InitStruct.Pin = USB_DM_Pin|USB_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_USB;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : RST_Pin */
  GPIO_InitStruct.Pin = RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RST_GPIO_Port, &GPIO_InitStruct);

  /**/
  __HAL_SYSCFG_FASTMODEPLUS_ENABLE(SYSCFG_FASTMODEPLUS_PB8);

  /**/
  __HAL_SYSCFG_FASTMODEPLUS_ENABLE(SYSCFG_FASTMODEPLUS_PB9);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
