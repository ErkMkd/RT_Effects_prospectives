#include "ShadersPrimitives.h"

        GLint ShadersPrimitives::openGl_minorVersion=0;
        GLint ShadersPrimitives::openGl_majorVersion=0;

    //---------------- Eclairage Phong, 2 sources infinies:

        GLchar* ShadersPrimitives::source_vertex_prim_phong2si=NULL;
        GLchar* ShadersPrimitives::source_fragment_prim_phong2si=NULL;
        GLuint ShadersPrimitives::shader_prim_phong2si_id1=0;
        GLuint ShadersPrimitives::shader_prim_phong2si_id2=0;
        GLuint ShadersPrimitives::programme_prim_phong2si=0;

        //Uniform blocks:
        GLint ShadersPrimitives::prim_phong2si_matrices=0;
        GLint ShadersPrimitives::prim_phong2si_eclairages=0;
        //Vertex attributes:
        GLint ShadersPrimitives::prim_phong2si_position=0;
        GLint ShadersPrimitives::prim_phong2si_normale=0;
        //Uniforms
        GLint ShadersPrimitives::prim_phong2si_matrice_instance=0;
        GLint ShadersPrimitives::prim_phong2si_matrice_normale_instance=0;
        GLint ShadersPrimitives::prim_phong2si_materiau_ambiance=0;
        GLint ShadersPrimitives::prim_phong2si_materiau_diffusion=0;
        GLint ShadersPrimitives::prim_phong2si_materiau_luminosite=0;
        GLint ShadersPrimitives::prim_phong2si_materiau_specularite=0;
        GLint ShadersPrimitives::prim_phong2si_materiau_brillance=0;


    //--------------------------------------------------------------
    //      Création des shaders des primitives
    //--------------------------------------------------------------


	//Phong, deux sources lumineuses infinies (2si):
    bool ShadersPrimitives::init_shader_prim_phong2si()
	{
		 //---------- Charge les sources des shaders:

                source_vertex_prim_phong2si=charge_source_shader("Primitives/shaders/prim_phong2si_vertex.glsl");
                source_fragment_prim_phong2si=charge_source_shader("Primitives/shaders/prim_phong2si_fragment.glsl");
                if (source_fragment_prim_phong2si==NULL || source_vertex_prim_phong2si==NULL) return false;

                 //---------- Création des shaders:
                shader_prim_phong2si_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_prim_phong2si_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_prim_phong2si_id1==0 || shader_prim_phong2si_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader prim_phong2si"<<endl;
                        return false;
                }


                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_prim_phong2si};

                glShaderSource(shader_prim_phong2si_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_prim_phong2si;
                glShaderSource(shader_prim_phong2si_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:

                if (!compile_shader(shader_prim_phong2si_id1)) return false;
                if (!compile_shader(shader_prim_phong2si_id2)) return false;


                //----------- Création des programmes
                programme_prim_phong2si=glCreateProgram();
                if (programme_prim_phong2si==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - prim_phong2si"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_prim_phong2si,shader_prim_phong2si_id1);
                glAttachShader(programme_prim_phong2si,shader_prim_phong2si_id2);

                //------------ Déclaration du buffer de sortie du fragment shader:
                //if(openGl_majorVersion>=3 && openGl_minorVersion>=3)glBindFragDataLocation(programme_flat,0,"outColor");

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_prim_phong2si)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_prim_phong2si)) return false;

                //----------- Création des liens avec les variables:


                prim_phong2si_position=glGetAttribLocation(programme_prim_phong2si,"position");
                prim_phong2si_normale=glGetAttribLocation(programme_prim_phong2si,"normale");

                prim_phong2si_matrices=glGetUniformBlockIndex(programme_prim_phong2si,"matrices");
                prim_phong2si_eclairages=glGetUniformBlockIndex(programme_prim_phong2si,"eclairages_2si");

                prim_phong2si_matrice_instance=glGetUniformLocation(programme_prim_phong2si,"matrice_instance");
                prim_phong2si_matrice_normale_instance=glGetUniformLocation(programme_prim_phong2si,"matrice_normale_instance");
                prim_phong2si_materiau_ambiance=glGetUniformLocation(programme_prim_phong2si,"materiau_ambiance");
                prim_phong2si_materiau_diffusion=glGetUniformLocation(programme_prim_phong2si,"materiau_diffusion");
                prim_phong2si_materiau_luminosite=glGetUniformLocation(programme_prim_phong2si,"materiau_luminosite");
                prim_phong2si_materiau_specularite=glGetUniformLocation(programme_prim_phong2si,"materiau_specularite");
                prim_phong2si_materiau_brillance=glGetUniformLocation(programme_prim_phong2si,"materiau_brillance");

                return true;
	}


        bool ShadersPrimitives::initShaders()
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
                if (!glewIsSupported("GL_ARB_instanced_arrays")) {cout<<"Instanciation non supportée"<<endl;erreur=true;}
                if (erreur) return false;

                glGetIntegerv(GL_MAJOR_VERSION,&openGl_majorVersion);
                glGetIntegerv(GL_MINOR_VERSION,&openGl_minorVersion);

                 //--------- Création des programmes:=0


                 if(!init_shader_prim_phong2si())return false;

                 return true;
        }

        //---------------------------------------------------------------
        //      Destruction des shaders
        //---------------------------------------------------------------
        void ShadersPrimitives::supprime_shader(GLuint programme, GLchar* source_vertex, GLchar* source_fragment,GLuint shader_id1,GLuint shader_id2 )
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

        void ShadersPrimitives::freeShaders()
        {
            supprime_shader(programme_prim_phong2si,source_vertex_prim_phong2si,source_fragment_prim_phong2si,shader_prim_phong2si_id1,shader_prim_phong2si_id2);
        }

