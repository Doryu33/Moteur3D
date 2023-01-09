#ifndef __MODELE_H__
#define __MODELE_H__

#include <string>
#include <vector>

/// @brief Structure pour représenter un vertex
struct Vertex {
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
  const std::vector<std::vector<int> >& GetFaces() const;
  int nbvertex();
  int nbfaces();

 private:
  std::string nom_fichier_;
  std::vector<Vertex> vertex_;
  std::vector<std::vector<int> > faces_;
};


#endif //__MODELE_H__