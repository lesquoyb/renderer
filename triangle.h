#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vertex.h"
#include "tools.h"
using namespace std;


class Triangle{

public:

    Vertex* v1,* v2,* v3;

    Triangle(Vertex* x1, Vertex* x2, Vertex* x3):
        v1(x1),v2(x2),v3(x3)
		{
		}

};

Vertex barycentre(const Vertex& v1, const Vertex &v2,const Vertex &v3,const int &x, const int &y){

    Vertex  x1(v3.x - v1.x,
               v2.x - v1.x,
               v1.x - x),
            x2(v3.y - v1.y,
               v2.y - v1.y,
               v1.y - y),
            x3 = x1.cross(x2);
    float xp =  1 - (x3.x +x3.y)/x3.z,
          yp =  x3.y/x3.z,
          zp =  x3.x/x3.z;
    return Vertex(xp, yp, zp);
}

#endif
