#include "interface_AD5668.h"

#define SPI2_SCK_RF_Pin GPIO_PIN_13
#define SPI2_SCK_RF_GPIO_Port GPIOB
#define SPI2_MISO_RF_Pin GPIO_PIN_14
#define SPI2_MISO_RF_GPIO_Port GPIOB
#define SPI2_MOSI_RF_Pin GPIO_PIN_15
#define SPI2_MOSI_RF_GPIO_Port GPIOB


#define DAC_nCS_Pin GPIO_PIN_8
#define DAC_nCS_GPIO_Port GPIOA


void AD5668_Init(void){
	// Сброс
	AD5668_Reset();
	HAL_Delay(100);
	// Настраиваем SPI2 для работы с ЦАПом
	// Disable SPI peripheral
	SPI2->CR1 &= ~SPI_CR1_SPE;
	// Настраиваем SPI2 для работы с ЦАПом
	// CPOL = 1
	SPI2->CR1 |= SPI_CR1_CPOL;
	// CPHA = 0
	SPI2->CR1 &= ~SPI_CR1_CPHA;	
	// Enable SPI peripheral
	SPI2->CR1 |= SPI_CR1_SPE;
	// Сброс на значения по умолчанию
		
	uint32_t sendData = AD5668_REF | AD5668_REF_ON;
	static uint8_t data[4];
	data[0] = ((sendData >> 24) & 0xFF);
	data[1] = ((sendData >> 16) & 0xFF);
	data[2] = ((sendData >> 8) & 0xFF);
	data[3] = ((sendData) & 0xFF);
	// Выбор микросхемы ЦАПа 
	HAL_GPIO_WritePin(DAC_nCS_GPIO_Port, DAC_nCS_Pin, GPIO_PIN_RESET);
	// Ждем когда DR осовободится на запись
  while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
  *((__IO uint8_t *)&SPI2->DR) = data[0];
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	*((__IO uint8_t *)&SPI2->DR) = data[1];
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
  *((__IO uint8_t *)&SPI2->DR) = data[2];
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	*((__IO uint8_t *)&SPI2->DR) = data[3];
	// Ожидаем завершение передачи данных
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	// Ожидание опустошения FIFO
	while (((SPI2->SR) & (SPI_SR_FTLVL)) != 0){};
	while (((SPI2->SR) & (SPI_SR_BSY)) == (SPI_SR_BSY)){}; 
	// Фиксируем данные
	HAL_GPIO_WritePin(DAC_nCS_GPIO_Port, DAC_nCS_Pin, GPIO_PIN_SET);
	
	// 100KOm
	sendData = AD5668_POWER_DOWN | AD5668_PD_ALL;
	data[0] = ((sendData >> 24) & 0xFF);
	data[1] = ((sendData >> 16) & 0xFF);
	data[2] = ((sendData >> 8) & 0xFF);
	data[3] = ((sendData) & 0xFF);
	// Выбор микросхемы ЦАПа 
	HAL_GPIO_WritePin(DAC_nCS_GPIO_Port, DAC_nCS_Pin, GPIO_PIN_RESET);
	// Ждем когда DR осовободится на запись
  while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
  *((__IO uint8_t *)&SPI2->DR) = data[0];
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	*((__IO uint8_t *)&SPI2->DR) = data[1];
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
  *((__IO uint8_t *)&SPI2->DR) = data[2];
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	*((__IO uint8_t *)&SPI2->DR) = data[3];
	// Ожидаем завершение передачи данных
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	// Ожидание опустошения FIFO
	while (((SPI2->SR) & (SPI_SR_FTLVL)) != 0){};
	while (((SPI2->SR) & (SPI_SR_BSY)) == (SPI_SR_BSY)){}; 
	// Фиксируем данные
	HAL_GPIO_WritePin(DAC_nCS_GPIO_Port, DAC_nCS_Pin, GPIO_PIN_SET);		
		
		
	// Асинхронный режим (синхронизация по внешнему сигналу LDAC)
	sendData = AD5668_LDAC;
	data[0] = ((sendData >> 24) & 0xFF);
	data[1] = ((sendData >> 16) & 0xFF);
	data[2] = ((sendData >> 8) & 0xFF);
	data[3] = ((sendData) & 0xFF);
	// Выбор микросхемы ЦАПа 
	HAL_GPIO_WritePin(DAC_nCS_GPIO_Port, DAC_nCS_Pin, GPIO_PIN_RESET);
	// Ждем когда DR осовободится на запись
  while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
  *((__IO uint8_t *)&SPI2->DR) = data[0];
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	*((__IO uint8_t *)&SPI2->DR) = data[1];
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
  *((__IO uint8_t *)&SPI2->DR) = data[2];
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	*((__IO uint8_t *)&SPI2->DR) = data[3];
	// Ожидаем завершение передачи данных
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	// Ожидание опустошения FIFO
	while (((SPI2->SR) & (SPI_SR_FTLVL)) != 0){};
	while (((SPI2->SR) & (SPI_SR_BSY)) == (SPI_SR_BSY)){}; 
	// Фиксируем данные
	HAL_GPIO_WritePin(DAC_nCS_GPIO_Port, DAC_nCS_Pin, GPIO_PIN_SET);
	while (((SPI2->SR) & (SPI_SR_FRLVL)) != 0){
		*((__IO uint8_t *)&SPI2->DR);
	};		
	
}
// Работаем с внешним ЦАПом по SPI2
void AD5668_SetValue(uint32_t chanel, int value){
	if (value < 0){value = 0;}
	value = value & 0xFFFF;			
	
	// Disable SPI peripheral
	SPI2->CR1 &= ~SPI_CR1_SPE;
	// Настраиваем SPI2 для работы с ЦАПом
	// CPOL = 1
	SPI2->CR1 |= SPI_CR1_CPOL;
	// CPHA = 0
	SPI2->CR1 &= ~SPI_CR1_CPHA;	
	// Enable SPI peripheral
	SPI2->CR1 |= SPI_CR1_SPE;
 	static uint32_t sendData = 0; 
	sendData = AD5668_WRITE_AND_UPDATE_N | chanel | AD5668_VALUE(value);
	static uint8_t data[4];
	data[0] = ((sendData >> 24) & 0xFF);
	data[1] = ((sendData >> 16) & 0xFF);
	data[2] = ((sendData >> 8) & 0xFF);
	data[3] = ((sendData) & 0xFF);	
	// Выбор микросхемы ЦАПа 
	HAL_GPIO_WritePin(DAC_nCS_GPIO_Port, DAC_nCS_Pin, GPIO_PIN_RESET);
	// Ждем когда DR осовободится на запись
  while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
  *((__IO uint8_t *)&SPI2->DR) = data[0];
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	*((__IO uint8_t *)&SPI2->DR) = data[1];
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
  *((__IO uint8_t *)&SPI2->DR) = data[2];
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	*((__IO uint8_t *)&SPI2->DR) = data[3];
	// Ожидаем завершение передачи данных
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	// Ожидание опустошения FIFO
	while (((SPI2->SR) & (SPI_SR_FTLVL)) != 0){};
	while (((SPI2->SR) & (SPI_SR_BSY)) == (SPI_SR_BSY)){}; 
	// Фиксируем данные
	HAL_GPIO_WritePin(DAC_nCS_GPIO_Port, DAC_nCS_Pin, GPIO_PIN_SET);	
	while (((SPI2->SR) & (SPI_SR_FRLVL)) != 0){
		*((__IO uint8_t *)&SPI2->DR);
	};

}


// Работаем с внешним ЦАПом по SPI2
void AD5668_WriteValue(uint32_t chanel, int value){

	if (value < 0){value = 0;}
	value = value & 0xFFFF;		
	
	// Disable SPI peripheral
	SPI2->CR1 &= ~SPI_CR1_SPE;
	// Настраиваем SPI2 для работы с ЦАПом
	// CPOL = 1
	SPI2->CR1 |= SPI_CR1_CPOL;
	// CPHA = 0
	SPI2->CR1 &= ~SPI_CR1_CPHA;	
	// Enable SPI peripheral
	SPI2->CR1 |= SPI_CR1_SPE;
 
	static uint32_t sendData = 0; 
	sendData = AD5668_WRITE_N | chanel | AD5668_VALUE(value);
	static uint8_t data[4];
	data[0] = ((sendData >> 24) & 0xFF);
	data[1] = ((sendData >> 16) & 0xFF);
	data[2] = ((sendData >> 8) & 0xFF);
	data[3] = ((sendData) & 0xFF);	
	// Выбор микросхемы ЦАПа 
	HAL_GPIO_WritePin(DAC_nCS_GPIO_Port, DAC_nCS_Pin, GPIO_PIN_RESET);
	// Ждем когда DR осовободится на запись
  while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
  *((__IO uint8_t *)&SPI2->DR) = data[0];
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	*((__IO uint8_t *)&SPI2->DR) = data[1];
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
  *((__IO uint8_t *)&SPI2->DR) = data[2];
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	*((__IO uint8_t *)&SPI2->DR) = data[3];
	// Ожидаем завершение передачи данных
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	// Ожидание опустошения FIFO
	while (((SPI2->SR) & (SPI_SR_FTLVL)) != 0){};
	while (((SPI2->SR) & (SPI_SR_BSY)) == (SPI_SR_BSY)){}; 
	// Фиксируем данные
	HAL_GPIO_WritePin(DAC_nCS_GPIO_Port, DAC_nCS_Pin, GPIO_PIN_SET);	
	SPI2->CR1 &= ~SPI_CR1_SPE;	
	while (((SPI2->SR) & (SPI_SR_FRLVL)) != 0){
		*((__IO uint8_t *)&SPI2->DR);
	};
}

// Сброс значений микросхем поумолчанию
void AD5668_Reset(void){
	volatile uint8_t rx = 0;
	// Настраиваем SPI2 для работы с ЦАПом
	// Disable SPI peripheral
	SPI2->CR1 &= ~SPI_CR1_SPE;
	// Настраиваем SPI2 для работы с ЦАПом
	// CPOL = 1
	SPI2->CR1 |= SPI_CR1_CPOL;
	// CPHA = 0
	SPI2->CR1 &= ~SPI_CR1_CPHA;	
	// Enable SPI peripheral
	SPI2->CR1 |= SPI_CR1_SPE;
	// Сброс на значения по умолчанию
	uint32_t sendData = AD5668_RESET;
	static uint8_t data[4];
	data[0] = ((sendData >> 24) & 0xFF);
	data[1] = ((sendData >> 16) & 0xFF);
	data[2] = ((sendData >> 8) & 0xFF);
	data[3] = ((sendData >> 0) & 0xFF);
	// Выбор микросхемы ЦАПа 
	HAL_GPIO_WritePin(DAC_nCS_GPIO_Port, DAC_nCS_Pin, GPIO_PIN_RESET);
	// Ждем когда DR осовободится на запись
  while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
  *((__IO uint8_t *)&SPI2->DR) = data[0];
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	*((__IO uint8_t *)&SPI2->DR) = data[1];
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
  *((__IO uint8_t *)&SPI2->DR) = data[2];
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	*((__IO uint8_t *)&SPI2->DR) = data[3];
	// Ожидаем завершение передачи данных
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	// Ожидание опустошения FIFO
	while (((SPI2->SR) & (SPI_SR_FTLVL)) != 0){};
	while (((SPI2->SR) & (SPI_SR_BSY)) == (SPI_SR_BSY)){}; 
	// Фиксируем данные
	HAL_GPIO_WritePin(DAC_nCS_GPIO_Port, DAC_nCS_Pin, GPIO_PIN_SET);	
	while (((SPI2->SR) & (SPI_SR_FRLVL)) != 0){
		rx = *((__IO uint8_t *)&SPI2->DR);
	};
}

