#include "scene1.h"

    GLuint Scene1::vboTest=0;
    GLuint Scene1::eboTest=0;
    GLuint Scene1::vaoTest=0;

    GLuint Scene1::vbo_rmGare=0;
    GLuint Scene1::vao_rmGare=0;
    int Scene1::iter=3;

    Contexte3d* Scene1::moteur3d=NULL;

    ChargeLWO2* Scene1::testObjetLWO2=NULL;
    ConvertionLWO2* Scene1::testObjetConv=NULL;
    ChargeLWO2* Scene1::testObjetLWO2_2=NULL;
    ConvertionLWO2* Scene1::testObjetConv_2=NULL;

    Locomotive* Scene1::loco=NULL;

//Commandes clavier:
    bool Scene1::dep_droite=false;
    bool Scene1::dep_gauche=false;
    bool Scene1::dep_haut=false;
    bool Scene1::dep_bas=false;
    bool Scene1::dep_avant=false;
    bool Scene1::dep_arriere=false;

    bool Scene1::rot_droite=false;
    bool Scene1::rot_gauche=false;
    bool Scene1::rot_haut=false;
    bool Scene1::rot_bas=false;
    bool Scene1::rot_r_gauche=false;
    bool Scene1::rot_r_droite=false;

    bool Scene1::iter_moins;
    bool Scene1::iter_plus;

    bool Scene1::inc_zMin_nette;
    bool Scene1::dec_zMin_nette;
    bool Scene1::inc_zMax_nette;
    bool Scene1::dec_zMax_nette;

    int Scene1::depId=1;

    float Scene1::aX=0.;
    float Scene1::aY=0.;
    float Scene1::aZ=0.;

    float Scene1::aR=0.;
    float Scene1::aT=0.;
    float Scene1::aL=0.;



bool Scene1::init()
{

    moteur3d=new Contexte3d();
    if(moteur3d->erreur!=C3D_OK)
    {
        cout<<"ERREUR et meeerde, impossible de créer le contexte 3d - Scene 1"<<endl;
        return false;
    }

    moteur3d->determine_profondeur(.1,1000.);
    moteur3d->brouillard_min=0.;
    moteur3d->brouillard_max=1000.;
    moteur3d->determine_taille_ecran(GVars::largeur_ecran,GVars::hauteur_ecran);
    moteur3d->obs.initXYZ(0.,1.7,0.);
    moteur3d->determine_couleur_fond(.1,.1,.3,1.);
    moteur3d->determine_profondeur_de_champ(1.,10.);

    //======= Affichage raymarching:

     GLfloat sommets_rmGare[2*4]{-1.,-1., -1.,1., 1.,1., 1.,-1.};
        glGenBuffers(1,&vbo_rmGare);
        glGenVertexArrays(1,&vao_rmGare);

        glBindVertexArray(vao_rmGare);
            glBindBuffer(GL_ARRAY_BUFFER,vbo_rmGare);
                glBufferData(GL_ARRAY_BUFFER,sizeof(sommets_rmGare),sommets_rmGare,GL_STATIC_DRAW);
                glVertexAttribPointer(ShadersDemo::attrib_rmGare_position,2,GL_FLOAT,GL_FALSE,0,0);
                glEnableVertexAttribArray(ShadersDemo::attrib_rmGare_position);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);


    //======= Objet:
    float sommets[]={-2.,1.,0.,
                    1.,1.,0.,
                    1.,-1.,0.,
                    -1.,-1.,0.,

                    -3.,4.,0.,
                    1.,4.,0.,
                    1.,3.,0.,
                    -1.,3.,0.

                    };

    GLuint aretes1[]={0,1,2, 2,3,0 ,4,5,6, 6,7,4};
    //GLuint aretes2[]={4,5,6,7};

    glGenVertexArrays(1,&vaoTest);
    glGenBuffers(1,&vboTest);
    glGenBuffers(1,&eboTest);

    glBindVertexArray(vaoTest);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,eboTest);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(aretes1),aretes1,GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER,vboTest);
        glBufferData(GL_ARRAY_BUFFER,sizeof(sommets),sommets,GL_STATIC_DRAW);

            glEnableVertexAttribArray(Shaders3d::flat_position);
            glVertexAttribPointer(Shaders3d::flat_position,3,GL_FLOAT,GL_FALSE,0,0);

    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    //-------- Le bureau:

        //testObjetLWO2=new ChargeLWO2("LWO/cosmo_hires.lwo");
        testObjetLWO2=new ChargeLWO2("LWO/bureau_triangles_unit.lwo","LWO/bureau/");
        //testObjetLWO2=new ChargeLWO2("LWO/cube.lwo");
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

        Volume* bureau=testObjetConv->renvoie_volume_index(0);
        bureau->determine_eclairage_phong(true);
        //bureau->determine_eclairage(false);
        //bureau->determine_mode_remplissage(VOLUME_REMPLISSAGE_COULEUR);



    //---------- Le vaisseau:

        /*

        testObjetLWO2_2=new ChargeLWO2("LWO/objets_lo.lwo","LWO/vaisseaux/");
        //testObjetLWO2_2=new ChargeLWO2("LWO/vaisseaux_hi.lwo","LWO/vaisseaux/");
        if(testObjetLWO2_2==NULL || testObjetLWO2_2->erreur!=CHARGELWO2_OK)
        {
            cout<<"MEeerdeeee - Impossible de charger ou parser le fichier LWO"<<endl;
            return false;
        }

        testObjetConv_2=new ConvertionLWO2(testObjetLWO2_2);
        if(testObjetConv_2==NULL || testObjetConv_2->erreur!=CONVERTIONLWO2_OK)
        {
            cout<<"Crotte. - Impossible de convertir l'objet LWO"<<endl;
            return false;
        }

        Volume* vaisseau=testObjetConv_2->renvoie_volume_index(0);
        vaisseau->determine_eclairage_phong(true);
        vaisseau=testObjetConv_2->renvoie_volume_index(1);
        vaisseau->determine_eclairage_phong(true);
        vaisseau=testObjetConv_2->renvoie_volume_index(2);
        vaisseau->determine_eclairage_phong(true);
        vaisseau=testObjetConv_2->renvoie_volume_index(3);
        vaisseau->determine_eclairage_phong(true);
        vaisseau=testObjetConv_2->renvoie_volume_index(4);
        vaisseau->determine_eclairage_phong(true);
        vaisseau=testObjetConv_2->renvoie_volume_index(5);
        vaisseau->determine_eclairage_phong(true);

        */



    loco=new Locomotive();
    if(loco==NULL || loco->erreur!=LOCO_OK)
    {
        cout<<"Tchouu...plouf. - Impossible de générer la locomotive :("<<endl;
        return false;
    }
    loco->initXYZ(0.,1.5,-2.);


    return true;
}
void Scene1::destruction()
{
    if(glIsBuffer(eboTest))glDeleteBuffers(1,&eboTest);
    if(glIsBuffer(vboTest))glDeleteBuffers(1,&vboTest);
    if(glIsVertexArray(vaoTest))glDeleteVertexArrays(1,&vaoTest);
    if(moteur3d!=NULL) delete moteur3d;
    if(testObjetConv_2!=NULL)delete testObjetConv_2;
    if(testObjetLWO2_2!=NULL)delete testObjetLWO2_2;
    if(testObjetConv!=NULL)delete testObjetConv;
    if(testObjetLWO2!=NULL)delete testObjetLWO2;
    if(loco!=NULL)delete loco;
    if (glIsBuffer(vbo_rmGare)) glDeleteBuffers(1,&vbo_rmGare);
    if (glIsVertexArray(vao_rmGare)) glDeleteVertexArrays(1,&vao_rmGare);
}


void Scene1::clavier(int key,bool etat)
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

                case GLFW_KEY_Q:inc_zMin_nette=etat;break;
                case GLFW_KEY_W:dec_zMin_nette=etat;break;
                case GLFW_KEY_S:inc_zMax_nette=etat;break;
                case GLFW_KEY_X:dec_zMax_nette=etat;break;

                case GLFW_KEY_F1: depId=1;break;
                case GLFW_KEY_F2: depId=2;break;
                case GLFW_KEY_F3: depId=3;break;
                case GLFW_KEY_F4: depId=4;break;
                case GLFW_KEY_F5: depId=5;break;
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


void Scene1::maj_eclairages()
{

}

void Scene1::maj_mouvements()
{
    float raX,raZ;

    if(iter_plus)iter++;
    else if (iter_moins){iter--;if (iter<1)iter=1;}

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
		GFunc::rotation_plan(-loco->l,aX,aZ,raX,raZ);

		loco->x+=raX;
		loco->y+=aY;
		loco->z+=raZ;

		loco->r+=aR;
		loco->t+=aT;
		loco->l+=aL;

		if (GVars::disp_donnees)
        {
            cout<<"Loco - X="<<loco->x<<" Y="<<loco->y<<" Z="<<loco->z<<" R="<<loco->r<<" T="<<loco->t<<" L="<<loco->l<<endl;

        }
	}

	GVars::disp_donnees=false;

    float deceleration=0.75;
    float acceleration=.001;

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

    float vitesse_modif_profChamp=0.01;
    float zminNette=moteur3d->zMin_nette;
    float zmaxNette=moteur3d->zMax_nette;
    if(inc_zMin_nette)zminNette+=vitesse_modif_profChamp;
    if(dec_zMin_nette)zminNette-=vitesse_modif_profChamp;
    if(inc_zMax_nette)zmaxNette+=vitesse_modif_profChamp;
    if(dec_zMax_nette)zmaxNette-=vitesse_modif_profChamp;
    moteur3d->determine_profondeur_de_champ(zminNette,zmaxNette);

}

void Scene1::affiche()
{
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,FrameBuffers::fbos[FBO_ECRAN_2]);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //moteur3d->maj_pointeurs_ubos();   Géré par le timeline
        moteur3d->calcul_matrice_projection();
        moteur3d->calcul_matrice_observateur();
        moteur3d->maj_ubo_eclairage2si();

        GVars::zMin=moteur3d->zMin;
        GVars::zMax=moteur3d->zMax;
        GVars::ratio_ecran=moteur3d->ratio_ecran;
        GVars::distance_focale=moteur3d->distanceFocale;
        GVars::zMin_nette=moteur3d->zMin_nette;
        GVars::zMax_nette=moteur3d->zMax_nette;

    //=============== Rendus Raymarching:

        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_CULL_FACE);

        moteur3d->obs.calcul_matrice(); //Pour le raytraceur
        moteur3d->obs.calcul_matrice_normale(); //Pour le raytraceur

        glViewport(0,0,FBO_LORES_L,FBO_LORES_H);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,FrameBuffers::fbos[FBO_ECRAN_LORES]);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(ShadersDemo::programme_rmGare);
        glUniform3f(ShadersDemo::uniform_rmGare_obs_pos,moteur3d->obs.x,moteur3d->obs.y,moteur3d->obs.z);
        glUniformMatrix3fv(ShadersDemo::uniform_rmGare_obsMat_normale,1,GL_FALSE,&moteur3d->obs.matrice_normale[0][0]);
        glUniform1f(ShadersDemo::uniform_rmGare_ratio_ecran,moteur3d->ratio_ecran);
        glUniform1f(ShadersDemo::uniform_rmGare_distanceFocale,moteur3d->distanceFocale);
        glUniform1f(ShadersDemo::uniform_rmGare_t,GVars::temps*0.);
        glUniform1i(ShadersDemo::uniform_rmGare_iter,iter);
        glUniform1f(ShadersDemo::uniform_rmGare_zMin,moteur3d->zMin);
        glUniform1f(ShadersDemo::uniform_rmGare_zMax,moteur3d->zMax);

        glBindVertexArray(vao_rmGare);
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



    //================ Rendu classique:

        Repere repere_objet(0.,0.,-10.,0.,0.,-57.3*GVars::temps);
        repere_objet.calcul_matrice();
        moteur3d->calcul_matrice_modelView(repere_objet.matrice);
        moteur3d->maj_ubo_matrices();
        glUseProgram(Shaders3d::programme_flat);
        glBindVertexArray(vaoTest);
        glUniform4f(Shaders3d::flat_couleur,1.,0.,0.,1.);
        glDrawElements(GL_TRIANGLES,12,GL_UNSIGNED_INT,0);
        //glDrawArrays(GL_TRIANGLES,0,4*2);
        glBindVertexArray(0);
        glUseProgram(0);


        Volume* bureau=testObjetConv->renvoie_volume_index(0);
        //bureau->initRTL(0.,0.,-17.3*GVars::temps);
        bureau->calcul_matrice();

        moteur3d->calcul_matrice_modelView(bureau->matrice);
        //moteur3d->calcul_matrice_modelView(repere_objet.matrice);
        moteur3d->maj_ubo_matrices();

        //bureau->affiche();


        /*
        Volume* vaisseau=testObjetConv_2->renvoie_volume_index(0);
        vaisseau->initXYZ(50.,10.,40.);
        //vaisseau->initRTL(0.,0.,-17.3*GVars::temps);
        vaisseau->calcul_matrice();
        moteur3d->calcul_matrice_modelView(vaisseau->matrice);
        moteur3d->maj_ubo_matrices();
        vaisseau->affiche();

        vaisseau=testObjetConv_2->renvoie_volume_index(1);
        vaisseau->initXYZ(-70.,10.,40.);
        //vaisseau->initRTL(0.,0.,20.712*GVars::temps);
        vaisseau->calcul_matrice();
        moteur3d->calcul_matrice_modelView(vaisseau->matrice);
        moteur3d->maj_ubo_matrices();
        vaisseau->affiche();

        vaisseau=testObjetConv_2->renvoie_volume_index(2);
        vaisseau->initXYZ(0.,10.,40.);
        //vaisseau->initRTL(0.,0.,30.123*GVars::temps);
        vaisseau->calcul_matrice();
        moteur3d->calcul_matrice_modelView(vaisseau->matrice);
        moteur3d->maj_ubo_matrices();
        vaisseau->affiche();

        vaisseau=testObjetConv_2->renvoie_volume_index(3);
        vaisseau->initXYZ(0.,10.,40.);
        //vaisseau->initRTL(0.,0.,30.123*GVars::temps);
        vaisseau->calcul_matrice();
        moteur3d->calcul_matrice_modelView(vaisseau->matrice);
        moteur3d->maj_ubo_matrices();
        vaisseau->affiche();

        vaisseau=testObjetConv_2->renvoie_volume_index(4);
        vaisseau->initXYZ(0.,10.,40.);
        //vaisseau->initRTL(0.,0.,30.123*GVars::temps);
        vaisseau->calcul_matrice();
        moteur3d->calcul_matrice_modelView(vaisseau->matrice);
        moteur3d->maj_ubo_matrices();
        vaisseau->affiche();

        vaisseau=testObjetConv_2->renvoie_volume_index(5);
        vaisseau->initXYZ(0.,10.,40.);
        //vaisseau->initRTL(0.,0.,30.123*GVars::temps);
        vaisseau->calcul_matrice();
        moteur3d->calcul_matrice_modelView(vaisseau->matrice);
        moteur3d->maj_ubo_matrices();
        vaisseau->affiche();
        */


        //loco->initRTL(27.*GVars::temps,44.*GVars::temps,-17.3*GVars::temps);
        loco->calcul_matrice();
        moteur3d->calcul_matrice_modelView(loco->matrice);
        moteur3d->maj_ubo_matrices();
        loco->affiche();

        glViewport(0,0,RESOLUTION_X_TRAVAIL,RESOLUTION_Y_TRAVAIL);
        glDisable(GL_DEPTH_TEST);
        GFunc::affiche_texture_profondeur_de_champ(FrameBuffers::fbos[FBO_ECRAN_1],FrameBuffers::fbTex[FBO_ECRAN_2],FrameBuffers::fbTexDepth[FBO_ECRAN_2]);
}
