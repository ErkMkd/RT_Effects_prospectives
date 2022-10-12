/************************************************************************

    Eclairage Phong, deux sources lumineuses infinies (2si)
    Sert pour les primitives instanciées.

/************************************************************************/


#version 330

in vec3 position;
in vec3 normale;

uniform mat4 matrice_instance;           //La matrice de l'instance
uniform mat3 matrice_normale_instance;           //La matrice de l'instance

layout(std140) uniform matrices
{
    mat4 mat_projection;
    mat4 mat_modelView;
    mat4 mat_modelViewProjection;   //la matrice du conteneur des instances
    mat3 mat_normale;
};

out vec3 position_sommet;
out vec3 normale_sommet;

void main()
{
    gl_Position = mat_modelViewProjection * matrice_instance * vec4(position, 1.0);

    position_sommet    = (mat_modelView * matrice_instance * vec4(position, 1.0)).xyz;
    normale_sommet     = mat_normale * matrice_normale_instance * normale;
}

