#include "assistfuncs.h"

float compute_pid_base(struct pid_param *pp, float value){
	// Предыдущие значение
	pp->past_value = pp->present_value;
	// Текущие значение
	pp->present_value = value;
	//===================================================
	// Пропорциональная составляющая
	//===================================================
	pp->value_p = pp->present_value;
	//===================================================
	// Интегральная составляющая
	//===================================================
	if (pp->factor_ki != 0){
		pp->value_i += pp->factor_ki * pp->value_p;
		if (pp->value_i > pp->range_max){
			pp->value_i = pp->range_max;
		}
		if(pp->value_i < pp->range_min){
			pp->value_i = pp->range_min;
		}
	}
	//===================================================
	// Дифференциальная составляющая
	//===================================================
	pp->value_d = pp->present_value - pp->past_value;
	//===================================================
	// Суммарное воздействие
	//===================================================
	// Первый проход (нет дифференциальной части)
	if (pp->flag_run == false){
		pp->flag_run = true;
		pp->value_s = pp->factor_kp * pp->value_p + pp->value_i;
	}
	else {
		pp->value_s = pp->factor_kp * pp->value_p + pp->value_i + pp->factor_kd * pp->value_d;
	}
	return pp->value_s;
}

void reset_pid(struct pid_param *pp){
	pp->value_p = 0.0f;
	pp->value_d = 0.0f;
	pp->value_s = 0.0f;
	pp->value_i = 0.0f;
	pp->present_value = 0.0f;
	pp->past_value = 0.0f;
	pp->flag_run = false;
}

float mod(float number){
	if (number < 0) return -1*number;
	else return number;
}