#version 330

layout (location=0) out vec4 outColor;

uniform sampler2D texture_profondeurs;
uniform vec2 taille_texture_sortie;
uniform float echelle_profondeur;

//varying vec2 coordonnees_texture;
#define M_PIS2 1.5708
#define EPSILON_PROFONDEUR 1e-6

void main( void )
{
    float x0=gl_FragCoord.x/taille_texture_sortie.x;
    float y0=gl_FragCoord.y/taille_texture_sortie.y;
    float profondeur0=vec4(texture( texture_profondeurs, vec2(x0,y0) )).r*echelle_profondeur;

    float angleMin=M_PIS2;
    float angle;
    vec3 vCone;
    float x1,y1,profondeur1;
    int px,py;
    int l_tex=int(taille_texture_sortie.x);
    int h_tex=int(taille_texture_sortie.y);

    for(py=0;py<h_tex;py++)
    {
        for(px=0;px<l_tex;px++)
        {
            x1=float(px)/taille_texture_sortie.x;
            y1=float(py)/taille_texture_sortie.y;
            profondeur1=texture( texture_profondeurs, vec2(x1,y1) ).r*echelle_profondeur;
            if(profondeur1-EPSILON_PROFONDEUR<profondeur0)
            {
                vCone=vec3(x1-x0,y1-y0,profondeur0-profondeur1);
                angle=acos(normalize(vCone).z);
                if(angle<angleMin)angleMin=angle;
            }
        }
    }


    //vec4 texel=texture2D( texture_profondeurs, coordonnees_texture );

	outColor = vec4(profondeur0,angleMin/M_PIS2,0.,1.);
}
