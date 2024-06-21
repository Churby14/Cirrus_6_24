// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: initsystem.c
// Author  : Rick Wright/Charlie Urban
// Revision: 3.2
//
// Description:     Initialize System
//
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Revision Log:
//
//  3.2    CRU     07/26/2022 Integration of released fuel sender software and throttle SW
//
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Design Details and Explanations:
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Implementation Notes:
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//                              Include Files
// ------------------------------------------------------------------------

#include "global.h"

// ------------------------------------------------------------------------
//                               Definitions
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//                  Local Conditional Compilation Switches
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//                          Local Variables
// ------------------------------------------------------------------------
char SWinfobuff[1]; // Store size of SoftWareIdString to put into the SWInfo pointer

// ------------------------------------------------------------------------
//
//  STM32_ADC_ConfigChannel - Configures the internal Analog to Digital converter
//
// Params:    hadc - ADC specific data structure containing the configuration information for the specified ADC
//            sConfig - ADC configuration structure
//
// Returns:   None
//
static void STM32_ADC_ConfigChannel(ADC_HandleTypeDef *hadc, ADC_ChannelConfTypeDef *sConfig) {

    hadc->Instance->CHSELR |= 1u << sConfig->Channel;
    hadc->Instance->SMPR &= ~(ADC_SMPR_SMP);
    hadc->Instance->SMPR |= sConfig->SamplingTime & ADC_SMPR_SMP;
    if (sConfig->Channel == ADC_CHANNEL_VREFINT) {
        ADC->CCR |= (1u << 22);
    }
}

// ------------------------------------------------------------------------
//
//  InitializeSystem - Performs the initialization of all peripherals on the MCU and checks the
//                     FLASH memory for errors
//
// Params:    None
//
// Returns:   None
//
void InitializeSystem(void) {

  ADC_ChannelConfTypeDef    sConfig;
  uint32_t *FirmwarePointer;

    RCC->CR |= 0x00000001u;        // HSI Clock Enable
    RCC->CFGR &= 0x08FFB80Cu;        //
    RCC->CR &= 0xFEF6FFFFu;        // HSE Clock Disable, Clock Security System Disable, PLL Disable
    RCC->CR &= 0xFFFBFFFFu;        // HSE Crystal Osc not bypassed
    RCC->CFGR &= 0xFFC0FFFFu;        //
    RCC->CFGR2 &= 0xFFFFFFF0u;        //
    RCC->CFGR3 &= 0xFFFCFE2Cu;        //
    RCC->CR2 &= 0xFFFFFFFEu;        //
    RCC->CIR = 0x00000000u;        //
  //
  // FLASH Pre-fetch
  //
  FLASH->ACR |= FLASH_ACR_PRFTBE;
  //
  // Enable Clocks
  //
  RCC->AHBENR |= (RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOFEN | RCC_AHBENR_CRCEN | RCC_AHBENR_DMA1EN);
  RCC->APB1ENR |= (RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM14EN | RCC_APB1ENR_I2C1EN | RCC_APB1ENR_I2C2EN | RCC_APB1ENR_USART2EN |
                   RCC_APB1ENR_TIM6EN | RCC_APB1ENR_TIM7EN | RCC_APB1ENR_CANEN) | RCC_APB1ENR_USART4EN;
  RCC->APB2ENR |= (RCC_APB2ENR_TIM1EN | RCC_APB2ENR_TIM15EN | RCC_APB2ENR_TIM16EN | RCC_APB2ENR_TIM17EN | RCC_APB2ENR_USART1EN | RCC_APB2ENR_ADC1EN |
                   RCC_APB2ENR_SYSCFGEN);
  //
  // Configure SysTick Module for 1ms interryupt rate
  //
  SysTick->LOAD = (48000000 / 1000) - 1uL;                         /* set reload register */
  SysTick->VAL = 0uL;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
  SysTick->CTRL |= SYSTICK_CLKSOURCE_HCLK;
  STM32_NVIC_SetPriority(SysTick_IRQn, 0, 0);
  //
  // Enable System Interrupts
  //
  STM32_NVIC_SetPriority(SVC_IRQn, 0, 0);
  STM32_NVIC_SetPriority(PendSV_IRQn, 0, 0);
  //
  // Configure HSE
  //
  RCC->CR |= RCC_CR_HSEBYP;     // HSE in Bypass and On
  RCC->CR |= RCC_CR_HSEON;
  do {
  } while ((RCC->CR & (1u << 17)) == RESET);
  //
  // Configure HSI14
  //
  RCC->CR2 |= RCC_CR2_HSI14DIS;
  RCC->CR2 |= RCC_CR2_HSI14ON;
  do {
  } while ((RCC->CR2 & (1u << 1)) == RESET);
  RCC->CR2 = (RCC->CR2 & RCC_CR2_HSI14TRIM) | (16u << 3u);
  //
  // Configures PLL
  //
  RCC->CR &= ~RCC_CR_PLLON;   // Disable PLL
  do {
  } while ((RCC->CR & (1u << 25u)) != RESET);
  RCC->CFGR2 = (RCC->CFGR2 & ~RCC_CFGR2_PREDIV) | RCC_PREDIV_DIV1;
  RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_PLLMUL | RCC_CFGR_PLLSRC)) | (RCC_PLL_MUL6 | RCC_PLLSOURCE_HSE);

  RCC->CR |= RCC_CR_PLLON;
  do {
  } while ((RCC->CR & (1u << 25u)) == RESET);
  //
  // Configure Flash Latency
  //
  FLASH->ACR = (FLASH->ACR & (~FLASH_ACR_LATENCY)) | 1;
  //
  //
  //
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE) | RCC_SYSCLK_DIV1;
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_SYSCLKSOURCE_PLLCLK;
  do {
  } while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_SYSCLKSOURCE_STATUS_PLLCLK);
  //
  //
  //
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE) | RCC_HCLK_DIV1;
  RCC->CFGR3 = (RCC->CFGR3 & ~RCC_CFGR3_USART1SW) | RCC_USART1CLKSOURCE_SYSCLK;
  RCC->CFGR3 = (RCC->CFGR3 & ~RCC_CFGR3_USART2SW) | RCC_USART2CLKSOURCE_SYSCLK;
  RCC->CFGR3 = (RCC->CFGR3 & ~RCC_CFGR3_I2C1SW) | RCC_I2C1CLKSOURCE_SYSCLK;

  InitializeWatchDog();
  //
  // Setup pointers to Configuration structures stored at end of FLASH.  The flash within the STM32
  // consists of 64 sectors, with each sector being 2048 bytes (64 * 2K = 128KB).
  //
  Fuzzy1Configuration = (stFuzzyConfiguration *)(0x08000000 + (2048 * 61)); // Contains fuzzy logic data for quality check #1
  Fuzzy2Configuration = (stFuzzyConfiguration *)(0x08000000 + (2048 * 61) + 1024); // Contains fuzzy logic data for quality check #2
  FuzzyPage = (stFuzzyPage *)(0x08000000 + (2048 * 61));
  FlashConfiguration = (stFlashConfiguration *)(0x08000000 + (2048 * 62));
  FactoryConfiguration = (stFactoryConfiguration *)(0x08000000 + (2048 * 63));
  FirmwarePointer = (uint32_t *)0x08000000u;
  //
  // Enable TSEN, VBATEN, and VREFEN
  //
  ADC1_COMMON->CCR |= ADC_CCR_TSEN | ADC_CCR_VBATEN | ADC_CCR_VREFEN;
  //
  // Initialize STM32 GPIO
  //
  STM32_GPIO_Init(DIR1_GPIO_Port, DIR1_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0, RESET);        // GPIO DIR1
  STM32_GPIO_Init(DIR2_GPIO_Port, DIR2_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0, RESET);        // GPIO DIR2
  STM32_GPIO_Init(DIR3_GPIO_Port, DIR3_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0, RESET);        // GPIO DIR3
  STM32_GPIO_Init(DIR4_GPIO_Port, DIR4_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0, RESET);        // GPIO DIR4
  STM32_GPIO_Init(DIR5_GPIO_Port, DIR5_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0, RESET);        // GPIO DIR5
  STM32_GPIO_Init(DIR6_GPIO_Port, DIR6_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0, RESET);        // GPIO DIR6
  STM32_GPIO_Init(MUX_SEL_GPIO_Port, MUX_SEL_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0, RESET);         // GPIO MUX_SEL
  STM32_GPIO_Init(EXT_PULLUP_GPIO_Port, EXT_PULLUP_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0, RESET);   // GPIO PULLUP
  STM32_GPIO_Init(EEPROM_WP_GPIO_Port, EEPROM_WP_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0, RESET);     // GPIO EEPROM WP
  STM32_GPIO_Init(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0, RESET);       // GPIO RS485 Enable
  STM32_GPIO_Init(CAN_EN_GPIO_Port, CAN_EN_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0, RESET);           // GPIO CAN Enable
  STM32_GPIO_Init(CAN_EN_GPIO_Port, CAN_EN_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0, RESET);           // GPIO CAN Enable
  //
  //Initialize STEM32 I2C1
  //
  STM32_GPIO_Init(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF1_I2C1, 0);
  STM32_GPIO_Init(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF1_I2C1, 0);

  I2C1->CR1 &= ~I2C_CR1_PE;
  I2C1->TIMINGR = 0x00E01A4Cu & TIMING_CLEAR_MASK;
  I2C1->OAR1 &= ~I2C_OAR1_OA1EN;
  I2C1->OAR1 = I2C_OAR1_OA1EN;
  I2C1->CR2 |= (I2C_CR2_AUTOEND | I2C_CR2_NACK);
  I2C1->OAR2 &= ~I2C_DUALADDRESS_ENABLE;
  I2C1->OAR2 = (I2C_DUALADDRESS_DISABLE | (I2C_OA2_NOMASK << 8));
  I2C1->CR1 = (I2C_GENERALCALL_DISABLE | I2C_NOSTRETCH_DISABLE);
  I2C1->CR1 &= ~I2C_CR1_ANFOFF;
  I2C1->CR1 &= ~I2C_CR1_DNF;          // Digital Filter
  I2C1->CR1 |= I2C_CR1_PE;
  //
  //Initialize STM32 I2C2
  //
  STM32_GPIO_Init(I2C2_SCL_GPIO_Port, I2C2_SCL_Pin, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF1_I2C2, 0);
  STM32_GPIO_Init(I2C2_SDA_GPIO_Port, I2C2_SDA_Pin, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF1_I2C2, 0);

  I2C2->CR1 &= ~I2C_CR1_PE;
  I2C2->TIMINGR = 0x00E01A4Cu & TIMING_CLEAR_MASK;
  I2C2->OAR1 &= ~I2C_OAR1_OA1EN;
  I2C2->OAR1 = I2C_OAR1_OA1EN;
  I2C2->CR2 |= (I2C_CR2_AUTOEND | I2C_CR2_NACK);
  I2C2->OAR2 &= ~I2C_DUALADDRESS_ENABLE;
  I2C2->OAR2 = (I2C_DUALADDRESS_DISABLE | (I2C_OA2_NOMASK << 8));
  I2C2->CR1 = (I2C_GENERALCALL_DISABLE | I2C_NOSTRETCH_DISABLE);
  I2C2->CR1 &= ~I2C_CR1_ANFOFF;
  I2C2->CR1 &= ~I2C_CR1_DNF;          // Digital Filter
  I2C2->CR1 |= I2C_CR1_PE;
  //
  // Initialize DMA
  //
  STM32_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);       // Used by UART1
  STM32_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

  STM32_NVIC_SetPriority(DMA1_Channel4_5_6_7_IRQn, 0, 0);   // Used by UART2
  STM32_NVIC_EnableIRQ(DMA1_Channel4_5_6_7_IRQn);

  hadc.Instance = ADC1;

  // ADC GPIO Configuration
  //    PA1     ------> ADC_IN0
  //    PA5     ------> ADC_IN5
  //    PA6     ------> ADC_IN6
  //    PA7     ------> ADC_IN7
  //
  STM32_GPIO_Init(AD_TEMP_GPIO_Port, AD_TEMP_Pin, GPIO_MODE_ANALOG, GPIO_NOPULL, 0, 0, 0);
  STM32_GPIO_Init(AD_VIN_GPIO_Port, AD_VIN_Pin, GPIO_MODE_ANALOG, GPIO_NOPULL, 0, 0, 0);
  STM32_GPIO_Init(AD_KMA_GPIO_Port, AD_KMA_Pin, GPIO_MODE_ANALOG, GPIO_NOPULL, 0, 0, 0);
  STM32_GPIO_Init(AD_5VDC_GPIO_Port, AD_5VDC_Pin, GPIO_MODE_ANALOG, GPIO_NOPULL, 0, 0, 0);

  hdma_adc.Instance = DMA1_Channel1;
  hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_adc.Init.Mode = DMA_CIRCULAR;
  hdma_adc.Init.Priority = DMA_PRIORITY_LOW;
  STM32_DMA_Init(&hdma_adc);
  hadc.DMA_Handle = &hdma_adc;
  hdma_adc.Parent = &hadc;

  hadc.Instance->CFGR1 = (hadc.Instance->CFGR1 & ~ADC_CFGR1_RES) | ADC_RESOLUTION_12B;
  hadc.Instance->CFGR2 = (hadc.Instance->CFGR2 & ~ADC_CFGR2_CKMODE) | ADC_CLOCK_ASYNC_DIV1;
  hadc.Instance->CFGR1 &= ~(ADC_CFGR1_DISCEN | ADC_CFGR1_AUTOFF | ADC_CFGR1_AUTDLY |
                            ADC_CFGR1_CONT | ADC_CFGR1_OVRMOD | ADC_CFGR1_EXTSEL |
                            ADC_CFGR1_EXTEN | ADC_CFGR1_ALIGN |
                            ADC_CFGR1_SCANDIR | ADC_CFGR1_DMACFG);
  hadc.Instance->CFGR1 |= ((1u << 13u) | (1u << 12u) |
                            ADC_DATAALIGN_RIGHT | (1u << 1u));


  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;

  sConfig.Channel = ADC_CHANNEL_0;                                                        // Temperature Sensor
  STM32_ADC_ConfigChannel(&hadc, &sConfig);
  sConfig.Channel = ADC_CHANNEL_5;                                                        // Power Input
  STM32_ADC_ConfigChannel(&hadc, &sConfig);
  sConfig.Channel = ADC_CHANNEL_6;                                                        // KMA
  STM32_ADC_ConfigChannel(&hadc, &sConfig);
  sConfig.Channel = ADC_CHANNEL_7;                                                        // +5VDC
  STM32_ADC_ConfigChannel(&hadc, &sConfig);
  sConfig.Channel = ADC_CHANNEL_VREFINT;                                                  // Internal Reference
  STM32_ADC_ConfigChannel(&hadc, &sConfig);

  //
  // Initialize CanBus Parameters
  //
  // Setup pointers for ECU information CAN Bus messages
  CanBus.ECUinfo[0] = FactoryConfiguration->ModelNumber;
  CanBus.ECUinfo[1] = FactoryConfiguration->SerialNumber;
  CanBus.ECUinfo[2] = FactoryConfiguration->LocationString;
  CanBus.ECUinfo[3] = FactoryConfiguration->TypeString;
  CanBus.ECUinfo[4] = FactoryConfiguration->ManufacturerString;
  CanBus.ECUinfo[5] = FactoryConfiguration->HardwareIdString;
  CanBus.ECUinfo[6] = FactoryConfiguration->DateString;
  SWinfobuff[0] = sizeof(FactoryConfiguration->SoftwareIdString);
  CanBus.SWinfo[0] = SWinfobuff;
  CanBus.SWinfo[1] = FactoryConfiguration->SoftwareIdString;
  CanBus.Componentinfo[0] = FactoryConfiguration->MakeString;
  CanBus.Componentinfo[1] = FactoryConfiguration->ModelString;
  CanBus.Componentinfo[2] = FactoryConfiguration->SerialString;
  CanBus.Componentinfo[3] = FactoryConfiguration->UnitString;
  CanBus.Caninfo[0] = CanBus.ECUinfo;
  CanBus.Caninfo[1] = CanBus.SWinfo;
  CanBus.Caninfo[2] = CanBus.Componentinfo;
  CanBus.InfoSize[0] = sizeof(FactoryConfiguration->ModelNumber);
  CanBus.InfoSize[1] = sizeof(FactoryConfiguration->SerialNumber);
  CanBus.InfoSize[2] = sizeof(FactoryConfiguration->LocationString);
  CanBus.InfoSize[3] = sizeof(FactoryConfiguration->TypeString);
  CanBus.InfoSize[4] = sizeof(FactoryConfiguration->ManufacturerString);
  CanBus.InfoSize[5] = sizeof(FactoryConfiguration->HardwareIdString);
  CanBus.InfoSize[6] = sizeof(FactoryConfiguration->DateString);
  CanBus.InfoSize[7] = sizeof(CanBus.SWinfo[0]);
  CanBus.InfoSize[8] = sizeof(FactoryConfiguration->SoftwareIdString);
  CanBus.InfoSize[9] = sizeof(FactoryConfiguration->MakeString);
  CanBus.InfoSize[10] = sizeof(FactoryConfiguration->ModelString);
  CanBus.InfoSize[11] = sizeof(FactoryConfiguration->SerialString);
  CanBus.InfoSize[12] = sizeof(FactoryConfiguration->UnitString);
  //
  // Verify Configuration PDI image has the correct CRC16
  //
  CalculatedCrc.Firmware = CalcCrc16Array((uint8_t *)FirmwarePointer, (60 * 2048));
  CalculatedCrc.FlashConfiguration = CalcCrc16Array((uint8_t *)FlashConfiguration, 2046);
  CalculatedCrc.FuzzyConfiguration = CalcCrc16Array((uint8_t *)FuzzyPage, 2046);
  CalculatedCrc.FactoryConfiguration = CalcCrc16Array((uint8_t *)FactoryConfiguration, 2046);

  bFlashConfigurationValid = (FlashConfiguration->Crc16 == CalculatedCrc.FlashConfiguration) & 1;
  bFactoryConfigurationValid =  (FactoryConfiguration->Crc16 == CalculatedCrc.FactoryConfiguration) & 1;
  bFuzzyPageValid = (FuzzyPage->Fuzzy2.Crc16 == CalculatedCrc.FuzzyConfiguration) & 1;
  bFirmwareValid =  (FactoryConfiguration->FirmwareCrc16 == CalculatedCrc.Firmware) & 1;

  //
  // If on startup something is wrong, load in default settings, not for FLASH - this comes
  // after initialization
  //
  ProcessDefaultProfile();

  //
  // Initialize CAN
  //
  if (((bFactoryConfigurationValid != 0) && (FactoryConfiguration->CanDriverPresent != 0))) {
    MX_CAN_Init();
  }
  //
  // Initialize UART1
  //
  MX_USART1_UART_Init();

  SysVars.Version = FIRMWARE_VERSION;

  InitializeDigitalIO(DIG_MODE_POWERUP); // Configure DIG_IO1 as SERIAL_INOUT, all other DIG I/O as Digital Inputs (disabled)
  InitializeExecutionTimers();
  InitializeEventLog();
  InitializeAccel();
  InitializePWM();
  STM32_Delay(50);
  InitializeMcp3421();
  //
  // Set Throttle Data Structure initial values
  //
  SysVars.Quality1 = 0;
  SysVars.Quality2 = 0;
  SysVars.ErrorStatus = FALSE;
  ForeignSysVars.ErrorStatus = FALSE;
  ThrottleInfo.Moved = 0;
  ThrottleInfo.Moving = 0;
  //
  // Get Sensor Unique ID
  //
  uint8_t* IdPtr = (uint8_t*)(UNIQUE_ID_BASE);
  UniqueId.XYCoordniates = IdPtr[0];
  UniqueId.XYCoordniates += IdPtr[1] << 8;
  UniqueId.XYCoordniates += IdPtr[2] << 16;
  UniqueId.XYCoordniates += IdPtr[3] << 24;

  UniqueId.WaferId = IdPtr[4];

  UniqueId.Lot1 = IdPtr[5];

  UniqueId.Lot2 = IdPtr[6];
  UniqueId.Lot2 += IdPtr[7];

  UniqueId.Lot3 = IdPtr[8];
  UniqueId.Lot3 += IdPtr[9] << 8;
  UniqueId.Lot3 += IdPtr[10] << 16;
  UniqueId.Lot3 += IdPtr[11] << 24;


  //
  //
  //
  if (FlashConfiguration->SensorMode == 0) {
    RS485.RSControl = 1;
    SysVars.SensorLetter = 0;
  }
  else {
    RS485.RSControl = 0;
    SysVars.SensorLetter = 1;
  }
  SysVars.SensorLetter = FlashConfiguration->SensorMode;

  SysVars.bStartupComplete = 0; // Startup flag must be 0 at the beginning


    //
    // When new firmware is uploaded, reflash.c is running from Page 55.  As the new firmware is being
    // programmed into FLASH, at some point, page 55 will needs to be programmed.  But, we can't
    // program page while executing from it.  So, reflash.c will actually put the Page 55 code into
    // Page 56.
    // Then, on power-up we check if there is code in Page 56.  If there is, the code is moved back to
    // Page 55 and then Page 56 is erased by programming it with data from PAge 57 (which is always erased).
    //
    uint8_t   bBlank = 1;
    uint32_t *Ptr = (uint32_t *)(0x08000000u + (2048 * 56));
    for (uint32_t i = 0; i < 512; i++) {
        if (Ptr[i] != 0xFFFFFFFFU) {
            bBlank = 0;
            break;
        }
    }
    if (bBlank == 0) {
        ReflashTuningParameters(55, (uint16_t *)(0x08000000u + (2048 * 56)));        // Copy Page 56 to Page 55
        ReflashTuningParameters(56, (uint16_t *)(0x08000000u + (2048 * 57)));        // Erase Page 56 by copying page 57 to 56
        LogEvent(EVENT_MESSAGE_ID_100, 0, 0);
    }
    LogEvent(EVENT_MESSAGE_ID_1, 0, 0);
}
