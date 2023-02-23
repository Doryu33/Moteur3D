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

struct Vecteur2f{
  double x;
  double y;
};

struct Vecteur3f{
  double x;
  double y;
  double z;
};

struct Vecteur4f
{
  double x;
  double y;
  double z;
  double v;
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
Matrix projection(double coeff);

Vertex matToVect(Matrix m);

Vecteur3f normalize(Vecteur3f vecteur);
Vecteur3f crossProduct(Vecteur3f u, Vecteur3f v);
double area_of_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
bool is_inside_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int px, int py, Vertex &bary);
#endif //__GEOMETRY_H__