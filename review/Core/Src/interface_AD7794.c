#include "interface_AD7794.h"

#define TMP_ADC_nCS_Pin GPIO_PIN_15
#define TMP_ADC_nCS_GPIO_Port GPIOA
#define TMP_ADC_SCK3_Pin GPIO_PIN_10
#define TMP_ADC_SCK3_GPIO_Port GPIOC
#define TMP_ADC_MISO3_Pin GPIO_PIN_11
#define TMP_ADC_MISO3_GPIO_Port GPIOC
#define TMP_ADC_MOSI3_Pin GPIO_PIN_12
#define TMP_ADC_MOSI3_GPIO_Port GPIOC

#define RF_SPI2_SCK_Pin GPIO_PIN_13
#define RF_SPI2_SCK_GPIO_Port GPIOB
#define RF_SPI2_MISO_Pin GPIO_PIN_14
#define RF_SPI2_MISO_GPIO_Port GPIOB
#define RF_SPI2_MOSI_Pin GPIO_PIN_15
#define RF_SPI2_MOSI_GPIO_Port GPIOB
#define RF_SPI_nCS0_Pin GPIO_PIN_7
#define RF_SPI_nCS0_GPIO_Port GPIOC

// Передача данных в AD7794 на верхней плате
void AD7794_UPR_SendData(uint8_t* sendData, uint8_t count){
	volatile uint8_t rx = 0;
	// Выключили SPI peripheral
	SPI2->CR1 &= ~SPI_CR1_SPE;
	// Настраиваем SPI2 для работы с АЦП
	// CPOL = 1
	SPI2->CR1 |= SPI_CR1_CPOL;
	// CPHA = 1
	SPI2->CR1 |= SPI_CR1_CPHA;
	// Включили SPI peripheral
	SPI2->CR1 |= SPI_CR1_SPE;

	// Выбор микросхемы АЦП
	HAL_GPIO_WritePin(RF_SPI_nCS0_GPIO_Port, RF_SPI_nCS0_Pin, GPIO_PIN_RESET);
	uint8_t i;
	for (i = 0; i < count; i++){
		// Ждем когда DR осовободится на запись
		while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
		*((__IO uint8_t *)&SPI2->DR) = sendData[i];
	}
	// Ожидаем завершение передачи данных
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	// Ожидание опустошения FIFO
	while (((SPI2->SR) & (SPI_SR_FTLVL)) != 0){};
	while (((SPI2->SR) & (SPI_SR_BSY)) == (SPI_SR_BSY)){};
	// Фиксируем данные
	HAL_GPIO_WritePin(RF_SPI_nCS0_GPIO_Port, RF_SPI_nCS0_Pin, GPIO_PIN_SET);
	SPI2->CR1 &= ~SPI_CR1_SPE;	
	while (((SPI2->SR) & (SPI_SR_FRLVL)) != 0){
		rx = *((__IO uint8_t *)&SPI2->DR);
	};
	
}

void AD7794_UPR_ReadData(uint8_t reg, uint8_t* pData, uint8_t count){
	volatile uint8_t rx = 0;
	// Настраиваем SPI2 для работы с АЦП
	// Выключили SPI peripheral
	SPI2->CR1 &= ~SPI_CR1_SPE;
	// Настраиваем SPI2 для работы с АЦП
	// CPOL = 1
	SPI2->CR1 |= SPI_CR1_CPOL;
	// CPHA = 1
	SPI2->CR1 |= SPI_CR1_CPHA;
	// Включили SPI peripheral
	SPI2->CR1 |= SPI_CR1_SPE;
	// Выбор микросхемы АЦП
	HAL_GPIO_WritePin(RF_SPI_nCS0_GPIO_Port, RF_SPI_nCS0_Pin, GPIO_PIN_RESET);
	// Ждем когда DR осовободится на запись
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	*((__IO uint8_t *)&SPI2->DR) = (reg | AD7794_CR_R_nW); 
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};	
	// Пустой байт
	*((__IO uint8_t *)&SPI2->DR) = 0;		
	// Ждем когда появятся данные на чтение		
	while((SPI2->SR & (SPI_FLAG_RXNE)) != (SPI_SR_RXNE)){};
	pData[0] = *((__IO uint8_t *)&SPI2->DR);	
	// Начинаем прием данных
	int i;	
	for (i = 0; i < count-1; i++){
		*((__IO uint8_t *)&SPI2->DR) = 0;		
		// Ждем когда появятся данные на чтение		
		while((SPI2->SR & (SPI_FLAG_RXNE)) != (SPI_SR_RXNE)){};
		pData[i] = *((__IO uint8_t *)&SPI2->DR);
	}	
	// Последний байт
	while((SPI2->SR & (SPI_FLAG_RXNE)) != (SPI_SR_RXNE)){};
	pData[count-1] = *((__IO uint8_t *)&SPI2->DR);
	
		// Ожидаем завершение передачи данных
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	// Ожидание опустошения FIFO
	while (((SPI2->SR) & (SPI_SR_FTLVL)) != 0){};
	while (((SPI2->SR) & (SPI_SR_BSY)) == (SPI_SR_BSY)){};
	// Завершаем чтение
	HAL_GPIO_WritePin(RF_SPI_nCS0_GPIO_Port, RF_SPI_nCS0_Pin, GPIO_PIN_SET);
	SPI2->CR1 &= ~SPI_CR1_SPE;	
	while (((SPI2->SR) & (SPI_SR_FRLVL)) != 0){
		rx = *((__IO uint8_t *)&SPI2->DR);
	};
}

// Передача данных в AD7794 на средней плате
void AD7794_MDL_SendData(uint8_t* sendData, uint8_t count){
	volatile uint8_t rx = 0;
	// Выключили SPI peripheral
	SPI3->CR1 &= ~SPI_CR1_SPE;
	// Настраиваем SPI3 для работы с АЦП
	// CPOL = 1
	SPI3->CR1 |= SPI_CR1_CPOL;
	// CPHA = 1
	SPI3->CR1 |= SPI_CR1_CPHA;
	// Включили SPI peripheral
	SPI3->CR1 |= SPI_CR1_SPE;

	// Выбор микросхемы АЦП
	HAL_GPIO_WritePin(TMP_ADC_nCS_GPIO_Port, TMP_ADC_nCS_Pin, GPIO_PIN_RESET);
	uint8_t i;
	for (i = 0; i < count; i++){
		// Ждем когда DR осовободится на запись
		while (((SPI3->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
		*((__IO uint8_t *)&SPI3->DR) = sendData[i];
	}
	// Ожидаем завершение передачи данных
	while (((SPI3->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	// Ожидание опустошения FIFO
	while (((SPI3->SR) & (SPI_SR_FTLVL)) != 0){};
	while (((SPI3->SR) & (SPI_SR_BSY)) == (SPI_SR_BSY)){};
	// Фиксируем данные
	HAL_GPIO_WritePin(TMP_ADC_nCS_GPIO_Port, TMP_ADC_nCS_Pin, GPIO_PIN_SET);
	SPI3->CR1 &= ~SPI_CR1_SPE;	
	while (((SPI3->SR) & (SPI_SR_FRLVL)) != 0){
		rx = *((__IO uint8_t *)&SPI3->DR);
	};
}

void AD7794_MDL_ReadData(uint8_t reg, uint8_t* pData, uint8_t count){
	volatile uint8_t rx = 0;
	// Настраиваем SPI3 для работы с АЦП
	// Выключили SPI peripheral
	SPI3->CR1 &= ~SPI_CR1_SPE;
	// Настраиваем SPI3 для работы с АЦП
	// CPOL = 1
	SPI3->CR1 |= SPI_CR1_CPOL;
	// CPHA = 1
	SPI3->CR1 |= SPI_CR1_CPHA;
	// Включили SPI peripheral
	SPI3->CR1 |= SPI_CR1_SPE;
	// Выбор микросхемы АЦП
	HAL_GPIO_WritePin(TMP_ADC_nCS_GPIO_Port, TMP_ADC_nCS_Pin, GPIO_PIN_RESET);
	// Ждем когда DR осовободится на запись
	while (((SPI3->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	*((__IO uint8_t *)&SPI3->DR) = (reg | AD7794_CR_R_nW); 
	while (((SPI3->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};	
	// Пустой байт
	*((__IO uint8_t *)&SPI3->DR) = 0;		
	// Ждем когда появятся данные на чтение		
	while((SPI3->SR & (SPI_FLAG_RXNE)) != (SPI_SR_RXNE)){};
	pData[0] = *((__IO uint8_t *)&SPI3->DR);	
	// Начинаем прием данных
	int i;	
	for (i = 0; i < count-1; i++){
		*((__IO uint8_t *)&SPI3->DR) = 0;		
		// Ждем когда появятся данные на чтение		
		while((SPI3->SR & (SPI_FLAG_RXNE)) != (SPI_SR_RXNE)){};
		pData[i] = *((__IO uint8_t *)&SPI3->DR);
	}	
	// Последний байт
	while((SPI3->SR & (SPI_FLAG_RXNE)) != (SPI_SR_RXNE)){};
	pData[count-1] = *((__IO uint8_t *)&SPI3->DR);
	
		// Ожидаем завершение передачи данных
	while (((SPI3->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	// Ожидание опустошения FIFO
	while (((SPI3->SR) & (SPI_SR_FTLVL)) != 0){};
	while (((SPI3->SR) & (SPI_SR_BSY)) == (SPI_SR_BSY)){};
	// Завершаем чтение
	HAL_GPIO_WritePin(TMP_ADC_nCS_GPIO_Port, TMP_ADC_nCS_Pin, GPIO_PIN_SET);
	SPI3->CR1 &= ~SPI_CR1_SPE;	
	while (((SPI3->SR) & (SPI_SR_FRLVL)) != 0){
		rx = *((__IO uint8_t *)&SPI3->DR);
	};
}
//=========================================================
// Настроили источник тока
//=========================================================
void AD7794mdl_Setup_IO(void){
	// Настроили источник тока 1->1 2->2 10uA
	uint8_t AD7794_RegIO[2];
	AD7794_RegIO[0] = AD7794_REG_IO;
	AD7794_RegIO[1] = AD7794_IO_IEXCEN_210uA; // PDA
	// Передача данных
	AD7794_MDL_SendData(AD7794_RegIO, 2);
}
void AD7794upr_Setup_IO(void){
	// Настроили источник тока 1->1 2->2 10uA
	uint8_t AD7794_RegIO[2];
	AD7794_RegIO[0] = AD7794_REG_IO;
	AD7794_RegIO[1] = AD7794_IO_IEXCEN_210uA; // PDA
	// Передача данных
	AD7794_UPR_SendData(AD7794_RegIO, 2);
}
// Температура ячейки
void AD7794mdl_Setup_CELL_TEMP(void){
	uint32_t data;
	// Настройка входа и опорного напряжения
	uint8_t AD7794_RegCFG[3];
	AD7794_RegCFG[0] = AD7794_CR_REG_CFG;
	data = AD7794_CFG_VBIAS_DISABLED | 
				 AD7794_CFG_GAIN1 |	
				 AD7794_CFG_UNI_nBIPOLAR | 
				 AD7794_CFG_REFSEL_REFIN1 | 
				 AD7794_CFG_REF_BUF | 
				 AD7794_CFG_CH1;
	AD7794_RegCFG[1] = ((data >> 8) & 0xFF);
	AD7794_RegCFG[2] = (data & 0xFF);
	// Передача данных
	AD7794_MDL_SendData(AD7794_RegCFG, 3);
	// Настройка скорости работы
	uint8_t AD7794_RegMR[3];
	AD7794_RegMR[0] = AD7794_REG_MR;
	data =	AD7794_MR_UPDATE_RATE_242Hz | 
					AD7794_MR_MODE_SINGLE_CONV | 
					AD7794_MR_CLK_INT_64K_PIN_DISABLED;
	AD7794_RegMR[1] = ((data >> 8) & 0xFF);
	AD7794_RegMR[2] = (data & 0xFF);
	// Передача данных
	AD7794_MDL_SendData(AD7794_RegMR, 3);
}

void AD7794mdl_Setup_ADC_TEMP(void){
	uint32_t data;
	// Настройка входа и опорного напряжения
	uint8_t AD7794_RegCFG[3];
	AD7794_RegCFG[0] = AD7794_CR_REG_CFG;
	data = AD7794_CFG_VBIAS_DISABLED | 
				 AD7794_CFG_GAIN1 |	
				 AD7794_CFG_UNI_nBIPOLAR | 
				 AD7794_CFG_REFSEL_INT1V17 | 
				 AD7794_CFG_REF_BUF | 
				 AD7794_CFG_CH_TEMP;
	AD7794_RegCFG[1] = ((data >> 8) & 0xFF);
	AD7794_RegCFG[2] = (data & 0xFF);
	// Передача данных
	AD7794_MDL_SendData(AD7794_RegCFG, 3);
	// Настройка скорости работы
	uint8_t AD7794_RegMR[3];
	AD7794_RegMR[0] = AD7794_REG_MR;
	data =	AD7794_MR_UPDATE_RATE_242Hz | 
					AD7794_MR_MODE_SINGLE_CONV | 
					AD7794_MR_CLK_INT_64K_PIN_DISABLED;
	AD7794_RegMR[1] = ((data >> 8) & 0xFF);
	AD7794_RegMR[2] = (data & 0xFF);
	// Передача данных
	AD7794_MDL_SendData(AD7794_RegMR, 3);
}


// Температура хвостика ячейки
void AD7794mdl_Setup_AUX2_TEMP(void){
	uint32_t data;
	// Настройка входа и опорного напряжения
	uint8_t AD7794_RegCFG[3];
	AD7794_RegCFG[0] = AD7794_CR_REG_CFG;
	data = AD7794_CFG_VBIAS_DISABLED | 
				 AD7794_CFG_GAIN1 |	
				 AD7794_CFG_UNI_nBIPOLAR | 
				 AD7794_CFG_REFSEL_REFIN2 | 
				 AD7794_CFG_REF_BUF | 
				 AD7794_CFG_CH2;
	AD7794_RegCFG[1] = ((data >> 8) & 0xFF);
	AD7794_RegCFG[2] = (data & 0xFF);
	// Передача данных
	AD7794_MDL_SendData(AD7794_RegCFG, 3);
	// Настройка скорости работы и запуск
	uint8_t AD7794_RegMR[3];
	AD7794_RegMR[0] = AD7794_REG_MR;
	data =	AD7794_MR_UPDATE_RATE_242Hz | 
					AD7794_MR_MODE_SINGLE_CONV | 
					AD7794_MR_CLK_INT_64K_PIN_DISABLED;
	AD7794_RegMR[1] = ((data >> 8) & 0xFF);
	AD7794_RegMR[2] = (data & 0xFF);
	// Передача данных
	AD7794_MDL_SendData(AD7794_RegMR, 3);
}

// Температура лазера
void AD7794upr_Setup_VCSEL_TEMP(void){
	uint32_t data;
	// Настройка входа и опорного напряжения
	uint8_t AD7794_RegCFG[3];
	AD7794_RegCFG[0] = AD7794_CR_REG_CFG;
	data = AD7794_CFG_VBIAS_DISABLED | 
				 AD7794_CFG_GAIN1 |	
				 AD7794_CFG_UNI_nBIPOLAR | 
				 AD7794_CFG_REFSEL_REFIN1 | 
				 AD7794_CFG_REF_BUF | 
				 AD7794_CFG_CH1;
	AD7794_RegCFG[1] = ((data >> 8) & 0xFF);
	AD7794_RegCFG[2] = (data & 0xFF);
	// Передача данных
	AD7794_UPR_SendData(AD7794_RegCFG, 3);
	// Настройка скорости работы  и запуск
	uint8_t AD7794_RegMR[3];
	AD7794_RegMR[0] = AD7794_REG_MR;
	data =	AD7794_MR_UPDATE_RATE_242Hz | 
					AD7794_MR_MODE_SINGLE_CONV | 
					AD7794_MR_CLK_INT_64K_PIN_DISABLED;
	AD7794_RegMR[1] = ((data >> 8) & 0xFF);
	AD7794_RegMR[2] = (data & 0xFF);
	// Передача данных
	AD7794_UPR_SendData(AD7794_RegMR, 3);
}

// Температура СВЧ генератора
void AD7794upr_Setup_AUX1_TEMP(void){
	uint32_t data;
	// Настройка входа и опорного напряжения
	uint8_t AD7794_RegCFG[3];
	AD7794_RegCFG[0] = AD7794_CR_REG_CFG;
	data = AD7794_CFG_VBIAS_DISABLED | 
				 AD7794_CFG_GAIN1 |	
				 AD7794_CFG_UNI_nBIPOLAR | 
				 AD7794_CFG_REFSEL_REFIN2 | 
				 AD7794_CFG_REF_BUF | 
				 AD7794_CFG_CH2;
	AD7794_RegCFG[1] = ((data >> 8) & 0xFF);
	AD7794_RegCFG[2] = (data & 0xFF);
	// Передача данных
	AD7794_UPR_SendData(AD7794_RegCFG, 3);
	// Настройка скорости работы  и запуск
	uint8_t AD7794_RegMR[3];
	AD7794_RegMR[0] = AD7794_REG_MR;
	data =	AD7794_MR_UPDATE_RATE_242Hz | 
					AD7794_MR_MODE_SINGLE_CONV | 
					AD7794_MR_CLK_INT_64K_PIN_DISABLED;
	AD7794_RegMR[1] = ((data >> 8) & 0xFF);
	AD7794_RegMR[2] = (data & 0xFF);
	// Передача данных
	AD7794_UPR_SendData(AD7794_RegMR, 3);
}


void AD7794upr_Setup_ADC_TEMP(void){
	uint32_t data;
	// Настройка входа и опорного напряжения
	uint8_t AD7794_RegCFG[3];
	AD7794_RegCFG[0] = AD7794_CR_REG_CFG;
	data = AD7794_CFG_VBIAS_DISABLED | 
				 AD7794_CFG_GAIN1 |	
				 AD7794_CFG_UNI_nBIPOLAR | 
				 AD7794_CFG_REFSEL_INT1V17 | 
				 AD7794_CFG_REF_BUF | 
				 AD7794_CFG_CH_TEMP;
	AD7794_RegCFG[1] = ((data >> 8) & 0xFF);
	AD7794_RegCFG[2] = (data & 0xFF);
	// Передача данных
	AD7794_UPR_SendData(AD7794_RegCFG, 3);
	
	// Настройка скорости работы  и запуск
	uint8_t AD7794_RegMR[3];
	AD7794_RegMR[0] = AD7794_REG_MR;
	data =	AD7794_MR_UPDATE_RATE_242Hz | 
					AD7794_MR_MODE_SINGLE_CONV | 
					AD7794_MR_CLK_INT_64K_PIN_DISABLED;
	AD7794_RegMR[1] = ((data >> 8) & 0xFF);
	AD7794_RegMR[2] = (data & 0xFF);
	// Передача данных
	AD7794_UPR_SendData(AD7794_RegMR, 3);
}

bool AD7794upr_GetFlagCompleted(void){
	bool result = false;
	uint8_t data[1];
	uint8_t reg = AD7794_REG_rSTAT;
	AD7794_UPR_ReadData(reg, data, 1);
	result = ((bool)((data[0] & AD7794_STAT_nRDY) == AD7794_STAT_nRDY));
	if (result == true){
		return false;
	}else{
		return true;
	}
}

bool AD7794mdl_GetFlagCompleted(void){
	bool result = false;
	uint8_t data[1];
	uint8_t reg = AD7794_REG_rSTAT;
	AD7794_MDL_ReadData(reg, data, 1);
	result = ((bool)((data[0] & AD7794_STAT_nRDY) == AD7794_STAT_nRDY));
	if (result == true){
		return false;
	}else{
		return true;
	}
}

int AD7794upr_GetValue(void){
	uint8_t reg = AD7794_REG_DATA | AD7794_CR_R_nW;
	uint8_t data[3];
	// Чтение регистра reg
	AD7794_UPR_ReadData(reg, &data[0], 3);
	int result = (data[0] << 16) | (data[1] << 8) | (data[2]);
	return result;	
}

int AD7794mdl_GetValue(void){
	uint8_t reg = AD7794_REG_DATA | AD7794_CR_R_nW;
	uint8_t data[3];
	// Чтение регистра reg
	AD7794_MDL_ReadData(reg, &data[0], 3);
	int result = (data[0] << 16) | (data[1] << 8) | (data[2]);
	return result;	
}


// Сброс на значения по умолчанию
void AD7794_MDL_Reset(void){
	uint8_t resetData[4] = {0xFF,0xFF,0xFF,0xFF};
	AD7794_MDL_SendData(&resetData[0], 4);
	// 500 uSec не доступен
	//#warning "Not work Delay"
	//HAL_Delay(2);
}

// Сброс на значения по умолчанию
void AD7794_UPR_Reset(void){
	uint8_t resetData[4] = {0xFF,0xFF,0xFF,0xFF};
	AD7794_UPR_SendData(&resetData[0], 4);
	// 500 uSec не доступен
	//#warning "Not work Delay"
	//HAL_Delay(2);
}
void AD7794_Init(void){
	// Сбросили две микросхемы
	AD7794_MDL_Reset();
	AD7794_MDL_Reset();
	AD7794_UPR_Reset();
	AD7794_UPR_Reset();
	// Настроили источник тока
	AD7794mdl_Setup_IO();
	AD7794upr_Setup_IO();
}

float ConvertToTempADC(int value){
	return ((((float)value)*1.17f/16777215.0f)/0.81f*1000.0f - 273.0f);
}

// Перевод в градусы цельсия датчика ячейки
float ConvertToTempNTC_CELL(int value, float resist){
// Каллибровочные значения NTC Датчика
	static const float tableFactorNTC_CELL[30][2] = 	{	
				{-55.0f, 52.6240f}, {-50.0f, 38.4520f}, {-45.0f, 28.4010f}, 
				{-40.0f, 21.1940f}, {-35.0f, 15.9720f}, {-30.0f, 12.1490f}, 
				{-25.0f, 9.32460f}, {-20.0f, 7.21810f}, {-15.0f, 5.63320f}, 
				{-10.0f, 4.43080f}, {-5.0f, 3.51120f}, 	{0.0f,  2.80240f}, 
				{5.0f,  2.25200f}, 	{10.0f, 1.82160f}, 	{15.0f, 1.48270f}, 
				{20.0f, 1.21420f}, 	{25.0f, 1.00000f}, 	{30.0f, 0.82818f}, 
				{35.0f, 0.68954f}, 	{40.0f, 0.57703f}, 	{45.0f, 0.48525f}, 
				{50.0f, 0.41000f}, 	{55.0f, 0.34798f}, 	{60.0f, 0.29663f},
				{65.0f, 0.25392f}, 	{70.0f, 0.21824f}, 	{75.0f, 0.18830f}, 
				{80.0f, 0.16307f}, 	{85.0f, 0.14174f}, 	{90.0f, 0.12362f}};
	float result = 90.0;
	// Определяем изменение NTC резистора от эталоного значения
	resist /= 10000.0f;
	float factorNTC = (float)(((double)value * (double)resist) / 16777215.0);
	// Находим ближайшее соответствующие значение из таблицы
	uint8_t i;	
	for ( i = 1; i < 30; i++){
		if ( tableFactorNTC_CELL[i][1] <= factorNTC){
			// Линейно апроксимируем с шагом 5 градусов
			float k = 5.0f / (tableFactorNTC_CELL[i][1] - tableFactorNTC_CELL[i-1][1]);
			float b = tableFactorNTC_CELL[i][0] - k*tableFactorNTC_CELL[i][1];
			result = k * factorNTC + b;
			break;
		}
	}
	return result;
}

// Перевод в градусы цельсия датчика ячейки
float ConvertToTempNTC_AUX1(int value, float resist){
// Каллибровочные значения NTC Датчика
	static const float tableFactorNTC_AUX1[30][2] = 	{	
				{-55.0f, 52.6240f}, {-50.0f, 38.4520f}, {-45.0f, 28.4010f}, 
				{-40.0f, 21.1940f}, {-35.0f, 15.9720f}, {-30.0f, 12.1490f}, 
				{-25.0f, 9.32460f}, {-20.0f, 7.21810f}, {-15.0f, 5.63320f}, 
				{-10.0f, 4.43080f}, {-5.0f, 3.51120f}, 	{0.0f,  2.80240f}, 
				{5.0f,  2.25200f}, 	{10.0f, 1.82160f}, 	{15.0f, 1.48270f}, 
				{20.0f, 1.21420f}, 	{25.0f, 1.00000f}, 	{30.0f, 0.82818f}, 
				{35.0f, 0.68954f}, 	{40.0f, 0.57703f}, 	{45.0f, 0.48525f}, 
				{50.0f, 0.41000f}, 	{55.0f, 0.34798f}, 	{60.0f, 0.29663f},
				{65.0f, 0.25392f}, 	{70.0f, 0.21824f}, 	{75.0f, 0.18830f}, 
				{80.0f, 0.16307f}, 	{85.0f, 0.14174f}, 	{90.0f, 0.12362f}};
	float result = 90.0;
	// Определяем изменение NTC резистора от эталоного значения
	resist /= 10000.0f;
	float factorNTC = (float)(((double)value * (double)resist) / 16777215.0);
	// Находим ближайшее соответствующие значение из таблицы
	uint8_t i;	
	for ( i = 1; i < 30; i++){
		if ( tableFactorNTC_AUX1[i][1] <= factorNTC){
			// Линейно апроксимируем с шагом 5 градусов
			float k = 5.0f / (tableFactorNTC_AUX1[i][1] - tableFactorNTC_AUX1[i-1][1]);
			float b = tableFactorNTC_AUX1[i][0] - k*tableFactorNTC_AUX1[i][1];
			result = k * factorNTC + b;
			break;
		}
	}
	return result;
}

// Перевод в градусы цельсия датчика ячейки
float ConvertToTempNTC_AUX2(int value, float resist){
// Каллибровочные значения NTC Датчика
	static const float tableFactorNTC_AUX2[30][2] = 	{	
				{-55.0f, 52.6240f}, {-50.0f, 38.4520f}, {-45.0f, 28.4010f}, 
				{-40.0f, 21.1940f}, {-35.0f, 15.9720f}, {-30.0f, 12.1490f}, 
				{-25.0f, 9.32460f}, {-20.0f, 7.21810f}, {-15.0f, 5.63320f}, 
				{-10.0f, 4.43080f}, {-5.0f, 3.51120f}, 	{0.0f,  2.80240f}, 
				{5.0f,  2.25200f}, 	{10.0f, 1.82160f}, 	{15.0f, 1.48270f}, 
				{20.0f, 1.21420f}, 	{25.0f, 1.00000f}, 	{30.0f, 0.82818f}, 
				{35.0f, 0.68954f}, 	{40.0f, 0.57703f}, 	{45.0f, 0.48525f}, 
				{50.0f, 0.41000f}, 	{55.0f, 0.34798f}, 	{60.0f, 0.29663f},
				{65.0f, 0.25392f}, 	{70.0f, 0.21824f}, 	{75.0f, 0.18830f}, 
				{80.0f, 0.16307f}, 	{85.0f, 0.14174f}, 	{90.0f, 0.12362f}};
	float result = 90.0;
	// Определяем изменение NTC резистора от эталоного значения
	resist /= 10000.0f;
	float factorNTC = (float)(((double)value * (double)resist) / 16777215.0);
	// Находим ближайшее соответствующие значение из таблицы
	uint8_t i;	
	for ( i = 1; i < 30; i++){
		if ( tableFactorNTC_AUX2[i][1] <= factorNTC){
			// Линейно апроксимируем с шагом 5 градусов
			float k = 5.0f / (tableFactorNTC_AUX2[i][1] - tableFactorNTC_AUX2[i-1][1]);
			float b = tableFactorNTC_AUX2[i][0] - k*tableFactorNTC_AUX2[i][1];
			result = k * factorNTC + b;
			break;
		}
	}
	return result;
}


// Перевод в градусы цельсия датчика лазера
float ConvertToTempNTC_VCSEL(int value, float resist){
// Каллибровочные значения NTC Датчика
	static const float tableFactorNTC_VCLES[30][2] = 	{	
				{-55.0f, 120.6397f}, 	{-50.0f, 80.8433f}, {-45.0f, 55.1343f}, 
				{-40.0f, 38.2239f}, 	{-35.0f, 26.9111f}, {-30.0f, 19.2220f}, 
				{-25.0f, 13.9175f}, 	{-20.0f, 10.2062f}, {-15.0f, 7.5752f}, 
				{-10.0f, 5.6864f}, 		{-5.0f, 4.3145f}, 	{0.0f,  3.3036f}, 
				{5.0f,  2.5589f}, 		{10.0f, 1.9982f}, 	{15.0f, 1.5738f}, 
				{20.0f, 1.2497f}, 		{25.0f, 1.00000f}, 	{30.0f, 0.80612f}, 
				{35.0f, 0.654400f}, 	{40.0f, 0.53478f}, 	{45.0f, 0.43981f}, 
				{50.0f, 0.36390f}, 		{55.0f, 0.30284f}, 	{60.0f, 0.25341f},
				{65.0f, 0.21318f}, 		{70.0f, 0.18024f}, 	{75.0f, 0.15313f}, 
				{80.0f, 0.13069f}, 		{85.0f, 0.11204f}, 	{90.0f, 0.09646f}};
	float result = 90.0;
	// Определяем изменение NTC резистора от эталоного значения
	resist /= 10000.0f;
	float factorNTC = (float)(((double)value * (double)resist) / 16777215.0);
	// Находим ближайшее соответствующие значение из таблицы
	uint8_t i;	
	for ( i = 1; i < 30; i++){
		if ( tableFactorNTC_VCLES[i][1] <= factorNTC){
			// Линейно апроксимируем с шагом 5 градусов
			float k = 5.0f / (tableFactorNTC_VCLES[i][1] - tableFactorNTC_VCLES[i-1][1]);
			//[0]-y [1]-x
			float b = tableFactorNTC_VCLES[i][0] - k*tableFactorNTC_VCLES[i][1];
			result = k * factorNTC + b;
			break;
		}
	}
	return result;
}

