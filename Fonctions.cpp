#include "Fonctions.h"


GLuint GFunc::vbo_affTex=0;
GLuint GFunc::vao_affTex=0;
GLuint GFunc::vbo_afftex_bitmap=0;
GLuint GFunc::vao_afftex_bitmap=0;
GLuint GFunc::vao_affTex_zBuffer=0;
GLuint GFunc::vbo_affTex_zBuffer=0;
GLuint GFunc::vao_profChamp=0;
GLuint GFunc::vbo_profChamp=0;


bool GFunc::erreur_openGl(const char* message)
    {
        GLenum erreur_openGl=glGetError();
        if (erreur_openGl!=GL_NO_ERROR)
        {
            cout<<message<<" : "<<gluErrorString(erreur_openGl)<<endl;
            return true;
        }
        return false;
    }

void GFunc::glfw_error_callback(int error,const char* description)
{
        cout<<"Erreur GLFW: "<<error<<" - "<<description<<endl;
}

void GFunc::maj_temps()
{
    GVars::temps_prec=GVars::temps_reel;
    GVars::temps_reel=glfwGetTime();
    GVars::vitesse_temps=float(int(GVars::vitesse_temps*10.))/10.;
    GVars::temps+=(GVars::temps_reel-GVars::temps_prec)*GVars::vitesse_temps;
    //GVars::temps=GVars::temps_reel;
}

void GFunc::dimensionne_ecran()
{
                //float ratio;
                int width, height;
                glfwGetFramebufferSize(GVars::ecran, &width, &height);
                cout<<"Width: "<<width<<" - "<<"Height: "<<height<<endl;
                //ratio = width / (float) height;
                //glViewport(0, 0, width, height);
                //glClear(GL_COLOR_BUFFER_BIT);
}

void GFunc::redimensionne_ecran(GLFWwindow* window, int width, int height)
{
        GVars::largeur_ecran=width;
        GVars::hauteur_ecran=height;
        dimensionne_ecran();
        //FrameBuffers::redimensionne_frameBuffers();
        //Sprites_2d::determine_resolution(GVars::largeur_ecran,GVars::hauteur_ecran);
}

void GFunc::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //--------------Touches globales:

    //cout<<key<<endl;
    bool etat=false;
    if(action==GLFW_PRESS || action==GLFW_REPEAT) etat=true; else etat=false;

    if(key==GLFW_KEY_ESCAPE && etat==true) glfwSetWindowShouldClose(window,GL_TRUE);
    else if(key==GLFW_KEY_SPACE && etat==true) GVars::disp_donnees=true;   //A mettre à false par la scène.

    else if(key==GLFW_KEY_ENTER && etat==true)
    {
        GVars::scene_id++;
        if(GVars::scene_id>SCENE_MAX)GVars::scene_id=0;
    }

    switch (GVars::scene_id)
    {
        case SCENE_SCENE1: Scene1::clavier(key,etat);break;
        case SCENE_FRACTALE_2D: Fractale2d::clavier(key,etat);break;
        case SCENE_RAYMARCHING: Raymarching::clavier(key,etat);break;
        case SCENE_TERRAIN: Scene_terrain::clavier(key,etat);break;
        default:break;
    }

}

bool GFunc::setup_opengl()
{
        //Initialisation des vbo pour la chaine de rendu:
        GLfloat sommets_texture_ecran[2*4]{-1.,-1., -1.,1., 1.,1., 1.,-1.};
        glGenBuffers(1,&vbo_affTex);
        glGenBuffers(1,&vbo_profChamp);
        glGenBuffers(1,&vbo_afftex_bitmap);
        glGenBuffers(1,&vbo_affTex_zBuffer);

        glGenVertexArrays(1,&vao_affTex);
        glGenVertexArrays(1,&vao_afftex_bitmap);
        glGenVertexArrays(1,&vao_affTex_zBuffer);
        glGenVertexArrays(1,&vao_profChamp);

        glBindVertexArray(vao_affTex);
            glBindBuffer(GL_ARRAY_BUFFER,vbo_affTex);
                glBufferData(GL_ARRAY_BUFFER,sizeof(sommets_texture_ecran),sommets_texture_ecran,GL_STATIC_DRAW);
                glVertexAttribPointer(ShadersDemo::attrib_affTex_position,2,GL_FLOAT,GL_FALSE,0,0);
                glEnableVertexAttribArray(ShadersDemo::attrib_affTex_position);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);

        glBindVertexArray(vao_afftex_bitmap);
            glBindBuffer(GL_ARRAY_BUFFER,vbo_afftex_bitmap);
                glBufferData(GL_ARRAY_BUFFER,sizeof(sommets_texture_ecran),sommets_texture_ecran,GL_STATIC_DRAW);
                glVertexAttribPointer(ShadersDemo::attrib_affTex_bitmap_position,2,GL_FLOAT,GL_FALSE,0,0);
                glEnableVertexAttribArray(ShadersDemo::attrib_affTex_bitmap_position);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);

        glBindVertexArray(vao_affTex_zBuffer);
            glBindBuffer(GL_ARRAY_BUFFER,vbo_affTex_zBuffer);
                glBufferData(GL_ARRAY_BUFFER,sizeof(sommets_texture_ecran),sommets_texture_ecran,GL_STATIC_DRAW);
                glVertexAttribPointer(ShadersDemo::attrib_affTex_zBuffer_position,2,GL_FLOAT,GL_FALSE,0,0);
                glEnableVertexAttribArray(ShadersDemo::attrib_affTex_zBuffer_position);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);

        glBindVertexArray(vao_profChamp);
            glBindBuffer(GL_ARRAY_BUFFER,vbo_profChamp);
                glBufferData(GL_ARRAY_BUFFER,sizeof(sommets_texture_ecran),sommets_texture_ecran,GL_STATIC_DRAW);
                glVertexAttribPointer(ShadersDemo::attrib_profChamp_position,2,GL_FLOAT,GL_FALSE,0,0);
                glEnableVertexAttribArray(ShadersDemo::attrib_profChamp_position);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);

        /* Culling. */
        glCullFace( GL_BACK );
        glFrontFace( GL_CW );
        glEnable( GL_CULL_FACE );
        glDepthMask(GL_TRUE);
        /* Set the clear color. */
        glReadBuffer(GL_BACK);
        glDrawBuffer(GL_BACK);
        glClearColor( 0, 0, 0, 0 );
        glDepthRange(-1.,1.);

        return true;
}

void GFunc::remove_opengl()
{
    if (glIsBuffer(vbo_affTex)) glDeleteBuffers(1,&vbo_affTex);
    if (glIsBuffer(vbo_profChamp)) glDeleteBuffers(1,&vbo_profChamp);
    if (glIsBuffer(vbo_affTex_zBuffer)) glDeleteBuffers(1,&vbo_affTex_zBuffer);
    if (glIsVertexArray(vao_affTex)) glDeleteVertexArrays(1,&vao_affTex);
    if (glIsVertexArray(vao_affTex_zBuffer)) glDeleteVertexArrays(1,&vao_affTex_zBuffer);
    if (glIsVertexArray(vao_profChamp)) glDeleteVertexArrays(1,&vao_profChamp);
}

//====================================================================
//              Désactive toutes les textures.
//====================================================================



//A partir des matrices de projection et de transformation calcul les coordonnées du point sur l'écran:
//Le point de sortie comporte 3 coordonnées, x,y,z-Buffer
void  GFunc::calcul_coordonnees_ecran(float* pointEspace,float* pointEcran)
{
    /*
    float p[4]={pointEspace[0],pointEspace[1],pointEspace[2],1.};
    float p_temp[4];
    Matrice4x4f modelView;
    Matrice4x4f projection;
    glGetFloatv(GL_MODELVIEW_MATRIX,modelView.matrice);
    glGetFloatv(GL_PROJECTION_MATRIX,projection.matrice);
    modelView.multiplie_point3d(p,p_temp);
    projection.multiplie_point3d(p_temp,p);
    //Coordonnées normalisées:
    float inverse_w=1./p[3];
    p[0]*=inverse_w;
    p[1]*=inverse_w;
    p[2]*=inverse_w;
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT,vp);
    pointEcran[0]=(p[0]*0.5+0.5)*vp[2]+vp[0];
    pointEcran[1]=(p[1]*0.5+0.5)*vp[3]+vp[1];
    pointEcran[2]=(1.+p[2])*0.5;    //Z doit être compris entre 0 et 1, sinon il est hors-champ.
    */
}

// rotation d'un vecteur dans un plan vertical ou horizontal:
void GFunc::rotation_plan(float angle,float sX, float sY, float& dX, float& dY)
{
	float c=cosf(angle*DEG_RAD);
	float s=sinf(angle*DEG_RAD);
	dX=sX*c-sY*s;
	dY=sX*s+sY*c;
}


void GFunc::RGBi_RGBf(uint32_t RGBi,float RGBf[3])
{
    RGBf[0]=float((RGBi>>16)&0xff)/255.;
    RGBf[1]=float((RGBi>>8)&0xff)/255.;
    RGBf[2]=float(RGBi&0xff)/255.;
}

void GFunc::BGRi_RGBf(uint32_t BGRi,float RGBf[3])
{
    RGBf[2]=float((BGRi>>16)&0xff)/255.;
    RGBf[1]=float((BGRi>>8)&0xff)/255.;
    RGBf[0]=float(BGRi&0xff)/255.;
}



void GFunc::affiche_texture(GLuint id_fbo,GLuint id_texture)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,id_fbo);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,id_texture);
    glUseProgram(ShadersDemo::programme_affTex);
    glUniform1i(ShadersDemo::uniform_affTex_texture,0);
        glBindVertexArray(vao_affTex);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
    glUseProgram(0);
}

void GFunc::affiche_bitmap(GLuint id_fbo,GLuint id_texture,float echelle,int largeur_texture, int hauteur_texture, float x,float y )
{
    GLint taille_ecran[4];
    glGetIntegerv(GL_VIEWPORT,taille_ecran);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,id_fbo);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,id_texture);
    glUseProgram(ShadersDemo::programme_affTex_bitmap);
    glUniform1i(ShadersDemo::uniform_affTex_bitmap_texture,0);
    glUniform1f(ShadersDemo::uniform_afftex_bitmap_echelle,echelle);
    glUniform2f(ShadersDemo::uniform_afftex_bitmap_taille_texture,float(largeur_texture),float(hauteur_texture));
    glUniform2f(ShadersDemo::uniform_afftex_bitmap_taille_ecran,float(taille_ecran[2]),float(taille_ecran[3]));
    glUniform2f(ShadersDemo::uniform_afftex_bitmap_position_bitmap,x,y);
        glBindVertexArray(vao_afftex_bitmap);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
    glUseProgram(0);
}

void GFunc::affiche_texture_zBuffer(GLuint id_fbo,GLuint id_texture_couleur,GLuint id_texture_zBuffer)
{
    GLint taille_ecran[4];
    glGetIntegerv(GL_VIEWPORT,taille_ecran);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,id_fbo);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,id_texture_couleur);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,id_texture_zBuffer);
    glUseProgram(ShadersDemo::programme_affTex_zBuffer);
    glUniform1i(ShadersDemo::uniform_affTex_zBuffer_texture_couleur,0);
    glUniform1i(ShadersDemo::uniform_affTex_zBuffer_texture_zBuffer,1);
    glUniform1f(ShadersDemo::uniform_affTex_zBuffer_taille_tramage,GVars::taille_tramage);
    glUniform2f(ShadersDemo::uniform_affTex_zBuffer_taille_ecran,float(taille_ecran[2]),float(taille_ecran[3]));
        glBindVertexArray(vao_affTex_zBuffer);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
    glUseProgram(0);
}

void GFunc::affiche_texture_profondeur_de_champ(GLuint id_fbo,GLuint id_texture_couleur,GLuint id_texture_zBuffer)
{
    GLint taille_ecran[4];
    glGetIntegerv(GL_VIEWPORT,taille_ecran);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,id_fbo);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,id_texture_couleur);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,id_texture_zBuffer);
    glUseProgram(ShadersDemo::programme_profChamp);
    glUniform1i(ShadersDemo::uniform_profChamp_texture_couleur,0);
    glUniform1i(ShadersDemo::uniform_profChamp_texture_zBuffer,1);
    glUniform1f(ShadersDemo::uniform_profChamp_zMin,GVars::zMin);
    glUniform1f(ShadersDemo::uniform_profChamp_zMax,GVars::zMax);
    glUniform1f(ShadersDemo::uniform_profChamp_zMax_nette,GVars::zMax_nette);
    glUniform1f(ShadersDemo::uniform_profChamp_zMin_nette,GVars::zMin_nette);
    glUniform1f(ShadersDemo::uniform_profChamp_ratio_ecran,GVars::ratio_ecran);
    glUniform1f(ShadersDemo::uniform_profChamp_distance_focale,GVars::distance_focale);
    glUniform2f(ShadersDemo::uniform_profChamp_taille_ecran,float(taille_ecran[2]),float(taille_ecran[3]));
        glBindVertexArray(vao_profChamp);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
    glUseProgram(0);
}



//---------------------------------------------------------------
//Rotation de points autour d'un axe:
//---------------------------------------------------------------
void GFunc::rotation_points(float cx,float cy,float cz,float ax, float ay, float az, float angle, float* points, float* points_resultat, uint32_t numPoints)
{
	for (uint32_t i = 0; i < numPoints; i++)
	{
		float px = points[3 * i] - cx;
		float py = points[3 * i + 1] - cy;
		float pz = points[3 * i + 2] - cz;
		float prod_scal =  px * ax + py * ay + pz * az;
		float cos_angle = cos(angle);
		float sin_angle = sin(angle);

		points_resultat[3 * i] = cos_angle * px + sin_angle * (ay * pz - az * py) + (1. - cos_angle) * prod_scal * ax + cx;
		points_resultat[3 * i + 1] = cos_angle * py + sin_angle * (az * px - ax * pz) + (1. - cos_angle) * prod_scal * ay + cy;
		points_resultat[3 * i + 2] = cos_angle * pz + sin_angle * (ax * py - ay * px) + (1. - cos_angle) * prod_scal * az + cz;
	}
}


//---------------------------------------------------------------
//Rotation de points en fonction des angles de Roulis, Tangage et Lacet:
//  Les angles sont en radians.
//---------------------------------------------------------------
void GFunc::rotation_RTL(float cx, float cy, float cz,float roulis,float tangage, float lacet, float* points, float* resultat,uint32_t numPoints)
{
	float sin_aR=sin(roulis);
	float cos_aR=cos(roulis);
	float sin_aT=sin(tangage);
	float cos_aT=cos(tangage);
	float sin_aL=sin(lacet);
	float cos_aL=cos(lacet);

	for(uint32_t i=0;i<numPoints;i++)
	{
		float px=points[3*i]-cx;
		float py=points[3*i+1]-cy;
		float pz=points[3*i+2]-cz;
		resultat[3*i]= sin_aL * ( pz*cos_aT - sin_aT * ( px*sin_aR + py*cos_aR ) ) +  cos_aL * ( px*cos_aR - py*sin_aR )+cx;
		resultat[3*i+1]= pz * sin_aT + cos_aT * ( px*sin_aR + py*cos_aR )+cy;
		resultat[3*i+2]= cos_aL * ( pz*cos_aT - sin_aT * ( px*sin_aR + py*cos_aR ) ) - sin_aL * ( px*cos_aR - py*sin_aR )+cz;
	}

}
