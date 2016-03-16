#ifndef VERTEX_H
#define VERTEX_H

#include <cmath>
#include <iostream>
#include <ostream>
#include <sstream>

using std::ostream;

const double PI = 3.1415926535;
class Vertex{

public:

    float x, y, z;

    Vertex():Vertex(0,0,0) {}

    Vertex(float x, float y, float z): x(x),y(y), z(z){}

    Vertex normalized() const{
        return Vertex(  x / norm(),
                        y / norm(),
                        z / norm());
    }

    void normalize(const int &width, const int & height){
            x++;
            y++;
            z++;
          /*  x *= width;
            y *= width;
            z *= width;//TODO: ça n'a aucun sens :) mais ça ne marche pas autrement ... question de proportions, toussa toussa
            */
     }

	 double norm()const{
		return sqrt(x*x+y*y+z*z);
	 }

	Vertex cross(const Vertex &v)const{
        return Vertex(      y*v.z - z*v.y,
                            z*v.x - x*v.z,
                            x*v.y - y*v.x);
	}
	Vertex operator-(const Vertex &v)const{
		return Vertex(x -v.x, y - v.y, z - v.z);
	}
    Vertex operator+(const Vertex &v)const{
        return Vertex(x + v.x, y + v.y, z + v.z);
    }
	Vertex operator*(const double &i)const{
		return Vertex(x*i, y*i, z*i);
	}

    Vertex operator *(const Vertex &v) const {
        return Vertex(x*v.x,   y*v.y , z*v.z);
	}

    float operator[](const int &index)const{
        return (index == 0) ? x : (index == 1) ? y : z;
    }


};


ostream& operator<<(ostream & s, const Vertex & v){
    return s << v.x <<" " << v.y << " " << v.z;
}




#endif
