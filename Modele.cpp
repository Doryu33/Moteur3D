#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include "modele.h"

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
      // On commence à l'indice 2 pour ignorer le "f "
      size_t debut = 2; 

      // On cherche le "/" pour délimiter.
      size_t fin = ligne.find('/', debut);

      //On boucle tant que la premiere liste n'est pas remplie
      while (nombres.size() < 3 && fin != std::string::npos)
      {
        nombres.push_back(std::stoi(ligne.substr(debut, fin - debut)));
        //Une fois le premier nombre trouvé on passe au groupe suivant (séparé par un espace)
        fin = ligne.find(' ', debut);
        debut = fin + 1;
      }
      // Ajoute le dernier nombre s'il y en a encore un (2nd cas d'arret du while())
      if (nombres.size() < 3)
      {
        nombres.push_back(std::stoi(ligne.substr(debut)));
      }
      faces_.push_back(nombres);
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

// Getter
const std::vector<Vertex> &Modele::GetVertex() const { return vertex_; }
const std::vector<VertexTexture> &Modele::GetVertexTexture() const { return vertexT_; }

// Getter
const std::vector<std::vector<int> > &Modele::GetFaces() const { return faces_; }
const std::vector<std::vector<int> > &Modele::GetFacesCoord() const { return facesCoord_; }