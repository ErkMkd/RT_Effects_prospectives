#version 330

layout (location=0) out vec4 outColor;
in vec2 position_texCoords;
uniform sampler2D texture_couleur;
uniform sampler2D texture_zBuffer;
uniform vec2 taille_ecran;
uniform float zMin;
uniform float zMax;

uniform float zMin_nette;   //Zone de netteté
uniform float zMax_nette;

varying vec3 rayObs_dir;


vec4 floutage(float blurFacteur)
{
    vec2 pixSize=vec2(1./taille_ecran.x,1./taille_ecran.y);
    vec4 couleur=texture( texture_couleur, position_texCoords+blurFacteur*vec2(0.,-pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(pixSize.x,0.));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(0.,pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(-pixSize.x,0.));
    couleur+=texture( texture_couleur, position_texCoords-blurFacteur*pixSize);
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*pixSize);
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(-pixSize.x,pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(pixSize.x,-pixSize.y));

    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(0.,-2*pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(2*pixSize.x,0.));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(0.,2*pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(-2*pixSize.x,0.));

    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(2.*pixSize.x,pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(pixSize.x,2.*pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(-pixSize.x,2.*pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(-2.*pixSize.x,pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(2.*pixSize.x,-pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(pixSize.x,2.*-pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(-pixSize.x,2.*-pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(-2.*pixSize.x,-pixSize.y));

    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(3.*pixSize.x,0.));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(3.*pixSize.x,pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(2.*pixSize.x,2.*pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(1.*pixSize.x,3.*pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(0.,3.*pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(-1.*pixSize.x,3.*pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(-2.*pixSize.x,2.*pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(-3.*pixSize.x,pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(-3.*pixSize.x,0.));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(3.*pixSize.x,-pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(2.*pixSize.x,-2.*pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(1.*pixSize.x,-3.*pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(0.,-3.*pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(-1.*pixSize.x,-3.*pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(-2.*pixSize.x,-2.*pixSize.y));
    couleur+=texture( texture_couleur, position_texCoords+blurFacteur*vec2(-3.*pixSize.x,-pixSize.y));


    //couleur+=texture( texture_couleur, position_texCoords);

    return couleur/36.;
}

void main()
{
    float normeZ=normalize(rayObs_dir).z;
    float fragDepth=texture(texture_zBuffer,position_texCoords);
    float a=zMax/(zMax-zMin);
    float b=zMax*zMin/(zMin-zMax);
    float zFrag=(b/(fragDepth-a));
    float distFrag=zFrag/normeZ;
    float blurFacteur;
    float facteur_flou=3.;
    vec4 couleur;

    if(distFrag<zMin_nette)
    {
        blurFacteur=facteur_flou*(1.-distFrag/zMin_nette);
        couleur=floutage(blurFacteur);
    }
    else if(distFrag>zMax_nette)
    {
        a=zMax/(zMax-zMax_nette);
        b=zMax*zMax_nette/(zMax_nette-zMax);
        blurFacteur=facteur_flou*(a+b/distFrag);
        couleur=floutage(blurFacteur);
    }
    else couleur=texture(texture_couleur,position_texCoords);


    //gl_FragDepth=fragDepth;//texture(texture_zBuffer,position_texCoords);
    outColor = couleur;//texture(texture_couleur,position_texCoords);
}
