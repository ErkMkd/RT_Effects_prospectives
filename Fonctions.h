#ifndef FONCTIONS_H_INCLUDED
#define FONCTIONS_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <stdlib.h>
#include <iostream>
#include "math.h"

#include "Variables.h"
#include "repere.h"
#include "scene1.h"
#include "Scene_terrain.h"
#include "scene_etudeMandelbrot.h"
#include "Raymarching.h"

using namespace std;

class GFunc
{
    public:
        static GLuint vbo_affTex;
        static GLuint vao_affTex;
        static GLuint vbo_affTex_zBuffer;
        static GLuint vao_affTex_zBuffer;
        static GLuint vbo_profChamp;
        static GLuint vao_profChamp;
        static GLuint vbo_afftex_bitmap;
        static GLuint vao_afftex_bitmap;

    public:
        static bool erreur_openGl(const char* message);
        static void maj_temps();
        static void glfw_error_callback(int error,const char* description);
        static void dimensionne_ecran();
        static void redimensionne_ecran(GLFWwindow* window, int width, int height);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static bool setup_opengl();
        static void remove_opengl();
        static void calcul_coordonnees_ecran(float* point,float* pointEcran);
        static void rotation_plan(float angle,float sX, float sY, float& dX, float& dY);

        static void RGBi_RGBf(uint32_t RGBi,float RGBf[3]);
        static void BGRi_RGBf(uint32_t BGRi,float RGBf[3]);
        static void affiche_texture(GLuint id_fbo,GLuint id_texture);
        static void affiche_bitmap(GLuint id_fbo,GLuint id_texture,float echelle,int largeur_texture, int hauteur_texture,float x=0., float y=0.);
        static void affiche_texture_zBuffer(GLuint id_fbo,GLuint id_texture_couleur,GLuint id_texture_zBuffer);
        static void affiche_texture_profondeur_de_champ(GLuint id_fbo,GLuint id_texture_couleur,GLuint id_texture_zBuffer);
        static void rotation_points(float cx,float cy,float cz,float ax, float ay, float az, float angle, float* points, float* points_resultat, uint32_t numPoints);
        static void rotation_RTL(float cx, float cy, float cz,float roulis,float tangage, float lacet, float* points, float* resultat,uint32_t numPoints);
};
#endif // FONCTIONS_H_INCLUDED
