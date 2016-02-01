#ifndef TOOLS_H
#define TOOLS_H

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
#include "vertex.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green   = TGAColor(0, 255,   0,   255);


using std::vector;
using std::cout;
using std::cin;
using std::string;
using std::istringstream;



void line( int x0, int y0, int x1, int y1, TGAImage &image, TGAColor c){
    //algo ultime de Bresenham
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


bool startswith(char* str, char* start){
    return strncmp(str, start, strlen(start) ) == 0;
}


#endif
