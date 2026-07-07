#pragma once
#include "tgaimage.h"
#include "geometry.h"
#include <vector>

void line_dda(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);

void line_bresenham(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);

void triangle_wireframe(int x0,int y0,int x1,int y1,int x2,int y2,TGAImage& image,const TGAColor& color);

void triangle_filled(int x0,int y0,int x1,int y1,int x2,int y2,TGAImage& image,const TGAColor& color);

Vec3 barycentric(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& p);
void triangle_barycentric_filled(const Vec2& a,const Vec2& b,const Vec2& c,TGAImage& image,const TGAColor& color);

void triangle_barycentric_gradient(const Vec2& a,const Vec2& b,const Vec2& c,const TGAColor& ca,const TGAColor& cb,const TGAColor& cc,TGAImage& image);

void triangle_barycentric_depth(const Vec2& a,const Vec2& b,const Vec2& c,double za,double zb,double zc,TGAImage& image,std::vector<double>& zbuffer,const TGAColor& color);