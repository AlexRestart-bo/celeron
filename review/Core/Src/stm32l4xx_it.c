/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32l4xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32l4xx_it.h"
#include "assistfuncs.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#include "var.h"
#include <math.h>
#include "usart_protocol.h"
// 24 разрядыный АЦП
static inline void UpdateRS232(void);
static inline void UpdateDataADC1(void);
static inline void UpdateDataADC2(void);
static inline void UpdateDataADC3(void);
static inline void UpdateDetectorCRNT(void);
static inline void UpdateDetectorFREQ(void);
static inline void UpdateDetectorRF(void);
static inline void UpdateDetectorFREQ20KHZ(void);
void LMX2486_SetFreq(uint32_t valueN, uint32_t valueFN, uint32_t valueFD);

void CamputeUpdateAD5668(void);

static inline void funWorkMode_TEST_LOOP_FREQ(void);
static inline void funWorkMode_TEST_LOOP_CRNT(void);
static inline void funWorkMode_TEST_DAC_ADC(void);
static inline void funWorkMode_TEST_TEMP(void);
static inline void funWorkMode_NONE(void);
static inline void funWorkMode_SCAN_CRNT(void);
static inline void funWorkMode_SCAN_TEMP(void);
static inline void funWorkMode_SCAN_FREQ(void);
static inline void funWorkMode_FIND_DOPLER_CRNT(void);
static inline void funWorkMode_FIND_DOPLER_TEMP(void);
static inline void funWorkMode_FIND_LORENZ(void);
static inline void funWorkMode_SET_VCSEL_DC(void);
static inline void funWorkMode_SET_ATT_PWR(void);
static inline void funWorkMode_RUN(void);
static inline void funFindLocalPeak(void);
static inline void funWorkMode_SCAN_TEC_VCSEL(void);
static inline void funWorkMode_SCAN_RF_ATT(void);
static inline void funWorkMode_SCAN_FINE_TUNING_FREQ(void);

void FLASH_LoadOptionData(void);

static inline void search_zero_point(void);

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;
extern DMA_HandleTypeDef hdma_adc3;
extern DMA_HandleTypeDef hdma_dac_ch1;
extern DMA_HandleTypeDef hdma_dac_ch2;
extern UART_HandleTypeDef huart3;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */
	// ADC1
	uint32_t flag_it = DMA1->ISR;
	// Первая половина буфера заполнена
	if ( (flag_it & DMA_FLAG_HT1) == DMA_FLAG_HT1){
		// Отключаем прерывание по заполнению первой половины
    DMA1_Channel1->CCR &= ~DMA_IT_HT;
		// Номер части буфера заполнена 1
		itemPartResultDMA1_ADC1 = 1;	
	// Буфер был заполнен
  } else if ((flag_it & DMA_ISR_TCIF1) == DMA_ISR_TCIF1){ 
		// Включаем прерывание по заполнению первой половины
    DMA1_Channel1->CCR |= DMA_IT_HT;
		// Номер части буфера заполнена 2
		itemPartResultDMA1_ADC1 = 2;			
  }
	// Clear all interrupt flags ERRATA не очищать CGIF, взамен HTIFx, TCIFx и TEIFx
	DMA1->IFCR |= DMA_IFCR_CTCIF1;
	DMA1->IFCR |= DMA_IFCR_CHTIF1;
	DMA1->IFCR |= DMA_IFCR_CTEIF1;
	
	// Вся партия данных получена
	if ( itemPartResultDMA1_ADC1 != 0 && 
			 itemPartResultDMA1_ADC2 != 0 && 
			 itemPartResultDMA1_ADC3 != 0 &&
			((EXTI->PR1 & EXTI_PR1_PIF0) != EXTI_PR1_PIF0)){
		// Вызов программного прерывания 
		EXTI->SWIER1 |= EXTI_SWIER1_SWI0;
	}
	#if defined(AUTODMA1)
  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */
	#endif
  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel2 global interrupt.
  */
void DMA1_Channel2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_IRQn 0 */
	// ADC2
	uint32_t flag_it = DMA1->ISR;
	// Первая половина буфера заполнена
	if ( (flag_it & DMA_FLAG_HT2) == DMA_FLAG_HT2){
		// Отключаем прерывание по заполнению первой половины
    DMA1_Channel2->CCR &= ~DMA_IT_HT;
		// Номер части буфера заполнена 1
		itemPartResultDMA1_ADC2 = 1;	
	// Буфер был заполнен
  } else if ((flag_it & DMA_ISR_TCIF2) == DMA_ISR_TCIF2){ 
		// Включаем прерывание по заполнению первой половины
    DMA1_Channel2->CCR |= DMA_IT_HT;
		// Номер части буфера заполнена 2
		itemPartResultDMA1_ADC2 = 2;			
  }
	// Clear all interrupt flags ERRATA не очищать CGIF, взамен HTIFx, TCIFx и TEIFx
	DMA1->IFCR |= DMA_IFCR_CTCIF2;
	DMA1->IFCR |= DMA_IFCR_CHTIF2;
	DMA1->IFCR |= DMA_IFCR_CTEIF2;
	// Вся партия данных получена
	if ( itemPartResultDMA1_ADC1 != 0 && 
			 itemPartResultDMA1_ADC2 != 0 && 
			 itemPartResultDMA1_ADC3 != 0 &&
			((EXTI->PR1 & EXTI_PR1_PIF0) != EXTI_PR1_PIF0)){
		// Вызов программного прерывания 
		EXTI->SWIER1 |= EXTI_SWIER1_SWI0;
	}
	#if defined(AUTODMA1)
  /* USER CODE END DMA1_Channel2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc2);
  /* USER CODE BEGIN DMA1_Channel2_IRQn 1 */
	#endif
  /* USER CODE END DMA1_Channel2_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel3 global interrupt.
  */
void DMA1_Channel3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel3_IRQn 0 */
	// ADC3
	uint32_t flag_it = DMA1->ISR;
	// Первая половина буфера заполнена
	if ( (flag_it & DMA_FLAG_HT3) == DMA_FLAG_HT3){
		// Отключаем прерывание по заполнению первой половины
    DMA1_Channel3->CCR &= ~DMA_IT_HT;
		// Номер части буфера заполнена 1
		itemPartResultDMA1_ADC3 = 1;	
	// Буфер был заполнен
  } else if ((flag_it & DMA_ISR_TCIF3) == DMA_ISR_TCIF3){ 
		// Включаем прерывание по заполнению первой половины
    DMA1_Channel3->CCR |= DMA_IT_HT;
		// Номер части буфера заполнена 2
		itemPartResultDMA1_ADC3 = 2;			
  }
	// Clear all interrupt flags ERRATA не очищать CGIF, взамен HTIFx, TCIFx и TEIFx
	DMA1->IFCR |= DMA_IFCR_CTCIF3;
	DMA1->IFCR |= DMA_IFCR_CHTIF3;
	DMA1->IFCR |= DMA_IFCR_CTEIF3;
	// Вся партия данных получена
	if ( itemPartResultDMA1_ADC1 != 0 && 
			 itemPartResultDMA1_ADC2 != 0 && 
			 itemPartResultDMA1_ADC3 != 0 &&
			((EXTI->PR1 & EXTI_PR1_PIF0) != EXTI_PR1_PIF0)){
		// Вызов программного прерывания 
		EXTI->SWIER1 |= EXTI_SWIER1_SWI0;
	}
	#if defined(AUTODMA1)
  /* USER CODE END DMA1_Channel3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc3);
  /* USER CODE BEGIN DMA1_Channel3_IRQn 1 */
	#endif

  /* USER CODE END DMA1_Channel3_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */
	UpdateRS232();
	#if defined(AUTOUSART3)
  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */
	#endif
  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles DMA2 channel4 global interrupt.
  */
void DMA2_Channel4_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Channel4_IRQn 0 */
	//DMA2->IFCR |= DMA_IFCR_CTCIF4;
	//DMA2->IFCR |= DMA_IFCR_CHTIF4;
	//DMA2->IFCR |= DMA_IFCR_CTEIF4;
	//#if defined(AUTODMA2)
  /* USER CODE END DMA2_Channel4_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_dac_ch1);
  /* USER CODE BEGIN DMA2_Channel4_IRQn 1 */
	//#endif
  /* USER CODE END DMA2_Channel4_IRQn 1 */
}

/**
  * @brief This function handles DMA2 channel5 global interrupt.
  */
void DMA2_Channel5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Channel5_IRQn 0 */
	//DMA2->IFCR |= DMA_IFCR_CTCIF5;
	//DMA2->IFCR |= DMA_IFCR_CHTIF5;
	//DMA2->IFCR |= DMA_IFCR_CTEIF5;
	//#if defined(AUTODMA2)
  /* USER CODE END DMA2_Channel5_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_dac_ch2);
  /* USER CODE BEGIN DMA2_Channel5_IRQn 1 */
	//#endif
  /* USER CODE END DMA2_Channel5_IRQn 1 */
}

/* USER CODE BEGIN 1 */

// Обработчик программного прерывания 
void EXTI0_IRQHandler(void){
	static uint16_t almsg = 0;
	// Сбрасываем счетчик
	DWT->CYCCNT = 0;
	// Включаем счетчик
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	// 1000 Hz
	// Очистили флаг прерывания
	EXTI->PR1 |= EXTI_PR1_PIF0;
	/*if (DWT->CYCCNT > DEBUG_maxDWT3){
		DEBUG_maxDWT3 = DWT->CYCCNT;
	}*/
	// Забираем все данные из буфера
	milisec ++;
	UpdateDataADC1();
	UpdateDataADC2();		
	UpdateDataADC3();	

	UpdateDetectorFREQ();
	UpdateDetectorCRNT();
	//UpdateDetectorRF();
	UpdateDetectorFREQ20KHZ();
	
	itemPartResultDMA1_ADC1 = 0;
	itemPartResultDMA1_ADC2 = 0;
	itemPartResultDMA1_ADC3 = 0;
	if (DWT->CYCCNT > DEBUG_maxDWT1){
		DEBUG_maxDWT1 = DWT->CYCCNT;
	}
	
	tiksDivAD7794++;
	//DEBUG_tiksDWT = DWT->CYCCNT;
	switch(itemModeWork){
		// Отсутвия выходных данных
		case WORK_NONE:
			funWorkMode_NONE();
			break;
		// Запуск стандарта
		case WORK_RUN:
			funWorkMode_RUN();
			break;
		// Скиновароние тока лазера
		case WORK_SCAN_CRNT:
			funWorkMode_SCAN_CRNT();
			break;
		// Сканирование температурой лазера
		case WORK_SCAN_TEMP:
			funWorkMode_SCAN_TEMP();
			break;
		// Сканирование частоты
		case WORK_SCAN_FREQ:	
			funWorkMode_SCAN_FREQ();
			break;
		// Поиск пика поглощения
		case WORK_FIND_DOPLER_CRNT:
			funWorkMode_FIND_DOPLER_CRNT();		
			break;
		case WORK_FIND_DOPLER_TEMP:
			//my_N1++;
			funWorkMode_FIND_DOPLER_TEMP();
			break;
		// Поиск пика Лоренца (КПН)
		case WORK_FIND_LORENZ:
			funWorkMode_FIND_LORENZ();
			break;
		// Поток данных синхронного детектора
		case WORK_TEST_DAC_ADC:
			funWorkMode_TEST_DAC_ADC();
			break;
		// Поток данных температуры стандарта
		case WORK_TEST_TEMP:
			funWorkMode_TEST_TEMP();
			break;
		// Тест ПпД регулятора привязки к пику поглощения (доплера)
		case WORK_TEST_LOOP_CRNT:
			funWorkMode_TEST_LOOP_CRNT();
			break;
		// Тест ПпД регулятора привязки частоты к Лоренцу (КПН)
		case WORK_TEST_LOOP_FREQ:
			funWorkMode_TEST_LOOP_FREQ();
			break;
		// Сканирование нагревателем лазера
		case WORK_SCAN_TEC_VCSEL:
			funWorkMode_SCAN_TEC_VCSEL();
			break;
		case WORK_SCAN_RF_ATT:
			funWorkMode_SCAN_RF_ATT();
			break;
		case WORK_TEST_VALUES:
			funWorkMode_SCAN_FINE_TUNING_FREQ();
			break;
	}
	
	// Вычисляем воздействие PID-регуляторов
	// Захват частоты
	ProgressPID_BASE(1);
	// Захват доплера током лазера
	ProgressPID_BASE(0);
	// Захват доплера температурой
	ProgressPID_BASE(6);
	//ProgressPID_DOPLER_TEMP_TEC();
	ProgressPID_BASE(10);
	
	
	// Программирование внешнего ЦАПа
	CamputeUpdateAD5668();
	
	if ( tiksDivAD7794 >= tiksCountAD7794 ){
		tiksDivAD7794 = 0;
		flagUpdateAD7794 = true;
	}
	// Перестройка частоты синтезатора
	// пзменение частоты синтезатора
	if (flag_LMX248 == true){
		flag_LMX248 = false;
		LMX2486_SetFreq(LMX_ValueN, LMX_ValueFN, LMX_ValueFD);
	}
	if (milisec == 7) milisec = -1;
	almsg++;
	if (almsg == 250){
		almsg = 0;
		int alarm = mod(discr_wave.resume_dw*1000);
		cc = (int)mod(dd / alarm);
		GetTestValue(GET_MIN_MAX_SIZE_STEK_HEAP, &aa, &bb, &cc, &dd);
	}
}

static inline void UpdateDetectorRF(void){
	static uint16_t count;
	static uint16_t step;
	// Мнимая часть
	static float Q;
	// Действительная часть
	static float I;
	switch(itemSourceDataRF){
		case 0:
			count = COUNT_DATA_HALF_SIZE_RF;
			step = LENGTH_PERIOD_RF;
			//===================================================
			// Квадратурны синхронный детектор СВЧ мощности
			//===================================================
			for( int k = 0; k < countAvrDetectorRF; k++){
				sumRF_Q[k] = 0;
				sumRF_I[k] = 0;
			}
			// Вынесли сумму
			for( int i = 0; i < count; i += step*2){
				for( int k = 0; k < countAvrDetectorRF; k++){
					sumRF_Q[k] += (int)ptrDataRF[i + k*2];
					sumRF_I[k] += (int)ptrDataRF[i + k*2];
				}
				// Считаем сколько раз усреднили
				indexAverage_ADC_RF++;
			}
			Q = 0.0f;
			I = 0.0f;
			// Среднее по полученому полубуферу
			for( int k = 0; k < countAvrDetectorRF; k++){
				Q += ((float)sumRF_Q[k])*tableRF_Sin[k];
				I += ((float)sumRF_I[k])*tableRF_Cos[k];
			}
			// Копим среднее по нескольник полубуферам
			value_Q_RF += Q;
			value_I_RF += I;
			break;
		// Source  CRNT
		case 1:
		// Source  DC
		case 2:
		// Source FREQ
		case 3:
			break;
	}
	
	// Захват RF 
	if (indexAverage_ADC_RF >= countAverage_ADC_RF){		
		countAverage_ADC_RF = indexAverage_ADC_RF;	
		value_Q_RF = value_Q_RF / ((float)countAverage_ADC_RF);
		value_Q_RF /= ((float)countAvrDetectorRF);	
		value_I_RF = value_I_RF / ((float)countAverage_ADC_RF);
		value_I_RF /= ((float)countAvrDetectorRF);	
		resultAverage_ADC_RF = value_I_RF - shiftDetector_RF;
		flagUpdateCampute_PDA_RF_ATT = true;
		value_Q_RF = 0.0f;
		value_I_RF = 0.0f;
		indexAverage_ADC_RF = 0;
		value_ADC_RF = 0;				
	}
	
}

//===================================================
// Квадратурны синхронный детектор частоты
//===================================================
/**
@param itemSourceDataFREQ принимает значения от нуля до трех
				0 - Source SRCH,	1 - Source CRNT,	2 - Source DC, 3 - Source FREQ
*/
static inline void UpdateDetectorFREQ(void){
	static float Q;			// Мнимая часть
	static float I;			// Действительная часть
	
	static const uint16_t count = COUNT_DATA_HALF_SIZE_FREQ;
	static const uint16_t step = LENGTH_PERIOD_FREQ;
	
	static const float factors[4] = {39.1f, 1.0f, 359.3f, 1.0f};
	
	for( int k = 0; k < countAvrDetectorFREQ; k++){
		sumFREQ_Q[k] = 0;
		sumFREQ_I[k] = 0;
	}
	// Вынесли сумму
	for( int i = 0; i < count; i += step){
		for( int k = 0; k < countAvrDetectorFREQ; k++){
			switch(itemSourceDataFREQ){ 
				case 0:
					sumFREQ_Q[k] += (int)ptrDataRF[2*(i + k)];
					sumFREQ_I[k] += (int)ptrDataRF[2*(i + k)];
					break;
				case 1:
					sumFREQ_Q[k] += (int)ptrDataCRNT[i + k];
					sumFREQ_I[k] += (int)ptrDataCRNT[i + k];
					break;
				case 2:
					sumFREQ_Q[k] += (int)ptrDataDC[2*(i + k)];
					sumFREQ_I[k] += (int)ptrDataDC[2*(i + k)];
					break;
				case 3:
					sumFREQ_Q[k] += (int)ptrDataFREQ[i + k];
					sumFREQ_I[k] += (int)ptrDataFREQ[i + k];
					break;
			}
		}
	indexAverage_ADC_FREQ++;
	}
	Q = 0.0f;
	I = 0.0f;
	// Среднее по полученому полубуферу
	for( int k = 0; k < countAvrDetectorFREQ; k++){
		Q += ((float)sumFREQ_Q[k])*tableFREQ_Sin[k];
		I += ((float)sumFREQ_I[k])*tableFREQ_Cos[k];
	}
	
	Q *= factors[itemSourceDataFREQ]; 		//для того чтобы в первых трех режимах 
	I *= factors[itemSourceDataFREQ];			//была одинакова амплитуда графиков
	// Копим среднее по нескольник полубуферам
	value_Q_FREQ += Q;
	value_I_FREQ += I;
	
	{
		// введем новые переменные для дискриминатора частоты
		//вставка 20 способа работы дискриминатор частоты
    FQ = Q / ((float)countAvrDetectorCRNT); //действительная часть
		FI = I / ((float)countAvrDetectorCRNT);//мнимая часть
		RESULT_ADC_FREQ = FI;
	}
	
	if (indexAverage_ADC_FREQ >= countAverage_ADC_FREQ){				
		countAverage_ADC_FREQ = indexAverage_ADC_FREQ;	
		value_Q_FREQ /= (float)countAverage_ADC_FREQ;
		value_Q_FREQ /= (float)countAvrDetectorFREQ;
		value_I_FREQ /= (float)countAverage_ADC_FREQ;
		value_I_FREQ /= (float)countAvrDetectorFREQ;
		ADC_FREQ = value_I_FREQ - shiftDetector_FREQ;
		ADC_FREQ_90 = value_Q_FREQ;
		//конец 
		if (isRecordScreenFREQ == true){
			saveData_FREQ[indexSaveFREQ] = (int)(ADC_FREQ*1000.0f);
			indexSaveFREQ++;
			if (indexSaveFREQ == COUNT_DATA_SAVE){
				indexSaveFREQ = 0;
				isRecordScreenFREQ = false;
				isScreenSaveDoneFREQ = true;
			}
		}
		value_Magnitude_FREQ = 0.0f;
		value_Q_FREQ = 0.0f;
		value_I_FREQ = 0.0f;
		indexAverage_ADC_FREQ = 0;
		permission |= (1 << 1);
	}
}

static inline void UpdateDetectorFREQ20KHZ(void){
	static uint16_t count;
	static uint16_t step;
	static float Q;
	static float I;
	static float accum_Q = 0;
	static float accum_I = 0;
	static int assemb_I[PERIOD_20KHZ];
	static int assemb_Q[PERIOD_20KHZ];
	static int jump = 0;
	static const int avrnums = 200;
	count = COUNT_DATA_HALF_SIZE_FREQ;
	step = PERIOD_20KHZ;
	
	for( int k = 0; k < PERIOD_20KHZ; k++){
		assemb_Q[k] = 0;
		assemb_I[k] = 0;
	}
	
	float fM = 1;
	// Вынесли сумму
	for( int i = 0; i < count; i += step){
		for( int k = 0; k < PERIOD_20KHZ; k++){
			switch(itemSourceDataFREQ){
				case 0:
					assemb_Q[k] += (int)ptrDataRF[2*(i + k)];
					assemb_I[k] += (int)ptrDataRF[2*(i + k)];
				  //fM = 39.1f;
					break;
				case 1:
					assemb_Q[k] += (int)ptrDataCRNT[i + k];
					assemb_I[k] += (int)ptrDataCRNT[i + k];
					break;
				case 2:
					assemb_Q[k] += (int)ptrDataDC[2*(i + k)];
					assemb_I[k] += (int)ptrDataDC[2*(i + k)];
					//fM = 359.3f;
					break;
				case 3:
					assemb_Q[k] += (int)ptrDataFREQ[i + k];
					assemb_I[k] += (int)ptrDataFREQ[i + k];
					break;
			}
		}
		jump++;
	}
	Q = 0.0f;
	I = 0.0f;
	// Среднее по полученому полубуферу
	for( int k = 0; k < PERIOD_20KHZ; k++){
		Q += ((float)assemb_Q[k])*tableFREQ20KHZ_Sin[k];
		I += ((float)assemb_I[k])*tableFREQ20KHZ_Cos[k];
	}
	accum_Q += Q*fM;
	accum_I += I*fM;
		
	if (jump >= avrnums){	
		accum_Q /= (float)(avrnums*PERIOD_20KHZ);
		accum_I /= (float)(avrnums*PERIOD_20KHZ);
		FREQ_POS = accum_I - shiftDetector_FREQ;
		FREQ_NEG = accum_Q;
		accum_Q = 0.0f;
		accum_I = 0.0f;
		jump = 0;
		//permission |= (1 << 1);
	}
}

//===================================================
// Квадратурны синхронный детектор тока
//===================================================
/**
@param itemSourceDataCRNT принимает значения от нуля до трех
				0 - Source RF,	1 - Source CRNT,	2 - Source DC, 3 - Source FREQ
*/
static inline void UpdateDetectorCRNT(void){
	static float Q;			// Мнимая часть
	static float I;			// Действительная часть
	
	const uint16_t count = COUNT_DATA_HALF_SIZE_CRNT;
	const uint16_t step = LENGTH_PERIOD_CRNT;
	
	const float factors[4] = {37.9f, 1.0f, 324.7f, 1.0f};
	
	for( int k = 0; k < countAvrDetectorCRNT; k++){
		sumCRNT_Q[k] = 0;
		sumCRNT_I[k] = 0;
	}
	
	// Вынесли сумму
	for( int i = 0; i < count; i += step*((itemSourceDataCRNT % 2 == 0) ? 2 : 1)){
		for( int k = 0; k < countAvrDetectorCRNT; k++){
			switch(itemSourceDataCRNT){
				case 0:
					sumCRNT_Q[k] += (int)ptrDataRF[i + k*2];
					sumCRNT_I[k] += (int)ptrDataRF[i + k*2];
					break;
				case 1:
					sumCRNT_Q[k] += (int)ptrDataCRNT[i + k];
					sumCRNT_I[k] += (int)ptrDataCRNT[i + k];
					break;
				case 2:
					sumCRNT_Q[k] += (int)ptrDataDC[i + 2*k];
					sumCRNT_I[k] += (int)ptrDataDC[i + 2*k];
					break;
				case 3:
					sumCRNT_Q[k] += (int)ptrDataFREQ[i + k];
					sumCRNT_I[k] += (int)ptrDataFREQ[i + k];
					break;
			}
		}
		// Считаем сколько раз усреднили
		indexAverage_ADC_DOPLER++;
	}
	
	Q = 0.0f;
	I = 0.0f;
	// Среднее по полученому полубуферу
	for( int k = 0; k < countAvrDetectorCRNT; k++){
		Q += ((float)sumCRNT_Q[k])*tableCRNT_Sin[k];
		I += ((float)sumCRNT_I[k])*tableCRNT_Cos[k];
	}
	
	Q *= factors[itemSourceDataCRNT];	//для того чтобы в первых трех режимах 
	I *= factors[itemSourceDataCRNT];	//была одинакова амплитуда графиков
	// Копим среднее по нескольник полубуферам
	value_Q_DOPLER += Q;
	value_I_DOPLER += I;
	
	{
		QQ = Q  / ((float)countAvrDetectorCRNT); 		//действительная часть
		II = I / ((float)countAvrDetectorCRNT);			//мнимая часть
		discr_wave.instant_dw = II;
	}
		switch (milisec) {
		case 0:
			break;
		case 1:			
			discr_wave.transit_first += discr_wave.instant_dw;
			ADC_FREQ_0 += RESULT_ADC_FREQ;
			break;
		case 2:
			discr_wave.transit_first += discr_wave.instant_dw;
			ADC_FREQ_0 += RESULT_ADC_FREQ;
			break;
		case 3:
			discr_wave.transit_first += discr_wave.instant_dw;
			ADC_FREQ_0 += RESULT_ADC_FREQ;
			flag_VCSEL_DC = true;
			if(my_N12){
				LMX2486_SetFreq(LMX_ValueN_N_1, LMX_ValueFN_N_1, LMX_ValueFD_N_1);
			}
			break;
		case 4:
			break;
		case 5:
			discr_wave.transit_second += discr_wave.instant_dw;
			ADC_FREQ_1 += RESULT_ADC_FREQ;
			break;
		case 6:
			discr_wave.transit_second += discr_wave.instant_dw;
			ADC_FREQ_1 += RESULT_ADC_FREQ;
			break;
		case 7:
			discr_wave.transit_second += discr_wave.instant_dw;
			ADC_FREQ_1 += RESULT_ADC_FREQ;
			ADC_FREQ_0 /= 3;
			ADC_FREQ_1 /= 3;
			discr_wave.transit_first /= 3;
			discr_wave.transit_second /= 3;
		  //discr_wave.resume_dw = (discr_wave.transit_first + discr_wave.transit_second) / (2*8) - shiftDetector_DOPLER;
			discr_wave.directly_dw += (discr_wave.transit_first + discr_wave.transit_second) / (2*8) - (shiftDetector_DOPLER / discr_wave.factor_beauty);
			ADC_FREQ_P = (ADC_FREQ_0 + ADC_FREQ_1)*15;
			ADC_FREQ_N = (ADC_FREQ_0 - ADC_FREQ_1)*15;
			debug_vars.aa = discr_wave.directly_dw;
			discr_wave.transit_first = 0.0f;
			discr_wave.transit_second = 0.0f;
			ADC_FREQ_0 = 0.0f;
			ADC_FREQ_1 = 0.0f;
			flag_VCSEL_DC = true;
			permission |= (1 << 6);
			permission |= (1 << 3);
			if(my_N12){
				LMX2486_SetFreq(LMX_ValueN_N_2, LMX_ValueFN_N_2, LMX_ValueFD_N_2);
			}
			break;
	}	
	// раз в восемь милисекунд вычисляется величина discr_wave.directly_dw, при этом, за каждую милисекунду 
	// величина indexAverage_ADC_DOPLER увеличивается на (count / step*((itemSourceDataCRNT % 2 == 0) ? 2 : 1),
	// поэтому самое малое число отсчетов indexAverage_ADC_DOPLER для изменения величины discr_wave.resume_dw
	// должно быть 8*(count / step*((itemSourceDataCRNT % 2 == 0) ? 2 : 1))
	if (indexAverage_ADC_DOPLER >= 8*(count / step*((itemSourceDataCRNT % 2 == 0) ? 2 : 1))*discr_wave.factor_average){	
		discr_wave.resume_dw = discr_wave.directly_dw / (float)(count / step)*discr_wave.factor_average;
		permission |= (1 << 0);
		if (isRecordScreenCRNT == true){
			saveData_CRNT[indexSaveCRNT] = (int)(discr_wave.resume_dw*1000.0f);
			indexSaveCRNT++;
			if (indexSaveCRNT == COUNT_DATA_SAVE){
				indexSaveCRNT = 0;
				isRecordScreenCRNT = false;
				isScreenSaveDoneCRNT = true;
			}
		}
		value_Q_DOPLER = 0.0f;
		value_I_DOPLER = 0.0f;
		indexAverage_ADC_DOPLER = 0;
		discr_wave.directly_dw = 0;
	}
}

// Разбираем данные с ADC1
static inline void UpdateDataADC1(void){
	// Обрабатываем результирующий буфер DMA1
	switch(itemPartResultDMA1_ADC1){
		// Первая часть буфера
		case 1:
			//for (int i = 0; i < ADC_ARRAY_DMA_ADC1_HALF_SIZE; i++){
			//	resultData_CRNT[i] = DMA_ADC1_Data[i];
			//}
			ptrDataCRNT = &DMA_ADC1_Data[0];
			break;
		// Вторая часть буфера
		case 2:
			//for (int i = 0; i < ADC_ARRAY_DMA_ADC1_HALF_SIZE; i++){
			//	resultData_CRNT[i] = DMA_ADC1_Data[ADC_ARRAY_DMA_ADC1_HALF_SIZE + i];	
			//}
			ptrDataCRNT = &DMA_ADC1_Data[ADC_ARRAY_DMA_ADC1_HALF_SIZE];
			break;
	}
}

// Разбираем данные с ADC2
static inline void UpdateDataADC2(void){
		// Обрабатываем результирующий буфер DMA1
	switch(itemPartResultDMA1_ADC2){
		// Первая часть буфера
		case 1:
			//for (int i = 0; i < ADC_ARRAY_DMA_ADC2_HALF_SIZE; i++){
			//	resultData_FREQ[i] = DMA_ADC2_Data[i];
			//}
			ptrDataFREQ = &DMA_ADC2_Data[0];
			break;
		// Вторая часть буфера
		case 2:
			//for (int i = 0; i < ADC_ARRAY_DMA_ADC2_HALF_SIZE; i++){
			//	resultData_FREQ[i] = DMA_ADC2_Data[ADC_ARRAY_DMA_ADC2_HALF_SIZE + i];	
			//}
			ptrDataFREQ = &DMA_ADC2_Data[ADC_ARRAY_DMA_ADC2_HALF_SIZE];
			break;
	}
}

// Разбираем данные с ADC3
static inline void UpdateDataADC3(void){
	
	// Обрабатываем результирующий буфер DMA1
	switch(itemPartResultDMA1_ADC3){
		// Первая часть буфера
		case 1:
			for (int i = 0; i < ADC_ARRAY_DMA_ADC3_HALF_SIZE; i += 80){
				value_ADC_OPTICS_PWR += DMA_ADC3_Data[i];
				indexAverage_ADC_OPTICS_PWR++;
			}
			ptrDataDC = &DMA_ADC3_Data[0];
			ptrDataRF = &DMA_ADC3_Data[1];
			break;
		// Вторая часть буфера
		case 2:
			for (int i = 0; i < ADC_ARRAY_DMA_ADC3_HALF_SIZE; i += 80){
				value_ADC_OPTICS_PWR += DMA_ADC3_Data[ADC_ARRAY_DMA_ADC3_HALF_SIZE + i];
				indexAverage_ADC_OPTICS_PWR++;
			}
			ptrDataDC = &DMA_ADC3_Data[ADC_ARRAY_DMA_ADC3_HALF_SIZE];
			ptrDataRF = &DMA_ADC3_Data[ADC_ARRAY_DMA_ADC3_HALF_SIZE+1];
			break;
	}
	
	if (indexAverage_ADC_OPTICS_PWR >= countAverage_ADC_OPTICS_PWR){		
		countAverage_ADC_OPTICS_PWR = indexAverage_ADC_OPTICS_PWR;	
		resultAverage_ADC_OPTICS_PWR = ((float)value_ADC_OPTICS_PWR) / ((float)countAverage_ADC_OPTICS_PWR);	
		// Подстройка уровня DC отключена
		if (!(statusLoopPID & (1 << 7))){
			levelWork_OPTICS_PWR = resultAverage_ADC_OPTICS_PWR;
			// Константа управляющего войздействия
			fixValue_POWER_DC = value_VCSEL_DC;			
		}
		indexAverage_ADC_OPTICS_PWR = 0;				
		value_ADC_OPTICS_PWR = 0;		
		permission |= (1 << 7);
	}
}


// Выставляем значения на выходах ЦАПа
void CamputeUpdateAD5668(void){
	// Грубая настройка частоты
	if (flag_VCO_CRS == true){
		flag_VCO_CRS = false;
		AD5668_SetValue(DAC_VCO_CRS, value_VCO_CRS);		
	}
	// Тонкая настройка частоты
	if (flag_VCO_FINE == true){
		flag_VCO_FINE = false;
		AD5668_SetValue(DAC_VCO_FINE, value_VCO_FINE);
	}
	// Настройка тока катушки
	if (flag_COIL_CRNT == true){
		flag_COIL_CRNT = false;
		AD5668_SetValue(DAC_COIL_CRNT, value_COIL_CRNT);
	}
	// Настройка тока лазера
	if (flag_VCSEL_DC == true){
		flag_VCSEL_DC = false;
		AD5668_SetValue(DAC_VCSEL_DC, value_VCSEL_DC);
	}
	// Мощность нагрева ячейки
	if (flag_CELL_HTR == true){
		flag_CELL_HTR = false;
		permission |= (1 << 9);
		AD5668_SetValue(DAC_CELL_HTR, value_CELL_HTR); 
	}
	// Мощность нагрева лазера
	if (flag_TEC_CTRL == true){
		flag_TEC_CTRL = false;
		AD5668_SetValue(DAC_TEC_CTRL, value_TEC_CTRL);
	}
	// Настройка аттенюатора
	if (flag_RF_ATT == true){
		flag_RF_ATT = false;
		AD5668_SetValue(DAC_RF_ATT, value_RF_ATT);
	}
	// Мощность охлаждение хвостика ячейки
	if (flag_AUX_TEC == true){
		flag_AUX_TEC = false;
		AD5668_SetValue(DAC_AUX_TEC, value_AUX_TEC);
	}
	
	 
}

// Обрабатываем запрос USART
static inline void UpdateRS232(void){
		// Забираем данные с UART 
	uint32_t isrflags = USART3->ISR;
  uint32_t cr1its = USART3->CR1;
  uint32_t cr3its = USART3->CR3;
  // Есть байт на прием
  if(((isrflags & USART_ISR_RXNE) == USART_ISR_RXNE)){
  // Забираем байт с USART  
		USART_rx_buffer[USART_rx_wr_index]= (uint8_t)USART3->RDR;
		// Очистили флаг приёма байта
		USART3->ICR = USART_ISR_RXNE;
		// Количество принятых байт
    USART_rx_count++;
    USART_rx_wr_index++;
    // Буфер цикличный
    if (USART_rx_wr_index == 255) {
      USART_rx_wr_index = 0;
    } 
    if (USART_rx_count == 255){
			USART_rx_count = 0; 
			USART_rx_wr_index = 0;
			USART_tx_rd_index = 0;
      USART_rx_buffer_overflow = 1;  
    }  
  }
  
	// Передатчик готов передавать данные
	if ((isrflags & UART_FLAG_TXE) == UART_FLAG_TXE){
		if ( USART_tx_buffer_overflow == true ){
			// количество байт на передачу при переполнении
			USART_tx_count = USART_tx_wr_index + (255 - USART_tx_rd_index);
		} else {
			// количество байт на передачу при нормальной работе
			USART_tx_count = USART_tx_wr_index - USART_tx_rd_index;
		}
		// Есть байт на передачу
		if (USART_tx_count > 0){
			// Отправляем байt
			USART3->TDR = (uint16_t)USART_tx_buffer[USART_tx_rd_index];
			USART_tx_buffer[USART_tx_rd_index] = 0;
			// Очистили флаг передачи байта
			USART3->ICR = UART_FLAG_TXE;
			USART_tx_rd_index++;
			if (USART_tx_rd_index == 255) {
				USART_tx_rd_index = 0;
				USART_tx_buffer_overflow = false;
			}
			USART_tx_count--;		
			if (USART_tx_count == 0){
				// Отключаем прерывание по окончанию передачи
				USART3->CR1 &= ~USART_CR1_TCIE;
			}
		}
	}
	// Очищаем флаг ошибки, если не успели принять байт
	USART3->ICR = UART_CLEAR_OREF;
	USART3->ICR = UART_CLEAR_NEF;
	USART3->ICR = UART_CLEAR_PEF;
	
}

static inline void funWorkMode_TEST_LOOP_FREQ(void){
	static uint32_t delaySend = 0;
	switch(itemStageWork_TEST_LOOP_FREQ){
		case 0:
			delaySend = 0;
			indexSaveFREQ = 0;
			isScreenSaveDoneFREQ = false;
			isRecordScreenFREQ = true;
			value_VCO_CRS += TESTLOOP_FREQ_step;
			flag_VCO_CRS = true;
			itemStageWork_TEST_LOOP_FREQ = 1;
			break;
		case 1:
			delaySend++;
			if (delaySend == 250*3){
				delaySend = 0;
				itemStageWork_TEST_LOOP_FREQ = 2;
			}
			break;
		case 2:
			if (isScreenSaveDoneFREQ == true){
				delaySend++;
				if (delaySend % 40 == 0){		
					static uint32_t dataSend[5];
					dataSend[0] = indexSaveFREQ;
					dataSend[1] = saveData_FREQ[indexSaveFREQ];
					dataSend[2] = (int)(0);
					dataSend[3] = (int)(ADC_FREQ*1000);
					dataSend[4] = saveData_CRNT[indexSaveFREQ];
					SendPkgData(STREAM_FREQ, ((uint8_t*)&dataSend), 4*4);
					indexSaveFREQ++;
					if ( indexSaveFREQ == COUNT_DATA_SAVE){
						indexSaveFREQ = 0;
						itemStageWork_TEST_LOOP_FREQ = 3;
					}
				}
			}
			break;
	}	
}

static inline void funWorkMode_TEST_LOOP_CRNT(void){
	static uint32_t delaySend = 0;
	switch(itemStageWork_TEST_LOOP_CRNT){
		case 0:
			delaySend = 0;
			indexSaveCRNT = 0;
			isScreenSaveDoneCRNT = false;
			isRecordScreenCRNT = true;
			value_VCSEL_DC += TESTLOOP_CRNT_step;
			fixValue_VCSEL_DC = value_VCSEL_DC;
			flag_VCSEL_DC = true;
			itemStageWork_TEST_LOOP_CRNT = 1;
			break;
		case 1:
			delaySend++;
			if (delaySend == 250*3){
				delaySend = 0;
				itemStageWork_TEST_LOOP_CRNT = 2;
			}
			break;
		case 2:
			if (isScreenSaveDoneCRNT == true){
				delaySend++;
				if (delaySend % 50 == 0){		
					static uint32_t dataSend[5];
					dataSend[0] = indexSaveCRNT;
					dataSend[1] = saveData_CRNT[indexSaveCRNT];
					dataSend[2] = (int)(0);
					dataSend[3] = (int)(ADC_FREQ*1000);
					dataSend[4] = saveData_FREQ[indexSaveCRNT];
					SendPkgData(STREAM_FREQ, ((uint8_t*)&dataSend), 4*4);
					indexSaveCRNT++;
					if ( indexSaveCRNT == COUNT_DATA_SAVE){
						indexSaveCRNT = 0;
						itemStageWork_TEST_LOOP_CRNT = 3;
					}
				}
			}
			break;
	}
}

static inline void funWorkMode_TEST_TEMP(void){
	static char md = 0;
	static int delaySend = 0;
	static int delaySend_2 = 0;
	static int32_t dataSend[19] ={0};
	
	//static uint16_t delaySend = 0;
	switch(md){
		case 0:		
			value_VCO_CRS = VCO_ScanLimitMin;
			flag_VCO_CRS = true;
			// Устанавливаем источник данных
			//itemSourceDataFREQ = 3;
			// Устанавливаем количество усреднений
			indexAverage_ADC_FREQ = 0;
			countAverage_ADC_FREQ = 100; // 100 Hz
			permission &= ~(1 << 1);
			delaySend = 0;
			md = 1;		
			break;
		case 1:
			delaySend++;
			if (delaySend >= 40){
				delaySend = 0;		
				dataSend[0] = (int)value_VCO_CRS;
				value_VCO_CRS += VCO_ScanStep;
				if (value_VCO_CRS >= VCO_ScanLimitMax){
					value_VCO_CRS = VCO_ScanLimitMin;
				}
				flag_VCO_CRS = true;				
				
				for(int i = 1; i < 18; i++)
					dataSend[i] /= 40;				
				SendPkgData(STREAM_FREQ, ((uint8_t*)&dataSend), 18*4);
				for(int i = 1; i < 19; i++)
					dataSend[i] = 0;
			}
			break;
	}
}

static inline void funWorkMode_TEST_DAC_ADC(void){
	static uint32_t delaySend = 0;
	delaySend++;
	// 10 Гц
	if (delaySend % 100 == 0){			
		static int dataSend[8];
		dataSend[0] = (int)delaySend/100;
		dataSend[1] = (int)(0);
		dataSend[2] = (int)(0);
		dataSend[3] = (int)(0);
		dataSend[4] = (int)(0);	
		dataSend[5] = (int)(0);			
		dataSend[6] = (int)(ADC_FREQ*1000);
		dataSend[7] = (int)(value_CELL_HTR*1000);
		SendPkgData(STREAM_FREQ, ((uint8_t*)&dataSend), 8*4);
	}
}

static inline void funWorkMode_NONE(void){
	switch(itemStageWork_NONE){
		case 0:
			itemStageWork_NONE = 1;
			break;
		case 1:		
			break;
	}
}

// Сканирование нагревателем лазера
static inline void funWorkMode_SCAN_TEC_VCSEL(void){
	static uint16_t delaySend = 0;
	switch(itemStageWork_SCAN_TEC_VCSEL){
		case 0:
			value_TEC_CTRL = SCAN_TEC_VCSEL_LimitMax;
			flag_TEC_CTRL = true;
			itemStageWork_SCAN_TEC_VCSEL = 1;
			break;
		case 1:
			// 1000 Гц / 10 =  10 Гц
			// Частота передачи данных 10 Гц
			delaySend++;
			if (delaySend % 100 == 0){				
				value_TEC_CTRL -= SCAN_TEC_VCSEL_Step;
				if ( value_TEC_CTRL <= SCAN_TEC_VCSEL_LimitMin){
					value_TEC_CTRL = SCAN_TEC_VCSEL_LimitMax;
				}	
				flag_TEC_CTRL = true;				
				static uint32_t dataSend[5];
				dataSend[0] = (int)(value_TEC_CTRL);
				dataSend[1] = (int)(0);
				dataSend[2] = (int)(resultAvrTempVCSEL*1000);
				dataSend[3] = (int)(ADC_FREQ*1000);
				dataSend[4] = (int)(resultAverage_ADC_OPTICS_PWR*1000);			 
				SendPkgData(STREAM_FREQ, ((uint8_t*)&dataSend), 5*4);
			}
			break;
	}
}

// Сканирование СВЧ мощностью
static inline void funWorkMode_SCAN_RF_ATT(void){
	static uint16_t delaySend = 0;
	switch(itemStageWork_SCAN_RF_ATT){
		case 0:
			itemStageWork_SCAN_RF_ATT = 1;
			break;
		case 1:
			// 250 Гц / 25 =  10 Гц
			// Частота передачи данных 10 Гц
			delaySend++;
			if (delaySend % 200 == 0){				
				static uint32_t dataSend[7];
				dataSend[0] = (int)(value_RF_ATT);
				dataSend[1] = (int)(resultAverage_Magnitude_RF*1000);
				dataSend[2] = (int)(resultAverage_Phase_RF*1000);
				dataSend[3] = (int)(resultAverage_ADC_RF*1000);
				dataSend[4] = (int)(0);			 
				dataSend[5] = (int)(resultAverage_ADC_OPTICS_PWR*1000);	
				dataSend[6] = (int)(ADC_FREQ*1000);	
				SendPkgData(STREAM_FREQ, ((uint8_t*)&dataSend), 7*4);
				
				value_RF_ATT += SCAN_RF_ATT_Step;
				if(value_RF_ATT > SCAN_RF_ATT_LimitMax){
					value_RF_ATT = SCAN_RF_ATT_LimitMax;
					SCAN_RF_ATT_Step = -SCAN_RF_ATT_Step;
				}
				if(value_RF_ATT < SCAN_RF_ATT_LimitMin){
					value_RF_ATT = SCAN_RF_ATT_LimitMin;
					SCAN_RF_ATT_Step = -SCAN_RF_ATT_Step;
				}
				flag_RF_ATT = true;
			}
			break;
	}
}

static inline void funWorkMode_SCAN_TEMP(void){	
	static float out = 0.0f;
	static uint16_t delaySend = 0;
	static uint16_t dev = 40;
	switch(itemStageWork_SCAN_CRNT){
		case 0:		
			delaySend = 0;
			itemStageWork_SCAN_CRNT = 1;	
			value_TEC_CTRL = 50000;
			isLoopPID_TEC_CTRL = false;		
			break;
		case 1:
			delaySend++;
			out += discr_wave.resume_dw;
			if (dd < discr_wave.resume_dw*1000) dd = discr_wave.resume_dw*1000;
			if (delaySend >= dev){
				delaySend = 0;
				out /= (float)dev;
				value_TEC_CTRL -= heat_cap.step;
				flag_TEC_CTRL = true;
				if ( value_TEC_CTRL <= heat_cap.min || value_TEC_CTRL > heat_cap.max){
					dd = 0;
					value_TEC_CTRL = heat_cap.max;
				}
				if(perm_auto_capt)search_zero_point();
				static uint32_t dataSend[7];
				dataSend[0] = (int)value_TEC_CTRL;
				dataSend[1] = (int)(ADC_FREQ*1000);
				dataSend[2] = (int)(out*1000); 
				dataSend[3] = (int)(discr_wave.resume_dw*1000*discr_wave.factor_beauty);
				dataSend[4] = (int)(ADC_FREQ*1000);
				dataSend[5] = (int)(0);		
				dataSend[6] = (int)(resultAverage_ADC_OPTICS_PWR*1000);//(((value_VCO_CRS<<16)|value_VCO_FINE));			
				SendPkgData(STREAM_FREQ, ((uint8_t*)&dataSend), 7*4);
				out = 0.0f;
			}
			break;
	}
}

static inline void funWorkMode_SCAN_CRNT(void){	
	static float out = 0.0f;
	static float outq = 0.0f;
	static uint16_t delaySend = 0;
	static uint16_t dev = 40;
	switch(itemStageWork_SCAN_CRNT){
		case 0:		
			delaySend = 0;
			itemStageWork_SCAN_CRNT = 1;	
			//value_TEC_CTRL = 50000;
			isLoopPID_TEC_CTRL = false;		
			break;
		case 1:
			// 1000 Гц / 40 =  25 Гц
			// Частота передачи данных 25 Гц
			delaySend++;
			//if (dd < discr_wave.resume_dw*100) dd = discr_wave.resume_dw*100;
			out += discr_wave.resume_dw;
			outq += ADC_FREQ;
			if (delaySend >= dev){
				delaySend = 0;
				out /= (float)dev;
				outq /= (float)dev;
				value_VCSEL_DC += VCSEL_ScanStep;
				if ( value_VCSEL_DC <= VCSEL_ScanLimitMin){
					//dd = 0;
					value_VCSEL_DC = VCSEL_ScanLimitMax;
				}else if ( value_VCSEL_DC >= VCSEL_ScanLimitMax){
					value_VCSEL_DC = VCSEL_ScanLimitMin;
				}
				flag_VCSEL_DC = true;
				//if(perm_auto_capt)search_zero_point();
				static uint32_t dataSend[7];
				dataSend[0] = value_VCSEL_DC;
				dataSend[1] = (int)(outq*100);
				dataSend[2] = (int)(out*100); 
				dataSend[3] = (int)(discr_wave.resume_dw);
				dataSend[4] = (int)(ADC_FREQ*1000);
				dataSend[5] = (int)(0);		
				dataSend[6] = (int)0;//(((value_VCO_CRS<<16)|value_VCO_FINE));			
				SendPkgData(STREAM_FREQ, ((uint8_t*)&dataSend), 7*4);
				out = 0.0f;
			}
			break;
	}
}

//моя функция тонкой настройки частоты
static inline void funWorkMode_SCAN_FINE_TUNING_FREQ(void){
	static uint16_t delaySend = 0;
	static uint32_t delaySend_2 = 0;
	static float accum1 = 0;
	const static int dev = 10;
	
	delaySend++;
	accum1 += discr_wave.resume_dw;
	if (delaySend >= dev){
		delaySend = 0;
		delaySend_2 ++;
		accum1 /= (float)dev;
		static uint32_t dataSend[8];
		dataSend[0] = (int)	delaySend_2;
		dataSend[1] = (int)(accum1*1000);
		dataSend[2] = value_VCSEL_DC - 45000;
		dataSend[3] = 0;
		dataSend[4] = 0;
		dataSend[5] = 0;
		dataSend[6] = 0;
		dataSend[7] = 0;
		SendPkgData(STREAM_FREQ, ((uint8_t*)&dataSend), 8*4);
		if (delaySend_2 > 5000) delaySend_2 = 0;
	}
}				
//конец функции тонкой настройки частоты

static inline void funWorkMode_SCAN_FREQ(void){
	int i;
	static int delaySend = 0;
	static int delaySend_2 = 0;
	static int32_t dataSend[9] ={0};
	//static float accum_delta = 0;
	//static float accum_dc = 0;
	
	//static uint16_t delaySend = 0;
	switch(itemStageWork_SCAN_FREQ){
		case 0:		
			value_VCO_CRS = VCO_ScanLimitMin;
			flag_VCO_CRS = true;
			// Устанавливаем источник данных
			//itemSourceDataFREQ = 3;
			// Устанавливаем количество усреднений
			indexAverage_ADC_FREQ = 0;
			countAverage_ADC_FREQ = 100; // 100 Hz
			permission &= ~(1 << 1);
			delaySend = 0;
			itemStageWork_SCAN_FREQ = 1;		
			break;
		case 1:
			// 1000 Гц / 40 = 25 Гц
			// Частота передачи данных 25 Гц
			dataSend[1] += (int)(RESULT_ADC_FREQ);
		  dataSend[2] += (int)(ADC_FREQ*1000);
			dataSend[3] += (int)(ADC_FREQ_90*1000);
			dataSend[4] += (int)(FREQ_POS*1000000);
		  dataSend[5] += (int)(FREQ_NEG*1000000);
		  dataSend[6] += (int)(ADC_FREQ_P*1);
			dataSend[7] += (int)(ADC_FREQ_N*10);
			
			delaySend++;
			if (delaySend >= 40){
				delaySend = 0;		
				dataSend[0] = (int)value_VCO_CRS;
				value_VCO_CRS += VCO_ScanStep;
				if (value_VCO_CRS >= VCO_ScanLimitMax){
					value_VCO_CRS = VCO_ScanLimitMin;
				}
				flag_VCO_CRS = true;				
				
				for(i = 1; i < 8; i++)
					dataSend[i] /= 40;
				//здесь будет проверяться ноль дискриминатора и выводиться ток
				//dataSend[1] = (int)(accum_delta*100.0f);
				//dataSend[4] = (int)(accum_dc*100.0f);
				SendPkgData(STREAM_FREQ, ((uint8_t*)&dataSend), 8*4);
				for(i = 1; i < 8; i++)
					dataSend[i] = 0;
			}
			break;
	}
}

static inline void funWorkMode_FIND_DOPLER_TEMP(void){
	static uint32_t time = 0;	
	static int delaySend = 0;
	static float accum_delta = 0;
	static float either_out = 0;
	static float accum_dc = 0;
	static uint32_t dataSend[5];
	delaySend++;
	accum_delta += ADC_FREQ;
	accum_dc += value_VCO_FINE - 32768;
	either_out += discr_wave.resume_dw;
	if (delaySend == 50){				
		delaySend = 0;
		//accum_delta /= 5;
		//accum_dc /= 50;
		dataSend[0] = (int)(time++);
		dataSend[1] = value_VCO_CRS;//(int)(accum_delta*1000.0f);
		dataSend[2] = value_VCO_FINE;//(int)(accum_dc*100.0f);
		dataSend[3] = 0;//(int)(either_out*10.0f);
		dataSend[4] = 0;//(int)(value_TEC_CTRL - 40000);
		SendPkgData(STREAM_FREQ, ((uint8_t*)&dataSend), 5*4);
		if(time == 65000) time = 0;
		accum_dc = 0;
		accum_delta = 0;
		either_out = 0;
	}
}


// (сейчас выбираем самый большой, это не всегда (оптимум) и зависит от СВЧ мощности)
// DEBUG искать нужно без СВЧ с переключением на нужный ток и включением нужного СВЧ
// растояние между пика фиксировавнная велечина, относительное расположение пика фиксированная величина
// пик уплывает от свч на фиксированную величину
// длина волны уплывает со временем
static inline void funWorkMode_FIND_DOPLER_CRNT(void){
	int j;
	int i;
	static uint32_t delaySend = 0;	
	static int itemSpectrum = 0;
	switch(itemStageWork_FIND_DOPLER_CRNT){
		case 0:
			reset_pid(&primal_set[0]);
			reset_pid(&primal_set[6]);
			// Устанавливаем источник данных
			itemSourceDataCRNT = 1;
			// Устанавливаем количество усреднений
			indexAverage_ADC_DOPLER = 0;
			permission &= ~(1 << 0);
			// Устанавливаем санирование в начало
			VCSEL_ScanValue = VCSEL_ScanLimitMin;
			value_VCSEL_DC = VCSEL_ScanValue;
			flag_VCSEL_DC = true;
			// Устанавливаем оптимальное ослабление мощности СВЧ
			value_RF_ATT = option_FIND_DC_PWR_RF_ATT;
			flag_RF_ATT = true;
			delaySend = 0;
			itemSpectrum = 0;
			VCSEL_tiks = 0;
		// Скользящая симметричная производная +-5
			VCSEL_ScanIndex = 0;
			VCSEL_ScanIndex_P1 = 0;
			VCSEL_ScanIndex_P2 = 10;
			itemModePeak = 0;
			VCSEL_indexPeak = 0;
			VCSEL_countPeak = 0;
			VCSEL_LocalBeginPeak_X = 0;
			VCSEL_LocalBeginPeak_Y = 0;
			VCSEL_LocalEndPeak_X = 0;
			VCSEL_LocalEndPeak_Y =0;
			VCSEL_LocalMaxPeak_X = 0;
			VCSEL_LocalMaxPeak_Y = 0;		
			VCSEL_GlobalMaxPeak_X = 0;		
			VCSEL_GlobalMaxPeak_Y = 0;
			for (i = 0; i < 11; i++){
				VCSEL_dataX[i] = value_VCSEL_DC;
				VCSEL_dataY[i] = 0.0f;
			}
			isDoneFindDopler = false;
			itemStageWork_FIND_DOPLER_CRNT = 1;
			break;
		// инициализация данных 1 раз
		case 1:
			// 250 Hz
			if (permission & (1 << 0)){
				permission &= ~(1 << 0);
				VCSEL_dataY[VCSEL_ScanIndex] = discr_wave.resume_dw;
				VCSEL_dataX[VCSEL_ScanIndex] = value_VCSEL_DC;
				VCSEL_ScanIndex++;
				// Окно производной
				if (VCSEL_ScanIndex == 11){
					VCSEL_ScanIndex = 0;
					itemStageWork_FIND_DOPLER_CRNT = 2;
				}
			}
			break;
		// Нахождение списка максимумов
		case 2:
			// 250 Hz
			if (permission & (1 << 0)){
				permission &= ~(1 << 0);
				// Значение сравнения для поиска пика
				VCSEL_ScanValue_X = (VCSEL_dataX[VCSEL_ScanIndex_P2] + 
														 VCSEL_dataX[VCSEL_ScanIndex_P1])/2;
				VCSEL_ScanValue_Y = VCSEL_dataY[VCSEL_ScanIndex_P1] - 
														VCSEL_dataY[VCSEL_ScanIndex_P2];
				
				if (VCSEL_ScanValue_Y < 0){
					VCSEL_ScanValue_Y = 0;
				}									
				// Обновили значение кольцевого буфера
				VCSEL_dataY[VCSEL_ScanIndex] = discr_wave.resume_dw;
				VCSEL_dataX[VCSEL_ScanIndex] = value_VCSEL_DC;
				//=================================================
				// Анализируем на присутствие пика
				//=================================================
				funFindLocalPeak();	
				// Переходим к следующему измерению
				VCSEL_ScanIndex_P1++;
				VCSEL_ScanIndex_P2++;
				VCSEL_ScanIndex++;
				if (VCSEL_ScanIndex == 11){
					VCSEL_ScanIndex = 0;
				}
				if (VCSEL_ScanIndex_P1 == 11){
					VCSEL_ScanIndex_P1 = 0;
				}
				if (VCSEL_ScanIndex_P2 == 11){
					VCSEL_ScanIndex_P2 = 0;
				}		
				VCSEL_ScanValue += VCSEL_ScanStep;
				// Прошли весь диапазон
				if (VCSEL_ScanValue >= VCSEL_ScanLimitMax){
					VCSEL_ScanValue = VCSEL_ScanLimitMax;
					// Завершаем сканирование 
					itemStageWork_FIND_DOPLER_CRNT = 3;
				}
				value_VCSEL_DC = VCSEL_ScanValue;
				flag_VCSEL_DC = true;			
			}
			break;
		// Выставляем максимальный пик
		case 3:
			// Сортируем список от большого к меньшему
			// Посчитай такты DEBUG
			// Сортировка пузырьком
			for ( j = 1; j < VCSEL_countPeak; j++){
				for ( i = 1; i < VCSEL_countPeak-j; i++){
					// Переставляем местами значения 
					if ( VCSEL_listPeak_Y[i] > VCSEL_listPeak_Y[i-1]){
						int swapY, swapX;
						swapY = VCSEL_listPeak_Y[i-1];
						swapX = VCSEL_listPeak_X[i-1];
						VCSEL_listPeak_Y[i-1] = VCSEL_listPeak_Y[i];
						VCSEL_listPeak_X[i-1] = VCSEL_listPeak_X[i];
						VCSEL_listPeak_Y[i] = swapY;
						VCSEL_listPeak_X[i] = swapX;
					}
				}
			}
			itemStageWork_FIND_DOPLER_CRNT = 4;
			// Самый левый и большой
			if (VCSEL_countPeak > 1){				
				// DEBUG (ПРОВЕРЬ)
				if ( VCSEL_listPeak_X[0] < VCSEL_listPeak_X[1]){
					value_VCSEL_DC = VCSEL_listPeak_X[0];
				} else {
					//value_VCSEL_DC = VCSEL_listPeak_X[0];
					value_VCSEL_DC = VCSEL_listPeak_X[1];
				}
				value_VCSEL_DC = VCSEL_listPeak_X[0];
				fixValue_VCSEL_DC = value_VCSEL_DC;
				flag_VCSEL_DC = true;
				isDoneFindDopler = true;
			}else {
				if (VCSEL_countPeak == 1){
					value_VCSEL_DC = VCSEL_listPeak_X[0];
					fixValue_VCSEL_DC = value_VCSEL_DC;
					flag_VCSEL_DC = true;
					isDoneFindDopler = true;
				}
			}				
			VCSEL_indexPeak = 0;
			break;
		case 4:
			// 250 Гц / 25 = 10 Гц
			// Частота передачи данных 10 Гц
			delaySend++;
			if (delaySend >= 200){
				delaySend = 0;			
				static uint32_t dataSend[5];
				switch(itemSpectrum){
					case 0:
						dataSend[0] = VCSEL_listPeak_X[VCSEL_indexPeak]-1;
						dataSend[1] = 0;
						dataSend[2] = VCSEL_indexPeak;
						dataSend[3] = 0;
						dataSend[4] = 0;			
						SendPkgData(STREAM_FREQ, ((uint8_t*)&dataSend), 5*4);
						itemSpectrum = 1;
						break;
					case 1:
						dataSend[0] = VCSEL_listPeak_X[VCSEL_indexPeak];
						dataSend[1] = (int)(1000*VCSEL_listPeak_Y[VCSEL_indexPeak]);
						dataSend[2] = VCSEL_indexPeak;
						dataSend[3] = 0;
						dataSend[4] = 0;							
						SendPkgData(STREAM_FREQ, ((uint8_t*)&dataSend), 5*4);
						itemSpectrum = 2;
						break;
					case 2:
						dataSend[0] = VCSEL_listPeak_X[VCSEL_indexPeak]+1;
						dataSend[1] = 0;
						dataSend[2] = VCSEL_indexPeak;
						dataSend[3] = 0;
						dataSend[4] = 0;				
						SendPkgData(STREAM_FREQ, ((uint8_t*)&dataSend), 5*4);
						VCSEL_indexPeak++;
						// Прекращаем записываем пики
						if (VCSEL_indexPeak >= VCSEL_countPeak || VCSEL_indexPeak >= 100){
							VCSEL_indexPeak = 0;
							itemStageWork_FIND_DOPLER_CRNT = 5;
						}
						itemSpectrum = 0;
						break;
				}
			}
			break;
	}
}

// Определяем пик по пересечению уровня DEBUG технический долг по логике работы
static inline void funFindLocalPeak(void){
	// Определяем пик по уровню
	switch(itemModePeak){
		// Определение начала пика
		case 0:
			// Перешли за уровень детектирования
			if (VCSEL_ScanValue_Y >= VCSEL_levelDetector){
				VCSEL_LocalBeginPeak_X = VCSEL_ScanValue_X;
				VCSEL_LocalBeginPeak_Y = VCSEL_ScanValue_Y;
			}
			// Уровень ушел дальше уровня шумов (вверх)
			if ( VCSEL_ScanValue_Y >= VCSEL_levelDetector + VCSEL_levelSigma &&
					 VCSEL_ScanValue_X >= VCSEL_ScanLimitMin + 11*VCSEL_ScanStep){
				// Переходим к определнию конца пика
				itemModePeak = 1;
			}
			break;
		case 1:						
			// ищем максимум
			if (VCSEL_ScanValue_Y >= VCSEL_LocalMaxPeak_Y){
				VCSEL_LocalMaxPeak_X = VCSEL_ScanValue_X;
				VCSEL_LocalMaxPeak_Y = VCSEL_ScanValue_Y;
			}
			// Перешли за уровень детектирования
			if (VCSEL_ScanValue_Y >= VCSEL_levelDetector){
				VCSEL_LocalEndPeak_X = VCSEL_ScanValue_X;
				VCSEL_LocalEndPeak_Y = VCSEL_ScanValue_Y;		
			}
			// Уровень ушел дальше уровня шумов (вниз)
			if ( VCSEL_ScanValue_Y <= VCSEL_levelDetector - VCSEL_levelSigma){
				// Записываем положение пика
				VCSEL_listPeak_X[VCSEL_indexPeak] = (VCSEL_LocalBeginPeak_X + VCSEL_LocalEndPeak_X) / 2;
				VCSEL_listPeak_Y[VCSEL_indexPeak] = VCSEL_LocalMaxPeak_Y;
				VCSEL_indexPeak++;
				VCSEL_countPeak++;
				// Прекращаем записываем пики
				if (VCSEL_indexPeak >= 100){
					VCSEL_indexPeak = 0;
				}
				// пщем глобальный максимум
				if ( VCSEL_LocalMaxPeak_Y >= VCSEL_GlobalMaxPeak_Y){						
					VCSEL_GlobalMaxPeak_X = VCSEL_LocalMaxPeak_X;
					VCSEL_GlobalMaxPeak_Y = VCSEL_LocalMaxPeak_Y;
				}
				VCSEL_LocalBeginPeak_X = 0;
				VCSEL_LocalEndPeak_X = 0;
				// Сбрасываем локальный максимум
				VCSEL_LocalMaxPeak_X = 0;
				VCSEL_LocalMaxPeak_Y = 0;
				// Переходим к определению начала следующего пика								
				itemModePeak = 0;
			}			
			break;
	}
}

static inline void funWorkMode_FIND_LORENZ(void){
	static uint32_t delaySend = 0;	
	static int itemSpectrum = 0;
	switch(itemStageWork_FIND_LORENZ){
		case 0:
			itemSourceDataFREQ = 3;
			value_Magnitude_FREQ = 0;
			value_Q_FREQ = 0.0f;
			value_I_FREQ = 0.0f;
			indexAverage_ADC_FREQ = 0;
			countAverage_ADC_FREQ = 100; // 100 Hz
			permission &= ~(1 << 1);		
			VCO_ScanValue = VCO_ScanLimitMin;
			value_VCO_CRS = VCO_ScanValue;
			flag_VCO_CRS = true;
			// Скользящая симметричная производная +-5
			VCO_ScanIndex = 0;
			VCO_ScanIndex_P1 = 0;
			VCO_ScanIndex_P2 = 4;
			VCO_GlobalMaxPeak_X[0] = 0;
			VCO_GlobalMaxPeak_Y[0] = 0.0f;
			VCO_GlobalMaxPeak_X[1] = 0;
			VCO_GlobalMaxPeak_Y[1] = 0.0f;
			VCO_GlobalMaxPeak_X[2] = 0;
			VCO_GlobalMaxPeak_Y[2] = 0.0f;
			VCO_GlobalPeakTriger = 0;
			VCO_ScanValue_X = 0;
			VCO_ScanValue_Y = 0;
			VCO_indexPeak = 0;
			value_COIL_CRNT = 65000;
			flag_COIL_CRNT = true;
			itemSpectrum = 0;
			isDoneFindLorenz = false;
			itemStageWork_FIND_LORENZ = 1;
			break;
		// инициализация даных 1 раз
		case 1:
			// 100 Hz
			if (permission & (1 << 1)){
				permission &= ~(1 << 1);
				VCO_dataY[VCO_ScanIndex] = ADC_FREQ;
				VCO_dataX[VCO_ScanIndex] = value_VCO_CRS;
				VCO_ScanValue += VCO_ScanStep;
				value_VCO_CRS = VCO_ScanValue;
				flag_VCO_CRS = true;	
				VCO_ScanIndex++;
				// Окно производной
				if (VCO_ScanIndex == 5){
					VCO_ScanIndex = 0;
					itemStageWork_FIND_LORENZ = 2;
				}
			}
			break;
		case 2:
			// 100 Hz
			if (permission & (1 << 1)){
				permission &= ~(1 << 1);
				// Значение сравнения для поиска пика
				VCO_ScanValue_X = (VCO_dataX[VCO_ScanIndex_P2] + 
													 VCO_dataX[VCO_ScanIndex_P1])/2;
				// Обратная величина
				VCO_ScanValue_Y = (VCO_dataY[VCO_ScanIndex_P1] - 
													VCO_dataY[VCO_ScanIndex_P2]);
				if (VCO_ScanValue_Y < 0){
					VCO_ScanValue_Y = 0;
				}					
				// Находим максимум
				if ( VCO_ScanValue_Y > 50 && VCO_ScanValue_X > 5000){
					if (VCO_ScanValue_X > VCO_GlobalPeakTriger + 1000){				
						VCO_GlobalMaxPeak_Y[2] = VCO_GlobalMaxPeak_Y[1];
						VCO_GlobalMaxPeak_X[2] = VCO_GlobalMaxPeak_X[1];
						VCO_GlobalMaxPeak_Y[1] = VCO_GlobalMaxPeak_Y[0];
						VCO_GlobalMaxPeak_X[1] = VCO_GlobalMaxPeak_X[0];
					}
					VCO_GlobalMaxPeak_Y[0] = VCO_ScanValue_Y;
					VCO_GlobalMaxPeak_X[0] = VCO_ScanValue_X;	
					VCO_GlobalPeakTriger = VCO_ScanValue_X;					
				}				
				// Обновили значение кольцевого буфера
				VCO_dataY[VCO_ScanIndex] = ADC_FREQ;
				VCO_dataX[VCO_ScanIndex] = value_VCO_CRS;
				// Переходим к следующему измерению
				// Скользящяя производная
				VCO_ScanIndex_P1++;
				VCO_ScanIndex_P2++;
				VCO_ScanIndex++;
				if (VCO_ScanIndex == 5){
					VCO_ScanIndex = 0;
				}
				if (VCO_ScanIndex_P1 == 5){
					VCO_ScanIndex_P1 = 0;
				}
				if (VCO_ScanIndex_P2 == 5){
					VCO_ScanIndex_P2 = 0;
				}		
				VCO_ScanValue += VCO_ScanStep;
				// Прошли весь диапазон
				if (VCO_ScanValue >= VCO_ScanLimitMax){
					VCO_ScanValue = VCO_ScanLimitMin;					
					// Завершаем сканирование 
					itemStageWork_FIND_LORENZ = 3;
				}
				value_VCO_CRS = VCO_ScanValue;
				flag_VCO_CRS = true;	
			}
			break;
		// Выставляем максимальный пик
		case 3:
			// Наибольший правильный 
			if (VCO_GlobalMaxPeak_Y[0] > VCO_GlobalMaxPeak_Y[1] && VCO_GlobalMaxPeak_Y[0] > VCO_GlobalMaxPeak_Y[2]){
				VCO_ScanValue = VCO_GlobalMaxPeak_X[0];
			}
			if (VCO_GlobalMaxPeak_Y[1] > VCO_GlobalMaxPeak_Y[0] && VCO_GlobalMaxPeak_Y[1] > VCO_GlobalMaxPeak_Y[2]){
				VCO_ScanValue = VCO_GlobalMaxPeak_X[1];
			}
			if (VCO_GlobalMaxPeak_Y[2] > VCO_GlobalMaxPeak_Y[1] && VCO_GlobalMaxPeak_Y[2] > VCO_GlobalMaxPeak_Y[0]){
				VCO_ScanValue = VCO_GlobalMaxPeak_X[2];
			}
			value_VCO_CRS = VCO_ScanValue;
			flag_VCO_CRS = true;	
			isDoneFindLorenz = true;
			VCO_indexPeak = 0;
			itemStageWork_FIND_LORENZ = 4;
			// Возвращаем на 1 КГц
			indexAverage_ADC_FREQ = 0;
			countAverage_ADC_FREQ = 10; // 1000 Hz
			break;
		case 4:
			// Частота передачи данных 5 Гц
			delaySend++;
			if (delaySend >= 200){
				delaySend = 0;			
				static uint32_t dataSend[5];
				switch(itemSpectrum){
					case 0:
						dataSend[0] = VCO_GlobalMaxPeak_X[VCO_indexPeak]-1;
						dataSend[1] = 0;
						dataSend[2] = VCO_indexPeak;
						dataSend[3] = 0;
						dataSend[4] = 0;			
						SendPkgData(STREAM_FREQ, ((uint8_t*)&dataSend), 5*4);
						itemSpectrum = 1;
						break;
					case 1:
						dataSend[0] = VCO_GlobalMaxPeak_X[VCO_indexPeak];
						dataSend[1] = (int)(1000*VCO_GlobalMaxPeak_Y[VCO_indexPeak]);
						dataSend[2] = VCO_indexPeak;
						dataSend[3] = 0;
						dataSend[4] = 0;							
						SendPkgData(STREAM_FREQ, ((uint8_t*)&dataSend), 5*4);
						itemSpectrum = 2;
						break;
					case 2:
						dataSend[0] = VCO_GlobalMaxPeak_X[VCO_indexPeak]+1;
						dataSend[1] = 0;
						dataSend[2] = VCO_indexPeak;
						dataSend[3] = 0;
						dataSend[4] = 0;				
						SendPkgData(STREAM_FREQ, ((uint8_t*)&dataSend), 5*4);
						VCO_indexPeak++;
						// Прекращаем записываем пики
						if (VCO_indexPeak > 1){
							itemStageWork_FIND_LORENZ = 5;
						}
						itemSpectrum = 0;
						break;
				}
			}
			break;
	}
	
}

// Обновление 1000 Гц
static inline void funWorkMode_RUN(void){
	static uint32_t delaySend = 0;	
	static uint32_t delayPause = 0;
	static uint32_t tiksStart = 0;
	
	switch (itemStageWork_RUN){
		// пнициализация
		case 0:
			tiksStart = 0;
			FLASH_LoadOptionData();
			value_VCSEL_DC = option_VCSEL_DC;
			flag_VCSEL_DC = true;	
			flag_AUX_TEC = true;
			// Отключаем все ПпД регуляторы
			statusLoopPID = 0;
			reset_pid(&primal_set[1]);
			reset_pid(&primal_set[0]);
			reset_pid(&primal_set[6]);
			reset_pid(&primal_set[3]);
			reset_pid(&primal_set[2]);			
			reset_pid(&primal_set[7]);
			countLoopPID_CELL_HTR = 0;
			countLoopPID_TEC_CTRL = 0;
			value_TEC_CTRL = CRNT_TEC_OFF;
			delaySend = 0;	
			delayPause = 0;
			flag_TEC_CTRL = true;
			SVD_IsDone = false;
			SATTP_IsDone = false;
			isErrorTempVCSEL = false;
			isLoopPID_CELL_HTR = false;
			isLoopPID_TEC_CTRL = false;
			isLoopPID_VCO = false;
			isDoneFindLorenz = false;
			permission &= ~(1 << 7);
			permission &= ~(1 << 0);
			permission &= ~(1 << 1);			
			itemStageWork_FIND_DOPLER_CRNT = 0;
			itemStageWork_FIND_DOPLER_TEMP = 0;
			value_COIL_CRNT = 65000;
			fixValue_COIL_CRNT = option_COIL_CRNT;
			flag_COIL_CRNT = true;
			// Охлаждаем хвостик ячейки
			value_AUX_TEC = option_AUX_TEC;
			flag_AUX_TEC = true;			
			itemStageWork_RUN = 1;
			break;
		// Включение захвата тока лазера и ячейки
		case 1:
			// Включили PID регулятор температуры лазера
			//statusLoopPID |= PID_FLAG_LOOP_TEC_CTRL;
			// Включили нагрев на максимум
			value_CELL_HTR = 65535;
			flag_CELL_HTR = true;
			itemStageWork_RUN = 2;
			break;
		// Ожидание захвата температуры лазера и ячейки
		case 2:
			// Ускорение выхода на режим температуры ячейки
			// Устраняем известную инерцию датчика температуры
			// пнтегратор на 0
			if (resultAvrTempCELL_HTR > levelStartTempCELL && tiksStart >= 10000){
				// Выключили нагрев, но запустили ПпД ячейки
				value_CELL_HTR = 0;
				flag_CELL_HTR = true;
				statusLoopPID |= (1 << 3);	
				itemStageWork_RUN = 3;
			}
			break;
		// Поиск пика поглощения
		case 3:
			// Ищем температуры лазера
			funWorkMode_FIND_DOPLER_TEMP();
			// Затем ищем ток лазера
			if (itemStageWork_FIND_DOPLER_TEMP == 2 || itemStageWork_FIND_DOPLER_TEMP == 5){
				itemStageWork_FIND_DOPLER_TEMP = 5;
				funWorkMode_FIND_DOPLER_CRNT();
			}
			// Пик найден и выбран
			if (isDoneFindDopler == true){					
				// Устанавливаем источник данных
				isDoneFindDopler = false;
				itemStageWork_FIND_DOPLER_CRNT = 0;
				delayPause = 0;
				itemStageWork_RUN = 5;
			} else {
				// Поиск закончен
				if (itemStageWork_FIND_DOPLER_CRNT == 4){
					// Пик не был найден (ячейка не успела прогреться)
					// Повтряем поиск
					itemStageWork_FIND_DOPLER_CRNT = 0;
					itemStageWork_FIND_DOPLER_TEMP = 0;
					itemStageWork_RUN = 3;					
				}
			}	
			break;
		// Ждем установки температуры лазера 1 сек
		case 4:
			delayPause++;
			if (delayPause >= (500)){
				delayPause = 0;
				itemStageWork_RUN = 5;
			}
			break;
		// Повторный поиск пика
		case 5:
			funWorkMode_FIND_DOPLER_CRNT();
			// Пик найден и выбран
			if (isDoneFindDopler == true){					
				// Устанавливаем источник данных
				itemSourceDataCRNT = 1;
				// Устанавливаем количество усреднений
				value_Q_DOPLER = 0.0f;
				value_I_DOPLER = 0.0f;
				indexAverage_ADC_DOPLER = 0;
				permission &= ~(1 << 0);
				fixValue_VCSEL_DC = value_VCSEL_DC;
				reset_pid(&primal_set[0]);
				// Включаем привязку к пику поглощения током
				statusLoopPID |= (1 << 0);
				delayPause = 0;
				itemStageWork_RUN = 6;
			} else {
				if (itemStageWork_FIND_DOPLER_CRNT == 4){
					itemStageWork_FIND_DOPLER_CRNT = 0;
					itemStageWork_FIND_DOPLER_TEMP = 0;
					itemStageWork_RUN = 3;		
				}					
			}
			break;
		// Ожидание захвата пика поглощения
		case 6:			
			delayPause++;
			if (delayPause >= (2000)){
				delayPause = 0;
				// Устанавливаем источник данных
				itemSourceDataCRNT = 1;
				// Устанавливаем количество усреднений				
				value_Q_DOPLER = 0.0f;
				value_I_DOPLER = 0.0f;
				indexAverage_ADC_DOPLER = 0;
				permission &= ~(1 << 0);
				reset_pid(&primal_set[6]);				
				// Выключаем привязку к пику поглощения
				statusLoopPID &= ~(1 << 0);
				// Выключили захват температуры лазера (датчик)
				statusLoopPID &= ~(1 << 2);								
				// Включили захват пика поглощения с помощью температуры
				statusLoopPID |= (1 << 6);		
				itemStageWork_RUN = 7;			
			}
			break;
			// Поиск пика КПН
		// Подстраиваем ток лазера до рабочего значения
		case 7:
			funWorkMode_SET_VCSEL_DC();
			if (SVD_IsDone == true){
				itemStageWork_RUN = 8;
				delayPause = 0;
			}
			break;
		case 8:
			delayPause++;
			if (delayPause >= 5000){
				funWorkMode_FIND_LORENZ();
			}
			if ( isDoneFindLorenz == true){
				delayPause = 0;
				itemSourceDataFREQ = 3;
				value_Magnitude_FREQ = 0;
				value_Q_FREQ = 0.0f;
				value_I_FREQ = 0.0f;
				indexAverage_ADC_FREQ = 0;
				countAverage_ADC_FREQ = 10; // 1000 Hz
				permission &= ~(1 << 1);		
				value_COIL_CRNT = option_COIL_CRNT;
				fixValue_COIL_CRNT = option_COIL_CRNT;
				flag_COIL_CRNT = true;
				// Включаем привязку к Лоренцу (КПН)
				statusLoopPID |= (1 << 1);
				itemStageWork_RUN = 9;
			}
			break;	
		// Подстраиваем СВЧ мощность до рабочего значения
		case 9:
			funWorkMode_SET_ATT_PWR();
			if (SATTP_IsDone == true){
				// Включаем термокомпенсацию
				statusLoopPID |= (1 << 8);
				itemStageWork_RUN = 10;
			}
			break;
		case 10:
			if (tiksStart >= (1000*150) && (isLoopPID_CELL_HTR == true)){			
				statusLoopPID |= (1 << 7);
				//statusLoopPID |= PID_FLAG_LOOP_RF_ATT;
				itemStageWork_RUN = 11;
			}
			break;
		// В захвате
		case 11:
			break;
	}
	tiksStart++;
	// пнформационный регистр о исправной работе 
	status_CPT = (itemStageWork_RUN & 0xFF);
	// Привязка температуры ячейки на уровне sigmaPID_CELL_HTR
	if (isLoopPID_CELL_HTR == true){
		status_CPT |= STATUS_LOOP_TEMP_CELL;
	}
	// Привязка длины волны лазера к линии поглощения без ошибок в рабочем режиме
	if (isErrorTempVCSEL == false){
		status_CPT |= STATUS_LOOP_VCSEL;
	}
	if (isLoopPID_VCO == true){
		status_CPT |= STATUS_LOOP_VCO;
	}
	// Есть сигнал на выходе канала фотоприёмника DC (лазер светит)
	if (resultAverage_ADC_OPTICS_PWR > 1000.0f && resultAverage_ADC_OPTICS_PWR < 3800.0f){
		status_CPT |= STATUS_DC;
	}
	// Управляющее значение кварцевым генератором находится в рабочем диапазоне
	if (value_VCO_CRS > 1000 && value_VCO_CRS < 64535){
		status_CPT |= STATUS_VCO_CRS;
	}
	// Мощность нагрева лазера находится в рабочем диапазоне
	if (value_TEC_CTRL > 2000 && value_TEC_CTRL < 64535){
		status_CPT |= STATUS_PWR_VCSEL;
	}
	// Мощность нагрева ячейки находится в рабочем диапазоне
	if (value_CELL_HTR > 1000 && value_CELL_HTR < 64535){
		status_CPT |= STATUS_PWR_CELL;
	}
	// Привязка температуры лазера в момент запуска на уровне sigmaPID_TEC_CTRL
	if (isLoopPID_TEC_CTRL == true){
		status_CPT |= STATUS_LOOP_TEMP_VCSEL;
		if ((itemStageWork_RUN & 0xFF) == 11){
			status_CPT |= STATUS_LOOP_TEMP_VCSEL;
		}
	}
	
	// Вывод информации для отладки
	if ( (statusAccessRS232 & RS232_DEBUG) == RS232_DEBUG){
		// 10 Hz
		if (tiksStart % 100 == 0){
			static int dataSend[10];
			dataSend[0] = (int)tiksStart;
			dataSend[1] = (int)((value_VCO_CRS<<16)|value_VCO_FINE);
			dataSend[2] = 0;
			dataSend[3] = (int)(resultAverage_ADC_OPTICS_PWR*1000);
			dataSend[4] = (int)(resultAvrTempVCSEL*1000);	
			dataSend[5] = (int)(resultAvrTempCELL_HTR*1000);							
			dataSend[6] = (int)(resultValueTempUPR*1000);
			dataSend[7] = (int)(value_TEC_CTRL);
			dataSend[8] = (int)(value_CELL_HTR);
			dataSend[9] = (int)(value_VCSEL_DC);
			SendPkgData(STREAM_FREQ, ((uint8_t*)&dataSend), 10*4);
		}
	// Автоматическое оповещение пользователя о состоянии системы
	}else{
		delaySend++;
		// 1 Hz
		if (delaySend % 1000 == 0){
			if (updateStatus_CPT == 1){
				SendPkgData(GET_STATUS_CPT, ((uint8_t*)&status_CPT), 4);
			}
		}
	}
}


// Установка рабочего значения тока
static inline void funWorkMode_SET_VCSEL_DC(void){
	static uint16_t delay = 0;
	switch (itemStageWork_SET_VCSEL_DC){
		case 0:
			itemStageWork_SET_VCSEL_DC = 1;
			SVD_IsDone = false;
			delay = 0;
			break;
		case 1:
			delay++;
			if ( delay == SVD_tiksCount ){
				delay = 0;
				if ( value_VCSEL_DC > option_VCSEL_DC){
					value_VCSEL_DC -= SVD_step;
				}
				if ( value_VCSEL_DC < option_VCSEL_DC){
					value_VCSEL_DC += SVD_step;
				}
				// Установили нужный ток
				if ( (value_VCSEL_DC >= (option_VCSEL_DC - SVD_step)) &&  
						 (value_VCSEL_DC <= (option_VCSEL_DC + SVD_step))){
					value_VCSEL_DC = option_VCSEL_DC;
					SVD_IsDone = true;
					itemStageWork_SET_VCSEL_DC = 2;
				}
				flag_VCSEL_DC = true;
			}
			break;
	}
}


// Установка рабочей СВЧ мощности
static inline void funWorkMode_SET_ATT_PWR(void){
	static uint16_t delay = 0;
	switch (itemStageWork_SET_ATT_PWR){
		case 0:
			itemStageWork_SET_ATT_PWR = 1;
			SATTP_IsDone = false;
			delay = 0;
			break;
		case 1:
			delay++;
			if ( delay == SATTP_tiksCount ){
				delay = 0;
				if ( value_RF_ATT > option_POWER_RF_ATT){
					value_RF_ATT -= SATTP_step;
				}
				if ( value_RF_ATT < option_POWER_RF_ATT){
					value_RF_ATT += SATTP_step;
				}
				// Установили нужную мощность СВЧ
				if ( (value_RF_ATT >= (option_POWER_RF_ATT - SATTP_step)) &&  
						 (value_RF_ATT <= (option_POWER_RF_ATT + SATTP_step))){
					value_RF_ATT = option_POWER_RF_ATT;
					fixValue_RF_ATT = value_RF_ATT;
					SATTP_IsDone = true;
					itemStageWork_SET_ATT_PWR = 2;
				}
				flag_RF_ATT = true;
			}
			break;
	}
}

static inline void search_zero_point(void){
	static int past_y = 0;
	static int past_x = 0;
	static char first_return = 1;
	if(first_return){
		first_return = 0;
		past_y = discr_wave.resume_dw;
		past_x = value_TEC_CTRL;
		return;
	}else{
		if (past_y*discr_wave.resume_dw < 0 && discr_wave.resume_dw < 0){
			itemModeWork = WORK_NONE;
			value_TEC_CTRL = (value_TEC_CTRL + past_x) / 2;
			statusLoopPID |= (1 << 0 | 1 << 10);
			perm_auto_capt = false;
			first_return = 1;
		}
		past_y = discr_wave.resume_dw;
		past_x = value_TEC_CTRL;
	}
	
}


/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
