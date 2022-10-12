#ifndef SHADERSDEMO_H_INCLUDED
#define SHADERSDEMO_H_INCLUDED

#include "GestionShaders.h"
#include "Variables.h"

class ShadersDemo :public GestionShaders
{
    public:
        //---------------- Shader d'affichage des textures de rendu:
        static GLchar* source_vertex_affTex;
        static GLchar* source_fragment_affTex;
        static GLuint shader_affTex_id1;
        static GLuint shader_affTex_id2;
        static GLuint programme_affTex;

        static GLint attrib_affTex_position;
        static GLint uniform_affTex_texture;

        //---------------- Shader d'affichage des bitmaps pour le développement:
        static GLchar* source_vertex_affTex_bitmap;
        static GLchar* source_fragment_affTex_bitmap;
        static GLuint shader_affTex_bitmap_id1;
        static GLuint shader_affTex_bitmap_id2;
        static GLuint programme_affTex_bitmap;

        static GLint attrib_affTex_bitmap_position;
        static GLint uniform_affTex_bitmap_texture;
        static GLint uniform_afftex_bitmap_echelle;
        static GLint uniform_afftex_bitmap_taille_ecran;
        static GLint uniform_afftex_bitmap_taille_texture;
        static GLint uniform_afftex_bitmap_position_bitmap;

        //---------------- Shader d'affichage des textures de rendu avec zBuffer:
        static GLchar* source_vertex_affTex_zBuffer;
        static GLchar* source_fragment_affTex_zBuffer;
        static GLuint shader_affTex_zBuffer_id1;
        static GLuint shader_affTex_zBuffer_id2;
        static GLuint programme_affTex_zBuffer;

        static GLint attrib_affTex_zBuffer_position;
        static GLint uniform_affTex_zBuffer_texture_couleur;
        static GLint uniform_affTex_zBuffer_texture_zBuffer;
        static GLint uniform_affTex_zBuffer_taille_ecran;
        static GLint uniform_affTex_zBuffer_taille_tramage;

        //---------------- Shader de profondeur de champ:
        static GLchar* source_vertex_profChamp;
        static GLchar* source_fragment_profChamp;
        static GLuint shader_profChamp_id1;
        static GLuint shader_profChamp_id2;
        static GLuint programme_profChamp;

        static GLint attrib_profChamp_position;
        static GLint uniform_profChamp_texture_couleur;
        static GLint uniform_profChamp_texture_zBuffer;
        static GLint uniform_profChamp_taille_ecran;
        static GLint uniform_profChamp_zMin;
        static GLint uniform_profChamp_zMax;

        static GLint uniform_profChamp_zMin_nette;
        static GLint uniform_profChamp_zMax_nette;
        static GLint uniform_profChamp_ratio_ecran;
        static GLint uniform_profChamp_distance_focale;

        //---------------- Shader de précalc des cônes de relief pour les textures de relief:
        static GLchar* source_vertex_relief_genCones;
        static GLchar* source_fragment_relief_genCones;
        static GLuint shader_relief_genCones_id1;
        static GLuint shader_relief_genCones_id2;
        static GLuint programme_relief_genCones;

        static GLint attrib_relief_genCones_position;
        static GLint uniform_relief_genCones_texture_profondeurs;
        static GLint uniform_relief_genCones_taille_texture_sortie;
        static GLint uniform_relief_genCones_echelle_profondeur;


        //---------------- Shader de la fractale 2d

        static GLchar* source_vertex_mb;
        static GLchar* source_fragment_mb;
        static GLuint shader_mb_id1;
        static GLuint shader_mb_id2;
        static GLuint programme_mb;

        static GLint uniform_mb_matrice_projection;
        static GLint attrib_mb_position;
        static GLint uniform_mb_zoom;
        static GLint uniform_mb_xpos;
        static GLint uniform_mb_ypos;
        static GLint uniform_mb_alpha;
        static GLint uniform_mb_puissance;
        static GLint uniform_mb_ecranDim;
        static GLint uniform_mb_juliaC;
        static GLint uniform_mb_iter;


        //---------------- Shader du raymarching fractales

        static GLchar* source_vertex_rm;
        static GLchar* source_fragment_rm;
        static GLuint shader_rm_id1;
        static GLuint shader_rm_id2;
        static GLuint programme_rm;

        static GLint attrib_rm_position;
        static GLint uniform_rm_ratio_ecran;
        static GLint uniform_rm_distanceFocale;
        static GLint uniform_rm_obs_pos;
        static GLint uniform_rm_obsMat_normale;
        static GLint uniform_rm_t;
        static GLint uniform_rm_iter;
        static GLint uniform_rm_l1_position;
        static GLint uniform_rm_l2_position;
        static GLint uniform_rm_l3_position;
        static GLint uniform_rm_zMin;
        static GLint uniform_rm_zMax;
        static GLint uniform_rm_fogZmin;
        static GLint uniform_rm_fogZmax;

        //---------------- Shader du raymarching scene gare

        static GLchar* source_vertex_rmGare;
        static GLchar* source_fragment_rmGare;
        static GLuint shader_rmGare_id1;
        static GLuint shader_rmGare_id2;
        static GLuint programme_rmGare;

        static GLint attrib_rmGare_position;
        static GLint uniform_block_rmGare_eclairages;

        static GLint uniform_rmGare_ratio_ecran;
        static GLint uniform_rmGare_distanceFocale;
        static GLint uniform_rmGare_obs_pos;
        static GLint uniform_rmGare_obsMat_normale;
        static GLint uniform_rmGare_t;
        static GLint uniform_rmGare_iter;

        static GLint uniform_rmGare_zMin;
        static GLint uniform_rmGare_zMax;

        //---------------- Shader du raymarching de terrain

        static GLchar* source_vertex_rmt;
        static GLchar* source_fragment_rmt;
        static GLuint shader_rmt_id1;
        static GLuint shader_rmt_id2;
        static GLuint programme_rmt;

        static GLint attrib_rmt_position;
        static GLint uniform_rmt_ratio_ecran;
        static GLint uniform_rmt_distanceFocale;
        static GLint uniform_rmt_obs_pos;
        static GLint uniform_rmt_obsMat_normale;
        static GLint uniform_rmt_zMin;
        static GLint uniform_rmt_zMax;
        static GLint uniform_rmt_texture_terrain;
        static GLint uniform_rmt_texture_terrain2;
        static GLint uniform_rmt_texture_terrain3;
        static GLint uniform_rmt_facteur_echelle_terrain;
        static GLint uniform_rmt_facteur_echelle_terrain2;
        static GLint uniform_rmt_facteur_echelle_terrain3;
        static GLint uniform_rmt_amplitude_terrain;
        static GLint uniform_rmt_amplitude_terrain2;
        static GLint uniform_rmt_amplitude_terrain3;
        static GLint uniform_rmt_facteur_precision_distance;
        static GLint uniform_block_rmt_eclairages;

        //---------------- Shader des nuages

        static GLchar* source_vertex_nuages;
        static GLchar* source_fragment_nuages;
        static GLuint shader_nuages_id1;
        static GLuint shader_nuages_id2;
        static GLuint programme_nuages;

        static GLint attrib_nuages_position;
        static GLint uniform_block_nuages_eclairages;
        static GLint uniform_block_nuages_matrices;
        static GLint uniform_nuages_zMin;
        static GLint uniform_nuages_zMax;
        static GLint uniform_nuages_obsPos;
        static GLint uniform_nuages_texture_o1;
        static GLint uniform_nuages_texture_o2;
        static GLint uniform_nuages_texture_o3;
        static GLint uniform_nuages_t;

    public:
        static bool init_shader_affTex();
        static bool init_shader_affTex_bitmap();
        static bool init_shader_affTex_zBuffer();
        static bool init_shader_profondeur_de_champ();
        static bool init_shader_relief_genCones();

        static bool init_shader_mb();
        static bool init_shader_rm();
        static bool init_shader_rmGare();
        static bool init_shader_rmt();
        static bool init_shader_nuages();

        static bool initShaders();
        static void supprime_shader(GLuint programme, GLchar* source_vertex, GLchar* source_fragment,GLuint shader_id1,GLuint shader_id2 );
        static void freeShaders();
};

#endif // SHADERSDEMO_H_INCLUDED
