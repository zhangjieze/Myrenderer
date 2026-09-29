#include <numbers>

#include "geometry.h"
#include "model.h"
#include "tgaimage.h"
#include "rasterizer.h"

namespace{
    Vec2 to_screen(const Vec3& local_p,const Mat4& clip_transform,const Mat4& vp){
        const Vec4 local{local_p.x,local_p.y,local_p.z,1.0};
        const Vec4 clip = clip_transform * local;
        const Vec3 ndc = perspective_divide(clip);
        const Vec4 screen = vp * Vec4{ndc.x, ndc.y, ndc.z, 1.0};
        return xy(screen);
    }
}

int main(){
    TGAImage image(800,800);
    TGAColor white{255,255,255};
    Model object("/Users/mac/Desktop/Myrenderer/obj/african_head.obj");


    Mat4 model = identity();
    const Mat4 view = look_at(
        {0.0, 0.0, 3.0},
        {0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0}
    );
    const double aspect = static_cast<double>(image.width()) / image.height();
    Mat4 projection = perspective(60.0 * std::numbers::pi / 180.0,aspect,0.1,100.0);

    const Mat4 clip_transform = projection * view * model;
    const Mat4 vp = viewport(image.width(), image.height());

    for (const Model::Face& face : object.faces()){
        const Vec2 a = to_screen(object.vertices()[face[0]], clip_transform, vp);
        const Vec2 b = to_screen(object.vertices()[face[1]], clip_transform, vp);
        const Vec2 c = to_screen(object.vertices()[face[2]], clip_transform, vp);
        triangle_wireframe(
            static_cast<int>(a.x), static_cast<int>(a.y),
            static_cast<int>(b.x), static_cast<int>(b.y),
            static_cast<int>(c.x), static_cast<int>(c.y),
            image,
            white
        );
    }
    image.write("obj_wireframe.tga");

    


}