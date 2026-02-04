#ifndef __VAR_H__
#define __VAR_H__

#include <stdbool.h>
#include <stdint.h>

// ЦАП
#include "AD5668.h"
// Синтезатор
#include "interface_LMX2486.h"
// 24 разрядыный АЦП
#include "interface_AD7794.h"
// 16 разрядный ЦАП
#include "interface_AD5668.h"
#include "assistfuncs.h"

void ProgressPID_BASE(int item_pid);

extern uint16_t permission;
extern uint16_t pid_actuation;
extern int desired_crnt;

extern int moddemod;
//начало моих переменных
void change_moddemod(int* new_freq);
struct prop_consid {
	float min;
	float max;
	float step;
};

extern struct prop_consid heat_cap;

struct discrete_set{
	int cp_tim2;
	int cp_tim15;
	uint32_t cp_left;
	uint32_t cp_right;
	float f_left;
	float f_right;
};

extern struct discrete_set ds1;
extern float ADC_FREQ_90;
extern float FREQ_POS;
extern float FREQ_NEG;
extern short milisec;

extern float ordinates[3];

extern float QQ;
extern float II;
extern float Magnitude_DOPLER;
extern float Phase_DOPLER;

extern float ADC_FREQ_0;
extern float ADC_FREQ_1;
extern float ADC_FREQ_P;
extern float ADC_FREQ_N;

extern float RESULT_ADC_FREQ;


extern float FQ; //действительная часть
extern float FI;//мнимая часть
extern float Magnitude_FREQ;//модуль числа
extern float Phase_FREQ;//фаза

extern int LMX_ValueN_N_1;
extern int LMX_ValueFN_N_1;
extern int LMX_ValueFD_N_1;

extern int LMX_ValueN_N_2;
extern int LMX_ValueFN_N_2;
extern int LMX_ValueFD_N_2;

extern int my_N12;
extern int my_N12_;

extern bool perm_auto_capt;

extern uint32_t aa,bb,cc,dd;
extern wavelength discr_wave;
extern dbg debug_vars;
//конец моих переменных

enum itemWork{
	WORK_NONE = 0,
	WORK_RUN = 1,

	WORK_SCAN_CRNT = 2,
	WORK_SCAN_TEMP = 3,
	WORK_SCAN_FREQ = 4,

	WORK_FIND_DOPLER_CRNT = 5,
	WORK_FIND_LORENZ = 6,
	
	WORK_TEST_DAC_ADC = 7,
	WORK_TEST_TEMP = 8,
	WORK_TEST_LOOP_CRNT = 9,
	WORK_TEST_LOOP_FREQ = 10,
	
	WORK_FIND_TEMP_LASER = 11,
	WORK_FIND_CRNT_WORK = 12,
	WORK_FIND_ATT_RF_WORK = 13,
	WORK_FIND_TEMP_CELL_WORK = 14,

	WORK_MODE_CRNT = 15,
	WORK_MODE_RF = 16,
	
	WORK_SCAN_TEC_VCSEL = 17,
	WORK_SCAN_RF_ATT = 18,
	
	WORK_FIND_DOPLER_TEMP = 19,
	
	WORK_TEST_VALUES = 20

};

#define VERSION_CPT_FLASH (1156)

#define PDKEY1 0x45670123
#define PDKEY2 0xCDEF89AB
// 3.19
#define FLASH_PAGE_61 0x0801E800
// 2.19
#define FLASH_PAGE_62 0x0801F000
// 5.19
#define FLASH_PAGE_63 0x0801F800

#define FLASH_PAGE_WORK FLASH_PAGE_62
#define FLASH_PAGE_ITEM 62

extern volatile uint32_t idSTMPart[3];
extern int DEBUG_maxDWT1;

extern int itemModeWork;
extern int itemStageWork_NONE;
extern int itemStageWork_SCAN_CRNT;
extern int itemStageWork_SCAN_FINE_TUNING_FREQ;
extern int itemStageWork_SCAN_TEMP;
extern int itemStageWork_SCAN_FREQ;
extern int itemStageWork_FIND_DOPLER_CRNT;
extern int itemStageWork_FIND_DOPLER_TEMP;
extern int itemStageWork_FIND_LORENZ;
extern int itemStageWork_SET_VCSEL_DC;
extern int itemStageWork_SET_ATT_PWR;
extern int itemStageWork_RUN;
extern int itemStageWork_STREAM_SCREEN;
extern int itemStageWork_TEST_LOOP_CRNT;
extern int itemStageWork_TEST_LOOP_FREQ;
extern int itemStageWork_MODE_CRNT;
extern int itemStageWork_MODE_RF;
extern int itemStageWork_SCAN_TEC_VCSEL;
extern int itemStageWork_SCAN_RF_ATT;

extern int status_CPT;
extern int updateStatus_CPT;

extern int ID_NAP_KPN;

extern int my_N_1;
extern int my_N_2;
extern int my_N_3;
extern int my_N_4;

extern int verProg;
extern uint32_t stateFuse;

#define RS232_ADMIN 				(0x1F)
#define RS232_MANUFACTURER 	(0x0F)
#define RS232_OPERATOR 			(0x03)
#define RS232_USER 					(0x01)

#define RS232_DEBUG					(0x10)
#define RS232_FIRST_RUN		  (0x08)
#define RS232_SETUP_PID			(0x04)
#define RS232_INFO					(0x02)
#define RS232_SIMPLE				(0x01)

extern int statusAccessRS232;


// Включение PID регуляторов
extern uint16_t statusLoopPID;
/*
	слово statusLoopPID состоит из 
	0й бит (маска 1<<0) - PID_FLAG_LOOP_DOPLER_CRNT			(0x0001)	// Захват пика поглощения током лазера
	1й бит (маска 1<<0) - PID_FLAG_LOOP_VCO							(0x0002)	// Захват частоты
	2й бит (маска 1<<0) - PID_FLAG_LOOP_TEC_CTRL				(0x0004)	// Захват температуры лазера по датчику
	3й бит (маска 1<<0) - PID_FLAG_LOOP_CELL_HTR				(0x0008)	// Захват температуры ячейки
	4й бит (маска 1<<0) - PID_FLAG_LOOP_AUX_TEC					(0x0010)	// Захват температуры хвостика ячейки
	5й бит (маска 1<<0) - PID_FLAG_LOOP_RF_ATT					(0x0020)	// Захват мощности СВЧ
	6й бит (маска 1<<0) - PID_FLAG_LOOP_DOPLER_TEMP 		(0x0040) 	// Захват температуры лазера по пику поглощения
	7й бит (маска 1<<0) - PID_FLAG_LOOP_OPTICS_PWR			(0x0080) 	// Захват тока лазера по постоянной составляющей
	8й бит (маска 1<<0) - PID_FLAG_LOOP_TEMP_FREQ				(0x0100)	// Компенсация смещение частоты от температуры
	9й бит (маска 1<<0) - PID_FLAG_LOOP_COMPCRNT_FREQ		(0x0200)	// Компенсация смещение частоты от тока
*/

#define STATUS_LOOP_TEMP_CELL					(0x0100)
#define STATUS_LOOP_VCSEL							(0x0200)	
#define STATUS_LOOP_VCO								(0x0400)	
#define STATUS_DC											(0x0800)
#define STATUS_VCO_CRS								(0x1000)
#define STATUS_PWR_VCSEL							(0x2000)
#define STATUS_PWR_CELL								(0x4000)
#define STATUS_LOOP_TEMP_VCSEL				(0x8000)


extern int item_direction_VCO_shift;

extern volatile bool flag_VCO_CRS;
extern volatile bool flag_VCO_FINE;
extern volatile bool flag_COIL_CRNT;
extern volatile bool flag_AUX_TEC;
extern volatile bool flag_VCSEL_DC;
extern volatile bool flag_VCSEL_DC_MOD;
extern volatile bool flag_RF_FREQ_MOD;
extern volatile bool flag_RF_ATT;
extern volatile bool flag_CELL_HTR;
extern volatile bool flag_TEC_CTRL;

extern volatile bool flag_LMX248;

extern int itemTypeModeFREQ;
extern int itemTypeModeCRNT;

extern int option_status;
// Начальный режим работы после запуска
extern int option_start_mode;
extern int option_VCSEL_DC;
extern int option_POWER_RF_ATT;
extern int option_AUX_TEC;
extern int option_COIL_CRNT;
extern int option_FIND_DC_PWR_RF_ATT;
extern int option_polarity_TEC;
extern int option_order_sleep;
extern int option_speed_uart;
extern int option_irq_uart;

float extern option_factor_A_TEMP_FREQ;
float extern option_factor_B_TEMP_FREQ;
float extern option_factor_C_TEMP_FREQ;

float extern option_factor_A_COIL;
float extern option_factor_B_COIL;
float extern option_factor_C_COIL;

extern int LMX_Current_PLL;

extern int LMX_ValueN;
extern int LMX_ValueFN;
extern int LMX_ValueFD;
extern int LMX_ValueINT;
extern float LMX_ShiftFreq;

extern int LMX_DataValueFN[256];
extern int LMX_DataValueFD[256];
extern int itemOptionLMX;

// Грубая настройка частоты (ALL 50 КГц)
extern int value_VCO_CRS;
// Тонкая настройка частоты (ALL 5 КГц)
extern int value_VCO_FINE;
// Магнитное поле
extern int value_COIL_CRNT;
// Настройка мощности охлаждения хвостика ячейки
extern int value_AUX_TEC;
// Настройка тока лазера
extern int value_VCSEL_DC;
// Настройка аттенюатора
extern int value_RF_ATT;
// Настройка мощности нагрева ячейки
extern int value_CELL_HTR;
// Настройка мощности нагрева/охлаждения лазера
extern int value_TEC_CTRL;
#define CRNT_TEC_OFF 65535

extern int fixValue_COIL_CRNT;
extern int fixValue_POWER_DC;
extern int fixValue_VCSEL_DC;
extern int fixValue_CELL_HTR;
extern int fixValue_AUX_TEC;
extern int fixValue_RF_ATT;
extern int fixValue_VCO_CRS;
extern int fixValue_VCO_FINE;

extern float resultAvrTempVCSEL;
extern int rawValueAvrTempVCSEL;
extern int rawValueDataTempVCSEL[10];
extern int indexDataTempVCSEL;
extern int countDataTempVCSEL;
extern bool flagAvrTempVCSEL;

extern float resultAvrTempCELL_HTR;
extern int rawValueAvrTempCELL_HTR;
extern int rawValueDataTempCELL_HTR[10];
extern int indexDataTempCELL_HTR;
extern int countDataTempCELL_HTR;
extern bool flagAvrTempCELL_HTR;

extern float resultAvrTempNTC_AUX1;
extern int rawValueAvrTempNTC_AUX1;
extern int rawValueDataTempNTC_AUX1[10];
extern int indexDataTempNTC_AUX1;
extern int countDataTempNTC_AUX1;
extern bool flagAvrTempNTC_AUX1;

extern float resultAvrTempNTC_AUX2;
extern int rawValueAvrTempNTC_AUX2;
extern int rawValueDataTempNTC_AUX2[10];
extern int indexDataTempNTC_AUX2;
extern int countDataTempNTC_AUX2;
extern bool flagAvrTempNTC_AUX2;

extern float valueAvrVCO;
extern float valueDataVCO[10];
extern int indexDataVCO;
extern int countDataVCO;
extern bool flagAvrVCO;

extern float valueAvrDOPLER;
extern float valueDataDOPLER[10];
extern int indexDataDOPLER;
extern int countDataDOPLER;
extern bool flagAvrDOPLER;

extern float resultValueTempMDL;
extern int rawValueAvrTempMDL;
extern int rawDataTempMDL[10];
extern int indexDataTempMDL;
extern int countDataTempMDL;
extern bool flagAvrTempMDL;
extern float levelValueTemp;

extern float resultValueTempUPR;
extern int rawValueAvrTempUPR;
extern int rawDataTempUPR[10];
extern int indexDataTempUPR;
extern int countDataTempUPR;
extern bool flagAvrTempUPR;

// Температура лазера
extern float valueTempVCSEL;
// Температура СВЧ генератора
extern float valueTempAUX1;
// Температура ячейки
extern float valueTempCELL;
// Температура хвостика ячейки
extern float valueTempAUX2;

// Температура АЦП MDL
extern float valueTempMDL;
// Температура АЦП UPR
extern float valueTempUPR;

// Отклонение тока от центра пика поглощения
extern float valueCRNT;
// Отклонение частоты от центра КПН
extern float valueFREQ;

// Разрешение на расчет компесанции мощности СВЧ
extern bool flagUpdateCamputeATT_MOD;
// Разрешение на расчет компенсации температуры СВЧ генератора
extern bool flagUpdateCamputeTempAUX1;
// Разрешение на расчет компенсации температуры хвостика ячейки
extern bool flagUpdateCamputeTempAUX2;
// Разрешение на расчет тока лазера для захвата пика поглощение 
extern bool flagUpdateCamputeVCSELDC;

// Ошибка температуры лазера (вылючаем нагрев)
extern bool isErrorTempVCSEL;
extern float levelLimitMaxTempVCSEL;
extern float levelLimitMinTempVCSEL;

// Начальная фаза модуляции тока
extern int startPhaseCRNT;
// Начальная фаза модуляции частоты
extern int startPhaseHF;
// Начальная фаза обновления АЦП записи модуляции тока
extern int startPhaseCRNT_ADC;
// Начальная фаза обновления  АЦП записи частоты
extern int startPhaseFREQ_ADC;
// Начальная фаза запуска потока работы с оборудованием
extern int startPhaseStream;
// Частота модуляции частоты СВЧ
extern int FreqModRF_ATT;
extern int FreqFREQ_ADC;

// Требуемый уровень температуры лазера
extern float levelTempVCSEL;
// Требуемый уровень температуры СВЧ генератора
extern float levelTempAUX1;
// Требуемый уровень температуры ячейки
extern float levelTempCELL;
// Требуемый уровень температуры хвостика ячейки
extern float levelTempAUX2;
// Уровень температуры ячейки при котором разрешен поиск пика поглощения
extern float levelStartTempCELL;

// Рабочий уровень оптической мощности
extern float levelWork_OPTICS_PWR;
// Рабочий уровень температуры
extern float levelWorkTempDC;

extern float sigmaPID_CELL_HTR;
extern int timeLoopPID_CELL_HTR;
extern int countLoopPID_CELL_HTR;
extern bool isLoopPID_CELL_HTR;

extern float sigmaPID_TEC_CTRL;
extern int timeLoopPID_TEC_CTRL;
extern int countLoopPID_TEC_CTRL;
extern bool isLoopPID_TEC_CTRL;

extern float sigmaPID_VCO;
extern int timeLoopPID_VCO;
extern int countLoopPID_VCO;
extern bool isLoopPID_VCO;


// Эталонный резистор для температуры лазера
extern float refResistVCSEL;
// Эталонный резистор для температуры СВЧ генератора
extern float refResistAUX1;
// Эталонный резистор для температуры ячейки
extern float refResistCELL;
// Эталонный резистор для температуры хвостика ячейки
extern float refResistAUX2;

// Расчет термокомпенсации по таблице
extern float tableCompensationFREQ[2][32];
extern int isTableCompensationFREQ[32];
extern int isUseTableCompensationFREQ;

// DAC MOD_CRNT SIZE
#define DAC_ARRAY_DMA_OUT2_SIZE 25
// DAC MOD_FREQ HALF
#define DAC_ARRAY_DMA_OUT1_HALF_SIZE 10

#define ADC_ARRAY_DMA_ADC1_HALF_SIZE 80
#define ADC_ARRAY_DMA_ADC2_HALF_SIZE 80
#define ADC_ARRAY_DMA_ADC3_HALF_SIZE 160

#define COUNT_DATA_HALF_SIZE_CRNT 80
#define COUNT_DATA_HALF_SIZE_FREQ 80
#define COUNT_DATA_HALF_SIZE_DC 80
#define COUNT_DATA_HALF_SIZE_RF 80

#define COUNT_DATA_SAVE 400

// Количество отсчетов ADC в периоде модуляции
#define LENGTH_PERIOD_CRNT 5
#define LENGTH_PERIOD_FREQ 8
#define LENGTH_PERIOD_RF 20
#define PERIOD_20KHZ 4

extern float tableCRNT_Sin[LENGTH_PERIOD_CRNT];
extern float tableCRNT_Cos[LENGTH_PERIOD_CRNT];
extern float phaseTableCRNT;
										
extern float tableFREQ_Sin[LENGTH_PERIOD_FREQ];
extern float tableFREQ_Cos[LENGTH_PERIOD_FREQ];
extern float phaseTableFREQ;

extern float tableRF_Sin[LENGTH_PERIOD_RF];
extern float tableRF_Cos[LENGTH_PERIOD_RF];
extern float phaseTableRF; 

extern float tableFREQ20KHZ_Sin[PERIOD_20KHZ];
extern float tableFREQ20KHZ_Cos[PERIOD_20KHZ];
extern float phaseTableFREQ20KHZ;

extern float amplitudeModulationCRNT;
extern float phaseModulationCRNT;

extern float amplitudeModulationFREQ;
extern float phaseModulationFREQ;

extern float amplitudeModulationFREQ20KHZ;
extern float phaseModulationFREQ20KHZ;

extern uint16_t DMA_DAC1_Data[DAC_ARRAY_DMA_OUT1_HALF_SIZE*2];
extern uint16_t DMA_DAC2_Data[DAC_ARRAY_DMA_OUT2_SIZE];

extern uint16_t DMA_ADC1_Data[ADC_ARRAY_DMA_ADC1_HALF_SIZE*2];
extern uint16_t DMA_ADC2_Data[ADC_ARRAY_DMA_ADC2_HALF_SIZE*2];
extern uint16_t DMA_ADC3_Data[ADC_ARRAY_DMA_ADC3_HALF_SIZE*2];

extern int resultData_CRNT[COUNT_DATA_HALF_SIZE_CRNT];
extern int resultData_FREQ[COUNT_DATA_HALF_SIZE_FREQ];
extern int resultData_DC[COUNT_DATA_HALF_SIZE_DC];
extern int resultData_RF[COUNT_DATA_HALF_SIZE_RF];

extern int sumCRNT_Q[LENGTH_PERIOD_CRNT];
extern int sumCRNT_I[LENGTH_PERIOD_CRNT];

extern int sumFREQ_Q[LENGTH_PERIOD_FREQ];
extern int sumFREQ_I[LENGTH_PERIOD_FREQ];

extern int sumRF_Q[LENGTH_PERIOD_RF];
extern int sumRF_I[LENGTH_PERIOD_RF];

extern uint16_t* ptrDataRF;
extern uint16_t* ptrDataDC;
extern uint16_t* ptrDataCRNT;
extern uint16_t* ptrDataFREQ;

extern uint8_t itemPartResultDMA1_ADC1;
extern uint8_t itemPartResultDMA1_ADC2;
extern uint8_t itemPartResultDMA1_ADC3;

extern int itemScreenSave;
extern bool isPowerLaser;

extern int saveData_CRNT[COUNT_DATA_SAVE];
extern int saveData_FREQ[COUNT_DATA_SAVE];
extern int indexSaveCRNT;
extern int indexSaveFREQ;
extern int indexSaveScreen;
extern bool isRecordScreenCRNT;
extern bool isScreenSaveDoneCRNT;
extern bool isRecordScreenFREQ;
extern bool isScreenSaveDoneFREQ;


extern int itemSourceDataRF;
extern int countAvrDetectorRF;
extern int shiftIndexDetectorRF;

// Источник данных для синхронного детектора тока
extern int itemSourceDataCRNT;
extern int countAvrDetectorCRNT;
extern int shiftIndexDetectorCRNT;

// Источник данных для синхронного детектора частоты
extern int itemSourceDataFREQ;
extern int countAvrDetectorFREQ;
extern int shiftIndexDetectorFREQ;

//=========================================================
// Оптическая мощность
//=========================================================
extern int value_ADC_OPTICS_PWR;
extern int indexAverage_ADC_OPTICS_PWR;
extern int countAverage_ADC_OPTICS_PWR;
extern float resultAverage_ADC_OPTICS_PWR;
//=========================================================
// Синхронный детектор мощности СВЧ
//=========================================================
extern float value_Q_RF;
extern float value_I_RF;
extern int value_ADC_RF;
extern int indexAverage_ADC_RF;
extern int countAverage_ADC_RF;
extern float shiftDetector_RF;
extern float resultAverage_Magnitude_RF;
extern float resultAverage_Phase_RF;
extern float resultAverage_ADC_RF;
extern bool flagUpdateCampute_ADC_RF;
//=========================================================
// Синхронный детектор доплера
//=========================================================
extern float value_Q_DOPLER;
extern float value_I_DOPLER;
extern int indexAverage_ADC_DOPLER;
extern int countAverage_ADC_DOPLER;
//extern float shiftResultMagnitude_DOPLER;
extern float shiftDetector_DOPLER;
//=========================================================
// Синхронный детектор частоты
//=========================================================
extern float value_Magnitude_FREQ;
extern float value_Q_FREQ;
extern float value_I_FREQ;
extern int indexAverage_ADC_FREQ;
extern int countAverage_ADC_FREQ;
//extern float shiftResultMagnitude_FREQ;
extern float shiftDetector_FREQ;
extern float ADC_FREQ;
//=========================================================
// Синхронный детектор доплера для поиска
//=========================================================
extern int value_ADC_SRCH;
extern int indexAverage_ADC_SRCH;
extern int countAverage_ADC_SRCH;
extern float resultAverage_ADC_SRCH;
extern bool flagUpdateCampute_ADC_SRCH;

// Машина состояний для 24 разрядных АЦП
extern volatile uint8_t stageAD7794upr;
extern volatile uint8_t stageAD7794mdl;

// Поиск пика поглощения
extern int VCSEL_ScanValue_X;
extern float VCSEL_ScanValue_Y;
extern int VCSEL_indexPeak;
extern int VCSEL_countPeak;
extern int VCSEL_listPeak_X[100];
extern float VCSEL_listPeak_Y[100];
extern float VCSEL_levelDetector;
extern float VCSEL_levelSigma;
extern int VCSEL_ScanLimitMin;
extern int VCSEL_ScanLimitMax;
extern int itemModePeak;
extern int VCSEL_LocalBeginPeak_X;
extern float VCSEL_LocalBeginPeak_Y;
extern int VCSEL_LocalEndPeak_X;
extern float VCSEL_LocalEndPeak_Y;
extern int VCSEL_LocalMaxPeak_X;
extern float VCSEL_LocalMaxPeak_Y;
extern int VCSEL_GlobalMaxPeak_X;
extern float VCSEL_GlobalMaxPeak_Y;
extern int VCSEL_ScanValue;
extern int VCSEL_ScanStep;
extern int VCSEL_ScanIndex_P1;
extern int VCSEL_ScanIndex_P2;
extern int VCSEL_ScanIndex;
extern int VCSEL_dataX[11];
extern float VCSEL_dataY[11];
extern int VCSEL_tiks;
extern int VCSEL_tiksDiv;
extern bool VCSEL_isReverse;
extern bool isDoneFindDopler;

extern int VCO_indexPeak;
extern int VCO_ScanValue_X;
extern float VCO_ScanValue_Y;
extern int VCO_ScanLimitMin;
extern int VCO_ScanLimitMax;
extern int VCO_GlobalMaxPeak_X[3];
extern float VCO_GlobalMaxPeak_Y[3];
extern int VCO_GlobalPeakTriger;
extern int VCO_ScanValue;
extern int VCO_ScanStep;
extern int VCO_ScanIndex;
extern int VCO_ScanIndex_P1;
extern int VCO_ScanIndex_P2;
extern int VCO_dataX[5];
extern float VCO_dataY[5];
extern int VCO_tiks;
extern int VCO_tiksDiv;
extern bool isDoneFindLorenz;

// Сканирование током лазера 
//extern int SCAN_CRNT_LimitMin;
//extern int SCAN_CRNT_LimitMax;
//extern int SCAN_CRNT_Step;

// Сканирование температурой лазера 
extern float SCAN_TEMP_LimitMin;
extern float SCAN_TEMP_LimitMax;
extern float SCAN_TEMP_Step;

// Сканирование мощности TEC VCSEL
extern int SCAN_TEC_VCSEL_Step;
extern int SCAN_TEC_VCSEL_LimitMin;
extern int SCAN_TEC_VCSEL_LimitMax;
extern float SCAN_TEC_VCSEL_levelDetector;
// Сканирование частотой генератора
//extern int SCAN_FREQ_LimitMin;
//extern int SCAN_FREQ_LimitMax;
//extern int SCAN_FREQ_Step;

extern int SCAN_RF_ATT_Step;
extern int SCAN_RF_ATT_LimitMin;
extern int SCAN_RF_ATT_LimitMax;

// Плавная перестройка тока лазера
extern int SVD_step;
extern bool SVD_IsDone;
extern int SVD_tiksDiv;
extern int SVD_tiksCount;

// Плавная перестройка мощности СВЧ
extern int SATTP_step;
extern bool SATTP_IsDone;
extern int SATTP_tiksDiv;
extern int SATTP_tiksCount; // 10 Hz

// Плавная перестройка частоты СВЧ генератора
extern bool SWAVE_IsDone;
extern int SWAVE_tiksCount;

extern int TESTLOOP_CRNT_step;

extern int TESTLOOP_FREQ_step;

//=========================================================
// Оптическая мощность
//=========================================================
extern float value_PDA_OPTICS_PWR;
extern int indexAverage_PDA_OPTICS_PWR;
extern int countAverage_PDA_OPTICS_PWR;
extern float resultAverage_PDA_OPTICS_PWR;
extern float lvlValue_PDA_OPTICS_PWR;
extern bool flagUpdateCampute_PDA_OPTICS_PWR;

//=========================================================
// СВЧ мощность
//=========================================================
extern float value_PDA_RF_ATT[2];
extern int indexAverage_PDA_RF_ATT;
extern int countAverage_PDA_RF_ATT;
extern float resultAverage_PDA_RF_ATT[2];
extern float lvlValue_PDA_RF_ATT;
extern bool flagUpdateCampute_PDA_RF_ATT;

//
extern float COMP_CRNT_A;
extern float COMP_CRNT_B;
extern float COMP_CRNT_C;
extern int LMX_ValueINT_CRNT_CELL;
//=========================================================
// Данные для работы цикличного буфера UART
//=========================================================
extern volatile uint8_t   	USART_tx_buffer[256];
extern volatile uint16_t   	USART_tx_count; // Количество данных на отправку
extern volatile uint16_t   	USART_tx_wr_index; // Индекс хвоста буфера (куда писать данные )
extern volatile uint16_t   	USART_tx_rd_index; // Индекс начала буфера (откуда читать данные)
extern volatile bool    		USART_tx_buffer_overflow; // Флаг переполнения буфера
extern volatile uint8_t   	USART_rx_buffer[256];
extern volatile uint16_t   	USART_rx_count; // Количество данных в принятом буфере
extern volatile uint16_t   	USART_rx_wr_index; // Индекс хвоста буфера (куда писать данные )
extern volatile uint16_t   	USART_rx_rd_index; // Индекс начала буфера (откуда читать данные)
extern volatile bool    		USART_rx_buffer_overflow; // Флаг переполнения буфера
extern volatile bool 				USART_TransmitComplete;

extern uint8_t BufferMsg[256];
extern uint16_t lengthBufferCmd;
extern int byteUSART;
extern int cmdID;
extern int BufferDataPack[4];

//=========================================================
// Делители таймера (прерывание)
//=========================================================
extern bool  flagUpdateAD7794;
extern uint32_t tiksDivAD7794;
extern uint32_t tiksCountAD7794;
extern uint32_t tiksCountSend;

#endif
