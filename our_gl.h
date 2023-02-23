#ifndef __OUR_GL_H__
#define __OUR_GL_H__
#include "tgaimage.h"
#include "geometry.h"

struct IShader
{
    virtual ~IShader();
    virtual Vecteur4f vertex(int iface, int nthvert) = 0;
    virtual bool fragment(Vecteur3f bar, TGAColor &color) = 0;
};

void triangleRasterizer(Vecteur4f *points, IShader &shader, TGAImage &image, float *zbuffer);

#endif //__OUR_GL_H__