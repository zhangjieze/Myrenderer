#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct TGAColor{
    std::uint8_t b = 0;
    std::uint8_t g = 0;
    std::uint8_t r = 0;
};

class TGAImage{
public:
    TGAImage() = default; //创建空图
    TGAImage(int width,int height);
    
    int width() const;
    int height() const; //只读

    bool set(int x,int y , const TGAColor& color); //set: 调用者 -> 图像
    bool get(int x,int y , TGAColor& color) const; //读取这个位置像素的颜色写回color,get: 图像 -> 调用者
    bool read(const std::string& filename);
    bool write(const std::string& filename) const;

private:
    int width_ = 0;
    int height_ = 0;
    std::vector<std::uint8_t> pixels_; // 存储像素数据
};

