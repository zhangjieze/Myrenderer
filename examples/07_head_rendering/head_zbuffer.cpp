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

    double inv_w(const Vec3& local_p,const Mat4& clip_transform){
        const Vec4 local{local_p.x,local_p.y,local_p.z,1.0};
        const Vec4 clip = clip_transform * local;
        return 1.0 / clip.w;
    }

    Vec3 facenormal(const Mat4& modelview,const Vec3& alocal,const Vec3& blocal,const Vec3& clocal){
        const Vec4 aview = modelview * Vec4{alocal.x,alocal.y,alocal.z,1.0};
        const Vec4 bview = modelview * Vec4{blocal.x,blocal.y,blocal.z,1.0};
        const Vec4 cview = modelview * Vec4{clocal.x,clocal.y,clocal.z,1.0};

        return normalized(cross(xyz(bview)-xyz(aview), xyz(cview)-xyz(aview)));
    }

    Vec3 normal_to_view(const Mat4& normalmatrix,const Vec3& normal_local){
        const Vec4 trans = normalmatrix * Vec4{normal_local.x, normal_local.y, normal_local.z, 0.0};
        return normalized(xyz(trans));
    }


}

int main(){
    TGAImage image(800,800);
    //const TGAColor white{255, 255, 255}; 已用flat shading取代
    TGAImage source;
    if (!source.read("/Users/mac/Desktop/Myrenderer/obj/african_head_diffuse.tga")) return 1;

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
    //构造normal_to_view矩阵
    Mat4 linear = identity();
    for (int i = 0 ; i < 3 ;++i){ //只换左上角非平移部分
        for (int j = 0 ; j < 3 ;++j){
            linear.m[i][j] = modelview.m[i][j];
        }
    }
    Mat4 linear_inverse;
    inverse(linear,linear_inverse);
    const Mat4 normal_matrix = transpose(linear_inverse);

    const Vec3 light_direction_view = normalized(Vec3{0.0, 0.0, 1.0});

    const double aspect = static_cast<double>(image.width()) / image.height();
    Mat4 projection = perspective(60.0 * std::numbers::pi / 180.0,aspect,0.1,100.0);

    const Mat4 clip_transform = projection *  modelview;
    const Mat4 vp = viewport(image.width(), image.height());

    for (const Model::Face& face : object.faces()){
        //local
        const Vec3 a_local = object.vertices()[face[0].position_index];
        const Vec3 b_local = object.vertices()[face[1].position_index];
        const Vec3 c_local = object.vertices()[face[2].position_index];


        const Vec2 a = to_screen(a_local, clip_transform, vp);
        const Vec2 b = to_screen(b_local, clip_transform, vp);
        const Vec2 c = to_screen(c_local, clip_transform, vp);


        double a_ndc = ndc(a_local, clip_transform);
        double b_ndc = ndc(b_local, clip_transform);
        double c_ndc = ndc(c_local, clip_transform);
        //uv
        const Vec2 a_uv = object.texcoords()[face[0].texcoord_index];
        const Vec2 b_uv = object.texcoords()[face[1].texcoord_index];
        const Vec2 c_uv = object.texcoords()[face[2].texcoord_index];


        double inv_wa = inv_w(a_local,clip_transform);
        double inv_wb = inv_w(b_local,clip_transform);
        double inv_wc = inv_w(c_local,clip_transform);

        //normal
        const Vec3 a_normal = object.normal()[face[0].normal_index];
        const Vec3 b_normal = object.normal()[face[1].normal_index];
        const Vec3 c_normal = object.normal()[face[2].normal_index];
        const Vec3 a_normal_view = normal_to_view(normal_matrix,a_normal);
        const Vec3 b_normal_view = normal_to_view(normal_matrix,b_normal);
        const Vec3 c_normal_view = normal_to_view(normal_matrix,c_normal);

        triangle_barycentric_uv_normal_depth(a,b,c,a_ndc,b_ndc,c_ndc,inv_wa,inv_wb,inv_wc,a_uv,b_uv,c_uv,a_normal_view,b_normal_view,c_normal_view,light_direction_view,image,source,zbuffer);

        
    }

    image.write("head_uv_normal.tga");

}




