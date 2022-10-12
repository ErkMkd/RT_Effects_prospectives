#include "Contexte3d.h"

//====================== Gestion Uniforms Blocks ==================================================

    shaders_matrices Contexte3d::ubo_matrices;
    GLuint Contexte3d::ubo_matrices_id=0;
    shaders_eclairage_2si Contexte3d::ubo_eclairage2si;
    GLuint Contexte3d::ubo_eclairage2si_id=0;

    shaders_eclairage_3sp Contexte3d::ubo_eclairage3sp;
    GLuint Contexte3d::ubo_eclairage3sp_id=0;

    bool Contexte3d::init_ubos()
    {
        //Création des buffers:
            glGenBuffers(1,&ubo_matrices_id);
            glGenBuffers(1,&ubo_eclairage2si_id);

            glBindBuffer(GL_UNIFORM_BUFFER,ubo_matrices_id);
                glBufferData(GL_UNIFORM_BUFFER,sizeof(ubo_matrices),&ubo_matrices,GL_DYNAMIC_COPY);

            glBindBuffer(GL_UNIFORM_BUFFER,ubo_eclairage2si_id);
                glBufferData(GL_UNIFORM_BUFFER,sizeof(ubo_eclairage2si),&ubo_eclairage2si,GL_DYNAMIC_COPY);

            glBindBuffer(GL_UNIFORM_BUFFER,0);

            if(!glIsBuffer(ubo_matrices_id) || !glIsBuffer(ubo_eclairage2si_id)) return false;

        //Initialise les pointeurs UBO:

            glBindBufferBase(GL_UNIFORM_BUFFER,C3D_BINDING_POINT_UBO_MATRICES,ubo_matrices_id);
                glUniformBlockBinding(Shaders3d::programme_flat,Shaders3d::flat_matrices,C3D_BINDING_POINT_UBO_MATRICES);
                glUniformBlockBinding(Shaders3d::programme_gouraud2si,Shaders3d::gouraud2si_matrices,C3D_BINDING_POINT_UBO_MATRICES);
                glUniformBlockBinding(Shaders3d::programme_phong2si,Shaders3d::phong2si_matrices,C3D_BINDING_POINT_UBO_MATRICES);
                glUniformBlockBinding(Shaders3d::programme_texture,Shaders3d::texture_matrices,C3D_BINDING_POINT_UBO_MATRICES);
                glUniformBlockBinding(Shaders3d::programme_texture_gouraud2si,Shaders3d::texture_gouraud2si_matrices,C3D_BINDING_POINT_UBO_MATRICES);
                glUniformBlockBinding(Shaders3d::programme_texture_phong2si,Shaders3d::texture_phong2si_matrices,C3D_BINDING_POINT_UBO_MATRICES);

                glUniformBlockBinding(ShadersDemo::programme_nuages,ShadersDemo::uniform_block_nuages_matrices,C3D_BINDING_POINT_UBO_MATRICES);

                glUniformBlockBinding(ShadersPrimitives::programme_prim_phong2si,ShadersPrimitives::prim_phong2si_matrices,C3D_BINDING_POINT_UBO_MATRICES);

            glBindBufferBase(GL_UNIFORM_BUFFER,C3D_BINDING_POINT_UBO_ECLAIRAGE_2SI,ubo_eclairage2si_id);
                glUniformBlockBinding(Shaders3d::programme_gouraud2si,Shaders3d::gouraud2si_eclairages,C3D_BINDING_POINT_UBO_ECLAIRAGE_2SI);
                glUniformBlockBinding(Shaders3d::programme_phong2si,Shaders3d::phong2si_eclairages,C3D_BINDING_POINT_UBO_ECLAIRAGE_2SI);
                glUniformBlockBinding(Shaders3d::programme_texture_gouraud2si,Shaders3d::texture_gouraud2si_eclairages,C3D_BINDING_POINT_UBO_ECLAIRAGE_2SI);
                glUniformBlockBinding(Shaders3d::programme_texture_phong2si,Shaders3d::texture_phong2si_eclairages,C3D_BINDING_POINT_UBO_ECLAIRAGE_2SI);

                glUniformBlockBinding(ShadersDemo::programme_rmt,ShadersDemo::uniform_block_rmt_eclairages,C3D_BINDING_POINT_UBO_ECLAIRAGE_2SI);
                glUniformBlockBinding(ShadersDemo::programme_rmGare,ShadersDemo::uniform_block_rmGare_eclairages,C3D_BINDING_POINT_UBO_ECLAIRAGE_2SI);
                glUniformBlockBinding(ShadersDemo::programme_nuages,ShadersDemo::uniform_block_nuages_eclairages,C3D_BINDING_POINT_UBO_ECLAIRAGE_2SI);

                glUniformBlockBinding(ShadersPrimitives::programme_prim_phong2si,ShadersPrimitives::prim_phong2si_eclairages,C3D_BINDING_POINT_UBO_ECLAIRAGE_2SI);

        return true;
    }

    void Contexte3d::detruit_ubos()
    {
        if(glIsBuffer(ubo_matrices_id))glDeleteBuffers(1,&ubo_matrices_id);
        if(glIsBuffer(ubo_eclairage2si_id))glDeleteBuffers(1,&ubo_eclairage2si_id);
    }

//========================================================================

    Contexte3d::Contexte3d()
    {
        erreur=C3D_OK;
        for(int i=0;i<C3D_MAX_LUMIERES;i++) lumieres[i]=NULL;

        zMin=0.1;
        zMax=1000.;
        determine_taille_ecran(RESOLUTION_X_TRAVAIL,RESOLUTION_Y_TRAVAIL);
        determine_Fov(100.);
        obs.initXYZ(0.,0.,0.);
        obs.initRTL(0.,0.,0.);

        zMin_nette=2.;
        zMax_nette=4.;

        //sources lumineuses:
        for(int i=0;i<C3D_MAX_LUMIERES;i++) lumieres[i]=new Source_Lumineuse();

        //Ambiance:
        determine_couleur_fond(0.,0.,0.,1.);
        determine_lumiere_ambiante(0.,0.,0.,1.);
        determine_couleur_brouillard(0.,0.,0.,1.);
        determine_limites_brouillard(10.,1000.);

        //Init du modèle d'éclairage de base 2 sources infinies:
        lumieres[0]->determine_ambiance(.01,0.01,0.2,1.);
        lumieres[0]->determine_couleur(1.,1.,1.,1.);
        lumieres[0]->initRTL(0.,45.,30.-180.);
        lumieres[0]->intensite=1.;

        lumieres[1]->determine_ambiance(0.,0.,0.,1.);
        lumieres[1]->determine_couleur(60./255.,100./255.,142./255.,1.);
        lumieres[1]->initRTL(0.,-45.,30);
        lumieres[1]->intensite=1.;

    }

    Contexte3d::~Contexte3d()
    {
        for(int i=0;i<C3D_MAX_LUMIERES;i++) if(lumieres[i]!=NULL) delete lumieres[i];
    }

    void Contexte3d::determine_couleur_fond(float r, float v, float b, float a)
    {
        couleur_fond[0]=r;couleur_fond[1]=v;couleur_fond[2]=b;couleur_fond[3]=a;
    }

    void Contexte3d::determine_lumiere_ambiante(float r, float v, float b, float a)
    {
        lumiere_ambiante[0]=r;lumiere_ambiante[1]=v;lumiere_ambiante[2]=b;lumiere_ambiante[3]=a;
    }

    void Contexte3d::determine_couleur_brouillard(float r, float v, float b, float a)
    {
        couleur_brouillard[0]=r;couleur_brouillard[1]=v;couleur_brouillard[2]=b;couleur_brouillard[3]=a;
    }

    void Contexte3d::determine_limites_brouillard(float b_zmin, float b_zmax)
    {
        brouillard_min=b_zmin;
        brouillard_max=b_zmax;
    }

    void Contexte3d::determine_Fov(float p_Fov)
    {
        Fov=p_Fov;
        maj_distanceFocale();
    }

    void Contexte3d::maj_distanceFocale()
    {
        distanceFocale=1./(2.*tan(Fov/180.*M_PI/2.));
    }

    void Contexte3d::determine_taille_ecran(int largeur, int hauteur)
    {
        largeur_ecran=largeur;
        hauteur_ecran=hauteur;
        ratio_ecran=float(hauteur_ecran)/float(largeur_ecran);
    }

    void Contexte3d::determine_profondeur(float p_zMin, float p_zMax)
    {
        zMin=p_zMin;
        zMax=p_zMax;
    }

    void Contexte3d::determine_profondeur_de_champ(float p_zMin, float p_zMax)
    {
        zMin_nette=p_zMin;
        zMax_nette=p_zMax;
        //if(zMin_nette<0.)zMin_nette=0.;
        if(zMax_nette<zMin_nette)zMax_nette=zMin_nette;

    }

    void Contexte3d::calcul_matrice_projection()
    {
        float frustum_largeur=.5/distanceFocale*zMin;
        float frustum_hauteur=ratio_ecran/2./distanceFocale*zMin;
        matrice_projection=glm::frustum(-frustum_largeur,frustum_largeur,-frustum_hauteur,frustum_hauteur,zMin,zMax);
    }

    void Contexte3d::calcul_matrice_observateur()
    {
        glm::mat4 rot_r=glm::rotate(glm::mat4(1.),-obs.r*float(DEG_RAD),glm::vec3(0.,0.,1.));
        glm::mat4 rot_t=glm::rotate(rot_r,-obs.t*float(DEG_RAD),glm::vec3(1.,0.,0.));
        glm::mat4 rot_l=glm::rotate(rot_t,-obs.l*float(DEG_RAD),glm::vec3(0.,1.,0.));
        matrice_observateur=glm::translate(rot_l,glm::vec3(-obs.x,-obs.y,-obs.z));
        //matrice_observateur=glm::lookAt()
        matrice_normaleObservateur = glm::inverseTranspose(glm::mat3x3(matrice_observateur));
    }

    void Contexte3d::calcul_matrice_modelView(glm::mat4 &matrice_objet)
    {
        matrice_modelView=matrice_observateur*matrice_objet;
    }

//***************************************************************
//      Gestion des blocs Uniform
//***************************************************************


    //Les données contenues dans les blocs Uniform peuvent être modifiées à chaque rendu
    void Contexte3d::maj_ubo_matrices()
    {
        matrice_modelViewProjection=matrice_projection*matrice_modelView;
        matrice_normaleModel = glm::inverseTranspose(glm::mat3x3(matrice_modelView));
        glm::mat3x4 matriceNormal_align=glm::mat3x4(matrice_normaleModel);

        memcpy(&ubo_matrices.mat_projection[0],&matrice_projection[0][0],sizeof(ubo_matrices.mat_projection));
        memcpy(&ubo_matrices.mat_modelView[0],&matrice_modelView[0][0],sizeof(ubo_matrices.mat_modelView));
        memcpy(&ubo_matrices.mat_modelViewProjection[0],&matrice_modelViewProjection[0][0],sizeof(ubo_matrices.mat_modelViewProjection));
        memcpy(&ubo_matrices.mat_normale[0],&matriceNormal_align[0][0],sizeof(ubo_matrices.mat_normale));

        glBindBuffer(GL_UNIFORM_BUFFER,ubo_matrices_id);
        GLvoid* ptr=glMapBuffer(GL_UNIFORM_BUFFER,GL_WRITE_ONLY);
        memcpy(ptr,&ubo_matrices,sizeof(ubo_matrices));
        glUnmapBuffer(GL_UNIFORM_BUFFER);
        glBindBuffer(GL_UNIFORM_BUFFER,0);
    }

    void Contexte3d::maj_source_infinie(int indice)
    {
        lumieres[indice]->calcul_repere_vectoriel();
        vec3f* l_dir=lumieres[indice]->renvoie_direction();
        glm::vec3 l_dirAbs(-l_dir->x,-l_dir->y,-l_dir->z);
        l_direction=matrice_normaleObservateur*l_dirAbs;
        l_c[0]=lumieres[indice]->couleur[0]*lumieres[indice]->intensite;
        l_c[1]=lumieres[indice]->couleur[1]*lumieres[indice]->intensite;
        l_c[2]=lumieres[indice]->couleur[2]*lumieres[indice]->intensite;
        l_c[3]=lumieres[indice]->couleur[3]*lumieres[indice]->intensite;
    }

    void Contexte3d::maj_ubo_eclairage2si()
    {
        memcpy(&ubo_eclairage2si.couleur_brouillard[0],&couleur_brouillard[0],sizeof(ubo_eclairage2si.couleur_brouillard));
        memcpy(&ubo_eclairage2si.lumiere_ambiante[0],&lumiere_ambiante[0],sizeof(ubo_eclairage2si.lumiere_ambiante));
        ubo_eclairage2si.brouillard_min=brouillard_min;
        ubo_eclairage2si.brouillard_max=brouillard_max;

        maj_source_infinie(0);
        memcpy(&ubo_eclairage2si.l1_direction[0],&l_direction[0],sizeof(float)*3);
        memcpy(&ubo_eclairage2si.l1_couleur[0],&l_c[0],sizeof(ubo_eclairage2si.l1_couleur));
        memcpy(&ubo_eclairage2si.l1_ambiance[0],&lumieres[0]->ambiance[0],sizeof(ubo_eclairage2si.l1_ambiance));

        maj_source_infinie(1);
        memcpy(&ubo_eclairage2si.l2_direction[0],&l_direction[0],sizeof(float)*3);
        memcpy(&ubo_eclairage2si.l2_couleur[0],&l_c[0],sizeof(ubo_eclairage2si.l2_couleur));
        memcpy(&ubo_eclairage2si.l2_ambiance[0],&lumieres[1]->ambiance[0],sizeof(ubo_eclairage2si.l2_ambiance));

        glBindBuffer(GL_UNIFORM_BUFFER,ubo_eclairage2si_id);
        GLvoid* ptr=glMapBuffer(GL_UNIFORM_BUFFER,GL_WRITE_ONLY);
        memcpy(ptr,&ubo_eclairage2si,sizeof(ubo_eclairage2si));
        glUnmapBuffer(GL_UNIFORM_BUFFER);
        glBindBuffer(GL_UNIFORM_BUFFER,0);
    }

    void Contexte3d::maj_ubo_eclairage3sp()
    {
    }



