#ifndef MATRIX4_H
#define MATRIX4_H

#include "triangle.h"
#include <cmath>

class Matrix2{

public:
    double data[2][2];
    Matrix2():data{{0,0},{0,0}}{}

    vect2 operator*(const vect2 &v){
        vect2 vr;
        vr.x = data[0][0] * v.x + data[0][1] * v.y;
        vr.y = data[1][0] * v.x + data[1][1] * v.y;
        return vr;
    }

    static Matrix2 identity(){
        Matrix2 m;
        m.data[0][0] = 1;
        m.data[1][1] = 1;
        return m;
    }
};

class Matrix4{



public:

    double data[4][4];

    Matrix4(): data{{0,0,0,0},
                    {0,0,0,0},
                    {0,0,0,0},
                    {0,0,0,0}}{}

    static Matrix4 identity(){
        Matrix4 m;
        for(int i = 0 ; i < 4 ; i++)
            m.data[i][i] = 1;
        return m;
    }

    static Matrix4 rotationMatrix(const double &alpha){
        Matrix4 m = identity();//TODO: vérifier
        m.data[0][0] = cos(alpha);
        m.data[0][1] = -sin(alpha);
        m.data[1][0] = sin(alpha);
        m.data[1][1] = cos(alpha);
        return m;
    }

    Vertex operator*(const Vertex &v)const{
        double returnValues[4];
        for(int i = 0 ; i < 4 ; i++){
            returnValues[i] = data[i][0] * v.x + data[i][1] * v.y + data[i][2] * v.z + data[i][3];
        }
        //passage en 3D
        return Vertex(  returnValues[0]/returnValues[3],
                        returnValues[1]/returnValues[3],
                        returnValues[2]/returnValues[3]);
    }

};


#endif // MATRIX4_H
