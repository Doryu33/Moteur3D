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

/// @brief Structure pour représenter un vecteur 3D
struct Vecteur3i {
  int x;
  int y;
  int z;
};

struct Vecteur3f{
  double x;
  double y;
  double z;
};

class Modele {
 public:
  // Constructeur qui prend en argument le nom du fichier à lire
  explicit Modele(const std::string& nom_fichier);

  // Méthode pour lire le fichier et stocker les données
  void LireFichier();

  // Méthode pour accéder aux données stockées
  const std::vector<Vertex>& GetVertex() const;
  const std::vector<VertexTexture>& GetVertexTexture() const;
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
  std::vector<VertexTexture> vertexT_;
  std::vector<std::vector<int> > faces_;
  std::vector<std::vector<int> > facesCoord_;
};


#endif //__MODELE_H__