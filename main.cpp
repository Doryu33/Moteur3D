#include "tgaimage.h"
#include <math.h>
#include "modele.h"
#include <iostream>

// Constantes de couleur
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
// Taille de la fenètre
int width = 500;
int height = 500;
// Modele contenant les données du fichier obj
Modele *modele = NULL;

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++)
    {
        if (steep)
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx)
        {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void wireframe(Modele *modele, TGAImage &image, TGAColor color){
    // Constante utile pour le placement des points
    int w = width / 2;
    int h = height / 2;


    std::vector<std::vector<int> > faces = modele->GetFaces();
    std::vector<Vertex> vertex = modele->GetVertex();

    for (int i = 0; i < modele->nbfaces(); i++)
    {
        std::vector<int> face = faces[i];
        
        Vertex v0 = vertex[face[0]-1];
        Vertex v1 = vertex[face[1]-1];
        Vertex v2 = vertex[face[2]-1];
        
        int x0 = (v0.x*w) + w;
        int y0 = (v0.y*h) + h;

        int x1 = (v1.x*w) + w;
        int y1 = (v1.y*h) + h;

        int x2 = (v2.x*w) + w;
        int y2 = (v2.y*h) + h;
        
        line(x0, y0, x1, y1, image, white);
        line(x1, y1, x2, y2, image, white);
        line(x2, y2, x0, y0, image, white);
    }
}


int main(int argc, char **argv)
{
    // Création de l'image.
    TGAImage image(width, height, TGAImage::RGB);
    // On vérifie le nombre d'argument: si un argument est précisé, on s'en sert.
    // Sinon par défaut on ouvre le fichier "obj/african_head.obj"
    if (2 == argc)
    {
        modele = new Modele(argv[1]);
    } else  {
        modele = new Modele("obj/african_head.obj");
    }

    wireframe(modele,image,white);
    

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}