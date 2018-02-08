#include "stdafx.h"
#include "line_ekv_second.h"

void line_ekv_second(double angle, double start_angle, double angle_ref, double Y, double X, double n1, double n2, double &angle_out, double &K, double &M)
{
	double y_temp;
	double x_temp;

	//Snells lag
	angle_out = Snells(n1, n2, angle);
	//Temporär punkt
	y_temp = Y - 0.1*tan(angle_out - 60 + start_angle);
	x_temp = X + 0.1;
	//Räknar ut nya K och M värden
	K = (y_temp - Y) / (x_temp - X);
	M = Y - K * X;
	return;
}

