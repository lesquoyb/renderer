#ifndef VERTEXSWARM_H
#define VERTEXSWARM_H

#include "vertex.h"
#include "triangle.h"


class VertexSwarm{


public:
    vector<Vertex*> swarm;
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

    }


    TGAImage* generateImage(int width, int height, int wOffset = 0, int hOffset = 0 ) const{

		TGAImage* img = new TGAImage(width,height,TGAImage::RGB);
        
		int realW = (width - wOffset)/2,
			realH = (height - hOffset)/2;
		
		 for(auto v : swarm){
            v->normalize(realW, realH);
        }
	
		for(Triangle t : trigs){
			//line( t.v1->x, t.v1->y, t.v2->x, t.v2->y, *img, red);
            //line( t.v1->x, t.v1->y, t.v3->x, t.v3->y, *img, red);
            //line( t.v3->x, t.v3->y, t.v2->x, t.v2->y, *img, red);
			t.barycentre(*img,red, realW, realH);
        }
	    
		img->flip_vertically();

        return img;
    }


};



#endif
