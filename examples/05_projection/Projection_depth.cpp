#include <limits>
#include <numbers>
#include <vector>


#include "geometry.h"
#include "tgaimage.h"
#include "rasterizer.h"


int main(){
	TGAImage image(400, 300);
	std::vector<double> zbuffer(image.width()*image.height(), std::numeric_limits<double>::infinity());

	Vec4 a_local{-0.6, -0.5, 0.0, 1.0};
    Vec4 b_local{ 0.6, -0.5, 0.0, 1.0};
    Vec4 c_local{ 0.0,  0.7, 0.0, 1.0};

    TGAColor red{0, 0, 255};
    TGAColor blue{255, 0, 0};

    Mat4 model_near = translation(0.0, 0.0, -2.0);
    Mat4 model_far = translation(1.2, 0.0, -5.0);

    Mat4 view = look_at(
        Vec3{0.0, 0.0, 0.0},
        Vec3{0.0, 0.0, -1.0},
        Vec3{0.0, 1.0, 0.0}
    );

    const double aspect = static_cast<double>(image.width()) / image.height();

    Mat4 projection = perspective(
        60.0 * std::numbers::pi / 180.0, //pi / 3的广角
        aspect,
        0.1,
        10.0
    );

    Mat4 vp = viewport(image.width(), image.height());


    //组装
    const Mat4 transform_near = projection * view * model_near;
    const Mat4 transform_far = projection * view * model_far;

    auto to_screen = [&](const Mat4& clip_transform, const Vec4& p_local) {
        const Vec4 p_clip = clip_transform * p_local;
        const Vec3 p_ndc = perspective_divide(p_clip); //剪裁空间规范化处理方便进行透视除法

        return vp * Vec4{p_ndc.x, p_ndc.y, p_ndc.z, 1.0};
    };


    //near
    Vec4 a_near = to_screen(transform_near, a_local);
    Vec4 b_near = to_screen(transform_near, b_local);
    Vec4 c_near = to_screen(transform_near, c_local);

    triangle_barycentric_depth(xy(a_near),xy(b_near),xy(c_near),a_near.z,b_near.z,c_near.z,image,zbuffer,red);

    //far
    Vec4 a_far = to_screen(transform_far, a_local);
    Vec4 b_far = to_screen(transform_far, b_local);
    Vec4 c_far = to_screen(transform_far, c_local);

    triangle_barycentric_depth(xy(a_far),xy(b_far),xy(c_far),a_far.z,b_far.z,c_far.z,image,zbuffer,blue);

    image.write("projection_depth.tga");




}