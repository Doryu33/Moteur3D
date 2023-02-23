#include <cmath>
#include <limits>
#include <cstdlib>
#include "our_gl.h"

/*
void triangleRasterizer(Vecteur4f *points, IShader &shader, TGAImage &image, float *zbuffer){
    //#On créer les boites pour vérifier le minimum et le maximum
    Vecteur2f bboxmin{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
    Vecteur2f bboxmax{-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()};

    for (int i = 0; i<3; i++)
    {
        bboxmin.x = std::min(bboxmin.x, points[i].x/points[i].v);
        bboxmin.y = std::min(bboxmin.y, points[i].y/points[i].v);
        bboxmax.x = std::max(bboxmax.x, points[i].x/points[i].v);
        bboxmax.y = std::max(bboxmax.y, points[i].y/points[i].v);
    }

    Vertex P;
    Vertex barycentrique;
    TGAColor color;
    for(P.x=bboxmin.x; P.x<=bboxmax.x; P.x++){
        for(P.y=bboxmin.y; P.y<=bboxmax.y; P.y++){
            if(is_inside_triangle(points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y, P.x, P.y, barycentrique)){
                float z = points[0].z * barycentrique.x + points[1].z * barycentrique.y + points[2].z * barycentrique.z;
                float w = points[0].v * barycentrique.x + points[1].v * barycentrique.y + points[2].v * barycentrique.z;
                int frag_dept = z/w;

                Vecteur3f c = {barycentrique.x, barycentrique.y, barycentrique.z};
                int px = P.x;
                int py = P.y;
                if(c.x<0 || c.y<0 || c.z || zbuffer[px+ py * image.get_width()]>frag_dept) continue;
                bool discard = shader.fragment(c, color);
                if(!discard) {
                    zbuffer[px+ py * image.get_width()] = frag_dept;
                    image.set(P.x,P.y, color);
                }
            }
        }
    }
}
    */