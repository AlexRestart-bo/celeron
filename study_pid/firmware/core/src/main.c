#include "main.h"
#define TIM2ARRValue 45000

void InitPorts(void);

void Clear_IND(void);
void SetPos(char);
void Digit_ON(char);
void AllSeg_OFF(void);
extern unsigned short int  ADC_RegularConvertedValueTab[9];
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
  SetPos(cPos);
  cDig = 0;
  Digit_ON(cDig);
  ADC1_Init();
   
  Delay(100000);
   
  cPos = 1;
  SetPos(cPos);
  cDig = 1;
  Digit_ON(cDig);
   
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
      Clear_IND();
      c__Dig = c_Dig[c_Pos];
      if(Flag_m == 0) Digit_ON(c__Dig);
      if(c_Pos == 1) GPIO_ResetBits(GPIOA,GPIO_Pin_7);
      if(Flag_m == 0) SetPos(c_Pos);
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

void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

void InitPorts(void){
GPIO_InitTypeDef GPIO_InitStructure;

GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOA, &GPIO_InitStructure);
GPIO_SetBits(GPIOA,GPIO_Pin_0);
GPIO_SetBits(GPIOA,GPIO_Pin_1);
GPIO_SetBits(GPIOA,GPIO_Pin_2);
GPIO_SetBits(GPIOA,GPIO_Pin_3);
GPIO_ResetBits(GPIOA,GPIO_Pin_4);
GPIO_ResetBits(GPIOA,GPIO_Pin_5);
GPIO_ResetBits(GPIOA,GPIO_Pin_6);
GPIO_ResetBits(GPIOA,GPIO_Pin_7); 
GPIO_SetBits(GPIOA,GPIO_Pin_8);
GPIO_ResetBits(GPIOA,GPIO_Pin_9);
GPIO_ResetBits(GPIOA,GPIO_Pin_10);

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

void AllSeg_OFF()
{
  uint16_t my_GPIO_Pin;
  my_GPIO_Pin = 0xFF;
  GPIO_SetBits(GPIOA,my_GPIO_Pin);
}

void Clear_IND()
{
  uint16_t my_GPIO_Pin;
  my_GPIO_Pin = 0xFF;
  GPIO_SetBits(GPIOA,my_GPIO_Pin);
  my_GPIO_Pin = 0x0700;
  GPIO_ResetBits(GPIOA,my_GPIO_Pin);
}

void Digit_ON(char cDig_)
{
  uint16_t my_GPIO_Pin;
  my_GPIO_Pin = 0;
   switch(cDig_)
   {
   case 0:
     my_GPIO_Pin = 63;
     break;
   case 1:
     my_GPIO_Pin = 6;
     break;
   case 2:
     my_GPIO_Pin = 91;
     break;
   case 3:
     my_GPIO_Pin = 79;
     break;
   case 4:
     my_GPIO_Pin = 102;
     break;
   case 5:
     my_GPIO_Pin = 109;
     break;
   case 6:
     my_GPIO_Pin = 125;
     break;
   case 7:
     my_GPIO_Pin = 7;
     break;
   case 8:
     my_GPIO_Pin = 127;
     break;
   case 9:
     my_GPIO_Pin = 111;
     break;
   default:
     my_GPIO_Pin = 128;
   }
  
  GPIO_ResetBits(GPIOA,my_GPIO_Pin);
}

void SetPos(char cPos_)
{
  uint16_t my_GPIO_Pin;
  switch(cPos_)
   {
   case 0:
     my_GPIO_Pin = GPIO_Pin_8;
     break;
   case 1:
     my_GPIO_Pin = GPIO_Pin_9;
     break;
   case 2:
     my_GPIO_Pin = GPIO_Pin_10;
     break;   
   }
  
  GPIO_SetBits(GPIOA,my_GPIO_Pin);
}


void Enable_Clocks(void){
    // Включение тактирования ADC1, GPIOA, GPIOB
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;
}

void delay_ms(int sec){
    for (volatile int i = 0; i < sec*100000; i++);
}

void ADC1_2_IRQHandler(void)
{
  if (ADC1->SR & ADC_SR_EOC){
    redirect_data(ADC1->DR);
    ADC1->SR &= ~ADC_SR_EOC;
  }
}

void  redirect_data(uint32_t data){
    ADC_RegularConvertedValueTab[num_Sample++] = ADC_Read();
    if(num_Sample > 8) 
    {
      num_Sample = 0;
      GetAdc=1;
    } 
    Get_Adc = 1;
    n_h++;
    if(n_h == 1000) 
    {
      n_h = 0;
    }  
}
