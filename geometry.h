#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__
#include <cmath>
#include <vector>
#include <ostream>
#include <cassert>

const int DEFAULT_ALLOC=4;

/// @brief Structure pour représenter un vertex
struct Vertex {
  double x;
  double y;
  double z;
};

/// @brief Structure pour représenter un vertex de texture
struct VertexTexture {
  double x;
  double y;
  double z;
};

struct Vecteur3f{
  double x;
  double y;
  double z;
};

class Matrix {
    std::vector<std::vector<float> > m;
    int rows, cols;
public:
    Matrix(int r=DEFAULT_ALLOC, int c=DEFAULT_ALLOC);
    Matrix(Vertex * v);    
    inline int nrows();
    inline int ncols();
    static Matrix identity(int dimensions);
    std::vector<float>& operator[](const int i);
    Matrix operator*(const Matrix& a);
    Matrix transpose();
    Matrix inverse();
    friend std::ostream& operator<<(std::ostream& s, Matrix& m);
};

Matrix viewport(int x, int y, int w, int h, int depth);
Matrix lookat(Vecteur3f eye, Vecteur3f center, Vecteur3f up);
Matrix projection(Vecteur3f eye, Vecteur3f center);

Vertex matToVect(Matrix m);

Vecteur3f normalize(Vecteur3f vecteur);
Vecteur3f crossProduct(Vecteur3f u, Vecteur3f v);

#endif //__GEOMETRY_H__