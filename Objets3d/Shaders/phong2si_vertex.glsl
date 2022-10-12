/************************************************************************

    Eclairage Phong, deux sources lumineuses infinies (2si)

/************************************************************************/


#version 330

in vec3 position;
in vec3 normale;

layout(std140) uniform matrices
{
    mat4 mat_projection;
    mat4 mat_modelView;
    mat4 mat_modelViewProjection;
    mat3 mat_normale;
};

out vec3 position_sommet;
out vec3 normale_sommet;

void main()
{
    gl_Position = mat_modelViewProjection * vec4(position, 1.0);

    position_sommet    = (mat_modelView * vec4(position, 1.0)).xyz;
    normale_sommet     = mat_normale * normale;
}

