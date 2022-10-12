#include "ShadersDemo.h"

    //---------------- Shader d'affichage des textures de rendu:
    GLchar* ShadersDemo::source_vertex_affTex=NULL;
    GLchar* ShadersDemo::source_fragment_affTex=NULL;
    GLuint ShadersDemo::shader_affTex_id1=0;
    GLuint ShadersDemo::shader_affTex_id2=0;
    GLuint ShadersDemo::programme_affTex=0;

    GLint ShadersDemo::attrib_affTex_position;
    GLint ShadersDemo::uniform_affTex_texture;

    //---------------- Shader d'affichage des bitmaps pour le développement:
    GLchar* ShadersDemo::source_vertex_affTex_bitmap;
    GLchar* ShadersDemo::source_fragment_affTex_bitmap;
    GLuint ShadersDemo::shader_affTex_bitmap_id1=0;
    GLuint ShadersDemo::shader_affTex_bitmap_id2=0;
    GLuint ShadersDemo::programme_affTex_bitmap=0;

    GLint ShadersDemo::attrib_affTex_bitmap_position;
    GLint ShadersDemo::uniform_affTex_bitmap_texture;
    GLint ShadersDemo::uniform_afftex_bitmap_echelle;
    GLint ShadersDemo::uniform_afftex_bitmap_taille_ecran;
    GLint ShadersDemo::uniform_afftex_bitmap_taille_texture;
    GLint ShadersDemo::uniform_afftex_bitmap_position_bitmap;

    //---------------- Shader d'affichage des textures de rendu avec zBuffer:
    GLchar* ShadersDemo::source_vertex_affTex_zBuffer=NULL;
    GLchar* ShadersDemo::source_fragment_affTex_zBuffer=NULL;
    GLuint ShadersDemo::shader_affTex_zBuffer_id1=0;
    GLuint ShadersDemo::shader_affTex_zBuffer_id2=0;
    GLuint ShadersDemo::programme_affTex_zBuffer=0;

    GLint ShadersDemo::attrib_affTex_zBuffer_position;
    GLint ShadersDemo::uniform_affTex_zBuffer_texture_couleur;
    GLint ShadersDemo::uniform_affTex_zBuffer_texture_zBuffer;
    GLint ShadersDemo::uniform_affTex_zBuffer_taille_ecran;
    GLint ShadersDemo::uniform_affTex_zBuffer_taille_tramage;

    //---------------- Shader de profondeur de champ:
    GLchar* ShadersDemo::source_vertex_profChamp=NULL;
    GLchar* ShadersDemo::source_fragment_profChamp=NULL;
    GLuint ShadersDemo::shader_profChamp_id1=0;
    GLuint ShadersDemo::shader_profChamp_id2=0;
    GLuint ShadersDemo::programme_profChamp=0;

    GLint ShadersDemo::attrib_profChamp_position;
    GLint ShadersDemo::uniform_profChamp_texture_couleur;
    GLint ShadersDemo::uniform_profChamp_texture_zBuffer;
    GLint ShadersDemo::uniform_profChamp_taille_ecran;
    GLint ShadersDemo::uniform_profChamp_zMin;
    GLint ShadersDemo::uniform_profChamp_zMax;
    GLint ShadersDemo::uniform_profChamp_zMin_nette;
    GLint ShadersDemo::uniform_profChamp_zMax_nette;
    GLint ShadersDemo::uniform_profChamp_ratio_ecran;
    GLint ShadersDemo::uniform_profChamp_distance_focale;

    //---------------- Shader de précalc des cônes de relief pour les textures de relief:
    GLchar* ShadersDemo::source_vertex_relief_genCones;
    GLchar* ShadersDemo::source_fragment_relief_genCones;
    GLuint ShadersDemo::shader_relief_genCones_id1=0;
    GLuint ShadersDemo::shader_relief_genCones_id2=0;
    GLuint ShadersDemo::programme_relief_genCones=0;

    GLint ShadersDemo::attrib_relief_genCones_position;
    GLint ShadersDemo::uniform_relief_genCones_texture_profondeurs;
    GLint ShadersDemo::uniform_relief_genCones_taille_texture_sortie;
    GLint ShadersDemo::uniform_relief_genCones_echelle_profondeur;


    //---------------- Fractale 2d:
    GLchar* ShadersDemo::source_vertex_mb=NULL;
    GLchar* ShadersDemo::source_fragment_mb=NULL;
    GLuint ShadersDemo::shader_mb_id1=0;
    GLuint ShadersDemo::shader_mb_id2=0;
    GLuint ShadersDemo::programme_mb=0;

    GLint ShadersDemo::attrib_mb_position;
    GLint ShadersDemo::uniform_mb_matrice_projection;
    GLint ShadersDemo::uniform_mb_zoom;
    GLint ShadersDemo::uniform_mb_xpos;
    GLint ShadersDemo::uniform_mb_ypos;
    GLint ShadersDemo::uniform_mb_alpha;
    GLint ShadersDemo::uniform_mb_puissance;
    GLint ShadersDemo::uniform_mb_ecranDim;
    GLint ShadersDemo::uniform_mb_juliaC;
    GLint ShadersDemo::uniform_mb_iter;

    //---------------- Raymarching:
    GLchar* ShadersDemo::source_vertex_rm=NULL;
    GLchar* ShadersDemo::source_fragment_rm=NULL;
    GLuint ShadersDemo::shader_rm_id1=0;
    GLuint ShadersDemo::shader_rm_id2=0;
    GLuint ShadersDemo::programme_rm=0;

    GLint ShadersDemo::attrib_rm_position;
    GLint ShadersDemo::uniform_rm_ratio_ecran;
    GLint ShadersDemo::uniform_rm_obs_pos;
    GLint ShadersDemo::uniform_rm_obsMat_normale;
    GLint ShadersDemo::uniform_rm_distanceFocale;
    GLint ShadersDemo::uniform_rm_t;
    GLint ShadersDemo::uniform_rm_iter;
    GLint ShadersDemo::uniform_rm_l1_position;
    GLint ShadersDemo::uniform_rm_l2_position;
    GLint ShadersDemo::uniform_rm_l3_position;
    GLint ShadersDemo::uniform_rm_zMin;
    GLint ShadersDemo::uniform_rm_zMax;
    GLint ShadersDemo::uniform_rm_fogZmin;
    GLint ShadersDemo::uniform_rm_fogZmax;

    //---------------- Shader du raymarching scene gare

    GLchar* ShadersDemo::source_vertex_rmGare;
    GLchar* ShadersDemo::source_fragment_rmGare;
    GLuint ShadersDemo::shader_rmGare_id1;
    GLuint ShadersDemo::shader_rmGare_id2;
    GLuint ShadersDemo::programme_rmGare;

    GLint ShadersDemo::attrib_rmGare_position;
    GLint ShadersDemo::uniform_block_rmGare_eclairages;

    GLint ShadersDemo::uniform_rmGare_ratio_ecran;
    GLint ShadersDemo::uniform_rmGare_distanceFocale;
    GLint ShadersDemo::uniform_rmGare_obs_pos;
    GLint ShadersDemo::uniform_rmGare_obsMat_normale;
    GLint ShadersDemo::uniform_rmGare_t;
    GLint ShadersDemo::uniform_rmGare_iter;

    GLint ShadersDemo::uniform_rmGare_zMin;
    GLint ShadersDemo::uniform_rmGare_zMax;

    //---------------- Shader du raymarching de terrain

   GLchar* ShadersDemo::source_vertex_rmt;
   GLchar* ShadersDemo::source_fragment_rmt;
   GLuint ShadersDemo::shader_rmt_id1=0;
   GLuint ShadersDemo::shader_rmt_id2=0;
   GLuint ShadersDemo::programme_rmt=0;

   GLint ShadersDemo::attrib_rmt_position;
   GLint ShadersDemo::uniform_rmt_ratio_ecran;
   GLint ShadersDemo::uniform_rmt_distanceFocale;
   GLint ShadersDemo::uniform_rmt_obs_pos;
   GLint ShadersDemo::uniform_rmt_obsMat_normale;
   GLint ShadersDemo::uniform_rmt_zMin;
   GLint ShadersDemo::uniform_rmt_zMax;
   GLint ShadersDemo::uniform_rmt_texture_terrain;
   GLint ShadersDemo::uniform_rmt_texture_terrain2;
   GLint ShadersDemo::uniform_rmt_texture_terrain3;
   GLint ShadersDemo::uniform_rmt_facteur_echelle_terrain;
   GLint ShadersDemo::uniform_rmt_facteur_echelle_terrain2;
   GLint ShadersDemo::uniform_rmt_facteur_echelle_terrain3;
   GLint ShadersDemo::uniform_rmt_amplitude_terrain;
   GLint ShadersDemo::uniform_rmt_amplitude_terrain2;
   GLint ShadersDemo::uniform_rmt_amplitude_terrain3;
   GLint ShadersDemo::uniform_rmt_facteur_precision_distance;
   GLint ShadersDemo::uniform_block_rmt_eclairages;

    //---------------- Shader des nuages

    GLchar* ShadersDemo::source_vertex_nuages;
    GLchar* ShadersDemo::source_fragment_nuages;
    GLuint ShadersDemo::shader_nuages_id1;
    GLuint ShadersDemo::shader_nuages_id2;
    GLuint ShadersDemo::programme_nuages;

    GLint ShadersDemo::attrib_nuages_position;
    GLint ShadersDemo::uniform_block_nuages_eclairages;
    GLint ShadersDemo::uniform_block_nuages_matrices;
    GLint ShadersDemo::uniform_nuages_zMin;
    GLint ShadersDemo::uniform_nuages_zMax;
    GLint ShadersDemo::uniform_nuages_obsPos;
    GLint ShadersDemo::uniform_nuages_texture_o1;
    GLint ShadersDemo::uniform_nuages_texture_o2;
    GLint ShadersDemo::uniform_nuages_texture_o3;
    GLint ShadersDemo::uniform_nuages_t;



    //--------------------------------------------------------------
    //      Création des shaders globaux de la démo
    //--------------------------------------------------------------

    bool ShadersDemo::init_shader_affTex()
	{
		 //---------- Charge les sources des shaders:

                source_vertex_affTex=charge_source_shader("shaders/affiche_texture_vertex.glsl");
                source_fragment_affTex=charge_source_shader("shaders/affiche_texture_fragment.glsl");
                if (source_fragment_affTex==NULL || source_vertex_affTex==NULL) return false;

                 //---------- Création des shaders:
                shader_affTex_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_affTex_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_affTex_id1==0 || shader_affTex_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader affiche_texture"<<endl;
                        return false;
                }


                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_affTex};

                glShaderSource(shader_affTex_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_affTex;
                glShaderSource(shader_affTex_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:

                if (!compile_shader(shader_affTex_id1)) return false;
                if (!compile_shader(shader_affTex_id2)) return false;


                //----------- Création des programmes
                programme_affTex=glCreateProgram();
                if (programme_affTex==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - affiche_texture"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_affTex,shader_affTex_id1);
                glAttachShader(programme_affTex,shader_affTex_id2);

                //------------ Déclaration du buffer de sortie du fragment shader:
                //if(GVars::openGl_majorVersion>=3 && GVars::openGl_minorVersion>=3)glBindFragDataLocation(programme_mb,0,"outColor");

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_affTex)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_affTex)) return false;

                //----------- Création des liens avec les variables:

                attrib_affTex_position=glGetAttribLocation(programme_affTex,"position");
                uniform_affTex_texture=glGetUniformLocation(programme_affTex,"texture_couleur");

                return true;
	}

	bool ShadersDemo::init_shader_affTex_bitmap()
	{
		 //---------- Charge les sources des shaders:

                source_vertex_affTex_bitmap=charge_source_shader("shaders/affiche_bitmap_vertex.glsl");
                source_fragment_affTex_bitmap=charge_source_shader("shaders/affiche_bitmap_fragment.glsl");
                if (source_fragment_affTex_bitmap==NULL || source_vertex_affTex_bitmap==NULL) return false;

                 //---------- Création des shaders:
                shader_affTex_bitmap_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_affTex_bitmap_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_affTex_bitmap_id1==0 || shader_affTex_bitmap_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader affiche_bitmap"<<endl;
                        return false;
                }


                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_affTex_bitmap};

                glShaderSource(shader_affTex_bitmap_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_affTex_bitmap;
                glShaderSource(shader_affTex_bitmap_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:

                if (!compile_shader(shader_affTex_bitmap_id1)) return false;
                if (!compile_shader(shader_affTex_bitmap_id2)) return false;


                //----------- Création des programmes
                programme_affTex_bitmap=glCreateProgram();
                if (programme_affTex_bitmap==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - affiche_bitmap"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_affTex_bitmap,shader_affTex_bitmap_id1);
                glAttachShader(programme_affTex_bitmap,shader_affTex_bitmap_id2);

                //------------ Déclaration du buffer de sortie du fragment shader:
                //if(GVars::openGl_majorVersion>=3 && GVars::openGl_minorVersion>=3)glBindFragDataLocation(programme_mb,0,"outColor");

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_affTex_bitmap)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_affTex_bitmap)) return false;

                //----------- Création des liens avec les variables:

                attrib_affTex_bitmap_position=glGetAttribLocation(programme_affTex_bitmap,"position");
                uniform_affTex_bitmap_texture=glGetUniformLocation(programme_affTex_bitmap,"texture_couleur");
                uniform_afftex_bitmap_echelle=glGetUniformLocation(programme_affTex_bitmap,"echelle");
                uniform_afftex_bitmap_taille_ecran=glGetUniformLocation(programme_affTex_bitmap,"taille_ecran");
                uniform_afftex_bitmap_taille_texture=glGetUniformLocation(programme_affTex_bitmap,"taille_texture");
                uniform_afftex_bitmap_position_bitmap=glGetUniformLocation(programme_affTex_bitmap,"position_bitmap");

                return true;
	}


	bool ShadersDemo::init_shader_affTex_zBuffer()
	{
		 //---------- Charge les sources des shaders:

                source_vertex_affTex_zBuffer=charge_source_shader("shaders/affiche_texture_zBuffer_vertex.glsl");
                source_fragment_affTex_zBuffer=charge_source_shader("shaders/affiche_texture_zBuffer_fragment.glsl");
                if (source_fragment_affTex_zBuffer==NULL || source_vertex_affTex_zBuffer==NULL) return false;

                 //---------- Création des shaders:
                shader_affTex_zBuffer_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_affTex_zBuffer_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_affTex_zBuffer_id1==0 || shader_affTex_zBuffer_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader affiche_texture_zBuffer"<<endl;
                        return false;
                }


                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_affTex_zBuffer};

                glShaderSource(shader_affTex_zBuffer_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_affTex_zBuffer;
                glShaderSource(shader_affTex_zBuffer_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:

                if (!compile_shader(shader_affTex_zBuffer_id1)) return false;
                if (!compile_shader(shader_affTex_zBuffer_id2)) return false;


                //----------- Création des programmes
                programme_affTex_zBuffer=glCreateProgram();
                if (programme_affTex_zBuffer==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - affiche_texture_zBuffer"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_affTex_zBuffer,shader_affTex_zBuffer_id1);
                glAttachShader(programme_affTex_zBuffer,shader_affTex_zBuffer_id2);

                //------------ Déclaration du buffer de sortie du fragment shader:
                //if(GVars::openGl_majorVersion>=3 && GVars::openGl_minorVersion>=3)glBindFragDataLocation(programme_mb,0,"outColor");

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_affTex_zBuffer)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_affTex_zBuffer)) return false;

                //----------- Création des liens avec les variables:

                attrib_affTex_zBuffer_position=glGetAttribLocation(programme_affTex_zBuffer,"position");
                uniform_affTex_zBuffer_texture_couleur=glGetUniformLocation(programme_affTex_zBuffer,"texture_couleur");
                uniform_affTex_zBuffer_texture_zBuffer=glGetUniformLocation(programme_affTex_zBuffer,"texture_zBuffer");
                uniform_affTex_zBuffer_taille_ecran=glGetUniformLocation(programme_affTex_zBuffer,"taille_ecran");
                uniform_affTex_zBuffer_taille_tramage=glGetUniformLocation(programme_affTex_zBuffer,"taille_tramage");

                return true;
	}

	bool ShadersDemo::init_shader_profondeur_de_champ()
	{
		 //---------- Charge les sources des shaders:

                source_vertex_profChamp=charge_source_shader("shaders/profondeur_de_champ_vertex.glsl");
                source_fragment_profChamp=charge_source_shader("shaders/profondeur_de_champ_fragment.glsl");
                if (source_fragment_profChamp==NULL || source_vertex_profChamp==NULL) return false;

                 //---------- Création des shaders:
                shader_profChamp_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_profChamp_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_profChamp_id1==0 || shader_profChamp_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader profondeur_de_champ"<<endl;
                        return false;
                }


                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_profChamp};

                glShaderSource(shader_profChamp_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_profChamp;
                glShaderSource(shader_profChamp_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:

                if (!compile_shader(shader_profChamp_id1)) return false;
                if (!compile_shader(shader_profChamp_id2)) return false;


                //----------- Création des programmes
                programme_profChamp=glCreateProgram();
                if (programme_profChamp==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - profondeur_de_champ"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_profChamp,shader_profChamp_id1);
                glAttachShader(programme_profChamp,shader_profChamp_id2);

                //------------ Déclaration du buffer de sortie du fragment shader:
                //if(GVars::openGl_majorVersion>=3 && GVars::openGl_minorVersion>=3)glBindFragDataLocation(programme_mb,0,"outColor");

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_profChamp)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_profChamp)) return false;

                //----------- Création des liens avec les variables:

                attrib_profChamp_position=glGetAttribLocation(programme_profChamp,"position");
                uniform_profChamp_texture_couleur=glGetUniformLocation(programme_profChamp,"texture_couleur");
                uniform_profChamp_texture_zBuffer=glGetUniformLocation(programme_profChamp,"texture_zBuffer");
                uniform_profChamp_taille_ecran=glGetUniformLocation(programme_profChamp,"taille_ecran");
                uniform_profChamp_zMin=glGetUniformLocation(programme_profChamp,"zMin");
                uniform_profChamp_zMax=glGetUniformLocation(programme_profChamp,"zMax");
                uniform_profChamp_zMin_nette=glGetUniformLocation(programme_profChamp,"zMin_nette");
                uniform_profChamp_zMax_nette=glGetUniformLocation(programme_profChamp,"zMax_nette");
                uniform_profChamp_ratio_ecran=glGetUniformLocation(programme_profChamp,"ratio_ecran");
                uniform_profChamp_distance_focale=glGetUniformLocation(programme_profChamp,"distance_focale");

                return true;
	}


    bool ShadersDemo::init_shader_relief_genCones()
	{
		 //---------- Charge les sources des shaders:

                source_vertex_relief_genCones=charge_source_shader("shaders/relief_genCones_vertex.glsl");
                source_fragment_relief_genCones=charge_source_shader("shaders/relief_genCones_fragment.glsl");
                if (source_fragment_relief_genCones==NULL || source_vertex_relief_genCones==NULL) return false;

                 //---------- Création des shaders:
                shader_relief_genCones_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_relief_genCones_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_relief_genCones_id1==0 || shader_relief_genCones_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader affiche_texture"<<endl;
                        return false;
                }


                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_relief_genCones};

                glShaderSource(shader_relief_genCones_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_relief_genCones;
                glShaderSource(shader_relief_genCones_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:

                if (!compile_shader(shader_relief_genCones_id1)) return false;
                if (!compile_shader(shader_relief_genCones_id2)) return false;


                //----------- Création des programmes
                programme_relief_genCones=glCreateProgram();
                if (programme_relief_genCones==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - affiche_texture"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_relief_genCones,shader_relief_genCones_id1);
                glAttachShader(programme_relief_genCones,shader_relief_genCones_id2);

                //------------ Déclaration du buffer de sortie du fragment shader:
                //if(GVars::openGl_majorVersion>=3 && GVars::openGl_minorVersion>=3)glBindFragDataLocation(programme_mb,0,"outColor");

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_relief_genCones)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_relief_genCones)) return false;

                //----------- Création des liens avec les variables:

                attrib_relief_genCones_position=glGetAttribLocation(programme_relief_genCones,"position");
                uniform_relief_genCones_texture_profondeurs=glGetUniformLocation(programme_relief_genCones,"texture_profondeurs");
                uniform_relief_genCones_taille_texture_sortie=glGetUniformLocation(programme_relief_genCones,"taille_texture_sortie");
                uniform_relief_genCones_echelle_profondeur=glGetUniformLocation(programme_relief_genCones,"echelle_profondeur");

                return true;
	}

    bool ShadersDemo::init_shader_mb()
	{
		 //---------- Charge les sources des shaders:

                source_vertex_mb=charge_source_shader("shaders/Mandelbrot_vertex_shader.glsl");
                source_fragment_mb=charge_source_shader("shaders/Mandelbrot_fragment_shader.glsl");
                if (source_fragment_mb==NULL || source_vertex_mb==NULL) return false;

                 //---------- Création des shaders:
                shader_mb_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_mb_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_mb_id1==0 || shader_mb_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader mb"<<endl;
                        return false;
                }


                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_mb};

                glShaderSource(shader_mb_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_mb;
                glShaderSource(shader_mb_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:

                if (!compile_shader(shader_mb_id1)) return false;
                if (!compile_shader(shader_mb_id2)) return false;


                //----------- Création des programmes
                programme_mb=glCreateProgram();
                if (programme_mb==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - mb"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_mb,shader_mb_id1);
                glAttachShader(programme_mb,shader_mb_id2);

                //------------ Déclaration du buffer de sortie du fragment shader:
                //if(GVars::openGl_majorVersion>=3 && GVars::openGl_minorVersion>=3)glBindFragDataLocation(programme_mb,0,"outColor");

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_mb)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_mb)) return false;

                //----------- Création des liens avec les variables:

                attrib_mb_position=glGetAttribLocation(programme_mb,"position");
                uniform_mb_zoom=glGetUniformLocation(programme_mb,"zoom");
                uniform_mb_xpos=glGetUniformLocation(programme_mb,"xpos");
                uniform_mb_ypos=glGetUniformLocation(programme_mb,"ypos");
                uniform_mb_alpha=glGetUniformLocation(programme_mb,"alpha");
                uniform_mb_puissance=glGetUniformLocation(programme_mb,"puissance");
                uniform_mb_ecranDim=glGetUniformLocation(programme_mb,"ecranDim");
                uniform_mb_juliaC=glGetUniformLocation(programme_mb,"juliaC");
                uniform_mb_iter=glGetUniformLocation(programme_mb,"iter");


                return true;
	}


    bool ShadersDemo::init_shader_rm()
	{
		 //---------- Charge les sources des shaders:

                source_vertex_rm=charge_source_shader("shaders/raymarching_vertex_shader.glsl");
                source_fragment_rm=charge_source_shader("shaders/raymarching_fragment_shader.glsl");
                if (source_fragment_rm==NULL || source_vertex_rm==NULL) return false;

                 //---------- Création des shaders:
                shader_rm_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_rm_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_rm_id1==0 || shader_rm_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader Raymarching"<<endl;
                        return false;
                }


                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_rm};

                glShaderSource(shader_rm_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_rm;
                glShaderSource(shader_rm_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:

                if (!compile_shader(shader_rm_id1)) return false;
                if (!compile_shader(shader_rm_id2)) return false;


                //----------- Création des programmes
                programme_rm=glCreateProgram();
                if (programme_rm==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - rm"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_rm,shader_rm_id1);
                glAttachShader(programme_rm,shader_rm_id2);

                //------------ Déclaration du buffer de sortie du fragment shader:
                //if(GVars::openGl_majorVersion>=3 && GVars::openGl_minorVersion>=3)glBindFragDataLocation(programme_mb,0,"outColor");

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_rm)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_rm)) return false;

                //----------- Création des liens avec les variables:

                attrib_rm_position=glGetAttribLocation(programme_rm,"position");
                uniform_rm_ratio_ecran=glGetUniformLocation(programme_rm,"ratio_ecran");
                uniform_rm_distanceFocale=glGetUniformLocation(programme_rm,"distanceFocale");
                uniform_rm_obs_pos=glGetUniformLocation(programme_rm,"obs_pos");
                uniform_rm_obsMat_normale=glGetUniformLocation(programme_rm,"obs_mat_normale");
                uniform_rm_t=glGetUniformLocation(programme_rm,"t");
                uniform_rm_iter=glGetUniformLocation(programme_rm,"iter");
                uniform_rm_l1_position=glGetUniformLocation(programme_rm,"l1_position");
                uniform_rm_l2_position=glGetUniformLocation(programme_rm,"l2_position");
                uniform_rm_l3_position=glGetUniformLocation(programme_rm,"l3_position");
                uniform_rm_zMin=glGetUniformLocation(programme_rm,"zMin");
                uniform_rm_zMax=glGetUniformLocation(programme_rm,"zMax");
                uniform_rm_fogZmin=glGetUniformLocation(programme_rm,"fogZmin");
                uniform_rm_fogZmax=glGetUniformLocation(programme_rm,"fogZmax");
                return true;
	}



    bool ShadersDemo::init_shader_rmGare()
	{
		 //---------- Charge les sources des shaders:

                source_vertex_rmGare=charge_source_shader("shaders/rmGare_vertex_shader.glsl");
                source_fragment_rmGare=charge_source_shader("shaders/rmGare_fragment_shader.glsl");
                if (source_fragment_rmGare==NULL || source_vertex_rmGare==NULL) return false;

                 //---------- Création des shaders:
                shader_rmGare_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_rmGare_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_rmGare_id1==0 || shader_rmGare_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader rmGare"<<endl;
                        return false;
                }


                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_rmGare};

                glShaderSource(shader_rmGare_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_rmGare;
                glShaderSource(shader_rmGare_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:

                if (!compile_shader(shader_rmGare_id1)) return false;
                if (!compile_shader(shader_rmGare_id2)) return false;


                //----------- Création des programmes
                programme_rmGare=glCreateProgram();
                if (programme_rmGare==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - rmGare"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_rmGare,shader_rmGare_id1);
                glAttachShader(programme_rmGare,shader_rmGare_id2);

                //------------ Déclaration du buffer de sortie du fragment shader:
                //if(GVars::openGl_majorVersion>=3 && GVars::openGl_minorVersion>=3)glBindFragDataLocation(programme_mb,0,"outColor");

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_rmGare)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_rmGare)) return false;

                //----------- Création des liens avec les variables:

                attrib_rmGare_position=glGetAttribLocation(programme_rmGare,"position");
                uniform_block_rmGare_eclairages=glGetUniformBlockIndex(programme_rmGare,"eclairages_2si");
                uniform_rmGare_ratio_ecran=glGetUniformLocation(programme_rmGare,"ratio_ecran");
                uniform_rmGare_distanceFocale=glGetUniformLocation(programme_rmGare,"distanceFocale");
                uniform_rmGare_obs_pos=glGetUniformLocation(programme_rmGare,"obs_pos");
                uniform_rmGare_obsMat_normale=glGetUniformLocation(programme_rmGare,"obs_mat_normale");
                uniform_rmGare_t=glGetUniformLocation(programme_rmGare,"t");
                uniform_rmGare_iter=glGetUniformLocation(programme_rmGare,"iter");

                uniform_rmGare_zMin=glGetUniformLocation(programme_rmGare,"zMin");
                uniform_rmGare_zMax=glGetUniformLocation(programme_rmGare,"zMax");

                return true;
	}

	bool ShadersDemo::init_shader_rmt()
	{
		 //---------- Charge les sources des shaders:

                source_vertex_rmt=charge_source_shader("shaders/raymarching_terrain_vertex_shader.glsl");
                source_fragment_rmt=charge_source_shader("shaders/raymarching_terrain_fragment_shader.glsl");
                if (source_fragment_rmt==NULL || source_vertex_rmt==NULL) return false;

                 //---------- Création des shaders:
                shader_rmt_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_rmt_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_rmt_id1==0 || shader_rmt_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader Raymarching terrain"<<endl;
                        return false;
                }


                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_rmt};

                glShaderSource(shader_rmt_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_rmt;
                glShaderSource(shader_rmt_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:

                if (!compile_shader(shader_rmt_id1)) return false;
                if (!compile_shader(shader_rmt_id2)) return false;


                //----------- Création des programmes
                programme_rmt=glCreateProgram();
                if (programme_rmt==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - rmt"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_rmt,shader_rmt_id1);
                glAttachShader(programme_rmt,shader_rmt_id2);

                //------------ Déclaration du buffer de sortie du fragment shader:
                //if(GVars::openGl_majorVersion>=3 && GVars::openGl_minorVersion>=3)glBindFragDataLocation(programme_mb,0,"outColor");

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_rmt)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_rmt)) return false;

                //----------- Création des liens avec les variables:

                uniform_block_rmt_eclairages=glGetUniformBlockIndex(programme_rmt,"eclairages_2si");
                attrib_rmt_position=glGetAttribLocation(programme_rmt,"position");
                uniform_rmt_ratio_ecran=glGetUniformLocation(programme_rmt,"ratio_ecran");
                uniform_rmt_distanceFocale=glGetUniformLocation(programme_rmt,"distanceFocale");
                uniform_rmt_obs_pos=glGetUniformLocation(programme_rmt,"obs_pos");
                uniform_rmt_obsMat_normale=glGetUniformLocation(programme_rmt,"obs_mat_normale");

                uniform_rmt_zMin=glGetUniformLocation(programme_rmt,"zMin");
                uniform_rmt_zMax=glGetUniformLocation(programme_rmt,"zMax");
                uniform_rmt_facteur_echelle_terrain=glGetUniformLocation(programme_rmt,"facteur_echelle_terrain");
                uniform_rmt_facteur_echelle_terrain2=glGetUniformLocation(programme_rmt,"facteur_echelle_terrain2");
                uniform_rmt_facteur_echelle_terrain3=glGetUniformLocation(programme_rmt,"facteur_echelle_terrain3");
                uniform_rmt_texture_terrain=glGetUniformLocation(programme_rmt,"texture_terrain");
                uniform_rmt_texture_terrain2=glGetUniformLocation(programme_rmt,"texture_terrain2");
                uniform_rmt_texture_terrain3=glGetUniformLocation(programme_rmt,"texture_terrain3");
                uniform_rmt_amplitude_terrain=glGetUniformLocation(programme_rmt,"amplitude_terrain");
                uniform_rmt_amplitude_terrain2=glGetUniformLocation(programme_rmt,"amplitude_terrain2");
                uniform_rmt_amplitude_terrain3=glGetUniformLocation(programme_rmt,"amplitude_terrain3");
                uniform_rmt_facteur_precision_distance=glGetUniformLocation(programme_rmt,"facteur_precision_distance");
                return true;
	}


    bool ShadersDemo::init_shader_nuages()
	{
		 //---------- Charge les sources des shaders:

                source_vertex_nuages=charge_source_shader("shaders/nuages_vertex.glsl");
                source_fragment_nuages=charge_source_shader("shaders/nuages_fragment.glsl");
                if (source_fragment_nuages==NULL || source_vertex_nuages==NULL) return false;

                 //---------- Création des shaders:
                shader_nuages_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_nuages_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_nuages_id1==0 || shader_nuages_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader Nuages"<<endl;
                        return false;
                }


                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_nuages};

                glShaderSource(shader_nuages_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_nuages;
                glShaderSource(shader_nuages_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:

                if (!compile_shader(shader_nuages_id1)) return false;
                if (!compile_shader(shader_nuages_id2)) return false;


                //----------- Création des programmes
                programme_nuages=glCreateProgram();
                if (programme_nuages==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - Nuages"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_nuages,shader_nuages_id1);
                glAttachShader(programme_nuages,shader_nuages_id2);

                //------------ Déclaration du buffer de sortie du fragment shader:
                //if(GVars::openGl_majorVersion>=3 && GVars::openGl_minorVersion>=3)glBindFragDataLocation(programme_mb,0,"outColor");

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_nuages)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_nuages)) return false;

                //----------- Création des liens avec les variables:

                uniform_block_nuages_eclairages=glGetUniformBlockIndex(programme_nuages,"eclairages_2si");
                uniform_block_nuages_matrices=glGetUniformBlockIndex(programme_nuages,"matrices");
                attrib_nuages_position=glGetAttribLocation(programme_nuages,"position");
                uniform_nuages_zMin=glGetUniformLocation(programme_nuages,"zMin");
                uniform_nuages_zMax=glGetUniformLocation(programme_nuages,"zMax");
                uniform_nuages_obsPos=glGetUniformLocation(programme_nuages,"position_observateur");
                uniform_nuages_texture_o1=glGetUniformLocation(programme_nuages,"texture_octave1");
                uniform_nuages_texture_o2=glGetUniformLocation(programme_nuages,"texture_octave2");
                uniform_nuages_texture_o3=glGetUniformLocation(programme_nuages,"texture_octave3");
                uniform_nuages_t=glGetUniformLocation(programme_nuages,"t");


                return true;
	}


        bool ShadersDemo::initShaders()
        {
                bool erreur=false;

                if (!GLEW_ARB_vertex_program || !GLEW_ARB_fragment_program)
                {
                        cout<<"Impossible d'initialiser les shaders !!!"<<endl;
                        erreur=true;
                }
                if (!glewIsSupported("GL_ARB_shading_language_100")) {cout<<"GLSL non supporté"<<endl; erreur=true;}
                if (!glewIsSupported("GL_ARB_shader_objects")) {cout<<"Objets shaders non supportés"<<endl;erreur=true;}
                if (!glewIsSupported("GL_ARB_vertex_shader")) {cout<<"Shaders de sommets non supportés"<<endl;erreur=true;}
                if (!glewIsSupported("GL_ARB_fragment_shader")) {cout<<"Shaders de fragments non supportés"<<endl;erreur=true;}
                if (erreur) return false;

                 //--------- Création des programmes:=0

                 if(!init_shader_affTex())return false;
                 if(!init_shader_affTex_bitmap())return false;
                 if(!init_shader_affTex_zBuffer())return false;
                 if(!init_shader_profondeur_de_champ())return false;
                 if(!init_shader_relief_genCones())return false;
                 if(!init_shader_mb())return false;
                 if(!init_shader_rm())return false;
                 if(!init_shader_rmGare())return false;
                 if(!init_shader_rmt())return false;
                 if(!init_shader_nuages())return false;

                 return true;
        }

        //---------------------------------------------------------------
        //      Destruction des shaders
        //---------------------------------------------------------------

        void ShadersDemo::supprime_shader(GLuint programme, GLchar* source_vertex, GLchar* source_fragment,GLuint shader_id1,GLuint shader_id2 )
        {
            if (source_fragment!=NULL) free (source_fragment);
            if (source_vertex!=NULL) free (source_vertex);
            if (programme!=0)
            {
                    glDetachShader(programme,shader_id1);
                    glDetachShader(programme,shader_id2);
                    glDeleteProgram(programme);
            }
            if (shader_id1!=0) glDeleteShader(shader_id1);
            if (shader_id2!=0) glDeleteShader(shader_id2);
        }

        void ShadersDemo::freeShaders()
        {
                supprime_shader(programme_affTex,source_vertex_affTex,source_fragment_affTex,shader_affTex_id1,shader_affTex_id2);
                supprime_shader(programme_affTex_bitmap,source_vertex_affTex_bitmap,source_fragment_affTex_bitmap,shader_affTex_bitmap_id1,shader_affTex_bitmap_id2);
                supprime_shader(programme_profChamp,source_vertex_profChamp,source_fragment_profChamp,shader_profChamp_id1,shader_profChamp_id2);
                supprime_shader(programme_relief_genCones,source_vertex_relief_genCones,source_fragment_relief_genCones,shader_relief_genCones_id1,shader_relief_genCones_id2);
                supprime_shader(programme_affTex_zBuffer,source_vertex_affTex_zBuffer,source_fragment_affTex_zBuffer,shader_affTex_zBuffer_id1,shader_affTex_zBuffer_id2);
                supprime_shader(programme_mb,source_vertex_mb,source_fragment_mb,shader_mb_id1,shader_mb_id2);
                supprime_shader(programme_rm,source_vertex_rm,source_fragment_rm,shader_rm_id1,shader_rm_id2);
                supprime_shader(programme_rmGare,source_vertex_rmGare,source_fragment_rmGare,shader_rmGare_id1,shader_rmGare_id2);
                supprime_shader(programme_rmt,source_vertex_rmt,source_fragment_rmt,shader_rmt_id1,shader_rmt_id2);
                supprime_shader(programme_nuages,source_vertex_nuages,source_fragment_nuages,shader_nuages_id1,shader_nuages_id2);
        }

