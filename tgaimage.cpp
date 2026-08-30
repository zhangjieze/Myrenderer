#include "tgaimage.h"
#include <cstdint>
#include <fstream>
#include <array>

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

bool TGAImage::get(int x,int y,TGAColor &color) const{
    if (x < 0 || x >= width_ || y < 0 || y >= height_){
        return false;
    }
    std::size_t index = (static_cast<std::size_t>(y) * width_ + x) * 3;
    color.b = pixels_[index];
    color.g = pixels_[index+1];
    color.r = pixels_[index+2];
    return true;
}

bool TGAImage::read(const std::string& filename){
    std::ifstream input(filename,std::ios::binary); //以2进制形式打开文件,而非文本
    if (!input) return false;
    std::array<std::uint8_t, 18> header{}; //准备 18 字节保存 TGA Header
    input.read(reinterpret_cast<char*>(header.data()),static_cast<std::streamsize>(header.size()));
    if (!input) return false;

    const std::uint8_t id_length = header[0];
    const std::uint8_t color_map_type = header[1];
    const std::uint8_t image_type = header[2];

    const int width = header[12] | (header[13] << 8);
    const int height = header[14] | (header[15] << 8);
    const std::uint8_t bits_per_pixel = header[16];

    if (color_map_type != 0 || image_type != 2 || bits_per_pixel != 24 || width <= 0 || height <= 0 || header[17] != 0) {return false;}
    
    input.ignore(static_cast<std::streamsize>(id_length));
    if(!input) return false;

    const std::size_t byte_cnt = static_cast<std::size_t>(width) * height * 3;
    std::vector<std::uint8_t> pixels(byte_cnt);
    input.read(reinterpret_cast<char*>(pixels.data()),static_cast<std::streamsize>(pixels.size()));
    if (!input) return false;

    width_ = width;
    height_ = height;
    pixels_.swap(pixels);
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

