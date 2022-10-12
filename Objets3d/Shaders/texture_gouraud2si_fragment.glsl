#version 330

layout (location=0) out vec4 outColor;

in vec4 couleur_rgba;
in vec2 coordonnees_texture;

uniform sampler2D texture_couleur;

void main()
{
    vec4 texel = texture2D(texture_couleur,coordonnees_texture);
    outColor = couleur_rgba*texel;
}
