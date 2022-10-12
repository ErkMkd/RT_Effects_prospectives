/************************************************************************

    Eclairage Gouraud, deux sources lumineuses infinies (2si)

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

uniform vec4 materiau_ambiance;
uniform vec4 materiau_diffusion;
uniform vec4 materiau_luminosite;
uniform vec4 materiau_specularite;
uniform float materiau_brillance;

out vec4 couleur_rgba;

void main()
{
    gl_Position = mat_modelViewProjection * vec4(position, 1.0);

    vec3 position_sommet    = (mat_modelView * vec4(position, 1.0)).xyz;
    vec3 normale_sommet     = normalize(mat_normale * normale);
	vec3 direction_oeil 	= -normalize(position_sommet);

    float angle_source1_normale  = max(dot(normale_sommet,l1_direction),0.);
    float angle_source2_normale  = max(dot(normale_sommet,l2_direction),0.);

    //Ambiance , diffusion, luminosité:
    couleur_rgba    = 	materiau_luminosite + materiau_ambiance * (l1_ambiance+l2_ambiance) * lumiere_ambiante;
    couleur_rgba    += 	materiau_diffusion * (l1_couleur *  angle_source1_normale + l2_couleur*angle_source2_normale);

    //Lumière spéculaire:
    vec3 reflet_source;

    if (angle_source1_normale>0.)
    {
        reflet_source	=   normalize(direction_oeil+l1_direction);
        couleur_rgba    +=   l1_couleur * materiau_specularite * pow(max(dot(reflet_source,normale_sommet),0.),materiau_brillance);
    }

    if (angle_source2_normale>0.)
    {
        reflet_source	=   normalize(direction_oeil+l2_direction);
        couleur_rgba    +=   l2_couleur * materiau_specularite * pow(max(dot(reflet_source,normale_sommet),0.),materiau_brillance);
    }

    //Brouillard:
    float distFrag=length(position_sommet);
    float facteur_brouillard=clamp((distFrag-brouillard_min)/(brouillard_max-brouillard_min),0.,1.);

    couleur_rgba = clamp(mix(couleur_rgba,couleur_brouillard,facteur_brouillard),0.,1.);
}

