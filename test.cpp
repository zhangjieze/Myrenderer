#include <iostream>
#include "rasterizer.h"
#include "tgaimage.h"

int main(){
    TGAImage image(100,100);
    TGAColor red{0,0,255};
    TGAColor blue{255,0,0};
    TGAColor green{0,255,0};

    line_bresenham(10,10,90,60,image,red);

    image.write("bresenham1.tga");
    return 0;
}