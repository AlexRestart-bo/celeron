#include "main.h"
#define TIM2ARRValue 45000

void delay_ms(uint32_t ms);

void InitPorts(void);
unsigned short int  ADC_RegularConvertedValueTab[9];
extern unsigned short int data_buffer[12];
extern unsigned char GetAdc;
extern unsigned char Get_Adc;
unsigned char ResultAdc[12];
unsigned short int TempAdc[3];

char cPos = 0;
char cDig = 0;
unsigned char c_Pos = 0;
uint8_t c_Dig[3] = {0,0,0};
uint8_t c_Dig_tag[3] = {4,1,5}; // 41.5 grad C
uint8_t c__Dig;
unsigned char Result_Adc[3];
uint16_t Temp_Adc_0, Temp_Adc_1, Temp_Adc_2, Temp_Adc, 
         Temp_Adc_h, Temp_Adc_hh, Temp_Adc_ht,
         T1_const = 42957,
         T2_const = 204;   
char CountSum_0, CountSum_1, CountSum_2;
char CountElements;
char Flag_1 = 0;
char Flag_2 = 0;
char Flag_3 = 0;
char Flag_4 = 0;
char Flag_m = 0;
uint16_t Cou_1 = 0;
uint16_t Cou_2 = 0;
uint16_t Cou_3 = 0;
uint16_t Cou_4 = 0;
uint8_t Div_h  = 114;
uint16_t knopky;
uint16_t tst_1, tst_2;
int main(void){
  Enable_Clocks();
  NVIC_Configuration();
  InitPorts();
  cPos = 1;
  cDig = 0;
  ADC1_Init();
   
  Delay(100000);
   
  cPos = 1;
  cDig = 1;
   
  Delay(100000);

  Temp_Adc = 0;
  Temp_Adc_1 = 0;
  Temp_Adc_2 = 0;
  CountSum_0 = 0;
    CountSum_1 = 0;
    CountSum_2 = 0;
  
  while (1)
  {    
    if(GetAdc == 1){
      Temp_Adc_0 = 0;
      for(CountElements = 0;CountElements < 8;CountElements ++){
        Temp_Adc_0 += ADC_RegularConvertedValueTab[CountElements];        
      }
      Temp_Adc_0 = Temp_Adc_0/8;
      Temp_Adc_1 += Temp_Adc_0;
      CountSum_0++;
      if(CountSum_0 == 8)
      {
        Temp_Adc_1 = Temp_Adc_1/8;
        Temp_Adc_2 += Temp_Adc_1;
        CountSum_0 = 0;
            CountSum_1++;
        }
        if(CountSum_1 == 8)
        {
        Temp_Adc_2 = Temp_Adc_2/8;
        Temp_Adc += Temp_Adc_2;
        CountSum_1 = 0;
        CountSum_2++;
        }
        if(CountSum_2 == 8)
        {
        Temp_Adc_h = Temp_Adc; 
        if(Temp_Adc < T1_const)
        {
          Temp_Adc_ht = T1_const - Temp_Adc;
        }
        else
        {
          Temp_Adc_ht = 0;
        }
      
        Temp_Adc_ht = Temp_Adc_ht/Div_h;
        Temp_Adc_h  = Temp_Adc_ht + T2_const; // +20.4 grad        
        
        Temp_Adc_hh = Temp_Adc_h/10;
        Temp_Adc_h = Temp_Adc_h - Temp_Adc_hh*10;
        c_Dig[2] = (uint8_t)Temp_Adc_h;
        Temp_Adc_h = Temp_Adc_hh;
        Temp_Adc_hh = Temp_Adc_h/10;
        Temp_Adc_h = Temp_Adc_h - Temp_Adc_hh*10;
        c_Dig[1] = (uint8_t)Temp_Adc_h;
        Temp_Adc_h = Temp_Adc_hh;
        Temp_Adc_hh = Temp_Adc_h/10;
        Temp_Adc_h = Temp_Adc_h - Temp_Adc_hh*10;
        c_Dig[0] = (uint8_t)Temp_Adc_h;      
       
        Temp_Adc = 0;
        CountSum_2 = 0;
      }
      c__Dig = c_Dig[c_Pos];
      if(c_Pos == 1) GPIO_ResetBits(GPIOA,GPIO_Pin_7);
      c_Pos++;
      if(c_Pos > 2) c_Pos = 0;
      knopky = GPIO_ReadInputData(GPIOB);
      tst_1 = knopky & GPIO_Pin_15;
      if(Flag_1 == 0)
      {
        if(tst_1 == 0) Flag_1 = 1;
      }
      else
      {
        if(tst_1 == 0) Cou_1++;
        else
        {
          Cou_1 = 0;
          Flag_1 = 0;
        }
        if(Cou_1 == 5000)
        {
          Cou_1--;
          Flag_2 = 1; 
        }
        else Flag_2 = 0;
      }
      tst_2 = knopky & GPIO_Pin_14;
      
      if(Flag_2 == 1)
      {
        Cou_2++;
        if(Cou_2 == 250)
        {
          Cou_2 = 0;
          Flag_m = (Flag_m == 0) ? 1 : 0;
        }
      }
      else
      {
        Cou_2 = 0;
        Flag_m = 0;
      }
      
      if(knopky & GPIO_Pin_15) GPIO_ResetBits(GPIOB,GPIO_Pin_13);
      GetAdc = 0;
    }
    else
    {

    }  
  }
}

void InitPorts(void){

GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOB, &GPIO_InitStructure);

GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14|GPIO_Pin_15;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
GPIO_Init(GPIOB, &GPIO_InitStructure);

GPIO_ResetBits(GPIOB,GPIO_Pin_13);
}

void Enable_Clocks(void){
    // Включение тактирования ADC1, GPIOA, GPIOB, TIM1, TIM2, TIM4
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_TIM1EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM4EN; 
}

void display_boot(void){
    if (!target){
        
    }else{

    }
}

void delay_ms(uint32_t ms){
    // Рассчет: 1 мс = частота_ядра / 1000 тиков
    uint32_t ticks_per_ms = SystemCoreClock / 1000;

    for (uint32_t i = 0; i < ms; i++)
    {
        SysTick->LOAD = ticks_per_ms - 1; // Устанавливаем счетчик
        SysTick->VAL = 0;                  // Очищаем текущее значение
        SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // Включаем, без прерываний

        // Ждем, пока COUNTFLAG (бит 16) не станет 1
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));

        SysTick->CTRL = 0; // Выключаем таймер
    }
}