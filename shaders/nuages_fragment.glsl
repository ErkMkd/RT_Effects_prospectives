#version 330

layout (location=0) out vec4 outColor;
in vec2 position_texCoords;
in vec3 position_sommet;

uniform sampler2D texture_octave1;
uniform sampler2D texture_octave2;
uniform sampler2D texture_octave3;
uniform float t;

const vec3 couleur_nuages=const vec3(.1,0.,.1);

layout(std140) uniform eclairages_2si
{
    vec4 lumiere_ambiante;
    vec4 couleur_brouillard;
    float brouillard_min;
    float brouillard_max;

    vec3  l1_direction; ///La direction doit avoir été calculée dans le bon repère au moment de l'initialisation du bloc
    vec4  l1_couleur;   //L'intensité est gérée en amont
    vec4  l1_ambiance;

    vec3  l2_direction;
    vec4  l2_couleur;
    vec4  l2_ambiance;
};


void main()
{
    float dist=length(position_sommet);
    float facteur_brouillard=1.-clamp((dist-brouillard_min)/(brouillard_max-brouillard_min),0.,1.);

    outColor = vec4(couleur_nuages,
                facteur_brouillard*(texture(texture_octave1,position_texCoords+t*0.00001)*
                                                (texture(texture_octave2,position_texCoords-t*0.0001)
                                                + texture(texture_octave3,position_texCoords+t*0.001))
                )/1.);
}
