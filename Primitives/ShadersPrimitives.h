#ifndef SHADERSPRIMITIVES_H_INCLUDED
#define SHADERSPRIMITIVES_H_INCLUDED

#include "../GestionShaders.h"

class ShadersPrimitives :public GestionShaders
{
    public:
        static GLint openGl_majorVersion;
        static GLint openGl_minorVersion;


        //---------------- Eclairage Phong:

        static GLchar* source_vertex_prim_phong2si;
        static GLchar* source_fragment_prim_phong2si;
        static GLuint shader_prim_phong2si_id1;
        static GLuint shader_prim_phong2si_id2;
        static GLuint programme_prim_phong2si;

        //Uniform blocks:
        static GLint prim_phong2si_matrices;
        static GLint prim_phong2si_eclairages;
        //Vertex attributes:
        static GLint prim_phong2si_position;
        static GLint prim_phong2si_normale;
        //Uniforms
        static GLint prim_phong2si_matrice_instance;
        static GLint prim_phong2si_matrice_normale_instance;
        static GLint prim_phong2si_materiau_ambiance;
        static GLint prim_phong2si_materiau_diffusion;
        static GLint prim_phong2si_materiau_luminosite;
        static GLint prim_phong2si_materiau_specularite;
        static GLint prim_phong2si_materiau_brillance;


    private:
        static void supprime_shader(GLuint programme, GLchar* source_vertex, GLchar* source_fragment,GLuint shader_id1,GLuint shader_id2 );
    public:
        static bool init_shader_prim_phong2si();

        static bool initShaders();
        static void freeShaders();
};

#endif // SHADERSPRIMITIVES_H_INCLUDED
