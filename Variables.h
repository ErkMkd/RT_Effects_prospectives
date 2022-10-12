#ifndef VARIABLES_H_INCLUDED
#define VARIABLES_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>

#define RESOLUTION_X_RENDU 1600     //La résolution après post-rendu
#define RESOLUTION_Y_RENDU 900

#define RESOLUTION_X_TRAVAIL  RESOLUTION_X_RENDU   //La résolution de travail
#define RESOLUTION_Y_TRAVAIL  RESOLUTION_Y_RENDU

#define RESOLUTION_X_VISIBLE  1600  //La résolution visible au final
#define RESOLUTION_Y_VISIBLE  900

#define DEG_RAD M_PI/180.
#define RAD_DEG 180./M_PI

#define SCENE_SCENE1 0
#define SCENE_FRACTALE_2D 1
#define SCENE_RAYMARCHING 2
#define SCENE_TERRAIN 3
#define SCENE_MAX 3



class GVars
{
public:
    static GLint openGl_majorVersion;
    static GLint openGl_minorVersion;
    static GLint openGl_max_vertices;
    static GLint openGl_max_indices;
    static GLFWwindow* ecran;
    static int largeur_ecran;
    static int hauteur_ecran;

    static float temps;
    static float temps_reel;
    static float temps_scene;
    static float vitesse_temps;
    static float temps_prec;

    static uint16_t scene_id;

    static bool editionTrajectoire;
    static bool disp_donnees;

    static float taille_tramage;    //Pour le shader affTex_zBuffer
    static float zMin;
    static float zMax;
    static float zMin_nette;
    static float zMax_nette;
    static float ratio_ecran;
    static float distance_focale;

};

#endif // VARIABLES_H_INCLUDED
