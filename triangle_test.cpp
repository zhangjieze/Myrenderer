#include <iostream>
#include "tgaimage.h"
#include "rasterizer.h"
#include "geometry.h"

int main(){
    TGAColor blue{255,0,0};
    TGAColor red{0,0,255};
    TGAImage image(100,100);
    Vec2 a{10,10};
    Vec2 b{80,20};
    Vec2 c{30,90};
    triangle_barycentric_filled(a,b,c,image,red);
    triangle_wireframe(10, 10, 80, 20, 30, 90, image, blue);
    image.write("triangle_barycentric.tga");
    return 0;
}