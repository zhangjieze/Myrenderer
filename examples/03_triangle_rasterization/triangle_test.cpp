/*
后续补充:
历史阶段代码：此时三角形属性仍直接使用屏幕重心坐标进行插值,尚未引入透视校正插值，因此本文件调用的是早期版本的光栅化接口.
后续在加入 Projection Matrix 后,引入每个顶点的 1/w进行矫正,此处为历史快照.
当前 rasterizer 接口已与此阶段不同,详细演进过程见 docs/learning_notes.md.
*/




#include <iostream>
#include "myrenderer/tgaimage.h"
#include "myrenderer/rasterizer.h"
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
    TGAColor blue{255, 0, 0};

    TGAColor yellow{0, 255, 255};
    TGAColor cyan{255, 255, 0};
    TGAColor magenta{255, 0, 255};
    
    triangle_barycentric_gradient_depth(
        Vec2{10, 10}, Vec2{80, 20}, Vec2{30, 90},
        0.2, 0.3, 0.4,
        red, green, blue,
        image, zbuffer
    );
    
    triangle_barycentric_gradient_depth(
        Vec2{20, 20}, Vec2{90, 30}, Vec2{50, 80},
        0.1, 0.5, 0.3,
        yellow, cyan, magenta,
        image, zbuffer
    );

    image.write("examples/03_triangle_rasterization/output/triangle_gradient_depth.tga");
}