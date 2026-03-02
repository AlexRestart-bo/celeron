#ifndef CONFIG_H
#define CONFIG_H

#define TIM1COUNTER (200 - 1)
#define TEMP_STEP 0.02f
#define TEMP_MIN 1.0f
#define TEMP_MAX 50.0f

extern float target_temp;
extern float temp;
extern volatile uint8_t status_lb;
extern volatile uint8_t status_rb;
extern uint8_t duty_cycle;

uint8_t Read_Button_PB14(void);
uint8_t Read_Button_PB15(void);

void show_current_temp(void);
void show_work_pid(void);
void falling_temp(void);
void raising_temp(void);
void enable_display(void);

#endif