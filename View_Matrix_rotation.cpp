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
    TGAColor red{0,0,255}; //center
    TGAColor blue{255,0,0}; //30.0
    TGAColor green{0,255,0}; //-45.0



    Mat4 vp = viewport(image.width(), image.height());
    Mat4 modeli = identity();
    double camera_angle1 = 30.0;
    double camera_angle2 = -45.0;
    Mat4 view1 = rotation_z(-camera_angle1);
    Mat4 view2 = rotation_z(-camera_angle2);

    //center
    Vec4 ascreen_center = vp * modeli * a_local;
    Vec4 bscreen_center = vp * modeli * b_local;
    Vec4 cscreen_center = vp * modeli * c_local;
    triangle_barycentric_filled(xy(ascreen_center), xy(bscreen_center), xy(cscreen_center), image, red);


    //30.0
    Mat4 tr1 = vp * view1 * modeli;
    Vec4 ascreen = tr1 * a_local;
    Vec4 bscreen = tr1 * b_local;
    Vec4 cscreen = tr1 * c_local;
    triangle_barycentric_filled(xy(ascreen), xy(bscreen), xy(cscreen), image, blue);

    //-45.0
    Mat4 tr2 = vp * view2 * modeli;
    Vec4 ascreen_45 = tr2 * a_local;
    Vec4 bscreen_45 = tr2 * b_local;
    Vec4 cscreen_45 = tr2 * c_local;
    triangle_barycentric_filled(xy(ascreen_45), xy(bscreen_45), xy(cscreen_45), image, green);


    
    
    image.write("view_matirx_rotation.tga");
    
    
}