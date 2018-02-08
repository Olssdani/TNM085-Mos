#include "stdafx.h"
#include "Snells.h"

double Snells(double n1, double n2, double angle)
{
	double out_angle = asin(n1 / n2 * sin(angle));
	return out_angle;
}
