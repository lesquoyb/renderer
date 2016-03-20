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
    vector<Triangle> normalTrigs;

    VertexSwarm(string file){
			
        std::ifstream is(file.c_str(), std::ios_base::in);
        string str, lol;
        while(getline(is, str)){

            std::istringstream iss(str.c_str());
            iss >> str;
            if(strcmp(str.c_str(),"v") == 0 && str.size() == 1){
                float x, y , z;
                iss  >>  x >> y >> z;
                swarm.push_back(new Vertex(x++, y++, z++));
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
                int x, y, z, tx, ty , tz, nx, ny, nz;
                char slash;
                iss >> x >> slash >> tx >> slash >> nx;
                iss >> y >> slash >> ty >> slash >> ny;
                iss >> z >> slash >> tz >> slash >> nz;
                x--;y--;z--;
                tx--;ty--;tz--;
                nx--;ny--;nz--;
                trigs.push_back(Triangle(swarm[x], swarm[y], swarm[z]));
                trigTextures.push_back(Triangle(textures[tx],textures[ty], textures[tz]));
                normalTrigs.push_back(Triangle(normalVectors[nx], normalVectors[ny], normalVectors[nz]) );
            }

        }

    }


    TGAImage* generateImage(int width, int height, const TGAImage& textureImg, const TGAImage & normal_map, const TGAImage &specular_map,int wOffset = 0, int hOffset = 0 ) const{

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

        Vertex sun(0, 0, 1);
        Vertex eye(0,0,1);
        Vertex center(0,0,0);
        Matrix4 modelView  = Matrix4::identity();//Matrix4::lookat(eye, center, Vertex(0,1,0));//TODO: pas bien il faut décomposer view et model
        Matrix4 projection = Matrix4::identity();
        projection[3][2] = -1.f/(eye-center).norm();


        Matrix4 viewport = Matrix4::viewport(0, 0, 255, width, height);
        Matrix4 rotation = Matrix4::rotationMatrix( 30 * PI/180);
        Matrix4 pipeline = viewport * rotation ; //* modelView ;


        for(int i = 0 ; i < trigs.size() ; i++){

            Vertex  screen_coord1 = pipeline * *trigs[i].v1 ,
                    screen_coord2 = pipeline * *trigs[i].v2 ,
                    screen_coord3 = pipeline * *trigs[i].v3 ;

            Triangle(&screen_coord1, &screen_coord2, &screen_coord3).draw(*img, trigTextures[i], textureImg, normal_map, specular_map, zBuffer, normalTrigs[i], sun );

      }


        img->flip_vertically();

        return img;
    }


};



#endif
