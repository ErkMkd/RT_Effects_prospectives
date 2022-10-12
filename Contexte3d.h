#ifndef CONTEXTE3D_H_INCLUDED
#define CONTEXTE3D_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "Objets3d/Shaders3d.h"
#include "ShadersDemo.h"
#include "Primitives/ShadersPrimitives.h"
#include "repere.h"
#include "Objets3d/Source_Lumineuse.h"
#include "Variables.h"


#define C3D_OK 0

#define C3D_MAX_LUMIERES 3

#define C3D_BINDING_POINT_UBO_MATRICES 1
#define C3D_BINDING_POINT_UBO_ECLAIRAGE_2SI 2


typedef struct
{
    float mat_projection[16];
    float mat_modelView[16];
    float mat_modelViewProjection[16];
    float mat_normale[12];

} shaders_matrices;

//Eclairage de base, deux sources directionnelles (lumière directe et clair-obscure)

typedef struct
{
    float lumiere_ambiante[4];
    float couleur_brouillard[4];
    float brouillard_min;
    float brouillard_max;
    float align0;
    float align1;

    float l1_direction[4];
    float l1_couleur[4];
    float l1_ambiance[4];

    float l2_direction[4];
    float l2_couleur[4];
    float l2_ambiance[4];

} shaders_eclairage_2si;


typedef struct
{
    float lumiere_ambiante[4];
    float couleur_brouillard[4];
    float brouillard_min;
    float brouillard_max;
    float align0;
    float align1;

    float l1_position[4];
    float l1_couleur[4];
    float l1_ambiance[4];

    float l2_position[4];
    float l2_couleur[4];
    float l2_ambiance[4];

    float l3_position[4];
    float l3_couleur[4];
    float l3_ambiance[4];

} shaders_eclairage_3sp;


class Contexte3d
{
    public:
        static shaders_matrices ubo_matrices;
        static GLuint ubo_matrices_id;

        static shaders_eclairage_2si ubo_eclairage2si;
        static GLuint ubo_eclairage2si_id;

        static shaders_eclairage_3sp ubo_eclairage3sp;
        static GLuint ubo_eclairage3sp_id;


        uint16_t erreur;


        Repere obs;
        float zMin;
        float zMax;
        int largeur_ecran;
        int hauteur_ecran;
        float ratio_ecran;  // hauteur/largeur

        //Profondeur de champ:
        float zMin_nette;
        float zMax_nette;


        glm::mat4 matrice_projection;
        glm::mat4 matrice_observateur;
        glm::mat4 matrice_modelView;
        glm::mat4 matrice_modelViewProjection;
        glm::mat3 matrice_normaleObservateur;   //glm::inverseTranspose(glm::mat3(GVars::matrice_modelView)
        glm::mat3 matrice_normaleModel;   //glm::inverseTranspose(glm::mat3(GVars::matrice_modelView)
        float Fov;
        float distanceFocale;

        float couleur_fond[4];
        float lumiere_ambiante[4];
        float couleur_brouillard[4];
        float brouillard_min;
        float brouillard_max;


        Source_Lumineuse* lumieres[C3D_MAX_LUMIERES];
    private:
        //Tempons de travail:
        glm::vec3 l_direction;
        float l_c[4];


    private:
        void maj_source_infinie(int indice);

    public:

        static bool init_ubos(); //Appeler au début de la démo
        static void detruit_ubos(); //Appeler à la fin de la démo

        Contexte3d();
        ~Contexte3d();

        void determine_couleur_fond(float r, float v, float b, float a);
        void determine_lumiere_ambiante(float r, float v, float b, float a);
        void determine_couleur_brouillard(float r, float v, float b, float a);
        void determine_limites_brouillard(float b_zmin, float b_zmax);

        void determine_Fov(float p_Fov);
        void maj_distanceFocale();
        void determine_taille_ecran(int largeur, int hauteur);
        void determine_profondeur(float p_zMin, float p_zMax);
        void determine_profondeur_de_champ(float p_zMin, float p_zMax);
        void calcul_matrice_projection();
        void calcul_matrice_observateur();
        void calcul_matrice_modelView(glm::mat4 &matrice_objet);

        void maj_ubo_matrices();    //Rafraichissement des données des blocs Uniform
        void maj_ubo_eclairage2si();
        void maj_ubo_eclairage3sp();

};
#endif // CONTEXTE3D_H_INCLUDED
