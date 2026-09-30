#include "myrenderer/tgaimage.h"

int main(){
    TGAImage source;
    source.read("assets/textures/african_head_diffuse.tga");

    TGAImage copy(source.width(),source.height());

    for (int x = 0 ; x < source.width();++x){
        for (int y = 0 ; y < source.height();++y){
            TGAColor color;
            source.get(x, y,color);
            copy.set(x,y,color);
        }
    }

    return copy.write("examples/01_image_io/output/african_head_diffuse_copy.tga") ? 0 : 1;

}