#include "var.h"

/*
	слово permission состоит из
	0й бит - flagUpdateCampute_ADC_DOPLER					Синхронный детектор доплера
	1й бит - flagUpdateCampute_ADC_FREQ						Синхронный детектор частоты
	2й бит - flagUpdateCamputeTempVCSEL						Разрешение на расчет компенсации температуры лазера
	3й бит - flagUpdateCamputeTempCELL						Разрешение на расчет компенсации температуры ячейки
	4й бит - занят, но не использован
	5й бит - занят, но не использован
	6й бит - flagUpdateCamputeDOPLERTECCTRL				Разрешение на расчет нагрева лазера для захвата пика поглощение
	7й бит - flagUpdateCampute_ADC_OPTICS_PWR			Оптическая мощность
	8й бит - flagUpdateCamputeTempFreq						Разрешение на расчет компенсации сдвига частоты от температуры
	9й бит - flagUpdateCamputeCompCRNTFreq				Разрешение на расчет компенсации сдвига частоты от температуры
*/
uint16_t permission = 0;

// в структуре хранятся данные о пределах и шаге сканирования мощностью нагрева лазера (heating capacity)
struct prop_consid heat_cap = {35000, 45000, 5};
uint16_t pid_actuation = 0;
int desired_crnt = 0;

int moddemod = 10000;
//начало моих переменных
uint32_t aa,bb,cc,dd;
float ADC_FREQ_90 = 0;
float FREQ_POS = 0;
float FREQ_NEG = 0;
short milisec = -1;

float ordinates[3];
float QQ = 0.0f;
float II = 0.0f;
float Magnitude_DOPLER;
float Phase_DOPLER = 0.0f;

int LMX_ValueN_N_1 = 0;
int LMX_ValueFN_N_1 = 0;
int LMX_ValueFD_N_1 = 1000000;

int LMX_ValueN_N_2 = 0;
int LMX_ValueFN_N_2 = 0;
int LMX_ValueFD_N_2 = 1000000;

int my_N12 = 0;
int my_N12_ = 0;

float ADC_FREQ_0 = 0.0f;
float ADC_FREQ_1 = 0.0f;
float ADC_FREQ_P = 0.0f;
float ADC_FREQ_N = 0.0f;

float RESULT_ADC_FREQ = 0.0f;

float FQ = 0.0f; //действительная часть
float FI = 0.0f;//мнимая часть
float Magnitude_FREQ = 0.0f;//модуль числа
float Phase_FREQ = 0.0f;//фаза

bool perm_auto_capt = false;
wavelength discr_wave = {1, 1, 0, 0, 0, 0, 0, 0, 0};
dbg debug_vars = {0, 0, 0, 0, 0, 0};
//конец моих переменных
volatile uint32_t idSTMPart[3] = {0, 0, 0};
int DEBUG_maxDWT1 = 0;

// Режим работы
int itemModeWork = 0;
int itemStageWork_NONE = 0;
int itemStageWork_RUN = 0;
int itemStageWork_SCAN_CRNT = 0;
int itemStageWork_SCAN_TEMP = 0;
int itemStageWork_SCAN_FREQ = 0;
int itemStageWork_FIND_DOPLER_CRNT = 0;
int itemStageWork_FIND_DOPLER_TEMP = 0;
int itemStageWork_FIND_LORENZ = 0;
int itemStageWork_SET_VCSEL_DC = 0;
int itemStageWork_SET_ATT_PWR = 0;

int itemStageWork_STREAM_SCREEN = 0;
int itemStageWork_TEST_LOOP_CRNT = 0;
int itemStageWork_TEST_LOOP_FREQ = 0;

int itemStageWork_MODE_CRNT = 0;
int itemStageWork_MODE_RF = 0;

int itemStageWork_SCAN_TEC_VCSEL = 0;
int itemStageWork_SCAN_RF_ATT = 0;

int itemStageWork_SCAN_FINE_TUNING_FREQ = 0;

int status_CPT = 0;
int updateStatus_CPT = 0;

int ID_NAP_KPN = 0xFEABCDEF;

int verProg = 20210126;
uint32_t stateFuse = 0;

int my_N_1 = 0;
int my_N_2 = 0;
int my_N_3 = 0;
int my_N_4 = 0;

int statusAccessRS232 = RS232_ADMIN;

// Включение PID регуляторов
uint16_t statusLoopPID = 0;

// Разрешение на обновление значений
volatile bool flag_VCO_CRS = false;
volatile bool flag_VCO_FINE = false;
volatile bool flag_COIL_CRNT = false;
volatile bool flag_AUX_TEC = false;
volatile bool flag_VCSEL_DC = false;
volatile bool flag_VCSEL_DC_MOD = false;
volatile bool flag_RF_FREQ_MOD = false;
volatile bool flag_RF_ATT = false;
volatile bool flag_CELL_HTR = false;
volatile bool flag_TEC_CTRL = false;

volatile bool flag_LMX248 = false;

int itemTypeModeFREQ = 0;
int itemTypeModeCRNT = 0;

// Заранее заданные оптимальные настройки
int option_status = -1;
// Начальный режим работы после запуска
int option_start_mode = 0;
int option_VCSEL_DC = 52000;
int option_POWER_RF_ATT = 53000;
int option_AUX_TEC = 42000;
int option_COIL_CRNT = 30000;
int option_FIND_DC_PWR_RF_ATT = 65000;
int option_polarity_TEC = -1;//0;
int option_order_sleep = 0;
int option_speed_uart = 4800;
int option_irq_uart = 0;


float option_factor_A_TEMP_FREQ = 0.783f;
float option_factor_B_TEMP_FREQ = -91.28f;
float option_factor_C_TEMP_FREQ = 2648.24f;

float option_factor_A_COIL = 0.0f;
float option_factor_B_COIL = 0.0f;
float option_factor_C_COIL = 0.0f;

int LMX_Current_PLL = 9;
// 4 596 ххх ххх
int LMX_ValueN = 919;
int LMX_ValueFN = 0;
int LMX_ValueFD = 400000;
int LMX_ValueINT = 0;
float LMX_ShiftFreq = 0.0f;

int LMX_DataValueFN[256];
int LMX_DataValueFD[256];
int itemOptionLMX = 128;

int item_direction_VCO_shift = 0;

// Грубая настройка частоты (ALL 50 КГц)
int value_VCO_CRS = 44180; //0; //42570;//
// Тонкая настройка частоты (ALL 5 КГц)
int value_VCO_FINE = 32768;
// Магнитное поле
int value_COIL_CRNT = 0;
// Настройка мощности охлаждения хвостика ячейки
int value_AUX_TEC = 0;
// Настройка тока лазера
int value_VCSEL_DC = 0;
// Настройка аттенюатора
int value_RF_ATT = 10000;
// Настройка мощности нагрева ячейки
int value_CELL_HTR = 0;
// Настройка мощности нагрева/охлаждения лазера
int value_TEC_CTRL = 65535;
//int value_TEC_CTRL = 0;
#warning TEC_MAX

int fixValue_COIL_CRNT = 0;
// Для удержания постоянной составляющей
int fixValue_POWER_DC = 0;
// Для удержания длины волны лазера его током
int fixValue_VCSEL_DC = 0;
int fixValue_CELL_HTR = 0;
int fixValue_AUX_TEC = 0;
int fixValue_RF_ATT = 0;
int fixValue_VCO_CRS = 0;
int fixValue_VCO_FINE = 0;

float resultAvrTempVCSEL = 0.0f;
int rawValueAvrTempVCSEL = 0;
int rawValueDataTempVCSEL[10];
int indexDataTempVCSEL = 0;
int countDataTempVCSEL = 10;
bool flagAvrTempVCSEL = true;

float resultAvrTempCELL_HTR = 0.0f;
int rawValueAvrTempCELL_HTR = 0;
int rawValueDataTempCELL_HTR[10];
int indexDataTempCELL_HTR = 0;
int countDataTempCELL_HTR = 10;
bool flagAvrTempCELL_HTR = true; // включили среднее

float resultAvrTempNTC_AUX1 = 0.0f;
int rawValueAvrTempNTC_AUX1 = 0;
int rawValueDataTempNTC_AUX1[10];
int indexDataTempNTC_AUX1 = 0;
int countDataTempNTC_AUX1 = 10;
bool flagAvrTempNTC_AUX1 = false;

float resultAvrTempNTC_AUX2 = 0.0f;
int rawValueAvrTempNTC_AUX2 = 0;
int rawValueDataTempNTC_AUX2[10];
int indexDataTempNTC_AUX2 = 0;
int countDataTempNTC_AUX2 = 10;
bool flagAvrTempNTC_AUX2 = false;

float resultValueTempMDL = 0.0f;
int rawValueAvrTempMDL = 0;
int rawDataTempMDL[10];
int indexDataTempMDL = 0;
int countDataTempMDL = 10;
bool flagAvrTempMDL = false;
float levelValueTemp = 35.5f;

float resultValueTempUPR = 0.0f;
int rawValueAvrTempUPR = 0;
int rawDataTempUPR[10];
int indexDataTempUPR = 0;
int countDataTempUPR = 10;
bool flagAvrTempUPR = false;

float valueAvrVCO = 0.0f;
float valueDataVCO[10];
int indexDataVCO = 0;
int countDataVCO = 10;
bool flagAvrVCO = false;

float valueAvrDOPLER = 0.0f;
float valueDataDOPLER[10];
int indexDataDOPLER = 0;
int countDataDOPLER = 10;
bool flagAvrDOPLER = false;

// Температура лазера
float valueTempVCSEL = 0.0f;
// Температура СВЧ генератора
float valueTempAUX1 = 0.0f;
// Температура ячейки
float valueTempCELL = 0.0f;
// Температура хвостика ячейки
float valueTempAUX2 = 0.0f;

// Температура АЦП MDL
float valueTempMDL = 0.0f;
// Температура АЦП UPR
float valueTempUPR = 0.0f;

// Отклонение тока от центра пика поглощения
float valueCRNT = 0.0f;
// Отклонение частоты от центра КПН
float valueFREQ = 0.0f;

// Разрешение на расчет компесанции мощности СВЧ
bool flagUpdateCamputeATT_MOD = false;
// Разрешение на расчет компенсации температуры СВЧ генератора
bool flagUpdateCamputeTempAUX1 = false;
// Разрешение на расчет компенсации температуры хвостика ячейки
bool flagUpdateCamputeTempAUX2 = false;
// Разрешение на расчет тока лазера для захвата пика поглощение 
bool flagUpdateCamputeVCSELDC = false;

// Ошибка температуры лазера (вылючаем нагрев)
bool isErrorTempVCSEL = false;
float levelLimitMaxTempVCSEL = 65.0f;
float levelLimitMinTempVCSEL = 40.0f;

// Требуемый уровень температуры лазера
float levelTempVCSEL = 58.66f;
// Требуемый уровень температуры СВЧ генератора
float levelTempAUX1 = 0.0f;
// Требуемый уровень температуры ячейки
float levelTempCELL = 60.0f;
// Требуемый уровень температуры хвостика ячейки
float levelTempAUX2 = 0.0f;
// Уровень температуры ячейки при котором начинается поиск пиков поглощения
float levelStartTempCELL = 55.0f;

// Рабочий уровень оптической мощности
float levelWork_OPTICS_PWR = 0;
// Рабочий уровень температуры
float levelWorkTempDC = 0;

// Индикация захвата температуры ячейки
float sigmaPID_CELL_HTR = 0.5;
int timeLoopPID_CELL_HTR = 500; // 100/50 Hz = 10s
int countLoopPID_CELL_HTR = 0;
bool isLoopPID_CELL_HTR = false;

// Индикация захвата температуры лазера
float sigmaPID_TEC_CTRL = 0.15;
int timeLoopPID_TEC_CTRL = 100; // 50/50 Hz = 1s
int countLoopPID_TEC_CTRL = 0;
bool isLoopPID_TEC_CTRL = false;

// Индикация захвата частоты
float sigmaPID_VCO = 75.0;
int timeLoopPID_VCO = 250; // 250 / 250 Hz = 1s
int countLoopPID_VCO = 0;
bool isLoopPID_VCO = false;

// Эталонный резистор для температуры лазера
float refResistVCSEL = 13300.0f;
// Эталонный резистор для температуры СВЧ генератора
float refResistAUX1 = 13300.0f;
// Эталонный резистор для температуры ячейки
float refResistCELL = 13300.0f;
// Эталонный резистор для температуры хвостика ячейки
float refResistAUX2 = 13300.0f;

// Расчет термокомпенсации по таблице
// Температура // Частота
float tableCompensationFREQ[2][32];
int isTableCompensationFREQ[32];
int isUseTableCompensationFREQ = 0;


float tableCRNT_Sin[LENGTH_PERIOD_CRNT];
float tableCRNT_Cos[LENGTH_PERIOD_CRNT];
float phaseTableCRNT = 5.06145483f; // 290 градусов //6.23257065f; // (357,1)
									 
float tableFREQ_Sin[LENGTH_PERIOD_FREQ];
float tableFREQ_Cos[LENGTH_PERIOD_FREQ];
float phaseTableFREQ = 3.08923277f; // (177,0)

float tableRF_Sin[LENGTH_PERIOD_RF];
float tableRF_Cos[LENGTH_PERIOD_RF];
float phaseTableRF = -0.87266463f;

float tableFREQ20KHZ_Sin[PERIOD_20KHZ];
float tableFREQ20KHZ_Cos[PERIOD_20KHZ]; 
float phaseTableFREQ20KHZ = 3.08923277f; // (177,0)

float amplitudeModulationCRNT = 10.0f;
float phaseModulationCRNT = 0.0f;

float amplitudeModulationFREQ = 1200.0f;
float phaseModulationFREQ = 0.0f;

float amplitudeModulationFREQ20KHZ = 1200.0f;
float phaseModulationFREQ20KHZ = 0.0f;

uint16_t DMA_DAC1_Data[DAC_ARRAY_DMA_OUT1_HALF_SIZE*2]__attribute__((aligned(4)));
uint16_t DMA_DAC2_Data[DAC_ARRAY_DMA_OUT2_SIZE]__attribute__((aligned(4)));

uint16_t DMA_ADC1_Data[ADC_ARRAY_DMA_ADC1_HALF_SIZE*2]__attribute__((aligned(4)));
uint16_t DMA_ADC2_Data[ADC_ARRAY_DMA_ADC2_HALF_SIZE*2]__attribute__((aligned(4)));
uint16_t DMA_ADC3_Data[ADC_ARRAY_DMA_ADC3_HALF_SIZE*2]__attribute__((aligned(4)));

int resultData_CRNT[COUNT_DATA_HALF_SIZE_CRNT];
int resultData_FREQ[COUNT_DATA_HALF_SIZE_FREQ];
int resultData_DC[COUNT_DATA_HALF_SIZE_DC];
int resultData_RF[COUNT_DATA_HALF_SIZE_RF];

int sumCRNT_Q[LENGTH_PERIOD_CRNT];
int sumCRNT_I[LENGTH_PERIOD_CRNT];

int sumFREQ_Q[LENGTH_PERIOD_FREQ];
int sumFREQ_I[LENGTH_PERIOD_FREQ];

int sumRF_Q[LENGTH_PERIOD_RF];
int sumRF_I[LENGTH_PERIOD_RF];

uint16_t* ptrDataRF;
uint16_t* ptrDataDC;
uint16_t* ptrDataCRNT;
uint16_t* ptrDataFREQ;

uint8_t itemPartResultDMA1_ADC1 = 0;
uint8_t itemPartResultDMA1_ADC2 = 0;
uint8_t itemPartResultDMA1_ADC3 = 0;

int itemScreenSave = 0;
bool isPowerLaser = true;

int saveData_CRNT[COUNT_DATA_SAVE];
int saveData_FREQ[COUNT_DATA_SAVE];
int indexSaveCRNT = 0;
int indexSaveFREQ = 0;
int indexSaveScreen = 0;
bool isRecordScreenCRNT = false;
bool isScreenSaveDoneCRNT = false;
bool isRecordScreenFREQ = false;
bool isScreenSaveDoneFREQ = false;


int itemSourceDataRF = 0; // SRCH = 0 CRNT = 1 DC = 2 FREQ = 3
int countAvrDetectorRF = LENGTH_PERIOD_RF; // 
int shiftIndexDetectorRF = 0;

// Источник данных для синхронного детектора тока
int itemSourceDataCRNT = 1; // SRCH = 0 CRNT = 1 DC = 2 FREQ = 3
int countAvrDetectorCRNT = LENGTH_PERIOD_CRNT; // 
int shiftIndexDetectorCRNT = 0;

// Источник данных для синхронного детектора частоты
int itemSourceDataFREQ = 3; // SRCH = 0 CRNT = 1 DC = 2 FREQ = 3
int countAvrDetectorFREQ = LENGTH_PERIOD_FREQ;
int shiftIndexDetectorFREQ = 0;

//=========================================================
// Оптическая мощность
//=========================================================
int value_ADC_OPTICS_PWR = 0;
int indexAverage_ADC_OPTICS_PWR = 0;
int countAverage_ADC_OPTICS_PWR = 100; 
float resultAverage_ADC_OPTICS_PWR = 0;
//=========================================================
// Синхронный детектор мощности СВЧ
//=========================================================
float value_Q_RF = 0.0f;
float value_I_RF = 0.0f;
int value_ADC_RF = 0;
int indexAverage_ADC_RF = 0;
int countAverage_ADC_RF = 40000; 
float shiftDetector_RF = 0.0f;
float resultAverage_Magnitude_RF = 0.0f; 
float resultAverage_Phase_RF = 0.0f; 
float resultAverage_ADC_RF = 0;
bool flagUpdateCampute_ADC_RF = false;
//=========================================================
// Синхронный детектор доплера
//=========================================================
float value_Q_DOPLER = 0.0f;
float value_I_DOPLER = 0.0f;
int indexAverage_ADC_DOPLER = 0;
//float shiftResultMagnitude_DOPLER = 0.0f;
float shiftDetector_DOPLER = 0.0;
//=========================================================
// Синхронный детектор частоты
//=========================================================
float value_Magnitude_FREQ = 0;
float value_Q_FREQ = 0.0f;
float value_I_FREQ = 0.0f;
int indexAverage_ADC_FREQ = 0;
int countAverage_ADC_FREQ = 200; // count * 150000Hz / 15v = 20 Hz
//float shiftResultMagnitude_FREQ = 0.0f;
float shiftDetector_FREQ = 0.0;
float ADC_FREQ = 0;
//=========================================================
// Синхронный детектор доплера для поиска
//=========================================================
int value_ADC_SRCH = 0;
int indexAverage_ADC_SRCH = 0;
int countAverage_ADC_SRCH = 75; // 200 Hz
float resultAverage_ADC_SRCH = 0;
bool flagUpdateCampute_ADC_SRCH = false;

// Машина состояний для 24 разрядных АЦП
volatile uint8_t stageAD7794upr = 0;
volatile uint8_t stageAD7794mdl = 0;

// Поиск пика поглощения (ток)
int VCSEL_ScanValue_X = 0;
float VCSEL_ScanValue_Y = 0;
int VCSEL_indexPeak = 0;
int VCSEL_countPeak = 0;
int VCSEL_listPeak_X[100];
float VCSEL_listPeak_Y[100];
// Уровень детектирования пика
float VCSEL_levelDetector = 50.0f;
float VCSEL_levelSigma = 20.0f;
int VCSEL_ScanLimitMin = 35000;
int VCSEL_ScanLimitMax = 55000;
int itemModePeak = 0;
int VCSEL_LocalBeginPeak_X = 0;
float VCSEL_LocalBeginPeak_Y = 0;
int VCSEL_LocalEndPeak_X = 0;
float VCSEL_LocalEndPeak_Y =0;
int VCSEL_LocalMaxPeak_X = 0;
float VCSEL_LocalMaxPeak_Y = 0;
int VCSEL_GlobalMaxPeak_X = 0;
float VCSEL_GlobalMaxPeak_Y = 0;
int VCSEL_ScanValue = 0;
int VCSEL_ScanStep = 10;
// Скользящая симметричная производная +-5
int VCSEL_ScanIndex_P1 = 0;
int VCSEL_ScanIndex_P2 = 0;
int VCSEL_ScanIndex = 0;
int VCSEL_tiks = 0;
int VCSEL_tiksDiv = 10;
int VCSEL_dataX[11];
float VCSEL_dataY[11];
bool VCSEL_isReverse = false;
bool isDoneFindDopler = false;

// Поиск пика КПН
int VCO_indexPeak = 0;
int VCO_ScanValue_X = 0;
float VCO_ScanValue_Y = 0;
int VCO_ScanLimitMin = 30000;
int VCO_ScanLimitMax = 65000;
int VCO_GlobalMaxPeak_X[3] = {0, 0, 0};
float VCO_GlobalMaxPeak_Y[3] = {0.0f, 0.0f, 0.0f};
int VCO_GlobalPeakTriger = 0;
int VCO_ScanValue = 0;
int VCO_ScanStep = 50;
// Скользящая симметричная производная +-2
int VCO_ScanIndex = 0;
int VCO_ScanIndex_P1 = 0;
int VCO_ScanIndex_P2 = 4;
int VCO_dataX[5];
float VCO_dataY[5];
int VCO_tiks = 0;
int VCO_tiksDiv = 10;
bool isDoneFindLorenz = false;

// Сканирование током лазера 
//int SCAN_CRNT_LimitMin = 35000;
//int SCAN_CRNT_LimitMax = 60000;
//int SCAN_CRNT_Step = 20;

// Сканирование температурой лазера 
float SCAN_TEMP_LimitMin = 45.0f;
float SCAN_TEMP_LimitMax = 65.0f;
float SCAN_TEMP_Step = 0.01f;

int SCAN_TEC_VCSEL_Step = 50;
int SCAN_TEC_VCSEL_LimitMin = 10000;
int SCAN_TEC_VCSEL_LimitMax = 65535;
float SCAN_TEC_VCSEL_levelDetector = 200.0f;
// Сканирование частотой генератора
//int SCAN_FREQ_LimitMin = 0;
//int SCAN_FREQ_LimitMax = 65000;
//int SCAN_FREQ_Step = 50;

int SCAN_RF_ATT_Step = -50;
int SCAN_RF_ATT_LimitMin = 30000;
int SCAN_RF_ATT_LimitMax = 65535;

// Плавная перестройка тока лазера
int SVD_step = 75;
bool SVD_IsDone = false;
int SVD_tiksDiv = 0;
int SVD_tiksCount = 100; // 10 Hz

// Плавная перестройка мощности СВЧ
int SATTP_step = 250;
bool SATTP_IsDone = false;
int SATTP_tiksDiv = 0;
int SATTP_tiksCount = 100; // 10 Hz

// Плавная перестройка частоты СВЧ генератора
bool SWAVE_IsDone = false;
int SWAVE_tiksCount = 100; // 10 Hz

// Единичный скачок тока для тестирования ПИД регулятора
int TESTLOOP_CRNT_step = 40;
// Единичный скачок частоты для тестирования ПИД регулятора
int TESTLOOP_FREQ_step = 70;

//=========================================================
// Оптическая мощность
//=========================================================
float value_PDA_OPTICS_PWR = 0;
int indexAverage_PDA_OPTICS_PWR = 0;
int countAverage_PDA_OPTICS_PWR = 125; 
float resultAverage_PDA_OPTICS_PWR = 0;
float lvlValue_PDA_OPTICS_PWR = 0;
bool flagUpdateCampute_PDA_OPTICS_PWR = false;

//=========================================================
// СВЧ мощность
//=========================================================
float value_PDA_RF_ATT[2];
int indexAverage_PDA_RF_ATT = 0;
int countAverage_PDA_RF_ATT = 250; 
float resultAverage_PDA_RF_ATT[2];
float lvlValue_PDA_RF_ATT = 0;
bool flagUpdateCampute_PDA_RF_ATT = false;

//
float COMP_CRNT_A = 0.0f;
float COMP_CRNT_B = 0.0f;
float COMP_CRNT_C = 0.0f;
int LMX_ValueINT_CRNT_CELL = 0;
//=========================================================
// Данные для работы цикличного буфера UART
//=========================================================
volatile uint8_t   	USART_tx_buffer[256];
volatile uint16_t   USART_tx_count = 0; // Количество данных на отправку
volatile uint16_t   USART_tx_wr_index = 0; // Индекс хвоста буфера (куда писать данные )
volatile uint16_t   USART_tx_rd_index = 0; // Индекс начала буфера (откуда читать данные)
volatile bool    		USART_tx_buffer_overflow = 0; // Флаг переполнения буфера

volatile uint8_t   	USART_rx_buffer[256];
volatile uint16_t   USART_rx_count = 0; // Количество данных в принятом буфере
volatile uint16_t   USART_rx_wr_index = 0; // Индекс хвоста буфера (куда писать данные )
volatile uint16_t   USART_rx_rd_index = 0; // Индекс начала буфера (откуда читать данные)
volatile bool   	 	USART_rx_buffer_overflow = 0; // Флаг переполнения буфера

uint8_t BufferMsg[256];
uint16_t lengthBufferCmd = 0;
int byteUSART = -1;
int cmdID = 0;
int BufferDataPack[4];
//=========================================================
// ВРЕМЕННО DEBUG
//=========================================================
bool  flagUpdateAD7794 = false;
uint32_t tiksDivAD7794 = 0;
uint32_t tiksCountAD7794 = 20;
uint32_t tiksCountSend = 100;

