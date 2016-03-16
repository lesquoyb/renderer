#ifndef MATRIX4_H
#define MATRIX4_H

#include "triangle.h"
#include <cmath>

enum Axis{X = 1, Y = 2, Z = 3};

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


    Matrix4 operator*(Matrix4& m)const{
        Matrix4 ret;
        for(int i = 0 ; i < 4 ; i++){
            for(int j = 0 ; j < 4 ; j++){

                ret[i][j] = 0;
                for(int k = 0 ; k < 4 ; k++)
                ret[i][j] += data[i][k] * m[k][j];//TODO: vérifier, c'est trop beau pour être vrai
            }
        }
        return ret;
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

    double* operator[](const int &index){
        return data[index];
    }


    static Matrix4 lookat(const Vertex &eye,const  Vertex &center, const Vertex &up) {
        Vertex z = (eye - center).normalized();
        Vertex x = up.cross(z).normalized();
        Vertex y = z.cross( x).normalized();
        Matrix4 Minv = Matrix4::identity();
        Matrix4 Tr   = Matrix4::identity();
        for (int i=0; i<3; i++) {
            Minv[0][i] = x[i];
            Minv[1][i] = y[i];
            Minv[2][i] = z[i];
            Tr[i][3] = - center[i];
        }
        return Minv*Tr;
    }

    static Matrix4 viewport(int x, int y,int depth, int w, int h) {
        Matrix4 m = Matrix4::identity();
        m[0][3] = x + w/2.f;
        m[1][3] = y + h/2.f;
        m[2][3] = depth/2.f;

        m[0][0] = w/2.f;
        m[1][1] = h/2.f;
        m[2][2] = depth/2.f;
        return m;
    }


    static Matrix4 identity(){
        Matrix4 m;
        for(int i = 0 ; i < 4 ; i++)
            m.data[i][i] = 1;
        return m;
    }


    static Matrix4 rotationMatrix(const double &alpha){//, const Axis& axis = Axis::X){
        Matrix4 m = identity();
        m[0][0] =  cos(alpha);
        m[0][2] = -sin(alpha);
        m[2][0] =  sin(alpha);
        m[2][2] =  cos(alpha);
        return m;
    }


};

ostream& operator<<(ostream & s, const Matrix4 &m){

}



#endif // MATRIX4_H
