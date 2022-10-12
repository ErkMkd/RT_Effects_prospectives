#include "Nuages.h"


Nuages::Nuages(int p_taille_textures)
{
    erreur=NUAGES_OK;
    altitude=1000.;

    taille_textures=p_taille_textures;
    bitmaps_nuages[0]=NULL;
    int i;
    for(i=0;i<NUAGES_NUM_BM;i++)textures_id[i]=0;

    bitmaps_nuages[0]=(float*)malloc(sizeof(float)*taille_textures*taille_textures*NUAGES_NUM_BM);
    if(bitmaps_nuages[0]==NULL){erreur=NUAGES_MEMOIRE;cout<<"ERREUR GRRR - Nuages() - Impossible d'allouer les bitmaps !"<<endl;return;}
    bitmaps_nuages[1]=bitmaps_nuages[0]+taille_textures*taille_textures;
    bitmaps_nuages[2]=bitmaps_nuages[1]+taille_textures*taille_textures;

    Bruit::genere_bruit_de_Perlin_2D_f(4,8,taille_textures,taille_textures,3,.75,bitmaps_nuages[0]);
    Bruit::genere_bruit_de_Perlin_2D_f(4,16,taille_textures,taille_textures,3,.75,bitmaps_nuages[1]);
    Bruit::genere_bruit_de_Perlin_2D_f(16,64,taille_textures,taille_textures,3,.85,bitmaps_nuages[2]);

    glGenTextures(NUAGES_NUM_BM,textures_id);
    //if(GFunc::erreur_openGl("ERREUR ! Pas possible de créer les textures de nuages - Nuages() - ")){erreur=NUAGES_TEXTURES;return;};


    for(i=0;i<NUAGES_NUM_BM;i++)
    {
        glBindTexture(GL_TEXTURE_2D,textures_id[i]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,taille_textures,taille_textures,0,GL_LUMINANCE,GL_FLOAT,bitmaps_nuages[i]);
    }


    GLfloat sommets_strate[3*4]{-1.,0.,-1., -1.,0.,1., 1.,0.,1., 1.,0.,-1.};
    //GLfloat sommets_strate[3*4]{1.,1.,-1., 1.,1.,1., -1.,1.,1., -1.,1.,-1.};
    glGenBuffers(1,&vbo);
    glGenVertexArrays(1,&vao);

    glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
            glBufferData(GL_ARRAY_BUFFER,sizeof(sommets_strate),sommets_strate,GL_STATIC_DRAW);
            glVertexAttribPointer(ShadersDemo::attrib_nuages_position,3,GL_FLOAT,GL_FALSE,0,0);
            glEnableVertexAttribArray(ShadersDemo::attrib_nuages_position);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

Nuages::~Nuages()
{
    if (glIsBuffer(vbo))glDeleteBuffers(1,&vbo);
    if (glIsVertexArray(vao))glDeleteVertexArrays(1,&vao);
    if(bitmaps_nuages[0]!=NULL) free(bitmaps_nuages[0]);
    if(glIsTexture(textures_id[0]))glDeleteTextures(NUAGES_NUM_BM,textures_id);
}

void Nuages::affiche(Contexte3d* c3d)
{
    initXYZ(c3d->obs.x,altitude,c3d->obs.z);
    calcul_matrice();
    c3d->calcul_matrice_modelView(matrice);
    c3d->maj_ubo_matrices();

    glUseProgram(ShadersDemo::programme_nuages);
    glUniform1f(ShadersDemo::uniform_nuages_zMin,c3d->zMin);
    glUniform1f(ShadersDemo::uniform_nuages_zMax,c3d->zMax);
    glUniform3f(ShadersDemo::uniform_nuages_obsPos,c3d->obs.x,c3d->obs.y,c3d->obs.z);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textures_id[0]);
    glUniform1i(ShadersDemo::uniform_nuages_texture_o1,0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,textures_id[1]);
    glUniform1i(ShadersDemo::uniform_nuages_texture_o2,1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,textures_id[2]);
    glUniform1i(ShadersDemo::uniform_nuages_texture_o3,2);
    glUniform1f(ShadersDemo::uniform_nuages_t,GVars::temps);


    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    //glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);

    glUseProgram(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,0);
    glActiveTexture(GL_TEXTURE0);

}
