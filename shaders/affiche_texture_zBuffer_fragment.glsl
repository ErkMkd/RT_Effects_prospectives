#version 330

/*
    Comme ce shader sert à superposer deux éléments de résolutions différentes, un tramage est appliqué
*/

layout (location=0) out vec4 outColor;

in vec2 position_texCoords;
uniform sampler2D texture_couleur;
uniform sampler2D texture_zBuffer;
uniform float taille_tramage;   //en pixels
uniform vec2 taille_ecran;

void main()
{
    vec2 coordsTexReflet;
    int fx=int(gl_FragCoord.x) & 0x1;
    int fy=int(gl_FragCoord.y) & 0x1;

    if(fy==0 && fx==0 )
    {

            coordsTexReflet=vec2((gl_FragCoord.x+taille_tramage)/taille_ecran.x,(gl_FragCoord.y+taille_tramage)/taille_ecran.y);

    }
    else if(fy==1 && fx==1 )
    {

            coordsTexReflet=vec2((gl_FragCoord.x-taille_tramage)/taille_ecran.x,(gl_FragCoord.y-taille_tramage)/taille_ecran.y);

    }

    else
    {
       coordsTexReflet=vec2((gl_FragCoord.x)/taille_ecran.x,(gl_FragCoord.y)/taille_ecran.y);
    }

    /*
    vec2 pixSize=vec2(1./800.,1./450.);
    float blurFacteur=min(zFrag/1500.,2.);
    vec4 couleur=texture( texture_couleur, coordsTexReflet+blurFacteur*vec2(0.,-pixSize.y));
    couleur+=texture( texture_couleur, coordsTexReflet+blurFacteur*vec2(pixSize.x,0.));
    couleur+=texture( texture_couleur, coordsTexReflet+blurFacteur*vec2(0.,pixSize.y));
    couleur+=texture( texture_couleur, coordsTexReflet+blurFacteur*vec2(-pixSize.x,0.));
    couleur+=texture( texture_couleur, coordsTexReflet-blurFacteur*pixSize);
    couleur+=texture( texture_couleur, coordsTexReflet+blurFacteur*pixSize);
    couleur+=texture( texture_couleur, coordsTexReflet+blurFacteur*vec2(-pixSize.x,pixSize.y));
    couleur+=texture( texture_couleur, coordsTexReflet+blurFacteur*vec2(pixSize.x,-pixSize.y));
    couleur+=texture( texture_couleur, coordsTexReflet+blurFacteur*vec2(0.,-2*pixSize.y));
    couleur+=texture( texture_couleur, coordsTexReflet+blurFacteur*vec2(2*pixSize.x,0.));
    couleur+=texture( texture_couleur, coordsTexReflet+blurFacteur*vec2(0.,2*pixSize.y));
    couleur+=texture( texture_couleur, coordsTexReflet+blurFacteur*vec2(-2*pixSize.x,0.));

    couleur/=12.;
    */


    gl_FragDepth=texture(texture_zBuffer,position_texCoords);
    outColor = texture(texture_couleur,coordsTexReflet);
}
