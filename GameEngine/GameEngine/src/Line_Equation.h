#pragma once
#include "pos_2D.h"
#include "glm/glm.hpp"
#include "Line_Equation.h"
class Line_Equation
{
public:
	//Constuctor
	Line_Equation(double _K, double _M);
	Line_Equation(pos_2D start, pos_2D end);
	~Line_Equation();

	double value(const double _X);

	void update(pos_2D start, pos_2D end);

	bool intersect(Line_Equation _Line, double _X);

//Variabler
	double K = 0;
	double M = 0;
	glm::vec2 Normal = glm::vec2(0.0, 0.0);
	bool Wall_found = false;
};


