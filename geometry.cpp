#include "geometry.h"
#include "modele.h"
#include <iostream>


Vertex matToVect(Matrix m){
  Vertex res;
  float z = m[3][0];
  res.x = m[0][0]/z;
  res.y = m[1][0]/z;
  res.z = m[2][0]/z;
  return res;
}

Vecteur3f normalize(Vecteur3f vecteur){
    double n = sqrt(vecteur.x * vecteur.x + vecteur.y * vecteur.y + vecteur.z * vecteur.z);
    Vecteur3f v;
    v.x = vecteur.x;
    v.y = vecteur.y;
    v.z = vecteur.z;

    v.x *= (1./n);
    v.y *= (1./n);
    v.z *= (1./n);

    return v;
};

Vecteur3f crossProduct(Vecteur3f v, Vecteur3f u){
  Vecteur3f res;
  res.x = v.y * u.z - v.z * u.y;
  res.y = v.z * u.x - v.x * u.z;
  res.z = v.x * u.y - v.y * u.x;
  return res;
};

Matrix::Matrix(int r, int c) : m(std::vector<std::vector<float> >(r, std::vector<float>(c, 0.f))), rows(r), cols(c) { }


Matrix::Matrix(Vertex* v) : m(std::vector<std::vector<float> >(4, std::vector<float>(1, 1.f))), rows(4), cols(1) {
    m[0][0] = v->x;
    m[1][0] = v->y;
    m[2][0] = v->z;
}

int Matrix::nrows() {
    return rows;
}

int Matrix::ncols() {
    return cols;
}

Matrix Matrix::identity(int dimensions) {
    Matrix E(dimensions, dimensions);
    for (int i=0; i<dimensions; i++) {
        for (int j=0; j<dimensions; j++) {
            E[i][j] = (i==j ? 1.f : 0.f);
        }
    }
    return E;
}

/**
 * @brief Permet de calculer la matrice "ViewPort"
 * 
 * @return Matrix viewport
 */
Matrix viewport(int x, int y, int w, int h, int depth)
{
    Matrix m = Matrix::identity(4);
    m[0][3] = x + w / 2.f;
    m[1][3] = y + h / 2.f;
    m[2][3] = depth / 2.f;

    m[0][0] = w / 2.f;
    m[1][1] = h / 2.f;
    m[2][2] = depth / 2.f;
    return m;
}

/**
 * @brief Permet de calculer la matrice "ModeleView"
 * 
 * @return Matrix ModeleView
 */
Matrix lookat(Vecteur3f eye, Vecteur3f center, Vecteur3f up)
{
    Vecteur3f temp = {eye.x - center.x, eye.y - center.y, eye.z - center.z};
    Vecteur3f z = normalize(temp);

    Vecteur3f x = crossProduct(up, z);
    x = normalize(x);
    Vecteur3f y = crossProduct(z, x);
    y = normalize(y);

    Matrix res = Matrix::identity(4);
    res[0][0] = x.x;
    res[0][1] = x.y;
    res[0][2] = x.z;
    res[0][3] = -center.x;

    res[1][0] = y.x;
    res[1][1] = y.y;
    res[1][2] = y.z;
    res[1][3] = -center.y;

    res[2][0] = z.x;
    res[2][1] = z.y;
    res[2][2] = z.z;
    res[2][3] = -center.z;
    return res;
}

/**
 * @brief Permet de calculer la matrice "Projection"
 * 
 * @return Matrix  Projection
 */
Matrix projection(Vecteur3f eye, Vecteur3f center){
    Matrix res = Matrix::identity(4);
    Vecteur3f temp = {eye.x - center.x, eye.y - center.y, eye.z - center.z};
    double n = sqrt(temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
    res[3][2] = -1. / n;
    return res;
}

std::vector<float>& Matrix::operator[](const int i) {
    assert(i>=0 && i<rows);
    return m[i];
}

Matrix Matrix::operator*(const Matrix& a) {
    assert(cols == a.rows);
    Matrix result(rows, a.cols);
    for (int i=0; i<rows; i++) {
        for (int j=0; j<a.cols; j++) {
            result.m[i][j] = 0.f;
            for (int k=0; k<cols; k++) {
                result.m[i][j] += m[i][k]*a.m[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::transpose() {
    Matrix result(cols, rows);
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            result[j][i] = m[i][j];
    return result;
}

Matrix Matrix::inverse() {
    assert(rows==cols);
    // augmenting the square matrix with the identity matrix of the same dimensions a => [ai]
    Matrix result(rows, cols*2);
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            result[i][j] = m[i][j];
    for(int i=0; i<rows; i++)
        result[i][i+cols] = 1;
    // first pass
    for (int i=0; i<rows-1; i++) {
        // normalize the first row
        for(int j=result.cols-1; j>=0; j--)
            result[i][j] /= result[i][i];
        for (int k=i+1; k<rows; k++) {
            float coeff = result[k][i];
            for (int j=0; j<result.cols; j++) {
                result[k][j] -= result[i][j]*coeff;
            }
        }
    }
    // normalize the last row
    for(int j=result.cols-1; j>=rows-1; j--)
        result[rows-1][j] /= result[rows-1][rows-1];
    // second pass
    for (int i=rows-1; i>0; i--) {
        for (int k=i-1; k>=0; k--) {
            float coeff = result[k][i];
            for (int j=0; j<result.cols; j++) {
                result[k][j] -= result[i][j]*coeff;
            }
        }
    }
    // cut the identity matrix back
    Matrix truncate(rows, cols);
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            truncate[i][j] = result[i][j+cols];
    return truncate;
}

std::ostream& operator<<(std::ostream& s, Matrix& m) {
    for (int i=0; i<m.nrows(); i++)  {
        for (int j=0; j<m.ncols(); j++) {
            s << m[i][j];
            if (j<m.ncols()-1) s << "\t";
        }
        s << "\n";
    }
    return s;
}