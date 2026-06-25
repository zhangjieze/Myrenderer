#include <iostream>
#include "rasterizer.h"
#include "tgaimage.h"

int main(){
    TGAImage image(100,100);
    TGAColor red{0,0,255};
    TGAColor blue{255,0,0};
    TGAColor green{0,255,0};

    line(10, 10, 90, 10, image, red);    // 水平
    line(10, 10, 10, 90, image, green);  // 垂直
    line(10, 10, 90, 60, image, blue);   // 低斜率
    line(10, 10, 40, 90, image, red);    // 高斜率
    line(90, 90, 10, 20, image, green);  // 反向
    line(50, 50, 50, 50, image, blue);   // 单点

    image.write("ddatest.tga");
    return 0;
}