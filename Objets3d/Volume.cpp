

//*****************************************************************************************************************************************************
//
//                                              Gestion des volumes
//
//  Lors de la création d'un volume, TOUJOURS déterminer les faces PUIS les surfaces. Sinon certaines initialisations ne sont font pas correctement.
//                                  -------------------------------------------------
//
///     Les volumes ici sont statiques, ils ne sont pas prévus pour des déformations de sommets, à moins de passer par les shaders.
//
///     Note sur les tangentes et les binormales:
///         Elles sont approximatives sur les sommets(du fait des contraintes techniques).
///         L'adoucissement n'est pas toujours tout à fait juste, mais suffisant pour rendre la rugosité.
///         Plus les textures sont déformées sur les faces, moins la rugosité est juste.
//
//******************************************************************************************************************************************************

#include "Volume.h"

using namespace std;

        bool Volume::erreur_openGl(const char* message)
            {
                GLenum erreur_openGl=glGetError();
                if (erreur_openGl!=GL_NO_ERROR)
                {
                    cout<<message<<" : "<<gluErrorString(erreur_openGl)<<endl;
                    return true;
                }
                return false;
            }

//========================================================================================================
//==================================== Constructeur du volume:
//========================================================================================================

        Volume::Volume( char* p_nom,
                uint32_t p_drapeaux_liberation_memoire,
                float px, float py, float pz,
                float pr,float pt,float pl,
                GLfloat* pSommets,
                uint32_t p_nbrSommets,
                Face** pFaces,
                uint32_t p_nbrFaces,
                Surface** pSurfaces,
                uint32_t p_nbrSurfaces)
		{

            x=px;y=py;z=pz;
            r=pr;t=pt;l=pl;
            ex=1.;ey=1.;ez=1.;
            copie_nom(p_nom);
		    drapeau_debug=false;
		    taille_points_sommets=3.;
		    //std::cout<<"------- Création du volume: "<<p_nom<<std::endl;
            initRVBA(noir,0.,0.,0.,1.);
            initRVB(couleur_sommets,1.,1.,1.);
            initRVB(couleur_aretes,1.,1.,1.);


            //-------- Initialise les variables par défaut:
		    erreur=VOLUME_OK;
		    drapeau_affiche=true;
		    sommets=NULL;
		    faces=NULL;
		    surfaces=NULL;

		    normales_sommets=NULL;
		    tangentes_sommets=NULL;
		    binormales_sommets=NULL;
		    normales_sommets=NULL;

            nbrSommets=0;
		    nbrFaces=0;
		    nbrAretes=0;
		    nbrSurfaces=0;

            drapeau_affecte_par_brouillard=true;

            drapeaux_liberation_memoire=p_drapeaux_liberation_memoire;

		    echelle_geometrique=1.;
		    facteur_luminescence=1.;
		    epsilon=1e-7;

            sens_polygones=GL_CCW;

            //Paramètres d'OpenGl:
            ///Voir à tester ces valeurs

		    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&nbr_attributs_sommets_max);


            //========================Création du volume:
            if (VOLUME_DEBUG) cout<<"------------------ Création du volume: "<<nom<<endl;


		    //--------Initialise les sommets:

            if (!determine_sommets(pSommets,p_nbrSommets)) return;

            //---------Initialise les faces:

		    if (!determine_faces(pFaces,nbrFaces)) return;

            //--------- Initialise les surfaces:

            if (!determine_surfaces(pSurfaces,nbrSurfaces)) return;

            //-------- Création des VBOs:

            ///Appelé à la fin de determine_surfaces():
            //if (!creation_VBOs()) return;

            initialise_pointeurs_rendus();

            determine_mode_remplissage(VOLUME_REMPLISSAGE_SURFACES);
		}

		//-------- Copie le nom:

            void Volume::copie_nom(char* p_nom)
            {
                uint32_t i,taille;
                taille=strlen(p_nom);
                if (taille>255)taille=255;
                for(i=0;i<taille;i++)
                {
                    nom[i]=p_nom[i];
                }
                nom[i]=0;
            }

        //===========================================================
		//                      Constructeur de copie:
		//===========================================================
        Volume::Volume(Volume& vol_org)
        {
            initXYZ(vol_org.x,vol_org.y,vol_org.z);
            initRTL(vol_org.r,vol_org.t,vol_org.l);
            initEchelle(1.,1.,1.);
            drapeau_debug=vol_org.drapeau_debug;
            uint32_t i;
            drapeaux_liberation_memoire=0;
            erreur=VOLUME_OK;
            sommets=NULL;
            normales_sommets=NULL;
            tangentes_sommets=NULL;
            binormales_sommets=NULL;
            nbrSommets=0;
            faces=NULL;
            nbrFaces=0;
            surfaces=NULL;
            nbrSurfaces=0;

		    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&nbr_attributs_sommets_max);

            copie_nom(vol_org.nom);

            echelle_geometrique=vol_org.echelle_geometrique;
            sens_polygones=vol_org.sens_polygones;
            facteur_luminescence=vol_org.facteur_luminescence;

            //------ Création des sommets:

            uint32_t p_nbrSommets=vol_org.nbrSommets;
            float* sommets_org=vol_org.renvoie_sommets();
            float* p_sommets=NULL;

            if (p_nbrSommets>0)
            {
                p_sommets=(float*)malloc(p_nbrSommets*3*sizeof(float));

                if (p_sommets==NULL)
                {
                    cout<<"ERREUR dans Volume::Volume(Volume&) - impossible d'allouer la table des sommets"<<endl;
                    erreur=VOLUME_ERREUR_MEMOIRE;
                    return;
                }
            }

            for(i=0;i<p_nbrSommets*3;i++)p_sommets[i]=sommets_org[i];
            determine_sommets(p_sommets,p_nbrSommets);
            drapeaux_liberation_memoire|=VOLUME_LIBERE_SOMMETS;

            //------------- Création des faces

            uint32_t p_nbrFaces=vol_org.nbrFaces;
            Face** p_faces=NULL;
            if (p_nbrFaces>0)
            {
                p_faces=(Face**)calloc(p_nbrFaces,sizeof(Face*));
                if (p_faces==NULL)
                {
                    cout<<"ERREUR dans Volume::Volume(Volume&) - impossible d'allouer la table des faces"<<endl;
                    erreur=VOLUME_ERREUR_MEMOIRE;
                    return;
                }
            }
            for(i=0;i<p_nbrFaces;i++)
            {
                p_faces[i]=new Face(*vol_org.renvoie_face(i));
            }
            determine_faces(p_faces,p_nbrFaces);
            drapeaux_liberation_memoire|=VOLUME_LIBERE_FACES;

            //-------Création des surfaces:


            uint32_t p_nbrSurfaces=vol_org.nbrSurfaces;
            Surface** p_surfaces=NULL;
            if (p_nbrSurfaces>0)
            {
                p_surfaces=(Surface**)calloc(p_nbrSurfaces,sizeof(Surface*));
                if (p_surfaces==NULL)
                {
                    cout<<"ERREUR dans Volume::Volume(Volume&) - impossible d'allouer la table des surfaces"<<endl;
                    erreur=VOLUME_ERREUR_MEMOIRE;
                    return;
                }
            }
            for(i=0;i<p_nbrSurfaces;i++)
            {
                p_surfaces[i]=new Surface(*vol_org.renvoie_surface(i));
            }
            determine_surfaces(p_surfaces,p_nbrSurfaces);
            drapeaux_liberation_memoire|=VOLUME_LIBERE_SURFACES;

            //------ Pointeurs de rendus:

            initialise_pointeurs_rendus();

            //------ Drapeaux:
            drapeau_affecte_par_brouillard=vol_org.drapeau_affecte_par_brouillard;
            drapeau_affiche=vol_org.drapeau_affiche;

            //-------- Mode de remplissage:

            determine_mode_remplissage(vol_org.mode_remplissage);
        }

		//===========================================================
		//                      Destructeur:
		//===========================================================

        Volume::~Volume()
        {
            if (VOLUME_DEBUG) cout<<"Destruction du volume: "<<nom<<endl;
            supprime_surfaces();
            //if(strcmp("baballe",nom)==0) cout<<"0"<<endl;
            supprime_faces();
            //if(strcmp("baballe",nom)==0) cout<<"1"<<endl;
            supprime_sommets();
            //if(strcmp("baballe",nom)==0) cout<<"2"<<endl;
            supprime_aretes();
        }


        //------- Supprime les surfaces:
        /// La table des surfaces est également supprimée !
        void Volume::supprime_surfaces()
        {
            uint32_t i;
            if ((drapeaux_liberation_memoire & VOLUME_LIBERE_SURFACES)!=0 && surfaces!=NULL)
            {
                for(i=0; i<nbrSurfaces;i++)
                {

                    if (surfaces[i]!=NULL) delete surfaces[i];
                }
                free(surfaces);
            }
            surfaces_transparentes.clear();
            nbrSurfaces_transparentes=0;
            surfaces=NULL;
            nbrSurfaces=0;
        }


        //------- Supprime les faces:
        void Volume::supprime_faces()
        {
            uint32_t i;
            if (faces!=NULL)
            {
                if ((drapeaux_liberation_memoire & VOLUME_LIBERE_FACES)!=0)
                {
                    for(i=0;i<nbrFaces;i++)
                    {
                        if (faces[i]!=NULL) delete faces[i];
                    }
                    free(faces);

                }
                faces=NULL;
            }
            nbrFaces=0;
        }

        //------- Supprime les sommets:
        void Volume::supprime_sommets()
        {
            if ( sommets != NULL )
            {
                if ((drapeaux_liberation_memoire & VOLUME_LIBERE_SOMMETS)!=0) free (sommets);
                sommets=NULL;
            }

            if ( normales_sommets != NULL )
            {
                free(normales_sommets);
                normales_sommets=NULL;
            }

            if ( tangentes_sommets != NULL )
            {
                free(tangentes_sommets);
                tangentes_sommets=NULL;
            }

            if ( binormales_sommets != NULL )
            {
                free(binormales_sommets);
                binormales_sommets=NULL;
            }


            nbrSommets=0;
        }

        //---------- Supprime la table des arêtes:
		void Volume::supprime_aretes()
		{
		    aretes.clear();
		    nbrAretes=0;
		}


        //=============================================================================================================================
        //                      Fonctions de définition de la forme du volume
        //=============================================================================================================================


        void Volume::initRVBA(float* couleur,float r, float v, float b, float a)
        {
            couleur[0]=r;couleur[1]=v;couleur[2]=b;couleur[3]=a;
        }

        void Volume::initRVB(float* couleur,float r, float v, float b)
        {
            couleur[0]=r;couleur[1]=v;couleur[2]=b;
        }

        //------------------------------------------
        //  Détermine les sommets du volume.
        //  La table en entrée est utilisée telle quelle, sans être copiée !!
        /// Il faut veiller à ce que les liens avec les faces restent cohérents.
        //------------------------------------------

        bool Volume::determine_sommets(GLfloat* pSommets,uint32_t p_nbrSommets)
        {
            supprime_sommets();

            nbrSommets=p_nbrSommets;
            sommets=pSommets;
            if (nbrSommets>0)
            {
                normales_sommets=(float*)malloc(sizeof(float)*nbrSommets*3);
                tangentes_sommets=(float*)malloc(sizeof(float)*nbrSommets*3);
                binormales_sommets=(float*)malloc(sizeof(float)*nbrSommets*3);
                if (normales_sommets==NULL || tangentes_sommets==NULL || binormales_sommets==NULL)
                {
                    cout<<"Erreur dans Volume::determine_sommets() - Mémoire insuffisante"<<endl;
                    erreur=VOLUME_ERREUR_MEMOIRE;
                    return false;
                }
            }
            calcul_centre_geometrique();
            //Création du Vertex Buffer Object:
            return true;
        }

        //------------------------------------------------
        //      Détermine l'échelle géométrique du volume
        ///      L'échelle géométrique modifie les sommets !
        //---------------------taille_points_sommets---------------------------
        void Volume::determine_echelle_geometrique(float e)
        {
            double ne=e/echelle_geometrique;
            for(uint32_t i=0;i<nbrSommets;i++)
            {
                sommets[3*i]*=ne;
                sommets[3*i+1]*=ne;
                sommets[3*i+2]*=ne;
            }
            echelle_geometrique=e;
        }

        //------------------------------------------
        //  Détermine les faces du volume.
        /// Il faut veiller à ce que les liens avec les surfaces restent cohérents.
        //------------------------------------------

        bool Volume::determine_faces(Face** pFaces,uint32_t p_nbrFaces)
        {
            supprime_faces();

            faces=pFaces;
            nbrFaces=p_nbrFaces;

            return initialise_faces();

        }

        //------------------------------------------
        //  Détermine les surfaces du volume.
        ///Il faut d'abord initialiser les faces, sinon
        ///les tables des faces des textures ne peuvent pas être créées.
        //------------------------------------------
        bool Volume::determine_surfaces(Surface** pSurfaces, uint32_t p_nbrSurfaces)
        {

            supprime_surfaces();
            surfaces=pSurfaces;
            nbrSurfaces=p_nbrSurfaces;

            if (VOLUME_DEBUG) cout<<"Détermine les surfaces du volume "<<nom<<" Nombre: "<<nbrSurfaces<<endl;
            if (!initialise_surfaces()) return false;
            if (!initialise_textures()) return false;
            initialise_modes_rendus_surfaces();
            if (!creation_VBOs())
            {
                return false;
            }
            return true;
        }


        //=============================================================================================================================
        //                      Initialisation des données géométriques relatives du volume
        //=============================================================================================================================

        //--------------------------------------------------------------------------------------
        //             Calcul les normales des sommets du volume
        //--------------------------------------------------------------------------------------
        void Volume::calcul_normales_sommets(uint32_t indice_depart,uint32_t nbr)
        {
            uint32_t i,j;
            if (nbr==0 && indice_depart==0) nbr=nbrSommets;
            else nbr+=indice_depart;

            for(i=indice_depart;i<nbr;i++)
            {
                GLfloat* nml_s=&normales_sommets[3*i];
                nml_s[0]=0.;
                nml_s[1]=0.;
                nml_s[2]=0.;
                liaison_faces* sommet_faces=&liaisons_sommets_faces[i];
                for (j=0;j<sommet_faces->indices_faces.size();j++)
                {
                    Face* face=faces[sommet_faces->indices_faces[j]];
                    nml_s[0]+=face->normale[0];
                    nml_s[1]+=face->normale[1];
                    nml_s[2]+=face->normale[2];
                }
                float nrm= sqrtf( nml_s[0]*nml_s[0] + nml_s[1]*nml_s[1] + nml_s[2]*nml_s[2]);
                nml_s[0]/=nrm;
                nml_s[1]/=nrm;
                nml_s[2]/=nrm;
                #if VOLUME_DEBUG && VOLUME_DEBUG_NORMALES_SOMMETS
                    cout<<"Sommet:"<<i<<"N:"<<nml_s[0]<<","<<nml_s[1]<<","<<nml_s[2]<<endl;
                #endif
            }
        }

        //------------------------------------------------------------------------------------------------------
        //             Calcul des tangentes et des binormales aux sommets du volume.
        //             Ces repères ne tiennent pas compte de l'orientation UV des textures,
        //              mais ils peuvent servir pour des shaders appliqués aux faces non texturées.
        //------------------------------------------------------------------------------------------------------
        void Volume::calcul_tangentes_binormales_sommets()
        {
            uint32_t i;
            for(i=0;i<nbrSommets;i++)
            {
                GLfloat* tgt_s=&tangentes_sommets[3*i];
                GLfloat* nml_s=&normales_sommets[3*i];

                //Tangente par produit vectoriel:
                //  ->  ->  ->
                //  Y ^ N = T
                tgt_s[1]=0;
                if (nml_s[1]==1.)
                {
                    tgt_s[0]=1.;
                    tgt_s[2]=0.;
                }
                else if (nml_s[1]==-1.)
                {
                    tgt_s[0]=-1.;
                    tgt_s[2]=0.;
                }
                else
                {
                    tgt_s[0]=nml_s[2];  //Simplification d'un produit vectoriel entre la normale et l'axe vertical (0,1,0)
                    tgt_s[2]=-nml_s[0];
                    GLfloat norme=sqrtf(tgt_s[0]*tgt_s[0]+tgt_s[2]*tgt_s[2]);
                    tgt_s[0]/=norme;
                    tgt_s[2]/=norme;
                }

                binormales_sommets[3*i]=nml_s[1]*tgt_s[2]-nml_s[2]*tgt_s[1];
                binormales_sommets[3*i+1]=nml_s[2]*tgt_s[0]-nml_s[0]*tgt_s[2];
                binormales_sommets[3*i+2]=nml_s[0]*tgt_s[1]-nml_s[1]*tgt_s[0];
            }
        }

        //--------------------------------------------------------------------------------------
        //              Calcul du centre géométrique relatif du volume
        //--------------------------------------------------------------------------------------
        void Volume::calcul_centre_geometrique()
        {
            uint32_t i;
            if(nbrSommets==0)
            {
                centre_geometrique[0] = 0.;
                centre_geometrique[1] = 0.;
                centre_geometrique[2] = 0.;
            }
            else
            {
                float cx=0;
                float cy=0;
                float cz=0;
                for (i=0;i<nbrSommets*3;)
                {
                    cx+= sommets[i++];
                    cy+= sommets[i++];
                    cz+= sommets[i++];
                }

                centre_geometrique[0] = cx/(float)nbrSommets;
                centre_geometrique[1] = cy/(float)nbrSommets;
                centre_geometrique[2] = cz/(float)nbrSommets;

            }
        }

        //=============================================================================================================================
        //                      Initialisation des faces
        /// Les surfaces doivent être déterminées après les faces.
        //=============================================================================================================================
        bool Volume::initialise_faces()
        {

            uint32_t i;
            for (i=0;i<nbrFaces;i++)
            {

                faces[i]->indice=i;
                faces[i]->volume=this;
                calcul_centre_gravite_face(faces[i]);
                calcul_normale_face(faces[i]);
                calcul_tangente_binormale_face(faces[i]);
            }

            initialise_liaisons_sommets_faces();
            calcul_normales_sommets();
            calcul_tangentes_binormales_sommets();

            return creation_aretes();
        }

        //----------------------------------------------------------------
        //  A appeler lorsque les sommets subissent des transformations
        //----------------------------------------------------------------
        void Volume::maj_faces()
        {
            for (uint32_t i=0;i<nbrFaces;i++)
            {
                calcul_centre_gravite_face(faces[i]);
                calcul_normale_face(faces[i]);
                calcul_tangente_binormale_face(faces[i]);
            }
            calcul_normales_sommets();
            calcul_tangentes_binormales_sommets();
        }


        //---------------------------------------------------
        //      Calcule les normales des faces
        //---------------------------------------------------

        void Volume::calcul_normale_face(Face* face)
        {
            GLfloat* nml=face->normale;

            uint32_t at0 = face->aretes[0] * 3;
            uint32_t at1 = face->aretes[1] * 3;
            uint32_t at2 = face->aretes[2] * 3;

            GLfloat v1x = sommets[at1] - sommets[at0];
            GLfloat v1y = sommets[at1 + 1] - sommets[at0 + 1];
            GLfloat v1z = sommets[at1 + 2] - sommets[at0 + 2];

            GLfloat v2x = sommets[at2] - sommets[at1];
            GLfloat v2y = sommets[at2 + 1] - sommets[at1 + 1];
            GLfloat v2z = sommets[at2 + 2] - sommets[at1 + 2];

            if (sens_polygones==GL_CCW)
            {
                nml[0]=v1y*v2z-v1z*v2y;
                nml[1]=v1z*v2x-v1x*v2z;
                nml[2]=v1x*v2y-v1y*v2x;
            }
            else
            {
                nml[0]=-(v1y*v2z-v1z*v2y);
                nml[1]=-(v1z*v2x-v1x*v2z);
                nml[2]=-(v1x*v2y-v1y*v2x);
            }

            GLfloat norme_nml=sqrtf(nml[0]*nml[0]+nml[1]*nml[1]+nml[2]*nml[2]);
            nml[0]/=norme_nml;
            nml[1]/=norme_nml;
            nml[2]/=norme_nml;
        }

        //--------------------------------------------------------
        //  Versions optimisées, pour les calculs lors des rendus:
        //--------------------------------------------------------
        void Volume::calcul_normales_faces_CCW(uint32_t indice_depart, uint32_t nbr)
        {
            uint32_t i,at0,at1,at2;
            GLfloat v1x,v1y,v1z;
            GLfloat v2x,v2y,v2z;
            GLfloat nx,ny,nz;
            GLfloat norme_nml;
            if(nbr==0 && indice_depart==0)nbr=nbrFaces;
            else nbr+=indice_depart;

            for (i=indice_depart;i<nbr;i++)
            {
                Face* face=faces[i];
                GLfloat* nml=face->normale;

                at0 = face->aretes[0] * 3;
                at1 = face->aretes[1] * 3;
                at2 = face->aretes[2] * 3;

                v1x = sommets[at1] - sommets[at0];
                v1y = sommets[at1 + 1] - sommets[at0 + 1];
                v1z = sommets[at1 + 2] - sommets[at0 + 2];

                v2x = sommets[at2] - sommets[at1];
                v2y = sommets[at2 + 1] - sommets[at1 + 1];
                v2z = sommets[at2 + 2] - sommets[at1 + 2];

                nx=v1y*v2z-v1z*v2y;
                ny=v1z*v2x-v1x*v2z;
                nz=v1x*v2y-v1y*v2x;

                norme_nml=sqrtf(nx*nx+ny*ny+nz*nz);
                nml[0]=nx/norme_nml;
                nml[1]=ny/norme_nml;
                nml[2]=nz/norme_nml;
            }

        }

        void Volume::calcul_normales_faces_CW(uint32_t indice_depart, uint32_t nbr)
        {
            uint32_t i,at0,at1,at2;
            GLfloat v1x,v1y,v1z;
            GLfloat v2x,v2y,v2z;

            if(nbr==0 && indice_depart==0)nbr=nbrFaces;
            else nbr+=indice_depart;

            for (i=indice_depart;i<nbr;i++)
            {
                Face* face=faces[i];
                GLfloat* nml=face->normale;

                at0 = face->aretes[0] * 3;
                at1 = face->aretes[1] * 3;
                at2 = face->aretes[2] * 3;

                v1x = sommets[at1] - sommets[at0];
                v1y = sommets[at1 + 1] - sommets[at0 + 1];
                v1z = sommets[at1 + 2] - sommets[at0 + 2];

                v2x = sommets[at2] - sommets[at1];
                v2y = sommets[at2 + 1] - sommets[at1 + 1];
                v2z = sommets[at2 + 2] - sommets[at1 + 2];

                nml[0]=-(v1y*v2z-v1z*v2y);
                nml[1]=-(v1z*v2x-v1x*v2z);
                nml[2]=-(v1x*v2y-v1y*v2x);
            }

        }


        //------------------------------------------------------------------------
        // Calcul les normales relatives des sommets des faces
        // Ces normales ne servent que si l'adoucissement de la surface
        // à laquelle est liée la face est activé.
        /// Cette fonction est appelée par l'initialisateur des surfaces,
        /// car les normales des sommets des faces sont calculées
        /// en fonction de l'angle limite d'adoucissement des surfaces.
        //------------------------------------------------------------------------
        void Volume::calcule_normales_sommets_faces()
        {
            uint32_t i,j,k,indice_sommet;
            float cos_angle,angle;
            liaison_faces* lien;
            GLfloat* nrm;
            float norme;

            for (i=0;i<nbrFaces;i++)
            {
                Face* face=faces[i];
                Surface* surface=face->surface;
                //if (!surface->drapeau_adoucissement) continue;
                for (j=0;j< face->nbrSommets;j++)
                {
                    indice_sommet=face->aretes[j];
                    lien=&liaisons_sommets_faces[indice_sommet];
                    nrm=&face->normales_sommets[j*3];
                    nrm[0]=face->normale[0];
                    nrm[1]=face->normale[1];
                    nrm[2]=face->normale[2];

                    for (k=0;k<lien->indices_faces.size();k++)
                    {
                        Face* face_B=faces[lien->indices_faces[k]];
                        if (face_B != face)
                        {
                            //Calcul l'angle avec une face adjacente:
                            cos_angle=
                                        face->normale[0]*face_B->normale[0]+
                                        face->normale[1]*face_B->normale[1]+
                                        face->normale[2]*face_B->normale[2];
                            angle=acosf(cos_angle);


                            //cout<<"Angle entre la face "<<face->indice<<" et la face "<<face_B->indice<<" : "<<angle<<" / "<<face->surface->angle_limite_adoucissement<<endl;

                            //Si l'angle limite n'est pas atteint, calcul la normale des sommets de l'arête:
                            if (surface->angle_limite_adoucissement > angle)
                            {
                                nrm[0]+=face_B->normale[0];
                                nrm[1]+=face_B->normale[1];
                                nrm[2]+=face_B->normale[2];
                            }
                            else face->drapeau_partiellement_adoucie=true;
                        }
                    }
                    norme=sqrt(nrm[0]*nrm[0]+nrm[1]*nrm[1]+nrm[2]*nrm[2]);
                    nrm[0]/=norme;
                    nrm[1]/=norme;
                    nrm[2]/=norme;

                }

                if (face->drapeau_partiellement_adoucie)
                {
                    /*
                    if (VOLUME_DEBUG && VOLUME_DEBUG_SURFACES)
                    {
                        cout<<"                Surface: "<<surface->nom<<" - Face "<<face->indice<<" partiellement adoucie"<<endl;
                    }
                    */
                    surface->ajoute_face_partiellement_adoucie(face);
                }
                else surface->ajoute_face_adoucie(face);
            }
        }

        //---------------------------------------------------
        //      Calcule les tangentes et binormales de la face
        //---------------------------------------------------

        void Volume::calcul_tangente_binormale_face(Face* face)
        {
            //Calcul de la tangente:
            // ->  ->  ->
            // Y ^ N = T
            GLfloat* tgt=face->tangente;
            GLfloat* bnm=face->binormale;
            tgt[1]=0.;
            GLfloat* nml=face->normale;
            if (nml[1] == 1.)
            {
                tgt[0]=1.;
                tgt[2]=0.;
            }
            else if (nml[1] == -1.)
            {
                tgt[0]=-1.;
                tgt[2]=0.;
            }
            else
            {
                tgt[0]=nml[2];  //Simplification d'un produit vectoriel entre la normale et l'axe vertical (0,1,0)
                tgt[2]=-nml[0];
                GLfloat norme=sqrt(tgt[0]*tgt[0]+tgt[2]*tgt[2]);
                tgt[0]/=norme;
                tgt[2]/=norme;
            }

            bnm[0]=nml[1]*tgt[2]-nml[2]*tgt[1];
            bnm[1]=nml[2]*tgt[0]-nml[0]*tgt[2];
            bnm[2]=nml[0]*tgt[1]-nml[1]*tgt[0];

        }

        //---------------------------------------------------------
        // Calcul le centre de gravité relatif d'une face
        //---------------------------------------------------------
        void Volume::calcul_centre_gravite_face(Face* face)
        {
            uint32_t indice;
            uint16_t j;
            float nbr,cx,cy,cz;

                cx=0.;
                cy=0.;
                cz=0.;


            for (j=0;j<face->nbrSommets;j++)
            {
                indice=face->aretes[j]*3;
                cx+=sommets[indice];
                cy+=sommets[indice+1];
                cz+=sommets[indice+2];
            }
            nbr=face->nbrSommets;
            face->centre_gravite[0]=cx/nbr;
            face->centre_gravite[1]=cy/nbr;
            face->centre_gravite[2]=cz/nbr;
        }

        //--------------------------------------------------------------------------------------
        //             Création de la table de correspondance entre les sommets et les faces
        //--------------------------------------------------------------------------------------
        void Volume::initialise_liaisons_sommets_faces()
        {
            uint32_t i,j,k;
            if(nbrSommets==0 || nbrFaces==0)
            {
                liaisons_sommets_faces.clear();
            }
            else
            {
                for (i=0;i<nbrSommets;i++)
                {
                    ajoute_liaison_sommet();
                }

                for (j=0;j<nbrFaces;j++)
                {
                    uint32_t* at=faces[j]->aretes;
                    for (k=0;k<faces[j]->nbrSommets;k++)
                    {
                            liaisons_sommets_faces[at[k]].indices_faces.push_back(j);
                    }
                }
            }
        }

        void Volume::ajoute_liaison_sommet()
        {
            liaison_faces liaison;
            liaisons_sommets_faces.push_back(liaison);
        }

        //=========================================================================================
        //                          Gestion des arêtes
        //=========================================================================================

        //--------------------------------------------------------------------------------------
        //              Création de la table des arêtes à partir des faces:
        //--------------------------------------------------------------------------------------

        bool Volume::creation_aretes()
		{
            uint32_t n,i,j,k,sommetA,sommetB;
            supprime_aretes();

			for (i = 0; i < nbrFaces; i++)
			{
			    n=faces[i]->nbrSommets;
				for (j = 0; j < n; j++ )
				{
					sommetA = faces[i]->aretes[j];
					sommetB = faces[i]->aretes[(j + 1) % n ];

					for (k = 0; k < nbrAretes*2; )
					{
						if (((aretes[k] == sommetA) && (aretes[k + 1] == sommetB)) || ((aretes[k] == sommetB) && (aretes[k + 1] == sommetA)))
						{break; }
						k += 2;
					}
					if (k == nbrAretes*2)
					{
						aretes.push_back(sommetA);
						aretes.push_back(sommetB);
						nbrAretes++;
					}
				}
			}
			if (VOLUME_DEBUG) cout<<"-----------------------Nombre d'arêtes:"<<nom<<" - "<<nbrAretes<<endl;

			return true;
		}



        //=============================================================================================================================
        //                      Initialisation des surfaces
        //  Initialise les pointeur de surface des faces, à partir du nom des surfaces spécifiés dans les faces,
        //  les table de faces des surfaces.
        ///  Les faces doivent avoir été déterminées en amont.
        //=============================================================================================================================
        bool Volume::initialise_surfaces()
        {
            uint32_t i;

            Surface* surface;

            //Initialise les paramètres des surfaces dépendants du volume:
            for (i=0;i<nbrSurfaces;i++)
            {
                surface=surfaces[i];
                surface->volume=this;
                if (surface->transparence>0.)
                {
                    surface->drapeau_transparence=true;
                    surfaces_transparentes.push_back(surface);
                    nbrSurfaces_transparentes++;
                }
                if(!surface->alloue_tableau_normales_sommets(nbrSommets))
                {
                    cout<<"ERREUR dans Volume::initialise_surfaces() - Mémoire insuffisante pour allouer le tableau des sommets"<<endl;
                    erreur=VOLUME_ERREUR_MEMOIRE;
                    return false;
                }
            }
            if (!initialise_liens_faces_surfaces()) return false;
            initialise_indices_sommets_surfaces();
            calcule_normales_sommets_faces();
            calcule_normales_sommets_surfaces();

            return true;
        }

        //---------- Initialise les pointeurs de surfaces des faces.
        bool Volume::initialise_liens_faces_surfaces()
        {
            uint32_t i;
            Surface* surface;
            Face* face;

            //Lie les faces aux surfaces:
            for (i=0;i<nbrFaces;i++)
            {
                face=faces[i];
                surface=renvoie_surface_nom(face->nom_surface);
                if (surface==NULL)
                {
                    cout<<"Erreur dans Volume.initialise_surfaces() - Volume: "<<nom<<" - La face "<<i<<" est liée à une surface inconnue: "<<face->nom_surface<<endl;
                    if(VOLUME_DEBUG)
                    {
                        cout<<"Nombre de faces:"<<nbrFaces<<endl;
                        cout<<"Surfaces présentes dans le volume:"<<endl;
                        for (i=0;i<nbrSurfaces;i++) cout<<surfaces[i]->nom<<endl;
                    }
                    erreur=VOLUME_ERREUR_LIEN_FACE_SURFACE;
                    return false;
                }
                face->surface=surface;
                surface->faces.push_back(face);
                surface->nbr_faces=surface->faces.size();
            }

            //Affiche les tables de faces des surfaces:
            #if (VOLUME_DEBUG && VOLUME_DEBUG_SURFACES)

                for (i=0;i<nbrSurfaces;i++)
                {
                    surface=surfaces[i];
                    cout<<"Surface: "<<surface->nom<<endl;
                    for (j=0;j<surface->faces.size();j++)
                    {
                        cout<<"     Face: "<<surface->faces[j]->indice<<endl;
                    }
                }
            #endif

            return true;
        }

        //-------Création des tables d'indices de sommets des surfaces:
        void Volume::initialise_indices_sommets_surfaces()
        {
            uint32_t i,j;
            liaison_faces* lien;
            Surface* surface;
            for (i=0;i<nbrSommets;i++)
            {
                lien=&liaisons_sommets_faces[i];
                for(j=0;j<lien->indices_faces.size();j++)
                {
                    surface=faces[lien->indices_faces[j]]->surface;
                    if (surface->indices_sommets.size()==0 || surface->indices_sommets[surface->indices_sommets.size()-1]!=i) surface->indices_sommets.push_back(i);
                }
            }
        }


        //-----------------------------------------------
        //  Calcul les normales des sommets des surfaces.
        //  Ces normales servent pour le rendu par l'utilisation des
        //  tableaux de pointeurs.
        //  En effet, dans ce mode, les angles-seuils d'adoucissement
        //  ne fonctionnent qu'aux sommets limitrophes des surfaces. Un seuil par
        //  face serait trop couteux en initialisations et rendrait les
        //  tableaux inutiles.
        //-----------------------------------------------
        void Volume::calcule_normales_sommets_surfaces()
        {
            uint32_t i,j,k,l;
            liaison_faces* lien;
            Face* face;

            if(nbrSommets==0 || nbrSurfaces==0) return; //Pour éviter l'initialisation d'un tableau vide (ligne suivante)

            bool sommets_limitrophes[nbrSommets];

            for(i=0;i<nbrSurfaces;i++)
            {
                Surface* surface=surfaces[i];
                //Copiage des normales des sommets du volume
                for (j=0;j<nbrSommets;j++)
                {
                    surface->normales_sommets[3*j]=normales_sommets[3*j];
                    surface->normales_sommets[3*j+1]=normales_sommets[3*j+1];
                    surface->normales_sommets[3*j+2]=normales_sommets[3*j+2];
                    sommets_limitrophes[j]=false;
                }

                //Recherche les sommets limitrophes

                for(j=0;j<surface->faces_partiellement_adoucies.size();j++)
                {
                    face=surface->faces_partiellement_adoucies[j];
                    for(k=0;k<face->nbrSommets;k++)
                    {
                        lien=&liaisons_sommets_faces[face->aretes[k]];

                        for(l=0;l<lien->indices_faces.size();l++)
                        {
                            Face* face_b=faces[lien->indices_faces[l]];
                            if (face!=face_b && face_b->surface!=surface)
                            {
                                sommets_limitrophes[face->aretes[k]]=true;
                                break;
                            }
                        }
                    }
                }

                //-------Calcul les normales aux sommets limitrophes:
                vector<Face*> faces_exterieures;
                float nrm[3],nrm_ext[3];
                float cptr,cos_angle,angle;


                for (j=0;j<nbrSommets;j++)
                {
                    if (sommets_limitrophes[j])
                    {
                        //D'abord on calcul la normale moyenne des faces DE LA SURFACE adjentes au sommet
                        lien=&liaisons_sommets_faces[j];
                        nrm[0]=0.;nrm[1]=0.;nrm[2]=0.;
                        faces_exterieures.clear();
                        cptr=0.;
                        for (k=0;k<lien->indices_faces.size();k++)
                        {
                            face=faces[lien->indices_faces[k]];
                            //Si la face est dans la surface, on l'ajoute pour la moyenne des normales, sans tenir compte du seuil d'adoucissement
                            if(face->surface==surface)
                            {
                                nrm[0]+=face->normale[0];
                                nrm[1]+=face->normale[1];
                                nrm[2]+=face->normale[2];
                                cptr++;
                            }
                            //Si la face n'est pas dans la surface, on la met de côté, pour vérifier son seuil
                            //avec la normale moyenne des faces incluses dans la surface à ce sommet.
                            else
                            {
                                faces_exterieures.push_back(face);
                            }
                        }
                        //Normale des faces incluses dans la surface à ce sommet:
                        nrm[0]/=cptr;
                        nrm[1]/=cptr;
                        nrm[2]/=cptr;
                        //Test le seuil d'adoucissement avec les faces adjacentes au sommet, extérieures à la surface:
                        cptr=1.;
                        nrm_ext[0]=nrm[0];nrm_ext[1]=nrm[1];nrm_ext[2]=nrm[2]; //Pour que le seuil d'adoucissement soit toujours comparé à la normale de la surface.
                        for(k=0;k<faces_exterieures.size();k++)
                        {
                            face=faces_exterieures[k];
                            //Calcul l'angle avec la normale moyenne:
                            cos_angle=
                                        face->normale[0]*nrm[0]+
                                        face->normale[1]*nrm[1]+
                                        face->normale[2]*nrm[2];
                            angle=acosf(cos_angle);

                            //Si l'angle limite n'est pas atteint, calcul la normale des sommets de l'arête:
                            if (surface->angle_limite_adoucissement > angle)
                            {
                                nrm_ext[0]+=face->normale[0];
                                nrm_ext[1]+=face->normale[1];
                                nrm_ext[2]+=face->normale[2];
                                cptr++;
                            }
                        }
                        nrm_ext[0]/=cptr;
                        nrm_ext[1]/=cptr;
                        nrm_ext[2]/=cptr;
                        //Stockage de la normale:
                        surface->normales_sommets[j*3]=nrm_ext[0];
                        surface->normales_sommets[j*3+1]=nrm_ext[1];
                        surface->normales_sommets[j*3+2]=nrm_ext[2];
                    }
                }
            }
        }

        //-----------------------------------------------------
        //      Initialise les modes de rendu des surfaces
        //-----------------------------------------------------
        void Volume::initialise_modes_rendus_surfaces()
        {
            for (uint32_t i=0 ;i<nbrSurfaces;i++) surfaces[i]->initialise_mode_rendu();
        }

        //=============================================================================================================================
        //                      Initialisation des textures
        //          Les liens entre les faces et les sommets ont été créés.
        //          Allocation et initialisation des tableaux de coordonnées UV, si il y en a.
        //=============================================================================================================================
        bool Volume::initialise_textures()
        {
            uint32_t i,j;
            Surface* surface;
            Texture* texture;
            vector<uint8_t> faces_utilisees; //Sert à mémoriser le status des faces incluses,adjacentes ou hors de la texture, pour les calculs de repères TBN

            if (VOLUME_DEBUG && VOLUME_DEBUG_TEXTURES) cout<<endl<<"**** Initialisation des textures du volume: "<<nom<<endl<<endl;

            for (i=0;i<nbrSurfaces;i++)
            {
                surface=surfaces[i];
                if (VOLUME_DEBUG && VOLUME_DEBUG_TEXTURES) cout<<" "<<surface->nom<<" ";

                //Ambiance
                for (j=0;j<surface->textures_ambiance.size();j++)
                {
                    texture=surface->textures_ambiance[j];
                    if (!creation_tableau_UV(texture)) return false;
                }
                for (j=0;j<surface->textures_ambiance_inactives.size();j++)
                {
                    texture=surface->textures_ambiance_inactives[j];
                    if (!creation_tableau_UV(texture)) return false;
                }

                //Couleur
                for (j=0;j<surface->textures_couleur.size();j++)
                {
                    texture=surface->textures_couleur[j];
                    if (!creation_tableau_UV(texture)) return false;
                }
                for (j=0;j<surface->textures_couleur_inactives.size();j++)
                {
                    texture=surface->textures_couleur_inactives[j];
                    if (!creation_tableau_UV(texture)) return false;
                }

                //Diffusion
                for (j=0;j<surface->textures_diffusion.size();j++)
                {
                    texture=surface->textures_diffusion[j];
                    if (!creation_tableau_UV(texture)) return false;
                }
                for (j=0;j<surface->textures_diffusion_inactives.size();j++)
                {
                    texture=surface->textures_diffusion_inactives[j];
                    if (!creation_tableau_UV(texture)) return false;
                }

                //Luminescence
                for (j=0;j<surface->textures_luminescence.size();j++)
                {
                    texture=surface->textures_luminescence[j];
                    if (!creation_tableau_UV(texture)) return false;
                }
                for (j=0;j<surface->textures_luminescence_inactives.size();j++)
                {
                    texture=surface->textures_luminescence_inactives[j];
                    if (!creation_tableau_UV(texture)) return false;
                }

                //Réflexion
                for (j=0;j<surface->textures_reflexion.size();j++)
                {
                    texture=surface->textures_reflexion[j];
                    if (!creation_tableau_UV(texture)) return false;
                }
                for (j=0;j<surface->textures_reflexion_inactives.size();j++)
                {
                    texture=surface->textures_reflexion_inactives[j];
                    if (!creation_tableau_UV(texture)) return false;
                }

                //Rugosite
                for (j=0;j<surface->textures_rugosite.size();j++)
                {
                    texture=surface->textures_rugosite[j];
                    if (!creation_table_faces_texture(texture,faces_utilisees)) return false;
                    if (!creation_tableau_UV(texture)) return false;
                    if (!creation_tangentes_binormales(texture,faces_utilisees)) return false;
                }
                for (j=0;j<surface->textures_rugosite_inactives.size();j++)
                {
                    texture=surface->textures_rugosite_inactives[j];
                    if (!creation_table_faces_texture(texture,faces_utilisees)) return false;
                    if (!creation_tableau_UV(texture)) return false;
                    if (!creation_tangentes_binormales(texture,faces_utilisees)) return false;
                }

                //Transparence:
                for (j=0;j<surface->textures_transparence.size();j++)
                {
                    texture=surface->textures_transparence[j];
                    if (!creation_tableau_UV(texture)) return false;
                }
                for (j=0;j<surface->textures_transparence_inactives.size();j++)
                {
                    texture=surface->textures_transparence_inactives[j];
                    if (!creation_tableau_UV(texture)) return false;
                }

                #if (VOLUME_DEBUG && VOLUME_DEBUG_TEXTURES)

                    cout<<" *** Surface:"<<surface->nom<<endl;
                    cout<<"            Nombre de textures:"<<endl;
                    if (surface->textures_ambiance.size()>0)    cout<<"                  Ambiance:"<<surface->textures_ambiance.size()<<endl;
                    if (surface->textures_ambiance_inactives.size()>0)    cout<<"                  -inactives:"<<surface->textures_ambiance.size()<<endl;
                    if (surface->textures_couleur.size()>0)     cout<<"                   Couleur:"<<surface->textures_couleur.size()<<endl;
                    if (surface->textures_couleur_inactives.size()>0)     cout<<"                   -inactives:"<<surface->textures_couleur.size()<<endl;
                    if (surface->textures_diffusion.size()>0)   cout<<"                 Diffusion:"<<surface->textures_diffusion.size()<<endl;
                    if (surface->textures_diffusion_inactives.size()>0)   cout<<"                 -inactives:"<<surface->textures_diffusion.size()<<endl;
                    if (surface->textures_luminescence.size()>0) cout<<"              Luminescence:"<<surface->textures_luminescence.size()<<endl;
                    if (surface->textures_luminescence_inactives.size()>0) cout<<"              -inactives:"<<surface->textures_luminescence.size()<<endl;
                    if (surface->textures_reflexion.size()>0)   cout<<"                 Réflexion:"<<surface->textures_reflexion.size()<<endl;
                    if (surface->textures_reflexion_inactives.size()>0)   cout<<"                 -inactives:"<<surface->textures_reflexion.size()<<endl;
                    if (surface->textures_rugosite.size()>0)    cout<<"                  Rugosité:"<<surface->textures_rugosite.size()<<endl;
                    if (surface->textures_rugosite_inactives.size()>0)    cout<<"                  -inactives:"<<surface->textures_rugosite.size()<<endl;
                    if (surface->textures_transparence.size()>0)cout<<"              Transparence:"<<surface->textures_transparence.size()<<endl;
                    if (surface->textures_transparence_inactives.size()>0)cout<<"              -inactives:"<<surface->textures_transparence.size()<<endl;
                #endif
            }

            return true;
        }

        //------------------- Création de la table des faces affectées par la texture
        ///Pour le moment, cette table n'est créée que pour les textures de rugosité

        bool Volume::creation_table_faces_texture(Texture* texture, vector<uint8_t> &faces_utilisees)
        {
            uint32_t i,j,k,l,indice_sommet;

            faces_utilisees.clear();
            //Libère la mémoire de l'ancienne table:
            if (texture->faces!=NULL) free(texture->faces);
            texture->nbrFaces=0;
            //Comptage des faces:
            for(i=0;i<nbrFaces;i++) faces_utilisees.push_back(0);

            for (i=0;i<texture->coordonnees_UV.size();i++)
            {
                indice_sommet=texture->coordonnees_UV[i].indice_sommet;
                liaison_faces* lien=&liaisons_sommets_faces[indice_sommet];
                for(j=0;j<lien->indices_faces.size();j++)
                {

                    if(faces_utilisees[lien->indices_faces[j]]==0)
                    {
                        //détermination des faces frontalières adjacentes (c'est à dire celles dont au moins un sommet n'a pas de coordonnées UV):
                        Face* face=faces[lien->indices_faces[j]];
                        for(k=0;k<face->nbrSommets;k++)
                        {
                            for (l=0;l<texture->coordonnees_UV.size();l++)
                            {
                                if (face->aretes[k]==texture->coordonnees_UV[l].indice_sommet) break;
                            }
                            if(l==texture->coordonnees_UV.size())
                            {
                                faces_utilisees[lien->indices_faces[j]]=VOLUME_FACE_FRONTALIERE; //Sont considérées comme frontalière TOUTES les faces ayant AU MOINS 1 sommet
                                                                                                    //non-inclus dans la table UV.
                                break;
                            }
                        }
                        if (k==face->nbrSommets) faces_utilisees[lien->indices_faces[j]]=VOLUME_FACE_INCLUSE;
                    }
                }
            }

            for(i=0;i<nbrFaces;i++) if (faces_utilisees[i]==VOLUME_FACE_INCLUSE) texture->nbrFaces++;
            // Allocation de la table:
            texture->faces=(uint32_t*)malloc(texture->nbrFaces*sizeof(uint32_t));
            if (texture->faces==NULL)
            {
                cout<<"ERREUR dans Volume.creation_table_faces_texture() - Mémoire insuffisante"<<endl;
                erreur=VOLUME_ERREUR_MEMOIRE;
                return false;
            }
            //Stockage des indexes des faces recouvertes par la texture:
            j=0;
            for(i=0;i<nbrFaces;i++) if (faces_utilisees[i]==VOLUME_FACE_INCLUSE) texture->faces[j++]=i;

            return true;
        }

        //---------------- Crée une table de coordonnées correspondantes à la texture
        bool Volume::creation_tableau_UV(Texture* texture)
        {
            uint32_t i,indice;
            GLfloat* tableau_UV;

            if (texture->type_projection==TEXTURE_PROJECTION_UV)
            {
                //Libère l'ancien tableau, le cas échéant:
                if (texture->coordonnees_UV_tableau!=NULL) free(texture->coordonnees_UV_tableau);

                //Alloue le nouveau tableau:
                texture->coordonnees_UV_tableau=(GLfloat*)malloc(nbrSommets*sizeof(GLfloat)*2);
                if (texture->coordonnees_UV_tableau==NULL)
                {
                    cout<<"Erreur dans Volume::allocation_tableau_UV() - Mémoire insuffisante"<<endl;
                    erreur=VOLUME_ERREUR_MEMOIRE;
                    return false;
                }
                //Initialise le tableau:
                tableau_UV=texture->coordonnees_UV_tableau;

                for (i=0;i<nbrSommets;i++)
                {
                    tableau_UV[2*i]=0.;
                    tableau_UV[2*i+1]=0.;
                }

                for (i=0;i<texture->nbrSommets;i++)
                {
                    indice=texture->coordonnees_UV[i].indice_sommet;
                    if (VOLUME_DEBUG && VOLUME_DEBUG_TEXTURES) cout<<indice<<endl;
                    tableau_UV[indice*2]=texture->coordonnees_UV[i].U;
                    tableau_UV[indice*2+1]=texture->coordonnees_UV[i].V;
                }

                return true;
            }
            return true;
        }


        //-------------- Calcul les tangentes et les binormales des faces et des sommets
        /// Les textures sont considérées comme n'étant pas ou peu déformées sur la face !!!

        //Seules les textures UV sont concernées.
        bool Volume::creation_tangentes_binormales(Texture* texture,vector<uint8_t>& faces_utilisees)
        {
            uint32_t i,j,indice_sommet,indice_face;

            Face* face;
            Vecteur tempon_tangente;
            liaison_faces* lien;

            if (!texture->type_projection==TEXTURE_PROJECTION_UV)
            {
                cout<<"ERREUR dans Volume::creation_tengantes_binormales() - La texture n'est pas en projection UV!"<<endl;
                return false;
            }

            //Libère les anciens tableaux, le cas échéant:
            if (texture->tangentes_faces!=NULL) free(texture->tangentes_faces);
            if (texture->binormales_faces!=NULL) free(texture->binormales_faces);
            if (texture->tangentes_sommets!=NULL) free(texture->tangentes_sommets);
            if (texture->binormales_sommets!=NULL) free(texture->binormales_sommets);

            //Alloue les nouveaux tableaux:
            texture->tangentes_faces=(GLfloat*)malloc(nbrFaces*sizeof(GLfloat)*3);
            texture->binormales_faces=(GLfloat*)malloc(nbrFaces*sizeof(GLfloat)*3);
            texture->tangentes_sommets=(GLfloat*)malloc(nbrSommets*sizeof(GLfloat)*3);
            texture->binormales_sommets=(GLfloat*)malloc(nbrSommets*sizeof(GLfloat)*3);

            if (texture->tangentes_faces==NULL || texture->binormales_faces==NULL || texture->tangentes_sommets==NULL || texture->binormales_sommets==NULL)
            {
                cout<<"ERREUR dans Volume::creation_tengantes_binormales() - Mémoire insuffisante"<<endl;
                erreur=VOLUME_ERREUR_MEMOIRE;
                return false;
            }

            //Initialise les tableaux:
            //Les repères TBN par défaut du volume sont utilisés:
            //  C'est plus propre dans le cas ou la texture ne couvre pas toute la surface


            for (i=0;i<nbrFaces;i++)
            {
               texture->tangentes_faces[3*i]=faces[i]->tangente[0];
               texture->tangentes_faces[3*i+1]=faces[i]->tangente[1];
               texture->tangentes_faces[3*i+2]=faces[i]->tangente[2];
               texture->binormales_faces[3*i]=faces[i]->binormale[0];
               texture->binormales_faces[3*i+1]=faces[i]->binormale[1];
               texture->binormales_faces[3*i+2]=faces[i]->binormale[2];
            }
            for (i=0;i<nbrSommets;i++)
            {
               texture->tangentes_sommets[3*i]=tangentes_sommets[3*i];
               texture->tangentes_sommets[3*i+1]=tangentes_sommets[3*i+1];
               texture->tangentes_sommets[3*i+2]=tangentes_sommets[3*i+2];
               texture->binormales_sommets[3*i]=binormales_sommets[3*i];
               texture->binormales_sommets[3*i+1]=binormales_sommets[3*i+1];
               texture->binormales_sommets[3*i+2]=binormales_sommets[3*i+2];
            }


            //Calcul des repères TBN des faces, alignés à la texture:
            for (i=0 ; i<texture->nbrFaces ;i++)
            {
                indice_face=texture->faces[i];
                face=faces[indice_face];
                //Vecteur UV de la première arête de la face:
                GLfloat Au=texture->coordonnees_UV_tableau[face->aretes[0]*2];
                GLfloat Av=texture->coordonnees_UV_tableau[face->aretes[0]*2+1];
                GLfloat Bu=texture->coordonnees_UV_tableau[face->aretes[1]*2];
                GLfloat Bv=texture->coordonnees_UV_tableau[face->aretes[1]*2+1];
                GLfloat Cu=Bu-Au;
                GLfloat Cv=Bv-Av;
                //Normalisation:
                GLfloat norme=sqrt(Cu*Cu+Cv*Cv);
                Cu/=norme;
                Cv/=norme;
                //Angle d'inclinaison de l'arête par rapport à l'axe des U:
                GLfloat angle=acos(Cu);
                if (Cv<0.)angle=-angle;
                //Vecteur spatial de l'arête:
                GLfloat Apx=sommets[face->aretes[0]*3];
                GLfloat Apy=sommets[face->aretes[0]*3+1];
                GLfloat Apz=sommets[face->aretes[0]*3+2];
                GLfloat Bpx=sommets[face->aretes[1]*3];
                GLfloat Bpy=sommets[face->aretes[1]*3+1];
                GLfloat Bpz=sommets[face->aretes[1]*3+2];
                GLfloat Cpx=Bpx-Apx;
                GLfloat Cpy=Bpy-Apy;
                GLfloat Cpz=Bpz-Apz;
                //Normalisation:
                norme=sqrtf(Cpx*Cpx+Cpy*Cpy+Cpz*Cpz);
                Cpx/=norme;
                Cpy/=norme;
                Cpz/=norme;
                //Rotation:
                tempon_tangente.x=Cpx;
                tempon_tangente.y=Cpy;
                tempon_tangente.z=Cpz;
                tempon_tangente.rotation(-angle,face->normale[0],face->normale[1],face->normale[2]);
                //Calcul de la binormale:
                texture->tangentes_faces[indice_face*3]=tempon_tangente.x;
                texture->tangentes_faces[indice_face*3+1]=tempon_tangente.y;
                texture->tangentes_faces[indice_face*3+2]=tempon_tangente.z;
                texture->binormales_faces[indice_face*3]=face->normale[1]*tempon_tangente.z - face->normale[2]*tempon_tangente.y;
                texture->binormales_faces[indice_face*3+1]=face->normale[2]*tempon_tangente.x - face->normale[0]*tempon_tangente.z;
                texture->binormales_faces[indice_face*3+2]=face->normale[0]*tempon_tangente.y - face->normale[1]*tempon_tangente.x;

            }

            //Calcul des repères TBN des sommets, par moyenne des faces incluses à la texture:
            ///Les seuils d'adoucissement des tangente et des binormales ne sont pas pris en compte pour les faces recouvertes par
            ///la texture. De plus les moyennes pour le calcul des composantes T,B sur les sommets ne tiennent pas compte des
            /// faces hors-texture. On obtient une tangente et une binormale approximative. Par contre la normale suit bien
            /// les adoucissements.
            for (i=0;i<texture->coordonnees_UV.size();i++)
            {
                indice_sommet=texture->coordonnees_UV[i].indice_sommet;
                GLfloat* tgt=&texture->tangentes_sommets[3*indice_sommet];
                GLfloat* bnm=&texture->binormales_sommets[3*indice_sommet];
                GLfloat* nml=&normales_sommets[3*indice_sommet];
                tgt[0]=0.;
                tgt[1]=0.;
                tgt[2]=0.;
                lien=&liaisons_sommets_faces[indice_sommet];
                GLfloat compteur_moyenne=0.;
                for(j=0;j<lien->indices_faces.size();j++)
                {
                    indice_face=lien->indices_faces[j];
                    if(faces_utilisees[indice_face]==VOLUME_FACE_INCLUSE)
                    {
                        tgt[0]+=texture->tangentes_faces[3*indice_face];
                        tgt[1]+=texture->tangentes_faces[3*indice_face+1];
                        tgt[2]+=texture->tangentes_faces[3*indice_face+2];
                        compteur_moyenne++;
                    }
                }
                tgt[0]/=compteur_moyenne;
                tgt[1]/=compteur_moyenne;
                tgt[2]/=compteur_moyenne;
                bnm[0]=nml[1]*tgt[2] - nml[2]*tgt[1];
                bnm[1]=nml[2]*tgt[0] - nml[0]*tgt[2];
                bnm[2]=nml[0]*tgt[1] - nml[1]*tgt[0];
            }
            return true;
        }

        //=============================================================================================================================
        //                      Gestion VBOs
        //=============================================================================================================================

        bool Volume::creation_VBOs()
        {
            for(uint32_t i=0;i<nbrSurfaces;i++)
            {
                Surface* surface=surfaces[i];
                if(!surface->creation_tableau_sommets_vbo(sommets))
                {
                    cout<<"Erreur dans Volume::creationVBOs(): impossible de créer le tableau des sommets de la surface: "<<surface->nom<<endl;
                    erreur=VOLUME_ERREUR_VBOS;
                    return false;
                }
                if(!surface->init_vbo())
                {
                    cout<<"Erreur dans Volume::creationVBOs(): impossible de créer le VBO de la surface: "<<surface->nom<<endl;
                    erreur=VOLUME_ERREUR_VBOS;
                    return false;
                }
            }
            return true;
        }

        bool Volume::maj_VBOs()
        {
            return creation_VBOs();
        }

        //=============================================================================================================================
        //                      Fonctions utiles
        //=============================================================================================================================

        //Renvoie une surface d'après son nom:
        //NULL si la surface n'existe pas.
        Surface* Volume::renvoie_surface_nom(char* nom_surface)
        {
            for (uint32_t i=0;i<nbrSurfaces;i++)
            {
                if(strcmp(surfaces[i]->nom,nom_surface)==0) return surfaces[i];
            }
            return NULL;
        }

        Surface* Volume::renvoie_surface(uint32_t indice)
        {
            if (indice>nbrSurfaces) return NULL;
            return surfaces[indice];
        }

        //Renvoie la table des sommets relatifs du volume:
        float* Volume::renvoie_sommets()
        {
            return sommets;
        }

        //Renvoie la Face correspondant à l'indice:
        Face* Volume::renvoie_face(uint32_t indice)
        {
            if (indice>=nbrFaces) return NULL;
            return faces[indice];
        }

        //Renvoie la normale du sommet:
        float* Volume::renvoie_normale_sommet(uint32_t indice)
        {
            if (indice>nbrSommets)return NULL;
            return &normales_sommets[indice*3];
        }


        //====================================================================================================================
        //                                              Gestion des couleurs
        //====================================================================================================================
        //Détermine les couleurs des surfaces
        void Volume::determine_couleur_ambiance(uint32_t couleur, float intensite)
        {
            for (uint32_t i=0;i<nbrSurfaces;i++)
            {
                Surface* surface=surfaces[i];
                surface->determine_couleur_ambiance(couleur,intensite);
            }
        }

        void Volume::determine_couleur_diffusion(uint32_t couleur, float intensite)
        {
            for (uint32_t i=0;i<nbrSurfaces;i++)
            {
                Surface* surface=surfaces[i];
                surface->determine_couleur_diffusion(couleur,intensite);
            }
        }

        void Volume::determine_couleur_specularite(uint32_t couleur, float intensite)
        {
            for (uint32_t i=0;i<nbrSurfaces;i++)
            {
                Surface* surface=surfaces[i];
                surface->determine_couleur_speculaire(couleur,intensite);
            }
        }

        void Volume::determine_couleur_luminescence(uint32_t couleur, float intensite)
        {
            for (uint32_t i=0;i<nbrSurfaces;i++)
            {
                Surface* surface=surfaces[i];
                surface->determine_couleur_luminescence(couleur,intensite);
            }
        }

        void Volume::determine_eclairage(bool drapeau)
        {
            for (uint32_t i=0;i<nbrSurfaces;i++)
            {
                Surface* surface=surfaces[i];
                surface->determine_eclairage(drapeau);
            }
            maj_VBOs();
        }

        void Volume::determine_eclairage_phong(bool drapeau)
        {
            for (uint32_t i=0;i<nbrSurfaces;i++)
            {
                Surface* surface=surfaces[i];
                surface->determine_phong(drapeau);
            }
            //maj_VBOs();
        }


        //Teinte les surfaces:
        void Volume::applique_teinte_diffusion(uint32_t teinte)
        {
            for(uint32_t i=0;i<nbrSurfaces;i++)
            {
                Surface* surface=surfaces[i];
                surface->applique_teinte_diffusion(teinte);
            }
        }

        void Volume::applique_teinte_specularite(uint32_t teinte)
        {
            for(uint32_t i=0;i<nbrSurfaces;i++)
            {
                Surface* surface=surfaces[i];
                surface->applique_teinte_specularite(teinte);
            }
        }

        void Volume::applique_teinte_luminescence(uint32_t teinte)
        {
            for(uint32_t i=0;i<nbrSurfaces;i++)
            {
                Surface* surface=surfaces[i];
                surface->applique_teinte_luminescence(teinte);
            }
        }

        void Volume::applique_teinte_ambiance(uint32_t teinte)
        {
            for(uint32_t i=0;i<nbrSurfaces;i++)
            {
                Surface* surface=surfaces[i];
                surface->applique_teinte_ambiance(teinte);
            }
        }


        //-----------------------------------------------------------------------------------
        //      Détermine si le volume est affecté par le brouillard
        //-----------------------------------------------------------------------------------
        void Volume::determine_affecte_par_brouillard(bool p_drapeau)
        {
            drapeau_affecte_par_brouillard=p_drapeau;
        }

        bool Volume::est_affecte_par_brouillard()
        {
            return drapeau_affecte_par_brouillard;
        }

        //------------------------------------------------------------------
        //  Détermine le sens dans lequel les polygones sont définis
        //  GL_CW: sens des aiguilles
        //  GL_CCW: sens inverse des aiguilles
        //-----------------------------------------------

        void Volume::determine_sens_polygones(GLint p_sens)
        {
            sens_polygones=p_sens;
        }

        GLint Volume::renvoie_sens_polygones()
        {
            return sens_polygones;
        }

        //-----------------------------------------------------------------------
        //      Inversion du sens des polygones.
        //  Les normales du volume sont également inversées
        ///  Les tangentes et binormales des repères TNB ne sont pas inversées.
        //-----------------------------------------------------------------------
        void Volume::inverse_sens_polygones()
        {
            uint32_t i;
            if (sens_polygones==GL_CCW) sens_polygones=GL_CW;
            else sens_polygones=GL_CCW;
            //Inverse les normales des sommets:
            for (i=0;i<nbrSommets;i++)
            {
                normales_sommets[3*i]=-normales_sommets[3*i];
                normales_sommets[3*i+1]=-normales_sommets[3*i+1];
                normales_sommets[3*i+2]=-normales_sommets[3*i+2];
            }
            //Inverse les normales des faces:
            for (i=0;i<nbrFaces;i++) { faces[i]->inverse_normales(); }
            //Inverse les normales des surfaces:
            for (i=0;i<nbrSurfaces;i++) { surfaces[i]->inverse_normales(); }
        }

        //------------------------------------------------------------------
        //  Détermine le mode de remplissage: couleurs,aretes, sommets ou surfaces
        //------------------------------------------------------------------
        void Volume::determine_mode_remplissage(uint8_t p_mode)
        {
            mode_remplissage=p_mode;
        }

        //****************************************************************************************************************
        //****************************************************************************************************************
        //                                              AFFICHAGE DU VOLUME
        //****************************************************************************************************************
        //****************************************************************************************************************

        //---------------------------------------------------------------
		//Initialise OpenGl pour le rendu d'un materiau:
		//---------------------------------------------------------------

        void Volume::initialise_materiau()
		{
            materiau_ambiance[0]=surface_courante->couleur_ambiance[0]*surface_courante->ambiance;
		    materiau_ambiance[1]=surface_courante->couleur_ambiance[1]*surface_courante->ambiance;
		    materiau_ambiance[2]=surface_courante->couleur_ambiance[2]*surface_courante->ambiance;
		    materiau_ambiance[3]=0.;

            materiau_diffusion[0]=surface_courante->couleur_diffusion[0]*surface_courante->diffusion;
            materiau_diffusion[1]=surface_courante->couleur_diffusion[1]*surface_courante->diffusion;
            materiau_diffusion[2]=surface_courante->couleur_diffusion[2]*surface_courante->diffusion;
            materiau_diffusion[3]=0.;          //Seul l'alpha de la couleur de diffusion est pris en compte

            materiau_specularite[0]=surface_courante->couleur_speculaire[0]*surface_courante->specularite;
            materiau_specularite[1]=surface_courante->couleur_speculaire[1]*surface_courante->specularite;
            materiau_specularite[2]=surface_courante->couleur_speculaire[2]*surface_courante->specularite;
            materiau_specularite[3]=surface_courante->reflexion;

            float f_lum=surface_courante->luminescence*facteur_luminescence;
            materiau_luminosite[0]=surface_courante->couleur_luminescence[0]*f_lum;
            materiau_luminosite[1]=surface_courante->couleur_luminescence[1]*f_lum;
            materiau_luminosite[2]=surface_courante->couleur_luminescence[2]*f_lum;
            materiau_luminosite[3]=1.-surface_courante->transparence;//-surface_courante->transparence;

            //cout<<"R:"<<ambiance_faces[0]<<" V:"<<ambiance_faces[1]<<" B:"<<ambiance_faces[2]<<endl;


            //Gestion de l'affichage double-face:
            //Attention, les normales ne sont pas recalculées selon l'orientation de la face par rapport à l'oeil.

            if(surface_courante->double_face)
            {
                glDisable(GL_CULL_FACE);
            }
            else
            {
                glEnable(GL_CULL_FACE); //Active la distinction face avant/face arrière des polygones pour le rendu.
            }
		}

        //---------------------------------------------------------------
		// Rendu d'un materiau luminescent. Sert pour le rendu des auras
		//---------------------------------------------------------------

        void Volume::initialise_materiau_luminescent(Surface* surface)
		{
            /*
		    surface_courante=surface;
		    float f_lum=surface->luminescence*facteur_luminescence;
		    if (f_lum>0.)
		    {
                couleur_luminescence[0]=surface->couleur_luminescence[0]*f_lum;
                couleur_luminescence[1]=surface->couleur_luminescence[1]*f_lum;
                couleur_luminescence[2]=surface->couleur_luminescence[2]*f_lum;
                couleur_luminescence[3]=1.-surface->transparence;
                //glColor3f(couleur_luminescence[0],couleur_luminescence[1],couleur_luminescence[2]);
                glColor3fv(couleur_luminescence);
		    }

		    //Surface opaque (sert juste à masquer les surfaces lumineuses)
		    else glColor4fv(noir);

            //Gestion de l'affichage double-face:
		    if(surface->double_face) glDisable(GL_CULL_FACE);
            else glEnable(GL_CULL_FACE); //Active la distinction face avant/face arrière des polygones pour le rendu.
                */
		}


        //============================================================================================================================================================
        //============================================================================================================================================================
        //                      Fonctions de rendu:
        //============================================================================================================================================================
        //============================================================================================================================================================


        void Volume::desactive_textures()
        {
            uint16_t i;
            GLint nbr_unites_textures_max;
            glGetIntegerv(GL_MAX_TEXTURE_UNITS,&nbr_unites_textures_max);
            for (i=0;i<nbr_unites_textures_max;i++)
                {
                    glActiveTexture(GL_TEXTURE0+i);
                    glDisable(GL_TEXTURE_2D);
                }
                glActiveTexture(GL_TEXTURE0);
		}

        //------------------------------------------------
        //              Affichage du volume
        //------------------------------------------------
        void Volume::affiche()
        {

            uint32_t i;
            GLboolean depthWriteMask_mem;
            if (erreur!=VOLUME_OK || (!drapeau_affiche)) return;

            //Sauvegarde des états d'openGl:
            //observateur_position=obsPos;
            glGetBooleanv(GL_DEPTH_WRITEMASK,&depthWriteMask_mem);

            //#include "debug_volume_affiche.cpp"
            glEnable(GL_DEPTH_TEST);

            //-------------------------- Affiche les sommets
            if (mode_remplissage==VOLUME_REMPLISSAGE_SOMMETS)
            {
                /*
                GLboolean lum_mem=glIsEnabled(GL_LIGHTING);
                glPointSize(taille_points_sommets);
                desactive_textures();
                glDisable(GL_LIGHTING);
                glColor3fv(couleur_sommets);
                for (i=0;i<nbrSommets;i++)
                {
                    glBegin(GL_POINTS);
                        glVertex3fv(&sommets[3*i]);
                    glEnd();
                }
                if (lum_mem)glEnable(GL_LIGHTING);
                    */
            }

            //-------------------------- Affiche les arètes
            else if (mode_remplissage==VOLUME_REMPLISSAGE_ARETES)
            {
                //desactive_textures();
                /*
                GLboolean lum_mem=glIsEnabled(GL_LIGHTING);
                glDisable(GL_LIGHTING);
                glColor3fv(couleur_aretes);
                for (i=0;i<nbrAretes;i++)
                {
                    glBegin(GL_LINES);
                        glVertex3fv(&sommets[aretes[i*2]*3]);
                        glVertex3fv(&sommets[aretes[i*2+1]*3]);
                    glEnd();
                }
                if (lum_mem)glEnable(GL_LIGHTING);
                    */
            }

            //-------------------------- Affiche les faces en aplats colorés:
            else if (mode_remplissage==VOLUME_REMPLISSAGE_COULEUR)
            {
                glFrontFace(sens_polygones);
                rendu_surfaces_aplats();
            }

            //------------------------- Affiche les surfaces:
            else if (mode_remplissage==VOLUME_REMPLISSAGE_SURFACES)
            {
                glFrontFace(sens_polygones);
                rendu_surfaces();
            }

            glUseProgram(0);
            glDepthMask(depthWriteMask_mem);
        }





        //-----------------------------------------------------------------------------
        //      Initialise les pointeurs sur les fonctions d'initialisation des rendus
        //-----------------------------------------------------------------------------
        void Volume::initialise_pointeurs_rendus()
        {
            rendus[SURFACE_RENDU_APLAT]=&Volume::rendu_aplats;
            rendus[SURFACE_RENDU_APLAT_GOURAUD]=&Volume::rendu_aplats_Gouraud;
            rendus[SURFACE_RENDU_APLAT_PHONG]=&Volume::rendu_aplats_Phong;
            rendus[SURFACE_RENDU_TEXTURE]=&Volume::rendu_texture;
            rendus[SURFACE_RENDU_TEXTURE_GOURAUD]=&Volume::rendu_texture_Gouraud;
            rendus[SURFACE_RENDU_TEXTURE_PHONG]=&Volume::rendu_texture_Phong;
        }

//----------------------------------------------------------------
//      Rendu des surfaces en aplats
//----------------------------------------------------------------
        void Volume::rendu_surfaces_aplats()
        {
            int i;
            for(i=0;i<nbrSurfaces;i++)
            {
                surface_courante=surfaces[i];
                if(!surface_courante->drapeau_transparence) rendu_aplats();
            }
            if(nbrSurfaces_transparentes>0)
            {
               glEnable(GL_BLEND);
               glDepthMask(GL_FALSE);
               for(i=0;i<nbrSurfaces_transparentes;i++)
                {
                    surface_courante=surfaces_transparentes[i];
                    rendu_aplats();
                }
                glDepthMask(GL_TRUE);
                glDisable(GL_BLEND);
            }
        }
//----------------------------------------------------------------
//      Fonctions de rendu des surfaces
//----------------------------------------------------------------
        void Volume::rendu_surfaces()
        {
            int i;
            for(i=0;i<nbrSurfaces;i++)
            {
                surface_courante=surfaces[i];
                if(!surface_courante->drapeau_transparence) (this->*rendus[surface_courante->mode_rendu])();
            }
            if(nbrSurfaces_transparentes>0)
            {
               glEnable(GL_BLEND);
               glDepthMask(GL_FALSE);
               for(i=0;i<nbrSurfaces_transparentes;i++)
                {
                    surface_courante=surfaces_transparentes[i];
                    (this->*rendus[surface_courante->mode_rendu])();
                }
                glDepthMask(GL_TRUE);
                glDisable(GL_BLEND);
            }
        }

        void Volume::rendu_surfaces_luminescentes()
        {

        }


        //---------------------------------------------------------
        //              Affichage des surfaces lumineuses
        //              Sert pour le rendu des auras
        //---------------------------------------------------------
        void Volume::affiche_couleurs_luminescentes()
        {
            /*
            if (erreur!=VOLUME_OK || (!drapeau_affiche)) return;
            GLboolean eclairage_mem=glIsEnabled(GL_LIGHTING);
            GLboolean brouillard_mem=glIsEnabled(GL_FOG);
            if (!drapeau_affecte_par_brouillard) glDisable(GL_FOG);
            glEnable(GL_DEPTH_TEST);
            glDisable(GL_LIGHTING);
            glShadeModel(GL_FLAT);
            desactive_textures();
            glUseProgram(0);
            glFrontFace(sens_polygones); //Détermine quelle est la face "avant" des polygones, selon
                                         // le sens des sommets: CCW (counter-clockwise) ou CW (clockwise)

            if(drapeau_liste_luminescences)
            {
                glDisableClientState(GL_VERTEX_ARRAY);
                rendu_surfaces_luminescentes_liste();
            }
            else
            {
                switch (mode_rendu)
                {
                    case VOLUME_MODE_IMMEDIAT:
                        glDisableClientState(GL_VERTEX_ARRAY);
                        rendu_surfaces_luminescentes_immediat();
                    break;

                    //Le volume en mode LISTE peut rendre ses surfaces luminescentes en mode Tableau.
                    //Ca permet d'avoir la possibilité de modifier sa luminescence en temps réel.
                    case VOLUME_MODE_TABLEAUX:
                    case VOLUME_MODE_LISTE:
                    case VOLUME_MODE_LISTE_OPAQUES_IMMEDIAT_TRANSPARENTES:
                    case VOLUME_MODE_LISTE_OPAQUES_TABLEAUX_TRANSPARENTES:
                        glEnableClientState(GL_VERTEX_ARRAY);
                        glVertexPointer(3,GL_FLOAT,0,sommets);
                        rendu_surfaces_luminescentes_tableaux();
                    break;

                    break;
                }
            }

            if (eclairage_mem) glEnable(GL_LIGHTING);
            if (!drapeau_affecte_par_brouillard && brouillard_mem) glEnable(GL_FOG);
            */
        }

        //------------------------------------------------------------------------------
        //Rendu en aplats de couleurs, sans éclairage.
        //------------------------------------------------------------------------------
        void Volume::rendu_aplats()
        {
            if(surface_courante->double_face)glDisable(GL_CULL_FACE);
            else glEnable(GL_CULL_FACE);
            glUseProgram(Shaders3d::programme_flat);
            glUniform4fv(Shaders3d::flat_couleur,1,&surface_courante->couleur_diffusion[0]);
            glBindVertexArray(surface_courante->vao);
                glDrawArrays(GL_TRIANGLES,0,surface_courante->nbr_faces*3);
            glBindVertexArray(0);
            glUseProgram(0);
        }


        //------------------------------------------------------------------------------
        //Rendu en aplats de couleurs, avec éclairage Gouraud
        //------------------------------------------------------------------------------
        void Volume::rendu_aplats_Gouraud()
        {
            initialise_materiau();

            glUseProgram(Shaders3d::programme_gouraud2si);
            glUniform4fv(Shaders3d::gouraud2si_materiau_ambiance,1,&materiau_ambiance[0]);
            glUniform4fv(Shaders3d::gouraud2si_materiau_diffusion,1,&materiau_diffusion[0]);
            glUniform4fv(Shaders3d::gouraud2si_materiau_luminosite,1,&materiau_luminosite[0]);
            glUniform4fv(Shaders3d::gouraud2si_materiau_specularite,1,&materiau_specularite[0]);
            glUniform1f(Shaders3d::gouraud2si_materiau_brillance,surface_courante->brillance);
            glBindVertexArray(surface_courante->vao);
                glDrawArrays(GL_TRIANGLES,0,surface_courante->nbr_faces*3);
            glBindVertexArray(0);
            glUseProgram(0);
        }

        //------------------------------------------------------------------------------
        //Rendu en aplats de couleurs, avec éclairage Phong
        //------------------------------------------------------------------------------
        void Volume::rendu_aplats_Phong()
        {
            initialise_materiau();

            glUseProgram(Shaders3d::programme_phong2si);
            glUniform4fv(Shaders3d::phong2si_materiau_ambiance,1,&materiau_ambiance[0]);
            glUniform4fv(Shaders3d::phong2si_materiau_diffusion,1,&materiau_diffusion[0]);
            glUniform4fv(Shaders3d::phong2si_materiau_luminosite,1,&materiau_luminosite[0]);
            glUniform4fv(Shaders3d::phong2si_materiau_specularite,1,&materiau_specularite[0]);
            glUniform1f(Shaders3d::phong2si_materiau_brillance,surface_courante->brillance);

            glBindVertexArray(surface_courante->vao);
                glDrawArrays(GL_TRIANGLES,0,surface_courante->nbr_faces*3);
            glBindVertexArray(0);
            glUseProgram(0);
        }

        //------------------------------------------------------------------------------
        //Rendu avec texture, sans éclairage
        //------------------------------------------------------------------------------
        void Volume::rendu_texture()
        {
            if(surface_courante->double_face)glDisable(GL_CULL_FACE);
            else glEnable(GL_CULL_FACE);
            glUseProgram(Shaders3d::programme_texture);
            glUniform4fv(Shaders3d::texture_couleur,1,&surface_courante->couleur_diffusion[0]);
            glActiveTexture(GL_TEXTURE0);
            Texture* texture_couleur=surface_courante->textures_couleur[0];
            glBindTexture(GL_TEXTURE_2D,texture_couleur->image->texture_id);
            glUniform1i(Shaders3d::texture_texture,0);
            glBindVertexArray(surface_courante->vao);
                glDrawArrays(GL_TRIANGLES,0,surface_courante->nbr_faces*3);
            glBindVertexArray(0);
            glUseProgram(0);
        }

        //------------------------------------------------------------------------------
        //Rendu avec texture, éclairage Gouraud
        //------------------------------------------------------------------------------
        void Volume::rendu_texture_Gouraud()
        {
            initialise_materiau();

            glUseProgram(Shaders3d::programme_texture_gouraud2si);
            glUniform4fv(Shaders3d::texture_gouraud2si_materiau_ambiance,1,&materiau_ambiance[0]);
            glUniform4fv(Shaders3d::texture_gouraud2si_materiau_diffusion,1,&materiau_diffusion[0]);
            glUniform4fv(Shaders3d::texture_gouraud2si_materiau_luminosite,1,&materiau_luminosite[0]);
            glUniform4fv(Shaders3d::texture_gouraud2si_materiau_specularite,1,&materiau_specularite[0]);
            glUniform1f(Shaders3d::texture_gouraud2si_materiau_brillance,surface_courante->brillance);

            glActiveTexture(GL_TEXTURE0);
            Texture* texture_couleur=surface_courante->textures_couleur[0];
            glBindTexture(GL_TEXTURE_2D,texture_couleur->image->texture_id);
            glUniform1i(Shaders3d::texture_gouraud2si_texture,0);

            glBindVertexArray(surface_courante->vao);
                glDrawArrays(GL_TRIANGLES,0,surface_courante->nbr_faces*3);
            glBindVertexArray(0);
            glUseProgram(0);
        }

        //------------------------------------------------------------------------------
        //Rendu avec texture, éclairage Phong
        //------------------------------------------------------------------------------
        void Volume::rendu_texture_Phong()
        {
            initialise_materiau();

            glUseProgram(Shaders3d::programme_texture_phong2si);
            glUniform4fv(Shaders3d::texture_phong2si_materiau_ambiance,1,&materiau_ambiance[0]);
            glUniform4fv(Shaders3d::texture_phong2si_materiau_diffusion,1,&materiau_diffusion[0]);
            glUniform4fv(Shaders3d::texture_phong2si_materiau_luminosite,1,&materiau_luminosite[0]);
            glUniform4fv(Shaders3d::texture_phong2si_materiau_specularite,1,&materiau_specularite[0]);
            glUniform1f(Shaders3d::texture_phong2si_materiau_brillance,surface_courante->brillance);

            glActiveTexture(GL_TEXTURE0);
            Texture* texture_couleur=surface_courante->textures_couleur[0];
            glBindTexture(GL_TEXTURE_2D,texture_couleur->image->texture_id);
            glUniform1i(Shaders3d::texture_phong2si_texture,0);

            glBindVertexArray(surface_courante->vao);
                glDrawArrays(GL_TRIANGLES,0,surface_courante->nbr_faces*3);
            glBindVertexArray(0);
            glUseProgram(0);
        }













