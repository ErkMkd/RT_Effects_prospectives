#include "Variables.h"

GLint GVars::openGl_majorVersion=0;
GLint GVars::openGl_minorVersion=0;
GLint GVars::openGl_max_vertices=0;
GLint GVars::openGl_max_indices=0;
GLFWwindow* GVars::ecran=NULL;;
int GVars::largeur_ecran=RESOLUTION_X_VISIBLE;
int GVars::hauteur_ecran=RESOLUTION_Y_VISIBLE;

float GVars::temps=0.;
float GVars::temps_reel=0.;
float GVars::temps_scene=0.;
float GVars::vitesse_temps=1.;
float GVars::temps_prec=0.;

uint16_t GVars::scene_id=SCENE_SCENE1;
bool GVars::editionTrajectoire=false;
bool GVars::disp_donnees=false;
float GVars::taille_tramage=1.;
float GVars::zMin=1.;
float GVars::zMax=1000.;
float GVars::zMin_nette=2.;
float GVars::zMax_nette=4.;
float GVars::ratio_ecran;
float GVars::distance_focale;

