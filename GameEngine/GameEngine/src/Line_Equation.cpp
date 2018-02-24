#include "Line_Equation.h"

Line_Equation::Line_Equation(double _K, double _M )
{
	K = _K;
	M = _M;
	Normal = glm::vec2(K, 1.0);
}

Line_Equation::Line_Equation(pos_2D start, pos_2D end)
{
	K = (end.y - start.y) / (end.x - start.x);
	M = start.y - K * start.x;
	Normal = glm::vec2(K, 1.0);
}


Line_Equation::~Line_Equation()
{
}


double Line_Equation::value(const double _X) 
{
	return _X * K + M;
}

void Line_Equation::update(pos_2D start, pos_2D end) 
{
	K = (end.y - start.y) / (end.x - start.x);
	M = start.y - K * start.x;
	Normal = glm::vec2(K, 1.0);
}


bool Line_Equation::intersect(Line_Equation _Line, double _X)
{
	double DELTA = 0.001;
	if (abs(this->value(_X) - _Line.value(_X)) < DELTA)
	{
		Wall_found = true;
		return true;
	}
	return false;
}