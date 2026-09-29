#include <cassert>
#include <cmath>
#include <limits>
#include <vector>

#include "geometry.h"
#include "rasterizer.h"
#include "tgaimage.h"

int main(){
    TGAImage image(100,100);
    std::vector<double> zbuffer(
        image.width() * image.height(),
        -std::numeric_limits<double>::infinity()
    );
    Vec4 a_local{-0.5, -0.5, 0.2, 1.0};
    Vec4 b_local{ 0.5, -0.5, 0.2, 1.0};
    Vec4 c_local{ 0.0,  0.5, 0.2, 1.0};
    TGAColor red{0,0,255};
    TGAColor blue{255,0,0};
    TGAColor green{0,255,0};
    TGAColor white{255,255,255};

    Mat4 vp = viewport(image.width(), image.height());
    Vec4 alscreen = vp * a_local;
    Vec4 blscreen = vp * b_local;
    Vec4 clscreen = vp * c_local; 

    Vec2 al2 = xy(alscreen);
    Vec2 bl2 = xy(blscreen);
    Vec2 cl2 = xy(clscreen);

    triangle_barycentric_depth(al2, bl2, cl2, alscreen.z, blscreen.z, clscreen.z, image, zbuffer, white);


    Mat4 trans = translation(0.2, 0.1, 0.0);
    double angle = 30.0;
    Mat4 rotate = rotation_z(angle);
    Mat4 sc = scale(0.8, 0.8,1.0);

    Mat4 model1 = trans * rotate * sc;
    Mat4 model2 = rotate * trans * sc;


    Vec4 a_m1 = model1 * a_local; //先进行几何空间坐标变换
    Vec4 b_m1 = model1 * b_local;
    Vec4 c_m1 = model1 * c_local;

    Vec4 am1screen = vp * a_m1; //再映射回屏幕坐标空间
    Vec4 bm1screen = vp * b_m1;
    Vec4 cm1screen = vp * c_m1;

    Vec2 am12 = xy(am1screen);
    Vec2 bm12 = xy(bm1screen);
    Vec2 cm12 = xy(cm1screen);
    //使用第一种变换model1
    triangle_barycentric_depth(am12, bm12, cm12, am1screen.z, bm1screen.z, cm1screen.z, image, zbuffer, red);

    Vec4 a_m2 = model2 * a_local;
    Vec4 b_m2 = model2 * b_local;
    Vec4 c_m2 = model2 * c_local;

    Vec4 am2screen = vp * a_m2;
    Vec4 bm2screen = vp * b_m2;
    Vec4 cm2screen = vp * c_m2;

    Vec2 am22 = xy(am2screen);
    Vec2 bm22 = xy(bm2screen);
    Vec2 cm22 = xy(cm2screen);
    //使用第二种变换model2
    triangle_barycentric_depth(am22, bm22, cm22, am2screen.z, bm2screen.z, cm2screen.z, image, zbuffer, green);
    image.write("NDC_MODEL_MATRIX.tga");
    
}






