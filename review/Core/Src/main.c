/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

// Глобальные переменные
#include "var.h"
#include <math.h>
// Протокол обмена USART3
#include "usart_protocol.h"
#include "stm32l4xx_hal_flash.h"
#include "assistfuncs.h"

#include <stdlib.h>
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
//ARM: Warning L6989W
//http://www.keil.com/support/docs/3853.htm
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void InitBufferDAC1(float amplitude, float phase);
void InitBufferDAC2(float amplitude, float phase);
void InitTableData(void);

void FLASH_SaveOptionData(void);
void FLASH_Unlock(void);
void FLASH_Lock(void);
//void FLASH_PageErase(uint32_t Page, uint32_t Banks);
//void FLASH_Program_Fast(uint32_t Address, uint32_t DataAddress);
void FLASH_ErasePage(void);
void FLASH_WriteWords(uint32_t* data, unsigned int address);
void FLASH_LoadOptionData(void);
void FLASH_Fuse(int state);

static inline void UpdateAD7794upr(void);
static inline void UpdateAD7794mdl(void);

static inline void ProgressPID_RF_ATT(void);
static inline void CompensationTemp_FREQ(void);
static inline void CompensationCrntCell_FREQ(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int dbg1 = 0;
int dbg2 = 0;
int dbg3 = 0;
int dbg4 = 0;
int dbg5 = 0;
int dbg6 = 0;

static inline void setting_initvals(int item_pid);
static inline void insertion_pid_feature(float* delta, char place, int item_pid);
static inline void consid_feature_lims(int* low_lim, int* upper_lim, int item_pid);
void ProgressPID_BASE(int item_pid);

int null = 0;
bool null_flag = false;

typedef enum {INT, FLOAT} type_value;

typedef union {
	int *ip;
	float *fp;
} bridge;

typedef struct {
	bridge bridge_val;
	type_value type;
} multi_type;

struct pid_param primal_set[11] = {	
		// см. структуру pid_param
		// kp, ki, kd, range_max, range_min, value_p, value_i, value_d, value_s, present_value, past_value, flag_run
		//
		{0.07f, 0.10f, 0.0f, 65535.0f, -65535.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false},				//item_pid = 0	// ПИД регулятор для удержания пика поглощения с помощью тока
		{-0.72f, -1.272f, -0.05f, 65535.0f, -65535.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false},	//item_pid = 1	// ПИД регулятор для удержания частоты
		{120.0f, 25.0f, 10.0f, 65535.0f, -65535.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false},			//item_pid = 2	// ПИД регулятор для удержания температуры лазера
		{17000.0f, 20.0f, 0.0f, 65535.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false},					//item_pid = 3	// ПИД регулятор для удержания температуры ячейки
		{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false},									//item_pid = 4	// ПИД регулятор для удержания температуры хвостика ячейки
		{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false},									//item_pid = 5	// ПИД регулятор для удержания мощности СВЧ
		{1.1f, 0.07f, 0.3f, 65535.0f, -65535.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false},				//item_pid = 6	// ПИД регулятор для удержания пика поглощения с помощью температуры лазера
		{0.55f, 1.32f, 0.0f, 65535.0f, -65535.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false},				//item_pid = 7	// ПИД регулятор для удержания оптичской мощности током лазера
		{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false},									//item_pid = 8	// Компенсация температурного дрейфа
		{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false},									//item_pid = 9	// Компенсация тока ячейки
		{0.0007f, 0.0018f, 0.0f, 65535.0f, -65535.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false}		//item_pid = 10 // ПИД регулятор для управления мощностью нагрева лазера
};

const int pid_countdown = 0b11011101110;		//здесь содержатся знаки при добавках к величинам 0 - "-", 1 - "+"
const float pid_factors[11] = {1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -0.0625f};

multi_type basic_values[11] = {{.bridge_val.fp = &discr_wave.resume_dw, .type = FLOAT}, {.bridge_val.fp = &ADC_FREQ, .type = FLOAT},
		{.bridge_val.fp = &resultAvrTempVCSEL, .type = FLOAT}, {.bridge_val.fp = &resultAvrTempCELL_HTR, .type = FLOAT}, {.bridge_val.ip = &null, .type = INT},
		{.bridge_val.ip = &null, .type = INT}, {.bridge_val.fp = &discr_wave.resume_dw, .type = FLOAT}, {.bridge_val.fp = &resultAverage_ADC_OPTICS_PWR, .type = FLOAT},
		{.bridge_val.ip = &null, .type = INT}, {.bridge_val.ip = &null, .type = INT}, {.bridge_val.ip = &value_VCSEL_DC, .type = INT}
};

int start_bvs[11] = {0};

multi_type changeable_values[11] = {{.bridge_val.ip = &value_VCSEL_DC, .type = INT}, {.bridge_val.ip = &value_VCO_FINE, .type = INT}, {.bridge_val.ip = &value_TEC_CTRL, .type = INT},
		{.bridge_val.ip = &value_CELL_HTR, .type = INT}, {.bridge_val.ip = &null, .type = INT}, {.bridge_val.ip = &null, .type = INT},
		{.bridge_val.ip = &value_TEC_CTRL, .type = INT}, {.bridge_val.ip = &value_VCSEL_DC, .type = INT}, {.bridge_val.ip = &null, .type = INT},
		{.bridge_val.ip = &null, .type = INT}, {.bridge_val.ip = &value_TEC_CTRL, .type = INT}
};
int start_cvs[11] = {0};

volatile bool *assertion[11] = {&flag_VCSEL_DC, &flag_VCO_FINE, &flag_TEC_CTRL, &flag_CELL_HTR, &null_flag, &null_flag, &flag_TEC_CTRL,
		&flag_VCSEL_DC, &null_flag, &null_flag, &flag_TEC_CTRL};
/* 
	 Все периоды и частоты найдены с помощью программы m_this\assistprogs\sn\sn.exe
	 Первая строка - периоды для таймера TIM2, при которых частоты модуляции тока и частоты, а также период TIM15 будут целыми
	 Вторая строка - периоды для таймера TIM15
	 Третья строка - частоты модуляции чатоты
	 Четвертая строка - частоты модуляции тока
		Периоды и прочее рассчитаны по формуле : PC_TIM15 = f_DAC1_0 * PC_TIM2 / f_ADC2_0, где период PC_TIM15 должен быть целым, 
		PC_TIM2 - целый, и частоты модуляции тоже целые.
*/
struct discrete_set ds1 = {0,0,0,0,0,0};
	// my test
#ifndef __caddr_t_defined
typedef	char *	caddr_t;
#define __caddr_t_defined
#endif

#define	ENOMEM 12	/* Not enough space */

	// end my test
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
	
	my_N_1++;

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_DAC1_Init();
  MX_TIM1_Init();
  MX_SPI2_Init();
  MX_USART3_UART_Init();
  MX_SPI3_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM15_Init();
  /* USER CODE BEGIN 2 */
	//=======================================================
	// Настройка ядра
	//=======================================================
	// Разрешаем использовать DWT (Подсчет тактов) В РЕЛпЗЕ?
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	// Сбрасываем счетчик
	DWT->CYCCNT = 0;
	// Включаем счетчик
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;	
	// Загружаем настройки если они записаны
	FLASH_LoadOptionData();	

	HAL_Delay(5);
	// Настройка внешних ADC
	AD7794_Init();
	HAL_Delay(5);
	// Настройка внешнего DAC
	AD5668_Init();	
	HAL_Delay(5);
	// Настройка синтезатора
	LMX2486_Init();
	HAL_Delay(5);
	// Заполнили буфер модуляции
	// FREQ
	InitBufferDAC1(amplitudeModulationFREQ, phaseModulationFREQ);
	// CRNT
	InitBufferDAC2(amplitudeModulationCRNT, phaseModulationCRNT);
	// Табличные косинус и синус
	InitTableData();
	
	my_N_1++;

	for(int i = 0; i < 10; i++){
		rawValueDataTempVCSEL[i] = 0;
		rawValueDataTempCELL_HTR[i] = 0;
		rawValueDataTempNTC_AUX1[i] = 0;
		rawValueDataTempNTC_AUX2[i] = 0;
		
		rawDataTempMDL[i] = 0;
		rawDataTempUPR[i] = 0;
		
		valueDataVCO[i] = 0.0f;
		valueDataDOPLER[i] = 0.0f;
	}
	
	idSTMPart[0] = ((uint32_t*)UID_BASE)[0];
	idSTMPart[1] = ((uint32_t*)UID_BASE)[1];
	idSTMPart[2] = ((uint32_t*)UID_BASE)[2];

	stateFuse = READ_BIT(FLASH->OPTR, FLASH_OPTR_RDP);
	
	my_N_1++;
	
	// Блокировка FLASH по умолчанию
	if (stateFuse == 0xAA){
		//FLASH_Fuse(1);
	}
	
	itemModeWork = option_start_mode;
		
	fixValue_VCO_FINE = value_VCO_FINE;
	// Устанавливаем магинтное поле согласно рабочему
	value_COIL_CRNT = option_COIL_CRNT;
	fixValue_COIL_CRNT = option_COIL_CRNT;
	#warning TEC_MAX
		
	// Сброс микросхемы
	AD5668_SetValue(AD5668_DAC_ALL, 0); 
	// Грубая настройка частоты
	AD5668_SetValue(DAC_VCO_CRS, value_VCO_CRS);
	// Тонкая настройка частоты
	AD5668_SetValue(DAC_VCO_FINE, value_VCO_FINE);
	// Магнитное поле
	AD5668_SetValue(DAC_COIL_CRNT, value_COIL_CRNT);
	// Мощность охлаждение хвостика ячейки
	AD5668_SetValue(DAC_AUX_TEC, value_AUX_TEC);
	// Ток лазера
	AD5668_SetValue(DAC_VCSEL_DC, value_VCSEL_DC);
	// Мощность нагрева лазера
	AD5668_SetValue(DAC_TEC_CTRL, value_TEC_CTRL);
	// Мощность нагрева ячейки
	AD5668_SetValue(DAC_CELL_HTR, value_CELL_HTR); 
	// Ослабленние аттенюатора
	AD5668_SetValue(DAC_RF_ATT, value_RF_ATT);
	
	my_N_1++;

	// пзменяем полярность нагревателя
	switch(option_polarity_TEC){
		case 1:	// Выключили
			HAL_GPIO_WritePin(TEC_CTRL_0_GPIO_Port, TEC_CTRL_0_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(TEC_CTRL_1_GPIO_Port, TEC_CTRL_1_Pin, GPIO_PIN_RESET);
			// +
			HAL_GPIO_WritePin(TEC_CTRL_0_GPIO_Port, TEC_CTRL_0_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(TEC_CTRL_1_GPIO_Port, TEC_CTRL_1_Pin, GPIO_PIN_SET);
			break;
		case 0:
			// Выключили					
			HAL_GPIO_WritePin(TEC_CTRL_0_GPIO_Port, TEC_CTRL_0_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(TEC_CTRL_1_GPIO_Port, TEC_CTRL_1_Pin, GPIO_PIN_RESET);
			break;
		case -1:
			// Выключили					
			HAL_GPIO_WritePin(TEC_CTRL_0_GPIO_Port, TEC_CTRL_0_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(TEC_CTRL_1_GPIO_Port, TEC_CTRL_1_Pin, GPIO_PIN_RESET);
			// -
			HAL_GPIO_WritePin(TEC_CTRL_0_GPIO_Port, TEC_CTRL_0_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(TEC_CTRL_1_GPIO_Port, TEC_CTRL_1_Pin, GPIO_PIN_RESET);	
			break;
	}
	
	my_N_1++;
	
	//=======================================================
	// Настраиваем программное прерывание
	//=======================================================
	// Программное прерывание EXTI0
	EXTI->IMR1 |= EXTI_IMR1_IM0;
	EXTI->RTSR1 = EXTI_RTSR1_RT0;
	// Сбрасываем флаги
	EXTI->PR1 |= EXTI_PR1_PIF0;
	HAL_NVIC_SetPriority(EXTI0_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	
	//=======================================================
	// Разрешаем прерывание UART3
	//=======================================================
	NVIC_EnableIRQ (USART3_IRQn);   
	USART3->CR1 |= USART_CR1_RXNEIE; // Прерывание по приему данных	
	
	//=======================================================
	// Настройка АЦП
	//=======================================================
	// Производим калибровку АЦП 1
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	uint32_t calibrationFactorADC1  = HAL_ADCEx_Calibration_GetValue(&hadc1, ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_SetValue(&hadc1, ADC_SINGLE_ENDED, calibrationFactorADC1);
	// Производим калибровку АЦП 2
	HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
	uint32_t calibrationFactorADC2  = HAL_ADCEx_Calibration_GetValue(&hadc2, ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_SetValue(&hadc2, ADC_SINGLE_ENDED, calibrationFactorADC2);
	// Производим калибровку АЦП 3
	HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);
	uint32_t calibrationFactorADC3  = HAL_ADCEx_Calibration_GetValue(&hadc3, ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_SetValue(&hadc3, ADC_SINGLE_ENDED, calibrationFactorADC3);
	
	my_N_1++;


	// Включаем ADC
	ADC_Enable(&hadc1);
	ADC_Enable(&hadc2);
	ADC_Enable(&hadc3);
	// Настраиваем буферы DMA для ADC
	// ADC1 
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)DMA_ADC1_Data, 2*ADC_ARRAY_DMA_ADC1_HALF_SIZE);
	// ADC2
	HAL_ADC_Start_DMA(&hadc2, (uint32_t*)DMA_ADC2_Data, 2*ADC_ARRAY_DMA_ADC2_HALF_SIZE);
	// ADC3
	HAL_ADC_Start_DMA(&hadc3, (uint32_t*)DMA_ADC3_Data, 2*ADC_ARRAY_DMA_ADC3_HALF_SIZE);
		
	// Включаем DAC
	HAL_DAC_Start(&hdac1,DAC_CHANNEL_1); 
	HAL_DAC_Start(&hdac1,DAC_CHANNEL_2); 
	// Настраиваем буферы DMA для DAC
	// OUT1 10000 Hz DMA
	HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, (uint32_t*)DMA_DAC1_Data, 
										2*DAC_ARRAY_DMA_OUT1_HALF_SIZE, DAC_ALIGN_12B_R);
	// OUT2 16000 Hz DMA
	HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_2, (uint32_t*)DMA_DAC2_Data, 
										DAC_ARRAY_DMA_OUT2_SIZE, DAC_ALIGN_12B_R);
	// Выключаем прерывание
	///DAC_IT_DMAUDR1;
	HAL_NVIC_DisableIRQ(DMA2_Channel5_IRQn);
	HAL_NVIC_DisableIRQ(DMA2_Channel4_IRQn);
	// Выключаем прерывание системного таймера
	SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk);
	HAL_NVIC_DisableIRQ(SysTick_IRQn);
	
	// TIM15 (UPDATE 80KHz) Обновление ADC1/ADC2/ADC3
	TIM15->CNT = 0;
	// TIM3 (UPDATE 16KHz) Модуляция мощности СВЧ
	TIM3->CNT = 0;
	// TIM2 (UPDATE 200KHz) Обновление DAC1/DAC2
	TIM2->CNT = 0;
	// TIM1 (UPDATE 5MHz)
	TIM1->CNT = 0;
		
	// Настраиваем выход синхронизации DAC 250 Hz (!игнорируется!)
	TIM_CCxChannelCmd(TIM3, TIM_CHANNEL_4, TIM_CCx_ENABLE);
	TIM3->BDTR |= (TIM_BDTR_MOE);
	
	// Синхронный запуск всех таймеров
	TIM1->CR1|=(TIM_CR1_CEN);
	// Запускаем выход 5 MHz
	TIM_CCxChannelCmd(TIM1, TIM_CHANNEL_2, TIM_CCx_ENABLE);
	TIM1->BDTR|=(TIM_BDTR_MOE);
	
	// Выключение контроллера
	//HAL_PWR_EnterSTANDBYMode();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	/*
#pragma asm
	int* p_my_int;
	p_my_int = (int*)malloc(4);
	*/
  while (1){
		my_N_3++;
		
		permission |= (1 << 10);
	  // Разбор команд
		byteUSART = USART_GetByte();
		if ( byteUSART != -1){
			BufferMsg[lengthBufferCmd] = (uint8_t)byteUSART;
			lengthBufferCmd++;
			if (lengthBufferCmd == 256){
				lengthBufferCmd = 0;
			}
			//===================================================
			// Обрабатываем сообщение и удаляем его из очереди
			//===================================================
			cmdID = GetCmdID(BufferMsg, &lengthBufferCmd);
			if (cmdID != 0){
				// Реагируем на команду
				CamputeCmd(cmdID);			
			}
		}
		else{
			if (option_order_sleep == 1){
				// Уходим в сон
				HAL_PWR_EnterSTANDBYMode();
				//HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
			}
		}			
		//=======================================================
		// Определение температуры по терморезисторам
		//=======================================================
		// Забираем данные с частотой 100 Hz
		if ( flagUpdateAD7794 == true){
			flagUpdateAD7794 = false;
			UpdateAD7794mdl();
			UpdateAD7794upr();
		}
		// Вычисляем воздействие PID-регуляторов
		// Захват температуры лазера
		ProgressPID_BASE(2);
		// Захват температуры ячейки
		ProgressPID_BASE(3);
		// Захват оптической мощностм
		ProgressPID_BASE(7);
		// Компенсируем смещение частоты от изменение окружающей температуры
		CompensationTemp_FREQ();
		// Компенсация тока нагревателя
		CompensationCrntCell_FREQ();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_SYSCLK;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */



// Модуляция тока
void InitBufferDAC2(float amplitude, float phase){
	double valueD = 0.0f;
	int valueI = 0;
	
	// MOD_CRNT
	for( int i = 0; i < DAC_ARRAY_DMA_OUT2_SIZE; i++){
		valueD = 2.0f*3.141592f*((float)i)/(12.5f) + phase;
		valueD = 2048.0 + amplitude*sin(valueD);
		valueI = (int)valueD;
		if (valueI < 0){
			valueI = 0;
		}
		if (valueI > 4095){
			valueI = 4095;
		}
		DMA_DAC2_Data[i] = (uint16_t)valueI;

	}
}

// Модуляция частоты
void InitBufferDAC1(float amplitude, float phase){
	double valueD = 0.0f;
	int valueI = 0;
	// MOD_FREQ
	for( int i = 0; i < 2*DAC_ARRAY_DMA_OUT1_HALF_SIZE; i++){
		valueD = 3.141592f*((float)i)/((float)DAC_ARRAY_DMA_OUT1_HALF_SIZE) + phase;
		valueD = 2048.0 + amplitude*(sin(valueD)- 0.0*cos(valueD*2));
		valueI = (int)valueD;
		if (valueI < 0){
			valueI = 0;
		}
		if (valueI > 4095){
			valueI = 4095;
		}
		switch(itemTypeModeFREQ){
			// Синус
			case 0:				
				DMA_DAC1_Data[i] = (uint16_t)valueI;
				break;
			// Меандр
			case 1:						
				if ( i < DAC_ARRAY_DMA_OUT1_HALF_SIZE){				
					DMA_DAC1_Data[i] = (uint16_t)(2048-amplitude); 
				} else {
					DMA_DAC1_Data[i] = (uint16_t)(2048+amplitude); 
				}	
				break;
			// Пила
			case 2:						
				DMA_DAC1_Data[i] = (uint16_t)((2048 - amplitude) + (float)i*amplitude/(float)DAC_ARRAY_DMA_OUT1_HALF_SIZE);
				break;
		}
	}
}


void InitTableData(void){
	for( int i = 0; i < LENGTH_PERIOD_FREQ; i++){
		tableFREQ_Sin[i] = -sin(2.0f*3.141592f*((float)i)/((float)LENGTH_PERIOD_FREQ)+ phaseTableFREQ + phaseModulationFREQ);
		tableFREQ_Cos[i] = cos(2.0f*3.141592f*((float)i)/((float)LENGTH_PERIOD_FREQ) + phaseTableFREQ + phaseModulationFREQ);
	}
	
	for( int i = 0; i < LENGTH_PERIOD_CRNT; i++){
		tableCRNT_Sin[i] = -sin(2.0f*3.141592f*((float)i)/((float)LENGTH_PERIOD_CRNT) + phaseTableCRNT + phaseModulationCRNT);
		tableCRNT_Cos[i] = cos(2.0f*3.141592f*((float)i)/((float)LENGTH_PERIOD_CRNT) + phaseTableCRNT + phaseModulationCRNT);
	}
	
	for( int i = 0; i < LENGTH_PERIOD_RF; i++){
		tableRF_Sin[i] = -sin(2.0f*3.141592f*((float)i)/((float)LENGTH_PERIOD_RF) + phaseTableRF);
		tableRF_Cos[i] = cos(2.0f*3.141592f*((float)i)/((float)LENGTH_PERIOD_RF) + phaseTableRF);
	}
	
	for( int i = 0; i < PERIOD_20KHZ; i++){
		tableFREQ20KHZ_Sin[i] = -sin(2.0f*3.141592f*((float)i)/((float)PERIOD_20KHZ)+ phaseTableFREQ20KHZ + phaseModulationFREQ20KHZ);
		tableFREQ20KHZ_Cos[i] = cos(2.0f*3.141592f*((float)i)/((float)PERIOD_20KHZ) + phaseTableFREQ20KHZ + phaseModulationFREQ20KHZ);
	}
}

void FLASH_Unlock(){
	FLASH->KEYR = PDKEY1;
	FLASH->KEYR = PDKEY2;
}
void FLASH_Lock(){
	FLASH->CR |= FLASH_CR_LOCK;
}

void FLASH_OB_Unlock(){
	WRITE_REG(FLASH->OPTKEYR, FLASH_OPTKEY1);
	WRITE_REG(FLASH->OPTKEYR, FLASH_OPTKEY2);
}

void FLASH_OB_Lock(){
	SET_BIT(FLASH->CR, FLASH_CR_OPTLOCK);
}


void FLASH_SaveOptionData(void){
	// Не оптимальная упаковка данных по размеру данных (страница 2Кб)
	// Если есть данные на сохранение
	HAL_NVIC_DisableIRQ(EXTI0_IRQn);
	if (option_status == 1){		
		option_status = VERSION_CPT_FLASH;
		FLASH_Unlock();
		// Очистили страницу и пдоготовили её для записи
		FLASH_ErasePage();
		int index = 0;
		//0 Флаг точки сохранения
		FLASH_WriteWords((uint32_t*)&option_status, FLASH_PAGE_WORK+index);
		index += 8;
		//1 Записали значение рабочего тока
		FLASH_WriteWords((uint32_t*)&option_VCSEL_DC, FLASH_PAGE_WORK+index);
		index += 8;
		//2 Записали значение рабочей мощности
		FLASH_WriteWords((uint32_t*)&option_POWER_RF_ATT, FLASH_PAGE_WORK+index);
		index += 8;
		//3 Записали значеие рабочего тока пельтье	хвостика ячейки	
		FLASH_WriteWords((uint32_t*)&option_AUX_TEC, FLASH_PAGE_WORK+index);
		index += 8;
		//4 Считываем рабочую температура лазера
		FLASH_WriteWords((uint32_t*)&levelTempVCSEL, FLASH_PAGE_WORK+index);
		index += 8;
		//5 Считываем рабочую температура ячейки
		FLASH_WriteWords((uint32_t*)&levelTempCELL, FLASH_PAGE_WORK+index);
		index += 8;
		// Записываем значение регистров синтезатора
		//6 РЕГпСТР N
		FLASH_WriteWords((uint32_t*)&LMX_ValueN, FLASH_PAGE_WORK+index);
		index += 8;
		//7 РЕГпСТР FN
		FLASH_WriteWords((uint32_t*)&LMX_ValueFN, FLASH_PAGE_WORK+index);
		index += 8;
		//8 РЕГпСТР FD
		FLASH_WriteWords((uint32_t*)&LMX_ValueFD, FLASH_PAGE_WORK+index);
		index += 8;
		//9 Значение смещение частоты относительно номинального в кодах
		FLASH_WriteWords((uint32_t*)&LMX_ValueINT, FLASH_PAGE_WORK+index);
		index += 8;
		//10 Смещение частоты СВЧ относительного номинального перехода D1 в ГЦ
		FLASH_WriteWords((uint32_t*)&LMX_ShiftFreq, FLASH_PAGE_WORK+index);
		index += 8;
		//11 Записываем фазу синхронного детектора модуляции тока
		FLASH_WriteWords((uint32_t*)&phaseTableCRNT, FLASH_PAGE_WORK+index);
		index += 8;
		//12 Записываем фазу синхронного детектора модуляции частоты
		FLASH_WriteWords((uint32_t*)&phaseTableFREQ, FLASH_PAGE_WORK+index);
		index += 8;
		//13 Записываем амплитуду модуляции тока
		FLASH_WriteWords((uint32_t*)&amplitudeModulationCRNT, FLASH_PAGE_WORK+index);
		index += 8;
		//14 Записываем амплитуду модуляции частоты
		FLASH_WriteWords((uint32_t*)&amplitudeModulationFREQ, FLASH_PAGE_WORK+index);
		index += 8;
		//15 Записываем смещение синхронного детектора доплера
		FLASH_WriteWords((uint32_t*)&shiftDetector_DOPLER, FLASH_PAGE_WORK+index);
		index += 8;
		//16 Записываем смещение синхронного детектора частоты
		FLASH_WriteWords((uint32_t*)&shiftDetector_FREQ, FLASH_PAGE_WORK+index);
		index += 8;
		// ПпД регулятор частоты
		//17 VCO Kp
		FLASH_WriteWords((uint32_t*)&primal_set[1].factor_kp, FLASH_PAGE_WORK+index);
		index += 8;
		//18 VCO Ki
		FLASH_WriteWords((uint32_t*)&primal_set[1].factor_ki, FLASH_PAGE_WORK+index);
		index += 8;
		//19 VCO Kd
		FLASH_WriteWords((uint32_t*)&primal_set[1].factor_kd, FLASH_PAGE_WORK+index);
		index += 8;
		// ПпД регулятор удержания пика поглощения температурой лазера
		//20 DOPLER TEMP Kp 
		FLASH_WriteWords((uint32_t*)&primal_set[6].factor_kp, FLASH_PAGE_WORK+index);
		index += 8;
		//21 DOPLER TEMP Ki
		FLASH_WriteWords((uint32_t*)&primal_set[6].factor_ki, FLASH_PAGE_WORK+index);
		index += 8;
		//22 DOPLER TEMP Kd
		FLASH_WriteWords((uint32_t*)&primal_set[6].factor_kd, FLASH_PAGE_WORK+index);
		index += 8;
		// ПпД регулятор температуры ячейки
		//23 CELLHTR Kp 
		FLASH_WriteWords((uint32_t*)&primal_set[3].factor_kp, FLASH_PAGE_WORK+index);
		index += 8;
		//24 CELLHTR Ki
		FLASH_WriteWords((uint32_t*)&primal_set[3].factor_ki, FLASH_PAGE_WORK+index);
		index += 8;
		//25 CELLHTR Kd
		FLASH_WriteWords((uint32_t*)&primal_set[3].factor_kd, FLASH_PAGE_WORK+index);
		index += 8;
		// ПпД регулятор удержания температуры лазера
		//26 TECCTRL Kp
		FLASH_WriteWords((uint32_t*)&primal_set[2].factor_kp, FLASH_PAGE_WORK+index);
		index += 8;
		//27 TECCTRL Ki
		FLASH_WriteWords((uint32_t*)&primal_set[2].factor_ki, FLASH_PAGE_WORK+index);
		index += 8;
		//28 TECCTRL Kd
		FLASH_WriteWords((uint32_t*)&primal_set[2].factor_kd, FLASH_PAGE_WORK+index);
		index += 8;
		// ПпД регулятор удержания линии поглощения током лазера
		//29 DOPLER_CRNT Kp
		FLASH_WriteWords((uint32_t*)&primal_set[0].factor_kp, FLASH_PAGE_WORK+index);
		index += 8;
		//30 DOPLER_CRNT Ki
		FLASH_WriteWords((uint32_t*)&primal_set[0].factor_ki, FLASH_PAGE_WORK+index);
		index += 8;
		//31 DOPLER_CRNT Kd
		FLASH_WriteWords((uint32_t*)&primal_set[0].factor_kd, FLASH_PAGE_WORK+index);
		index += 8;
		// ПпД регулятор удержания постоянного сигнала на фотоприёмнике
		//32 OPTICS_PWR Kp
		FLASH_WriteWords((uint32_t*)&primal_set[7].factor_kp, FLASH_PAGE_WORK+index);
		index += 8;
		//33 OPTICS_PWR Ki
		FLASH_WriteWords((uint32_t*)&primal_set[7].factor_ki, FLASH_PAGE_WORK+index);
		index += 8;
		//34 OPTICS_PWR Kd
		FLASH_WriteWords((uint32_t*)&primal_set[7].factor_kd, FLASH_PAGE_WORK+index);
		index += 8;
		//35 Начальный режим работы стандарта
		FLASH_WriteWords((uint32_t*)&option_start_mode, FLASH_PAGE_WORK+index);
		index += 8;
		//36 Права при работе с RS-232
		FLASH_WriteWords((uint32_t*)&statusAccessRS232, FLASH_PAGE_WORK+index);
		index += 8;
		//37 Значение мощности СВЧ для поиска рабочего пика поглощения
		FLASH_WriteWords((uint32_t*)&option_FIND_DC_PWR_RF_ATT, FLASH_PAGE_WORK+index);
		index += 8;	
		//38 Начальное значение тока при поиске пика поглощения
		FLASH_WriteWords((uint32_t*)&VCSEL_ScanLimitMin, FLASH_PAGE_WORK+index);
		index += 8;
		//39 Конечное значение тока при поиске пика поглощения
		FLASH_WriteWords((uint32_t*)&VCSEL_ScanLimitMax, FLASH_PAGE_WORK+index);
		index += 8;
		//40 Шаг поиска пика поглощения
		FLASH_WriteWords((uint32_t*)&VCSEL_ScanStep, FLASH_PAGE_WORK+index);
		index += 8;
		//41 Уровень детектирования пика поглощения
		FLASH_WriteWords((uint32_t*)&VCSEL_levelDetector, FLASH_PAGE_WORK+index);
		index += 8;
		//42 Гистерезис уровня детектирования пика поглощения
		FLASH_WriteWords((uint32_t*)&VCSEL_levelSigma, FLASH_PAGE_WORK+index);
		index += 8;		
		//43 Уровень температуры ячейки при которой разрешен поиск пика поглощения
		FLASH_WriteWords((uint32_t*)&levelStartTempCELL, FLASH_PAGE_WORK+index);
		index += 8;		
		//44 Разрешенный разброс данных температуры
		FLASH_WriteWords((uint32_t*)&sigmaPID_TEC_CTRL, FLASH_PAGE_WORK+index);
		index += 8;		
		//45 Рабочее магнитное поле
		FLASH_WriteWords((uint32_t*)&option_COIL_CRNT, FLASH_PAGE_WORK+index);
		index += 8;		
		// ПпД регулятор для удержания мощности СВЧ
		//46 RF_ATT Kp
		FLASH_WriteWords((uint32_t*)&primal_set[5].factor_kp, FLASH_PAGE_WORK+index);
		index += 8;
		//47 RF_ATT Ki
		FLASH_WriteWords((uint32_t*)&primal_set[5].factor_kp, FLASH_PAGE_WORK+index);
		index += 8;
		//48 RF_ATT Kd
		FLASH_WriteWords((uint32_t*)&primal_set[5].factor_kp, FLASH_PAGE_WORK+index);
		index += 8;
		//49 Номинал эталонного резистора лазера
		FLASH_WriteWords((uint32_t*)&refResistVCSEL, FLASH_PAGE_WORK+index);
		index += 8;
		//50 Номинал эталонного резистора AUX1 
		FLASH_WriteWords((uint32_t*)&refResistAUX1, FLASH_PAGE_WORK+index);
		index += 8;
		//51 Номинал эталонного резистора ячейки
		FLASH_WriteWords((uint32_t*)&refResistCELL, FLASH_PAGE_WORK+index);
		index += 8;
		//52 Номинал эталонного резистора AUX2
		FLASH_WriteWords((uint32_t*)&refResistAUX2, FLASH_PAGE_WORK+index);
		index += 8;		
		// Коэффициенты термокомпенсации
		//53 TEMP 2
		FLASH_WriteWords((uint32_t*)&option_factor_A_TEMP_FREQ, FLASH_PAGE_WORK+index);
		index += 8;
		//54 TEMP 1
		FLASH_WriteWords((uint32_t*)&option_factor_B_TEMP_FREQ, FLASH_PAGE_WORK+index);
		index += 8;
		//55 TEMP CONST
		FLASH_WriteWords((uint32_t*)&option_factor_C_TEMP_FREQ, FLASH_PAGE_WORK+index);
		index += 8;
		// Коэффициент сдвига частоты от магнитного поля
		//56 COIL 2
		FLASH_WriteWords((uint32_t*)&option_factor_A_COIL, FLASH_PAGE_WORK+index);
		index += 8;
		//57 COIL 1
		FLASH_WriteWords((uint32_t*)&option_factor_B_COIL, FLASH_PAGE_WORK+index);
		index += 8;
		//58 COIL CONST
		FLASH_WriteWords((uint32_t*)&option_factor_C_COIL, FLASH_PAGE_WORK+index);
		index += 8;
		//59 Полярность нагревателя
		FLASH_WriteWords((uint32_t*)&option_polarity_TEC, FLASH_PAGE_WORK+index);
		index += 8;		
		//60 Разрешение использовать таблицу
		FLASH_WriteWords((uint32_t*)&isUseTableCompensationFREQ, FLASH_PAGE_WORK+index);
		index += 8;
		//61%156 Таблица данных
		float value1 = 0.0f;
		float value2 = 0.0f;
		int value3 = 0;
		for (int i = 0; i < 32; i++){
			value1 = tableCompensationFREQ[0][i];
			FLASH_WriteWords((uint32_t*)&value1, FLASH_PAGE_WORK+index);
			index += 8;
			value2 = tableCompensationFREQ[1][i];
			FLASH_WriteWords((uint32_t*)&value2, FLASH_PAGE_WORK+index);
			index += 8;
			value3 = isTableCompensationFREQ[i];
			FLASH_WriteWords((uint32_t*)&value3, FLASH_PAGE_WORK+index);
			index += 8;
		}
		// 157
		FLASH_Lock();
		HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	}
}

void FLASH_LoadOptionData(void){
	int index = 0;
	//0
	option_status = (*(__IO int*)(FLASH_PAGE_WORK + index));
	index += 8;
	// Есть сохраненные данные
	if (option_status == VERSION_CPT_FLASH){
		//1 Считали значение рабочего тока
		option_VCSEL_DC = (*(__IO int*)(FLASH_PAGE_WORK + index));
		index += 8;
		//2 Считали значение рабочей мощности
		option_POWER_RF_ATT = (*(__IO int*)(FLASH_PAGE_WORK + index));
		index += 8;
		//3 Считали значение рабочего тока пельтье хвостика ячейки
		option_AUX_TEC = (*(__IO int*)(FLASH_PAGE_WORK + index));
		index += 8;
		//4 Считываем рабочую температура лазера
		levelTempVCSEL = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//5 Считываем рабочую температура ячейки
		levelTempCELL = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		// Считываем значение регистров синтезатора
		//6 РЕГпСТР N
		LMX_ValueN = (*(__IO int*)(FLASH_PAGE_WORK + index));
		index += 8;
		//7 РЕГпСТР FN
		LMX_ValueFN = (*(__IO int*)(FLASH_PAGE_WORK + index));
		index += 8;
		//8 РЕГпСТР FD
		LMX_ValueFD = (*(__IO int*)(FLASH_PAGE_WORK + index));
		index += 8;
		//9 Значение смещение частоты относительно номинального в кодах
		LMX_ValueINT = (*(__IO int*)(FLASH_PAGE_WORK + index));
		index += 8;
		//10 Считываем смещение частоты СВЧ относительного номинального перехода D1 в Гц
		LMX_ShiftFreq = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//11 Считываем фазу синхронного детектора модуляции тока
		phaseTableCRNT = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//12 Считываем фазу синхронного детектора модуляции частоты
		phaseTableFREQ = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//13 Считываем амплитуду модуляции тока
		amplitudeModulationCRNT = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//14 Считываем амплитуду модуляции частоты
		amplitudeModulationFREQ = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//15 Смещение синхронного детектора доплера
		shiftDetector_DOPLER = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//16 Смещение синхронного детектора частоты
		shiftDetector_FREQ = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		// ПпД регулятор частоты
		//17
		primal_set[1].factor_kp = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//18
		primal_set[1].factor_ki = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//19
		primal_set[1].factor_kd = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		// ПпД регулятор удержания пика поглощения температурой лазера
		//20
		primal_set[6].factor_kp = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//21
		primal_set[6].factor_ki = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//22
		primal_set[6].factor_kd = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		// ПпД регулятор температуры ячейки
		//23
		primal_set[3].factor_kp = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//24
		primal_set[3].factor_ki = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//25
		primal_set[3].factor_kd = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		// ПпД регулятор удержания температуры лазера
		//26
		primal_set[2].factor_kp = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//27
		primal_set[2].factor_ki = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//28
		primal_set[2].factor_kd = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		// ПпД регулятор удержания линии поглощения током лазера
		//29
		primal_set[0].factor_kp = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//30
		primal_set[0].factor_ki = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//31
		primal_set[0].factor_kd = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		// ПпД регулятор удержания постоянного сигнала на фотоприёмнике
		//32
		primal_set[7].factor_kp = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//33
		primal_set[7].factor_ki = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//34
		primal_set[7].factor_kd = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//35 Начальный режим работы стандарта
		option_start_mode = (*(__IO int*)(FLASH_PAGE_WORK + index));
		index += 8;
		//36 Права при работе с RS-232
		statusAccessRS232 = (*(__IO int*)(FLASH_PAGE_WORK + index));
		index += 8;
		//37 Значение мощности СВЧ для поиска рабочего пика поглощения
		option_FIND_DC_PWR_RF_ATT = (*(__IO int*)(FLASH_PAGE_WORK + index));
		index += 8;
		//38 Начальное значение тока при поиске пика поглощения
		VCSEL_ScanLimitMin = (*(__IO int*)(FLASH_PAGE_WORK + index));
		index += 8;
		//39 Конечное значение тока при поиске пика поглощения
		VCSEL_ScanLimitMax = (*(__IO int*)(FLASH_PAGE_WORK + index));
		index += 8;
		//40 Шаг поиска пика поглощения
		VCSEL_ScanStep = (*(__IO int*)(FLASH_PAGE_WORK + index));
		index += 8;
		//41 Уровень детектирования пика поглощения
		VCSEL_levelDetector = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//42 Гистерезис уровня детектирования пика поглощения
		VCSEL_levelSigma = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//43 Уровень температуры ячейки при которой разрешен поиск пика поглощения
		levelStartTempCELL = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;		
		//44 Разрешенный разброс данных температуры
		sigmaPID_TEC_CTRL = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;	
		//45 Рабочее магнитное поле
		option_COIL_CRNT = (*(__IO int*)(FLASH_PAGE_WORK + index));
		index += 8;		
		// ПпД регулятор для удержания мощности СВЧ
		//46
		primal_set[5].factor_kp = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//47
		primal_set[5].factor_ki = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//48
		primal_set[5].factor_kd = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//49 Номинал эталонного резистора лазера
		refResistVCSEL = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//50 Номинал эталонного резистора AUX1 
		refResistAUX1 = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//51 Номинал эталонного резистора ячейки
		refResistCELL = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//52 Номинал эталонного резистора AUX2
		refResistAUX2 = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;		
		// Коэффициенты термокомпенсации
		//53 A
		option_factor_A_TEMP_FREQ = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//54 B
		option_factor_B_TEMP_FREQ = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//55 C
		option_factor_C_TEMP_FREQ = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		// Коэффициент сдвига частоты от магнитного поля
		//56 A
		option_factor_A_COIL = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//57 B
		option_factor_B_COIL = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//58 C
		option_factor_C_COIL = (*(__IO float*)(FLASH_PAGE_WORK + index));
		index += 8;
		//59 Полярность нагревателя
		option_polarity_TEC = (*(__IO int*)(FLASH_PAGE_WORK + index));
		index += 8;
		//60 Разрешение использовать таблицу
		isUseTableCompensationFREQ = (*(__IO int*)(FLASH_PAGE_WORK + index));
		index += 8;
		//61%156 Таблица данных
		for (int i = 0; i < 32; i++){
			tableCompensationFREQ[0][i] =(*(__IO float*)(FLASH_PAGE_WORK + index));
			index += 8;
			tableCompensationFREQ[1][i] =(*(__IO float*)(FLASH_PAGE_WORK + index));
			index += 8;
			isTableCompensationFREQ[i] = (*(__IO int*)(FLASH_PAGE_WORK + index));
			index += 8;
		}
		// 157
		
	}
}

// pageAddress - любой адрес, принадлежащий стираемой странице
void FLASH_ErasePage(void) {
	while ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY){};
	if (FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR = FLASH_SR_EOP;
	}
	FLASH->SR = FLASH_SR_PGAERR;
	FLASH->SR = FLASH_SR_PGSERR;
	// Разрешине прерывания и флага FLASH_SR_EOP
	FLASH->CR |= FLASH_CR_EOPIE;
	FLASH->CR &= ~FLASH_CR_BKER;
	FLASH->CR |= FLASH_CR_PER;
	
	MODIFY_REG(FLASH->CR, FLASH_CR_PNB, ((FLASH_PAGE_ITEM & 0xFFU) << FLASH_CR_PNB_Pos));
	
	FLASH->CR |= FLASH_CR_STRT;
	//while ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY){};
	while ((FLASH->SR & FLASH_SR_EOP) != FLASH_SR_EOP){};
	FLASH->SR = FLASH_SR_EOP;
	FLASH->CR &= ~FLASH_CR_PER;
	
}

// data - указатель на записываемые данные
// address - адрес во flash
// count - количество записываемых слов
void FLASH_WriteWords(uint32_t* data, unsigned int address) {
	while ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY){};
	if (FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR = FLASH_SR_EOP;
	}
	// Очистили флаг ошибки
	FLASH->SR = FLASH_SR_PGAERR;
	FLASH->SR = FLASH_SR_PGSERR;
	// Разрешине прерывания и флага FLASH_SR_EOP
	FLASH->CR |= FLASH_CR_EOPIE;
	//Разрешаем программирование флеша
	FLASH->CR |= FLASH_CR_PG;
	// Записывает двойное слово
	*(__IO uint32_t*)(address) = *data;
	*(__IO uint32_t*)(address+4) = *data;
	//while ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY){};
	while ((FLASH->SR & FLASH_SR_EOP) != FLASH_SR_EOP){};
	FLASH->SR = FLASH_SR_EOP;	
	//Запрещаем программирование флеша
	FLASH->CR &= ~(FLASH_CR_PG);	
}

void FLASH_Fuse(int state){
	
	FLASH_Unlock();    
  FLASH_OB_Unlock();
	
  // Wait for last operation to be completed 
	while ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY){};
	if (FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR = FLASH_SR_EOP;
	}
  // Configure the RDP level in the option bytes register 
	switch (state){
		case 1:
			MODIFY_REG(FLASH->OPTR, FLASH_OPTR_RDP, OB_RDP_LEVEL_1);
			break;
		case 0xDEAD:
			MODIFY_REG(FLASH->OPTR, FLASH_OPTR_RDP, OB_RDP_LEVEL_0);
			break;
	}
		
  // Set OPTSTRT Bit 
  SET_BIT(FLASH->CR, FLASH_CR_OPTSTRT);
  // Wait for last operation to be completed */
  while ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY){};
	if (FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR = FLASH_SR_EOP;
	}
  // If the option byte program operation is completed, disable the OPTSTRT Bit 
  CLEAR_BIT(FLASH->CR, FLASH_CR_OPTSTRT);
	
	SET_BIT(FLASH->CR, FLASH_CR_OBL_LAUNCH);
	// Wait for last operation to be completed 
	while ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY){};
	if (FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR = FLASH_SR_EOP;
	}
	
	FLASH_OB_Lock();
	FLASH_Lock();
}



//=====================================================
// АЦП с верхней платы
//=====================================================
static inline void UpdateAD7794upr(void){
	switch (stageAD7794upr){
		case 0:
			// Настройка для считывания температуры лазера
			AD7794upr_Setup_ADC_TEMP();
			stageAD7794upr = 1;
			break;
		case 1:
			// Проверяем готовность данных
			if (AD7794upr_GetFlagCompleted() == true){
				// Расчет скользящего среднего
				rawValueAvrTempUPR -= rawDataTempUPR[indexDataTempUPR];
				rawDataTempUPR[indexDataTempUPR] = AD7794upr_GetValue();
				rawValueAvrTempUPR += rawDataTempUPR[indexDataTempUPR];
				// Забираем данные температуры АЦП верхней платы
				valueTempUPR = ConvertToTempADC(rawDataTempUPR[indexDataTempUPR]);				
				indexDataTempUPR++;
				if (indexDataTempUPR == countDataTempUPR){
					indexDataTempUPR = 0;
				}
				resultValueTempUPR = ((float)rawValueAvrTempUPR / (float)countDataTempUPR);
				resultValueTempUPR = ConvertToTempADC(resultValueTempUPR);
				// Разрешаем расчет компенсации сдвига частоты от температуры
				permission |= (1 << 8);	
			}			
			// Переключаем в режим считывания температуры лазера
			AD7794upr_Setup_VCSEL_TEMP();
			stageAD7794upr = 2;
			break;
		case 2:
			// Проверяем готовность данных
			if (AD7794upr_GetFlagCompleted() == true){
				// Расчет скользящего среднего
				rawValueAvrTempVCSEL -= rawValueDataTempVCSEL[indexDataTempVCSEL];
				rawValueDataTempVCSEL[indexDataTempVCSEL] = AD7794upr_GetValue();
				rawValueAvrTempVCSEL += rawValueDataTempVCSEL[indexDataTempVCSEL];
				// Забираем данные температуры лазера
				valueTempVCSEL = ConvertToTempNTC_VCSEL(rawValueDataTempVCSEL[indexDataTempVCSEL], refResistVCSEL);				
				indexDataTempVCSEL++;
				if (indexDataTempVCSEL == countDataTempVCSEL){				
					indexDataTempVCSEL = 0;
				}
				resultAvrTempVCSEL = ((float)rawValueAvrTempVCSEL / (float)countDataTempVCSEL);
				resultAvrTempVCSEL = ConvertToTempNTC_VCSEL(resultAvrTempVCSEL, refResistVCSEL);	
				// Разрешаем подстройку температуры лазера
				permission |= (1 << 2);
			}							
			AD7794upr_Setup_ADC_TEMP();
			stageAD7794upr = 1;///PDA
			break;
		case 3:
			// Проверяем готовность данных
			if (AD7794upr_GetFlagCompleted() == true){
				// Расчет скользящего среднего
				rawValueAvrTempNTC_AUX1 -= rawValueDataTempNTC_AUX1[indexDataTempNTC_AUX1];
				rawValueDataTempNTC_AUX1[indexDataTempNTC_AUX1] = AD7794upr_GetValue();
				rawValueAvrTempNTC_AUX1 += rawValueDataTempNTC_AUX1[indexDataTempNTC_AUX1];
				// Забираем данные температуры ячейки
				valueTempAUX1 = ConvertToTempNTC_AUX1(rawValueDataTempNTC_AUX1[indexDataTempNTC_AUX1], refResistAUX1);				
				indexDataTempNTC_AUX1++;
				if (indexDataTempNTC_AUX1 == countDataTempNTC_AUX1){
					indexDataTempNTC_AUX1 = 0;
				}
				resultAvrTempNTC_AUX1 = ((float)rawValueAvrTempNTC_AUX1 / (float)countDataTempNTC_AUX1);
				resultAvrTempNTC_AUX1 = ConvertToTempNTC_AUX1(resultAvrTempNTC_AUX1, refResistAUX1);
				// Разрешаем расчет компенсации температуры ячейки
				flagUpdateCamputeTempAUX1 = true;
			}			
			// Настройка для считывания температуры АЦП средней платы
			AD7794upr_Setup_VCSEL_TEMP();
			stageAD7794upr = 2;
			break;
	}
}

//=====================================================
// АЦП со средней платы
//=====================================================	
static inline void UpdateAD7794mdl(void){
	switch (stageAD7794mdl){
		case 0:
			// Настройка для считывания температуры ячейки
			AD7794mdl_Setup_ADC_TEMP();
			stageAD7794mdl = 1;
			break;
		case 1:
			// Проверяем готовность данных
			if (AD7794mdl_GetFlagCompleted() == true){	
				// Расчет скользящего среднего
				rawValueAvrTempMDL -= rawDataTempMDL[indexDataTempMDL];
				rawDataTempMDL[indexDataTempMDL] = AD7794mdl_GetValue();
				rawValueAvrTempMDL += rawDataTempMDL[indexDataTempMDL];
				// Забираем данные температуры АЦП средней платы
				valueTempMDL = ConvertToTempADC(rawDataTempMDL[indexDataTempMDL]);				
				indexDataTempMDL++;
				if (indexDataTempMDL == countDataTempMDL){
					resultValueTempMDL = ConvertToTempADC((float)rawValueAvrTempMDL / (float)countDataTempMDL) - levelValueTemp;
					indexDataTempMDL = 0;
					//flagUpdateCamputeATT_MOD = true;
				}
			}			
			// Настройка для считывания температуры ячейки
			AD7794mdl_Setup_CELL_TEMP();
			stageAD7794mdl = 2;
			
			break;
		case 2:
			// Проверяем готовность данных
			if (AD7794mdl_GetFlagCompleted() == true){
				// Расчет скользящего среднего
				rawValueAvrTempCELL_HTR -= rawValueDataTempCELL_HTR[indexDataTempCELL_HTR];
				rawValueDataTempCELL_HTR[indexDataTempCELL_HTR] = AD7794mdl_GetValue();
				rawValueAvrTempCELL_HTR += rawValueDataTempCELL_HTR[indexDataTempCELL_HTR];
				// Забираем данные температуры ячейки
				valueTempCELL = ConvertToTempNTC_CELL(rawValueDataTempCELL_HTR[indexDataTempCELL_HTR], refResistCELL);				
				indexDataTempCELL_HTR++;
				if (indexDataTempCELL_HTR == countDataTempCELL_HTR){
					indexDataTempCELL_HTR = 0;
				}
				resultAvrTempCELL_HTR = ((float)rawValueAvrTempCELL_HTR / (float)countDataTempCELL_HTR);
				resultAvrTempCELL_HTR = ConvertToTempNTC_CELL(resultAvrTempCELL_HTR, refResistCELL);
				// Разрешаем расчет компенсации температуры ячейки 2
				permission |= (1 << 3);
			}			
			// Настройка для считывания 
			//
			AD7794mdl_Setup_ADC_TEMP(); // PDA
			//AD7794mdl_Setup_AUX2_TEMP(); PDA
			stageAD7794mdl = 1;
			break;
		case 3:
			// Проверяем готовность данных
			if (AD7794mdl_GetFlagCompleted() == true){
				// Расчет скользящего среднего
				rawValueAvrTempNTC_AUX2 -= rawValueDataTempNTC_AUX2[indexDataTempNTC_AUX2];
				rawValueDataTempNTC_AUX2[indexDataTempNTC_AUX2] = AD7794mdl_GetValue();
				rawValueAvrTempNTC_AUX2 += rawValueDataTempNTC_AUX2[indexDataTempNTC_AUX2];
				// Забираем данные температуры ячейки
				valueTempAUX2 = ConvertToTempNTC_AUX2(rawValueDataTempNTC_AUX2[indexDataTempNTC_AUX2], refResistAUX2);				
				indexDataTempNTC_AUX2++;
				if (indexDataTempNTC_AUX2 == countDataTempNTC_AUX2){
					indexDataTempNTC_AUX2 = 0;				
				}
				resultAvrTempNTC_AUX2 = ((float)rawValueAvrTempNTC_AUX2 / (float)countDataTempNTC_AUX2);
				resultAvrTempNTC_AUX2 = ConvertToTempNTC_AUX2(resultAvrTempNTC_AUX2, refResistAUX2);
				// Разрешаем расчет компенсации температуры ячейки
				flagUpdateCamputeTempAUX2 = true;
			}			
			// Настройка для считывания температуры ячейки 1
			AD7794mdl_Setup_CELL_TEMP();
			stageAD7794mdl = 2;
			break;
	}
}

float CamputeShiftFreq(float value){
	float result = 0.0f;	
	// Поиск границы снизу
	float valueMax = -95;
	int indexDowmTemp = 0;
	bool flagDownTemp = false;
	// Поиск границу сверху
	float valueMin = 95;
	int indexUpTemp = 0;
	bool flagUpTemp = false;
	
	// пщем нижню границу температуры
	for (int i = 0; i < 32; i++){
		// Значение разрешено изспользовать		// Нижняя граница
		if (isTableCompensationFREQ[i] == 1 
			&& value >= tableCompensationFREQ[0][i] 
			&& tableCompensationFREQ[0][i] >= valueMax){
			valueMax = tableCompensationFREQ[0][i];
			indexDowmTemp = i;
			flagDownTemp = true;
		}
	}
	
	// пщем верхнюю границу температуры
	for (int i = 0; i < 32; i++){
		// Значение разрешено изспользовать			// Верхняя граница
		if (isTableCompensationFREQ[i] == 1 && flagDownTemp == true 
			&& tableCompensationFREQ[0][indexDowmTemp] < tableCompensationFREQ[0][i] 
			&& tableCompensationFREQ[0][i] <= valueMin){
			valueMin = tableCompensationFREQ[0][i];
			indexUpTemp = i;
			flagUpTemp = true;
		}
	}
	
	// Вычисляем линейное приближение
	if (flagDownTemp == true && flagUpTemp == true){
		float x1 = tableCompensationFREQ[0][indexDowmTemp];
		float y1 = tableCompensationFREQ[1][indexDowmTemp];
		float x2 = tableCompensationFREQ[0][indexUpTemp];		
		float y2 = tableCompensationFREQ[1][indexUpTemp];
		if ( x2 != x1){
			result = -((y1-y2)*value + (x1*y2-x2*y1))/(x2-x1);
		}
	}
	return result;
}

// Компенсация влияния тока ячейки дрейфа
static inline void CompensationCrntCell_FREQ(void){
	static float delta = 0.0f;
	if (statusLoopPID & (1 << 9)){
		if (permission & (1 << 9)){
			permission &= ~(1 << 9);
			delta = option_factor_C_TEMP_FREQ*value_CELL_HTR*value_CELL_HTR*value_CELL_HTR + 
							COMP_CRNT_A*value_CELL_HTR*value_CELL_HTR + 
							COMP_CRNT_B*value_CELL_HTR + 
							COMP_CRNT_C;
			LMX_ValueINT_CRNT_CELL = LMX_ValueINT + (int)delta;
			CamputeOptionSetValueLMX(LMX_ValueINT_CRNT_CELL);	
		}
	}
	
	
}

// Компенсация температурного дрейфа
static inline void CompensationTemp_FREQ(void){
	static float deltaA = 0.0f;
	static float deltaB = 0.0f;
	if (statusLoopPID & (1 << 8)){
		if (permission & (1 << 8)){
			permission &= ~(1 << 8);
			// Полином 2
			if (isUseTableCompensationFREQ == 0){
				// Детектирование по верхней плате
				deltaA = option_factor_A_TEMP_FREQ*(resultValueTempUPR*resultValueTempUPR);
				deltaA += option_factor_B_TEMP_FREQ*(resultValueTempUPR);
				deltaA += option_factor_C_TEMP_FREQ;
			// Таблица
			} else {
				deltaA = CamputeShiftFreq(resultValueTempUPR);
			}
			deltaB = 0.0f;
			// Компенсируем сдвиг магнитным полем							
			if (option_factor_B_COIL != 0.0f){
				deltaB = (deltaA - option_factor_C_COIL)/option_factor_B_COIL;							
			}
			value_COIL_CRNT = fixValue_COIL_CRNT + deltaB;	
			flag_COIL_CRNT = true;		
		}
	}
}

void ProgressPID_BASE(int item_pid){
	/*
	 * item_pid consist of
	 * Захват доплера током лазера 									0
	 * Захват частоты																1
	 * Захват температуры лазера по датчику					2
	 * Захват температуры ячейки										3
	 * Захват температуры хвостика ячейки						4
	 * Захват СВЧ мощности													5
	 * Захват доплера температурой									6
	 * Захват постоянной составляющей								7
	 * Компенсация температурного дрейфа						8
	 * Компенсация тока ячейки											9
	 * Захват мощности нагрева лазера 							10
	 * В массиве basic_values находятся указатели отслеживаемых величин (например, дискриминаторы), 
	 * в массиве changeable_values находятся указатели изменяемых (регулируемых) величин
	 * Функция insertion_pid_feature() применяет особенности некоторых ПИД-регуляторов 
	 * и в зависимости от места нахождения подразделяется
	 * на три части, она улучшает читаемость текста
	 * Действие ((((pid_countdown & (1 << item_pid)) >> item_pid)*2)-1) должно дать либо -1 либо 1 так, что правильно учтется знак
	 * Массивы start_cvs[] start_bvs[] содержат целевые значения и очищаются после отключения ПИД-регулятора
	 * cvs - changeable values: регулируемые величины 
	 * bvs - basic values: отслеживаемые величины (входные велчины)
	 * 
	 * в слове statusLoopPID побитово в соответствии с выше представленным списком представлены данные о включенных ПИД-регуляторах
	 * слово pid_actuation устроено как и statusLoopPID, однако в нем хранятся лишь данные о том, был ли ПИД-регулятор включен ранее
	 *
	 * */
	float delta = 0.0f;
	int low_lim = 0;
	int upper_lim = 65535;
	// установка пределов изменения 
	consid_feature_lims(&low_lim, &upper_lim, item_pid);

	if (statusLoopPID & (1 << item_pid)){
		if (permission & (1 << item_pid)){	// == true
			permission &= ~(1 << item_pid); //false
			if(!(pid_actuation & (1 << item_pid))){
				// установить
				pid_actuation |= 1 << item_pid;
				// в первое включение берется опорная мощность
				if(item_pid == 10) {
					start_cvs[item_pid] = value_TEC_CTRL;
					dbg1 = value_TEC_CTRL;
				}else if(item_pid == 0){
					start_cvs[item_pid] = value_VCSEL_DC;
				}
			}
			// начальные действия
			setting_initvals(item_pid);
			//в зависимости от типа указателя (типа величины) выбирается соответствующее поле объединения union
			if(basic_values[item_pid].type == FLOAT)
				delta = pid_factors[item_pid]*(float)(*(basic_values[item_pid].bridge_val.fp) - start_bvs[item_pid]);
			else if(basic_values[item_pid].type == INT)
				delta = pid_factors[item_pid]*(float)(*(basic_values[item_pid].bridge_val.ip) - start_bvs[item_pid]);
			
			if(item_pid == 10) dbg2 = delta;
			if(item_pid == 0) dbg4 = delta;

			insertion_pid_feature(&delta, 1, item_pid);

			if (changeable_values[item_pid].bridge_val.ip != &null && changeable_values[item_pid].type == INT){
				*(changeable_values[item_pid].bridge_val.ip) = (int)start_cvs[item_pid] +
						((((pid_countdown & (1 << item_pid)) >> item_pid)*2)-1)*(int)compute_pid_base(&primal_set[item_pid], delta);
				//установка пределов
				if (*(changeable_values[item_pid].bridge_val.ip) < low_lim){
					*(changeable_values[item_pid].bridge_val.ip) = low_lim;
				}else if (*(changeable_values[item_pid].bridge_val.ip) >= upper_lim){
					*(changeable_values[item_pid].bridge_val.ip) = upper_lim;
				}
			}else if (changeable_values[item_pid].type == FLOAT){
				*(changeable_values[item_pid].bridge_val.fp) = (int)start_cvs[item_pid] +
														((((pid_countdown & (1 << item_pid)) >> item_pid)*2)-1)*(int)compute_pid_base(&primal_set[item_pid], delta);
				//установка пределов
				if (*(changeable_values[item_pid].bridge_val.fp) < low_lim){
					*(changeable_values[item_pid].bridge_val.fp) = low_lim;
				}else if (*(changeable_values[item_pid].bridge_val.fp) >= upper_lim){
					*(changeable_values[item_pid].bridge_val.fp) = upper_lim;
				}
			}
			
			if (item_pid == 10) dbg3 = *(changeable_values[item_pid].bridge_val.ip);
			if (item_pid == 0) dbg5 = *(changeable_values[item_pid].bridge_val.ip);
			//установка флагов для разрешения изменения величин
			if(*(assertion + item_pid)!= &null_flag)
				**(assertion + item_pid) = true;
		}
		
		insertion_pid_feature(&delta, 2, item_pid);
		
	}
	else if (pid_actuation & (1 << item_pid))
	{
		// сбросить
		pid_actuation &= ~(1 << item_pid);
		reset_pid(&primal_set[item_pid]);
		start_bvs[item_pid] = 0;
		start_cvs[item_pid] = 0;
	}
}

static inline void consid_feature_lims(int* low_lim, int* upper_lim, int item_pid){
	switch(item_pid){
	case 0:
		*upper_lim = VCSEL_ScanLimitMax;
		*low_lim = VCSEL_ScanLimitMin;
		break;
	case 6:
		*upper_lim = 45000;
		*low_lim = 40000;
		break;
	case 10:
		*low_lim = 20000;
		break;
	default:
		break;
	}
}

static inline void setting_initvals(int item_pid){
	switch(item_pid){
		case 0:
			//start_cvs[item_pid] = fixValue_VCSEL_DC;
			break;
		case 1:
			start_cvs[item_pid] = fixValue_VCO_FINE;
			break;
		case 2:
			start_bvs[item_pid] = levelTempVCSEL;
			start_cvs[item_pid] = 65535;
			break;
		case 3:
			start_bvs[item_pid] = levelTempCELL;
			start_cvs[item_pid] = fixValue_CELL_HTR;
			break;
		case 6:
			start_bvs[item_pid] = 0;
			start_cvs[item_pid] = 42500;
			break;
		case 7:
			start_bvs[item_pid] = levelWork_OPTICS_PWR;
			start_cvs[item_pid] = fixValue_POWER_DC;
			break;
		case 10:
			start_bvs[item_pid] = desired_crnt;
			break;
		default:
			break;
	}
}

static inline void insertion_pid_feature(float* delta, char place, int item_pid){
	static char delay_CRS; 
	float absDelta = *delta;
		if(place == 1){
		switch(item_pid){
		case 1:
			// Определяем выполнен ли захват
			if (absDelta < 0){
				absDelta = -absDelta;
			}	
			if (absDelta <= sigmaPID_VCO){
				countLoopPID_VCO++;
			} else{
				countLoopPID_VCO = 0;
				isLoopPID_VCO = false;
			}
			if (countLoopPID_VCO >= timeLoopPID_VCO){
				countLoopPID_VCO = timeLoopPID_VCO;
				isLoopPID_VCO = true;
			}			
			
			if (flagAvrVCO == true){ //OFF
				// Расчет скользящего среднего
				valueAvrVCO -= valueDataVCO[indexDataVCO];
				valueDataVCO[indexDataVCO] = *delta;
				valueAvrVCO += valueDataVCO[indexDataVCO];
				indexDataVCO++;
				if (indexDataVCO == countDataVCO){
					indexDataVCO = 0;
				}
				*delta = valueAvrVCO / (float)countDataVCO;		
			}
			break;
		case 2:
			if(absDelta < 0){
				absDelta = -absDelta;
			}	
			if (absDelta <= sigmaPID_TEC_CTRL){
				countLoopPID_TEC_CTRL++;
			} else{
				countLoopPID_TEC_CTRL = 0;
				isLoopPID_TEC_CTRL = false;
			}
			if (countLoopPID_TEC_CTRL >= timeLoopPID_TEC_CTRL){
				countLoopPID_TEC_CTRL = timeLoopPID_TEC_CTRL;
				isLoopPID_TEC_CTRL = true;
			}
			break;
		case 3:
			if (absDelta < 0){
				absDelta = -absDelta;
			}	
			if (absDelta <= sigmaPID_CELL_HTR){
				countLoopPID_CELL_HTR++;
			} else{
				countLoopPID_CELL_HTR = 0;
				isLoopPID_CELL_HTR = false;
			}
			if (countLoopPID_CELL_HTR >= timeLoopPID_CELL_HTR){
				countLoopPID_CELL_HTR = timeLoopPID_CELL_HTR;
				isLoopPID_CELL_HTR = true;
			}
			break;
		default:
			break;
		}
	}else if(place == 2 && item_pid == 1){
		delay_CRS++;
		// Грубый шаг не синхронизирован с плавным (допнительный шум)
		// Перерасчитать Грубый канал, что бы тонкий был на уровне половины питания
		if (delay_CRS == 10){
			delay_CRS = 0;
			if ((value_VCO_FINE - 32768) > 5000){
				item_direction_VCO_shift = 1;
			}
			if ((value_VCO_FINE - 32768) < -5000){
				item_direction_VCO_shift = -1;
			}
			switch(item_direction_VCO_shift){
				case -1:
					value_VCO_CRS -= 1;
					flag_VCO_CRS = true;
					if ((value_VCO_FINE - 32768) > 0){
						item_direction_VCO_shift = 0;
					}
					break;
				case 0:
					break;
				case 1:
					value_VCO_CRS += 1;
					flag_VCO_CRS = true;
					if ((value_VCO_FINE - 32768) < 0){
						item_direction_VCO_shift = 0;
					}
					break;
			}
		}
	}
}

void change_moddemod(int* new_freq){
	/*
		Сначала вычисляется целая часть деления f_CLK / *new_freq, таким образом получаем период для 2го таймера, однако для 15го таймера период 
		тоже должен быть целым. Затем берутся два значения [x] и [x + 1], где x - отношение тактовой частоты к желаемой частоте модуляции *new_freq.
		Существует требование: выражение f_DAC1_0*CP_TIM2/f_ADC2_0 или 20*ds1.cp_left(right)/8 должно быть целым.
		Вычисляются две частоты модуляции f1 = f_CLK / [x] и f2 = f_CLK / [x + 1], и выполняется список условий:
			1. Если f1 и f2 подходят по требованию, то производится сравнение. Если f1 ближе к *new_freq, то устанавливаем ее,ели f2 ближе, то устанавливается f2.
			2. Если лишь одна из частот (f1 или f2) подходит по требованию, то устанавливается та, что подошла.
			3. Если ни одна из частот не удовлетворяет требованию, то все действия заново воспроизводятся, но уже для новых чисел - [x - 1] и [x + 2].
	*/
	float check_cp_left_tim15;
	float check_cp_right_tim15;
	ds1.cp_left = (uint32_t)(2000000 / *new_freq);
	ds1.cp_right = ds1.cp_left + 1;
	my_N_1 = *new_freq;
	while (1){
		check_cp_left_tim15 = 20.0f*(float)ds1.cp_left/8.0f;		// таким образом можно вычислить периоды для 15го таймера
		check_cp_right_tim15 = 20.0f*(float)ds1.cp_right/8.0f;	
		// далее производится проверка на целое обоих периодов
		if (check_cp_left_tim15 - (float)((int)check_cp_left_tim15) == 0 && check_cp_right_tim15 - (float)((int)check_cp_right_tim15) == 0){
			ds1.f_left = 2000000.0f / (float)ds1.cp_left;
			ds1.f_right = 2000000.0f / (float)ds1.cp_right;
			if(ds1.f_left - *new_freq > *new_freq - ds1.f_right){
				*new_freq = (int)(ds1.f_right*100);
				ds1.cp_tim2 = (int)ds1.cp_right;
				ds1.cp_tim15 = (int)check_cp_right_tim15;
				break;				
			}else{
				*new_freq = (int)(ds1.f_left*100);
				ds1.cp_tim2 = (int)ds1.cp_left;
				ds1.cp_tim15 = (int)check_cp_left_tim15;
				break;
			}
		}else if(check_cp_left_tim15 - (float)((int)check_cp_left_tim15) == 0){
			ds1.f_left = 2000000.0f / (float)ds1.cp_left;
			*new_freq = (int)(ds1.f_left*100);
			ds1.cp_tim2 = (int)ds1.cp_left;
			ds1.cp_tim15 = (int)check_cp_left_tim15;
			break;
		}else if(check_cp_right_tim15 - (float)((int)check_cp_right_tim15) == 0){
			ds1.f_right = 2000000.0f / (float)ds1.cp_right;
			*new_freq = (int)(ds1.f_right*100);
			ds1.cp_tim2 = (int)ds1.cp_right;
			ds1.cp_tim15 = (int)check_cp_right_tim15;
			break;
		}else{
			ds1.cp_left--;
			ds1.cp_right++;
		}
		if (ds1.cp_left < 190 || ds1.cp_right > 2050) return;
	}
	htim2.Init.Period = ds1.cp_tim2 - 1;
	htim15.Init.Period = ds1.cp_tim15 - 1;
	TIM2->CNT = 0;
	TIM15->CNT = 0;
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
  while(1)
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
