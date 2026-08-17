#pragma once

#include <array>
#include <string>
#include <vector>
#include "geometry.h"

class Model {
public:
    using Face = std::array<int,3>;
    explicit Model(const std::string& filename); //声明构造函数,禁止隐式转换

    const std::vector<Vec3>& vertices() const; //返回引用
    const std::vector<Face>& faces() const; //face保存的是这个面的顶点的索引,必须是int,使用array保存

private:
    std::vector<Vec3> vertices_;
    std::vector<Face> faces_;
};