#version 330

in vec3 position;
out vec3 position_sommet;
out vec2 position_texCoords;

layout(std140) uniform matrices
{
    mat4 mat_projection;
    mat4 mat_modelView;
    mat4 mat_modelViewProjection;
    mat3 mat_normale;
};

uniform vec3 position_observateur;
uniform float zMin;
uniform float zMax;

void main()
{
    position_sommet=(mat_modelView*vec4(position*vec3(zMax,1.,zMax),1.)).xyz;
    gl_Position=mat_projection*(vec4(position_sommet,1.));
    position_texCoords=position.xz/2.+vec2(.5,.5)+(vec2(position_observateur.x,position_observateur.z)/(zMax*2.));
}
