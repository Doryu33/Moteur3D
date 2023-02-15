#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include "modele.h"

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

// Constructeur qui prend en argument le nom du fichier à lire
Modele::Modele(const std::string &nom_fichier) : nom_fichier_(nom_fichier) {
  LireFichier();
}

// Méthode pour lire le fichier et stocker les données dans les listes
void Modele::LireFichier()
{
  std::ifstream fichier(nom_fichier_);
  if (!fichier.is_open())
  {
    // Erreur de lecture du fichier
    std::cout << "Erreur de lecture du fichier. \n";
    return;
  }

  // Acces au fichier valide
  std::cout << "Ouverture du fichier \"" << nom_fichier_ << "\"...\n";


  std::string ligne;
  while (std::getline(fichier, ligne))
  {
    // On vérifie si la ligne commence bien par "v" (et pas "vf")
    if (ligne[0] == 'v' && ligne[1]== ' ')
    {
      // On crée un objet vertex pour stocker les 3 premiers nombres dedans
      Vertex vertex;

      // On commence à l'indice 2 pour ignorer le "v "
      size_t debut = 2; 
      // On cherche l'espace pour délimiter la fin.
      size_t fin = ligne.find(' ', debut);

      // On attribue a vertex.x le premier nombre extrait de la ligne
      vertex.x = std::stod(ligne.substr(debut, fin - debut));

      //On decalle le début de un char.
      debut = fin + 1;
      // On cherche l'espace pour délimiter la fin.
      fin = ligne.find(' ', debut);

      // On attribue a vertex.y le second nombre extrait de la ligne
      vertex.y = std::stod(ligne.substr(debut, fin - debut));

      //On decalle le debut de un char.
      debut = fin + 1;

      //On attribue a vertex.z le dernier nombre
      vertex.z = std::stod(ligne.substr(debut));
      vertex_.push_back(vertex);
    }
    //Si la ligne commence par "f"
    else if (ligne[0] == 'f' && ligne[1]== ' ')
    {
      // Extrait les 3 premiers nombres de la ligne et les stocke dans le tableau
      std::vector<int> nombres;
      std::vector<int> nombres2;
      // On commence à l'indice 2 pour ignorer le "f "
      size_t debut = 2; 

      // On cherche le "/" pour délimiter.
      size_t fin = ligne.find('/', debut);
      size_t debut2 = fin+1;
      size_t fin2 = ligne.find('/', debut2);
      //On boucle tant que la premiere liste n'est pas remplie
      while (nombres.size() < 3 && fin != std::string::npos)
      {
        nombres.push_back(std::stoi(ligne.substr(debut, fin - debut)));
        nombres2.push_back(std::stoi(ligne.substr(debut2, fin2 - debut2)));
        //Une fois le premier nombre trouvé on passe au groupe suivant (séparé par un espace)
        fin = ligne.find(' ', debut);
        debut = fin + 1;

        fin2 = ligne.find('/', debut);
        debut2 = fin2 + 1;


      }
      // Ajoute le dernier nombre s'il y en a encore un (2nd cas d'arret du while())
      if (nombres.size() < 3)
      {
        nombres.push_back(std::stoi(ligne.substr(debut)));
      }
      faces_.push_back(nombres);
      facesCoord_.push_back(nombres2);
    }
    else if (ligne[0] == 'v' && ligne[1] == 't'){
      // On crée un objet vertex pour stocker les 3 premiers nombres dedans
      VertexTexture vertexTexture;

      // On commence à l'indice 3 pour ignorer le "vt "
      size_t debut = 4; 
      // On cherche l'espace pour délimiter la fin.
      size_t fin = ligne.find(' ', debut);

      // On attribue a vertex.x le premier nombre extrait de la ligne
      vertexTexture.x = std::stod(ligne.substr(debut, fin - debut));

      //On decalle le début de un char.
      debut = fin + 1;
      // On cherche l'espace pour délimiter la fin.
      fin = ligne.find(' ', debut);

      // On attribue a vertex.y le second nombre extrait de la ligne
      vertexTexture.y = std::stod(ligne.substr(debut, fin - debut));

      //On decalle le debut de un char.
      debut = fin + 1;

      //On attribue a vertex.z le dernier nombre
      vertexTexture.z = std::stod(ligne.substr(debut));
      vertexT_.push_back(vertexTexture);
    }
  }
  // On ferme le fichier
  fichier.close();
}

/// @brief Getter de la taille de la liste de vertex
/// @return taille liste de vertex
int Modele::nbvertex()
{
  return (int)vertex_.size();
}

/// @brief Getter de la taille de la liste de faces
/// @return taille de la liste de faces
int Modele::nbfaces(){
  return (int)faces_.size();
}

int Modele::nbvertexText(){
  return (int)vertexT_.size();
}

int Modele::nbfacesCoord(){
  return (int)facesCoord_.size();
}

void Modele::project(double c){
    Matrix m;
    Matrix identify;
    identify = m.identity(4);
    identify[3][2] = (-1/c);

    for (size_t i = 0; i < nbvertex(); i++)
    {
        m = Matrix(4,1);
        m[0][0] = vertex_[i].x;
        m[1][0] = vertex_[i].y;
        m[2][0] = vertex_[i].z;
        m[3][0] = 1.0;

        m = identify * m;

        vertex_[i].x = m[0][0] / m[3][0];
        vertex_[i].y = m[1][0] / m[3][0];
        vertex_[i].z = m[2][0] / m[3][0];
    }
}

// Getter
const std::vector<Vertex> &Modele::GetVertex() const { return vertex_; }
const std::vector<VertexTexture> &Modele::GetVertexTexture() const { return vertexT_; }

// Getter
const std::vector<std::vector<int> > &Modele::GetFaces() const { return faces_; }
const std::vector<std::vector<int> > &Modele::GetFacesCoord() const { return facesCoord_; }