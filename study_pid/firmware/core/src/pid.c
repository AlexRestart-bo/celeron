#include "main.h"

struct pid_param pp = {0.0001, 0.0001, 0, 50.0, 1.0, 0, 0, 0, 0, 0, 0, false};

float compute_pid_base(struct pid_param *pp, float value);

void compute_pid(void){
    float tdc = duty_cycle + compute_pid_base(&pp, target_temp - temp); 
    if (tdc <= 0) duty_cycle = 0;
    if (tdc >= 100) duty_cycle = 100;
}

float compute_pid_base(struct pid_param *pp, float value){
	pp->past_value = pp->present_value;
	pp->present_value = value;
	pp->value_p = pp->present_value;
	if (pp->factor_ki != 0){
		pp->value_i += pp->factor_ki * pp->value_p;
		if (pp->value_i > pp->range_max){
			pp->value_i = pp->range_max;
		}
		if(pp->value_i < pp->range_min){
			pp->value_i = pp->range_min;
		}
	}
	pp->value_d = pp->present_value - pp->past_value;
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
