#ifndef MATRIX4_H
#define MATRIX4_H

#include "triangle.h"
#include <cmath>

enum Axis{X = 1, Y = 2, Z = 3};



//TOUJOURS PAS MON CODE, ET CETTE FOIS C'EST VRAIMENT, VRAIMENT MOCHE


#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>

template<size_t DimCols,size_t DimRows,typename T> class mat;

template <size_t DIM, typename T> struct vec {
    vec() { for (size_t i=DIM; i--; data_[i] = T()); }
          T& operator[](const size_t i)       { assert(i<DIM); return data_[i]; }
    const T& operator[](const size_t i) const { assert(i<DIM); return data_[i]; }
private:
    T data_[DIM];
};

/////////////////////////////////////////////////////////////////////////////////

template <typename T> struct vec<2,T> {
    vec() : x(T()), y(T()) {}
    vec(T X, T Y) : x(X), y(Y) {}
    template <class U> vec<2,T>(const vec<2,U> &v);
          T& operator[](const size_t i)       { assert(i<2); return i<=0 ? x : y; }
    const T& operator[](const size_t i) const { assert(i<2); return i<=0 ? x : y; }

    T x,y;
};

/////////////////////////////////////////////////////////////////////////////////

template <typename T> struct vec<3,T> {
    vec() : x(T()), y(T()), z(T()) {}
    vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
    template <class U> vec<3,T>(const vec<3,U> &v);
          T& operator[](const size_t i)       { assert(i<3); return i<=0 ? x : (1==i ? y : z); }
    const T& operator[](const size_t i) const { assert(i<3); return i<=0 ? x : (1==i ? y : z); }
    float norm() { return std::sqrt(x*x+y*y+z*z); }
    vec<3,T> & normalize(T l=1) { *this = (*this)*(l/norm()); return *this; }

    T x,y,z;
};

/////////////////////////////////////////////////////////////////////////////////

template<size_t DIM,typename T> T operator*(const vec<DIM,T>& lhs, const vec<DIM,T>& rhs) {
    T ret = T();
    for (size_t i=DIM; i--; ret+=lhs[i]*rhs[i]);
    return ret;
}


template<size_t DIM,typename T>vec<DIM,T> operator+(vec<DIM,T> lhs, const vec<DIM,T>& rhs) {
    for (size_t i=DIM; i--; lhs[i]+=rhs[i]);
    return lhs;
}

template<size_t DIM,typename T>vec<DIM,T> operator-(vec<DIM,T> lhs, const vec<DIM,T>& rhs) {
    for (size_t i=DIM; i--; lhs[i]-=rhs[i]);
    return lhs;
}

template<size_t DIM,typename T,typename U> vec<DIM,T> operator*(vec<DIM,T> lhs, const U& rhs) {
    for (size_t i=DIM; i--; lhs[i]*=rhs);
    return lhs;
}

template<size_t DIM,typename T,typename U> vec<DIM,T> operator/(vec<DIM,T> lhs, const U& rhs) {
    for (size_t i=DIM; i--; lhs[i]/=rhs);
    return lhs;
}

template<size_t LEN,size_t DIM,typename T> vec<LEN,T> embed(const vec<DIM,T> &v, T fill=1) {
    vec<LEN,T> ret;
    for (size_t i=LEN; i--; ret[i]=(i<DIM?v[i]:fill));
    return ret;
}

template<size_t LEN,size_t DIM, typename T> vec<LEN,T> proj(const vec<DIM,T> &v) {
    vec<LEN,T> ret;
    for (size_t i=LEN; i--; ret[i]=v[i]);
    return ret;
}

template <typename T> vec<3,T> cross(vec<3,T> v1, vec<3,T> v2) {
    return vec<3,T>(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

template <size_t DIM, typename T> std::ostream& operator<<(std::ostream& out, vec<DIM,T>& v) {
    for(unsigned int i=0; i<DIM; i++) {
        out << v[i] << " " ;
    }
    return out ;
}

/////////////////////////////////////////////////////////////////////////////////

template<size_t DIM,typename T> struct dt {
    static T det(const mat<DIM,DIM,T>& src) {
        T ret=0;
        for (size_t i=DIM; i--; ret += src[0][i]*src.cofactor(0,i));
        return ret;
    }
};

template<typename T> struct dt<1,T> {
    static T det(const mat<1,1,T>& src) {
        return src[0][0];
    }
};

/////////////////////////////////////////////////////////////////////////////////

template<size_t DimRows,size_t DimCols,typename T> class mat {
    vec<DimCols,T> rows[DimRows];
public:
    mat() {}

    vec<DimCols,T>& operator[] (const size_t idx) {
        assert(idx<DimRows);
        return rows[idx];
    }

    const vec<DimCols,T>& operator[] (const size_t idx) const {
        assert(idx<DimRows);
        return rows[idx];
    }

    vec<DimRows,T> col(const size_t idx) const {
        assert(idx<DimCols);
        vec<DimRows,T> ret;
        for (size_t i=DimRows; i--; ret[i]=rows[i][idx]);
        return ret;
    }

    void set_col(size_t idx, vec<DimRows,T> v) {
        assert(idx<DimCols);
        for (size_t i=DimRows; i--; rows[i][idx]=v[i]);
    }

    static mat<DimRows,DimCols,T> identity() {
        mat<DimRows,DimCols,T> ret;
        for (size_t i=DimRows; i--; )
            for (size_t j=DimCols;j--; ret[i][j]=(i==j));
        return ret;
    }

    T det() const {
        return dt<DimCols,T>::det(*this);
    }

    mat<DimRows-1,DimCols-1,T> get_minor(size_t row, size_t col) const {
        mat<DimRows-1,DimCols-1,T> ret;
        for (size_t i=DimRows-1; i--; )
            for (size_t j=DimCols-1;j--; ret[i][j]=rows[i<row?i:i+1][j<col?j:j+1]);
        return ret;
    }

    T cofactor(size_t row, size_t col) const {
        return get_minor(row,col).det()*((row+col)%2 ? -1 : 1);
    }

    mat<DimRows,DimCols,T> adjugate() const {
        mat<DimRows,DimCols,T> ret;
        for (size_t i=DimRows; i--; )
            for (size_t j=DimCols; j--; ret[i][j]=cofactor(i,j));
        return ret;
    }

    mat<DimRows,DimCols,T> invert_transpose() {
        mat<DimRows,DimCols,T> ret = adjugate();
        T tmp = ret[0]*rows[0];
        return ret/tmp;
    }
};

/////////////////////////////////////////////////////////////////////////////////

template<size_t DimRows,size_t DimCols,typename T> vec<DimRows,T> operator*(const mat<DimRows,DimCols,T>& lhs, const vec<DimCols,T>& rhs) {
    vec<DimRows,T> ret;
    for (size_t i=DimRows; i--; ret[i]=lhs[i]*rhs);
    return ret;
}

template<size_t R1,size_t C1,size_t C2,typename T>mat<R1,C2,T> operator*(const mat<R1,C1,T>& lhs, const mat<C1,C2,T>& rhs) {
    mat<R1,C2,T> result;
    for (size_t i=R1; i--; )
        for (size_t j=C2; j--; result[i][j]=lhs[i]*rhs.col(j));
    return result;
}

template<size_t DimRows,size_t DimCols,typename T>mat<DimCols,DimRows,T> operator/(mat<DimRows,DimCols,T> lhs, const T& rhs) {
    for (size_t i=DimRows; i--; lhs[i]=lhs[i]/rhs);
    return lhs;
}

template <size_t DimRows,size_t DimCols,class T> std::ostream& operator<<(std::ostream& out, mat<DimRows,DimCols,T>& m) {
    for (size_t i=0; i<DimRows; i++) out << m[i] << std::endl;
    return out;
}

/////////////////////////////////////////////////////////////////////////////////

typedef vec<2,  float> Vec2f;
typedef vec<2,  int>   Vec2i;
typedef vec<3,  float> Vec3f;
typedef vec<3,  int>   Vec3i;
typedef vec<4,  float> Vec4f;
typedef mat<4,4,float> Matrix;




class Matrix4{



public:

    double data[4][4];

    Matrix4(): data{{0,0,0,0},
                    {0,0,0,0},
                    {0,0,0,0},
                    {0,0,0,0}}{}



    Matrix4 operator*(double d){
        Matrix4 ret;
        for(int i = 0 ; i < 4 ; i++){
            for(int j = 0 ; j < 4 ; j++){
                ret[i][j] = data[i][j] * d;
            }
        }
        return ret;
    }

    Matrix4 operator*(Matrix4& m)const{
        Matrix4 ret;
        for(int i = 0 ; i < 4 ; i++){
            for(int j = 0 ; j < 4 ; j++){

                ret[i][j] = 0;
                for(int k = 0 ; k < 4 ; k++)
                ret[i][j] += data[i][k] * m[k][j];
            }
        }
        return ret;
    }

    Matrix4 operator*(const double &d)const{
        Matrix4 ret;
        for(int i = 0 ; i < 4 ; i++){
            for(int j = 0 ; j < 4 ; j++)
            ret[i][j] = data[i][j] * d;
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


    // PAS MON CODE + dirty hacks :)
    static Matrix4 lookat(const Vertex &eye,const  Vertex &center, const Vertex &up) {
        Vertex z = (eye - center).normalized();
        Vertex x = up.cross(z).normalized();
        Vertex y = z.cross(x).normalized();
        Matrix4 Minv = Matrix4::identity();
        Matrix4 Tr   = Matrix4::identity();
        for (int i=0; i<3; i++) {
            Minv[0][i] = x[i];
            Minv[1][i] = y[i];
            Minv[2][i] = z[i];
            Tr[i][3] = - center[i];
        }
        return Minv * Tr;
    }

    Matrix4 invert_transpose(){
        Matrix4 ret;
        Matrix m;
        for(int i = 0 ; i < 4 ; i++){
            for(int j =0;j<4; j++){
                m[i][j] = data[i][j];
            }
        }
        Matrix tmp = m.invert_transpose();
        for(int i = 0 ; i < 4 ; i++){
            for(int j =0;j<4; j++){
                ret[i][j] = tmp[i][j];
            }
        }
        return ret;
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

// OUF C'ÉTAIT MOCHE

    static Matrix4 identity(){
        Matrix4 m;
        for(int i = 0 ; i < 4 ; i++)
            m.data[i][i] = 1;
        return m;
    }


    static Matrix4 rotationMatrix(const double &alpha, const Axis &axis = Y){//, const Axis& axis = Axis::X){

        Matrix4 m = identity();
        m[0][0] =  cos(alpha);
        m[0][2] = -sin(alpha);
        m[2][0] =  sin(alpha);
        m[2][2] =  cos(alpha);
        return m;
    }

};

ostream& operator<<(ostream & s, Matrix4 &m){
    for(int i = 0 ; i < 4 ; i++){
        for(int j = 0 ; j < 4 ; j++){
            s << m[i][j] << " ";
        }
        s << endl;
    }
    return s;
}



#endif // MATRIX4_H
