#include <iostream>
#include "rasterizer.h"
#include "tgaimage.h"

int main(){
    TGAImage image(100,100);
    TGAColor red{0,0,255};
    TGAColor blue{255,0,0};
    TGAColor green{0,255,0};
    line_bresenham(10, 10, 90, 10, image, red);    // 水平
    line_bresenham(10, 10, 10, 90, image, green);  // 垂直
    line_bresenham(10, 10, 90, 60, image, blue);   // 低斜率正
    line_bresenham(10, 60, 90, 10, image, red);    // 低斜率负
    line_bresenham(10, 10, 40, 90, image, green);  // 高斜率正
    line_bresenham(40, 90, 10, 10, image, blue);   // 高斜率反向
    line_bresenham(90, 90, 10, 20, image, red);    // 反向混合
    line_bresenham(50, 50, 50, 50, image, blue);   // 单点
    image.write("bresenham_final_test.tga");
    return 0;
}