#include "tgaimage.h"
#include "vertexSwarm.h"


using namespace std;


int main(int argc, char** argv) {
	VertexSwarm v("obj/african_head.obj");
	TGAImage* i = v.generateImage(600, 800);
    i->write_tga_file("output.tga");
    return 0;

}



