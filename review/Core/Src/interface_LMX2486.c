#include "interface_LMX2486.h"

#define SYNT_LE_Pin GPIO_PIN_8
#define SYNT_LE_GPIO_Port GPIOC

// Запись данных в LMX2486 по SPI2
void LMX2486_SendData(uint32_t sendData){
	volatile uint8_t rx = 0;
	// Настраиваем SPI2 для работы с синтезатором
	// Disable SPI peripheral
	SPI2->CR1 &= ~SPI_CR1_SPE;
	// Настраиваем SPI2 для работы с ЦАПом
	// CPOL = 0
	SPI2->CR1 &= ~SPI_CR1_CPOL;
	// CPHA = 0
	SPI2->CR1 &= ~SPI_CR1_CPHA;
	// Enable SPI peripheral
	SPI2->CR1 |= SPI_CR1_SPE;
	// Выбор микросхемы синтезатора
	HAL_GPIO_WritePin(SYNT_LE_GPIO_Port, SYNT_LE_Pin, GPIO_PIN_RESET);
	// Ждем когда DR осовободится на запись
  while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
  *((__IO uint8_t *)&SPI2->DR) = ((sendData >> 16) & 0xFF);
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	*((__IO uint8_t *)&SPI2->DR) = ((sendData >> 8) & 0xFF);
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	*((__IO uint8_t *)&SPI2->DR) = ((sendData >> 0) & 0xFF);
	// Ожидаем завершение передачи данных
	while (((SPI2->SR) & (SPI_SR_TXE)) != (SPI_SR_TXE)){};
	// Ожидание опустошения FIFO
	while (((SPI2->SR) & (SPI_SR_FTLVL)) != 0){};
	while (((SPI2->SR) & (SPI_SR_BSY)) == (SPI_SR_BSY)){};
	// Фиксируем данные
	HAL_GPIO_WritePin(SYNT_LE_GPIO_Port, SYNT_LE_Pin, GPIO_PIN_SET);
	SPI2->CR1 &= ~SPI_CR1_SPE;	
	while (((SPI2->SR) & (SPI_SR_FRLVL)) != 0){
		rx = *((__IO uint8_t *)&SPI2->DR);
	};
}



// Инициализация микросхемы синтезатора
void LMX2486_Init(void){
	// Целоеъ
	int valueC = (int)(LMX_ValueN / 32);
	int valueB = (int)((LMX_ValueN - valueC * 32) / 4);
	int valueA = (int)(LMX_ValueN - valueC * 32 - valueB * 4);
	
	uint32_t RF_N_Reg0 =	LMX2486_R0_RF_C(valueC) |
												LMX2486_R0_RF_B(valueB) |
												LMX2486_R0_RF_A(valueA);
	// Числитель
	uint32_t RF_FN =  LMX_ValueFN;
	uint32_t RF_FN_Reg5 = (((RF_FN >> 12) & 0x03FF) << 4);
	uint32_t RF_FN_Reg0 = ((RF_FN & 0x0FFF) << 1);
	// Знаменатель
	uint32_t RF_FD = LMX_ValueFD;
	uint32_t RF_FD_Reg5 = (((RF_FD >> 12) & 0x03FF) << 14);
	uint32_t RF_FD_Reg1 = ((RF_FD & 0xFFF) << 4); // 0xE00
	//=======================================================
	// REG 5
	//=======================================================
	uint32_t lmx2486Reg5 = 	LMX2486_REG_R5 | 
													RF_FN_Reg5 | 
													RF_FD_Reg5;
	//=======================================================
	// REG 1
	//=======================================================
	// RF_R(5) RF_P(32)
	uint32_t lmx2486Reg1 = LMX2486_REG_R1 | 
												 LMX2486_R1_RF_R(2) | 
												 LMX2486_R1_RF_P | 
												 RF_FD_Reg1;								 
	//=======================================================
	// REG 2
	//=======================================================
	// Отключаем IF PLL
	uint32_t lmx2486Reg2 = LMX2486_REG_R2 | 
												 LMX2486_R2_IF_PD;
	//=======================================================
	// REG 3
	//=======================================================
	// 10X 	950 	uA
	uint32_t lmx2486Reg3 = LMX2486_REG_R3 | 
												 LMX2486_R3_ACCESS_0_SET | 
												 LMX2486_R3_ACCESS_1_SET |
												 LMX2486_R3_RF_CPG(LMX_Current_PLL);
	//=======================================================
	// REG 4
	//=======================================================	
	uint32_t lmx2486Reg4 = 	LMX2486_REG_R4 | LMX2486_R4_R4 |
													LMX2486_R4_RF_CP_POSITIVE | 
													LMX2486_R4_OSC_OUT_OFF | 
													LMX2486_R4_FM_MODE_3X | 
													LMX2486_R4_DITH_OFF | 
													LMX2486_R4_MUX_RF_N_DIV2;
	//=======================================================
	// REG 0
	//=======================================================	
	uint32_t lmx2486Reg0 = 	LMX2486_REG_R0 | 
													RF_N_Reg0 |
													RF_FN_Reg0;
	// Передаем данные в микросхему R0 регистр заполняем последнимJ
	LMX2486_SendData(lmx2486Reg3);
	LMX2486_SendData(lmx2486Reg5);
	LMX2486_SendData(lmx2486Reg1);
	LMX2486_SendData(lmx2486Reg2);
	LMX2486_SendData(lmx2486Reg4);
	LMX2486_SendData(lmx2486Reg0);

}

void LMX2486_SetFreq(uint32_t valueN, uint32_t valueFN, uint32_t valueFD){
	// Целое
	int valueC = (int)(valueN / 32);
	int valueB = (int)((valueN - valueC * 32) / 4);
	int valueA = (int)(valueN - valueC * 32 - valueB * 4);
	
	uint32_t RF_N_Reg0 =	LMX2486_R0_RF_C(valueC) |
												LMX2486_R0_RF_B(valueB) |
												LMX2486_R0_RF_A(valueA);
	// Числитель
	uint32_t RF_FN = valueFN;
	uint32_t RF_FN_Reg5 = (((RF_FN >> 12) & 0x03FF) << 4);
	uint32_t RF_FN_Reg0 = ((RF_FN & 0x0FFF) << 1);
	// Знаменатель
	uint32_t RF_FD = valueFD;
	uint32_t RF_FD_Reg5 = (((RF_FD >> 12) & 0x03FF) << 14);
	uint32_t RF_FD_Reg1 = ((RF_FD & 0xFFF) << 4); // 0xE00
	uint32_t lmx2486Reg5 = 	LMX2486_REG_R5 | 
													RF_FN_Reg5 | 
													RF_FD_Reg5;	
	uint32_t lmx2486Reg1 = LMX2486_REG_R1 | 
												 LMX2486_R1_RF_R(2) | 
												 LMX2486_R1_RF_P | 
												 RF_FD_Reg1;													
	uint32_t lmx2486Reg0 = 	LMX2486_REG_R0 | 
													RF_N_Reg0 |
													RF_FN_Reg0;
													
	uint32_t lmx2486Reg3 = LMX2486_REG_R3 | 
												 LMX2486_R3_ACCESS_0_SET | 
												 LMX2486_R3_ACCESS_1_SET |
												 LMX2486_R3_RF_CPG(LMX_Current_PLL);
												 
	LMX2486_SendData(lmx2486Reg3);
	
	LMX2486_SendData(lmx2486Reg5);
	LMX2486_SendData(lmx2486Reg1);
	LMX2486_SendData(lmx2486Reg0);
	
}


void CamputeOptionSetValueLMX(int value){
	// (1/СВЧ * 1E+13) (большое число делим на большое число) = 2175.656
	double dblValue = (double)(value)/2175.656;
	double delta = dblValue;
	double centreFreq = 4596315885.006 + (double)LMX_ShiftFreq;
	double realValue = centreFreq + delta;
	CamputeOptionLMX(realValue, (double)LMX_ValueN, &LMX_ValueFN, &LMX_ValueFD);
	flag_LMX248 = true;
}

void CamputeOptionAddValueLMX(int value){
	// (1/СВЧ * 1E+13) (большое число делим на большое число) = 2175.656
	double dblValue = (double)(LMX_ValueINT + value)/2175.656; 
	LMX_ValueINT += value;
	double delta = dblValue;
	double centreFreq = 4596315885.0 + (double)LMX_ShiftFreq;
	double realValue = centreFreq + delta;
	CamputeOptionLMX(realValue, (double)LMX_ValueN, &LMX_ValueFN, &LMX_ValueFD);
	flag_LMX248 = true;
}

void CamputeOptionDataLMX(int value){
	// (1/СВЧ * 1E+13) (большое число делим на большое число) = 2175.656
	double dblValue = (double)(value)/2175.656;
	LMX_ValueINT = value;
	double delta = dblValue;
	double centreFreq = 4596315885.0 + (double)LMX_ShiftFreq;
	double realValue = centreFreq + delta;
	for (int i = 0; i < 256; i++){
		delta	= (double)(i - 128)*0.005;
		realValue = centreFreq + delta;
		CamputeOptionLMX(realValue, (double)LMX_ValueN, &LMX_DataValueFN[i], &LMX_DataValueFD[i]);
	}
}

void CamputeOptionLMX(double real_value, double valueN, int* valueFN, int* valueFD){
	real_value = (real_value - valueN * 5000000.0) / 5000000.0;
	double x;
	long long maxden = 4194303L; //0x3FFFFF;
	long long ai, FN1, FD1;
	long long m[2][2];
	x = real_value;
	
	/// initialize matrix
	m[0][0] = 1L;
	m[1][1] = 1L;
	m[0][1] = 0L;
	m[1][0] = 0L;
	
	/// loop finding terms until denom gets too big
	while (m[1][0] *  ( ai = (long long)x ) + m[1][1] <= maxden) {
			long long t;
			t = m[0][0] * ai + m[0][1];
			m[0][1] = m[0][0];
			m[0][0] = t;
			t = m[1][0] * ai + m[1][1];
			m[1][1] = m[1][0];
			m[1][0] = t;
			if (x == (double)ai) { break; }   /// AF: division by zero
			x = 1/(x - (double) ai);
			if (x > (double)0x7FFFFFFF) { break; } /// AF: representation failure
	}
	
	/// now remaining x is between 0 and 1/ai */
	/// approx as either 0 or 1/m where m is max that will fit in maxden */
	/// first try zero */
	///
	if(m[1][0]>0 && m[0][0]>0)
	{
			FN1 = m[0][0];
			FD1 = m[1][0];
	}else{
			ai = (maxden - m[1][1]) / m[1][0];
			m[0][0] = m[0][0] * ai + m[0][1];
			m[1][0] = m[1][0] * ai + m[1][1];
	
			FN1 = m[0][0];
			FD1 = m[1][0];
	}
	double reg1Res = FN1 / FD1;
	(*valueFN) = (int)(FN1 - reg1Res * FD1);
	if (valueFN < 0) {
			valueFN = 0;
	}
	(*valueFD) = FD1;
}	

