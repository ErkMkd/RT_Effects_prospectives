//******************************************************************
//
//		Ray marching
//
//*******************************************************************

#version 400
#define ECRAN_RATIO 1.f
#define NBR_PAS_MAX 200
#define DISTANCE_MIN 1e-4

const float INFINITY=1./0.;

layout (location=0) out vec4 outColor;

uniform float t;
uniform  vec2 ecranDim;	//Dimensions de l'écran
uniform float distanceFocale;
uniform mat4 obs_mat;		//Matrice de l'observateur

struct Ray
{
	vec3 pos;
	vec3 dir;
	vec3 posMarche;
	int nbrPas;
	float dist;
};

struct Sphere
{
    vec3 pos;
    float r;
};

//Estimateur de distance pour une simple sphère:
float de_sphere(Sphere s,vec3 p)
{
    vec3 posRel=s.pos-p;
    return max(0.0,length(posRel)-s.r);
}

//Estimateur de distance pour une sphère avec modulo:
float de_sphere_modulo(Sphere s,vec3 p)
{
    vec3 posRel=vec3(mod(p.x,320.)-160.,mod(p.y,360.)-180.,mod(p.z,200.)-100.);
    return abs(length(posRel)-s.r+90.*cos(.24*p.z+t*2.1)*sin(0.13*p.y+t*.023)*cos(0.06*p.z+t*2.45));
}

void main( void )
{
        Ray rayObs,rayAbs;
        Sphere s1;
        float dist_temp;
        s1.pos.xyz=vec3(1.,1.,10.);
        s1.r=60.;
        int i;

    //Calcul le vecteur directeur du rayon dans le repère de l'observateur:
        float x=(gl_FragCoord.x-(ecranDim.x/2));
        float y=(gl_FragCoord.y-(ecranDim.y/2))*ECRAN_RATIO;

    //Calcul le vecteur directeur du rayon dans le repère de l'observateur:
        rayObs.pos=vec3(0.,0.,0.);
        rayObs.dir=normalize(vec3(x,y,distanceFocale));

    //Calcul la position et le vecteur directeur du rayon dans l'espace absolu (pour le moment l'observateur est dans le même repère que l'espace)
        rayAbs.pos=(obs_mat*vec4(rayObs.pos,1.)).xyz;
        rayAbs.dir=(obs_mat*vec4(rayObs.dir,1.)).xyz-rayAbs.pos;

    //Couleur ciel:
    vec3 couleur_brouillard=vec3(0.4,0.1,0.3);
    vec3 couleur=vec3(0.,0.,0.);

    //Calcul sphère:
        rayAbs.posMarche=rayAbs.pos;
        rayAbs.dist=INFINITY;
        rayAbs.nbrPas=0;
        bool hit=false;
        for(i=0;i<NBR_PAS_MAX;i++)
        {

            dist_temp=de_sphere_modulo(s1,rayAbs.posMarche);

            //if(dist_temp>rayAbs.dist) break;

            if(dist_temp<=DISTANCE_MIN)
            {
                float val=1.-(float(rayAbs.nbrPas)/float(NBR_PAS_MAX));
                couleur=vec3(val,val*0.75,val*0.1);
                break;
            }
            rayAbs.nbrPas++;
            rayAbs.dist=dist_temp;
            rayAbs.posMarche+=rayAbs.dir*rayAbs.dist;
        }

        couleur+=float(rayAbs.nbrPas)*vec3(1.0,0.,0.0)*.02;
        float fogZmin=1.;
        float fogZmax=1000.;

        float distFrag=length(rayAbs.posMarche-rayAbs.pos);
        float facteur_brouillard=clamp((distFrag-fogZmin)/(fogZmax-fogZmin),0.,1.);

        outColor = vec4(mix(couleur,couleur_brouillard,facteur_brouillard),1.);
}
