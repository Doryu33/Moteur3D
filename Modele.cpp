#include <fstream>
#include <string>
#include <vector>

struct Vertex {
  double x;
  double y;
  double z;
};

class Modele {
 public:
  // Constructeur qui prend en argument le nom du fichier à lire
  explicit Modele(const std::string& nom_fichier) : nom_fichier_(nom_fichier) {}

  // Méthode pour lire le fichier et stocker les données
  void LireFichier() {
    std::ifstream fichier(nom_fichier_);
    if (!fichier.is_open()) {
      // Erreur de lecture du fichier
      return;
    }

    std::string ligne;
    while (std::getline(fichier, ligne)) {
      // Vérifie si la ligne commence par "v"
      if (ligne[0] == 'v') {
        // Extrait les 3 nombres de la ligne et les stocke dans un objet Vertex
        Vertex vertex;
        size_t debut = 2;  // On commence à l'indice 2 pour ignorer le "v "
        size_t fin = ligne.find(' ', debut);
        vertex.x = std::stod(ligne.substr(debut, fin - debut));
        debut = fin + 1;
        fin = ligne.find(' ', debut);
        vertex.y = std::stod(ligne.substr(debut, fin - debut));
        debut = fin + 1;
        vertex.z = std::stod(ligne.substr(debut));
        vertex_.push_back(vertex);
      } else if (ligne[0] == 'f') {
        // Extrait les 3 premiers nombres de la ligne et les stocke dans le tableau
        std::vector<int> nombres;
        size_t debut = 2;  // On commence à l'indice 2 pour ignorer le "f "
        size_t fin = ligne.find('/', debut);
        while (nombres.size() < 3 && fin != std::string::npos) {
          nombres.push_back(std::stoi(ligne.substr(debut, fin - debut)));
          debut = fin + 1;
          fin = ligne.find('/', debut);
        }
        // Ajoute le dernier nombre s'il y en a encore un
        if (nombres.size() < 3) {
          nombres.push_back(std::stoi(ligne.substr(debut)));
        }
        faces_.push_back(nombres);
      }
    }
    fichier.close();
  }

 // Méthode pour accéder aux données stockées
  const std::vector<Vertex>& GetVertex() const { return vertex_; }
  const std::vector<std::vector<int>>& GetFaces() const { return faces_; }

 private:
  std::string nom_fichier_;
  std::vector<Vertex> vertex_;
  std::vector<std::vector<int>> faces_;
};