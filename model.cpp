#include "model.h"
#include <cassert>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


namespace{
    FaceVertex parse_face_vertex(const std::string& reference){
        FaceVertex result;
        const std::size_t first_slash = reference.find('/');
        if (first_slash == std::string::npos){ //如果只有position索引
            result.position_index = std::stoi(reference) - 1;
            return result;
        }
        
        result.position_index = std::stoi(reference.substr(0, first_slash)) - 1;
        
        const std::size_t second_slash = reference.find('/',first_slash + 1);
        const std::size_t uv_end = (second_slash == std::string::npos ? reference.size() : second_slash); //找边界
        
        const std::string uv_text = reference.substr(first_slash + 1, uv_end - first_slash - 1);

        if (!uv_text.empty()){ //处理空uv索引
            result.texcoord_index = std::stoi(uv_text) -1;
        }
        return result;
    }
}

Model::Model(const std::string& filename){//:: 作用域解析运算符
    std::ifstream input(filename);
    assert(input && "cannot open obj file,no!!!");

    std::string line;
    while (std::getline(input, line)){ //每次读取文件的一行
        std::istringstream stream(line); //把这一行当作一个小型输入流,再用 >> 按空格分割读取
        std::string kind;
        stream >> kind;
        if (kind == "v") {
            double x = 0.0;
            double y = 0.0;
            double z = 0.0;
            stream >> x >> y >> z;

            vertices_.push_back({x, y, z});
        }
        else if(kind == "f"){
            std::string a;
            std::string b;
            std::string c;
            stream >> a >> b >> c;
            faces_.push_back({parse_face_vertex(a),parse_face_vertex(b),parse_face_vertex(c)});
        }
        else if(kind == "vt"){
            double u = 0.0;
            double v = 0.0;
            stream >> u >> v;
            texcoords_.push_back({u,v});
        }
    }

}



const std::vector<Vec3>& Model::vertices() const{
    return vertices_;
}

const std::vector<Model::Face>& Model::faces() const{
    return faces_;
}

const std::vector<Vec2>& Model::texcoords() const{
    return texcoords_;
}