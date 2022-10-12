#ifndef PRIMITIVES_H_INCLUDED
#define PRIMITIVES_H_INCLUDED

/********************************************************************************

    Pour le moment, utilisent les shaders des objets 3d modélisés.
    Optimiser les primitives plans: "position" à 2 coordonnées plutôt que 3,
    et sans passage de normale.

/********************************************************************************/

#include "../glew.h"
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <iostream>
#include "math.h"

#include "ShadersPrimitives.h"
#include "../vec3f.h"

#define DEG_RAD M_PI/180.
#define NOMBREDOR float((1.+sqrt(5.)) / 2.)
#define ANGLEDIAGONALE_OR float(58.2825255885)*float(DEG_RAD)

//Modes de rendus:
#define PRIM_LISSE 1
#define PRIM_PLAT 2

class Primitives
{
    public:
        static float triangle_sommets[2*3];
        static float plan_sommets[2*4];
        static float cube_sommets[3*8];
        static uint16_t cube_faces[4*6];
        static float cube_normales[3*6];
        static float tetraedre_sommets[3*4];
        static uint16_t tetraedre_faces[3*4];
        static float tetraedre_normales[3*4];

        static float icosaedre_sommets[12*3];
        static uint16_t icosaedre_faces[20*3];

        static float materiau_amb[4];
        static float materiau_diff[4];
        static float materiau_lum[4];
        static float materiau_spec[4];
        static float materiau_brillance;


    public:

        static void genere_disque(float rayon,uint16_t numCotes,float* coordsTab, float y=0.,float angleDepart=0.);
        static void genere_disque2d(float rayon,uint16_t numCotes,float* coordsTab);
        static void genere_cylindre(float rayon, float hauteur, uint16_t numCotes, uint16_t numSections, float* coordsTab);
        static void genere_faces_cylindre(uint16_t numCotes, uint16_t numSections, uint16_t* facesTab);
        static void genere_normales_cylindre(uint32_t numPoints, float* coordsTab, float* normalesTab);
        static void genere_tore(float rayon1,float rayon2, uint16_t numCotes, uint16_t numSections, float* coordsTab);
        static void genere_faces_tore(uint16_t numCotes, uint16_t numSections, uint16_t* facesTab);
        static void genere_normales_tore(float rayon,uint32_t numPoints,float* coordsTab, float* normalesTab);
        static void genere_sphere(float rayon, uint16_t numCotes, uint16_t numSegments, float* coordsTab);
        static void genere_faces_sphere(uint16_t numCotes, uint16_t numSegments, uint16_t* facesTab);
        static void genere_normales_sphere(uint32_t numPoints, float* coordsTab, float* normalesTab);
        static void genere_sphere_tesselation(float rayon, uint16_t numDivisions, float* coordsTab);
        static void genere_faces_sphere_tesselation(uint16_t numDivisions, uint16_t* facesTab);
        static void genere_normales_cone(uint16_t numCotes, float* coordsTab, float* normalesTab );
        static void genere_normales_cone_tronc(uint16_t numCotes, float* coordsTab, float* normalesTab );

        static void init_materiau(float Ramb,float Vamb, float Bamb,float Aamb,
                                       float Rdiff,float Vdiff,float Bdiff,float Adiff,
                                       float Rlum,float Vlum,float Blum,float Alum,
                                       float Rspec,float Vspec,float Bspec,float Aspec,
                                       float brill);

        static void init_materiau(uint32_t RVBAamb, uint32_t RVBAdiff, uint32_t RVBAlum, uint32_t RVBAspec,float brill);
        static void init_phong2si();


        static void genere_buffers_phong2si(GLuint& vbo, GLuint& vao);
        static void maj_tableau_sommets_vbo_flat(int numFaces,uint16_t* facesTab,float* sommets, float* normalesTab, float* sommetsVBO);
        static void maj_tableau_sommets_vbo_smooth(int numFaces,uint16_t* facesTab,float* sommets, float* normalesTab, float* sommetsVBO);

    public:
        GLuint vbo;
        GLuint vao;
        uint16_t numSommets;
        uint16_t numFaces;
        uint16_t numSommetsVBO;
        float* sommetsVBO;
        float* sommets;
        float* normales;
        uint16_t* faces;
        uint8_t mode;   //PRIM_LISSE, PRIM_PLAT

        Primitives();
        ~Primitives();

};

class Plan : public Primitives
{
    public:
        float xSize;
        float zSize;

        Plan(float xs=1.,float zs=1.);
        ~Plan();
        void maj_sommetsVBO();
        void affiche();
};

class Plan2Cotes : public Primitives
{
    public:
        float xSize;
        float zSize;

        Plan2Cotes(float xs=1.,float zs=1.);
        ~Plan2Cotes();
        void maj_sommetsVBO();
        void affiche();
};

class Disque : public Primitives
{
    public:
        float rayon;

        Disque(float r,uint16_t p_numSommets);
        ~Disque();
        void maj_sommetsVBO();
        void affiche();
};

class Disque2Cotes : public Primitives
{
    public:
        float rayon;

        Disque2Cotes(float r,uint16_t p_numSommets);
        ~Disque2Cotes();
        void maj_sommetsVBO();
        void affiche();
};


class Cube : public Primitives
{
    public:
        float xSize;
        float ySize;
        float zSize;
        Cube(float xs,float ys, float zs);
        ~Cube();
        void maj_sommetsVBO();
        void affiche();
};

class Cylindre : public Primitives
{
    public:
        float rayon;
        float longueur;
        uint16_t numCotes;
        uint16_t numSections;


        Cylindre(float r, float l, uint16_t p_numCotes, uint16_t p_numSections, uint8_t p_mode=PRIM_LISSE);
        ~Cylindre();
        void maj_sommetsVBO();
        void affiche();
};

class CylindreFerme : public Primitives
{
    public:
        float rayon;
        float longueur;
        uint16_t numCotes;
        uint16_t numSections;

        CylindreFerme(float r, float l, uint16_t p_numCotes, uint16_t p_numSections, uint8_t p_mode=PRIM_LISSE);
        ~CylindreFerme();
        void maj_sommetsVBO();
        void affiche();
};

class Sphere : public Primitives
{
    public:
        float rayon;
        uint16_t numCotes;
        uint16_t numSegments;

        Sphere(float r,uint16_t p_numCotes,uint16_t p_numSegments, uint8_t p_mode=PRIM_LISSE);
        ~Sphere();
        void maj_sommetsVBO();
        void affiche();
};

class SphereTesselation : public Primitives
{
    public:
        float rayon;
        uint16_t numDivisions;

        SphereTesselation(float r, uint16_t p_numDivisions, uint8_t p_mode=PRIM_LISSE);
        ~SphereTesselation();
        void maj_sommetsVBO();
        void affiche();
};

class Tore : public Primitives
{
    public:
        float rayon;
        float rayon_section;
        uint16_t numCotes;
        uint16_t numSegments;

        Tore(float r, float r_section, uint16_t p_numCotes,uint16_t p_numSegments,uint8_t p_mode=PRIM_LISSE);
        ~Tore();
        void maj_sommetsVBO();
        void affiche();

};

class Cone : public Primitives
{
    public:
        float rayon;
        float hauteur;
        uint16_t numCotes;

        Cone(float r, float h, uint16_t p_numCotes,uint8_t p_mode=PRIM_LISSE);
        ~Cone();
        void maj_sommetsVBO();
        void affiche();
};


class ConeTronc : public Primitives
{
    public:
        float rayon_sommet;
        float rayon_base;
        float hauteur;
        uint16_t numCotes;

        ConeTronc(float rs, float rb, float h, uint16_t p_numCotes,uint8_t p_mode=PRIM_LISSE);
        ~ConeTronc();
        void maj_sommetsVBO();
        void affiche();
};

class BoiteArrondie : public Primitives
{
    public:
        float xSize;
        float ySize;
        float zSize;
        float rayon;
        uint16_t numCotes;
        uint16_t numSegments;
        BoiteArrondie(float xs,float ys, float zs,float r, uint16_t p_numCotes, uint16_t p_numSegments,uint8_t p_mode=PRIM_LISSE);
        ~BoiteArrondie();
        void maj_sommetsVBO();
        void affiche();
};

class Capsule : public Primitives
{
    public:
        float rayon;
        float hauteur_fu;
        uint16_t numCotes;
        uint16_t numSegments_extremites;
        Capsule(float r, float h, uint16_t p_numCotes,uint16_t p_numSections,uint8_t p_mode=PRIM_LISSE);
        ~Capsule();
        void maj_sommetsVBO();
        void affiche();
};




#endif // PRIMITIVES_H_INCLUDED
