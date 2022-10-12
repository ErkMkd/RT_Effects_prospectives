#include "Scene_terrain.h"


    Contexte3d* Scene_terrain::moteur3d=NULL;

    ChargeLWO2* Scene_terrain::testObjetLWO2=NULL;
    ConvertionLWO2* Scene_terrain::testObjetConv=NULL;
    Volume* Scene_terrain::robot=NULL;

//Terrain:
    GLuint Scene_terrain::vbo_rmt=0;
    GLuint Scene_terrain::vao_rmt=0;

    float* Scene_terrain::bitmap_terrain=NULL;
    float* Scene_terrain::bitmap_terrain2=NULL;
    float* Scene_terrain::bitmap_terrain3=NULL;
    GLuint Scene_terrain::textures_terrain[3]{0,0,0};

    uint32_t Scene_terrain::tailles_textures_terrain[2*3]{512,512, 256,256, 256,256};
    uint32_t Scene_terrain::taille_fondamentale[2*3]{8,8,4,4,16,16};
    uint32_t Scene_terrain::nbr_octaves[3]{50.,50.,20};
    float Scene_terrain::persistance[3]{0.75,0.80,0.7};
    uint32_t Scene_terrain::rand_init[3]{1,1,1};

    float Scene_terrain::echelle_terrain[2*3]{20000.,20000.,1000.,1000.,90.,90.};
    float Scene_terrain::amplitude_terrain[3]{6000.,90.,1.5};
    float Scene_terrain::facteur_precision_distance=1.01;

    Nuages* Scene_terrain::ptitsNuages=NULL;

//Controle des paramètres de génération du bitmap
    bool Scene_terrain::inc_terr_fond_xSize=false;
    bool Scene_terrain::dec_terr_fond_xSize=false;
    bool Scene_terrain::inc_terr_fond_ySize=false;
    bool Scene_terrain::dec_terr_fond_ySize=false;
    bool Scene_terrain::inc_terr_taille_xSize=false;
    bool Scene_terrain::dec_terr_taille_xSize=false;
    bool Scene_terrain::inc_terr_taille_zSize=false;
    bool Scene_terrain::dec_terr_taille_zSize=false;
    bool Scene_terrain::inc_terr_nOct=false;
    bool Scene_terrain::dec_terr_nOct=false;
    bool Scene_terrain::inc_terr_pers=false;
    bool Scene_terrain::dec_terr_pers=false;
    bool Scene_terrain::inc_terr_srand=false;
    bool Scene_terrain::dec_terr_srand=false;
    bool Scene_terrain::inc_amplitude=false;
    bool Scene_terrain::dec_amplitude=false;


//Commandes clavier:
    bool Scene_terrain::dep_droite=false;
    bool Scene_terrain::dep_gauche=false;
    bool Scene_terrain::dep_haut=false;
    bool Scene_terrain::dep_bas=false;
    bool Scene_terrain::dep_avant=false;
    bool Scene_terrain::dep_arriere=false;

    bool Scene_terrain::rot_droite=false;
    bool Scene_terrain::rot_gauche=false;
    bool Scene_terrain::rot_haut=false;
    bool Scene_terrain::rot_bas=false;
    bool Scene_terrain::rot_r_gauche=false;
    bool Scene_terrain::rot_r_droite=false;

    bool Scene_terrain::inc_zMin_nette;
    bool Scene_terrain::dec_zMin_nette;
    bool Scene_terrain::inc_zMax_nette;
    bool Scene_terrain::dec_zMax_nette;

    int Scene_terrain::depId=1;
    int Scene_terrain::bmTerrainId=0;

    float Scene_terrain::aX=0.;
    float Scene_terrain::aY=0.;
    float Scene_terrain::aZ=0.;

    float Scene_terrain::aR=0.;
    float Scene_terrain::aT=0.;
    float Scene_terrain::aL=0.;



bool Scene_terrain::init()
{
    moteur3d=new Contexte3d();
    if(moteur3d->erreur!=C3D_OK)
    {
        cout<<"ERREUR et meeerde, impossible de créer le contexte 3d - Scene Terrain"<<endl;
        return false;
    }

    moteur3d->determine_profondeur(.1,10000.);
    moteur3d->brouillard_min=1.;
    moteur3d->brouillard_max=10000.;
    moteur3d->determine_taille_ecran(GVars::largeur_ecran,GVars::hauteur_ecran);
    moteur3d->obs.initXYZ(-0.7,30.,-1.3);
    //moteur3d->obs.initRTL(0.,3.89,-154.79);
    moteur3d->obs.initRTL(0.,0.,0.);
    moteur3d->determine_couleur_fond(70./255.,150./255.,255./255.,1.);
    //moteur3d->determine_couleur_fond(76./255.,128./255.,255./255.,1.);
    //moteur3d->determine_couleur_brouillard(77./255.,158./255.,255./255.,1.);
    moteur3d->determine_couleur_brouillard(70./255.,150./255.,255./255.,1.);
    moteur3d->lumieres[1]->determine_couleur(77./255.,158./255.,255./255.,1.);
    moteur3d->lumieres[1]->intensite=0.5;
    moteur3d->determine_profondeur_de_champ(2.,10000.);
    moteur3d->lumieres[0]->determine_couleur(255./255.,250./255.,223./255.,1.);
    moteur3d->lumieres[0]->initRTL(0.,15.,30.+180.);
    moteur3d->lumieres[1]->initRTL(0.,45.,30.);
    //-------- Polygone de rendu raymarching:

    GLfloat sommets_rendu[2*4]{-1.,-1., -1.,1., 1.,1., 1.,-1.};
    glGenBuffers(1,&vbo_rmt);
    glGenVertexArrays(1,&vao_rmt);

    glBindVertexArray(vao_rmt);
        glBindBuffer(GL_ARRAY_BUFFER,vbo_rmt);
            glBufferData(GL_ARRAY_BUFFER,sizeof(sommets_rendu),sommets_rendu,GL_STATIC_DRAW);
            glVertexAttribPointer(ShadersDemo::attrib_rmt_position,2,GL_FLOAT,GL_FALSE,0,0);
            glEnableVertexAttribArray(ShadersDemo::attrib_rmt_position);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    //-------- Le bureau:

        //testObjetLWO2=new ChargeLWO2("LWO/cosmo_hires.lwo");
        testObjetLWO2=new ChargeLWO2("LWO/robot.lwo","LWO/robot/");
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

        robot=testObjetConv->renvoie_volume_index(0);
        robot->determine_eclairage_phong(true);
        robot->initXYZ(0.53,28.46,1.42);
        robot->initRTL(0.,0.,-188);

        //Allocation bitmap terrain:
        bitmap_terrain=(float*)(malloc(sizeof(float)*tailles_textures_terrain[0]*tailles_textures_terrain[1]));
        if(bitmap_terrain==NULL)
        {
            cout<<"ERREUR dans Scene_terrain_init() - Impossible d'allouer le bitmap de terrain 1"<<endl;
            return false;
        }

        bitmap_terrain2=(float*)(malloc(sizeof(float)*tailles_textures_terrain[2]*tailles_textures_terrain[3]));
        if(bitmap_terrain2==NULL)
        {
            cout<<"ERREUR dans Scene_terrain_init() - Impossible d'allouer le bitmap de terrain 2"<<endl;
            return false;
        }

        bitmap_terrain3=(float*)(malloc(sizeof(float)*tailles_textures_terrain[4]*tailles_textures_terrain[5]));
        if(bitmap_terrain3==NULL)
        {
            cout<<"ERREUR dans Scene_terrain_init() - Impossible d'allouer le bitmap de terrain 3"<<endl;
            return false;
        }

        if(!maj_bitmap_terrain())return false;
        if(!maj_bitmap_terrain2())return false;
        if(!maj_bitmap_terrain3())return false;

        //Création de la texture de terrain:
        glGenTextures(3,textures_terrain);
        if(GFunc::erreur_openGl("ERREUR ! Pas possible de créer les textures de terrain - Scene_terrain - "))return false;

        glBindTexture(GL_TEXTURE_2D,textures_terrain[0]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,tailles_textures_terrain[0],tailles_textures_terrain[1],0,GL_LUMINANCE,GL_FLOAT,bitmap_terrain);

        glBindTexture(GL_TEXTURE_2D,textures_terrain[1]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,tailles_textures_terrain[2],tailles_textures_terrain[3],0,GL_LUMINANCE,GL_FLOAT,bitmap_terrain2);

        glBindTexture(GL_TEXTURE_2D,textures_terrain[2]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,tailles_textures_terrain[4],tailles_textures_terrain[5],0,GL_LUMINANCE,GL_FLOAT,bitmap_terrain3);


        ptitsNuages=new Nuages(256);
        if(ptitsNuages->erreur!=NUAGES_OK)
        {
            cout<<"ERREUR de MerDeEEuh dans Scene_terrain->init() - Pas possible de crer les p'tits nuages à la con !"<<endl;
            return false;
        }

        ptitsNuages->altitude=2500.;
        return true;
}

bool Scene_terrain::maj_bitmap_terrain()
{
    for (int i=0;i<tailles_textures_terrain[0]*tailles_textures_terrain[1];i++) bitmap_terrain[i]=0.;
    Bruit::genere_bruit_de_Perlin_2D_f(taille_fondamentale[0],taille_fondamentale[1],tailles_textures_terrain[0],tailles_textures_terrain[1],nbr_octaves[0],persistance[0],bitmap_terrain);
    /*
    for (int y=0;y<tailles_textures_terrain[1];y++)
        for (int x=0;x<tailles_textures_terrain[0];x++)
                bitmap_terrain[x+y*tailles_textures_terrain[0]]=powf(bitmap_terrain[x+y*tailles_textures_terrain[0]],5.);
        return true;
        */
}

bool Scene_terrain::maj_bitmap_terrain2()
{
    for (int i=0;i<tailles_textures_terrain[2]*tailles_textures_terrain[3];i++) bitmap_terrain2[i]=0.;
    Bruit::genere_bruit_de_Perlin_2D_f(taille_fondamentale[2],taille_fondamentale[3],tailles_textures_terrain[2],tailles_textures_terrain[3],nbr_octaves[1],persistance[1],bitmap_terrain2);
        return true;
}

bool Scene_terrain::maj_bitmap_terrain3()
{
    for (int i=0;i<tailles_textures_terrain[4]*tailles_textures_terrain[5];i++) bitmap_terrain3[i]=0.;
    Bruit::genere_bruit_de_Perlin_2D_f(taille_fondamentale[4],taille_fondamentale[5],tailles_textures_terrain[4],tailles_textures_terrain[5],nbr_octaves[2],persistance[2],bitmap_terrain3);
        return true;
}


bool Scene_terrain::maj_texture_terrain()
{
        glBindTexture(GL_TEXTURE_2D,textures_terrain[0]);
        glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,tailles_textures_terrain[0],tailles_textures_terrain[1],0,GL_LUMINANCE,GL_FLOAT,bitmap_terrain);
        if(GFunc::erreur_openGl("Erreur lors de la mise à jour de la texture - ")) {return false;}
        return true;
}

bool Scene_terrain::maj_texture_terrain2()
{
        glBindTexture(GL_TEXTURE_2D,textures_terrain[1]);
        glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,tailles_textures_terrain[2],tailles_textures_terrain[3],0,GL_LUMINANCE,GL_FLOAT,bitmap_terrain2);
        if(GFunc::erreur_openGl("Erreur lors de la mise à jour de la texture - ")) {return false;}
        return true;
}

bool Scene_terrain::maj_texture_terrain3()
{
        glBindTexture(GL_TEXTURE_2D,textures_terrain[2]);
        glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,tailles_textures_terrain[4],tailles_textures_terrain[5],0,GL_LUMINANCE,GL_FLOAT,bitmap_terrain3);
        if(GFunc::erreur_openGl("Erreur lors de la mise à jour de la texture - ")) {return false;}
        return true;
}

void Scene_terrain::destruction()
{
    if(moteur3d!=NULL) delete moteur3d;
    if(testObjetConv!=NULL)delete testObjetConv;
    if(testObjetLWO2!=NULL)delete testObjetLWO2;
    if(glIsTexture(textures_terrain[0]))glDeleteTextures(3,textures_terrain); //Si la première est bien une texture, alors les autres aussi, puisque créées ensemble.
    if(bitmap_terrain!=NULL)free(bitmap_terrain);
    if(bitmap_terrain2!=NULL)free(bitmap_terrain2);
    if(bitmap_terrain3!=NULL)free(bitmap_terrain3);
    if (glIsBuffer(vbo_rmt)) glDeleteBuffers(1,&vbo_rmt);
    if (glIsVertexArray(vao_rmt)) glDeleteVertexArrays(1,&vao_rmt);
    if(ptitsNuages!=NULL)delete ptitsNuages;
}


void Scene_terrain::clavier(int key,bool etat)
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

                case GLFW_KEY_Q:inc_zMin_nette=etat;break;
                case GLFW_KEY_W:dec_zMin_nette=etat;break;
                case GLFW_KEY_S:inc_zMax_nette=etat;break;
                case GLFW_KEY_X:dec_zMax_nette=etat;break;

                case GLFW_KEY_4:inc_amplitude=etat;break;
                case GLFW_KEY_3:dec_amplitude=etat;break;
                case GLFW_KEY_R:inc_terr_taille_xSize=etat;break;
                case GLFW_KEY_E:dec_terr_taille_xSize=etat;break;
                case GLFW_KEY_F:inc_terr_taille_zSize=etat;break;
                case GLFW_KEY_D:dec_terr_taille_zSize=etat;break;
                case GLFW_KEY_Y:inc_terr_fond_xSize=etat;break;
                case GLFW_KEY_T:dec_terr_fond_xSize=etat;break;
                case GLFW_KEY_H:inc_terr_fond_ySize=etat;break;
                case GLFW_KEY_G:dec_terr_fond_ySize=etat;break;
                case GLFW_KEY_V:inc_terr_nOct=etat;break;
                case GLFW_KEY_C:dec_terr_nOct=etat;break;
                case GLFW_KEY_6:inc_terr_pers=etat;break;
                case GLFW_KEY_5:dec_terr_pers=etat;break;
                case GLFW_KEY_N:inc_terr_srand=etat;break;
                case GLFW_KEY_B:dec_terr_srand=etat;break;

                case GLFW_KEY_TAB: if(etat){bmTerrainId++;if(bmTerrainId>2)bmTerrainId=0;} break;

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


void Scene_terrain::maj_eclairages()
{

}

void Scene_terrain::maj_mouvements()
{
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

		GFunc::rotation_plan(-robot->l,aX,aZ,raX,raZ);

		robot->x+=raX;
		robot->y+=aY;
		robot->z+=raZ;

		robot->r+=aR;
		robot->t+=aT;
		robot->l+=aL;

		if (GVars::disp_donnees)
        {
            cout<<"robot - X="<<robot->x<<" Y="<<robot->y<<" Z="<<robot->z<<" R="<<robot->r<<" T="<<robot->t<<" L="<<robot->l<<endl;

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

    float vitesse_modif_profChamp=1.;
    float zminNette=moteur3d->zMin_nette;
    float zmaxNette=moteur3d->zMax_nette;
    if(inc_zMin_nette)zminNette+=vitesse_modif_profChamp;
    if(dec_zMin_nette)zminNette-=vitesse_modif_profChamp;
    if(inc_zMax_nette)zmaxNette+=vitesse_modif_profChamp;
    if(dec_zMax_nette)zmaxNette-=vitesse_modif_profChamp;
    moteur3d->determine_profondeur_de_champ(zminNette,zmaxNette);

    float pas_amplitudes[3]{10.,1.,0.1};

    if(inc_amplitude){amplitude_terrain[bmTerrainId]+=pas_amplitudes[bmTerrainId];}
    if(dec_amplitude){amplitude_terrain[bmTerrainId]-=pas_amplitudes[bmTerrainId];
                        if(amplitude_terrain[bmTerrainId]<pas_amplitudes[bmTerrainId])amplitude_terrain[bmTerrainId]=pas_amplitudes[bmTerrainId];}


    if(inc_terr_taille_xSize){echelle_terrain[bmTerrainId*2]+=pas_amplitudes[bmTerrainId];}
    if(dec_terr_taille_xSize){echelle_terrain[bmTerrainId*2]-=pas_amplitudes[bmTerrainId];
                                if(echelle_terrain[bmTerrainId*2]<pas_amplitudes[bmTerrainId])echelle_terrain[bmTerrainId*2]=pas_amplitudes[bmTerrainId];
                                }
    if(inc_terr_taille_zSize){echelle_terrain[bmTerrainId*2+1]+=pas_amplitudes[bmTerrainId];}
    if(dec_terr_taille_zSize){echelle_terrain[bmTerrainId*2+1]-=pas_amplitudes[bmTerrainId];
                                if(echelle_terrain[bmTerrainId*2+1]<pas_amplitudes[bmTerrainId])echelle_terrain[bmTerrainId*2+1]=pas_amplitudes[bmTerrainId];
                                }
    bool dmt=false;
    if(inc_terr_fond_xSize){taille_fondamentale[bmTerrainId*2]++;dmt=true;}
    if(dec_terr_fond_xSize){taille_fondamentale[bmTerrainId*2]--;if(taille_fondamentale[bmTerrainId*2]<1)taille_fondamentale[bmTerrainId*2]=1;dmt=true;}
    if(inc_terr_fond_ySize){taille_fondamentale[bmTerrainId*2+1]++;dmt=true;}
    if(dec_terr_fond_ySize){taille_fondamentale[bmTerrainId*2+1]--;if(taille_fondamentale[bmTerrainId*2+1]<1)taille_fondamentale[bmTerrainId*2+1]=1;dmt=true;}
    if(inc_terr_nOct){nbr_octaves[bmTerrainId]++;dmt=true;}
    if(dec_terr_nOct){nbr_octaves[bmTerrainId]--;if(nbr_octaves[bmTerrainId]<1)nbr_octaves[bmTerrainId]=1;dmt=true;}
    if(inc_terr_pers){persistance[bmTerrainId]+=0.05;dmt=true;}
    if(dec_terr_pers){persistance[bmTerrainId]-=0.05;if(persistance[bmTerrainId]<0.05)persistance[bmTerrainId]=0.1;dmt=true;}
    if(inc_terr_srand){rand_init[bmTerrainId]++;dmt=true;}
    if(dec_terr_srand){rand_init[bmTerrainId]--;if(rand_init[bmTerrainId]<1)rand_init[bmTerrainId]=1;dmt=true;}
    if(dmt)
    {
        srand(rand_init[bmTerrainId]);
        if(bmTerrainId==0)
        {
            maj_bitmap_terrain();
            maj_texture_terrain();
        }
        else if(bmTerrainId==1)
        {
            maj_bitmap_terrain2();
            maj_texture_terrain2();
        }
        else if(bmTerrainId==2)
        {
            maj_bitmap_terrain3();
            maj_texture_terrain3();
        }
    }
}

void Scene_terrain::affiche_textures()
{

        GFunc::affiche_bitmap(FrameBuffers::fbos[FBO_ECRAN_1],textures_terrain[0],1.,tailles_textures_terrain[0],tailles_textures_terrain[1],5.,5.);
        GFunc::affiche_bitmap(FrameBuffers::fbos[FBO_ECRAN_1],textures_terrain[1],1.,tailles_textures_terrain[2],tailles_textures_terrain[3],5.+tailles_textures_terrain[0]+5.,5.);
        GFunc::affiche_bitmap(FrameBuffers::fbos[FBO_ECRAN_1],textures_terrain[2],1.,tailles_textures_terrain[4],tailles_textures_terrain[5],5.+tailles_textures_terrain[0]+5.+tailles_textures_terrain[2]+5.,5.);
        GFunc::affiche_bitmap(FrameBuffers::fbos[FBO_ECRAN_1],ptitsNuages->textures_id[0],1.,ptitsNuages->taille_textures,ptitsNuages->taille_textures,
                              5.+tailles_textures_terrain[0]+5.
                              +tailles_textures_terrain[2]+5.
                              +tailles_textures_terrain[4]+5.
                              ,5.);
        GFunc::affiche_bitmap(FrameBuffers::fbos[FBO_ECRAN_1],ptitsNuages->textures_id[1],1.,ptitsNuages->taille_textures,ptitsNuages->taille_textures,
                              5.+tailles_textures_terrain[0]+5.
                              +tailles_textures_terrain[2]+5.
                              +tailles_textures_terrain[4]+5.
                              +ptitsNuages->taille_textures+5.
                              ,5.);
        GFunc::affiche_bitmap(FrameBuffers::fbos[FBO_ECRAN_1],ptitsNuages->textures_id[2],1.,ptitsNuages->taille_textures,ptitsNuages->taille_textures,
                              5.+tailles_textures_terrain[0]+5.
                              +tailles_textures_terrain[2]+5.
                              +tailles_textures_terrain[4]+5.
                              +ptitsNuages->taille_textures+5.
                              +ptitsNuages->taille_textures+5.
                              ,5.);
}

void Scene_terrain::affiche()
{

        //moteur3d->maj_pointeurs_ubos();   Géré par le timeline
        moteur3d->calcul_matrice_projection();
        moteur3d->obs.calcul_matrice(); //Pour le raytraceur
        moteur3d->obs.calcul_matrice_normale(); //Pour le raytraceur
        moteur3d->calcul_matrice_observateur();
        moteur3d->maj_ubo_eclairage2si();

        GVars::zMin=moteur3d->zMin;
        GVars::zMax=moteur3d->zMax;
        GVars::ratio_ecran=moteur3d->ratio_ecran;
        GVars::distance_focale=moteur3d->distanceFocale;
        GVars::zMin_nette=moteur3d->zMin_nette;
        GVars::zMax_nette=moteur3d->zMax_nette;

        //-------- Rendu raymarching:
            glActiveTexture(GL_TEXTURE0);
            glViewport(0,0,FBO_LORES_L,FBO_LORES_H);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER,FrameBuffers::fbos[FBO_ECRAN_LORES]);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,textures_terrain[0]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D,textures_terrain[1]);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D,textures_terrain[2]);
            glUseProgram(ShadersDemo::programme_rmt);
            glUniform3f(ShadersDemo::uniform_rmt_obs_pos,moteur3d->obs.x,moteur3d->obs.y,moteur3d->obs.z);
            glUniformMatrix3fv(ShadersDemo::uniform_rmt_obsMat_normale,1,GL_FALSE,&moteur3d->obs.matrice_normale[0][0]);
            glUniform1f(ShadersDemo::uniform_rmt_ratio_ecran,moteur3d->ratio_ecran);
            glUniform1f(ShadersDemo::uniform_rmt_distanceFocale,moteur3d->distanceFocale);
            glUniform1f(ShadersDemo::uniform_rmt_zMin,moteur3d->zMin);
            glUniform1f(ShadersDemo::uniform_rmt_zMax,moteur3d->zMax);
            glUniform1f(ShadersDemo::uniform_rmt_amplitude_terrain,amplitude_terrain[0]);
            glUniform1f(ShadersDemo::uniform_rmt_amplitude_terrain2,amplitude_terrain[1]);
            glUniform1f(ShadersDemo::uniform_rmt_amplitude_terrain3,amplitude_terrain[2]);
            glUniform1f(ShadersDemo::uniform_rmt_facteur_precision_distance,facteur_precision_distance);
            glUniform1i(ShadersDemo::uniform_rmt_texture_terrain,0);
            glUniform1i(ShadersDemo::uniform_rmt_texture_terrain2,1);
            glUniform1i(ShadersDemo::uniform_rmt_texture_terrain3,2);
            glUniform2f(ShadersDemo::uniform_rmt_facteur_echelle_terrain,1./echelle_terrain[0],1./echelle_terrain[1]);
            glUniform2f(ShadersDemo::uniform_rmt_facteur_echelle_terrain2,1./echelle_terrain[2],1./echelle_terrain[3]);
            glUniform2f(ShadersDemo::uniform_rmt_facteur_echelle_terrain3,1./echelle_terrain[4],1./echelle_terrain[5]);


            glBindVertexArray(vao_rmt);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glBindVertexArray(0);

            glUseProgram(0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D,0);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D,0);

        //Superposition résolution:
        glActiveTexture(GL_TEXTURE0);
        glViewport(0,0,RESOLUTION_X_TRAVAIL,RESOLUTION_Y_TRAVAIL);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,FrameBuffers::fbos[FBO_ECRAN_2]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GVars::taille_tramage=.5;
        GFunc::affiche_texture_zBuffer(FrameBuffers::fbos[FBO_ECRAN_2],FrameBuffers::fbTex[FBO_ECRAN_LORES],FrameBuffers::fbTexDepth[FBO_ECRAN_LORES]);

        //-------- Rendu nuages:

        ptitsNuages->affiche(moteur3d);

        //-------- Rendu polygones:

        //robot->initRTL(0.,0.,-17.3*GVars::temps);
        robot->calcul_matrice();
        moteur3d->calcul_matrice_modelView(robot->matrice);
        //moteur3d->calcul_matrice_modelView(repere_objet.matrice);
        moteur3d->maj_ubo_matrices();

        robot->affiche();


        glActiveTexture(GL_TEXTURE0);
        glViewport(0,0,RESOLUTION_X_TRAVAIL,RESOLUTION_Y_TRAVAIL);
        glDisable(GL_DEPTH_TEST);
        GFunc::affiche_texture_profondeur_de_champ(FrameBuffers::fbos[FBO_ECRAN_1],FrameBuffers::fbTex[FBO_ECRAN_2],FrameBuffers::fbTexDepth[FBO_ECRAN_2]);

        glActiveTexture(GL_TEXTURE0);
        glDisable(GL_DEPTH_TEST);

        //affiche_textures();

}
