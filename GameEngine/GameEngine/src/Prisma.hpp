//
//  Prisma.hpp
//  Prisma-test
//
//  Created by Lucas Palnén-Rung on 2018-02-15.
//  Copyright © 2018 Lucas Palnén-Rung. All rights reserved.
//

#ifndef Prisma_hpp
#define Prisma_hpp

#include <iostream>
#include <stdio.h>
#include <cmath>
#include <numeric>
#include "pos_2D.h"
#include "glm/glm.hpp"
#include "Line_Equation.h"
class prisma
{
public:
	//constructor
	prisma(double _n1, double _n2);

	//Destructor
	~prisma();

	//Update funktion som updaterar prismats position i rymden
	void update(float X[], float Y[]);

	//VARIABLER
	//Vart som strålen går ut
	pos_2D pos_out;
	//Vinkeln ut ur prismat
	double angle_out =0;
	//Om ljus kommer ut ur prismat
	bool light_out;
	bool light_out_middle;
	pos_2D second_hit;

	//Position där ljuset bryter
	pos_2D first_hit;

private:
	const double PI = 3.1415926536;
	//Brytningsindex
	double n1 = 0;
	double n2 = 0;
	//kritiska vinkeln för mediet
	double critical_angle = 0;
	
	//Steglängd
	double step = 0.0001;
	
	//Invinklar till snells lag
	double first_angle_in;
	double second_angle_in;
	
	//Vinklar från snells lag
	double first_angle;
	double second_angle;

	//
	std::string riktning;

	//Privata funktioner
	//Snells lag
	double Snells(double n1, double n2, double angle);
	//Kollar brytningarna i prismat
	void  first_break(Line_Equation _Line, Line_Equation &_Light, pos_2D &First, pos_2D &Second, double _X);
	void  second_break(Line_Equation _Line, Line_Equation &_Light, double _X);
	//Kolalr ifall ljuset prickar en hörnpunkt
	bool check_ends(pos_2D pos, double _X, double _Y);
};


#endif /* Prisma_hpp */
