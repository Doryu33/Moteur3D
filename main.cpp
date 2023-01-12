#include "tgaimage.h"
#include <cmath>
#include <algorithm>
#include "modele.h"
#include <iostream>
#include <algorithm>

// Constantes de couleur
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
// Taille de la fenètre
int width = 1000;
int height = 1000;
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

int area_of_triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    return (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0;
}

bool is_inside_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int px, int py)
{
    /* Calculate area of triangle ABC */
    int A = area_of_triangle(x1, y1, x2, y2, x3, y3);
    /* Calculate area of triangle PBC */
    int A1 = area_of_triangle(x1, y1, x2, y2, px, py);
    /* Calculate area of triangle PAC */
    int A2 = area_of_triangle(x2, y2, x3, y3, px, py);
    /* Calculate area of triangle PAB */
    int A3 = area_of_triangle(x3, y3, x1, y1, px, py);
    
    double alpha = (double) A2 / (double) A;
    double beta = (double) A3 / (double) A;
    double gamma = (double) A1 / (double) A;

    return alpha> -0.01 && beta > -0.01 && gamma > -0.01;
}

void triangle(std::vector<Vertex> points, TGAImage &image, TGAColor color)
{
    int bboxminx = image.get_width() - 1;
    int bboxminy = image.get_height() - 1;
    int bboxmaxx = 0;
    int bboxmaxy = 0;
    int clampx = image.get_width() - 1;
    int clampy = image.get_height() - 1;
    // Calcul de la boite autour du triangle
    for (int i = 0; i < 3; i++)
    {
        bboxminx = std::max(std::min(bboxminx, (int) points[i].x), 0);
        bboxminy = std::max(std::min(bboxminy, (int) points[i].y), 0);
        bboxmaxx = std::min(std::max(bboxmaxx, (int) points[i].x), clampx);
        bboxmaxy = std::min(std::max(bboxmaxy, (int) points[i].y), clampy);
    }

    Vertex p;
    for (p.x = bboxminx; p.x < bboxmaxx; p.x++)
    {
        for (p.y = bboxminy; p.y < bboxmaxy; p.y++)
        {
            if(is_inside_triangle(points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y, p.x, p.y)){
                image.set(p.x,p.y,color);
            }
        }
    }
}

void wireframe(Modele *modele, TGAImage &image, TGAColor color)
{
    // Constante utile pour le placement des points
    int w = width / 2;
    int h = height / 2;

    std::vector<std::vector<int> > faces = modele->GetFaces();
    std::vector<Vertex> vertex = modele->GetVertex();

    for (int i = 0; i < modele->nbfaces(); i++)
    {
        std::vector<int> face = faces[i];

        Vertex v0 = vertex[face[0] - 1];
        Vertex v1 = vertex[face[1] - 1];
        Vertex v2 = vertex[face[2] - 1];

        int x0 = (v0.x * w) + w;
        int y0 = (v0.y * h) + h;

        int x1 = (v1.x * w) + w;
        int y1 = (v1.y * h) + h;

        int x2 = (v2.x * w) + w;
        int y2 = (v2.y * h) + h;

        line(x0, y0, x1, y1, image, white);
        line(x1, y1, x2, y2, image, white);
        line(x2, y2, x0, y0, image, white);
    }
}

void flat_shading_render(Modele *modele, TGAImage &image){
    // Constante utile pour le placement des points
    int w = width / 2;
    int h = height / 2;

    Vecteur3f light_direction;
    light_direction.x = 0;
    light_direction.y = 0;
    light_direction.z = -1;


    std::vector<std::vector<int> > faces = modele->GetFaces();
    std::vector<Vertex> vertex = modele->GetVertex();

    Vecteur3f worldcoord[3];
    for (int i = 0; i < modele->nbfaces(); i++)
    {
        std::vector<int> face = faces[i];

        Vertex v0 = vertex[face[0] - 1];

        Vertex v1 = vertex[face[1] - 1];

        Vertex v2 = vertex[face[2] - 1];

        Vecteur3f v01 = {v1.x-v0.x, v1.y-v0.y, v1.z-v0.z};
        Vecteur3f v02 = {v2.x-v0.x, v2.y-v0.y, v2.z-v0.z};
        Vecteur3f n = {v01.y*v02.z - v01.z*v02.y, v01.z*v02.x - v01.x*v02.z, v01.x*v02.y - v01.y*v02.x};
        double len = std::sqrt(n.x*n.x + n.y*n.y + n.z*n.z);
        n.x /= len;
        n.y /= len;
        n.z /= len;
        double intensity = -(light_direction.x*n.x + light_direction.y*n.y + light_direction.z*n.z);

        v0.x = (v0.x * w) + w;
        v0.y = (v0.y * h) + h;

        v1.x = (v1.x * w) + w;
        v1.y = (v1.y * h) + h;

        v2.x = (v2.x * w) + w;
        v2.y = (v2.y * h) + h;

        std::vector<Vertex> points;
        points.push_back(v0);
        points.push_back(v1);
        points.push_back(v2);
        if (intensity>0)
        triangle(points,image,TGAColor(255.*intensity, 255.*intensity, 255.*intensity, 255));
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
    }
    else
    {
        modele = new Modele("obj/african_head.obj");
    }

    //wireframe(modele, image, white);
    flat_shading_render(modele,image);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}