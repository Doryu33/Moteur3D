#ifndef __OUR_GL_H__
#define __OUR_GL_H__
#include "tgaimage.h"
#include "geometry.h"

struct IShader
{
    virtual void vertex(Vertex &v, int idx) = 0;
    virtual bool fragment(TGAColor &normal, TGAColor &color, Vecteur3f &bar) = 0;
};

void triangleRasterizer(Vecteur4f *points, IShader &shader, TGAImage &image, float *zbuffer);

#endif //__OUR_GL_H__