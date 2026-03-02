#ifndef PID_H
#define PID_H

#include "main.h"

struct pid_param {
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

extern struct pid_param pp;

void compute_pid(void);

#endif