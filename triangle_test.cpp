#include <iostream>
#include "tgaimage.h"
#include "rasterizer.h"

int main(){
    TGAColor blue{255,0,0};
    TGAImage image(100,100);
    triangle_wireframe(10, 10, 80, 20, 30, 90, image, blue);
    image.write("triangle1.tga");
    return 0;
}