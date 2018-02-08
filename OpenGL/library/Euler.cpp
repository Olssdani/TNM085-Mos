#include "stdafx.h"
#include "Euler.h"

void Euler(double x, double y, double w, double &X, double &Y)
{
	double theta;
	double thetaN;
	double X = 0;
	double Y = 0;
	double h = 0.1;


	double r = sqrt(pow(x, 2) + pow(y, 2));
	const double PI = 3.14159265358979323846;

	if (x > 0)
	{
		theta = atan(y / x);
	}
	else if (x < 0 && y >= 0)
	{
		theta = atan(y / x) + PI;
	}
	else if (x < 0 && y < 0)
	{
		theta = atan(y / x) - PI;
	}
	else if (x == 0 && y > 0)
	{
		theta = PI / 2;
	}
	else
	{
		theta = -PI / 2;
	}

	//Euler
	thetaN = theta + h * w;

	X = r * cos(thetaN);
	Y = r * sin(thetaN);

	return;
}
