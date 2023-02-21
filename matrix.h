#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <cmath>
#include <vector>
#include <ostream>
#include <cassert>

const int DEFAULT_ALLOC=4;
struct Vertex;
struct Vecteur3f;

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

#endif //__MATRIX_H__