#version 330

layout (location=0) out vec4 outColor;

in vec4 couleur_rgba;

void main()
{
    outColor = couleur_rgba;
}
