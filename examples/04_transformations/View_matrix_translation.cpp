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
    TGAColor blue{255,0,0}; //right
    TGAColor green{0,255,0}; //up


    Mat4 view_center = translation(0.0, 0.0,0.0);
    Mat4 view_right = translation(-0.3, 0.0,0.0);
    Mat4 view_up = translation(0.0, -0.3,0.0);

    Mat4 vp = viewport(image.width(), image.height());
    Mat4 modeli = identity();
    Mat4 trcenter = vp * view_center * modeli;
    Mat4 trright = vp * view_right * modeli;
    Mat4 trup = vp * view_up * modeli;
    
    //center
    Vec4 a_center = trcenter * a_local;
    Vec4 b_center = trcenter * b_local;
    Vec4 c_center = trcenter * c_local;
    Vec2 ac = xy(a_center);
    Vec2 bc = xy(b_center);
    Vec2 cc = xy(c_center);

    triangle_barycentric_filled(ac, bc, cc, image, red);

    //right
    Vec4 a_right = trright * a_local;
    Vec4 b_right = trright * b_local;
    Vec4 c_right = trright * c_local;
    Vec2 ar = xy(a_right);
    Vec2 br = xy(b_right);
    Vec2 cr = xy(c_right);
    triangle_barycentric_filled(ar, br, cr, image, blue);

    //up
    Vec4 a_up = trup * a_local;
    Vec4 b_up = trup * b_local;
    Vec4 c_up = trup * c_local;
    Vec2 au = xy(a_up);
    Vec2 bu = xy(b_up);
    Vec2 cu = xy(c_up);
    triangle_barycentric_filled(au, bu, cu, image, green);

    image.write("view_matirx_translation.tga");
    
    
}