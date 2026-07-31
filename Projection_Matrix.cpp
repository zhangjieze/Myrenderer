#include <numbers>
#include "tgaimage.h"
#include "rasterizer.h"
#include "geometry.h"

int main(){
	TGAImage image(400,300);

	Vec4 a_local{-0.6, -0.5, 0.0, 1.0};
    Vec4 b_local{ 0.6, -0.5, 0.0, 1.0};
    Vec4 c_local{ 0.0,  0.7, 0.0, 1.0};

    TGAColor red{0, 0, 255};
    TGAColor blue{255, 0, 0};


    Mat4 model_near = translation(0.0, 0.0, -2.0);
    Mat4 model_far = translation(0.0, 0.0, -5.0);

    Mat4 view = look_at(//摄像机look_at
        Vec3{0.0, 0.0, 0.0}, //eye
        Vec3{0.0, 0.0, -1.0}, //target
        Vec3{0.0, 1.0, 0.0} //up
    );

    const double aspect = static_cast<double> (image.width()) / image.height();
    //projection
    Mat4 projection = perspective(
        60.0 * std::numbers::pi / 180.0, //pi / 3的广角
        aspect,
        0.1,
        10.0
    );
    //viewport
    Mat4 vp = viewport(image.width(), image.height());


    const Mat4 transform_near = projection * view * model_near;
    const Mat4 transform_far = projection * view * model_far;

    auto toscreen = [&](const Mat4& clip_transform, const Vec4& p_local) {
        const Vec4 p_clip = clip_transform * p_local;
        const Vec3 p_ndc = perspective_divide(p_clip); //剪裁空间规范化处理方便进行透视除法

        return vp * Vec4{p_ndc.x, p_ndc.y, p_ndc.z, 1.0};
    };

    Vec4 anear = toscreen(transform_near,a_local);
    Vec4 bnear = toscreen(transform_near,b_local);
    Vec4 cnear = toscreen(transform_near,c_local);

    triangle_wireframe(static_cast<int>(anear.x),static_cast<int>(anear.y),static_cast<int>(bnear.x),static_cast<int>(bnear.y),static_cast<int>(cnear.x),static_cast<int>(cnear.y),image,red);

    Vec4 afar = toscreen(transform_far, a_local);
    Vec4 bfar = toscreen(transform_far, b_local);
    Vec4 cfar = toscreen(transform_far, c_local);

    triangle_wireframe(static_cast<int>(afar.x),static_cast<int>(afar.y),static_cast<int>(bfar.x),static_cast<int>(bfar.y),static_cast<int>(cfar.x),static_cast<int>(cfar.y),image,blue);

    image.write("Projectin_matrix.tga");


}