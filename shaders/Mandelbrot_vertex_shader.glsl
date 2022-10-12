#version 400

in vec2 position;

uniform mat4 matrice_projection;

void main( void )
{
    gl_Position = matrice_projection * vec4(position,-1.0, 1.0);
}
