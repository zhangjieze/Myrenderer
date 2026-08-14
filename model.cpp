#include "model.h"
#include <cassert>
#include <fstream>
#include <sstream>
#include <vector>

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
    }

}



const std::vector<Vec3>& Model::vertices() const{
    return vertices_;
}

const std::vector<Model::Face>& Model::faces() const{
    return faces_;
}