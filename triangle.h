#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vertex.h"
#include "tools.h"
using namespace std;

class vect2{
	public:
        double x, y;

        vect2():vect2(0,0){}
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

    Triangle(Vertex* x1, Vertex* x2, Vertex* x3):
        v1(x1),v2(x2),v3(x3)
		{

			Vertex tmp((*v3 - (*v1)).cross(*v2 - (*v1)));
		}

	Vertex barycentre(const int &x, const int &y) const{
		
		Vertex  x1(v3->x - v1->x, 
				   v2->x - v1->x, 
				   v1->x - x),
				x2(v3->y - v1->y, 
				   v2->y - v1->y, 
				   v1->y - y),
				x3 = x1.cross(x2);
		float xp =  1 - (x3.x +x3.y)/x3.z,
			  yp =  x3.y/x3.z,
			  zp =  x3.x/x3.z;
		return Vertex(xp, yp, zp);	
	}



    void draw(TGAImage &img, const Triangle &texture, const TGAImage &textureImg ,float** zBuffer,const Vertex& light_source ) const{
		
        //get the bounding box
        int xMin = min(min(v1->x, v2->x), v3->x);
        int yMin = min(min(v1->y, v2->y), v3->y);
        int xMax = max(max(v1->x, v2->x), v3->x);
        int yMax = max(max(v1->y, v2->y), v3->y);
        float lumiere;

        Vertex light_vector = (*v1 - *v3).cross(*v2 - *v3);
        light_vector = light_vector.normalized();
        cout << xMin << " " << yMin << " " << xMax << " " << yMax << endl;
        if(xMin >= 0 and yMin>=0 and xMax<= textureImg.get_width() and yMax <= textureImg.get_height())
        for( int x = xMin; x <= xMax ; x++){
			for( int y = yMin; y <= yMax ; y++){
				Vertex center = barycentre(x, y);
		    	if(center.x >= 0 and center.y >= 0 and center.z >= 0){

					float Pz = 	  v1->z * center.x 
								+ v2->z * center.y 
								+ v3->z * center.z;
                    if(zBuffer[x][y] < Pz){
                        double vx =     texture.v1->x * center.x
                                    +  texture.v2->x * center.y
                                    +  texture.v3->x * center.z;
                        double vy =    texture.v1->y * center.x
                                    +  texture.v2->y * center.y
                                    +  texture.v3->y * center.z;
                        TGAColor c = textureImg.get( vx * textureImg.get_width(), vy * textureImg.get_height());

                        lumiere = abs(  light_source.x*light_vector.x
                                      + light_source.y*light_vector.y
                                      + light_source.z*light_vector.z);

                        TGAColor tColor( (int) (c.r * lumiere),
                                         (int) (c.g * lumiere),
                                         (int) (c.b * lumiere),
                                                          c.a);
						img.set(x, y, tColor);
						zBuffer[x][y] = Pz;
					}
				}
			}
		}

	}


};

#endif
