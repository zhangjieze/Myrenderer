/*
此时 OBJ 的 Face (array<int,3>)只保存顶点位置索引,因此可以直接使用 face[i] 访问 vertices.
后续为了支持纹理坐标 vt 和顶点法线 vn，引入了 FaceVertex,
当前 Face 已同时保存 position/texcoord/normal 三类索引，接口因此发生变化.
*/



#include "myrenderer/model.h"
#include <iostream>

int main(){
    Model model("assets/models/african_head.obj");
    const auto& vertices = model.vertices();
    const auto& faces = model.faces();
    std::cout << "v cnt : " << vertices.size() << std::endl;
    std::cout << "f cnt : " << faces.size() << std::endl;

    //测试是否已经把顶点读入vertices_
    for (int i = 0 ; i < 5 ;++i){
        const Vec3& v = vertices[i];
        std::cout << "v" << i << ": "
                  << v.x << ", "
                  << v.y << ", "
                  << v.z << '\n';
    }
    //测试是否已经把面的索引读入faces_
    for (int i = 0 ; i < 5 ;++i){
        const Model::Face& f = faces[i];
        std::cout << "f" << i << ": " << f[0] << ", " << f[1] << ", " << f[2] << "\n";
    }
    

    //验证是否能取出这个顶点
    const Model::Face& face = faces[0];
    const Vec3& a = vertices[face[0]];
    const Vec3& b = vertices[face[1]];
    const Vec3& c = vertices[face[2]];

    std::cout << "first triangle:\n";
    std::cout << "a: " << a.x << ", " << a.y << ", " << a.z << '\n';
    std::cout << "b: " << b.x << ", " << b.y << ", " << b.z << '\n';
    std::cout << "c: " << c.x << ", " << c.y << ", " << c.z << '\n';


    return 0;
}


