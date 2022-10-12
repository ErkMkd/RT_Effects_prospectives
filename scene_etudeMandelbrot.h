#ifndef SCENE_ETUDEMANDELBROT_H_INCLUDED
#define SCENE_ETUDEMANDELBROT_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <iostream>
#include "math.h"

#include "Variables.h"
#include "Fonctions.h"
#include "repere.h"
#include "ShadersDemo.h"

class Fractale2d
{
    public:
        static GLuint vbo_mb;
        static GLuint vao_mb;

        static double xpos;
        static double ypos;
        static double zoom;
        static float zoom_ar;
        static double facteur_zoom;
        static double azoom;
        static double axpos;
        static double aypos;
        static double azpos;
        static double deceleration;
        static double acceleration;
        static int iter;
        static float alpha;
        static double puissance;

        static bool dep_droite;
        static bool dep_gauche;
        static bool dep_avant;
        static bool dep_arriere;
        static bool dep_haut;
        static bool dep_bas;
        static bool zoom_in;
        static bool zoom_out;
        static bool rot_CCW;
        static bool rot_CW;
        static bool iter_plus;
        static bool iter_moins;
        static bool juliaC_r_dec;
        static bool juliaC_r_inc;
        static bool juliaC_i_dec;
        static bool juliaC_i_inc;
        static bool puissance_inc;
        static bool puissance_dec;

        static int  depId;

        static double point_x;
        static double point_y;
        static double juliaC[2];

    public:

        static bool init();
        static void destruction();
        static void clavier(int key,bool etat);
        static void maj_eclairages();
        static void maj_mouvements();
        static void convertion_coords_mb(double* pt, float* pt_ecran);
        static void affiche();
};

#endif // SCENE_ETUDEMANDELBROT_H_INCLUDED
