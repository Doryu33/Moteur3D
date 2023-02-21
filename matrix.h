#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <cmath>
#include <vector>
#include <ostream>
#include <cassert>

const int DEFAULT_ALLOC=4;
struct Vertex;

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

#endif //__MATRIX_H__