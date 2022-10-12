//******************************************************************
//
//		Ray marching
//
//*******************************************************************

#version 400
#define NBR_PAS_MAX 50
#define DISTANCE_MIN 1.e-1
#define ECHELLE 5000.
#define LOGBASE log(4.)

#define EPSILON_NORMALE 1.e-1

const float INFINITY=1./0.;

layout (location=0) out vec4 outColor;

uniform int iter;
uniform float t;
uniform vec3 obs_pos;		//Matrice de l'observateur
uniform mat3 obs_mat_normale;		//Matrice normale de l'observateur
uniform float zMin;
uniform float zMax;
uniform float fogZmin;
uniform float fogZmax;
uniform vec3 l1_position;
uniform vec3 l2_position;
uniform vec3 l3_position;

in vec3 rayObs_dir;

struct Ray
{
	vec3 pos;
	vec3 dir;
	vec3 posMarche;
	int nbrPas;
	float dist;
	float distTotale;
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
    //return max(0.0,length(posRel)-s.r);
    return (length(posRel)-s.r);
}

//Estimateur de distance pour une sphère avec modulo:
float de_sphere_modulo(Sphere s,vec3 p)
{
    vec3 posRel=vec3(mod(p.x,120.)-60.,mod(p.y,134.)-67.,mod(p.z,90.)-45.);
    return (length(posRel)-s.r+50.*sin(.003212*p.x+t*0.815)*cos(.0023*p.y+t*.183)*cos(0.00927*p.z+t*0.285));
}

float de_sphere_modulo2(Sphere s,vec3 p)
{
    float var=20.*sin(.014*p.x+t*0.146)*cos(.043*p.y+t*.33)*cos(0.024*p.z+t*.5);
    vec3 posRel=vec3(mod(p.x,400.)-200.,mod(p.y,314.)-157.,mod(p.z,490.)-245.);
    return (length(posRel)-s.r-var);
}


float de_boite(vec3 boite, vec3 p)
{
    vec3 d=abs(p)-boite;
    return min(max(d.x,max(d.y,d.z)),0.)+length(max(d,0.));
}

float de_boite2d(vec2 boite, vec2 p)
{
    vec2 d=abs(p)-boite;
    return min(max(d.x,d.y),0.)+length(max(d,0.));
}

float de_croix(vec3 p )
{
  float da = de_boite2d(vec2(1.0),p.xy);
  float db = de_boite2d(vec2(1.0),p.yz);
  float dc = de_boite2d(vec2(1.0),p.zx);
  return min(da,min(db,dc));
}

float de_boite_ronde(vec3 boite, float r,vec3 p)
{

    return length(max(abs(p)-boite,0.))-r;
}

float de_boite_ronde_modulo(vec3 boite, float r,vec3 p)
{
    vec3 posRel=vec3(mod(p.x,20.)-10.,mod(p.y,30.)-15.,mod(p.z,14.)-7.);
    return (length(max(abs(posRel)-boite,0.))-r);
}


float de_mandelbulb(vec3 pos)
{
    float Power=8.;
	vec3 z = pos;
	float dr = 1.0;
	float r = 0.0;
	for (int i = 0; i < iter ; i++)
    {
		r = length(z);
		if (r>2.) break;

		// convert to polar coordinates
		float theta = acos(z.z/r);
		float phi = atan(z.y,z.x);
		dr =  pow( r, Power-1.0)*Power*dr + 1.0;

		// scale and rotate the point
		float zr = pow( r,Power);
		theta = theta*Power;
		phi = phi*Power;

		// convert back to cartesian coordinates
		z = zr*vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
		z+=pos;
	}
	return (0.5*log(r)*r/dr);
}


float de_BistorBrot(vec3 p)
{

        float Cr=p.x;
        float Ci=p.y;
        float Cj=p.z;

        float Wr=0.;
        float Wi=0.;
        float Wj=0.;
        float tmpr,tmpi;

        float Wpr=1.;
        float Wpi=0.;
        float Wpj=0.;

        int i=0;

        float d=0.;//sqrt(Cr*Cr+Ci*Ci+Cj*Cj);
        float n,np;

        //if(d>2.) return (d-1.99);

        //d=0.;

        while(Wr*Wr+Wi*Wi+Wj*Wj<10. && i<iter*2)
        {
                tmpr=2.*(Wr*Wpr-Wi*Wpi-Wj*Wpj)+1.;
                tmpi=2.*(Wr*Wpi+Wi*Wpr+Wj*Wpi);
                Wpj=2.*(Wr*Wpj-Wi*Wpj+Wj*Wpr);
                Wpr=tmpr;
                Wpi=tmpi;

                tmpr=Wr*Wr-Wi*Wi-Wj*Wj+Cr;
                tmpi=2.*Wr*Wi+Wi*Wj+Ci;
                Wj=2.*Wr*Wj-Wi*Wj+Cj;
                Wr=tmpr;
                Wi=tmpi;

                i++;
        }


        //Distance à l'ensemble:
        n=sqrt(Wr*Wr+Wi*Wi+Wj*Wj);
        np=sqrt(Wpr*Wpr+Wpi*Wpi+Wpj*Wpj);
        d=0.5*log(n)*n/np;


        return d;
}

float de_menger( vec3 p )
{
   float d = de_boite(vec3(1.0),p);

   float s = 1.0;
   for( int m=0; m<iter; m++ )
   {
      vec3 a = mod( p*s, 2.0 )-2.;
      s *= 3.0;
      vec3 r = 1. - 3.0*abs(a);

      float c = de_croix(r)/s;
      d = max(d,-c);
   }

   return d;
}

Ray rayObs,rayAbs;
Sphere s1,s2,ls1,ls2,ls3;
float dist_l1,dist_l2,dist_l3;


float estimation_scene(vec3 p)
{
    float dist_temp_forme,dist_temp_forme2,dist_temp_l1,dist_temp_smod1,dist_temp_smod2;


    dist_temp_forme=de_menger(p/float(ECHELLE))*float(ECHELLE);
     dist_temp_forme2=de_BistorBrot(p/float(2500.))*float(2500.);
    //dist_temp_forme=de_boite_ronde(vec3(50.,30.,40.),10.,p);
    //dist_temp_smod1=de_sphere(s1,rayAbs.posMarche);



    //dist_temp_a=de_mandelbulb(rayAbs.posMarche/ECHELLE)*ECHELLE;
    //dist_temp=max(max(dist_temp_a,-dist_temp_b),dist_temp);

    /*
    dist_temp_forme=de_menger(p/ECHELLE)*ECHELLE;
    dist_temp_l1=de_sphere_modulo2(s2,p);
    dist_temp_smod1=de_sphere_modulo(s1,p);
    return max(max(dist_temp_l1,-dist_temp_smod1),dist_temp_forme);
    */

    //dist_temp_forme=de_menger(p/ECHELLE)*ECHELLE;
    //dist_temp_l1=de_sphere_modulo2(s2,p);
    //dist_temp_smod1=de_sphere_modulo(s1,p);
    //return max(dist_temp_forme,-dist_temp_smod1);



    return max(dist_temp_forme,-dist_temp_forme2);

    //return max(dist_temp_forme,-dist_temp_smod1);
    //return dist_temp_l1;


    //return dist_temp_forme;
}

void main( void )
{

        vec3 normale_forme=vec3(0.,0.,0.);
        vec3 xDir=vec3(EPSILON_NORMALE,0.,0.);
        vec3 yDir=vec3(0.,EPSILON_NORMALE,0.);
        vec3 zDir=vec3(0.,0.,EPSILON_NORMALE);

        float dist_temp;
        float zFrag=-zMax;

        int i;
        s1.pos.xyz=vec3(0.,0.,0.);
        s2.r=150.;
        s1.r=50.;
        ls1.pos=l1_position;
        ls2.pos=l2_position;
        ls3.pos=l3_position;
        ls1.r=10.;
        ls2.r=1.;
        ls3.r=1.;


    //Rayon avec les coordonnées du rectangle:

        rayObs.pos=vec3(0.,0.,0.);
        rayObs.dir=normalize(rayObs_dir);

    //Calcul la position et le vecteur directeur du rayon dans l'espace absolu (pour le moment l'observateur est dans le même repère que l'espace)
        rayAbs.pos=obs_pos;
        rayAbs.dir=obs_mat_normale*rayObs.dir;

    //Couleur ciel:
    vec3 couleur_brouillard=vec3(7./255.,12./255.,50./255.);
    vec3 couleur=couleur_brouillard*.5;

    //Marche:

        rayAbs.posMarche=rayAbs.pos;
        rayAbs.dist=0.;
        rayAbs.nbrPas=0;
        for(i=0;i<int(NBR_PAS_MAX);i++)
        {


            dist_temp=estimation_scene(rayAbs.posMarche);
            //dist_temp=de_mandelbulb(rayAbs.posMarche/ECHELLE)*ECHELLE;
            //Calcul la normale:
            if(dist_temp<=float(DISTANCE_MIN))
            {

                /*
                float val=1.-(float(rayAbs.nbrPas)/float(NBR_PAS_MAX));
                couleur=vec3(val*254./255.,val*224./255.,val*155./255.);
                */
                /*
                normale_forme=normalize(vec3(de_sphere_modulo2(s2,rayAbs.posMarche+xDir)-de_sphere_modulo2(s2,rayAbs.posMarche-xDir),
                                         de_sphere_modulo2(s2,rayAbs.posMarche+yDir)-de_sphere_modulo2(s2,rayAbs.posMarche-yDir),
                                         de_sphere_modulo2(s2,rayAbs.posMarche+zDir)-de_sphere_modulo2(s2,rayAbs.posMarche-zDir))
                                    );*/


                normale_forme=normalize(vec3(estimation_scene(rayAbs.posMarche+xDir)-estimation_scene(rayAbs.posMarche-xDir),
                                         estimation_scene(rayAbs.posMarche+yDir)-estimation_scene(rayAbs.posMarche-yDir),
                                         estimation_scene(rayAbs.posMarche+zDir)-estimation_scene(rayAbs.posMarche-zDir))
                                    );



                vec3 lumiere_ambiante=couleur_brouillard*0.1;


                vec3 l1_couleur=vec3(.1,.5,1.);
                vec3 l1_ambiance=couleur_brouillard*0.1;
                vec3 l1_direction=l1_position-rayAbs.posMarche;
                float l1_distance=length(l1_direction);
                l1_direction=normalize(l1_direction);
                float l1_attenuation_constante=0.001;
                float l1_attenuation=1./(l1_attenuation_constante*l1_distance);
                float angle_source1_normale  = max(dot(normale_forme,l1_direction),0.);

                vec3 l2_couleur=vec3(1.,.4,.1);
                vec3 l2_ambiance=couleur_brouillard*0.1;
                vec3 l2_direction=l2_position-rayAbs.posMarche;
                float l2_distance=length(l2_direction);
                l2_direction=normalize(l2_direction);
                float l2_attenuation_constante=0.01/(5.5+2.5*cos(t*2));
                float l2_attenuation=1./(l2_attenuation_constante*l2_distance);
                float angle_source2_normale  = max(dot(normale_forme,l2_direction),0.);

                vec3 l3_couleur=vec3(.25,1.,.25);
                vec3 l3_ambiance=couleur_brouillard*0.1;
                vec3 l3_direction=l3_position-rayAbs.posMarche;
                float l3_distance=length(l3_direction);
                l3_direction=normalize(l3_direction);
                float l3_attenuation_constante=0.001;
                float l3_attenuation=1./(l3_attenuation_constante*l3_distance);
                float angle_source3_normale  = max(dot(normale_forme,l3_direction),0.);

                float occlusion=1.;//-.75*float(rayAbs.nbrPas)/float(100.);

                vec3 materiau_luminosite=vec3(0.,0.,0.);
                vec3 materiau_ambiance=vec3(1.,1.,1.);
                vec3 materiau_diffusion=vec3(254./255.,224./255.,155./255.);
                couleur    = 	materiau_luminosite + materiau_ambiance * (l1_ambiance+l2_ambiance+l3_ambiance) * lumiere_ambiante;
                couleur    += 	occlusion*materiau_diffusion * (l1_couleur *  angle_source1_normale * l1_attenuation
                                                     + l2_couleur *  angle_source2_normale * l2_attenuation
                                                     + l3_couleur *  angle_source3_normale * l3_attenuation);




                break;
            }
            rayAbs.dist+=dist_temp;
            rayAbs.nbrPas++;
            if(rayAbs.dist>zMax)break;
            rayAbs.posMarche=rayAbs.pos+rayAbs.dir*rayAbs.dist;
        }
        //if(i==NBR_PAS_MAX)couleur=vec3(0.,1.,0.);
        zFrag=-rayAbs.dist*rayObs.dir.z;

        couleur+=float(rayAbs.nbrPas)*vec3(0.5,.0,1.0)*.005;


        float facteur_brouillard=clamp((rayAbs.dist-fogZmin)/(fogZmax-fogZmin),0.,1.);

        outColor = vec4(mix(couleur,couleur_brouillard,facteur_brouillard),1.);

        float a=zMax/(zMax-zMin);
        float b=zMax*zMin/(zMin-zMax);
        gl_FragDepth=a+b/zFrag;


}
