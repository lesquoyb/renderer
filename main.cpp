#include <iostream>
#include "vertexSwarm.h"
#include "matrix4.h"

using namespace std;

const string body = "obj/boggie/body";
const string head = "obj/boggie/head";
const string diablo = "obj/diablo3_pose/diablo3_pose";


TGAImage* generate(const string &file_path);

int main(int argc, char *argv[]){


    TGAImage* img = generate(diablo);
    if(img->write_tga_file("output.tga"))
        return 0;
    return 1;

}

TGAImage* generate(const string &file_path){
    VertexSwarm v(file_path + ".obj");
    TGAImage texture;
    texture.read_tga_file(( file_path + "_diffuse.tga").c_str());
    texture.flip_vertically();
    return v.generateImage(texture.get_width(), texture.get_height(), texture);

}
