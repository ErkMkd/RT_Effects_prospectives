#include "Shaders3d.h"

        GLint Shaders3d::openGl_minorVersion=0;
        GLint Shaders3d::openGl_majorVersion=0;

    //---------------- Aplat de couleur sans éclairage:

        GLchar* Shaders3d::source_vertex_flat=NULL;
        GLchar* Shaders3d::source_fragment_flat=NULL;
        GLuint Shaders3d::shader_flat_id1=0;
        GLuint Shaders3d::shader_flat_id2=0;
        GLuint Shaders3d::programme_flat=0;
        GLint Shaders3d::flat_position=0;

        GLint Shaders3d::flat_matrices=0;
        GLint Shaders3d::flat_couleur=0;

    //---------------- Eclairage Gouraud, 2 sources infinies:

        GLchar* Shaders3d::source_vertex_gouraud2si=NULL;
        GLchar* Shaders3d::source_fragment_gouraud2si=NULL;
        GLuint Shaders3d::shader_gouraud2si_id1=0;
        GLuint Shaders3d::shader_gouraud2si_id2=0;
        GLuint Shaders3d::programme_gouraud2si=0;

        //Uniform blocks:
        GLint Shaders3d::gouraud2si_matrices=0;
        GLint Shaders3d::gouraud2si_eclairages=0;
        //Vertex attributes:
        GLint Shaders3d::gouraud2si_position=0;
        GLint Shaders3d::gouraud2si_normale=0;
        //Uniforms
        GLint Shaders3d::gouraud2si_materiau_ambiance=0;
        GLint Shaders3d::gouraud2si_materiau_diffusion=0;
        GLint Shaders3d::gouraud2si_materiau_luminosite=0;
        GLint Shaders3d::gouraud2si_materiau_specularite=0;
        GLint Shaders3d::gouraud2si_materiau_brillance=0;

    //---------------- Eclairage Phong, 2 sources infinies:

        GLchar* Shaders3d::source_vertex_phong2si=NULL;
        GLchar* Shaders3d::source_fragment_phong2si=NULL;
        GLuint Shaders3d::shader_phong2si_id1=0;
        GLuint Shaders3d::shader_phong2si_id2=0;
        GLuint Shaders3d::programme_phong2si=0;

        //Uniform blocks:
        GLint Shaders3d::phong2si_matrices=0;
        GLint Shaders3d::phong2si_eclairages=0;
        //Vertex attributes:
        GLint Shaders3d::phong2si_position=0;
        GLint Shaders3d::phong2si_normale=0;
        //Uniforms
        GLint Shaders3d::phong2si_materiau_ambiance=0;
        GLint Shaders3d::phong2si_materiau_diffusion=0;
        GLint Shaders3d::phong2si_materiau_luminosite=0;
        GLint Shaders3d::phong2si_materiau_specularite=0;
        GLint Shaders3d::phong2si_materiau_brillance=0;

        //---------------- Texture sans éclairage:

        GLchar* Shaders3d::source_vertex_texture=NULL;
        GLchar* Shaders3d::source_fragment_texture=NULL;
        GLuint Shaders3d::shader_texture_id1=0;
        GLuint Shaders3d::shader_texture_id2=0;
        GLuint Shaders3d::programme_texture=0;

        GLint Shaders3d::texture_position=0;
        GLint Shaders3d::texture_coordonnees_UV=0;

        GLint Shaders3d::texture_matrices=0;
        GLint Shaders3d::texture_couleur=0;
        GLint Shaders3d::texture_texture=0;

        //---------------- Eclairage Gouraud avec texture, 2 sources infinies:

        GLchar* Shaders3d::source_vertex_texture_gouraud2si=NULL;
        GLchar* Shaders3d::source_fragment_texture_gouraud2si=NULL;
        GLuint Shaders3d::shader_texture_gouraud2si_id1=0;
        GLuint Shaders3d::shader_texture_gouraud2si_id2=0;
        GLuint Shaders3d::programme_texture_gouraud2si=0;

        //Uniform blocks:
        GLint Shaders3d::texture_gouraud2si_matrices=0;
        GLint Shaders3d::texture_gouraud2si_eclairages=0;
        //Vertex attributes:
        GLint Shaders3d::texture_gouraud2si_position=0;
        GLint Shaders3d::texture_gouraud2si_normale=0;
        GLint Shaders3d::texture_gouraud2si_coordonnees_UV=0;
        //Uniforms
        GLint Shaders3d::texture_gouraud2si_materiau_ambiance=0;
        GLint Shaders3d::texture_gouraud2si_materiau_diffusion=0;
        GLint Shaders3d::texture_gouraud2si_materiau_luminosite=0;
        GLint Shaders3d::texture_gouraud2si_materiau_specularite=0;
        GLint Shaders3d::texture_gouraud2si_materiau_brillance=0;
        GLint Shaders3d::texture_gouraud2si_texture=0;

        //---------------- Eclairage Phong avec texture, 2 sources infinies:

        GLchar* Shaders3d::source_vertex_texture_phong2si=NULL;
        GLchar* Shaders3d::source_fragment_texture_phong2si=NULL;
        GLuint Shaders3d::shader_texture_phong2si_id1=0;
        GLuint Shaders3d::shader_texture_phong2si_id2=0;
        GLuint Shaders3d::programme_texture_phong2si=0;

        //Uniform blocks:
        GLint Shaders3d::texture_phong2si_matrices=0;
        GLint Shaders3d::texture_phong2si_eclairages=0;
        //Vertex attributes:
        GLint Shaders3d::texture_phong2si_position=0;
        GLint Shaders3d::texture_phong2si_normale=0;
        GLint Shaders3d::texture_phong2si_coordonnees_UV=0;
        //Uniforms
        GLint Shaders3d::texture_phong2si_materiau_ambiance=0;
        GLint Shaders3d::texture_phong2si_materiau_diffusion=0;
        GLint Shaders3d::texture_phong2si_materiau_luminosite=0;
        GLint Shaders3d::texture_phong2si_materiau_specularite=0;
        GLint Shaders3d::texture_phong2si_materiau_brillance=0;
        GLint Shaders3d::texture_phong2si_texture=0;



    //--------------------------------------------------------------
    //      Création des shaders du moteur 3d
    //--------------------------------------------------------------

    // Aplat sans éclairage:
    bool Shaders3d::init_shader_flat()
	{
		 //---------- Charge les sources des shaders:

                source_vertex_flat=charge_source_shader("Objets3d/shaders/flat_vertex.glsl");
                source_fragment_flat=charge_source_shader("Objets3d/shaders/flat_fragment.glsl");
                if (source_fragment_flat==NULL || source_vertex_flat==NULL) return false;

                 //---------- Création des shaders:
                shader_flat_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_flat_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_flat_id1==0 || shader_flat_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader flat"<<endl;
                        return false;
                }


                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_flat};

                glShaderSource(shader_flat_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_flat;
                glShaderSource(shader_flat_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:

                if (!compile_shader(shader_flat_id1)) return false;
                if (!compile_shader(shader_flat_id2)) return false;


                //----------- Création des programmes
                programme_flat=glCreateProgram();
                if (programme_flat==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - flat"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_flat,shader_flat_id1);
                glAttachShader(programme_flat,shader_flat_id2);

                //------------ Déclaration du buffer de sortie du fragment shader:
                //if(openGl_majorVersion>=3 && openGl_minorVersion>=3)glBindFragDataLocation(programme_flat,0,"outColor");

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_flat)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_flat)) return false;

                //----------- Création des liens avec les variables:


                flat_position=glGetAttribLocation(programme_flat,"position");
                flat_matrices=glGetUniformBlockIndex(programme_flat,"matrices");

                flat_couleur=glGetUniformLocation(programme_flat,"couleur");


                return true;
	}

    //Gouraud, deux sources lumineuses infinies (2si):
    bool Shaders3d::init_shader_gouraud_2si()
	{
		 //---------- Charge les sources des shaders:

                source_vertex_gouraud2si=charge_source_shader("Objets3d/shaders/gouraud2si_vertex.glsl");
                source_fragment_gouraud2si=charge_source_shader("Objets3d/shaders/gouraud2si_fragment.glsl");
                if (source_fragment_gouraud2si==NULL || source_vertex_gouraud2si==NULL) return false;

                 //---------- Création des shaders:
                shader_gouraud2si_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_gouraud2si_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_gouraud2si_id1==0 || shader_gouraud2si_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader gouraud2si"<<endl;
                        return false;
                }


                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_gouraud2si};

                glShaderSource(shader_gouraud2si_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_gouraud2si;
                glShaderSource(shader_gouraud2si_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:

                if (!compile_shader(shader_gouraud2si_id1)) return false;
                if (!compile_shader(shader_gouraud2si_id2)) return false;


                //----------- Création des programmes
                programme_gouraud2si=glCreateProgram();
                if (programme_gouraud2si==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - gouraud2si"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_gouraud2si,shader_gouraud2si_id1);
                glAttachShader(programme_gouraud2si,shader_gouraud2si_id2);

                //------------ Déclaration du buffer de sortie du fragment shader:
                //if(openGl_majorVersion>=3 && openGl_minorVersion>=3)glBindFragDataLocation(programme_flat,0,"outColor");

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_gouraud2si)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_gouraud2si)) return false;

                //----------- Création des liens avec les variables:


                gouraud2si_position=glGetAttribLocation(programme_gouraud2si,"position");
                gouraud2si_normale=glGetAttribLocation(programme_gouraud2si,"normale");

                gouraud2si_matrices=glGetUniformBlockIndex(programme_gouraud2si,"matrices");
                gouraud2si_eclairages=glGetUniformBlockIndex(programme_gouraud2si,"eclairages_2si");

                gouraud2si_materiau_ambiance=glGetUniformLocation(programme_gouraud2si,"materiau_ambiance");
                gouraud2si_materiau_diffusion=glGetUniformLocation(programme_gouraud2si,"materiau_diffusion");
                gouraud2si_materiau_luminosite=glGetUniformLocation(programme_gouraud2si,"materiau_luminosite");
                gouraud2si_materiau_specularite=glGetUniformLocation(programme_gouraud2si,"materiau_specularite");
                gouraud2si_materiau_brillance=glGetUniformLocation(programme_gouraud2si,"materiau_brillance");

                return true;
	}

	//Phong, deux sources lumineuses infinies (2si):
    bool Shaders3d::init_shader_phong_2si()
	{
		 //---------- Charge les sources des shaders:

                source_vertex_phong2si=charge_source_shader("Objets3d/shaders/phong2si_vertex.glsl");
                source_fragment_phong2si=charge_source_shader("Objets3d/shaders/phong2si_fragment.glsl");
                if (source_fragment_phong2si==NULL || source_vertex_phong2si==NULL) return false;

                 //---------- Création des shaders:
                shader_phong2si_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_phong2si_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_phong2si_id1==0 || shader_phong2si_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader phong2si"<<endl;
                        return false;
                }


                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_phong2si};

                glShaderSource(shader_phong2si_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_phong2si;
                glShaderSource(shader_phong2si_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:

                if (!compile_shader(shader_phong2si_id1)) return false;
                if (!compile_shader(shader_phong2si_id2)) return false;


                //----------- Création des programmes
                programme_phong2si=glCreateProgram();
                if (programme_phong2si==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - phong2si"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_phong2si,shader_phong2si_id1);
                glAttachShader(programme_phong2si,shader_phong2si_id2);

                //------------ Déclaration du buffer de sortie du fragment shader:
                //if(openGl_majorVersion>=3 && openGl_minorVersion>=3)glBindFragDataLocation(programme_flat,0,"outColor");

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_phong2si)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_phong2si)) return false;

                //----------- Création des liens avec les variables:


                phong2si_position=glGetAttribLocation(programme_phong2si,"position");
                phong2si_normale=glGetAttribLocation(programme_phong2si,"normale");

                phong2si_matrices=glGetUniformBlockIndex(programme_phong2si,"matrices");
                phong2si_eclairages=glGetUniformBlockIndex(programme_phong2si,"eclairages_2si");

                phong2si_materiau_ambiance=glGetUniformLocation(programme_phong2si,"materiau_ambiance");
                phong2si_materiau_diffusion=glGetUniformLocation(programme_phong2si,"materiau_diffusion");
                phong2si_materiau_luminosite=glGetUniformLocation(programme_phong2si,"materiau_luminosite");
                phong2si_materiau_specularite=glGetUniformLocation(programme_phong2si,"materiau_specularite");
                phong2si_materiau_brillance=glGetUniformLocation(programme_phong2si,"materiau_brillance");

                return true;
	}

    // Texture sans éclairage:
    bool Shaders3d::init_shader_texture()
	{
		 //---------- Charge les sources des shaders:

                source_vertex_texture=charge_source_shader("Objets3d/shaders/texture_vertex.glsl");
                source_fragment_texture=charge_source_shader("Objets3d/shaders/texture_fragment.glsl");
                if (source_fragment_texture==NULL || source_vertex_texture==NULL) return false;

                 //---------- Création des shaders:
                shader_texture_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_texture_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_texture_id1==0 || shader_texture_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader texture"<<endl;
                        return false;
                }


                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_texture};

                glShaderSource(shader_texture_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_texture;
                glShaderSource(shader_texture_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:

                if (!compile_shader(shader_texture_id1)) return false;
                if (!compile_shader(shader_texture_id2)) return false;


                //----------- Création des programmes
                programme_texture=glCreateProgram();
                if (programme_texture==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - texture"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_texture,shader_texture_id1);
                glAttachShader(programme_texture,shader_texture_id2);

                //------------ Déclaration du buffer de sortie du fragment shader:
                //if(openGl_majorVersion>=3 && openGl_minorVersion>=3)glBindFragDataLocation(programme_texture,0,"outColor");

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_texture)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_texture)) return false;

                //----------- Création des liens avec les variables:


                texture_position=glGetAttribLocation(programme_texture,"position");
                texture_coordonnees_UV=glGetAttribLocation(programme_texture,"coordonnees_UV");

                texture_matrices=glGetUniformBlockIndex(programme_texture,"matrices");

                texture_couleur=glGetUniformLocation(programme_texture,"couleur");
                texture_texture=glGetUniformLocation(programme_texture,"texture_couleur");


                return true;
	}


    //Gouraud avec texture et deux sources lumineuses infinies (2si):

    bool Shaders3d::init_shader_texture_gouraud2si()
	{
		 //---------- Charge les sources des shaders:

                source_vertex_texture_gouraud2si=charge_source_shader("Objets3d/shaders/texture_gouraud2si_vertex.glsl");
                source_fragment_texture_gouraud2si=charge_source_shader("Objets3d/shaders/texture_gouraud2si_fragment.glsl");
                if (source_fragment_texture_gouraud2si==NULL || source_vertex_texture_gouraud2si==NULL) return false;

                 //---------- Création des shaders:
                shader_texture_gouraud2si_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_texture_gouraud2si_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_texture_gouraud2si_id1==0 || shader_texture_gouraud2si_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader texture_gouraud2si"<<endl;
                        return false;
                }


                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_texture_gouraud2si};

                glShaderSource(shader_texture_gouraud2si_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_texture_gouraud2si;
                glShaderSource(shader_texture_gouraud2si_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:

                if (!compile_shader(shader_texture_gouraud2si_id1)) return false;
                if (!compile_shader(shader_texture_gouraud2si_id2)) return false;


                //----------- Création des programmes
                programme_texture_gouraud2si=glCreateProgram();
                if (programme_texture_gouraud2si==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - texture_gouraud2si"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_texture_gouraud2si,shader_texture_gouraud2si_id1);
                glAttachShader(programme_texture_gouraud2si,shader_texture_gouraud2si_id2);

                //------------ Déclaration du buffer de sortie du fragment shader:
                //if(openGl_majorVersion>=3 && openGl_minorVersion>=3)glBindFragDataLocation(programme_flat,0,"outColor");

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_texture_gouraud2si)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_texture_gouraud2si)) return false;

                //----------- Création des liens avec les variables:


                texture_gouraud2si_position=glGetAttribLocation(programme_texture_gouraud2si,"position");
                texture_gouraud2si_coordonnees_UV=glGetAttribLocation(programme_texture_gouraud2si,"coordonnees_UV");
                texture_gouraud2si_normale=glGetAttribLocation(programme_texture_gouraud2si,"normale");

                texture_gouraud2si_matrices=glGetUniformBlockIndex(programme_texture_gouraud2si,"matrices");
                texture_gouraud2si_eclairages=glGetUniformBlockIndex(programme_texture_gouraud2si,"eclairages_2si");

                texture_gouraud2si_materiau_ambiance=glGetUniformLocation(programme_texture_gouraud2si,"materiau_ambiance");
                texture_gouraud2si_materiau_diffusion=glGetUniformLocation(programme_texture_gouraud2si,"materiau_diffusion");
                texture_gouraud2si_materiau_luminosite=glGetUniformLocation(programme_texture_gouraud2si,"materiau_luminosite");
                texture_gouraud2si_materiau_specularite=glGetUniformLocation(programme_texture_gouraud2si,"materiau_specularite");
                texture_gouraud2si_materiau_brillance=glGetUniformLocation(programme_texture_gouraud2si,"materiau_brillance");
                texture_gouraud2si_texture=glGetUniformLocation(programme_texture_gouraud2si,"texture_couleur");

                return true;
	}

	//Phong avec texture et deux sources lumineuses infinies (2si):

    bool Shaders3d::init_shader_texture_phong2si()
	{
		 //---------- Charge les sources des shaders:

                source_vertex_texture_phong2si=charge_source_shader("Objets3d/shaders/texture_phong2si_vertex.glsl");
                source_fragment_texture_phong2si=charge_source_shader("Objets3d/shaders/texture_phong2si_fragment.glsl");
                if (source_fragment_texture_phong2si==NULL || source_vertex_texture_phong2si==NULL) return false;

                 //---------- Création des shaders:
                shader_texture_phong2si_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_texture_phong2si_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_texture_phong2si_id1==0 || shader_texture_phong2si_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader texture_phong2si"<<endl;
                        return false;
                }


                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_texture_phong2si};

                glShaderSource(shader_texture_phong2si_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_texture_phong2si;
                glShaderSource(shader_texture_phong2si_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:

                if (!compile_shader(shader_texture_phong2si_id1)) return false;
                if (!compile_shader(shader_texture_phong2si_id2)) return false;


                //----------- Création des programmes
                programme_texture_phong2si=glCreateProgram();
                if (programme_texture_phong2si==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - texture_phong2si"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_texture_phong2si,shader_texture_phong2si_id1);
                glAttachShader(programme_texture_phong2si,shader_texture_phong2si_id2);

                //------------ Déclaration du buffer de sortie du fragment shader:
                //if(openGl_majorVersion>=3 && openGl_minorVersion>=3)glBindFragDataLocation(programme_flat,0,"outColor");

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_texture_phong2si)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_texture_phong2si)) return false;

                //----------- Création des liens avec les variables:


                texture_phong2si_position=glGetAttribLocation(programme_texture_phong2si,"position");
                texture_phong2si_coordonnees_UV=glGetAttribLocation(programme_texture_phong2si,"coordonnees_UV");
                texture_phong2si_normale=glGetAttribLocation(programme_texture_phong2si,"normale");

                texture_phong2si_matrices=glGetUniformBlockIndex(programme_texture_phong2si,"matrices");
                texture_phong2si_eclairages=glGetUniformBlockIndex(programme_texture_phong2si,"eclairages_2si");

                texture_phong2si_materiau_ambiance=glGetUniformLocation(programme_texture_phong2si,"materiau_ambiance");
                texture_phong2si_materiau_diffusion=glGetUniformLocation(programme_texture_phong2si,"materiau_diffusion");
                texture_phong2si_materiau_luminosite=glGetUniformLocation(programme_texture_phong2si,"materiau_luminosite");
                texture_phong2si_materiau_specularite=glGetUniformLocation(programme_texture_phong2si,"materiau_specularite");
                texture_phong2si_materiau_brillance=glGetUniformLocation(programme_texture_phong2si,"materiau_brillance");
                texture_phong2si_texture=glGetUniformLocation(programme_texture_phong2si,"texture_couleur");

                return true;
	}

        bool Shaders3d::initShaders()
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

                glGetIntegerv(GL_MAJOR_VERSION,&openGl_majorVersion);
                glGetIntegerv(GL_MINOR_VERSION,&openGl_minorVersion);

                 //--------- Création des programmes:=0

                 if(!init_shader_flat())return false;
                 if(!init_shader_gouraud_2si())return false;
                 if(!init_shader_phong_2si())return false;
                 if(!init_shader_texture())return false;
                 if(!init_shader_texture_gouraud2si())return false;
                 if(!init_shader_texture_phong2si())return false;

                 return true;
        }

        //---------------------------------------------------------------
        //      Destruction des shaders
        //---------------------------------------------------------------
        void Shaders3d::supprime_shader(GLuint programme, GLchar* source_vertex, GLchar* source_fragment,GLuint shader_id1,GLuint shader_id2 )
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

        void Shaders3d::freeShaders()
        {
            supprime_shader(programme_flat,source_vertex_flat,source_fragment_flat,shader_flat_id1,shader_flat_id2);
            supprime_shader(programme_gouraud2si,source_vertex_gouraud2si,source_fragment_gouraud2si,shader_gouraud2si_id1,shader_gouraud2si_id2);
            supprime_shader(programme_phong2si,source_vertex_phong2si,source_fragment_phong2si,shader_phong2si_id1,shader_phong2si_id2);
            supprime_shader(programme_texture,source_vertex_texture,source_fragment_texture,shader_texture_id1,shader_texture_id2);
            supprime_shader(programme_texture_gouraud2si,source_vertex_texture_gouraud2si,source_fragment_texture_gouraud2si,shader_texture_gouraud2si_id1,shader_texture_gouraud2si_id2);
            supprime_shader(programme_texture_phong2si,source_vertex_texture_phong2si,source_fragment_texture_phong2si,shader_texture_phong2si_id1,shader_texture_phong2si_id2);
        }

