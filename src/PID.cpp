#include "PID.h"

#include <iostream>
#include <math.h>

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
	//initialize the hyperparameters
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
    d_error = 0;
	p_error = 0;
	i_error = 0;
}

void PID::UpdateError(double cte) {
	//calculate all errors
	d_error = cte - p_error;
	p_error = cte;
	i_error += cte;
}

double PID::TotalError() {
	//return total error
    return -(Kp * p_error + Kd * d_error + Ki * i_error);
}
