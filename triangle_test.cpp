#include <iostream>
#include "tgaimage.h"
#include "rasterizer.h"

int main(){
    TGAColor blue{255,0,0};
    TGAColor red{0,0,255};
    TGAImage image(100,100);
    triangle_filled(10, 10, 80, 20, 30, 90, image, red);
    triangle_wireframe(10, 10, 80, 20, 30, 90, image, blue);
    image.write("triangle2.tga");
    return 0;
}