#ifndef __MODELE_H__
#define __MODELE_H__

#include <string>
#include <vector>
#include "matrix.h"

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

Vertex matToVect(Matrix m);
Vecteur3f normalize(Vecteur3f vecteur);
Vecteur3f crossProduct(Vecteur3f u, Vecteur3f v);

class Modele {
 public:
  // Constructeur qui prend en argument le nom du fichier à lire
  explicit Modele(const std::string& nom_fichier);

  // Méthode pour lire le fichier et stocker les données
  void LireFichier();

  // Méthode pour accéder aux données stockées
  const std::vector<Vertex>& GetVertex() const;
  const std::vector<Vertex>& GetVertexTexture() const;
  const std::vector<std::vector<int> >& GetFaces() const;
  const std::vector<std::vector<int> >& GetFacesCoord() const;
  int nbvertex();
  int nbvertexText();
  int nbfaces();
  int nbfacesCoord();
  void project(double c);


 private:
  std::string nom_fichier_;
  std::vector<Vertex> vertex_;
  std::vector<Vertex> vertexT_;
  std::vector<std::vector<int> > faces_;
  std::vector<std::vector<int> > facesCoord_;
};


#endif //__MODELE_H__
