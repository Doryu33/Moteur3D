#include "tgaimage.h"
#include <cmath>
#include <algorithm>
#include "modele.h"
#include <iostream>
#include <algorithm>
#include "our_gl.h"

//################################
// Constantes de couleur
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
//################################

//################################
//Constantes vecteurs
Vecteur3f light_direction = {0., 0., -1.};
Vecteur3f eye = {1., 1., 3.};
Vecteur3f center = {0., 0., 0.};
Vecteur3f up = {0., 1., 0.};
//################################

//################################
//Constante Matrix
Matrix ModelView;
Matrix ViewPort;
Matrix Projection;
//################################

//################################
// Taille de la fenetre
int width = 1000;
int height = 1000;
int depth = 255;
//################################

//################################
// Modele contenant les données du fichier obj
Modele *modele = NULL;
//################################

struct GouraudShader : public IShader
{
    Vecteur3f varying_intensity;

    void vertex(Vertex &v, int idx) override
    {
        Vecteur3f temp = {v.x, v.y, v.z};
        Matrix gl_Vertex = Matrix(&v);

        Vecteur3f prod = crossProduct(temp, light_direction);
        varying_intensity[idx] = std::max(0., sqrt(temp.x * temp.x + temp.y * temp.y + temp.z * temp.z));

        Matrix resultat = ViewPort * Projection * ModelView * gl_Vertex;
        v = matToVect(resultat);
    }

    bool fragment(TGAColor &normal, TGAColor &color, Vecteur3f &bar) override
    {
        Vecteur3f norm,rgb,light;

        norm.x = normal.r * 2. / 255. -1.;
        norm.y = normal.g * 2. / 255. -1.;
        norm.z = normal.b * 2. / 255. -1.;

        norm = normalize(norm);
        light = normalize(light_direction);

        float intensity = sqrt(light_direction.x*light_direction.x + light_direction.y*light_direction.y + light_direction.z*light_direction.z);

        rgb.x = color.r;
        rgb.y = color.g;
        rgb.z = color.b;

        double diffuse = std::min(1., std::max(0., rgb*norm) / 255.);
        
        color.r *= diffuse;
        color.g *= diffuse;
        color.b *= diffuse;

        return false;
    }
};



/**
 * @brief Permet de colorier des pixel sur une ligne définie par 2 points x et y.
 * 
 */
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

/**
 * @brief Permet de dessiner un triangle avec la couleur qui corresponds a la texture
 * 
 * @param points Vertex contenant les coordonnées
 * @param pointsTextures Vertex contenant les coordonnées des textures
 * @param zbuffer Zbuffer
 * @param image L'image sur laquelle dessiner
 * @param texture L'image contenant la texture
 * @param intensity L'intensité du triangle
 */
void triangle(std::vector<Vertex> points, std::vector<Vertex> pointsTextures, float *zbuffer, TGAImage &image, TGAImage &texture, TGAImage &normal, GouraudShader shader,double intensity)
{
    Vertex p;
    Vertex barycentrique;
    //Nouvelle couleur
    TGAColor colorTexture;
    TGAColor colorNormal;

    int bboxminx = image.get_width() - 1;
    int bboxminy = image.get_height() - 1;
    int bboxmaxx = 0;
    int bboxmaxy = 0;
    int clampx = image.get_width() - 1;
    int clampy = image.get_height() - 1;
    // Calcul de la boite autour du triangle
    for (int i = 0; i < 3; i++)
    {
        bboxminx = std::max(std::min(bboxminx, (int)points[i].x), 0);
        bboxminy = std::max(std::min(bboxminy, (int)points[i].y), 0);
        bboxmaxx = std::min(std::max(bboxmaxx, (int)points[i].x), clampx);
        bboxmaxy = std::min(std::max(bboxmaxy, (int)points[i].y), clampy);
    }
    //On itère sur tout les pixels de la boite
    for (p.x = bboxminx; p.x <= bboxmaxx; p.x++)
    {
        for (p.y = bboxminy; p.y <= bboxmaxy; p.y++)
        {
            //Si le pixel est dans le triangle
            if (is_inside_triangle(points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y, p.x, p.y, barycentrique))
            {
                //Calcul coordonnées barycentrique
                double baryx = barycentrique.x * pointsTextures[0].x + barycentrique.y * pointsTextures[1].x + barycentrique.z * pointsTextures[2].x;
                double baryy = barycentrique.x * pointsTextures[0].y + barycentrique.y * pointsTextures[1].y + barycentrique.z * pointsTextures[2].y;

                //Calcul des coordonnées correspondantes dans la texture
                int textureX, textureY;
                textureX = baryx * texture.get_width();
                textureY = baryy * texture.get_height();
                //On récupère la couleur du pixel
                colorTexture = texture.get(textureX, textureY);

                int normX, normY;
                normX = barycentrique.x * textureX + barycentrique.y * textureX + barycentrique.z * textureX;
                normY = barycentrique.x * textureY + barycentrique.y * textureY + barycentrique.z * textureY;

                colorNormal = normal.get(normX, normY);

                p.z = 0;
                p.z += points[0].z * barycentrique.x;
                p.z += points[1].z * barycentrique.y;
                p.z += points[2].z * barycentrique.z;
                if (zbuffer[int(p.x + p.y * width)] < p.z)
                {
                    zbuffer[int(p.x + p.y * width)] = p.z;
                    Vecteur3f bar = {barycentrique.x, barycentrique.y, barycentrique.z};
                    bool discard = shader.fragment(colorNormal,colorTexture,bar);
                    //if(!discard)
                        image.set(p.x, p.y, TGAColor(colorTexture.r * intensity, colorTexture.g * intensity, colorTexture.b * intensity, 255));
                }
            }
        }
    }
}

/**
 * @brief Permet un rendu en file de fer
 * 
 * @param modele Modele du .obj utilisé
 * @param image Image sur laquelle déssiner
 * @param color Couleur des fils de fer
 */
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




void render(Modele *modele, float *zbuffer, TGAImage &image, TGAImage &texture, TGAImage &normal, GouraudShader shader)
{
    //################################
    //Definition des matrice ModelView, ViewPort et Projection
    ModelView = lookat(eye, center, up);
    ViewPort = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);
    Vecteur3f temp = {eye.x - center.x, eye.y - center.y, eye.z - center.z};
    double n = sqrt(temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
    Projection = projection(-1./n);
    //################################

    //################################
    //On recupere les informations du .obj via modele.
    std::vector<std::vector<int> > faces = modele->GetFaces();
    std::vector<std::vector<int> > faces2 = modele->GetFacesCoord();
    std::vector<Vertex> vertex = modele->GetVertex();
    std::vector<Vertex> vertexT = modele->GetVertexTexture();
    //################################

    //################################
    //On itère sur toutes les faces de l'objet.
    for (int i = 0; i < modele->nbfaces(); i++)
    {
        //On récupère les information de la face actuelle
        std::vector<int> face = faces[i];
        std::vector<int> faceCoord = faces2[i];

        //################################
        //On va créer 2x3 vertex à partir des faces (Un vertex de coordonnée, et un pour les textures)            
        Vertex v0 = vertex[face[0] - 1];
        Vertex vt0 = vertexT[faceCoord[0] - 1];

        Vertex v1 = vertex[face[1] - 1];
        Vertex vt1 = vertexT[faceCoord[1] - 1];

        Vertex v2 = vertex[face[2] - 1];
        Vertex vt2 = vertexT[faceCoord[2] - 1];
        
        shader.vertex(v0, 0);
        shader.vertex(v1, 1);
        shader.vertex(v2, 2);

        //On crée 2 vecteurs
        Vecteur3f v01 = {v1.x - v0.x, v1.y - v0.y, v1.z - v0.z};
        Vecteur3f v02 = {v2.x - v0.x, v2.y - v0.y, v2.z - v0.z};
        //Calcul de la normale des deux vecteurs
        Vecteur3f n = {v01.y * v02.z - v01.z * v02.y, v01.z * v02.x - v01.x * v02.z, v01.x * v02.y - v01.y * v02.x};
        //On passe le vecteur normal en vecteur unitaire
        double len = std::sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
        n.x /= len;
        n.y /= len;
        n.z /= len;
        //Calcul de l'intensité
        double intensity = -(light_direction.x * n.x + light_direction.y * n.y + light_direction.z * n.z);
        
        //On crée un tableau avec les trois vertex de texture
        //pour récupérer la couleur correspondante dans le fichier texture
        std::vector<Vertex> pointsTexture;
        pointsTexture.push_back(vt0);
        pointsTexture.push_back(vt1);
        pointsTexture.push_back(vt2);

        //Meme chose mais avec les coordonnées + mise a l'échelle sur l'écran
        std::vector<Vertex> screen_coords;
        //screen_coords.push_back(matToVect(ViewPort * Projection * ModelView * Matrix(&v0)));
        //screen_coords.push_back(matToVect(ViewPort * Projection * ModelView * Matrix(&v1)));
        //screen_coords.push_back(matToVect(ViewPort * Projection * ModelView * Matrix(&v2)));
        screen_coords.push_back(v0);
        screen_coords.push_back(v1);
        screen_coords.push_back(v2);

        //Si intensity > 0 = triangle éclairé = on l'affiche
        if (intensity > 0)
            triangle(screen_coords, pointsTexture, zbuffer, image, texture, normal, shader, intensity);
    }
    //################################

}

int main(int argc, char **argv)
{
    int fin;
    std::string fileName;
    std::string fileTextureName;
    std::string fileNormalName;
    // On vérifie le nombre d'argument: si un argument est précisé, on s'en sert.
    // Sinon par défaut on ouvre le fichier "obj/african_head.obj"
    if (2 == argc)
    {
        fileName = argv[1];
        fin = fileName.find(".");
        fileTextureName = fileName.substr(0, fin) + "_diffuse.tga";
        fileNormalName = fileName.substr(0, fin) + "_nm.tga";
        modele = new Modele(argv[1]);
    }
    else
    {
        std::cout << "Aucun fichier spécifié en argument. Utilisation du fichier par défaut (african_head).";
        fileName = "obj/african_head.obj";
        fileTextureName = "obj/african_head_diffuse.tga";
        fileNormalName = "obj/african_head_nm.tga";
        modele = new Modele("obj/african_head.obj");
    }
    GouraudShader shader;

    // Création de l'image.
    TGAImage image(width, height, TGAImage::RGB);
    TGAImage texture = TGAImage();
    texture.read_tga_file(fileTextureName.c_str());
    texture.flip_vertically();

    TGAImage normal = TGAImage();
    normal.read_tga_file(fileNormalName.c_str());
    normal.flip_vertically();

    modele = new Modele(fileName);
    // Z buffer
    float *zbuffer = new float[width * height];
    // Init a -l'infinie.
    for (int i = width * height; i--; zbuffer[i] = -std::numeric_limits<float>::max());
    
    //wireframe(modele, image, white);
    render(modele, zbuffer, image, texture, normal, shader);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");

    delete modele;
    return 0;
}