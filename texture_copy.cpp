#include "tgaimage.h"

int main(){
    TGAImage source;
    source.read("/Users/mac/Desktop/Myrenderer/obj/african_head_diffuse.tga");

    TGAImage copy(source.width(),source.height());

    for (int x = 0 ; x < source.width();++x){
        for (int y = 0 ; y < source.height();++y){
            TGAColor color;
            source.get(x, y,color);
            copy.set(x,y,color);
        }
    }

    return copy.write("african_head_diffuse_copy.tga") ? 0 : 1;

}