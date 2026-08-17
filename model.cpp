#include "model.h"
#include <cassert>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


namespace{
    int position_index(const std::string& reference){ 
        const std::size_t slash = reference.find('/'); //找到第一个/的位置
        return std::stoi(reference.substr(0,slash)) - 1; //obj从1开始编号,vector从0开始编号,所以这里需要减一
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
            faces_.push_back({position_index(a),position_index(b),position_index(c)});
        }
    }

}



const std::vector<Vec3>& Model::vertices() const{
    return vertices_;
}

const std::vector<Model::Face>& Model::faces() const{
    return faces_;
}