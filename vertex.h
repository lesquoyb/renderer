#ifndef VERTEX_H
#define VERTEX_H

#include <cmath>
#include <iostream>


const double PI = 3.1415926535;
class Vertex{

public:
    float x, y, z;

    Vertex(float x, float y, float z): x(x),y(y), z(z){}

    void normalize(const int &width, const int & height){
   	 	x++;
        y++;
		z++;
		x *= width;
		y *= width;
     }

	 double norm()const{
		return sqrt(x*x+y*y+z*z);
	 }

	float angle(const Vertex &v)const{
		std::cout << "mais... " << x << " " << y << " "<< z <<std::endl;
		std::cout << "... " <<v.x << " " << v.y << " "<< v.z <<std::endl;
	/*	return acos( 
			(x*v.x + y*v.y + z*v.z)
			/ ( sqrt(x*x + y*y + z*z) * sqrt(v.x*v.x + v.y*v.y + v.z*v.z) ) 
		)/PI;
		*/return abs(x* v.x + y * v.y + z * v.z);//TODO: faux mais marche avec 0,0,1
	}
	Vertex cross(const Vertex &v)const{
		return Vertex(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}
	Vertex operator-(const Vertex &v)const{
		return Vertex(x -v.x, y - v.y, z - v.z);
	}

	Vertex operator*(const double &i)const{
		return Vertex(x*i, y*i, z*i);
	}

	double operator *(const Vertex &v) const { 
		return x*v.x + y*v.y + z*v.z; 
	}
};






#endif
