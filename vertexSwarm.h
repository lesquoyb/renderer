#ifndef VERTEXSWARM_H
#define VERTEXSWARM_H

#include <climits>
#include "vertex.h"
#include "triangle.h"
#include "tgaimage.h"
#include "matrix4.h"
#include "shader.h"


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

        float**  zBuffer = (float**) malloc(width * sizeof(float*));
        for(int i = 0 ; i < width; i++){
            zBuffer[i] = (float*) malloc(height * sizeof(float) );
            for(int j = 0 ; j < height ; j++){
                zBuffer[i][j] = -9999;
            }
        }

        Vertex light_source(0, 1, 1);
        Vertex eye(0,0,1);
        Vertex center(0,0,0);
        Vertex up(0,1,0);
        Matrix4 modelView  = Matrix4::Matrix4::lookat(eye, center, up);//TODO: pas bien il faut décomposer view et model
        Matrix4 projection = Matrix4::rotationMatrix( 45 * PI/180);

        Matrix4 viewport = Matrix4::viewport(width/8, height/8, 255, width*0.8, height*0.8);


        RealisticShader real(viewport, projection, modelView, light_source, normal_map, specular_map);
        Metal metal(viewport, projection, modelView, light_source, normal_map);
        Gouraud gouraud(viewport,projection, modelView, normalTrigs[0], light_source);
        Cartoon cartoon(viewport, projection, modelView, normalTrigs[0], light_source);
        vector<Shader*> shaderList;
     //   shaderList.push_back(new Contour(viewport,projection, modelView));
        //shaderList.push_back(&gouraud);
        shaderList.push_back(&real);
       //   shaderList.push_back(&metal);
     //   shaderList.push_back(new BlackAndWhite(viewport, projection, modelView));
     //   shaderList.push_back(&cartoon);
      //  shaderList.push_back(new DominantColor(viewport, projection, modelView));

        bool voxel = true;
        for(int i = 0 ; i < trigs.size() ; i++){

            gouraud.normals = normalTrigs[i];
            cartoon.normals = normalTrigs[i];
            Vertex  v1 = gouraud.vertex(*trigs[i].v1),
                    v2 = gouraud.vertex(*trigs[i].v2),
                    v3 = gouraud.vertex(*trigs[i].v3);
            //get the bounding box
            int xMin = min(min(v1.x, v2.x), v3.x);
            int yMin = min(min(v1.y, v2.y), v3.y);
            int xMax = max(max(v1.x, v2.x), v3.x);
            int yMax = max(max(v1.y, v2.y), v3.y);

            if (( xMin >= 0 and yMin >= 0 and xMax <= textureImg.get_width() and yMax <= textureImg.get_height() )){//si tout le triangle est dans l'image, sinon flemme de faire un découpage propre

                for( int x = xMin; x <= xMax ; x++ ){
                    for( int y = yMin; y <= yMax ; y++ ){

                        Vertex bary = barycentre(v1, v2, v3, x, y);

                        if(bary.x >= 0 and bary.y >= 0 and bary.z >= 0){

                            float z_interpolation =   v1.z * bary.x
                                                    + v2.z * bary.y
                                                    + v3.z * bary.z;

                            if(zBuffer[x][y] < z_interpolation){

                                double vx =    trigTextures[i].v1->x * bary.x
                                            +  trigTextures[i].v2->x * bary.y
                                            +  trigTextures[i].v3->x * bary.z;
                                double vy =    trigTextures[i].v1->y * bary.x
                                            +  trigTextures[i].v2->y * bary.y
                                            +  trigTextures[i].v3->y * bary.z;
                                real.x = vx;
                                real.y = vy;
                                TGAColor color = textureImg.get(vx * textureImg.get_width(),vy * textureImg.get_height());
                                bool draw = false;
                                for(int i = 0 ; i < shaderList.size() ;i++){
                                    if(!shaderList[i]->fragment(bary, color)){
                                        draw = true;
                                    }
                                }
                                if(draw){
                                    img->set(x, y, color);
                                    zBuffer[x][y] = z_interpolation;
                                }
                            }
                        }
                    }
                }
            }
      }


        img->flip_vertically();

        return img;
    }


};



#endif
