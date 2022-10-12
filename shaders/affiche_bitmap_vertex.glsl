#version 330

in vec2 position;
out vec2 position_texCoords;

uniform float echelle;
uniform vec2 taille_ecran;
uniform vec2 taille_texture;
uniform vec2 position_bitmap;

void main()
{
    position_texCoords=position/2.+vec2(.5,.5);
    float xmax=echelle/taille_ecran.x*taille_texture.x;
    float ymax=echelle/taille_ecran.y*taille_texture.y;
    gl_Position = vec4((position+vec2(1.,1.))*vec2(xmax/2.,ymax/2.)-vec2(1.,1.)+position_bitmap/taille_ecran,0.0, 1.0);
}
