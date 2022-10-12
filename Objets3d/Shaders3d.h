#ifndef SHADERS3D_H_INCLUDED
#define SHADERS3D_H_INCLUDED

#include "../GestionShaders.h"

class Shaders3d :public GestionShaders
{
    public:
        static GLint openGl_majorVersion;
        static GLint openGl_minorVersion;

        //---------------- Aplat sans éclairage:

        static GLchar* source_vertex_flat;
        static GLchar* source_fragment_flat;
        static GLuint shader_flat_id1;
        static GLuint shader_flat_id2;
        static GLuint programme_flat;
        //Uniform blocsk:
        static GLint flat_matrices;
        //Vertex attributes:
        static GLint flat_position;
        //Uniforms:
        static GLint flat_couleur;

        //---------------- Eclairage Gouraud:

        static GLchar* source_vertex_gouraud2si;
        static GLchar* source_fragment_gouraud2si;
        static GLuint shader_gouraud2si_id1;
        static GLuint shader_gouraud2si_id2;
        static GLuint programme_gouraud2si;

        //Uniform blocks:
        static GLint gouraud2si_matrices;
        static GLint gouraud2si_eclairages;
        //Vertex attributes:
        static GLint gouraud2si_position;
        static GLint gouraud2si_normale;
        //Uniforms
        static GLint gouraud2si_materiau_ambiance;
        static GLint gouraud2si_materiau_diffusion;
        static GLint gouraud2si_materiau_luminosite;
        static GLint gouraud2si_materiau_specularite;
        static GLint gouraud2si_materiau_brillance;

        //---------------- Eclairage Phong:

        static GLchar* source_vertex_phong2si;
        static GLchar* source_fragment_phong2si;
        static GLuint shader_phong2si_id1;
        static GLuint shader_phong2si_id2;
        static GLuint programme_phong2si;

        //Uniform blocks:
        static GLint phong2si_matrices;
        static GLint phong2si_eclairages;
        //Vertex attributes:
        static GLint phong2si_position;
        static GLint phong2si_normale;
        //Uniforms
        static GLint phong2si_materiau_ambiance;
        static GLint phong2si_materiau_diffusion;
        static GLint phong2si_materiau_luminosite;
        static GLint phong2si_materiau_specularite;
        static GLint phong2si_materiau_brillance;


        //---------------- Texture sans éclairage:

        static GLchar* source_vertex_texture;
        static GLchar* source_fragment_texture;
        static GLuint shader_texture_id1;
        static GLuint shader_texture_id2;
        static GLuint programme_texture;
        //Uniform blocsk:
        static GLint texture_matrices;
        //Vertex attributes:
        static GLint texture_position;
        static GLint texture_coordonnees_UV;
        //Uniforms:
        static GLint texture_couleur;
        static GLint texture_texture;


        //---------------- Texture avec éclairage Gouraud 2 sources infinies:

        static GLchar* source_vertex_texture_gouraud2si;
        static GLchar* source_fragment_texture_gouraud2si;
        static GLuint shader_texture_gouraud2si_id1;
        static GLuint shader_texture_gouraud2si_id2;
        static GLuint programme_texture_gouraud2si;

        //Uniform blocks:
        static GLint texture_gouraud2si_matrices;
        static GLint texture_gouraud2si_eclairages;
        //Vertex attributes:
        static GLint texture_gouraud2si_position;
        static GLint texture_gouraud2si_normale;
        static GLint texture_gouraud2si_coordonnees_UV;
        //Uniforms
        static GLint texture_gouraud2si_texture;
        static GLint texture_gouraud2si_materiau_ambiance;
        static GLint texture_gouraud2si_materiau_diffusion;
        static GLint texture_gouraud2si_materiau_luminosite;
        static GLint texture_gouraud2si_materiau_specularite;
        static GLint texture_gouraud2si_materiau_brillance;

        //---------------- Texture avec éclairage Phong 2 sources infinies:

        static GLchar* source_vertex_texture_phong2si;
        static GLchar* source_fragment_texture_phong2si;
        static GLuint shader_texture_phong2si_id1;
        static GLuint shader_texture_phong2si_id2;
        static GLuint programme_texture_phong2si;

        //Uniform blocks:
        static GLint texture_phong2si_matrices;
        static GLint texture_phong2si_eclairages;
        //Vertex attributes:
        static GLint texture_phong2si_position;
        static GLint texture_phong2si_normale;
        static GLint texture_phong2si_coordonnees_UV;
        //Uniforms
        static GLint texture_phong2si_texture;
        static GLint texture_phong2si_materiau_ambiance;
        static GLint texture_phong2si_materiau_diffusion;
        static GLint texture_phong2si_materiau_luminosite;
        static GLint texture_phong2si_materiau_specularite;
        static GLint texture_phong2si_materiau_brillance;

    private:
        static void supprime_shader(GLuint programme, GLchar* source_vertex, GLchar* source_fragment,GLuint shader_id1,GLuint shader_id2 );
    public:
        static bool init_shader_flat();
        static bool init_shader_gouraud_2si();
        static bool init_shader_phong_2si();
        static bool init_shader_texture();
        static bool init_shader_texture_gouraud2si();
        static bool init_shader_texture_phong2si();

        static bool initShaders();
        static void freeShaders();
};

#endif // SHADERS3D_H_INCLUDED
