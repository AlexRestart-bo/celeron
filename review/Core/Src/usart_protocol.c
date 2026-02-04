#include "usart_protocol.h"
#include "assistfuncs.h"

extern UART_HandleTypeDef huart3;
#define TEC_CTRL_0_Pin GPIO_PIN_1
#define TEC_CTRL_0_GPIO_Port GPIOA
#define TEC_CTRL_1_Pin GPIO_PIN_2
#define TEC_CTRL_1_GPIO_Port GPIOA

float ConvertToTemp(float value);
void CamputeBufferModulationVCSEL(uint32_t scale, uint32_t shift);
void AD9956_WriteProfile(uint8_t u8ShiftProfile, uint64_t u64Freq, uint16_t u16Phase);
void LMX2486_SetFreq(uint32_t valueN, uint32_t valueFN, uint32_t valueFD);
void InitTableData(void);
void InitBufferDAC1(float amplitude, float phase);
void InitBufferDAC2(float amplitude, float phase);
void CamputeOptionSetValueLMX(int value);
void CamputeOptionAddValueLMX(int value);
void FLASH_SaveOptionData(void);
void FLASH_LoadOptionData(void);
void FLASH_Fuse(int state);

// Копия текущей команда 
static uint8_t strCommand[256];

///////////////////////////////////////////////////////////
// Приём данных из USART3
///////////////////////////////////////////////////////////
int USART_GetByte(void){
	int data; 
	// Контейнер пустой
	if (USART_rx_count == 0){
		return (-1);
	}
	// Забираем данные из начала буфера
	data = USART_rx_buffer[USART_rx_rd_index];
	USART_rx_buffer[USART_rx_rd_index] = 0;
	USART_rx_rd_index++;
	// Цикличный буфер
	if (USART_rx_rd_index == 255) {
		USART_rx_rd_index = 0; 
	}
	// Уменьшаем счетчик принятых данных (достали из буфера)
	USART_rx_count--;
	my_N_4++;
	return data;
}

// Передача байта
void USART_SendByte(uint8_t byte){
		USART_tx_buffer[USART_tx_wr_index] = byte;
		USART_tx_wr_index++;
		USART_tx_count++;

		if (USART_tx_wr_index == 255){
			USART_tx_wr_index = 0;		
			USART_tx_buffer_overflow = true; 			
		}
		if (USART_tx_count > 255){
			USART_tx_count = 0;
			USART_tx_buffer_overflow = false; 	
		}
}
void USART_SendString(uint8_t* str, uint8_t count){
	uint8_t i;
	for ( i = 0; i < count; i++){
		USART_SendByte((uint8_t)str[i]);
	}
}

void EraseStr(uint8_t* str, uint16_t length, uint16_t count){
  uint16_t i;
  for (i=0; i < length; i++){
    if(count + i < length){
      str[i] = str[count+i];
      str[count+i] = 0;
    } else {
      str[i] = 0;  
    }
  }
};

float unpackFloat(uint8_t *buf){
	float result = 0.0;
	*((uint8_t*)(&result) + 3) = buf[3];
	*((uint8_t*)(&result) + 2) = buf[2];
	*((uint8_t*)(&result) + 1) = buf[1];
	*((uint8_t*)(&result) + 0) = buf[0];
  return result;
}
uint32_t packFloat(float value){
	uint32_t result = 0;
	result = (*((uint8_t*)(&value) + 0));
	result |= (*((uint8_t*)(&value) + 1))<<8;
	result |= (*((uint8_t*)(&value) + 2))<<16;
	result |= (*((uint8_t*)(&value) + 3))<<24;
  return result;
}
// Binary protocol RS-232
// Start 0xAF
// Command ID
// Count Byte
// Data
// Stop 0xFA
void SendPkgData(int type, uint8_t* data, uint8_t countByte){
	uint8_t i = 0;
	static uint8_t msg[64];
	msg[0] = BYTE_START;
	msg[1] = type;
	msg[2] = countByte;
	// Данные
	for ( i = 0; i < countByte; i++){
		msg[3+i] = (data[i]) & 0xFF;
	}
	// Стоповый байт
	msg[2+countByte+1] = BYTE_STOP;
	USART_SendString((uint8_t*)msg, 4+countByte);	
	// Включаем прерывание по окончанию передачи
	USART3->CR1 |= USART_CR1_TCIE;	
}

// Рекурсивный поиск до первой найденой команды
int GetCmdID(uint8_t* strCMD, uint16_t* countByte){
  // Посылка отсутствует
	static bool flagFindedPkg = false;
  static uint8_t indexStartByte = 0;
  static uint8_t indexStopByte = 0;
  static uint8_t i;
  // Ищем стартовый-стоповый байт
  for ( i = 0; i < (*countByte); i++){
    // Первый найденный стартовый байт
    if (strCMD[i] == BYTE_START){
      indexStartByte = i;
			flagFindedPkg = true;
      break;
    }
  }
	// Не нашли стартовый байт
	if (flagFindedPkg == false){
		// Удаляем все данные
		EraseStr(strCMD, (*countByte), (*countByte));
    (*countByte) = 0;
		return 0;
	}
  // Слишком мало данных
  if (indexStartByte + 4 > (*countByte)){
    // Ждем данные
    return 0;
  }
  static uint16_t countData;
	countData = strCMD[indexStartByte+2];
  // Ошибка размера пакета
  if (countData > 64){
    // Удаляем все включая стартовый байт
      EraseStr(strCMD, (*countByte), indexStartByte + 1);
      (*countByte) -= indexStartByte + 1;
      return GetCmdID(strCMD, countByte);
  }
	 // Слишком мало данных
  if (indexStartByte + 4 + countData  > (*countByte)){
    // Ждем данные
    return 0;
  }
  // Ошибка стопового байта
  if (strCMD[indexStartByte + countData + 3] != BYTE_STOP){
    // Удаляем все включая стартовый байт
    EraseStr(strCMD, (*countByte), indexStartByte + 1);
    (*countByte) -= indexStartByte + 1;
    return GetCmdID(strCMD, countByte);
  }
  indexStopByte = indexStartByte + countData + 3;
  // Посылка нужной длины
  // Определяем ID посылки и данные
  uint8_t ID = strCMD[indexStartByte + 1];
	// Копируем команду
	for ( i = 0; i < (indexStopByte - indexStartByte) + 1; i++){
		strCommand[i] = strCMD[indexStartByte + i];
	}
  // Удаляем все включая стоповый байт
  EraseStr(strCMD, (*countByte), indexStopByte + 1);
  (*countByte) -= indexStopByte + 1;
  return ID;
}

void SetValueFloat(uint8_t id, uint8_t* cmd, float* value){
	static uint32_t dataSend[1];
	(*value) = unpackFloat(&cmd[3]);
	dataSend[0] = packFloat((*value));
	SendPkgData(id, ((uint8_t*)&dataSend), 4);
}
void SetOptionPID(int id, uint8_t* cmd, float* Kp, float* Ki, float* Kd){
	static uint32_t dataSend[3];
	(*Kp) = unpackFloat(&cmd[3]);
	(*Ki) = unpackFloat(&cmd[7]);
	(*Kd) = unpackFloat(&cmd[11]);
	dataSend[0] = packFloat((*Kp));
	dataSend[1] = packFloat((*Ki));
	dataSend[2] = packFloat((*Kd));
	SendPkgData(id, ((uint8_t*)&dataSend), 12);
}
void GetOptionPID(int id, float* Kp, float* Ki, float* Kd){
	static uint32_t dataSend[3];
	dataSend[0] = packFloat((*Kp));
	dataSend[1] = packFloat((*Ki));
	dataSend[2] = packFloat((*Kd));
	SendPkgData(id, ((uint8_t*)&dataSend), 12);
}

void GetItemTable(int id, int index){
	static uint32_t dataSend[4];
	dataSend[0] = index;
	dataSend[1] = packFloat(tableCompensationFREQ[0][index]);
	dataSend[2] = packFloat(tableCompensationFREQ[1][index]);
	dataSend[3] = isTableCompensationFREQ[index];
	SendPkgData(id, ((uint8_t*)&dataSend), 16);
}

void GetTestValue(int id, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d){
	static uint32_t dataSend[4];
	dataSend[0] = *a;
	dataSend[1] = *b;
	dataSend[2] = *c;
	dataSend[3] = *d;
	SendPkgData(id, ((uint8_t*)&dataSend), 16);
}
//=========================================================
// Выполнение команд
//=========================================================
void CamputeCmd(int id){
	int value = 0;
	static uint8_t ID;
	ID = (uint8_t)(id&0xFF);
	my_N_2++;
	// Отладочные настройки
	if ((statusAccessRS232 & RS232_DEBUG) == RS232_DEBUG){	
		switch(ID){
			// Тип модуляции частоты
			case SET_ITEM_TYPE_MOD_FREQ:
				itemTypeModeFREQ = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&itemTypeModeFREQ), 4);
				break;
			case GET_ITEM_TYPE_MOD_FREQ:
				SendPkgData(id, ((uint8_t*)&itemTypeModeFREQ), 4);
				break;
			// Тип модуляции тока
			case SET_ITEM_TYPE_MOD_CRNT:
				itemTypeModeCRNT = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&itemTypeModeCRNT), 4);
				break;
			case GET_ITEM_TYPE_MOD_CRNT:		
				SendPkgData(id, ((uint8_t*)&itemTypeModeCRNT), 4);
				break;
			case SET_VCSEL_SCANLIMIT_MIN:
				VCSEL_ScanLimitMin = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&VCSEL_ScanLimitMin), 4);
				break;
			case GET_VCSEL_SCANLIMIT_MIN:
				SendPkgData(id, ((uint8_t*)&VCSEL_ScanLimitMin), 4);
				break;
			case SET_VCSEL_SCANLIMIT_MAX:
				VCSEL_ScanLimitMax = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&VCSEL_ScanLimitMax), 4);
				break;
			case GET_VCSEL_SCANLIMIT_MAX:
				SendPkgData(id, ((uint8_t*)&VCSEL_ScanLimitMax), 4);
				break;
			case SET_VCSEL_SCAN_STEP:
				VCSEL_ScanStep = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&VCSEL_ScanStep), 4);
				break;
			case GET_VCSEL_SCAN_STEP:
				SendPkgData(id, ((uint8_t*)&VCSEL_ScanStep), 4);
				break;
			case SET_VCSEL_LVL_DETECTOR:
				SetValueFloat(id, strCommand, &VCSEL_levelDetector);
				break;
			case GET_VCSEL_LVL_DETECTOR:
				SendPkgData(id, ((uint8_t*)&VCSEL_levelDetector), 4);
				break;
			case SET_VCSEL_LVL_SIGMA:
				SetValueFloat(id, strCommand, &VCSEL_levelSigma);
				break;
			case GET_VCSEL_LVL_SIGMA:
				SendPkgData(id, ((uint8_t*)&VCSEL_levelSigma), 4);
				break;	
			case SET_LVL_TEMP_CELL_RUN:
				SetValueFloat(id, strCommand, &levelStartTempCELL);
				break;
			case GET_LVL_TEMP_CELL_RUN:
				SendPkgData(id, ((uint8_t*)&levelStartTempCELL), 4);
				break;
			case SET_SCAN_FREQ_LIMIT_MIN:
				VCO_ScanLimitMin = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&VCO_ScanLimitMin), 4);
				break;
			case GET_SCAN_FREQ_LIMIT_MIN:
				SendPkgData(id, ((uint8_t*)&VCO_ScanLimitMin), 4);
				break;			
			case SET_SCAN_FREQ_LIMIT_MAX:
				VCO_ScanLimitMax = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&VCO_ScanLimitMax), 4);
				break;
			case GET_SCAN_FREQ_LIMIT_MAX:
				SendPkgData(id, ((uint8_t*)&VCO_ScanLimitMax), 4);
				break;			
			case SET_SCAN_FREQ_STEP:
				VCO_ScanStep = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&VCO_ScanStep), 4);
				break;
			case GET_SCAN_FREQ_STEP:
				SendPkgData(id, ((uint8_t*)&VCO_ScanStep), 4);
				break;
			case 	SET_MODCRNT_SHIFT:
				break;
			case 	GET_MODCRNT_SHIFT:
				break;
			case SET_OPTION_COIL_CRNT:
				option_COIL_CRNT = (*((int*)(&strCommand[3])));
				fixValue_COIL_CRNT = option_COIL_CRNT;
				SendPkgData(id, ((uint8_t*)&option_COIL_CRNT), 4);
				break;
			case GET_OPTION_COIL_CRNT:
				SendPkgData(id, ((uint8_t*)&option_COIL_CRNT), 4);
				break;
			case SET_OPTION_SLEEP:
				option_order_sleep = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&option_order_sleep), 4);
				break;
			case GET_OPTION_SLEEP:
				SendPkgData(id, ((uint8_t*)&option_order_sleep), 4);
				break;
			case SET_SIGMA_VCSEL:
				SetValueFloat(id, strCommand, &sigmaPID_TEC_CTRL);
				SendPkgData(id, ((uint8_t*)&sigmaPID_TEC_CTRL), 4);
				break;
			case GET_SIGMA_VCSEL:
				SendPkgData(id, ((uint8_t*)&sigmaPID_TEC_CTRL), 4);
				break;
			case GET_MIN_MAX_SIZE_STEK_HEAP:
				aa = (uint32_t)DMA_ADC2_Data;
				bb = (uint32_t)(DMA_ADC2_Data + ADC_ARRAY_DMA_ADC2_HALF_SIZE);
				cc = 0;
				dd = 0;
				GetTestValue(id, &aa, &bb, &cc, &dd);
				break;
			case SET_SCOPE:
				if (!((discr_wave.fbcheck = *((int*)(strCommand + 3))) < 1 || discr_wave.fbcheck > 10000 
								|| (discr_wave.facheck = *((int*)(strCommand + 3 + 4))) < 1 || discr_wave.facheck > 10000))
				{
						discr_wave.factor_beauty = discr_wave.fbcheck;
						discr_wave.factor_average = discr_wave.facheck;
				}
				break;
		}
	}
	
	//=======================================================
	// Настройки параметрров ПИД регуляторов
	//=======================================================
	if ((statusAccessRS232 & RS232_SETUP_PID) == RS232_SETUP_PID){	
		switch(ID){
			//===================================================
			// Привязка температуры лазера по датчику температуры
			//===================================================
			case	SET_TECCTRL_PID:
				SetOptionPID(id, strCommand, &primal_set[2].factor_kp,
																		 &primal_set[2].factor_ki,
																		 &primal_set[2].factor_kd);
				break;
			case	GET_TECCTRL_PID:
				GetOptionPID(id, &primal_set[2].factor_kp,
												 &primal_set[2].factor_ki,
												 &primal_set[2].factor_kd);
				break;
			//===================================================
			// Привязка к линии поглощения током лазера
			//===================================================
			case	SET_DOPLER_CRNT_PID:
				SetOptionPID(id, strCommand, &primal_set[0].factor_kp,
																		 &primal_set[0].factor_ki,
																		 &primal_set[0].factor_kd);			
				break;
			case	GET_DOPLER_CRNT_PID:
				GetOptionPID(id, &primal_set[0].factor_kp,
												 &primal_set[0].factor_ki,
												 &primal_set[0].factor_kd);			
				break;
			//===================================================
			// Привязка частоты к КПН резонансу
			//===================================================
			case	SET_VCO_PID:
				SetOptionPID(id, strCommand, &primal_set[1].factor_kp,
																		 &primal_set[1].factor_ki,
																		 &primal_set[1].factor_kd);
				break;
			case	GET_VCO_PID:
				GetOptionPID(id, &primal_set[1].factor_kp,
												 &primal_set[1].factor_ki,
												 &primal_set[1].factor_kd);	
				break;
			//===================================================
			// Привязка температуры ячейки по датчику температуры
			//===================================================
			case	SET_CELLHTR_PID:
				SetOptionPID(id, strCommand, &primal_set[3].factor_kp,
																		 &primal_set[3].factor_ki,
																		 &primal_set[3].factor_kd);
				break;
			case	GET_CELLHTR_PID:
				GetOptionPID(id, &primal_set[3].factor_kp,
												 &primal_set[3].factor_ki,
												 &primal_set[3].factor_kd);
			//===================================================
			// Резерв
			//===================================================
				break;
			case	SET_AUXTEC_PID:
				break;
			case	GET_AUXTEC_PID:
				break;
			case SET_DOPLER_TEMP_PID:
				SetOptionPID(id, strCommand, &primal_set[6].factor_kp,
																		 &primal_set[6].factor_ki,
																		 &primal_set[6].factor_kd);		
				break;
			case GET_DOPLER_TEMP_PID:			
				GetOptionPID(id, &primal_set[6].factor_kp,
												 &primal_set[6].factor_ki,
												 &primal_set[6].factor_kd);
				break;
			//===================================================
			// COMP
			//===================================================
			case SET_COMP_CRNT_FUN:
				SetOptionPID(id, strCommand, &COMP_CRNT_A, 
																		&COMP_CRNT_B, 
																		&COMP_CRNT_C);		
				break;
			case GET_COMP_CRNT_FUN:			
				GetOptionPID(id, &COMP_CRNT_A, 
												&COMP_CRNT_B, 
												&COMP_CRNT_C);
				break;
			//===================================================
			// Привязка оптической мощности током лазера по сигналу фотоприемника
			//===================================================
			case SET_OPTICS_PWR_PID:
				SetOptionPID(id, strCommand, &primal_set[7].factor_kp,
																		 &primal_set[7].factor_ki,
																		 &primal_set[7].factor_kd);		
				break;
			case GET_OPTICS_PWR_PID:			
				GetOptionPID(id, &primal_set[7].factor_kp,
												 &primal_set[7].factor_ki,
												 &primal_set[7].factor_kd);
				break;
			//===================================================
			// Резерв
			//===================================================
			case	SET_RFATT_PID:
				SetOptionPID(id, strCommand, &primal_set[5].factor_kp,
																		 &primal_set[5].factor_ki,
																		 &primal_set[5].factor_kd);	
				break;
			case	GET_RFATT_PID:
				GetOptionPID(id, &primal_set[5].factor_kp,
												 &primal_set[5].factor_ki,
												 &primal_set[5].factor_kd);
				break;
			case SCREEN_DATA:
				indexSaveCRNT = 0;
				indexSaveFREQ = 0;
				indexSaveScreen = 0;
				isScreenSaveDoneFREQ = false;
				isRecordScreenFREQ = true;
				isScreenSaveDoneCRNT = false;
				isRecordScreenCRNT = true;
				break;
			// Фаза синхронного детектора RF
			case SET_PHASE_TABLE_RF:
				SetValueFloat(id, strCommand, &phaseTableRF);
				InitTableData();
				break;
			// Фаза синхронного детектора тока
			case SET_PHASE_TABLE_CRNT:
				SetValueFloat(id, strCommand, &phaseTableCRNT);
				InitTableData();
				break;
			case GET_PHASE_TABLE_RF:
				SendPkgData(id, ((uint8_t*)&phaseTableRF), 4);
				break;
			case GET_PHASE_TABLE_CRNT:
				SendPkgData(id, ((uint8_t*)&phaseTableCRNT), 4);
				break;
			// Фаза синхронного детектора частоты
			case SET_PHASE_TABLE_FREQ:
				SetValueFloat(id, strCommand, &phaseTableFREQ);
				InitTableData();
				break;
			case GET_PHASE_TABLE_FREQ:
				SendPkgData(id, ((uint8_t*)&phaseTableFREQ), 4);
				break;
			// Амплитуда модуляции частоты
			case SET_AMPLITUDE_MODULATION_FREQ:
				SetValueFloat(id, strCommand, &amplitudeModulationFREQ);
				InitBufferDAC1(amplitudeModulationFREQ, phaseModulationFREQ);
				break;
			case GET_AMPLITUDE_MODULATION_FREQ:
				SendPkgData(id, ((uint8_t*)&amplitudeModulationFREQ), 4);
				break;
			// Начальная фаза модуляции частоты
			case SET_PHASE_MODULATION_FREQ:			
				SetValueFloat(id, strCommand, &phaseModulationFREQ);
				InitBufferDAC1(amplitudeModulationFREQ, phaseModulationFREQ);
				InitTableData();
				break;
			case GET_PHASE_MODULATION_FREQ:
				SendPkgData(id, ((uint8_t*)&phaseModulationFREQ), 4);
				break;
			// Амплитуда модуляции тока
			case SET_AMPLITUDE_MODULATION_CRNT:
				SetValueFloat(id, strCommand, &amplitudeModulationCRNT);
				InitBufferDAC2(amplitudeModulationCRNT, phaseModulationCRNT);
				break;
			case GET_AMPLITUDE_MODULATION_CRNT:
				SendPkgData(id, ((uint8_t*)&amplitudeModulationCRNT), 4);
				break;
			// Начальная фаза модуляции тока
			case SET_PHASE_MODULATION_CRNT:			
				SetValueFloat(id, strCommand, &phaseModulationCRNT);
				InitBufferDAC2(amplitudeModulationCRNT, phaseModulationCRNT);		//
				InitTableData();
				break;
			case GET_PHASE_MODULATION_CRNT:
				SendPkgData(id, ((uint8_t*)&phaseModulationCRNT), 4);
				break;
			// Источник данных фазового детектора RF
			case SET_ITEM_SOURCE_DATA_RF:
				itemSourceDataRF = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&itemSourceDataRF), 4);
				break;
			case GET_ITEM_SOURCE_DATA_RF:
				SendPkgData(id, ((uint8_t*)&itemSourceDataRF), 4);
				break;
			// Источник данных фазового детектора тока
			case SET_ITEM_SOURCE_DATA_CRNT:
				itemSourceDataCRNT = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&itemSourceDataCRNT), 4);
				switch(itemSourceDataCRNT){
					case 0:
						phaseTableCRNT = 4.53786f; //260 degrees				
						InitTableData();
						break;
					case 1:
						phaseTableCRNT = 1.91986f; //110 degrees
						InitTableData();
						break;
					case 2:
						phaseTableCRNT = 4.18879f; //240 degrees
						InitTableData();
						break;
					case 3:
						phaseTableCRNT = 1.48353f; //85 degrees
						InitTableData();
						break;
				}
				SendPkgData(GET_PHASE_TABLE_CRNT, ((uint8_t*)&phaseTableCRNT), 4);
				break;
			case GET_ITEM_SOURCE_DATA_CRNT:
				SendPkgData(id, ((uint8_t*)&itemSourceDataCRNT), 4);
				break;
			// Источник данных фазового детектора частоты
			case SET_ITEM_SOURCE_DATA_FREQ:
				itemSourceDataFREQ = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&itemSourceDataFREQ), 4);
				switch(itemSourceDataFREQ){
					case 0:
						phaseTableFREQ = 2.68781f; //154 degrees				
						InitTableData();
						break;
					case 1:
						phaseTableFREQ = 5.84685f; //335 degrees
						InitTableData();
						break;
					case 2:
						phaseTableFREQ = 2.68781f; //154 degrees
						InitTableData();
						break;
					case 3:
						phaseTableFREQ = 2.68781f; //154 degrees
						InitTableData();
						break;
				}
				SendPkgData(GET_PHASE_TABLE_FREQ, ((uint8_t*)&phaseTableFREQ), 4);
				break;
				break;
			case GET_ITEM_SOURCE_DATA_FREQ:
				SendPkgData(id, ((uint8_t*)&itemSourceDataFREQ), 4);
				break;
			// Смещение уровня дискриминатора частоты
			case SET_SHIFT_DETECTOR_FREQ:
				SetValueFloat(id, strCommand, &shiftDetector_FREQ);
				break;
			case GET_SHIFT_DETECTOR_FREQ:
				SendPkgData(id, ((uint8_t*)&shiftDetector_FREQ), 4);
				break;
			// Смещение уровня дискриминатора линии поглощения
			case SET_SHIFT_DETECTOR_DOPLER:
				SetValueFloat(id, strCommand, &shiftDetector_DOPLER);
				break;
			case GET_SHIFT_DETECTOR_DOPLER:
				SendPkgData(id, ((uint8_t*)&shiftDetector_DOPLER), 4);
				break;
			case	SET_PWR_ADJUSTMENT_PID:
				SetOptionPID(id, strCommand, &primal_set[10].factor_kp,
																		 &primal_set[10].factor_ki,
																		 &primal_set[10].factor_kd);	
				break;
			case	GET_PWR_ADJUSTMENT_PID:
				GetOptionPID(id, &primal_set[10].factor_kp,
												 &primal_set[10].factor_ki,
												 &primal_set[10].factor_kd);
				break;
		}
		// Продолжение switch (64 max)
		switch(ID){
			case GET_OPTION_FACTOR_COIL_FREQ:			
				GetOptionPID(id, &option_factor_A_COIL, 
												 &option_factor_B_COIL, 
												 &option_factor_C_COIL);	
				break;
			case SET_OPTION_FACTOR_COIL_FREQ:
				SetOptionPID(id, strCommand, &option_factor_A_COIL, 
																		 &option_factor_B_COIL, 
																		 &option_factor_C_COIL);	
				break;	
			case SET_OPTION_POLARITY_TEC:
				option_polarity_TEC = (*((int*)(&strCommand[3])));
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
				SendPkgData(id, ((uint8_t*)&option_polarity_TEC), 4);
				break;
			case GET_OPTION_POLARITY_TEC:
				SendPkgData(id, ((uint8_t*)&option_polarity_TEC), 4);
				break;
			case SET_SPEED_UART:
				option_speed_uart = (*((int*)(&strCommand[3])));
				huart3.Instance = USART3;
				huart3.Init.BaudRate = option_speed_uart;
				huart3.Init.WordLength = UART_WORDLENGTH_8B;
				huart3.Init.StopBits = UART_STOPBITS_1;
				huart3.Init.Parity = UART_PARITY_NONE;
				huart3.Init.Mode = UART_MODE_TX_RX;
				huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
				huart3.Init.OverSampling = UART_OVERSAMPLING_16;
				huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
				huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
				if (HAL_UART_Init(&huart3) != HAL_OK){};
				SendPkgData(id, ((uint8_t*)&option_speed_uart), 4);
				break;
			case SET_IRQ_UART:
				option_irq_uart = (*((int*)(&strCommand[3])));
				HAL_NVIC_SetPriority(USART3_IRQn, (uint32_t)option_irq_uart, 0);
				SendPkgData(id, ((uint8_t*)&option_irq_uart), 4);
				break;
			case GET_IRQ_UART:
				SendPkgData(id, ((uint8_t*)&option_irq_uart), 4);
				break;
			case SET_AUTO_CAPTURE:
				perm_auto_capt = true;
				break;
			case GET_AUTO_CAPTURE:
				SendPkgData(id, ((uint8_t*)&option_irq_uart), 4);
				break;
		}
	}
	
	//=======================================================
	// Настройка стандарта
	//=======================================================
	if ((statusAccessRS232 & RS232_FIRST_RUN) == RS232_FIRST_RUN){	
		switch(ID){
			// Установка рабочего режима
			case SET_MODE_WORK:
				itemModeWork = (*((int*)(&strCommand[3])));
				itemStageWork_NONE = 0;
				itemStageWork_SCAN_CRNT = 0;
				itemStageWork_SCAN_TEMP = 0;
				itemStageWork_SCAN_FREQ = 0;
				itemStageWork_FIND_DOPLER_CRNT = 0;
				itemStageWork_FIND_DOPLER_TEMP = 0;
				itemStageWork_FIND_LORENZ = 0;
				itemStageWork_SET_VCSEL_DC = 0;
				itemStageWork_SET_ATT_PWR = 0;
				itemStageWork_RUN = 0;
				itemStageWork_STREAM_SCREEN = 0;
				itemStageWork_TEST_LOOP_CRNT = 0;	
				itemStageWork_TEST_LOOP_FREQ = 0;		
				itemStageWork_MODE_CRNT = 0;
				itemStageWork_MODE_RF = 0;
				itemStageWork_SCAN_TEC_VCSEL = 0;
				itemStageWork_SCAN_RF_ATT = 0;
				itemStageWork_SCAN_FINE_TUNING_FREQ = 0;
				SendPkgData(id, ((uint8_t*)&itemModeWork), 4);
				break;
			// Отправка текущего режима
			case GET_MODE_WORK:
				SendPkgData(id, ((uint8_t*)&itemModeWork), 4);
				break;
			// Режим работы после запуска
			case SET_START_MODE:
				option_start_mode = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&option_start_mode), 4);
				break;
			case GET_START_MODE:
				SendPkgData(id, ((uint8_t*)&option_start_mode), 4);
				break;
			// Рабочий ток лазера
			case SET_OPTION_VCSEL_DC:
				option_VCSEL_DC = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&option_VCSEL_DC), 4);
				break;
			case GET_OPTION_VCSEL_DC:
				SendPkgData(id, ((uint8_t*)&option_VCSEL_DC), 4);
				break;
			// Рабочая мощность СВЧ
			case SET_OPTION_POWER_RF_ATT:
				option_POWER_RF_ATT = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&option_POWER_RF_ATT), 4);
				break;
			case GET_OPTION_POWER_RF_ATT:
				SendPkgData(id, ((uint8_t*)&option_POWER_RF_ATT), 4);
				break;
			// Рабочий ток охлождения хвостика ячейки
			case SET_OPTION_AUX_TEC:
				option_AUX_TEC = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&option_AUX_TEC), 4);
				break;
			case GET_OPTION_AUX_TEC:
				SendPkgData(id, ((uint8_t*)&option_AUX_TEC), 4);
				break;
			// СВЧ мощность при поиске пика поглощения
			case SET_OPTION_FIND_DC_PWR_RF:
				option_FIND_DC_PWR_RF_ATT = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&option_FIND_DC_PWR_RF_ATT), 4);
				break;
			case GET_OPTION_FIND_DC_PWR_RF:
				SendPkgData(id, ((uint8_t*)&option_FIND_DC_PWR_RF_ATT), 4);
				break;
			// Смещение частоты СВЧ относительно линии D1 Rb87
			case SET_SHIFT_LMX_FREQ:
				SetValueFloat(id, strCommand, &LMX_ShiftFreq);
				CamputeOptionSetValueLMX(LMX_ValueINT);
				break;
			case GET_SHIFT_LMX_FREQ:
				SendPkgData(id, ((uint8_t*)&LMX_ShiftFreq), 4);
				break;
			// Запуск/отключение ПИД регуляторов
			case SET_LOOP_PID:
				// Сброс на по умолчанию отключенный ПИД регулятор
				statusLoopPID = (*((uint16_t*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&statusLoopPID), 2);
				break;
			case GET_LOOP_PID:
				SendPkgData(id, ((uint8_t*)&statusLoopPID), 2);
				break;
			// Изменение частоты 10 МГц грубый канал
			case SET_VCO_CRS:
				value_VCO_CRS = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&value_VCO_CRS), 4);
				flag_VCO_CRS = true;
				break;
			case GET_VCO_CRS:
			 SendPkgData(id, ((uint8_t*)&value_VCO_CRS), 4);
			 break;
			// Изменение частоты 10 МГц тонкий канал (сброс ПИД регулятора)
			case SET_VCO_FINE:
				value_VCO_FINE = (*((int*)(&strCommand[3])));
				reset_pid(&primal_set[1]);
				SendPkgData(id, ((uint8_t*)&value_VCO_FINE), 4);
				flag_VCO_FINE = true;
				break;
			case GET_VCO_FINE:
				SendPkgData(id, ((uint8_t*)&value_VCO_FINE), 4);
				break;
			// Изменение тока катушек подмагничивания
			case SET_COIL_CRNT:
				value_COIL_CRNT = (*((int*)(&strCommand[3])));
				fixValue_COIL_CRNT = value_COIL_CRNT;
				option_COIL_CRNT = value_COIL_CRNT;
				SendPkgData(id, ((uint8_t*)&value_COIL_CRNT), 4);
				flag_COIL_CRNT = true;
				break;
			case GET_COIL_CRNT:
				SendPkgData(id, ((uint8_t*)&value_COIL_CRNT), 4);
				break;
			// Изменение тока лазера
			case SET_VCSEL_DC:
				value_VCSEL_DC = (*((int*)(&strCommand[3])));
			SendPkgData(id, ((uint8_t*)&value_VCSEL_DC), 4);
				fixValue_VCSEL_DC = value_VCSEL_DC;
				fixValue_POWER_DC = fixValue_VCSEL_DC;
				flag_VCSEL_DC = true;
				isErrorTempVCSEL = false;
				break;
			case GET_VCSEL_DC:
				SendPkgData(id, ((uint8_t*)&value_VCSEL_DC), 4);
				break;
			case SET_FIX_VCSEL_DC:
				fixValue_VCSEL_DC = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&fixValue_VCSEL_DC), 4);
				break;
			case GET_FIX_VCSEL_DC:
				SendPkgData(id, ((uint8_t*)&fixValue_VCSEL_DC), 4);
				break;
			case SET_SCAN_TEC_VCSEL_STEP:
				SCAN_TEC_VCSEL_Step = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&SCAN_TEC_VCSEL_Step), 4);
				break;
			case GET_SCAN_TEC_VCSEL_STEP:
				SendPkgData(id, ((uint8_t*)&SCAN_TEC_VCSEL_Step), 4);
				break;
			case SET_SCAN_TEC_VCSEL_LIMIT_MAX:
				SCAN_TEC_VCSEL_LimitMax = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&SCAN_TEC_VCSEL_LimitMax), 4);
				break;
			case GET_SCAN_TEC_VCSEL_LIMIT_MAX:
				SendPkgData(id, ((uint8_t*)&SCAN_TEC_VCSEL_LimitMax), 4);
				break;
			case SET_SCAN_TEC_VCSEL_LIMIT_MIN:
				SCAN_TEC_VCSEL_LimitMin = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&SCAN_TEC_VCSEL_LimitMin), 4);
				break;
			case GET_SCAN_TEC_VCSEL_LIMIT_MIN:
				SendPkgData(id, ((uint8_t*)&SCAN_TEC_VCSEL_LimitMin), 4);
				break;
			// Изменение СВЧ мощности
			case SET_RF_ATT:
				value_RF_ATT = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&value_RF_ATT), 4);
				fixValue_RF_ATT = value_RF_ATT;
				reset_pid(&primal_set[5]);
				flag_RF_ATT = true;
				break;
			case GET_RF_ATT:
				SendPkgData(id, ((uint8_t*)&value_RF_ATT), 4);
				break;
			// Изменение тока холодильника хвостика ячейки
			case SET_AUX_TEC:
				value_AUX_TEC = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&value_AUX_TEC), 4);
				flag_AUX_TEC = true;
				break;	
			case GET_AUX_TEC:
				SendPkgData(id, ((uint8_t*)&value_AUX_TEC), 4);
				break;
			// Изменение тока пельтье лазера (сброс ПИД регулятора)
			case SET_TEC_CTRL:
				value_TEC_CTRL = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&value_TEC_CTRL), 4);
				reset_pid(&primal_set[6]);
				flag_TEC_CTRL = true;
				break;
			case GET_TEC_CTRL:
				SendPkgData(id, ((uint8_t*)&value_TEC_CTRL), 4);
				break;
			// Изменение мощности нагрева ячейки
			case SET_CELL_HTR:
				value_CELL_HTR = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&value_CELL_HTR), 4);
				flag_CELL_HTR = true;
				break;
			case GET_CELL_HTR:
				SendPkgData(id, ((uint8_t*)&value_CELL_HTR), 4);
				break;
			// Установка номинальной температуры лазера
			case SET_LVL_TEMP_VCSEL:
				SetValueFloat(id, strCommand, &levelTempVCSEL);
				break;
			case GET_LVL_TEMP_VCSEL:
				SendPkgData(id, ((uint8_t*)&levelTempVCSEL), 4);
				break;
			// Установка номинальной температуры датчика 1
			case SET_LVL_TEMP_AUX1:
				SetValueFloat(id, strCommand, &levelTempAUX1);
				break;
			case GET_LVL_TEMP_AUX1:
				SendPkgData(id, ((uint8_t*)&levelTempAUX1), 4);
				break;
			// Установка номинальной температуры ячейки
			case SET_LVL_TEMP_CELL:
				SetValueFloat(id, strCommand, &levelTempCELL);
				break;
			case GET_LVL_TEMP_CELL:
				SendPkgData(id, ((uint8_t*)&levelTempCELL), 4);
				break;
			// Установка номинальной температуры датчика 2
			case SET_LVL_TEMP_AUX2:
				SetValueFloat(id, strCommand, &levelTempAUX2);
				break;
			case GET_LVL_TEMP_AUX2:
				SendPkgData(id, ((uint8_t*)&levelTempAUX2), 4);
				break;
			// Установка новых значений регистров синтезатора
			case SET_FREQ:
				LMX_ValueN = (*((int*)(&strCommand[3])));
				LMX_ValueFN = (*((int*)(&strCommand[7])));
				LMX_ValueFD = (*((int*)(&strCommand[11])));
				flag_LMX248 = true;
				BufferDataPack[0] = LMX_ValueN;
				BufferDataPack[1] = LMX_ValueFN;
				BufferDataPack[2] = LMX_ValueFD;
				SendPkgData(id, ((uint8_t*)&BufferDataPack[0]), 12);
				break;
			case SET_CURRENT_PLL:
				LMX_Current_PLL = (*((int*)(&strCommand[3])));
				flag_LMX248 = true;
				SendPkgData(id, ((uint8_t*)&LMX_Current_PLL), 4);
				break;
			case GET_CURRENT_PLL:
				SendPkgData(id, ((uint8_t*)&LMX_Current_PLL), 4);
				break;
			// Отправка текущих значений реистров синтезатора
			case GET_FREQ:
				BufferDataPack[0] = LMX_ValueN;
				BufferDataPack[1] = LMX_ValueFN;
				BufferDataPack[2] = LMX_ValueFD;
				SendPkgData(id, ((uint8_t*)&BufferDataPack[0]), 12);
				break;			
			// Отправка значения сопротивления измерительных резисторов
			case SET_REF_RES_VCSEL:
				SetValueFloat(id, strCommand, &refResistVCSEL);
				break;
			case GET_REF_RES_VCSEL:
				SendPkgData(id, ((uint8_t*)&refResistVCSEL), 4);
				break;
			case SET_REF_RES_AUX1:
				SetValueFloat(id, strCommand, &refResistAUX1);
				break;
			case GET_REF_RES_AUX1:
				SendPkgData(id, ((uint8_t*)&refResistAUX1), 4);
				break;
			case SET_REF_RES_CELL:
				SetValueFloat(id, strCommand, &refResistCELL);
				break;
			case GET_REF_RES_CELL:
				SendPkgData(id, ((uint8_t*)&refResistCELL), 4);
				break;
			case SET_REF_RES_AUX2:
				SetValueFloat(id, strCommand, &refResistAUX2);
				break;
			case GET_REF_RES_AUX2:
				SendPkgData(id, ((uint8_t*)&refResistAUX2), 4);
				break;
			case SET_OPTION_FACTOR_TEMP_FREQ:
				SetOptionPID(id, strCommand, &option_factor_A_TEMP_FREQ, 
																		 &option_factor_B_TEMP_FREQ, 
																		 &option_factor_C_TEMP_FREQ);	
				break;
			case GET_OPTION_FACTOR_TEMP_FREQ:
				GetOptionPID(id,  &option_factor_A_TEMP_FREQ, 
													&option_factor_B_TEMP_FREQ, 
													&option_factor_C_TEMP_FREQ);	
				break;
			case SET_OPTION_FACTOR_COIL:				
				break;
			case GET_OPTION_FACTOR_COIL:
				break;
			case SET_PLL_N1:
				LMX_ValueN_N_1 = (*((int*)(&strCommand[3])));
				LMX_ValueFN_N_1 = (*((int*)(&strCommand[7])));
				LMX_ValueFD_N_1 = (*((int*)(&strCommand[11])));
				break;
			case SET_PLL_N2:
				LMX_ValueN_N_2 = (*((int*)(&strCommand[3])));
				LMX_ValueFN_N_2 = (*((int*)(&strCommand[7])));
				LMX_ValueFD_N_2 = (*((int*)(&strCommand[11])));
				break;
			case SET_PLL_N12:
				my_N12_ = (*((int*)(&strCommand[3])));
				if (my_N12 == 1 && my_N12_ == 0){LMX2486_SetFreq(LMX_ValueN, LMX_ValueFN, LMX_ValueFD);}
				my_N12 = my_N12_;
				break;
			// Изменение тока лазера
			case SET_DESIRED_CRNT:
				desired_crnt = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&desired_crnt), 4);
				break;
			case GET_DESIRED_CRNT:
				SendPkgData(id, ((uint8_t*)&desired_crnt), 4);
				break;
			// структура heat_cap содержит в себе сведения о нижнем и верхнем пределах сканирования мощностью нагрева лазера и шаге сканирования
			case SET_HEAT_CAP:
				SetOptionPID(id, strCommand, &heat_cap.min, 
																		 &heat_cap.max, 
																		 &heat_cap.step);	
				break;
			case GET_HEAT_CAP:
				GetOptionPID(id, &heat_cap.min, 
												 &heat_cap.max, 
												 &heat_cap.step);	
				break;
			
			case SET_MODDEMOD:
				moddemod = (*((int*)(&strCommand[3])));
				change_moddemod(&moddemod);
				SendPkgData(id, ((uint8_t*)&moddemod), 4);
				break;
			case GET_MODDEMOD:
				SendPkgData(id, ((uint8_t*)&moddemod), 4);
				break;
		}
	}
	
	//=======================================================
	// Информационные настройки
	//=======================================================
	if ((statusAccessRS232 & RS232_INFO) == RS232_INFO){	
		switch(ID){
						// Отправка температуры лазера
			case GET_TEMP_VCSEL:
				SendPkgData(id, ((uint8_t*)&valueTempVCSEL), 4);
				break;
					// Отправка температуры датчика 1
			case GET_TEMP_AUX1:
				SendPkgData(id, ((uint8_t*)&valueTempAUX1), 4);
				break;
					// Отправка температуры ячейки
			case GET_TEMP_CELL:
				SendPkgData(id, ((uint8_t*)&valueTempCELL), 4);
				break;
						// Отправка температуры датчика 2
			case GET_TEMP_AUX2:
				SendPkgData(id, ((uint8_t*)&valueTempAUX2), 4);
				break;
			// Отправка температуры верхней платы
			case GET_TEMP_BOARD:
				SendPkgData(id, ((uint8_t*)&valueTempUPR), 4);				
				break;
		}
	}
	
	//=======================================================
	// Команды пользователя
	//=======================================================
		if ((statusAccessRS232 & RS232_SIMPLE) == RS232_SIMPLE){	
		switch(ID){
			// Отправка ID стандарта (тест подключения)
			case GET_TEST_ID:
				SendPkgData(id, ((uint8_t*)&ID_NAP_KPN), 4);
				break;
					// Установка новой номинальной частоты
			case SET_FREQ_WAVE:
				LMX_ValueINT = (*((int*)(&strCommand[3])));
				CamputeOptionSetValueLMX(LMX_ValueINT);
				SendPkgData(id, ((uint8_t*)&LMX_ValueINT), 4);
				break;
			// Смещение относительно номинальной частоты
			case ADD_FREQ_WAVE:
				value = (*((int*)(&strCommand[3])));
				CamputeOptionAddValueLMX(value);
				SendPkgData(id, ((uint8_t*)&LMX_ValueINT), 4);
				break;
			// Отправка кода номинальной частоты
			case GET_FREQ_WAVE:
				SendPkgData(id, ((uint8_t*)&LMX_ValueINT), 4);
				break;
			// Убираем права доступа по RS232
			case SET_ACCESS_RS232:
				value = (*((int*)(&strCommand[3])));
				if (value <= 0){
					value = RS232_SIMPLE | RS232_INFO;
				}else {
					if (value == 0x5DEAD5){
						value = RS232_ADMIN;
					} else {
						value = statusAccessRS232 & value;
					}
				}
				statusAccessRS232 = value;
				SendPkgData(id, ((uint8_t*)&statusAccessRS232), 4);
				break;
			// Отправка текущих прав доступа по RS232
			case GET_ACCESS_RS232:
				SendPkgData(id, ((uint8_t*)&statusAccessRS232), 4);
				break;
			case SET_FUSE_FLASH:
				value = (*((int*)(&strCommand[3])));
				FLASH_Fuse(value);
				stateFuse = READ_BIT(FLASH->OPTR, FLASH_OPTR_RDP);
				SendPkgData(id, ((uint8_t*)&stateFuse), 4);
				break;
			case GET_FUSE_FLASH:
				stateFuse = READ_BIT(FLASH->OPTR, FLASH_OPTR_RDP);
				SendPkgData(id, ((uint8_t*)&stateFuse), 4);
				break;
			// Запись значений во флеш память
			case SAVE_OPTION_DATA:
				option_status = 1;
				FLASH_SaveOptionData();
				SendPkgData(id, (uint8_t*)&option_status, 4);
				option_status = -1;
				break;
			// Пренудельная загрузка данных из флеш памяти
			case LOAD_OPTION_DATA:
				FLASH_LoadOptionData();
				SendPkgData(id, (uint8_t*)&option_status, 4);
				option_status = -1;
				break;
			case 	GET_ID_MC:
				SendPkgData(id, ((uint8_t*)&idSTMPart), 12);
				break;
			case GET_VER_PROG:
				SendPkgData(id, ((uint8_t*)&verProg), 4);
				break;
			case GET_STATUS_CPT:
				SendPkgData(id, ((uint8_t*)&status_CPT), 4);				
				break;
			case SET_UPDATE_STATUS_CPT:
				updateStatus_CPT = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&updateStatus_CPT), 4);
				break;
			case GET_UPDATE_STATUS_CPT:
				SendPkgData(id, ((uint8_t*)&updateStatus_CPT), 4);
				break;
			case SET_ITEM_TABLE_COMPENSATION_FREQ:
				// Получаем индекс таблицы
				value = (*((int*)(&strCommand[3])));
				if (value >= 0 && value < 32){
					tableCompensationFREQ[0][value] = unpackFloat(&strCommand[7]);
					tableCompensationFREQ[1][value] = unpackFloat(&strCommand[11]);
					isTableCompensationFREQ[value] = (*((int*)(&strCommand[15])));	
					GetItemTable(id, value);
				}					
				break;
			case GET_ITEM_TABLE_COMPENSATION_FREQ:
				value = (*((int*)(&strCommand[3])));
				if (value >= 0 && value < 32){
					GetItemTable(id, value);
				}
				break;
			case SET_USE_TABLE_COMPENSATION_FREQ:
				isUseTableCompensationFREQ = (*((int*)(&strCommand[3])));
				SendPkgData(id, ((uint8_t*)&isUseTableCompensationFREQ), 4);
				break;
			case GET_USE_TABLE_COMPENSATION_FREQ:
				SendPkgData(id, ((uint8_t*)&isUseTableCompensationFREQ), 4);
				break;
		}
	}
}

