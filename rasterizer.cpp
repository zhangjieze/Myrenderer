#include "rasterizer.h"
#include <cmath>
#include <algorithm>
#include <vector>


namespace {
//包围盒抽象 
struct BoundingBox{
    int min_x = 0;
    int max_x = 0;
    int min_y = 0;
    int max_y = 0;
};

//包围盒实现抽象
BoundingBox triangle_bounding_box(const Vec2& a,const Vec2& b,const Vec2& c,const TGAImage& image){
    BoundingBox box;
    //求包围盒
    box.min_x = static_cast<int>(std::floor(std::min({a.x, b.x, c.x})));
    box.max_x = static_cast<int>(std::ceil (std::max({a.x, b.x, c.x})));
    box.min_y = static_cast<int>(std::floor(std::min({a.y, b.y, c.y})));
    box.max_y = static_cast<int>(std::ceil (std::max({a.y, b.y, c.y})));
    //防止越界
    box.min_x = std::max(box.min_x, 0);
    box.min_y =std::max(box.min_y, 0);
    box.max_x = std::min(box.max_x, image.width() - 1);
    box.max_y = std::min(box.max_y, image.height() - 1);
    return box;
}


//插值颜色的辅助函数
inline std::uint8_t to_byte(double v) {
    v = std::max(0.0, std::min(255.0, v));
    return static_cast<std::uint8_t>(v + 0.5); //由于使用的是double类型,所以需要进行四舍五入,而static_cast是直接截断,所以在v >0时+0.5实现四舍五入
}

//颜色插值抽象
TGAColor interpolate_color(const TGAColor& ca,const TGAColor& cb,const TGAColor& cc,const Vec3& bc){
    TGAColor mixed;
    mixed.r = to_byte(ca.r * bc.x + cb.r * bc.y + cc.r * bc.z);
    mixed.g = to_byte(ca.g * bc.x + cb.g * bc.y + cc.g * bc.z);
    mixed.b = to_byte(ca.b * bc.x + cb.b * bc.y + cc.b * bc.z);
    return mixed;
}

//uv纹理插值抽象
Vec2 interpolate_uv(const Vec2& uv_a,const Vec2& uv_b,const Vec2& uv_c,const Vec3& bc){
    Vec2 realuv;
    realuv.x = bc.x * uv_a.x + bc.y * uv_b.x + bc.z * uv_c.x;
    realuv.y = bc.x * uv_a.y + bc.y * uv_b.y + bc.z * uv_c.y;
    return realuv;
}

//屏幕重心坐标变换回原始三维的重心坐标辅助函数
Vec3 perspective_correct_barycentric(const Vec3& screen_bc,double inv_wa,double inv_wb,double inv_wc){
    const double wa = screen_bc.x * inv_wa;
    const double wb = screen_bc.y * inv_wb;
    const double wc = screen_bc.z * inv_wc;

    const double sum = wa + wb + wc;

    return Vec3{wa / sum,wb / sum,wc / sum};
}

}





//DDA算法实现直线绘制
void line_dda(int x0, int y0, int x1, int y1,TGAImage& image, const TGAColor& color){
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

//bresenham
void line_bresenham(int x0, int y0, int x1, int y1, TGAImage &image, const TGAColor &color){
    //处理斜率过大,也就是dx < dy,此时取点应该由y轴取,交换坐标轴
    bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);
    if (steep){
        std::swap(x0,y0);
        std::swap(x1,y1);
    }

    //处理反向问题
    if (x0 > x1){
        std::swap(x0,x1);
        std::swap(y0,y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;


    //处理负斜率问题,负斜率的时候相当于y往下走,用一个ystep保存y需要走的间隔
    int ystep = 1;
    if (dy < 0){
        dy = -dy;
        ystep = -1;
    }


    int error = 0;
    int y = y0;
    for (int x = x0 ; x <= x1;++x){
        if (steep) image.set(y,x,color);
        else image.set(x,y,color);

        error += dy;
        if (2 * error >= dx){
            y += ystep;
            error -= dx;
        }
    }
}

//三角形轮廓
void triangle_wireframe(int x0,int y0,int x1,int y1,int x2,int y2,TGAImage& image,const TGAColor& color){
    line_bresenham(x0, y0, x1, y1, image, color);
    line_bresenham(x1, y1, x2, y2, image, color);
    line_bresenham(x2, y2, x0, y0, image, color);
}



//三角形填充,水平划线法
void triangle_filled(int x0,int y0,int x1,int y1,int x2,int y2,TGAImage& image,const TGAColor& color){
    //采用普通的填充法,保持y0 <= y1 <= y2
    if (y0 > y1) {
        std::swap(x0,x1);
        std::swap(y0,y1);
    }
    if (y0 > y2){
        std::swap(x0,x2);
        std::swap(y0,y2);
    }
    if (y1 > y2){
        std::swap(x1,x2);
        std::swap(y1,y2);
    }

    if (y0 == y2) return;

    if (y0 != y1){
        for (int y = y0; y <= y1;++y){
            double t1 = (y - y0) / static_cast<double>(y2 - y0);
            int xa = x0 + (x2 - x0) * t1;

            double t2 = (y - y0) / static_cast<double>(y1 - y0);
            int xb = x0 + (x1 - x0) * t2;

            //水平线不需要使用line函数,直接实现即可
            if (xa > xb) std::swap(xa,xb);
            for (int x = xa; x <= xb;++x){
                image.set(x,y,color);
            }
            
        }
    }

    if (y1 != y2){
        for (int y = y1 + 1; y <= y2;++y){ //从y1 + 1开始,y1只需要画一次
            double t1 = (y - y1) / static_cast<double>(y2 - y1);
            int xa = x1 + (x2 - x1) * t1;

            double t2 = (y - y0) / static_cast<double>(y2 - y0);
            int xb = x0 + (x2 - x0) * t2;

            if (xa > xb) std::swap(xa,xb);
            for (int x = xa; x <= xb;++x){
                image.set(x,y,color);
            }
        }
    }

}




//找重心坐标,克莱姆法则
Vec3 barycentric(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& p){
    double denom = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
    if (std::abs(denom) < 1e-8) return Vec3{-1,-1,-1};
    double alpha = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / denom;
    double beta = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) / denom;
    double gamma = 1 - alpha - beta;
    return Vec3{alpha,beta,gamma};
}


//重心坐标填充方式
void triangle_barycentric_filled(const Vec2& a,const Vec2& b,const Vec2& c,TGAImage& image,const TGAColor& color){
    BoundingBox box = triangle_bounding_box(a,b,c,image); //包围盒

    //遍历包围盒
    for (int y = box.min_y; y <= box.max_y;++y){
        for (int x = box.min_x; x <= box.max_x;++x){
            Vec2 p{static_cast<double>(x),static_cast<double>(y)}; //当前点
            Vec3 bc = barycentric(a,b,c,p);
            if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;
            image.set(x,y,color);
        }
    }
}



//实现颜色插值
void triangle_barycentric_gradient(const Vec2& a,const Vec2& b,const Vec2& c,const TGAColor& ca,const TGAColor& cb,const TGAColor& cc,TGAImage& image){
    BoundingBox box = triangle_bounding_box(a,b,c,image); //包围盒

    //遍历包围盒
    for (int y = box.min_y; y <= box.max_y;++y){
        for (int x = box.min_x; x <= box.max_x;++x){
            Vec2 p{static_cast<double>(x),static_cast<double>(y)}; //当前点
            Vec3 bc = barycentric(a,b,c,p);
            if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;
            TGAColor mixed = interpolate_color(ca, cb, cc, bc); //颜色插值
            image.set(x,y,mixed);
        }
    }

}



//深度插值-zbuffer
void triangle_barycentric_depth(const Vec2& a,const Vec2& b,const Vec2& c,double za,double zb,double zc,TGAImage& image,std::vector<double>& zbuffer,const TGAColor& color){
    BoundingBox box = triangle_bounding_box(a,b,c,image); //包围盒

    //遍历包围盒
    for (int y = box.min_y; y <= box.max_y;++y){
        for (int x = box.min_x; x <= box.max_x;++x){
            Vec2 p{static_cast<double>(x),static_cast<double>(y)}; //当前点
            Vec3 bc = barycentric(a,b,c,p);
            if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;
            //深度插值
            double z = za * bc.x + zb * bc.y + zc * bc.z;
            int index = x + y * image.width();
            if (z >= zbuffer[index]) continue;
            zbuffer[index] = z;
            image.set(x, y, color);
        }
    }
}


//结合深度和颜色插值
//screen_bc负责覆盖测试和 NDC z 插值, surface_bc负责颜色插值,之后也会用于 UV、法线等三维表面属性插值
void triangle_barycentric_gradient_depth(const Vec2 &a, const Vec2 &b, const Vec2 &c, double za,double zb,double zc,double inv_wa, double inv_wb, double inv_wc,const TGAColor &ca, const TGAColor &cb, const TGAColor &cc, TGAImage &image,std::vector<double>& zbuffer){
    BoundingBox box = triangle_bounding_box(a,b,c,image); //包围盒

    for (int y = box.min_y;y <= box.max_y;++y){
        for (int x = box.min_x;x <= box.max_x;++x){
            Vec2 p{static_cast<double>(x),static_cast<double>(y)};
            Vec3 screen_bc = barycentric(a,b,c,p);
            if (screen_bc.x < 0 || screen_bc.y < 0 || screen_bc.z < 0) continue;
            //先进行深度插值
            double z = za * screen_bc.x + zb * screen_bc.y + zc * screen_bc.z;
            int index = x + y * image.width();
            if (z >= zbuffer[index]) continue;
            zbuffer[index] = z;

            //屏幕重心 -> 三维坐标重心
            Vec3 surface_bc = perspective_correct_barycentric(screen_bc,inv_wa,inv_wb,inv_wc);
            TGAColor mixed = interpolate_color(ca, cb, cc, surface_bc);
            image.set(x,y,mixed);
        }
    }
}


//深度和uv插值
void triangle_barycentric_uv_depth(const Vec2& a,const Vec2& b,const Vec2& c,double za,double zb,double zc,double inv_wa, double inv_wb, double inv_wc,const Vec2& uv_a,const Vec2& uv_b,const Vec2& uv_c,TGAImage& image,const TGAImage& source,std::vector<double>& zbuffer){
    BoundingBox box = triangle_bounding_box(a,b,c,image); //包围盒

    //遍历包围盒
    for (int y = box.min_y; y <= box.max_y;++y){
        for (int x = box.min_x; x <= box.max_x;++x){
            Vec2 p{static_cast<double>(x),static_cast<double>(y)}; //当前点
            Vec3 bc = barycentric(a,b,c,p);
            if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;
            //深度插值
            double z = za * bc.x + zb * bc.y + zc * bc.z;
            int index = x + y * image.width();
            if (z >= zbuffer[index]) continue;
            zbuffer[index] = z;
            
            //屏幕重心 -> 三维坐标重心
            Vec3 surface_bc = perspective_correct_barycentric(bc,inv_wa,inv_wb,inv_wc);
            Vec2 uv = interpolate_uv(uv_a,uv_b,uv_c,surface_bc);
            TGAColor color;
            source.get(std::clamp(static_cast<int>(uv.x * source.width()),0,source.width() - 1),std::clamp(static_cast<int>(uv.y * source.height()),0,source.height() - 1),color);
            image.set(x,y,color);


        }
    }
}













