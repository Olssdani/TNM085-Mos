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
void  prisma::first_break(Line_Equation _Line, Line_Equation &_Light, double _X)
{
	//Beräknar vinklen in till prismat
	first_angle_in = PI / 2 - acos(glm::dot(_Light.Normal, _Line.Normal) / (glm::length(_Light.Normal)*glm::length(_Line.Normal)));
	
	if (abs(first_angle_in) <= critical_angle) {
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
		light_out = false;
	}
	
}


//Andra brytningen ur prismat
void  prisma::second_break(Line_Equation _Line, Line_Equation &_Light, double _X)
{
	//Beräknar vinklen in till prismat
	second_angle_in = PI/3 - first_angle;
	
	
	if (abs(second_angle_in) <= critical_angle) {
		//Brytningsvinkeln
		second_angle = Snells(n2, n1, second_angle_in);
		//Sätter att väggen är prickad och inte ska eftersökas mer
		second_hit.set(_X, _Line.value(_X));
		//skapa ut position
		pos_out.x = 1.0;
		pos_out.y = _Line.value(_X) - (1 - _X)*tan(second_angle - PI / 3 + first_angle_in);
	}
	else {
		light_out = false;
	}
	
}

// Kollar om ljustrålen prickar någon av ändpunkternA
bool prisma::check_ends(pos_2D pos, double _X, double _Y)
{
	double DELTA = 0.01;
	return (abs(pos.x-_X)<DELTA && abs(pos.y-_Y)<DELTA);
}

//Uppdaterar alla variabler
void prisma::update(float X[], float Y[]) 
{
	//sätter att en utsråle finns
	light_out = true;
	// ändpunkter
	pos_2D First_pos(X[0], Y[0]);
	pos_2D Second_pos(X[1], Y[1]);
	pos_2D Third_pos(X[2], Y[2]);

	// 4st linjens ekvation
	Line_Equation Light(0.0, 0.0);
	Line_Equation First_line(First_pos, Second_pos);
	Line_Equation Second_line(Second_pos, Third_pos);
	Line_Equation Third_line(Third_pos, First_pos);

	//Loop för första väggen
	double DELTA = 0.01;
	for (double X = -1.0; X < 0.0; X = X + step)
	{
		//Kollar om vi prickar en ände
		if (!(check_ends(First_pos, X, Light.value(X))|| check_ends(Second_pos, X, Light.value(X))|| check_ends(Third_pos, X, Light.value(X))))
		{
			//Kollar om ljuset korsar någon vägg
			if (!First_line.Wall_found && First_line.intersect(Light, X))
			{
				first_break(First_line, Light, X);
				break;
			}
			else if (!Second_line.Wall_found && Second_line.intersect(Light, X))
			{
				first_break(Second_line, Light, X);
				break;
			}
			else if (!Third_line.Wall_found && Third_line.intersect(Light, X))
			{
				first_break(Third_line, Light, X);
				break;
			}
		}else 
		{
			// ingen ljustråle finns 
			//TODO lägg in brytning i prismat
			light_out = false;
			
		}
	}
	
	if (light_out) 
	{
		//Loop för andra väggen.
		for (double X = first_hit.x; X < 1.0; X = X + step) {
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
	}
}
/*prisma::prisma(double x1, double x2, double X1[],double Y1[])
{
    n1 = x1;
    n2 = x2;
    
    for(int i=0;i<3;i++){
        X[i] = X1[i];
        Y[i] = Y1[i];
    }
    
}


prisma::~prisma()
{
}

void prisma::ToMatrix(){
    double pos[3][4] = {
        {X[0],X[1],X[2],0},
        {Y[0],Y[1],Y[2],0},
        {0,0,0,0}
    };
    for(int i=0;i<3;i++){
        for(int j=0;j<4;j++){
            position[i][j] = pos[i][j];
        }
    }
    return;
}

void prisma::line_ekv_first(double angle, double Y, double X, double &K, double &M)
{
    double y_temp;
    double x_temp;
    
    if (angle<60)
    {
        //Snells lag
        angle_out = prisma::Snells(angle);
        //Hittar tempor‰r punkt
        y_temp = Y - 0.1*tan(angle - angle_out);
        x_temp = X + 0.1;
        //R‰knar ut nya K och M v‰rden
        K = (y_temp - Y) / (x_temp - X);
        M = Y - K * X;
    }
    else
    {
        //Snells lag
        angle_out = prisma::Snells(angle);
        //Hittar tempor‰r punkt
        y_temp = Y + 0.1 * tan(180 - angle - angle_out);
        x_temp = X + 0.1;
        //R‰nkar ut nya K och M v‰rden
        K = (y_temp - Y) / (x_temp - X);
        M = Y - K * X;
    }
    return;
}

void prisma::line_ekv_second(double angle, double start_angle, double Y, double X, double &K, double &M)
{
    double y_temp;
    double x_temp;
    
    //Snells lag
    angle_out = Snells(angle);
    //Tempor‰r punkt
    y_temp = Y - 0.1*tan(angle_out - 60 + start_angle);
    x_temp = X + 0.1;
    //R‰knar ut nya K och M v‰rden
    K = (y_temp - Y) / (x_temp - X);
    M = Y - K * X;
    return;
}


double prisma::norm( const double x[], std::size_t sz ) // *** const
{
    // http://en.cppreference.com/w/cpp/algorithm/inner_product
    return std::sqrt( std::inner_product( x, x+sz, x, 0.0 ) ) ;
}

void prisma::Update()
{
   /* double pi = 3.1415926535897;
    double w = -pi/20;
    
    for(int i=0;i<3;i++){
        prisma::Euler(X[i], Y[i], w, i);
    }
    
    
    prisma::ToMatrix(); // sätter in i matris
    
    
    
    
    // Ljusstrålen, denna är alltid samma
    double K = 0;
    double M = 0;
    //Critical angle
    double crit_angle = asin(n1/n2)*(180/3.1415);
    
    // Allokering av minne f�r den brutna ljusstr�len
    
    
    // Linjerna f�r de olika prisma v�ggarna
    // F�rsta v�ggen
    double K_1 = ((position[1][0]-position[1][1])/(position[0][0]-position[0][1]));
    double M_1 = position[1][0]-K_1*position[0][0];
    
    // Andra v�ggen
    
    double K_2 = ((position[1][1]-position[1][2])/(position[0][1]-position[0][2]));
    double M_2 = position[1][1]-K_2*position[0][1];
    
    // Tredje v�ggen
    double K_3 = ((position[1][2]- position[1][0]) /(position[0][2]-position[0][0]));
    double M_3 = position[1][2]-K_3*position[0][2];
    
    // Normaler och vektorer
    // Ljus vecktorn
    double ljus[2] = {1,0};
    
    //F�rsta v�ggens normalen
    double norm1[2] = {K_1,1};
    
    //F�rsta v�ggens normalen
    double norm2[2] = {K_2, 1};
    
    //F�rsta v�ggens normalen
    double norm3[2] = {K_3, 1};
    
    // Brytningsloopen
    // Denna s�ker efter vilket v�gg som str�len prickar och sedan plockar
    // dennes normal och hittar den nya vinkeln.
    bool Wall1 = true;
    bool Wall2 = true;
    bool Wall3 = true;
    bool first = true;
    bool second = true;
    double angle = 0;
    double Yljus = 0;
    angle_out = 0;
    double angle_in = 0;
    // loopar �ver alla x v�rden
    double deltaT = -6.0;
    for(int i=0;i<12001;i++){
        
        
       
        //hitta högsa och lägsta värde
        int smallest = position[0][0];
        int biggest = position[0][0];
        for ( int i=1;  i < 3;  ++i ){
            if ( position[0][i] < smallest ){
                smallest = position[i][0];
                }
            if ( position[0][i] > biggest ){
                biggest = position[i][0];
            }
        }
        
     
        //Ber�knar Y v�rden mot v�ggarna
        // Ljustr�lens y v�rde
        Yljus = K*deltaT +M;
        if ( deltaT>=smallest &&  deltaT<=biggest)
        {
            bool  expression1= (abs(Yljus-position[1][0])<0.01 && abs(Yljus-position[0][0])<0.01);
            bool  expression2= (abs(Yljus-position[1][1])<0.01 && abs(Yljus-position[0][1])<0.01);
            bool  expression3= (abs(Yljus-position[1][2])<0.01 && abs(Yljus-position[0][2])<0.01);
            if ( !(expression1 || expression2 || expression3)){
              double  Y1 = (K_1*deltaT+M_1); // F�rsta v�ggen
              double  Y2 = (K_2*deltaT+M_2); // Andra v�ggen
              double  Y3 = (K_3*deltaT+M_3); // Tredje v�ggen
                
            
                
                if (Wall1 && abs(Yljus - Y1) < 0.02){
                    // adderar talet
                    
                    // Kollar om str�len �r i mediumet eller inte
                    if (first){
                        
                        
                        angle_in = acos(std::inner_product(std::begin(ljus), std::end(ljus), std::begin(norm1), 0.0)/(prisma::norm(ljus,sizeof(ljus))*prisma::norm(norm2,sizeof(norm2))))* (180.0 / 3.1415);
                        
                       line_ekv_first(angle_in, Yljus, deltaT,K,M);
                       
                        first = false;
                    }
                    else if (second){
                        angle = 60 - angle_out;
                        second = false;
                        if(abs(angle) <= crit_angle){
                            
                            line_ekv_second(angle,angle_in, Yljus, deltaT,K,M);
                        }
                        else{
                        
                        //deltaT = -1;
                        
                        }
                    }
                    ljus[0] = 1;
                    ljus[1] = K;
                    Wall1 = false;
                }
                
                else if(Wall2 && (abs(Yljus - Y2) < 0.02)){
                    //adderar talet till vektorn
                  
                    
                    //kollar om strålen är i mediumet eller inte
                    if(first){
                        angle_in = acos(std::inner_product(std::begin(ljus), std::end(ljus), std::begin(norm2), 0.0)/(norm(ljus,sizeof(ljus))*norm(norm2,sizeof(norm2))))* (180.0 / 3.1415);
                        line_ekv_first(angle_in, Yljus, deltaT, K ,M);
                        first = false;
                    }
                    else if(second){
                        angle = 60-angle_out;
                        second = false;
                        if(abs(angle) <= crit_angle){
                            line_ekv_second(angle,angle_in, Yljus, deltaT,K,M);
                        }
                        else{
                           
                            //deltaT = -1;
                        }
                    }
                    ljus[0] = 1;
                    ljus[1] = K;
                    Wall2 = false;
                }
                else if(Wall3 && (abs(Yljus - Y3) < 0.02)){
                    //adderar talet till vektorn
                    
                   
                    //kollar om strålen är i mediumet eller inte
                    if(first){
                        angle_in = acos(std::inner_product(std::begin(ljus), std::end(ljus), std::begin(norm3), 0.0)/(norm(ljus,sizeof(ljus))*norm(norm3,sizeof(norm3))))* (180.0 / 3.1415);
                        line_ekv_first(angle_in, Yljus, deltaT, K ,M);
                        first = false;
                    }
                    else if(second){
                        angle = 60-angle_out;
                        second = false;
                        if(abs(angle) <= crit_angle){
                            line_ekv_second(angle,angle_in, Yljus, deltaT,K,M);
                        }
                        else{
                           
                           // deltaT = -1;
                        }
                    }
                    Wall3 = false;
                }
                else{
                    //deltaT = -1;
                    
                }
                
            }
            
            
        
        }
       
        
        
        deltaT += 0.001;
    }
}*/


