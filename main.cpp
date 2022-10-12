#include "glew.h"
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "math.h"
#include "bass/bass.h"

#include "Variables.h"
#include "Fonctions.h"
#include "FrameBuffers.h"
#include "Lumieres.h"
#include "Primitives/ShadersPrimitives.h"
#include "Primitives/Primitives.h"
#include "ShadersDemo.h"
#include "Objets3d/Shaders3d.h"
#include "timeline.cpp"

#define FULLSCREEN false
#define VSYNC true

using namespace std;

void detruit_shaders()
{
    ShadersPrimitives::freeShaders();
    Shaders3d::freeShaders();
    ShadersDemo::freeShaders();
}

void fin_programme( int code=0 )
{
    detruit_timeline();
    Contexte3d::detruit_ubos();
    FrameBuffers::supprime_frameBuffers();
    GFunc::remove_opengl();
    if(GVars::ecran!=NULL) glfwDestroyWindow(GVars::ecran);
    detruit_shaders();
    glfwTerminate();
    cout<<"Et zouuu , Fin du programme..."<<endl;
    exit( code );
}

bool init_shaders()
{
    if(!ShadersDemo::initShaders())return false;
    if(!Shaders3d::initShaders())return false;
    if(!ShadersPrimitives::initShaders())return false;
    return true;
}

int main()
{
    glfwSetErrorCallback(GFunc::glfw_error_callback);
    if(!glfwInit())
    {
        cout<<"Heee meeerde ! Erreur lors de l'initialisation de GLFW"<<endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);

    #if FULLSCREEN
    GVars::ecran = glfwCreateWindow(GVars::largeur_ecran,GVars::hauteur_ecran, "MkdProject", glfwGetPrimaryMonitor(), NULL);
    #else
    GVars::ecran = glfwCreateWindow(GVars::largeur_ecran,GVars::hauteur_ecran, "MkdProject", NULL, NULL);
    #endif

    if(GVars::ecran==NULL)
    {
        cout<<"Erreur - Putain,impossible d'initialiser cette merde d'écran"<<endl;
        fin_programme(EXIT_FAILURE);
    }
    glfwSetInputMode(GVars::ecran,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
    glfwMakeContextCurrent(GVars::ecran);
    glfwSetWindowSizeCallback(GVars::ecran,GFunc::redimensionne_ecran);
    glfwSetKeyCallback(GVars::ecran,GFunc::key_callback);

    cout<<"==================================================================="<<endl;
    cout<<"                     Informations OpenGl"<<endl;
    cout<<"==================================================================="<<endl<<endl;
    glGetIntegerv(GL_MAJOR_VERSION,&GVars::openGl_majorVersion);
    glGetIntegerv(GL_MINOR_VERSION,&GVars::openGl_minorVersion);
    glGetIntegerv(GL_MAX_ELEMENTS_VERTICES,&GVars::openGl_max_vertices);
    glGetIntegerv(GL_MAX_ELEMENTS_INDICES,&GVars::openGl_max_indices);
    cout<<"Version: "<<glGetString(GL_VERSION)<<endl;
    cout<<"GVars::openGl_majorVersion:"<<GVars::openGl_majorVersion<<endl;
    cout<<"GVars::openGl_minorVersion:"<<GVars::openGl_minorVersion<<endl;
    cout<<"Max elements vertices:"<<GVars::openGl_max_vertices<<endl;
    cout<<"Max elements indices:"<<GVars::openGl_max_indices<<endl;
    cout<<"Chipset graphique: "<<glGetString(GL_RENDERER)<<endl;
    cout<<"Fabricant: "<<glGetString(GL_VENDOR)<<endl;
    //cout<<"Extensions: "<<glGetString(GL_EXTENSIONS)<<endl;
    cout<<endl;

    //=============== Init Glew

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
            fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
            fin_programme(EXIT_FAILURE);
    }
    else cout<<"Status: Using GLEW "<< glewGetString(GLEW_VERSION)<<endl;

    if(!init_shaders())
    {
        cout<<"Impossible de créer les shaders...et plouf."<<endl;
        fin_programme(EXIT_FAILURE);
    }
    cout<<"Shaders initialisés avec succès!"<<endl;


    FrameBuffers::initialise_frameBuffers(RESOLUTION_X_TRAVAIL,RESOLUTION_Y_TRAVAIL);

    if(!GFunc::setup_opengl())
    {
        cout<<"ERREUR à la con- Pas possible de créer le contexte OpenGl !"<<endl;
        fin_programme(EXIT_FAILURE);
    }

    if(!Contexte3d::init_ubos())
    {
        cout<<"ERREUR zuut - Pas possible de créer les Uniforms Blocks Object pourris du Contexte3d !"<<endl;
        fin_programme(EXIT_FAILURE);
    }

    if(!init_timeline())
    {
        cout<<"Merdouille de timeline qui veut pas s'initialiser. Fuckof."<<endl;
        fin_programme(EXIT_FAILURE);
    }

    glViewport(0,0,GVars::largeur_ecran,GVars::hauteur_ecran);

    glfwSetTime(0.);

    #if VSYNC
        glfwSwapInterval(1);
    #else
        glfwSwapInterval(0);
    #endif

    //=====================================================
    //          Boucle principale
    //=====================================================

    while(!glfwWindowShouldClose(GVars::ecran))
    {
        draw_screen();
        glfwPollEvents();
    }

    fin_programme(EXIT_SUCCESS);
}
