#include "stdafx.h"
#include "line_ekv_first.h"

void line_ekv_first(double angle, double Y, double X, double n1, double n2, double &angle_out, double &K, double &M)
{
	double y_temp;
	double x_temp;

	if (angle<60)
	{
		//Snells lag
		angle_out = Snells(n1, n2, angle);
		//Hittar temporär punkt
		y_temp = Y - 0.1*tan(angle - angle_out);
		x_temp = X + 0.1;
		//Räknar ut nya K och M värden
		K = (y_temp - Y) / (x_temp - X);
		M = Y - K * X;
	}
	else
	{
		//Snells lag
		angle_out = Snells(n1, n2, angle);
		//Hittar temporär punkt
		y_temp = Y + 0, 1 * tan(180 - angle - angle_out);
		x_temp = X + 0.1;
		//Ränkar ut nya K och M värden
		K = (y_temp - Y) / (x_temp - X);
		M = Y - K * X;
	}
	return;
}

