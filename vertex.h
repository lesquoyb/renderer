#ifndef VERTEX_H
#define VERTEX_H

#include <cmath>


class Vertex{

public:
    float x, y, z;

    Vertex(float x, float y, float z): x(x),y(y), z(z){}

    void normalize(const int &width, const int & height){
   	 	x++;
        y++;
		x *= width;
		y *= width;
     }

	float angle(const Vertex &v)const{
		return acos( 
			(x*v.x + y*v.y + z*v.z)
			/ ( sqrt(x*x + y*y + z*z) * sqrt(v.x*v.x + v.y*v.y + v.z*v.z) ) 
		);
	}

	Vertex operator*(const Vertex &v)const{
		return Vertex(x * v.x, y * v.y, z * v.y);
	}


};

	Vertex cross(const Vertex &v1, const Vertex &v2){
		return 	Vertex( v1.y * v2.z  -  v1.z * v2.y,
						v1.z * v2.x  -  v1.x * v2.z,
						v1.x * v2.y  -  v1.y * v2.x );		
	}


#endif
