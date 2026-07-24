#include "tgaimage.h"
#include "geometry.h"
#include "rasterizer.h"

int main(){
    TGAImage image(100,100);
    Vec4 a_local{-0.6, -0.4, -0.2, 1.0};
    Vec4 b_local{0.6, -0.4, -0.2, 1.0};
    Vec4 c_local{0.0,  0.8,  0.4, 1.0};

    TGAColor red{0,0,255};
    TGAColor blue{255,0,0};

    Mat4 model = identity();
    Mat4 vp = viewport(image.width(), image.height());

    Vec3 target{0.0,0.0,0.0};
    Vec3 up{0.0,1.0,0.0};

    Mat4 view_front = look_at(Vec3{0.0,0.0,2.0}, target, up); //从后往前正对着观察

    Mat4 view_oblique = look_at(Vec3{0.8,0.4,2.0}, target, up); //斜向看

    Mat4 transform_front = vp * view_front * model; // 先model matrix 再 view matrix 再 vp
    Mat4 transform_oblique = vp * view_oblique * model;

    Vec4 a_front = transform_front * a_local;
    Vec4 b_front = transform_front * b_local;
    Vec4 c_front = transform_front * c_local;

    triangle_wireframe(
        static_cast<int>(a_front.x),
        static_cast<int>(a_front.y),
        static_cast<int>(b_front.x),
        static_cast<int>(b_front.y),
        static_cast<int>(c_front.x),
        static_cast<int>(c_front.y),
        image,
        red
    ); //采用线框画三角形,更能看到透视效果同时更能说明现在发生在平面维度上的修改

    Vec4 a_oblique = transform_oblique * a_local;
    Vec4 b_oblique = transform_oblique * b_local;
    Vec4 c_oblique = transform_oblique * c_local;

    triangle_wireframe(
        static_cast<int>(a_oblique.x),
        static_cast<int>(a_oblique.y),
        static_cast<int>(b_oblique.x),
        static_cast<int>(b_oblique.y),
        static_cast<int>(c_oblique.x),
        static_cast<int>(c_oblique.y),
        image,
        blue
    );


    image.write("look_at.tga");
}