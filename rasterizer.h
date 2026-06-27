#pragma once
#include "tgaimage.h"

void line_dda(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);

void line_bresenham(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);

void triangle_wireframe(int x0,int y0,int x1,int y1,int x2,int y2,TGAImage& image,const TGAColor& color);