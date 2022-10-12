#ifndef FRAMEBUFFERS_H_INCLUDED
#define FRAMEBUFFERS_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "Variables.h"

//static bool erreur_openGl(char* message);

#define NUM_FBOS 6

#define FBO_ECRAN_1 0
#define FBO_ECRAN_2 1 //Pour l'image finale
#define FBO_ECRAN_LORES 2
#define FBO_SMALL_1 3
#define FBO_SMALL_2 4
#define FBO_SCREENSHOT 5    //Sert à sauvegarder un rendu, pour les transitions

#define FBO_SMALL_L 640
#define FBO_SMALL_H 360

#define FBO_LORES_L 1280
#define FBO_LORES_H 720


//Frame buffer objets - Tempons de rendus

class FrameBuffers
{
    public:
        static GLuint 	fbos[NUM_FBOS];
        static GLuint	fbDepth[NUM_FBOS];
        static GLuint	fbTex[NUM_FBOS];
        static GLuint	fbTexDepth[NUM_FBOS];
        static GLsizei largeur_travail;
        static GLsizei hauteur_travail;
        static float alpha_screenShot;

        static bool erreur_openGl(const char* message);
        static void supprime_frameBuffers();
        static bool verification_fbo();
        static bool initialise_frameBuffers(GLsizei largeur, GLsizei hauteur);
        static bool redimensionne_frameBuffers(GLsizei largeur, GLsizei hauteur);
        static void affiche_screenShot(uint16_t id_fbo_dest);
        static void screenShot(uint16_t id_fbo_source);

};


#endif // FRAMEBUFFERS_H_INCLUDED
