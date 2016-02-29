#ifndef VERTEXSWARM_H
#define VERTEXSWARM_H

#include <climits>
#include "vertex.h"
#include "triangle.h"
#include "tgaimage.h"


class VertexSwarm{


public:

    vector<Vertex*> swarm;
    vector<Triangle> trigs;
	vector<Vertex*> textures;
    vector<Triangle> trigTextures;

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
            else if(strcmp(str.c_str(),"vt") == 0 && str.size() == 2){
                float x, y, z;
                iss >> x >> y >> z;
                textures.push_back(new Vertex(x,y,z));

            }
            else if(strcmp(str.c_str(), "f") == 0  && str.size() == 1){
                int x, y, z, tx, ty , tz;
                char slash;
                iss  >> x >> slash >> tx >> lol >>  y >> slash >> ty >> lol >> z >> slash >> tz >> lol;
                x--;
                y--;
                z--;
                tx--;ty--;tz--;
                trigs.push_back(Triangle(swarm[x], swarm[y], swarm[z]));
                trigTextures.push_back(Triangle(textures[tx],textures[ty], textures[tz]));
            }
        }

    }


    TGAImage* generateImage(int width, int height, const TGAImage& textureImg, int wOffset = 0, int hOffset = 0 ) const{

		TGAImage* img = new TGAImage(width,height,TGAImage::RGB);
        
		int realW = (width - wOffset)/2,
			realH = (height - hOffset)/2;
		float**  zBuffer = (float**) malloc(width * sizeof(float*));
		for(int i = 0 ; i < width; i++){
			zBuffer[i] = (float*) malloc(height * sizeof(float) );
			for(int j = 0 ; j < height ; j++){
				zBuffer[i][j] = -9999; //TODO: YOLOOO
			}
		}
	
        for(Vertex* v : swarm){
            v->normalize(realW, realH);
        }
		
        Vertex sun(0.3, 0, 0.7);
			
        for(int i = 0 ; i < trigs.size() ; i++){
            Triangle t = trigs[i];
            t.draw(*img, trigTextures[i], textureImg , zBuffer, sun);
        }


		img->flip_vertically();

        return img;
    }


};



#endif
