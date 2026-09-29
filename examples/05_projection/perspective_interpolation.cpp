#include <limits>
#include <numbers>
#include <vector>

#include "geometry.h"
#include "rasterizer.h"
#include "tgaimage.h"

int main(){
    TGAImage image(100,100);
    std::vector<double> zbuffer(image.width() * image.height(),std::numeric_limits<double>::infinity());

    Vec4 a_local{-0.7, -0.5, -1.5, 1.0};
    Vec4 b_local{ 0.7, -0.5, -5.0, 1.0};
    Vec4 c_local{ 0.0,  0.8, -2.0, 1.0};

    TGAColor red{0,0,255};
    TGAColor green{0,255,0};
    TGAColor blue{255,0,0};

    //model
    Mat4 model = identity();
    //view
    Mat4 view = look_at(
        Vec3{0.0, 0.0, 0.0},
        Vec3{0.0, 0.0, -1.0},
        Vec3{0.0, 1.0, 0.0}
    );
    //projection
    const double aspect = static_cast<double>(image.width()) / image.height();
    Mat4 projection = perspective(60.0 * std::numbers::pi / 180.0,aspect,0.1,10.0);
    //viewport
    Mat4 vp = viewport(image.width(), image.height());

    //projection <- view <- model, clip裁剪空间
    Mat4 clip_transform = projection * view * model;
    
    Vec4 a_clip = clip_transform * a_local;
    Vec4 b_clip = clip_transform * b_local;
    Vec4 c_clip = clip_transform * c_local;

    Vec3 a_ndc = perspective_divide(a_clip);
    Vec3 b_ndc = perspective_divide(b_clip);
    Vec3 c_ndc = perspective_divide(c_clip);

    Vec4 a_screen = vp * Vec4{a_ndc.x, a_ndc.y, a_ndc.z, 1.0};
    Vec4 b_screen = vp * Vec4{b_ndc.x, b_ndc.y, b_ndc.z, 1.0};
    Vec4 c_screen = vp * Vec4{c_ndc.x, c_ndc.y, c_ndc.z, 1.0};

    triangle_barycentric_gradient_depth(xy(a_screen), xy(b_screen), xy(c_screen),a_ndc.z, b_ndc.z, c_ndc.z,1.0 / a_clip.w,1.0 / b_clip.w,1.0 / c_clip.w,red, green, blue,image, zbuffer);
    image.write("perspective_gradient.tga");
}