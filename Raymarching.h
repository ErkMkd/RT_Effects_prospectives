#ifndef RAYMARCHING_H_INCLUDED
#define RAYMARCHING_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <iostream>
#include "math.h"

#include "Variables.h"
#include "Fonctions.h"
#include "repere.h"
#include "ShadersDemo.h"
#include "Contexte3d.h"
#include "Primitives/Primitives.h"
#include "Objets3d/ChargeLWO2.h"
#include "Objets3d/ConvertionLWO2.h"
#include "FrameBuffers.h"

using namespace std;

class Raymarching
{
    public:
        static GLuint vbo_rm;
        static GLuint vao_rm;
        static int iter;

        static Contexte3d* moteur3d;

        static float l1_position[3];
        static float l2_position[3];
        static float l3_position[3];

        static Sphere* l1_sphere;
        static Sphere* l2_sphere;
        static Sphere* l3_sphere;
        static Repere* repere_l1s;
        static Repere* repere_l2s;
        static Repere* repere_l3s;

        static ChargeLWO2*  testObjetLWO2;
        static ConvertionLWO2*  testObjetConv;

        static vec3f objetsPos;

               //Commandes clavier:
        static bool dep_droite;
        static bool dep_gauche;
        static bool dep_haut;
        static bool dep_bas;
        static bool dep_avant;
        static bool dep_arriere;

        static bool rot_droite;
        static bool rot_gauche;
        static bool rot_haut;
        static bool rot_bas;
        static bool rot_r_gauche;
        static bool rot_r_droite;

        static bool iter_moins;
        static bool iter_plus;

        static bool inc_zMin_nette;
        static bool dec_zMin_nette;
        static bool inc_zMax_nette;
        static bool dec_zMax_nette;

        static int depId;

        static float aX;
        static float aY;
        static float aZ;

        static float aR;
        static float aT;
        static float aL;

    public:
        static bool init();
        static void destruction();
        static void clavier(int key,bool etat);
        static void maj_eclairages();
        static void maj_mouvements();
        static void affiche();

};

#endif // RAYMARCHING_H_INCLUDED
