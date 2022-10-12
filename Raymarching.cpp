#include "Raymarching.h"

/*

Estimateur de distance MandelBox.
A mettre dans la boucle d'itération.

DEfactor = scale;

fixedRadius = 1.0;
fR2 = fixedRadius * fixedRadius;
minRadius = 0.5;
mR2 = minRadius * minRadius;

if (x > 1.0)
x = 2.0 - x;
else if (x < -1.0) x = -2.0 - x;
if (y > 1.0)
y = 2.0 - y;
else if (y < -1.0) y = -2.0 - y;
if (z > 1.0)
z = 2.0 - z;
else if (z < -1.0) z = -2.0 - z;

r2 = x*x + y*y + z*z;

if (r2 < mR2)
{
   x = x * fR2 / mR2;
   y = y * fR2 / mR2;
   z = z * fR2 / mR2;
   DEfactor = DEfactor * fR2 / mR2;
}
else if (r2 < fR2)
{
   x = x * fR2 / r2;
   y = y * fR2 / r2;
   z = z * fR2 / r2;
   DEfactor *= fR2 / r2;
}

x = x * scale + cx;
y = y * scale + cy;
z = z * scale + cz;
DEfactor *= scale;

resultant estimated distance (after iteration loop):

distance = sqrt(x*x+y*y+z*z)/abs(DEfactor);

*/

#define ECHELLE_FRACTALE 5000.
    Contexte3d* Raymarching::moteur3d=NULL;
    GLuint Raymarching::vbo_rm=0;
    GLuint Raymarching::vao_rm=0;
    int Raymarching::iter=3;

    float Raymarching::l1_position[3]={0.,0.,0.};//{-1300.,-1700.,-1000.};
    float Raymarching::l2_position[3]={-1300.,2000.,550.};
    float Raymarching::l3_position[3]={360.,-950.,-3900.};

    Repere* Raymarching::repere_l1s=NULL;
    Repere* Raymarching::repere_l2s=NULL;
    Repere* Raymarching::repere_l3s=NULL;
    Sphere* Raymarching::l1_sphere=NULL;
    Sphere* Raymarching::l2_sphere=NULL;
    Sphere* Raymarching::l3_sphere=NULL;

    ChargeLWO2* Raymarching::testObjetLWO2=NULL;
    ConvertionLWO2* Raymarching::testObjetConv=NULL;
    vec3f Raymarching::objetsPos(1260.,5130.,0.);


//Commandes clavier:
    bool Raymarching::dep_droite=false;
    bool Raymarching::dep_gauche=false;
    bool Raymarching::dep_haut=false;
    bool Raymarching::dep_bas=false;
    bool Raymarching::dep_avant=false;
    bool Raymarching::dep_arriere=false;

    bool Raymarching::rot_droite=false;
    bool Raymarching::rot_gauche=false;
    bool Raymarching::rot_haut=false;
    bool Raymarching::rot_bas=false;
    bool Raymarching::rot_r_gauche=false;
    bool Raymarching::rot_r_droite=false;

    bool Raymarching::iter_plus=false;
    bool Raymarching::iter_moins=false;

    bool Raymarching::inc_zMin_nette;
    bool Raymarching::dec_zMin_nette;
    bool Raymarching::inc_zMax_nette;
    bool Raymarching::dec_zMax_nette;

    int Raymarching::depId=1;

    float Raymarching::aX=0.;
    float Raymarching::aY=0.;
    float Raymarching::aZ=0.;

    float Raymarching::aR=0.;
    float Raymarching::aT=0.;
    float Raymarching::aL=0.;



bool Raymarching::init()
{
    moteur3d=new Contexte3d();
    if(moteur3d->erreur!=C3D_OK)
    {
        cout<<"ERREUR et meeerde, impossible de créer le contexte 3d - Scene RayMarching"<<endl;
        return false;
    }

    moteur3d->determine_profondeur(1.,10000.);
    moteur3d->determine_profondeur_de_champ(100.,4000.);
    moteur3d->determine_limites_brouillard(100.,10000.);
    moteur3d->determine_taille_ecran(GVars::largeur_ecran,GVars::hauteur_ecran);
    moteur3d->determine_couleur_brouillard(7./255.,12./255.,50./255.,1.);
    moteur3d->determine_couleur_fond(7./255.,12./255.,50./255.,1.);


    //moteur3d->obs.initXYZ(-0.2*ECHELLE_FRACTALE,0.*ECHELLE_FRACTALE,-.5*ECHELLE_FRACTALE);
    moteur3d->obs.initXYZ(-0.2*ECHELLE_FRACTALE,0.91*ECHELLE_FRACTALE,-.5*ECHELLE_FRACTALE);
    moteur3d->obs.initRTL(0.,0.,180.);

    GLfloat sommets_rendu[2*4]{-1.,-1., -1.,1., 1.,1., 1.,-1.};
        glGenBuffers(1,&vbo_rm);
        glGenVertexArrays(1,&vao_rm);

        glBindVertexArray(vao_rm);
            glBindBuffer(GL_ARRAY_BUFFER,vbo_rm);
                glBufferData(GL_ARRAY_BUFFER,sizeof(sommets_rendu),sommets_rendu,GL_STATIC_DRAW);
                glVertexAttribPointer(ShadersDemo::attrib_rm_position,2,GL_FLOAT,GL_FALSE,0,0);
                glEnableVertexAttribArray(ShadersDemo::attrib_rm_position);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);

    repere_l1s=new Repere();
    repere_l2s=new Repere();
    repere_l3s=new Repere();
    //l1_sphere=new Sphere(5.,8,3);
    l1_sphere=new Sphere(50.,16,16);
    l2_sphere=new Sphere(50.,16,16);
    l3_sphere=new Sphere(50.,16,16);



    //---------- Le vaisseau:


        //testObjetLWO2=new ChargeLWO2("LWO/objets_lo.lwo","LWO/vaisseaux/");
        testObjetLWO2=new ChargeLWO2("LWO/objets_boites.lwo","LWO/vaisseaux/");
        //testObjetLWO2=new ChargeLWO2("LWO/vaisseaux_hi.lwo","LWO/vaisseaux/");
        if(testObjetLWO2==NULL || testObjetLWO2->erreur!=CHARGELWO2_OK)
        {
            cout<<"MEeerdeeee - Impossible de charger ou parser le fichier LWO"<<endl;
            return false;
        }

        testObjetConv=new ConvertionLWO2(testObjetLWO2);
        if(testObjetConv==NULL || testObjetConv->erreur!=CONVERTIONLWO2_OK)
        {
            cout<<"Crotte. - Impossible de convertir l'objet LWO"<<endl;
            return false;
        }

        Volume* vaisseau=testObjetConv->renvoie_volume_index(0);
        vaisseau->determine_eclairage_phong(true);
        vaisseau->initEchelle(10.,10.,10.);
        vaisseau=testObjetConv->renvoie_volume_index(1);
        vaisseau->determine_eclairage_phong(true);
        vaisseau->initEchelle(10.,10.,10.);
        vaisseau=testObjetConv->renvoie_volume_index(2);
        vaisseau->determine_eclairage_phong(true);
        vaisseau->initEchelle(10.,10.,10.);
        vaisseau=testObjetConv->renvoie_volume_index(3);
        vaisseau->determine_eclairage_phong(true);
        vaisseau->initEchelle(10.,10.,10.);
        vaisseau=testObjetConv->renvoie_volume_index(4);
        vaisseau->determine_eclairage_phong(true);
        vaisseau->initEchelle(10.,10.,10.);
        vaisseau=testObjetConv->renvoie_volume_index(5);
        vaisseau->determine_eclairage_phong(true);
        vaisseau->initEchelle(10.,10.,10.);




    return true;
}
void Raymarching::destruction()
{
    if(l1_sphere!=NULL) delete l1_sphere;
    if(l2_sphere!=NULL) delete l2_sphere;
    if(l3_sphere!=NULL) delete l3_sphere;
    if(repere_l1s!=NULL) delete repere_l1s;
    if(repere_l2s!=NULL) delete repere_l2s;
    if(repere_l3s!=NULL) delete repere_l3s;
    if(moteur3d!=NULL) delete moteur3d;
    if (glIsBuffer(vbo_rm)) glDeleteBuffers(1,&vbo_rm);
    if (glIsVertexArray(vao_rm)) glDeleteVertexArrays(1,&vao_rm);
    if(testObjetConv!=NULL)delete testObjetConv;
    if(testObjetLWO2!=NULL)delete testObjetLWO2;

}


void Raymarching::clavier(int key,bool etat)
{

    if(!GVars::editionTrajectoire)
    {
		switch (key)
        {
                case GLFW_KEY_RIGHT:dep_droite=etat;break;
                case GLFW_KEY_LEFT:dep_gauche=etat;break;
                case GLFW_KEY_UP:dep_avant=etat ;break;
                case GLFW_KEY_DOWN:dep_arriere=etat;break;
                case GLFW_KEY_PAGE_UP:dep_haut=etat ;break;
                case GLFW_KEY_PAGE_DOWN:dep_bas=etat;break;
                case GLFW_KEY_KP_1:rot_r_gauche=etat;break;
                case GLFW_KEY_KP_3:rot_r_droite=etat;break;
                case GLFW_KEY_KP_4:rot_gauche=etat;break;
                case GLFW_KEY_KP_6:rot_droite=etat;break;
                case GLFW_KEY_KP_8:rot_bas=etat;break;
                case GLFW_KEY_KP_2:rot_haut=etat;break;

                case GLFW_KEY_KP_7:iter_moins=etat;break;
                case GLFW_KEY_KP_9:iter_plus=etat;break;

                case GLFW_KEY_F1: depId=1;break;
                case GLFW_KEY_F2: depId=2;break;
                case GLFW_KEY_F3: depId=3;break;
                case GLFW_KEY_F4: depId=4;break;
                case GLFW_KEY_F5: depId=5;break;

                case GLFW_KEY_Q:inc_zMin_nette=etat;break;
                case GLFW_KEY_W:dec_zMin_nette=etat;break;
                case GLFW_KEY_S:inc_zMax_nette=etat;break;
                case GLFW_KEY_X:dec_zMax_nette=etat;break;
        }
    }
    else
    {
        /*
        Volume* cosmo=GVars::cosmo->renvoie_volume_parent();
        EditeurTrajectoire::ET_x=cosmo->x;
        EditeurTrajectoire::ET_y=cosmo->y;
        EditeurTrajectoire::ET_z=cosmo->z;
        EditeurTrajectoire::edit_trajectoire(key,etat);
        cosmo->x=EditeurTrajectoire::ET_x;
        cosmo->y=EditeurTrajectoire::ET_y;
        cosmo->z=EditeurTrajectoire::ET_z;
        */

        switch (key)
        {

            case GLFW_KEY_RIGHT:dep_droite=etat;break;
            case GLFW_KEY_LEFT:dep_gauche=etat;break;
            case GLFW_KEY_UP:dep_avant=etat ;break;
            case GLFW_KEY_DOWN:dep_arriere=etat;break;
            case GLFW_KEY_PAGE_UP:dep_haut=etat ;break;
            case GLFW_KEY_PAGE_DOWN:dep_bas=etat;break;
            case GLFW_KEY_KP_1:rot_r_gauche=etat;break;
            case GLFW_KEY_KP_3:rot_r_droite=etat;break;
            case GLFW_KEY_KP_4:rot_gauche=etat;break;
            case GLFW_KEY_KP_6:rot_droite=etat;break;
            case GLFW_KEY_KP_8:rot_bas=etat;break;
            case GLFW_KEY_KP_2:rot_haut=etat;break;
            case GLFW_KEY_1: depId=1;break;
            case GLFW_KEY_2: depId=2;break;
            case GLFW_KEY_3: depId=3;break;
            case GLFW_KEY_4: depId=4;break;
            case GLFW_KEY_5: depId=5;break;
        }
    }
}


void Raymarching::maj_eclairages()
{

}

void Raymarching::maj_mouvements()
{
    if(iter_plus)iter++;
    else if (iter_moins){iter--;if (iter<1)iter=1;}

    float raX,raZ;

	if (depId==1)
	{

		GFunc::rotation_plan(moteur3d->obs.l,aX,aZ,raX,raZ);

		moteur3d->obs.x+=raX;
		moteur3d->obs.y+=aY;
		moteur3d->obs.z-=raZ;

		moteur3d->obs.r+=aR;
		moteur3d->obs.t-=aT;
		moteur3d->obs.l-=aL;

		if (GVars::disp_donnees)
        {
            cout<<"Obs - X="<<moteur3d->obs.x<<" Y="<<moteur3d->obs.y<<" Z="<<moteur3d->obs.z<<" R="<<moteur3d->obs.r<<" T="<<moteur3d->obs.t<<" L="<<moteur3d->obs.l<<endl;

        }
	}

	if (depId==2)
	{

		l1_position[0]+=aX;
		l1_position[1]+=aY;
		l1_position[2]+=aZ;

		if (GVars::disp_donnees)
        {
            cout<<"L1 - X="<<l1_position[0]<<" Y="<<l1_position[1]<<" Z="<<l1_position[2]<<endl;

        }
	}

	if (depId==3)
	{

		l2_position[0]+=aX;
		l2_position[1]+=aY;
		l2_position[2]+=aZ;

		if (GVars::disp_donnees)
        {
            cout<<"L1 - X="<<l2_position[0]<<" Y="<<l2_position[1]<<" Z="<<l2_position[2]<<endl;

        }
	}

    if (depId==4)
	{

		l3_position[0]+=aX;
		l3_position[1]+=aY;
		l3_position[2]+=aZ;

		if (GVars::disp_donnees)
        {
            cout<<"L1 - X="<<l3_position[0]<<" Y="<<l3_position[1]<<" Z="<<l3_position[2]<<endl;

        }
	}

	if (depId==5)
	{

		objetsPos.x+=aX;
		objetsPos.y+=aY;
		objetsPos.z+=aZ;

		if (GVars::disp_donnees)
        {
            cout<<"Objets - X="<<objetsPos.x<<" Y="<<objetsPos.y<<" Z="<<objetsPos.z<<endl;

        }
	}

	GVars::disp_donnees=false;

    float deceleration=0.75;
    float acceleration=.05;

    float decel_ang=.85;
    float accel_ang=0.07;

    aX*=deceleration;
    aY*=deceleration;
    aZ*=deceleration;

    aR*=decel_ang;
    aT*=decel_ang;
    aL*=decel_ang;

    if(dep_droite)aX=aX/deceleration+acceleration;
    if(dep_gauche)aX=aX/deceleration-acceleration;

    if(dep_haut)aY=aY/deceleration+acceleration;
    if(dep_bas)aY=aY/deceleration-acceleration;

    if(dep_avant)aZ=aZ/deceleration+acceleration;
    if(dep_arriere)aZ=aZ/deceleration-acceleration;

    if(rot_droite)aL=aL/decel_ang+accel_ang;
    if(rot_gauche)aL=aL/decel_ang-accel_ang;
    if(rot_bas)aT=aT/decel_ang+accel_ang;
    if(rot_haut)aT=aT/decel_ang-accel_ang;
    if(rot_r_droite)aR=aR/decel_ang-accel_ang;
    if(rot_r_gauche)aR=aR/decel_ang+accel_ang;

    float vitesse_modif_profChamp=10.0;
    float zminNette=moteur3d->zMin_nette;
    float zmaxNette=moteur3d->zMax_nette;
    if(inc_zMin_nette)zminNette+=vitesse_modif_profChamp;
    if(dec_zMin_nette)zminNette-=vitesse_modif_profChamp;
    if(inc_zMax_nette)zmaxNette+=vitesse_modif_profChamp;
    if(dec_zMax_nette)zmaxNette-=vitesse_modif_profChamp;
    moteur3d->determine_profondeur_de_champ(zminNette,zmaxNette);

}

void Raymarching::affiche()
{
    GVars::zMin=moteur3d->zMin;
    GVars::zMax=moteur3d->zMax;
    GVars::zMin_nette=moteur3d->zMin_nette;
    GVars::zMax_nette=moteur3d->zMax_nette;
    GVars::ratio_ecran=moteur3d->ratio_ecran;
    GVars::distance_focale=moteur3d->distanceFocale;

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_CULL_FACE);

    moteur3d->calcul_matrice_projection();
    moteur3d->obs.calcul_matrice(); //Pour le raytraceur
    moteur3d->obs.calcul_matrice_normale(); //Pour le raytraceur
    moteur3d->calcul_matrice_observateur(); //Pour les polygones
    moteur3d->maj_ubo_eclairage2si();

    glViewport(0,0,FBO_LORES_L,FBO_LORES_H);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,FrameBuffers::fbos[FBO_ECRAN_LORES]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(ShadersDemo::programme_rm);
	glUniform3f(ShadersDemo::uniform_rm_obs_pos,moteur3d->obs.x,moteur3d->obs.y,moteur3d->obs.z);
	glUniformMatrix3fv(ShadersDemo::uniform_rm_obsMat_normale,1,GL_FALSE,&moteur3d->obs.matrice_normale[0][0]);
	glUniform3fv(ShadersDemo::uniform_rm_l1_position,1,&l1_position[0]);
	glUniform3fv(ShadersDemo::uniform_rm_l2_position,1,&l2_position[0]);
	glUniform3fv(ShadersDemo::uniform_rm_l3_position,1,&l3_position[0]);
	glUniform1f(ShadersDemo::uniform_rm_ratio_ecran,moteur3d->ratio_ecran);
	glUniform1f(ShadersDemo::uniform_rm_distanceFocale,moteur3d->distanceFocale);
	glUniform1f(ShadersDemo::uniform_rm_t,GVars::temps*0.);
	glUniform1i(ShadersDemo::uniform_rm_iter,iter);
	glUniform1f(ShadersDemo::uniform_rm_zMin,moteur3d->zMin);
	glUniform1f(ShadersDemo::uniform_rm_zMax,moteur3d->zMax);
	glUniform1f(ShadersDemo::uniform_rm_fogZmin,moteur3d->brouillard_min);
	glUniform1f(ShadersDemo::uniform_rm_fogZmax,moteur3d->brouillard_max);

	glBindVertexArray(vao_rm);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    glUseProgram(0);



    //glViewport(0,0,GVars::largeur_ecran,GVars::hauteur_ecran);
    //glDisable(GL_DEPTH_TEST);


    //glDisable(GL_DEPTH_TEST);

    glViewport(0,0,RESOLUTION_X_TRAVAIL,RESOLUTION_Y_TRAVAIL);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,FrameBuffers::fbos[FBO_ECRAN_2]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GFunc::affiche_texture_zBuffer(FrameBuffers::fbos[FBO_ECRAN_2],FrameBuffers::fbTex[FBO_ECRAN_LORES],FrameBuffers::fbTexDepth[FBO_ECRAN_LORES]);

    //glEnable(GL_DEPTH_TEST);

    glm::mat4 mat = glm::mat4(1.);
    moteur3d->calcul_matrice_modelView(mat);
    moteur3d->maj_ubo_matrices();

        /*
        l1_position[0]=-1800.+1500.*cos(GVars::temps);
        l1_position[2]=-1800.+1500.*sin(GVars::temps);
        l1_position[1]=2000.+600.*cos(GVars::temps*1.4863);

        l3_position[0]=-1800.+1500.*sin(GVars::temps*0.762);
        l3_position[2]=-1800.+1500.*cos(GVars::temps*0.6147);
        l3_position[1]=3000.+1000.*sin(GVars::temps*0.987123);
        */

        /*
        l1_position[0]=0.+ECHELLE_FRACTALE*cos(GVars::temps*0.1);
        l1_position[2]=0.+ECHELLE_FRACTALE*sin(GVars::temps*0.1);
        l1_position[1]=0.+1000.*cos(GVars::temps*0.4863);

        l3_position[0]=0.+ECHELLE_FRACTALE*sin(GVars::temps*0.0762);
        l3_position[2]=0.+ECHELLE_FRACTALE*cos(GVars::temps*0.06147);
        l3_position[1]=0.+2000.*sin(GVars::temps*0.0987123);

        l2_position[0]=moteur3d->obs.x;
        l2_position[1]=moteur3d->obs.y;
        l2_position[2]=moteur3d->obs.z;
        */

    repere_l1s->initXYZ(&l1_position[0]);
    repere_l1s->calcul_matrice();
    repere_l1s->calcul_matrice_normale();
    repere_l2s->initXYZ(&l2_position[0]);
    repere_l2s->calcul_matrice();
    repere_l2s->calcul_matrice_normale();
    repere_l3s->initXYZ(&l3_position[0]);
    repere_l3s->calcul_matrice();
    repere_l3s->calcul_matrice_normale();
    float l1i=1.;
    float l2i=1.5+1.2*cosf(GVars::temps*2.);;
    float l3i=2.;

    Primitives::init_materiau(1.,1.,1.,1., 0.1*l1i,0.5*l1i,1.*l1i,1., 1.,1.,1.,1., 1.,1.,1.,1., 100.);
    Primitives::init_phong2si();
    glUniformMatrix3fv(ShadersPrimitives::prim_phong2si_matrice_normale_instance,1,GL_FALSE,&repere_l1s->matrice_normale[0][0]);
    glUniformMatrix4fv(ShadersPrimitives::prim_phong2si_matrice_instance,1,GL_FALSE,&repere_l1s->matrice[0][0]);
    l1_sphere->affiche();

    Primitives::init_materiau(1.,1.,1.,1., 1.*l2i,0.4*l2i,0.1*l2i,1., 1.,1.,1.,1., 1.,1.,1.,1., 100.);
    Primitives::init_phong2si();
    glUniformMatrix3fv(ShadersPrimitives::prim_phong2si_matrice_normale_instance,1,GL_FALSE,&repere_l2s->matrice_normale[0][0]);
    glUniformMatrix4fv(ShadersPrimitives::prim_phong2si_matrice_instance,1,GL_FALSE,&repere_l2s->matrice[0][0]);
    l2_sphere->affiche();

    Primitives::init_materiau(1.,1.,1.,1., 0.25*l3i,1.*l3i,0.25*l3i,1., 1.,1.,1.,1., 1.,1.,1.,1., 100.);
    Primitives::init_phong2si();
    glUniformMatrix3fv(ShadersPrimitives::prim_phong2si_matrice_normale_instance,1,GL_FALSE,&repere_l3s->matrice_normale[0][0]);
    glUniformMatrix4fv(ShadersPrimitives::prim_phong2si_matrice_instance,1,GL_FALSE,&repere_l3s->matrice[0][0]);
    l3_sphere->affiche();


    glUseProgram(0);


        Volume* vaisseau=testObjetConv->renvoie_volume_index(0);
        vaisseau->initXYZ(500.+objetsPos.x,100.+objetsPos.y,400.+objetsPos.z);
        //vaisseau->initRTL(0.,0.,-17.3*GVars::temps);
        vaisseau->calcul_matrice();
        moteur3d->calcul_matrice_modelView(vaisseau->matrice);
        moteur3d->maj_ubo_matrices();
        vaisseau->affiche();

        vaisseau=testObjetConv->renvoie_volume_index(1);
        vaisseau->initXYZ(-700.+objetsPos.x,100.+objetsPos.y,400.+objetsPos.z);
        //vaisseau->initRTL(0.,0.,20.712*GVars::temps);
        vaisseau->calcul_matrice();
        moteur3d->calcul_matrice_modelView(vaisseau->matrice);
        moteur3d->maj_ubo_matrices();
        vaisseau->affiche();

        vaisseau=testObjetConv->renvoie_volume_index(2);
        vaisseau->initXYZ(0.+objetsPos.x,100.+objetsPos.y,400.+objetsPos.z);
        //vaisseau->initRTL(0.,0.,30.123*GVars::temps);
        vaisseau->calcul_matrice();
        moteur3d->calcul_matrice_modelView(vaisseau->matrice);
        moteur3d->maj_ubo_matrices();
        vaisseau->affiche();

        vaisseau=testObjetConv->renvoie_volume_index(3);
        vaisseau->initXYZ(0.+objetsPos.x,100.+objetsPos.y,400.+objetsPos.z);
        //vaisseau->initRTL(0.,0.,30.123*GVars::temps);
        vaisseau->calcul_matrice();
        moteur3d->calcul_matrice_modelView(vaisseau->matrice);
        moteur3d->maj_ubo_matrices();
        vaisseau->affiche();

        vaisseau=testObjetConv->renvoie_volume_index(4);
        vaisseau->initXYZ(0.+objetsPos.x,100.+objetsPos.y,400.+objetsPos.z);
        //vaisseau->initRTL(0.,0.,30.123*GVars::temps);
        vaisseau->calcul_matrice();
        moteur3d->calcul_matrice_modelView(vaisseau->matrice);
        moteur3d->maj_ubo_matrices();
        vaisseau->affiche();

        vaisseau=testObjetConv->renvoie_volume_index(5);
        vaisseau->initXYZ(0.+objetsPos.x,100.+objetsPos.y,400.+objetsPos.z);
        //vaisseau->initRTL(0.,0.,30.123*GVars::temps);
        vaisseau->calcul_matrice();
        moteur3d->calcul_matrice_modelView(vaisseau->matrice);
        moteur3d->maj_ubo_matrices();
        vaisseau->affiche();

        glViewport(0,0,RESOLUTION_X_TRAVAIL,RESOLUTION_Y_TRAVAIL);
        glDisable(GL_DEPTH_TEST);
        GFunc::affiche_texture_profondeur_de_champ(FrameBuffers::fbos[FBO_ECRAN_1],FrameBuffers::fbTex[FBO_ECRAN_2],FrameBuffers::fbTexDepth[FBO_ECRAN_2]);
}
