#include "glew.h"
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "math.h"

#include "Variables.h"
#include "fonctions.h"
#include "FrameBuffers.h"
#include "TexteMap.h"
#include "scene1.h"
#include "scene_etudeMandelbrot.h"
#include "Raymarching.h"
#include "Scene_terrain.h"
#include "Locomotive.h"


static bool drapeau_init_scene1=false;
static bool drapeau_init_scene_terrain=false;
static bool drapeau_init_scene_rm=false;

static TexteMap* infosDemo=NULL;

static bool init_timeline()
{
    infosDemo=new TexteMap("polices/Groupe.tif",GVars::largeur_ecran,GVars::hauteur_ecran);
    if(infosDemo==NULL || infosDemo->erreur!=TEXTEMAP_OK) return false;
    infosDemo->mode_rendu=TEXTEMAP_RENDU_2D;
    infosDemo->determine_position_curseur(0.01,0.97);
    infosDemo->determine_echelle(float(infosDemo->renvoie_largeur_caracteres())/float(GVars::largeur_ecran));
    infosDemo->determine_espace_lignes(1.1);

    if(!Scene1::init())return false;
    if(!Fractale2d::init())return false;
    if(!Raymarching::init())return false;
    if(!Scene_terrain::init())return false;
    return true;
}

static void detruit_timeline()
{
    Scene_terrain::destruction();
    Fractale2d::destruction();
    Scene1::destruction();
    Raymarching::destruction();
    if(infosDemo!=NULL) delete infosDemo;
}

static void scene_1()
{
    if(!drapeau_init_scene1)
    {
        glClearColor(Scene1::moteur3d->couleur_fond[0],Scene1::moteur3d->couleur_fond[1],Scene1::moteur3d->couleur_fond[2],Scene1::moteur3d->couleur_fond[3]);
        //drapeau_init_scene1=true;
    }
}


static void scene_rm()
{
    if(!drapeau_init_scene_rm)
    {
        glClearColor(Raymarching::moteur3d->couleur_fond[0],Raymarching::moteur3d->couleur_fond[1],Raymarching::moteur3d->couleur_fond[2],Raymarching::moteur3d->couleur_fond[3]);
        //drapeau_init_scene1=true;
    }
}

static void scene_terrain()
{
    if(!drapeau_init_scene_terrain)
    {
        glClearColor(Scene_terrain::moteur3d->couleur_fond[0],Scene_terrain::moteur3d->couleur_fond[1],Scene_terrain::moteur3d->couleur_fond[2],Scene_terrain::moteur3d->couleur_fond[3]);
        //drapeau_init_scene1=true;
    }
}

static void timeline()
{
    GFunc::maj_temps();

    switch (GVars::scene_id)
    {
        case SCENE_SCENE1:scene_1();break;
        case SCENE_RAYMARCHING: scene_rm();break;
        case SCENE_TERRAIN: scene_terrain();break;
        default:break;
    }



}

static void affiche_infosDemo()
{
    float fps=0.;
    double dt=GVars::temps_reel-GVars::temps_prec;
    if(dt>0.)fps=1./dt;
    char chaine[1024];
    if(GVars::scene_id==SCENE_FRACTALE_2D)
    {
        sprintf(chaine, "Temps(s):%.0f\nTemps X %.1f\nTemps reel: %.0f\nfps: %.2f\nIterations:%d\nPuissance:%.2f",
                GVars::temps,
                GVars::vitesse_temps,GVars::temps_reel,
                fps,
                Fractale2d::iter,
                Fractale2d::puissance);
    }
    else if(GVars::scene_id==SCENE_RAYMARCHING)
    {
        sprintf(chaine, "Temps(s):%.0f\nTemps X %.1f\nTemps reel: %.0f\nfps: %.2f\nProf.Champ - Min: %.2f  Max:%.2f\nIterations:%d",
                GVars::temps,
                GVars::vitesse_temps,GVars::temps_reel,
                fps,
                GVars::zMin_nette,
                GVars::zMax_nette,
                Raymarching::iter);
    }
    else if(GVars::scene_id==SCENE_SCENE1)
    {
        sprintf(chaine, "Temps(s):%.0f\nTemps X %.1f\nTemps reel: %.0f\nfps: %.2f\nProf.Champ - Min: %.2f  Max:%.2f\nObs X:%.2f Y:%.2f Z:%.2f\nR:%.2f T:%.2f L:%.2f",
                GVars::temps,
                GVars::vitesse_temps,GVars::temps_reel,
                fps,
                GVars::zMin_nette,
                GVars::zMax_nette,
                Scene1::moteur3d->obs.x,Scene1::moteur3d->obs.y,Scene1::moteur3d->obs.z,
                Scene1::moteur3d->obs.r,Scene1::moteur3d->obs.t,Scene1::moteur3d->obs.l
                );
    }
    else if(GVars::scene_id==SCENE_TERRAIN)
    {
        int tId=Scene_terrain::bmTerrainId;
        sprintf(chaine, "Temps(s):%.0f\nTemps X %.1f\nTemps reel: %.0f\nfps: %.2f\nProf.Champ - Min: %.2f  Max:%.2f\nObs X:%.2f Y:%.2f Z:%.2f\nR:%.2f T:%.2f L:%.2f\nBitmap: %d\n(3-4)Amplitude: %.2f\nEchelle tex. (E-R)X:%.2f (D-F)Z:%.2f\nTaille fond. (T-Y)X:%d (G-H)Y:%d\n(C-V)Octaves: %d\n(5-6)Persistance: %.2f\n(B-N)srand: %d",
                GVars::temps,
                GVars::vitesse_temps,GVars::temps_reel,
                fps,
                GVars::zMin_nette,
                GVars::zMax_nette,
                Scene_terrain::moteur3d->obs.x,Scene_terrain::moteur3d->obs.y,Scene_terrain::moteur3d->obs.z,
                Scene_terrain::moteur3d->obs.r,Scene_terrain::moteur3d->obs.t,Scene_terrain::moteur3d->obs.l,
                tId,
                Scene_terrain::amplitude_terrain[tId],
                Scene_terrain::echelle_terrain[tId*2],Scene_terrain::echelle_terrain[tId*2+1],
                Scene_terrain::taille_fondamentale[tId*2],Scene_terrain::taille_fondamentale[tId*2+1],
                Scene_terrain::nbr_octaves[tId],Scene_terrain::persistance[tId],
                Scene_terrain::rand_init[tId]
                );
    }
    //glDisable(GL_BLEND);
    glDepthMask(GL_FALSE);
    infosDemo->affiche_chaine_2d(chaine);
    //glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}
static void draw_screen()
{
    timeline();

	glViewport(0,0,RESOLUTION_X_TRAVAIL,RESOLUTION_Y_TRAVAIL);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,FrameBuffers::fbos[FBO_ECRAN_1]);

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

    switch(GVars::scene_id)
    {
        case SCENE_SCENE1:
                        Scene1::maj_mouvements();
                        Scene1::affiche();
                        break;
        case SCENE_FRACTALE_2D:
                        Fractale2d::maj_mouvements();
                        Fractale2d::affiche();
                        break;
        case SCENE_RAYMARCHING:
                        Raymarching::maj_mouvements();
                        Raymarching::affiche();
                        break;
        case SCENE_TERRAIN:
                        Scene_terrain::maj_mouvements();
                        Scene_terrain::affiche();
                        break;
        default:break;
    }

    glViewport(0,0,GVars::largeur_ecran,GVars::hauteur_ecran);
    GFunc::affiche_texture(0,FrameBuffers::fbTex[FBO_ECRAN_1]);
    affiche_infosDemo();
    glfwSwapBuffers(GVars::ecran);
}
