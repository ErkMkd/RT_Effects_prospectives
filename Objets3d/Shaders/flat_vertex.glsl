#version 330

in vec3 position;

layout(std140) uniform matrices
{
    mat4 mat_projection;
    mat4 mat_modelView;
    mat4 mat_modelViewProjection;
    mat3 mat_normale;
};

uniform vec4 couleur;

out vec4 couleur_rgba;

void main()
{
    couleur_rgba=couleur;
    gl_Position = mat_modelViewProjection * vec4(position, 1.0);
}

