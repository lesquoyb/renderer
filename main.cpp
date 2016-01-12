#include "tgaimage.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <istream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);


using std::vector;
using std::cout;
using std::cin;
using std::string;
using std::istringstream;



bool startswith(char* str, char* start){
    return strncmp(str, start, strlen(start) ) == 0;
}

class Vertex{

public:
    float x, y, z;

    Vertex(float x, float y, float z): x(x),y(y), z(z){}

    void normalize(const float & coefx, const float & coefy, const float & coefz){
        x /= coefx;
        y /= coefy;
        z /= coefz;

        x += 1;
        y += 1;
        z += 1;
    }

};



class Triangle{

public:

    Vertex* v1,* v2,* v3;

    Triangle(Vertex* x1, Vertex* x2, Vertex* x3):
        v1(x1),v2(x2),v3(x3){}


};


void line( int x0, int y0, int x1, int y1, TGAImage &image, TGAColor c){
    //algo ultime de Bresenham (enfin non, flemme de passer en entiers, mais voilà quoi)
    bool steep = std::abs(y1-y0) > std::abs(x1-x0);
    if(steep){
        std::swap(x0,y0);
        std::swap(x1,y1);
    }
    if( x1 <  x0 ){
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 -x0;
    int dy = y1 - y0;
    int derror = std::abs(dy)*2;
    int error = 0;
    int y = y0;
    for( int x = x0 ; x <= x1 ; x++){

        if(steep){
            image.set(y, x, c);
        }
        else{
            image.set(x, y, c);
        }
        error += derror;
        if(error> dx){
            y += y1 - y0 > 0 ? 1 : -1;
            error -= dx*2;
        }

    }
}

void lineBetweenVertices(const Vertex &v1, const Vertex &v2, TGAImage &img, int width, int height, const TGAColor &c){
    int hwidth = width/2, hheight = height/2;
    line( v1.x * hwidth, v1.y * hheight, v2.x * hwidth, v2.y * hheight, img, c);
}


class VertexSwarm{


    void normalize(){
		
		float maxx = swarm[0]->x, maxy = swarm[0]->y, maxz = swarm[0]->z;
        
		for(auto v : swarm){
            maxx = std::max(std::abs(v->x), maxx);
            maxy = std::max(std::abs(v->y), maxy);
            maxz = std::max(std::abs(v->z), maxz);
        }
		
        for(auto v : swarm){
            v->normalize(maxx, maxy, maxz);
        }

    }


public:
    vector<Vertex*> swarm;//TODO: faire des pointeurs, car redondance
    vector<Triangle> trigs;

    VertexSwarm(string file){

        std::ifstream is(file.c_str(), std::ios_base::in);
        string str, lol;
		
        while(getline(is, str)){
			
			std::istringstream iss(str.c_str());
			iss >> str;	
			if(strcmp(str.c_str(),"v") == 0 && str.size() == 1){
            	float x, y , z;
			    iss  >>  x >> y >> z;
				swarm.push_back(new Vertex(x, y, z));
            }
            else if(strcmp(str.c_str(), "f") == 0  && str.size() == 1){
            	int x, y , z;
                iss  >> x >> lol >> y >> lol >> z;	
				x-=1;
				y-=1;
				z-=1;   
				trigs.push_back(Triangle(swarm[x], swarm[y], swarm[z]));
            }
		}

		normalize();

    }


    TGAImage* generateImage(int width, int height, int wmargin = 0, int hmargin = 0 )const{
        TGAImage* img = new TGAImage(width,height,TGAImage::RGB);
        
		/*for(auto v : swarm){
			img->set(v->x, v->y,red);
		}*/
		for(Triangle t : trigs){
            lineBetweenVertices( *t.v1, *t.v2, *img, width - wmargin, height - hmargin, red);
            lineBetweenVertices( *t.v1, *t.v3, *img, width - wmargin, height - hmargin, red);
            lineBetweenVertices (*t.v3, *t.v2, *img, width - wmargin, height - hmargin, red);
        }
		/*		
		Triangle t = trigs[0];
		//cout t.v1->x << " " <<  t.v1->y << std::endl << t.v2->x << " "  << t.v2->y << std::endl << t.v3->x << " " << t.v3->y;*/

        img->flip_vertically();

        return img;
    }


};








int main(int argc, char** argv) {

    VertexSwarm v("obj/african_head.obj");

    TGAImage* i = v.generateImage(600, 800);

    i->write_tga_file("output.tga");

    return 0;

}



