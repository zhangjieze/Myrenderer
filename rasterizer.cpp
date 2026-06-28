#include "rasterizer.h"
#include <cmath>
#include <algorithm>

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



//三角形填充
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
        for (int y = y1; y <= y2;++y){
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


