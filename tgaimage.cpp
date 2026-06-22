#include "tgaimage.h"
#include <cstdint>
#include <fstream>
#include <array>
#include <cmath>
#include <algorithm>

TGAImage::TGAImage(int width,int height)
    : width_(width),height_(height),pixels_(width * height*3,0){}

int TGAImage::width() const {
    return width_;
}

int TGAImage::height() const{
    return height_;
}

bool TGAImage::set(int x, int y, const TGAColor &color){
    if (x < 0 || x >= width_ || y < 0 || y >= height_){
        return false;
    }
    std::size_t index = (static_cast<std::size_t>(y) * width_ + x) * 3; //通过坐标寻找像素存储的位置
    pixels_[index] = color.b;
    pixels_[index+1] = color.g;
    pixels_[index+2] = color.r;
    return true;
}

bool TGAImage::write(const std::string &filename) const {
    if (width_ <= 0 || height_ <= 0|| width_ > 65535 || height_ > 65535){
        return false;
    }
    std::array<std::uint8_t,18> header{};
    //header初始化,设置TGA文件头具体细节见note
    header[2] = 2;
    
    header[12] = width_ & 0xff;
    header[13] = (width_ >> 8) & 0xff;

    header[14] = height_ & 0xff;
    header[15] = (height_ >> 8) & 0xff;

    header[16] = 24;
    header[17] = 0;

    //文件流
    std::ofstream output(filename,std::ios::binary);
    if(!output){
        return false;
    }

    output.write(reinterpret_cast<const char*>(header.data()),static_cast<std::streamsize>(header.size()));
    output.write(reinterpret_cast<const char*>(pixels_.data()),static_cast<std::streamsize>(pixels_.size()));

    return output.good();
}

//dda
void line(int x0, int y0, int x1, int y1,TGAImage& image, const TGAColor& color){
    int dx = x1 - x0;
    int dy = y1 - y0;
    int steps = std::max(std::abs(dx), std::abs(dy));
    if (steps == 0){
        image.set(x0, y0, color);
        return;
    }
    for (int i = 0 ; i <= steps ;++i){
        double t = static_cast<double>(i) / steps;
        int x = x0 + static_cast<int>(std::round(dx * t));
        int y = y0 + static_cast<int>(std::round(dy * t));
        image.set(x, y, color);
    }
}
