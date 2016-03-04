#ifndef VERTEXSWARM_H
#define VERTEXSWARM_H

#include <climits>
#include "vertex.h"
#include "triangle.h"
#include "tgaimage.h"
#include "matrix4.h"


class VertexSwarm{


public:

    vector<Vertex*> swarm;
    vector<Triangle> trigs;
    vector<Vertex*> textures;
    vector<Triangle> trigTextures;
    vector<Vertex*> normalVectors;


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
            else if(strcmp(str.c_str(),"vn") == 0 && str.size() == 2){
                float x, y, z;
                iss >> x >> y >> z;
                normalVectors.push_back(new Vertex(x,y,z));
            }
            else if(strcmp(str.c_str(), "f") == 0  && str.size() == 1){
                int x, y, z, tx, ty , tz;
                char slash;
                iss  >> x >> slash >> tx >> lol >>  y >> slash >> ty >> lol >> z >> slash >> tz >> lol;
                x--;y--;z--;
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






        for(int i = 0 ; i < swarm.size() ; i++){

//            triangle(screen_coords[0], screen_coords[1], screen_coords[2], intensity[0], intensity[1], intensity[2], image, zbuffer);

            //cout << rotation[0][0] << " " << rotation[0][1] << " " << rotation[1][0] << endl;
            //      Vertex tmp = (viewport * rotation * *swarm[i]);
            //cout << tmp.x << " " << tmp.y << " " <<tmp.z << endl;
        //    *swarm[i]= tmp;
            //   swarm[i]->x = tmp.x;
       //     swarm[i]->y = tmp.y;
        //    swarm[i]->z = tmp.z;
        }

        Vertex sun(0.3, 0, 0.7);
        Vertex eye(0,0,1);
        Vertex center(0,0,0);
        Matrix4 modelView  = Matrix4::lookat(eye, center, Vertex(0,1,0));//TODO: pas bien il faut décomposer view et model
        Matrix4 projection = Matrix4::identity();
        projection[3][2] = -1.f/(eye-center).norm();
        Matrix4 viewport   = Matrix4::viewport(width/8, height/8, 255, width*3/4, height*3/4);
        Matrix4 z = viewport * projection * modelView;

        for(int i = 0 ; i < trigs.size() ; i++){

            Vertex screen_coords[3];
            Vertex world_coords[3];
            for (int j=0; j<3; j++) {

                Vertex* v = (j == 0 ) ? trigs[i].v1 : (j == 1) ? trigs[i].v2 : trigs[i].v3;
                screen_coords[j] =  Vertex(z * (Matrix4::identity()* *v));
                world_coords[j]  = *v;
            }
            Triangle(&screen_coords[0], &screen_coords[1], &screen_coords[2]).draw(*img,trigTextures[i],textureImg,zBuffer,sun);
      //    t.draw(*img, trigTextures[i], textureImg , zBuffer, sun);

            //            triangle(screen_coords[0], screen_coords[1], screen_coords[2], intensity[0], intensity[1], intensity[2], image, zbuffer);


/*
            Triangle t = trigs[i];
            t.draw(*img, trigTextures[i], textureImg , zBuffer, sun);
  */
      }


        img->flip_vertically();

        return img;
    }


};



#endif
