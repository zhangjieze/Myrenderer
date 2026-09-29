#include "model.h"
#include <iostream>

int main(){
    Model model("/Users/mac/Desktop/Myrenderer/obj/african_head.obj");
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


