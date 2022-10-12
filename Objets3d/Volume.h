#ifndef VOLUME_H_INCLUDED
#define VOLUME_H_INCLUDED


#include "../glew.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <string>
#include "math.h"

#include "AffichagesDebogue.h"
#include "../chargeurs_images/Image.h"
#include "Vecteur.h"
#include "Face.h"
#include "Texture.h"
#include "Surface.h"
#include "../Lumieres.h"
#include "../repere.h"
#include "Shaders3d.h"

#define VOLUME_EPSILON 1e-8

//Codes d'erreurs, à récupérer dans Volume.erreur:
#define VOLUME_OK 0
#define VOLUME_ERREUR_VOLUME 1
#define VOLUME_ERREUR_MEMOIRE 2
#define VOLUME_ERREUR_SURFACE 3
#define VOLUME_ERREUR_LIEN_FACE_SURFACE 4
#define VOLUME_ERREUR_FACES 5
#define VOLUME_ERREUR_VBOS 6

//Drapeaux pour la libération de la mémoire lors de la destruction:
#define VOLUME_LIBERE_FACES           0x00000001
#define VOLUME_LIBERE_BOITE_COLLISION 0x00000002
#define VOLUME_LIBERE_SOMMETS         0x00000004
#define VOLUME_LIBERE_SURFACES        0x00000008


//Modes de remplissage des faces:
#define VOLUME_REMPLISSAGE_SOMMETS 1     //N'affiche que les sommets du volume
#define VOLUME_REMPLISSAGE_ARETES 2      //Affiche seulement les arètes
#define VOLUME_REMPLISSAGE_COULEUR 3     //Affiche en aplats avec les couleurs de diffusion des surfaces
#define VOLUME_REMPLISSAGE_SURFACES 4    //Affiche avec les modes de rendus définis dans les surfaces


//Sert pour les calculs de repères TBN des textures:
#define VOLUME_FACE_INCLUSE 1
#define VOLUME_FACE_FRONTALIERE 2

//Drapeau de débogage:

#define VOLUME_DEBUG false
#define VOLUME_DEBUG_NORMALES_SOMMETS false
#define VOLUME_DEBUG_SURFACES false
#define VOLUME_DEBUG_DUREES false
#define VOLUME_DEBUG_FACES_TRANSPARENTES false
#define VOLUME_DEBUG_TEXTURES false

#define VOLUME_DEBUG_AFFICHE_NORMALES false
#define VOLUME_DEBUG_AFFICHE_REPERES_TBN_FACES false
#define VOLUME_DEBUG_AFFICHE_REPERES_TBN_SOMMETS false
#define VOLUME_DEBUG_AFFICHE_NORMALES_SOMMETS_FACES false
#define VOLUME_DEBUG_AFFICHE_REPERES_TBN_TEXTURES_RUGOSITE false

//Structure d'une liaison de faces:
//La liaison peut servir pour les sommets ou les arêtes.

class liaison_faces
{
    public:
        vector<uint32_t> indices_faces;
};



//===========================================================================================
//                          DEFINITION DE LA CLASSE VOLUME
//===========================================================================================

class Volume : public Repere
{
     public:
        bool drapeau_affiche;

        char nom[256];
        bool drapeau_debug;
        uint16_t erreur; //Code d'erreur en cas de problème
        float taille_points_sommets;    //Taille des sommets en mode de remplissage SOMMETS
        uint16_t mode_remplissage;

        //Sommets relatifs au volume:

		GLfloat* sommets;      //Pointe sur la table de sommets utilisée pour le rendu.
                                //Si le moteur est en mode TRANSFORMATIONS_MATRICES, ce sont les sommets
                                //relatifs qui sont utilisés. Si le moteur est en mode TRANSFORMATIONS_REPERES, les
                                //sommets absolus sont utilisés.
                                //Par défaut, ce sont les sommets relatifs qui sont utilisés.

        uint32_t nbrSommets;
		uint32_t nbrFaces;
		uint32_t nbrAretes;
		Face** faces;
		vector<uint32_t> aretes;    //La table des aretes du volume. Chaque arête est définie par un binôme d'indexes de sommets.

		float echelle_geometrique; //Facteur multiplicateur des sommets du volume
        float facteur_luminescence; //Facteur de luminescence général.


        //Tables de liaisons. Ces informations peuvent servir.
		//vector<uint32_t> aretes; //Tableau qui peut être généré à partir des faces avec "initialisation_aretes()"
        //vector<liaison_faces> liaisons_aretes_faces; //Vecteur de vecteurs d'indices de faces
        vector<liaison_faces> liaisons_sommets_faces; //Vecteur de vecteurs d'indices de faces
        GLfloat* normales_sommets; //Normales relatives au volume des sommets

        //Tables des repères orientés des faces:
        GLfloat* tangentes_sommets;
        GLfloat* binormales_sommets;

        //La table des surfaces.
		uint32_t nbrSurfaces;
		Surface** surfaces;
		uint32_t nbrSurfaces_transparentes;
		vector<Surface*> surfaces_transparentes;

		//Surface en cours
		Surface* surface_courante; //Surface en cours de rendu
		float materiau_ambiance[4];
		float materiau_diffusion[4];
		float materiau_luminosite[4];
		float materiau_specularite[4];

        //Données géométriques:
        float centre_geometrique[3];

        //Couleur des sommets en mode de remplissage SOMMETS
        float couleur_sommets[3];
        float couleur_aretes[3];

        //Paramètres de l'observateur lors de l'affichage, dans le repère du volume:
        float* observateur_position;

    protected:

        double epsilon; //Sert pour le calcul de planitude des faces. Il est différent de la constante globale EPSILON
        //Gestion mémoire:
        uint32_t drapeaux_liberation_memoire;

        GLfloat noir[4];

        double duree_rendu;

        GLint sens_polygones;   //Sens dans lequel sont définies les faces avant des polygones (GL_CW ou GL_CCW)

        GLint nbr_attributs_sommets_max; //Le nombre maximum d'attributs supplémentaires par sommet

        bool drapeau_affecte_par_brouillard;    //True par défaut: le volume est affecté par le brouillard.

        //Pointeurs sur les fonctions de rendu.
        void (Volume::*rendus[SURFACE_NBR_RENDUS])();

//========================================================================================================
//==================================== Constructeur du volume:
//========================================================================================================
    public:
        static bool erreur_openGl(const char* message);

        Volume( char* p_nom,
                uint32_t p_drapeaux_liberation_memoire=0,
                float px = 0., float py = 0., float pz = 0.,
                float pr=0.,float pt=0.,float pl=0.,
                GLfloat* pSommets = NULL,
                uint32_t p_nbrSommets=0,
                Face** pFaces = NULL,
                uint32_t p_nbrFaces=0,
                Surface** pSurfaces = NULL,
                uint32_t p_nbrSurfaces=0);

    private:
        void copie_nom(char* p_nom);

    public:
        Volume(Volume& vol_org);
        virtual ~Volume();
        void supprime_surfaces();
        void supprime_faces();
        void supprime_sommets();
		void supprime_aretes();
        void initRVBA(float* couleur,float r, float v, float b, float a=1.);
        void initRVB(float* couleur,float r, float v, float b);
        bool determine_sommets(GLfloat* pSommets,uint32_t p_nbrSommets);
        void determine_echelle_geometrique(float e);
        bool determine_faces(Face** pFaces,uint32_t p_nbrFaces);
        bool determine_surfaces(Surface** pSurfaces, uint32_t p_nbrSurfaces);
        void calcul_normales_sommets(uint32_t indice_depart=0,uint32_t nbr=0);
        void calcul_tangentes_binormales_sommets();
        void calcul_centre_geometrique();
        bool initialise_faces();
        void maj_faces();
        void initialise_faces_transparentes();
        void calcul_normale_face(Face* face);
        void calcul_normales_faces_CCW(uint32_t indice_depart=0, uint32_t nbr=0);
        void calcul_normales_faces_CW(uint32_t indice_depart=0, uint32_t nbr=0);
        void calcule_normales_sommets_faces();
        void calcul_tangente_binormale_face(Face* face);
        void calcul_centre_gravite_face(Face* face);
        void initialise_liaisons_sommets_faces();
        void ajoute_liaison_sommet();
        bool creation_aretes();
        bool initialise_surfaces();
        bool initialise_liens_faces_surfaces();
        void initialise_indices_sommets_surfaces();
        void calcule_normales_sommets_surfaces();
        void initialise_modes_rendus_surfaces();
        bool initialise_textures();
        bool creation_table_faces_texture(Texture* texture, vector<uint8_t> &faces_utilisees);
        bool creation_tableau_UV(Texture* texture);
        bool creation_tangentes_binormales(Texture* texture,vector<uint8_t>& faces_utilisees);

        bool creation_VBOs();   //Fonction finale, les vbos propres à chaque surface sont créés.
        bool maj_VBOs();

        Surface* renvoie_surface_nom(char* nom_surface);
        Surface* renvoie_surface(uint32_t indice);
        float* renvoie_sommets();
        Face* renvoie_face(uint32_t indice);
        float* renvoie_normale_sommet(uint32_t indice);
        void determine_couleur_ambiance(uint32_t couleur, float intensite=1.);
        void determine_couleur_diffusion(uint32_t couleur, float intensite=1.);
        void determine_couleur_specularite(uint32_t couleur, float intensite=1.);
        void determine_couleur_luminescence(uint32_t couleur, float intensite=1.);
        void determine_eclairage(bool drapeau);   //active/désactive l'éclairage pour toutes les surfaces
        void determine_eclairage_phong(bool drapeau);   //L'éclairage Phong est appliqué à toutes les surfaces
        void applique_teinte_diffusion(uint32_t teinte);
        void applique_teinte_specularite(uint32_t teinte);
        void applique_teinte_luminescence(uint32_t teinte);
        void applique_teinte_ambiance(uint32_t teinte);
        void determine_affecte_par_brouillard(bool p_drapeau);
        bool est_affecte_par_brouillard();
        void determine_sens_polygones(GLint p_sens);
        GLint renvoie_sens_polygones();
        void inverse_sens_polygones();
        void determine_mode_remplissage(uint8_t p_mode);
		void determine_position(float px, float py, float pz);
		void determine_position(float position[3]);
        void determine_RTL(float p_roulis, float p_tangage, float p_lacet);
		void determine_RTL(float rtl[3]);
		//void determine_echelle(float pex, float pey, float pez);
		//void determine_echelle(float pe[3]);
        void initialise_materiau();

    protected:
        void initialise_materiau_luminescent(Surface* surface);



    public:

        void desactive_textures();
        void affiche();
        void affiche_couleurs_luminescentes();

    protected:

        void initialise_pointeurs_rendus();

        void rendu_surfaces_aplats();
        void rendu_surfaces();
        void rendu_surfaces_luminescentes();
        void rendu_aplats();
        void rendu_aplats_Gouraud();
        void rendu_aplats_Phong();
        void rendu_texture();
        void rendu_texture_Gouraud();
        void rendu_texture_Phong();
        void rendu_rugosite();
        void rendu_rugosite_texture();

};

#endif // VOLUME_H_INCLUDED
