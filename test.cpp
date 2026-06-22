#include <iostream>
#include "tgaimage.h"

int main(){
    TGAColor red{0,0,255};
    TGAColor green{0,255,0};
    TGAColor blue{255,0,0};

    TGAImage image(100, 100);
    image.set(50, 50, red);
    image.set(60, 60, green);
    image.set(70, 70, blue);
    line(0, 0, 100, 100, image, red);
    image.write("day8line.tga");
    
    std::cout << "success" << std::endl;
    return 0;
}