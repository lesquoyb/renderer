#include <iostream>
#include "vertexSwarm.h"


using namespace std;

int main(int argc, char *argv[]){

    VertexSwarm v("obj/african_head.obj");
    TGAImage texture;
    texture.read_tga_file("obj/african_head_diffuse.tga");
    texture.flip_vertically();
    TGAImage* img = v.generateImage(600, 800, texture);
    if(img->write_tga_file("output.tga"))
        return 0;
    return 1;
}
