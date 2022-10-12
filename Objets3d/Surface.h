#ifndef SURFACE_H_INCLUDED
#define SURFACE_H_INCLUDED



/// Les textures de la surface sont détruites par celle-ci !!!

//----------------------------------
//  Structure des surfaces:
//
//  Une surface ne peut être rattachée qu'à un seul volume.
//
// Le nombre maximum de textures prises en compte dépend de l'implémentation de gestion du mode "Multitextures" d'OpenGl.
// En cas de nombre de textures supérieur au nombre maximum de textures max d'OpenGl, un ordre de priorité est appliqué,
// correspondant à l'ordre de définition des vecteurs de textures ci-après.
//
/// L'utilisation des VBOs oblige à avoir le même nombre de sommets par face...
//
//----------------------------------

#include "Texture.h"
#include "Face.h"
#include <list>

#define SURFACE_OK 0
#define SURFACE_ERREUR_VBO_NUM_SOMMETS 1    //Les faces n'ont pas toutes le même nombre de sommets.
#define SURFACE_ERREUR_VBO 2

//--------------------------------------------
//      Constantes de gestion des rendus
/// Il y a des rendus impossibles: ex: Aplat Gouraud Phong. Les codes correspondants sont inutilisés.
//--------------------------------------------

#define SURFACE_NBR_RENDUS 0b1111

//Bits de rendu:
#define SURFACE_BIT_RENDU_APLAT 0
#define SURFACE_BIT_RENDU_TEXTURE 1
#define SURFACE_BIT_RENDU_GOURAUD 2
#define SURFACE_BIT_RENDU_PHONG 3

#define SURFACE_MASK_RENDU_APLAT (1<<SURFACE_BIT_RENDU_APLAT)
#define SURFACE_MASK_RENDU_TEXTURE (1<<SURFACE_BIT_RENDU_TEXTURE)
#define SURFACE_MASK_RENDU_GOURAUD (1<<SURFACE_BIT_RENDU_GOURAUD)
#define SURFACE_MASK_RENDU_PHONG (1<<SURFACE_BIT_RENDU_PHONG)

//Modes de rendu des surfaces:
#define SURFACE_RENDU_APLAT             SURFACE_MASK_RENDU_APLAT
#define SURFACE_RENDU_APLAT_GOURAUD     SURFACE_MASK_RENDU_APLAT | SURFACE_MASK_RENDU_GOURAUD
#define SURFACE_RENDU_APLAT_PHONG       SURFACE_MASK_RENDU_APLAT | SURFACE_MASK_RENDU_PHONG
#define SURFACE_RENDU_TEXTURE           SURFACE_MASK_RENDU_TEXTURE
#define SURFACE_RENDU_TEXTURE_GOURAUD   SURFACE_MASK_RENDU_TEXTURE | SURFACE_MASK_RENDU_GOURAUD
#define SURFACE_RENDU_TEXTURE_PHONG     SURFACE_MASK_RENDU_TEXTURE | SURFACE_MASK_RENDU_PHONG



//=======================================================================================
//
//=======================================================================================

class Surface
{
    private:
        uint16_t bits_rendus;   //Drapeaux constituant le code du shader utilisé pour la surface
                                //Le mode de rendu est généré à partir de ces bits, car il y a parfois des adaptations à faire (shaders incompatibles)
    public:
        uint16_t erreur;
        char nom[256];
        Volume* volume;  //Le volume auquel appartient la surface
        uint16_t mode_rendu;    //Code du renddksEffetSpecial* effet_special;
        bool drapeau_recoit_ombres;  //True si la surface peut recevoir des ombres portées.
        bool drapeau_projete_ombres;

        bool double_face;   //Affiche les deux côtés des faces
        bool drapeau_adoucissement; //True si les faces sont adoucies
        bool drapeau_transparence;  //Champ initialisé par le volume - True si la surface est transparente.
        bool drapeau_phong; //True: La surface est éclairée en phong. False: éclairage Gouraud
        bool drapeau_eclairage; //True si la surface gère l'éclairage.

        float angle_limite_adoucissement;   //en radians
        GLfloat* normales_sommets; //Table utilisée pour le mode de rendu par tableaux de pointeurs
        uint32_t nbrNormales;

        float couleur_diffusion[4];
        float couleur_speculaire[4];
        float couleur_ambiance[4]; //Réflexion de la lumière ambiante
        float couleur_luminescence[4]; //Luminescence de la surface

        float luminescence;
        float diffusion;
        float ambiance;
        float specularite;

        float brillance;    //=Shininess
        float transparence;
        float transparence_speculaire;
        float translucidite; //Comment le gérer?
        float reflexion;// Gérée par les shaders
        float rugosite; //"Bump", géré par les shaders

        //Effet d'ondulations aqueuses
        float intensite_ondes;  //Mettre à 0 pour désactiver l'effet
        float amplitude_ondes;  //défaut: 0.2
        float frequence_ondes;  //défaut: 2
        float vitesse_ondes;    //défaut: 1
        float quantite_ondes;   //défaut: 5
        float perturbation_ondes; //défaut: 10
        float echelle_perturbation_ondes; //défaut: 10

        //Table globale des textures:
        vector<Texture*> textures;

        //Textures actives:
        vector<Texture*> textures_couleur;       //Image RVB,ARVB,Gris
        vector<Texture*> textures_diffusion;     //Image en niveaux de gris
        vector<Texture*> textures_ambiance;      //NG
        vector<Texture*> textures_luminescence;   //NG
        vector<Texture*> textures_transparence;  //NG
        vector<Texture*> textures_reflexion;     //NG
        vector<Texture*> textures_rugosite;      //Plusieurs modes de rugosité possibles: par décallage (NG) ou déviation de normales (RVB, ARVB)

        //Textures inactives:
        vector<Texture*> textures_couleur_inactives;
        vector<Texture*> textures_diffusion_inactives;
        vector<Texture*> textures_ambiance_inactives;
        vector<Texture*> textures_luminescence_inactives;
        vector<Texture*> textures_transparence_inactives;
        vector<Texture*> textures_reflexion_inactives;
        vector<Texture*> textures_rugosite_inactives;

        //Sommets liés à la surface: sert pour les générateurs de cooronnées UV de textures - table initialisée par Volume::initialise_indices_sommets_surfaces()
        vector<uint32_t> indices_sommets;
        //faces liées à la surface: Ces champs sont remplis automatiquement par le volume.
        uint32_t nbr_faces;
        vector<Face*> faces;

        vector<Face*> faces_adoucies;
        vector<Face*> faces_partiellement_adoucies; //Les faces dont une ou plusieurs arêtes forment un angle > à l'angle d'adoucissement.

        //Gestion vbo:
        uint32_t taille_sommet_vbo; //Nombre de float par sommet
        uint16_t nbr_sommets_par_face;  //A cause des VBO, toutes les faces doivent avoir le même nombre de sommets (3 ou 4)
        GLuint vao;
        GLuint vbo;
        uint32_t taille_tableau_vbo; //taille en octets du tableau
        float* tableau_sommets_vbo; //Les données envoyées au VBO

    public:
        static bool erreur_openGl(const char* message);

        Surface(char* p_nom=NULL);
        Surface(Surface& surface_originale);
        ~Surface();
        void initRVBA(float* couleur,float r, float v, float b, float a=1.);
        void determine_couleur_ambiance(uint32_t couleur, float intensite=0.2);
        void determine_couleur_diffusion(uint32_t couleur, float intensite=1.);
        void determine_couleur_speculaire(uint32_t couleur, float intensite=0.5);
        void determine_couleur_luminescence(uint32_t couleur, float intensite=0.2);
        void determine_ambiance(float intensite);
        void determine_diffusion(float intensite);
        void determine_specularite(float intensite);
        void determine_luminescence(float intensite);
        void determine_transparence(float p_transparence);
        void determine_reflexion(float p_reflexion);
        void determine_transparence_speculaire(float p_transparence);
        void determine_intensite_ondes(float p_intensite);
        void applique_teinte_diffusion(uint32_t teinte);
        void applique_teinte_specularite(uint32_t teinte);
        void applique_teinte_luminescence(uint32_t teinte);
        void applique_teinte_ambiance(uint32_t teinte);
        Texture* renvoie_texture_nom(char* nom_texture);
        void ajoute_texture(Texture* texture);
        bool texture_est_active(Texture* texture);
        uint16_t renvoie_texture_active_index(Texture* texture);
        uint16_t renvoie_texture_inactive_index(Texture* texture);
        void desactive_texture(Texture* texture);
        void active_texture(Texture* texture);
        void active_toutes_les_textures();
        void rafraichi_faces_adoucies();
        void ajoute_face_adoucie(Face* face);
        void ajoute_face_partiellement_adoucie(Face* face);
        bool alloue_tableau_normales_sommets(uint32_t p_nbrSommets);
        void inverse_normales();
        void determine_phong(bool p_drapeau);
        void initialise_mode_rendu();
        void determine_recoit_ombres(bool d);
        void determine_projete_ombres(bool d);
        void determine_ombre_portees(bool d);
        void determine_eclairage(bool p_drapeau);

        //Gestion Vertex Buffer Object:
        bool creation_tableau_sommets_vbo(GLfloat* sommets); //Création du tableau des sommets avec leurs attributs
		bool init_vbo(GLenum usage=GL_STATIC_DRAW);
		void detruit_vbo();
		bool maj_vbo();
        void affiche_debug();


    private:
        void determine_couleur_ARVB(float* tab,uint32_t couleur);
        void applique_teinte_ARVB(float* tab, uint32_t teinte);
};




#endif // SURFACE_H_INCLUDED
