#include "rasterizer.h"
#include <cmath>
#include <algorithm>
#include <vector>

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

//重心坐标填充
void triangle_barycentric_filled(const Vec2& a,const Vec2& b,const Vec2& c,TGAImage& image,const TGAColor& color){
    //求包围盒
    int min_x = static_cast<int>(std::floor(std::min({a.x, b.x, c.x})));
    int max_x = static_cast<int>(std::ceil (std::max({a.x, b.x, c.x})));
    int min_y = static_cast<int>(std::floor(std::min({a.y, b.y, c.y})));
    int max_y = static_cast<int>(std::ceil (std::max({a.y, b.y, c.y})));
    //防止越界
    min_x = std::max(min_x, 0);
    min_y = std::max(min_y, 0);
    max_x = std::min(max_x, image.width() - 1);
    max_y = std::min(max_y, image.height() - 1);
    //遍历包围盒
    for (int y = min_y; y <= max_y;++y){
        for (int x = min_x; x <= max_x;++x){
            Vec2 p{static_cast<double>(x),static_cast<double>(y)}; //当前点
            Vec3 bc = barycentric(a,b,c,p);
            if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;
            image.set(x,y,color);
        }
    }
}



//定义辅助函数,实现TGAColor的插值
inline std::uint8_t to_byte(double v){
    v = std::max(0.0,std::min(255.0,v)); //控制在rgb范围内
    return static_cast<std::uint8_t>(v + 0.5);
}

//实现颜色插值
void triangle_barycentric_gradient(const Vec2& a,const Vec2& b,const Vec2& c,const TGAColor& ca,const TGAColor& cb,const TGAColor& cc,TGAImage& image){
    //求包围盒
    int min_x = static_cast<int>(std::floor(std::min({a.x, b.x, c.x})));
    int max_x = static_cast<int>(std::ceil (std::max({a.x, b.x, c.x})));
    int min_y = static_cast<int>(std::floor(std::min({a.y, b.y, c.y})));
    int max_y = static_cast<int>(std::ceil (std::max({a.y, b.y, c.y})));
    //防止越界
    min_x = std::max(min_x, 0);
    min_y =std::max(min_y, 0);
    max_x = std::min(max_x, image.width() - 1);
    max_y = std::min(max_y, image.height() - 1);
    //遍历包围盒
    for (int y = min_y; y <= max_y;++y){
        for (int x = min_x; x <= max_x;++x){
            Vec2 p{static_cast<double>(x),static_cast<double>(y)}; //当前点
            Vec3 bc = barycentric(a,b,c,p);
            if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;
            TGAColor mixed;
            mixed.r = to_byte(ca.r * bc.x + cb.r * bc.y + cc.r * bc.z);
            mixed.g = to_byte(ca.g * bc.x + cb.g * bc.y + cc.g * bc.z);
            mixed.b = to_byte(ca.b * bc.x + cb.b * bc.y + cc.b * bc.z);
            image.set(x,y,mixed);
        }
    }

}



//深度插值-zbuffer
void triangle_barycentric_depth(const Vec2& a,const Vec2& b,const Vec2& c,double za,double zb,double zc,TGAImage& image,std::vector<double>& zbuffer,const TGAColor& color){
    //求包围盒
    int min_x = static_cast<int>(std::floor(std::min({a.x, b.x, c.x})));
    int max_x = static_cast<int>(std::ceil (std::max({a.x, b.x, c.x})));
    int min_y = static_cast<int>(std::floor(std::min({a.y, b.y, c.y})));
    int max_y = static_cast<int>(std::ceil (std::max({a.y, b.y, c.y})));
    //防止越界
    min_x = std::max(min_x, 0);
    min_y =std::max(min_y, 0);
    max_x = std::min(max_x, image.width() - 1);
    max_y = std::min(max_y, image.height() - 1);
    //遍历包围盒
    for (int y = min_y; y <= max_y;++y){
        for (int x = min_x; x <= max_x;++x){
            Vec2 p{static_cast<double>(x),static_cast<double>(y)}; //当前点
            Vec3 bc = barycentric(a,b,c,p);
            if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;
            //深度插值
            double z = za * bc.x + zb * bc.y + zc * bc.z;
            int index = x + y * image.width();
            if (z <= zbuffer[index]) continue;
            zbuffer[index] = z;
            image.set(x, y, color);
        }
    }
}
















