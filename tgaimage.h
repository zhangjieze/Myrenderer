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
    TGAImage(int width,int height);
    int width() const;
    int height() const; //只读 

    bool set(int x,int y , const TGAColor& color);
    bool write(const std::string& filename) const; // Write the image to a TGA file

private:
    int width_ = 0;
    int height_ = 0;
    std::vector<std::uint8_t> pixels_; // 存储像素数据
};

void line(int x0, int y0, int x1, int y1,TGAImage& image,const TGAColor& color);