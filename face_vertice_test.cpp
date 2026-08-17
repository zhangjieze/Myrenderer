#include "model.h"
#include <iostream>

int main(){
    Model model("/Users/mac/Desktop/Myrenderer/obj/african_head.obj");
    const auto& vertices = model.vertices();
    const auto& faces = model.faces();
    std::cout << "v cnt : " << vertices.size() << std::endl;
    std::cout << "f cnt : " << faces.size() << std::endl;

    for (int i = 0 ; i < 5 ;++i){
        const Vec3& v = vertices[i];
        std::cout << "v" << i << ": "
                  << v.x << ", "
                  << v.y << ", "
                  << v.z << '\n';
    }

    for (int i = 0 ; i < 5 ;++i){
        Model::Face f = faces[i];
        std::cout << "f" << i << ": " << f[0] << ", " << f[1] << ", " << f[2] << "\n";
    }
    return 0;
}