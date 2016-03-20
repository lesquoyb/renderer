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



    void draw(TGAImage &img, const Triangle &texture, const TGAImage &textureImg,const TGAImage &normal_map, const TGAImage &specular_map ,float** zBuffer, const Triangle &normals, const Vertex &light_source) const{
		
        //get the bounding box
        int xMin = min(min(v1->x, v2->x), v3->x);
        int yMin = min(min(v1->y, v2->y), v3->y);
        int xMax = max(max(v1->x, v2->x), v3->x);
        int yMax = max(max(v1->y, v2->y), v3->y);

        if( xMin >= 0 and yMin >= 0 and xMax <= textureImg.get_width() and yMax <= textureImg.get_height() )//si tout le triangle est dans l'image, sinon flemme de faire un découpage propre
            for( int x = xMin; x <= xMax ; x++ ){
                for( int y = yMin; y <= yMax ; y++ ){

                    Vertex bary = barycentre(x, y);

                    if(bary.x >= 0 and bary.y >= 0 and bary.z >= 0){

                        float z_interpolation =   v1->z * bary.x
                                                + v2->z * bary.y
                                                + v3->z * bary.z;

                        if(zBuffer[x][y] < z_interpolation){

                            double vx =    texture.v1->x * bary.x
                                        +  texture.v2->x * bary.y
                                        +  texture.v3->x * bary.z;
                            double vy =    texture.v1->y * bary.x
                                        +  texture.v2->y * bary.y
                                        +  texture.v3->y * bary.z;


                            Vertex n_gouraud = (*normals.v1 * bary.x + *normals.v2 * bary.y + *normals.v3 * bary.z).normalized() * light_source;
                            float gouraud_light = max( n_gouraud.x  + n_gouraud.y + n_gouraud.z , 0.f);

                            TGAColor texture_color = textureImg.get( vx * textureImg.get_width(), vy * textureImg.get_height() );

                            TGAColor normal_color  = normal_map.get( vx * normal_map.get_width(), vy * normal_map.get_height() );
                            Vertex n_map = Vertex( normal_color.b, normal_color.r, normal_color.g ).normalized();

                            Vertex reflected = ( n_map * 2 * (n_map * light_source) - light_source).normalized();

                            float diffuse_light  = max( n_map.x * light_source.x +     n_map.y * light_source.y +     n_map.z * light_source.z, 0.f);
                            float specular_light = max( pow( max(reflected.z, 0.0f), specular_map.get(vx,vy).r), 0.);
                            float ambient_light  = 10;


                            float light = diffuse_light + specular_light;

                            TGAColor tColor(min( (int) ( ambient_light + texture_color.r * light), 255),
                                            min( (int) ( ambient_light + texture_color.g * light), 255),
                                            min( (int) ( ambient_light + texture_color.b * light), 255),
                                            texture_color.a);
                            img.set(x, y, tColor);
                            zBuffer[x][y] = z_interpolation;
                        }
                    }
                }
            }

	}


};

#endif
