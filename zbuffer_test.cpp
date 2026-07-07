#include <iostream>
#include "tgaimage.h"
#include "rasterizer.h"
#include <vector>
#include <limits>

int main(){
    TGAImage image(100, 100);

    std::vector<double> zbuffer(
        image.width() * image.height(),
        -std::numeric_limits<double>::infinity()
    );
    
    TGAColor red{0, 0, 255};
    TGAColor green{0, 255, 0};
    
    triangle_barycentric_depth(
        Vec2{10, 10}, Vec2{80, 20}, Vec2{30, 90},
        0.2, 0.3, 0.4,
        image, zbuffer, red
    );
    
    triangle_barycentric_depth(
        Vec2{20, 20}, Vec2{90, 30}, Vec2{50, 80},
        0.1, 0.5, 0.3,
        image, zbuffer, green
    );

    image.write("triangle_barycentric_depth.tga");
}