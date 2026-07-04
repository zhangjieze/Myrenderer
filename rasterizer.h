#pragma once
#include "tgaimage.h"
#include "geometry.h"

void line_dda(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);

void line_bresenham(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);

void triangle_wireframe(int x0,int y0,int x1,int y1,int x2,int y2,TGAImage& image,const TGAColor& color);

void triangle_filled(int x0,int y0,int x1,int y1,int x2,int y2,TGAImage& image,const TGAColor& color);

Vec3 barycentric(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& p);
void triangle_barycentric_filled(const Vec2& a,const Vec2& b,const Vec2& c,TGAImage& image,const TGAColor& color);
