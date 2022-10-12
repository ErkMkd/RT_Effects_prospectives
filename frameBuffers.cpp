
#include "FrameBuffers.h"
#include "Fonctions.h"

using namespace std;

//Frame buffer objets - Tempons de rendus

    GLuint 	FrameBuffers::fbos[NUM_FBOS]={0};
    GLuint	FrameBuffers::fbTex[NUM_FBOS]={0};
    GLuint	FrameBuffers::fbTexDepth[NUM_FBOS]={0};
    GLuint	FrameBuffers::fbDepth[NUM_FBOS]={0};

    GLsizei FrameBuffers::largeur_travail=0;
    GLsizei FrameBuffers::hauteur_travail=0;

    float FrameBuffers::alpha_screenShot=1.;

    bool FrameBuffers::erreur_openGl(const char* message)
    {
        GLenum erreur_openGl=glGetError();
        if (erreur_openGl!=GL_NO_ERROR)
        {
            cout<<message<<" : "<<gluErrorString(erreur_openGl)<<endl;
            return true;
        }
        return false;
    }

    void FrameBuffers::supprime_frameBuffers()
    {
        glDeleteFramebuffers(NUM_FBOS,fbos);
        glDeleteRenderbuffers(NUM_FBOS,fbDepth);
        glDeleteTextures(NUM_FBOS,fbTex);
        glDeleteTextures(NUM_FBOS,fbTexDepth);
    }

    bool FrameBuffers::verification_fbo()
    {
        GLenum fb_status;
        fb_status=glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
        if(fb_status!=GL_FRAMEBUFFER_COMPLETE)
        {
            cout<<"Erreur de vérifiaction du frameBuffer. Status:"<<fb_status<<endl;
            return false;
        }
        return true;
    }

    bool FrameBuffers::initialise_frameBuffers(GLsizei largeur, GLsizei hauteur)
    {
        GLfloat couleur_bords[3]={0.,0.,0.};
        largeur_travail=largeur;
        hauteur_travail=hauteur;

        glGenTextures(NUM_FBOS,fbTex);
        if(erreur_openGl("Erreur lors de l'allocation des identifiants de textures de rendus - "))return false;
        glGenTextures(NUM_FBOS,fbTexDepth);
        if(erreur_openGl("Erreur lors de l'allocation des identifiants de textures de rendus Depths- "))return false;
        glGenFramebuffers(NUM_FBOS,fbos);
        if(erreur_openGl("Erreur lors de l'allocation des identifiants de tempons de rendus - "))return false;
        glGenRenderbuffers(NUM_FBOS,fbDepth);
        if(erreur_openGl("Erreur lors de l'allocation des identifiants de tempons de profondeur - "))return false;

        // Tempon de rendu principal:
        glBindRenderbuffer(GL_RENDERBUFFER,fbDepth[FBO_ECRAN_1]);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,largeur,hauteur);
        if(erreur_openGl("Erreur lors de la création des tempons de profondeur - "))return false;

        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_ECRAN_1]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,couleur_bords);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,largeur,hauteur,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);

        glBindTexture(GL_TEXTURE_2D,fbTexDepth[FBO_ECRAN_1]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,couleur_bords);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,largeur,hauteur,0,GL_DEPTH_COMPONENT,GL_UNSIGNED_INT,NULL);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbos[FBO_ECRAN_1]);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,fbDepth[FBO_ECRAN_1]);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,fbTex[FBO_ECRAN_1],0);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,fbTexDepth[FBO_ECRAN_1],0);

        if(!verification_fbo())return false;

        // 2ème Tempon de rendu principal:

        glBindRenderbuffer(GL_RENDERBUFFER,fbDepth[FBO_ECRAN_2]);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,largeur,hauteur);
        if(erreur_openGl("Erreur lors de la création des tempons de profondeur - "))return false;

        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_ECRAN_2]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,couleur_bords);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,largeur,hauteur,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);

        glBindTexture(GL_TEXTURE_2D,fbTexDepth[FBO_ECRAN_2]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,couleur_bords);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,largeur,hauteur,0,GL_DEPTH_COMPONENT,GL_UNSIGNED_INT,NULL);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbos[FBO_ECRAN_2]);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,fbDepth[FBO_ECRAN_2]);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,fbTex[FBO_ECRAN_2],0);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,fbTexDepth[FBO_ECRAN_2],0);

        if(!verification_fbo())return false;

        // Tempon de rendu réduit
        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_SMALL_1]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,couleur_bords);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,FBO_SMALL_L,FBO_SMALL_H,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbos[FBO_SMALL_1]);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,fbTex[FBO_SMALL_1],0);

        if(!verification_fbo())return false;

        // Tempon de rendu réduit
        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_SMALL_2]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,couleur_bords);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,FBO_SMALL_L,FBO_SMALL_H,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbos[FBO_SMALL_2]);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,fbTex[FBO_SMALL_2],0);

        if(!verification_fbo())return false;

        // Tempon de rendu dimensions écran basse résolution:
        glBindRenderbuffer(GL_RENDERBUFFER,fbDepth[FBO_ECRAN_LORES]);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,FBO_LORES_L,FBO_LORES_H);
        if(erreur_openGl("Erreur lors de la création des tempons de profondeur - "))return false;

        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_ECRAN_LORES]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,couleur_bords);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,FBO_LORES_L,FBO_LORES_H,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);

        glBindTexture(GL_TEXTURE_2D,fbTexDepth[FBO_ECRAN_LORES]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,couleur_bords);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,FBO_LORES_L,FBO_LORES_H,0,GL_DEPTH_COMPONENT,GL_UNSIGNED_INT,NULL);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbos[FBO_ECRAN_LORES]);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,fbDepth[FBO_ECRAN_LORES]);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,fbTex[FBO_ECRAN_LORES],0);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,fbTexDepth[FBO_ECRAN_LORES],0);

        if(!verification_fbo())return false;

        if(erreur_openGl("Erreur lors de l'initialisation des tempons de rendus - "))return false;

        // Tempon de sauvegarde écran:

        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_SCREENSHOT]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
        glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,couleur_bords);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,largeur,hauteur,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbos[FBO_SCREENSHOT]);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,fbTex[FBO_SCREENSHOT],0);

        if(!verification_fbo())return false;

        if(erreur_openGl("Erreur lors de l'initialisation des tempons de rendus - "))return false;


        //Restitution des tempons par défaut:
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
        glBindRenderbuffer(GL_RENDERBUFFER,0);
        glBindTexture(GL_TEXTURE_2D,0);

            return true;

    }

    bool FrameBuffers::redimensionne_frameBuffers(GLsizei largeur, GLsizei hauteur)
    {
        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_ECRAN_1]);
        glBindRenderbuffer(GL_RENDERBUFFER,fbDepth[FBO_ECRAN_1]);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,largeur,hauteur,0,GL_RGB,GL_UNSIGNED_BYTE,NULL); //Alloue la mémoire automatiquement.
        if(erreur_openGl("Erreur lors du redimensionnement des tempons de rendus - "))return false;
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,largeur,hauteur);
        if(erreur_openGl("Erreur lors du redimensionnement des tempons de profondeur - "))return false;

        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_ECRAN_2]);
        glBindRenderbuffer(GL_RENDERBUFFER,fbDepth[FBO_ECRAN_2]);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,largeur,hauteur,0,GL_RGB,GL_UNSIGNED_BYTE,NULL); //Alloue la mémoire automatiquement.
        if(erreur_openGl("Erreur lors du redimensionnement des tempons de rendus - "))return false;
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,largeur,hauteur);
        if(erreur_openGl("Erreur lors du redimensionnement des tempons de profondeur - "))return false;

        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_SCREENSHOT]);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,largeur,hauteur,0,GL_RGB,GL_UNSIGNED_BYTE,NULL); //Alloue la mémoire automatiquement.
        if(erreur_openGl("Erreur lors du redimensionnement des tempons de rendus - "))return false;

        /*
        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_ECRAN_LORES]);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,largeur/2,hauteur/2,0,GL_RGB,GL_UNSIGNED_BYTE,NULL); //Alloue la mémoire automatiquement.
        if(erreur_openGl("Erreur lors du redimensionnement des tempons de rendus - "))return false;
        return true;
        */
    }

    void FrameBuffers::screenShot(uint16_t id_fbo_source)
    {
        glViewport(0,0,largeur_travail,hauteur_travail);
        GFunc::affiche_texture(fbos[FBO_SCREENSHOT],fbTex[id_fbo_source]);
    }

    void FrameBuffers::affiche_screenShot(uint16_t id_fbo_dest)
    {
        GFunc::affiche_texture(fbos[id_fbo_dest],fbTex[FBO_SCREENSHOT]);
    }



