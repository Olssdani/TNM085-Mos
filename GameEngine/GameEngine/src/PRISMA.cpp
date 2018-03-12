//
//  Prisma.cpp
//  Prisma-test
//
//  Created by Lucas Palnén-Rung on 2018-02-15.
//  Copyright © 2018 Lucas Palnén-Rung. All rights reserved.

#include "Prisma.hpp"
#include <numeric>

//Constructor
prisma::prisma(double _n1, double _n2) 
{
	n1 = _n1;
	n2 = _n2;
	//Kritiskt vinkel
	critical_angle = asin(n1 / n2);
}
prisma::~prisma()
{

}

//Snells lag, ger tillbaka brytningsvinkeln
double prisma::Snells(double n1, double n2,double angle)
{
	return asin(n1 / n2 * sin(angle));
}

//Första brytningen in i prismat
void  prisma::first_break(Line_Equation _Line, Line_Equation &_Light, pos_2D &First, pos_2D &Second, double _X)
{
	glm::vec2 ljus = glm::vec2(1.0f, 0.0f);
	//Beräknar vinklen in till prismat
	first_angle_in = PI / 2 - acos(glm::dot(_Line.Normal, (_Light.Normal)) / (glm::length(_Light.Normal)*glm::length(_Line.Normal)));
	//std::cout << first_angle * 180 / PI << std::endl;

	pos_2D pos(_X, _Light.value(_X));
	if (First.euklidiean(pos) < Second.euklidiean(pos)) {

		riktning = "up";
		//Vinkeln in i prismat
		first_angle = Snells(n1, n2, first_angle_in);

		//Sätter att väggen är prickad och inte ska eftersökas mer
		first_hit.set(_X, _Line.value(_X));
		// Skapa ny ljusvektor
		pos_2D second_point;
		second_point.x = _X + 0.05;
		second_point.y = _Line.value(_X) - 0.05*tan(first_angle_in - first_angle);
		_Light.update(first_hit, second_point);
	}
	else {
		riktning = "down";
		//Vinkeln in i prismat
		first_angle = Snells(n1, n2, first_angle_in);
		//Sätter att väggen är prickad och inte ska eftersökas mer
		first_hit.set(_X, _Line.value(_X));
		// Skapa ny ljusvektor
		pos_2D second_point;
		second_point.x = _X + 0.05;
		second_point.y = _Line.value(_X) + 0.05*tan(first_angle_in - first_angle);
		_Light.update(first_hit, second_point);
	}

	//if ((first_angle_in) <= critical_angle) {
	if (first_angle_in < PI / 3) {

	}

}

//Andra brytningen ur prismat
void  prisma::second_break(Line_Equation _Line, Line_Equation &_Light, double _X)
{
	//Beräknar vinklen in till prismat
	second_angle_in = PI/3 - first_angle;
	//§std::cout << second_angle_in*180/PI<< std::endl;
	
	if (abs(second_angle_in) <= critical_angle) {
		if (riktning == "up") {
			//Brytningsvinkeln
			second_angle = Snells(n2, n1, second_angle_in);
			//Sätter att väggen är prickad och inte ska eftersökas mer
			second_hit.set(_X, _Line.value(_X));
			//skapa ut position
			pos_out.x = 1.0f;
			pos_out.y = _Line.value(_X) - (1 - _X)*tan(second_angle - PI / 3 + first_angle_in);
		}
		else {
			//Brytningsvinkeln
			second_angle = Snells(n2, n1, second_angle_in);
			
			//Sätter att väggen är prickad och inte ska eftersökas mer
			second_hit.set(_X, _Line.value(_X));
			//skapa ut position
			pos_out.x = 1.0f;
			pos_out.y = _Line.value(_X) + (1 - _X)*tan(PI/2-(second_angle - PI / 3 + first_angle_in));
		}
	}
	else {
		light_out = false;
	}
	
}

// Kollar om ljustrålen prickar någon av ändpunkternA
bool prisma::check_ends(pos_2D pos, double _X, double _Y)
{
	double DELTA = 0.005;
	return (abs(pos.x-_X)<DELTA && abs(pos.y-_Y)<DELTA);
}

//Uppdaterar alla variabler
void prisma::update(float X[], float Y[]) 
{
	//sätter att en utsråle finns
	light_out = true;
	light_out_middle = true;
	// ändpunkter
	pos_2D First_pos(X[0], Y[0]);
	pos_2D Second_pos(X[1], Y[1]);
	pos_2D Third_pos(X[2], Y[2]);

	// 4st linjens ekvation
	Line_Equation Light(0.0, 0.0);
	Line_Equation First_line(First_pos, Second_pos);
	Line_Equation Second_line(Second_pos, Third_pos);
	Line_Equation Third_line(Third_pos, First_pos);

	//std::cout << "K: " << First_line.K << " M: " << First_line.M << std::endl;
	double min = 1.0;
	if (min > First_pos.x) {
		min = First_pos.x;
	}
	if (min > Second_pos.x) {
		min = Second_pos.x;
	}
	if (min > Third_pos.x) {
		min = Third_pos.x;
	}
	//std::cout << min << std::endl;
	//Loop för första väggen
	double DELTA = 0.01;
	for (double X = min; X < 0.0; X = X + step)
	{
		//Kollar om vi prickar en ände
		if (!(check_ends(First_pos, X, Light.value(X))|| check_ends(Second_pos, X, Light.value(X))|| check_ends(Third_pos, X, Light.value(X))))
		{
			
			//Kollar om ljuset korsar någon vägg
			if (First_line.intersect(Light, X))
			{
				//std::cout << "X: " << X << std::endl;
				first_break(First_line, Light,First_pos, Second_pos, X);
				break;
			}
			else if (Second_line.intersect(Light, X))
			{
				//std::cout << "X: " << X << std::endl;
				first_break(Second_line, Light,Second_pos, Third_pos, X);
				break;
			}
			else if (Third_line.intersect(Light, X))
			{
				//std::cout << "X: " << X << std::endl;
				first_break(Third_line, Light,Third_pos, First_pos, X);
				break;
			}
		}else 
		{
			// ingen ljustråle finns 
			//TODO lägg in brytning i prismat
			first_hit.set(X, Light.value(X));
			light_out = false;
			light_out_middle = false;
			//std::cout << "Kant" << std::endl;	
		}
	}
	
	if (light_out || light_out_middle) 
	{
		
		//Loop för andra väggen.
		for (double X = first_hit.x; X < 1.0; X = X + step) {
			if (!(check_ends(First_pos, X, Light.value(X)) || check_ends(Second_pos, X, Light.value(X)) || check_ends(Third_pos, X, Light.value(X)))) {
				if (!First_line.Wall_found && First_line.intersect(Light, X))
				{
					second_break(First_line, Light, X);
					break;
				}
				else if (!Second_line.Wall_found && Second_line.intersect(Light, X))
				{
					second_break(Second_line, Light, X);
					break;
				}
				else if (!Third_line.Wall_found && Third_line.intersect(Light, X))
				{
					second_break(Third_line, Light, X);
					break;
				}
			}
			else {
				second_hit.set(X, Light.value(X));
				light_out = false;
			}
			
		}
	}
}



