#ifndef SCENE_TERRAIN_H_INCLUDED
#define SCENE_TERRAIN_H_INCLUDED

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
#include "Bruit.h"
#include "Nuages.h"



using namespace std;

class Scene_terrain
{
    public:

        static Contexte3d* moteur3d;

        static ChargeLWO2*  testObjetLWO2;
        static ConvertionLWO2*  testObjetConv;
        static Volume* robot;

        // Terrain:
        static GLuint vbo_rmt;
        static GLuint vao_rmt;

        static float* bitmap_terrain;
        static float* bitmap_terrain2;
        static float* bitmap_terrain3;
        static GLuint textures_terrain[3];

        static uint32_t tailles_textures_terrain[2*3];
        static uint32_t taille_fondamentale[2*3];
        static uint32_t nbr_octaves[3];
        static float persistance[3];
        static uint32_t rand_init[3];

        static float amplitude_terrain[3];
        static float facteur_precision_distance;

        static float echelle_terrain[2*3];

        static Nuages* ptitsNuages;

        //Controle des paramètres de génération du bitmap
        static bool inc_terr_fond_xSize;
        static bool dec_terr_fond_xSize;
        static bool inc_terr_fond_ySize;
        static bool dec_terr_fond_ySize;
        static bool inc_terr_taille_xSize;
        static bool dec_terr_taille_xSize;
        static bool inc_terr_taille_zSize;
        static bool dec_terr_taille_zSize;
        static bool inc_terr_nOct;
        static bool dec_terr_nOct;
        static bool inc_terr_pers;
        static bool dec_terr_pers;
        static bool inc_terr_srand;
        static bool dec_terr_srand;
        static bool inc_amplitude;
        static bool dec_amplitude;


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

        static int depId;
        static int bmTerrainId;

        static float aX;
        static float aY;
        static float aZ;

        static float aR;
        static float aT;
        static float aL;

    public:
        static bool init();
        static void destruction();
        static bool maj_bitmap_terrain();
        static bool maj_bitmap_terrain2();
        static bool maj_bitmap_terrain3();
        static bool maj_texture_terrain();
        static bool maj_texture_terrain2();
        static bool maj_texture_terrain3();
        static void clavier(int key,bool etat);
        static void maj_eclairages();
        static void maj_mouvements();
        static void affiche_textures();
        static void affiche();

};

#endif // SCENE_TERRAIN_H_INCLUDED
