#version 330

in vec3 position;
in vec2 coordonnees_UV;

layout(std140) uniform matrices
{
    mat4 mat_projection;
    mat4 mat_modelView;
    mat4 mat_modelViewProjection;
    mat3 mat_normale;
};

uniform vec4 couleur;

out vec4 couleur_rgba;
out vec2 coordonnees_texture;

void main()
{

    gl_Position = mat_modelViewProjection * vec4(position, 1.0);
    couleur_rgba=couleur;
    coordonnees_texture = coordonnees_UV.st;
}

