#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vertex.h"
#include "tools.h"
using namespace std;

class vect2{
	public:
		float x, y;
		vect2(const float &x, const float &y): x(x), y(y) {}
		vect2(const vect2 &a, const vect2 &b){
			x = max(a.x, b.x) - min(a.x, b.x);
			y = max(a.y, b.y) - min(a.y, b.y);
		}
		vect2(const Vertex &i): x(i.x), y(i.y){}
		
};


class Triangle{

public:

    Vertex* v1,* v2,* v3;
	double luminosity;

    Triangle(Vertex* x1, Vertex* x2, Vertex* x3, const Vertex &light):
        v1(x1),v2(x2),v3(x3), luminosity(light.angle(*v1 * (*v2)))
		{
		
		}


	bool isInTriangle(const int &x, const int &y) const{
		Vertex  x1(v3->x - v1->x, 
				   v2->x - v1->x, 
				   v1->x - x),
				x2(v3->y - v1->y, 
				   v2->y - v1->y, 
				   v1->y - y),
				c = cross(x1,x2);	
		return 1 - (c.x +c.y)/c.z >= 0 and c.y/c.z >= 0 and c.x/c.z >= 0;	
	}

	void barycentre(TGAImage &img, const TGAColor &c, int width, int height) const{
		//get the bounding box
		int xMax, yMax;
		int xMin = min(min(v1->x, v2->x), v3->x);  
		int yMin = min(min(v1->y, v2->y), v3->y);
 //	 	cout << luminosity << endl;
		xMax = max(max(v1->x, v2->x), v3->x);
		yMax = max(max(v1->y, v2->y), v3->y);
		TGAColor tColor((int) (c.r * luminosity),
						(int) (c.g * luminosity),
						(int) (c.b * luminosity),
						luminosity);
		for( ;xMin <= xMax ; xMin++){
			for( int y = yMin; y <= yMax ; y++){
				if( isInTriangle(xMin, y)){
					img.set(xMin, y, tColor);
				}
			}
		}
	}


};

#endif
