#include "rasterizer.h"
#include "model.h"
#include "geometry.h"
#include "tgaimage.h"

#include <vector>
#include <limits>
#include <numbers>
#include <algorithm>

namespace{
    Vec2 to_screen(const Vec3& local_p,const Mat4& clip_transform,const Mat4& vp){
        const Vec4 local{local_p.x,local_p.y,local_p.z,1.0};
        const Vec4 clip = clip_transform * local;
        const Vec3 ndc = perspective_divide(clip);
        const Vec4 screen = vp * Vec4{ndc.x, ndc.y, ndc.z, 1.0};
        return xy(screen);
    }

    double ndc(const Vec3& local_p,const Mat4& clip_transform){
        const Vec4 local{local_p.x,local_p.y,local_p.z,1.0};
        const Vec4 clip = clip_transform * local;
        const Vec3 ndc = perspective_divide(clip);
        return ndc.z;
    }

    Vec3 facenormal(const Mat4& modelview,const Vec3& alocal,const Vec3& blocal,const Vec3& clocal){
        const Vec4 aview = modelview * Vec4{alocal.x,alocal.y,alocal.z,1.0};
        const Vec4 bview = modelview * Vec4{blocal.x,blocal.y,blocal.z,1.0};
        const Vec4 cview = modelview * Vec4{clocal.x,clocal.y,clocal.z,1.0};

        return normalized(cross(xyz(bview)-xyz(aview), xyz(cview)-xyz(aview)));
    }
}

int main(){
    TGAImage image(800,800);
    //const TGAColor white{255, 255, 255}; 已用flat shading取代

    Model object("/Users/mac/Desktop/Myrenderer/obj/african_head.obj");

    std::vector<double> zbuffer(image.width() * image.height(),std::numeric_limits<double>::infinity());

    Mat4 model = identity();
    const Mat4 view = look_at(
        {0.0, 0.0, 3.0},
        {0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0}
    );
    //引入flat shading
    const Mat4 modelview = view * model;
    const Vec3 light_direction_view = normalized(Vec3{0.0, 0.0, 1.0});

    const double aspect = static_cast<double>(image.width()) / image.height();
    Mat4 projection = perspective(60.0 * std::numbers::pi / 180.0,aspect,0.1,100.0);

    const Mat4 clip_transform = projection *  modelview;
    const Mat4 vp = viewport(image.width(), image.height());

    for (const Model::Face& face : object.faces()){
        const Vec2 a = to_screen(object.vertices()[face[0]], clip_transform, vp);
        const Vec2 b = to_screen(object.vertices()[face[1]], clip_transform, vp);
        const Vec2 c = to_screen(object.vertices()[face[2]], clip_transform, vp);

        const Vec3 facenormalvec = facenormal(modelview, object.vertices()[face[0]], object.vertices()[face[1]], object.vertices()[face[2]]);


        double a_ndc = ndc(object.vertices()[face[0]], clip_transform);
        double b_ndc = ndc(object.vertices()[face[1]], clip_transform);
        double c_ndc = ndc(object.vertices()[face[2]], clip_transform);


        const double intensity = std::clamp(dot(facenormalvec, light_direction_view),0.0,1.0);
        const auto shade = static_cast<std::uint8_t>(255.0 * intensity);
        const TGAColor face_color{shade, shade, shade};

        triangle_barycentric_depth(
            a,
            b,
            c,
            a_ndc,
            b_ndc,
            c_ndc,
            image,
            zbuffer,
            face_color
        );
        
    }

    image.write("head_zbuffer_flat_shading.tga");

}