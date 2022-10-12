#ifndef SCENE1_H_INCLUDED
#define SCENE1_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <iostream>
#include "math.h"

#include "Variables.h"
#include "Fonctions.h"
#include "repere.h"
#include "Objets3d/Shaders3d.h"
#include "Contexte3d.h"
#include "Objets3d/ChargeLWO2.h"
#include "Objets3d/ConvertionLWO2.h"
#include "Locomotive.h"

using namespace std;

class Scene1
{
    public:

        static GLuint vboTest;
        static GLuint eboTest;
        static GLuint vaoTest;

        // Données raymarching:
        static GLuint vbo_rmGare;
        static GLuint vao_rmGare;
        static int iter;

        static Contexte3d* moteur3d;

        static ChargeLWO2*  testObjetLWO2;
        static ConvertionLWO2*  testObjetConv;

        static ChargeLWO2*  testObjetLWO2_2;
        static ConvertionLWO2*  testObjetConv_2;

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

        static bool inc_zMin_nette;
        static bool dec_zMin_nette;
        static bool inc_zMax_nette;
        static bool dec_zMax_nette;

        static bool iter_moins;
        static bool iter_plus;

        static int depId;

        static float aX;
        static float aY;
        static float aZ;

        static float aR;
        static float aT;
        static float aL;

        static Locomotive* loco;

    public:
        static bool init();
        static void destruction();
        static void clavier(int key,bool etat);
        static void maj_eclairages();
        static void maj_mouvements();
        static void affiche();

};
#endif // SCENE1_H_INCLUDED
