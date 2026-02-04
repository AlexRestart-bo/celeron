#ifndef ASSISTFUNCS_H
#define ASSISTFUNCS_H

#include <stdbool.h>

struct pid_param {
	//=========================================================
	// здесь сосредоточены все переменные для ПИД-регуляторов
	//=========================================================
	float factor_kp;
	float factor_ki;
	float factor_kd;

	float range_max;
	float range_min;

	float value_p;
	float value_i;
	float value_d;
	float value_s;
	float present_value;
	float past_value;
	bool flag_run;
};

typedef struct {
	bool default_moddemod;
	int a;
	float b;
	float new_factor_kp;
	float new_factor_ki;
	float new_factor_kd;
} recal_pid;

/* Структура собирает некоторые переменные, касающиеся дискриминатора длины волны, большинство из них задействовано в синхронном детекторе тока */
typedef struct {
	int factor_beauty;				// множитель позволяет видеть ДДЛ при сканировании в удобном масштабе
	int factor_average;				// множитель, означающий количество усреднений конечной величины ДДЛ (имеется ввиду величина, отслеживаемая 0вым ПИД)
	int fbcheck;							// проверка множителя (на правильный ввод) перед присваиванием factor_beauty
	int facheck;							// проверка множителя (на правильный ввод) перед присваиванием factor_average
	float instant_dw;					// dw - discriminator wavelength, мгновенное значение ДДЛ
	float transit_first;			// сумма мгновенной величины на первых нескольких отсчетах
	float transit_second;			// сумма мгновенной величины на следующих нескольких отсчетах
	float directly_dw;				// полусумма величин transit_first и transit_second
	float resume_dw;					// средняя величина от directly_dw
} wavelength;

/* Структура имеет внутри себя переменные для отладки */
typedef struct {
	int a;
	int b;
	int c;
	float aa;
	float bb;
	float cc;
} dbg;

typedef struct {		//для выяснения, откуда появилось смещение при сканировании частоты
	int mighty_array[16];
	float mightest[2];
	float accum_might[2];
	bool must_update;
} debug_discr_freq;

extern struct pid_param primal_set[11];
float compute_pid_base(struct pid_param *pp, float value);
void reset_pid(struct pid_param *pp);
float mod(float number);

#endif