#include "Primitives.h"

using namespace std;

//============================================================================================
//          Coordonnées des formes trop simples pour nécessiter des fonctions génératrices
//============================================================================================

float Primitives::plan_sommets[2*4]={-.5,-.5, .5,-.5, .5,.5, -.5,.5};

float Primitives::triangle_sommets[2*3]={0.,1.,
                                -0.8660254,-0.5,
                                0.8660254,-0.5};

float Primitives::cube_sommets[3*8]={-0.5,0.5,-0.5,
                                0.5,0.5,-0.5,
                                0.5,-0.5,-0.5,
                                -0.5,-0.5,-0.5,

                                -0.5,0.5,0.5,
                                -0.5,-0.5,0.5,
                                0.5,-0.5,0.5,
                                0.5,0.5,0.5};

uint16_t Primitives::cube_faces[4*6]={3,2,1,0,
                                2,6,7,1,
                                7,6,5,4,
                                4,5,3,0,
                                1,7,4,0,
                                3,5,6,2};

float Primitives::cube_normales[3*6]={0.,0.,-1.,
                                1.,0.,0.,
                                0.,0.,1.,
                                -1.,0.,0.,
                                0.,1.,0.,
                                0.,-1.,0.};

float Primitives::tetraedre_sommets[3*4]={-500.e-3,-177.8314e-3,-288.6751e-3,
                                            500.e-3,-177.8314e-3,-288.6751e-3,
                                            0.,-177.8314e-3,577.3503e-3,
                                            0.,533.4935e-3,0.};
float Primitives::tetraedre_normales[3*4]={0.,-1.,0.,
                                            0.,0.376041,-0.926603,
                                            -0.802462,0.376041,0.463301,
                                            0.802462,0.376041,0.463301};
uint16_t Primitives::tetraedre_faces[3*4]={0,2,1,
                                            0,1,3,
                                            0,3,2,
                                            1,2,3};


float Primitives::icosaedre_sommets[12*3]={
                                            0.5/NOMBREDOR,0.5,0.,
                                            0.5,0.,-0.5/NOMBREDOR,
                                            0.5,0.,0.5/NOMBREDOR,
                                            0.,0.5/NOMBREDOR,0.5,
                                            -0.5/NOMBREDOR,0.5,0.,
                                            0.,0.5/NOMBREDOR,-0.5,
                                            -0.5/NOMBREDOR,-0.5,0.,
                                            -0.5,0.,0.5/NOMBREDOR,
                                            0.,-0.5/NOMBREDOR,0.5,
                                            0.5/NOMBREDOR,-0.5,0.,
                                            0.,-0.5/NOMBREDOR,-0.5,
                                            -0.5,0.,-0.5/NOMBREDOR
                                        };
uint16_t Primitives::icosaedre_faces[20*3]={
                                            0,1,2, 0,2,3, 0,3,4, 0,4,5, 0,5,1,
                                            1,9,2, 2,9,8, 2,8,3, 3,8,7, 3,7,4, 4,7,11, 4,11,5, 5,11,10, 5,10,1, 1,10,9,
                                            6,7,8, 6,8,9, 6,9,10, 6,10,11, 6,11,7
                                            };

//=============================================================
//          Matériaux:
//=============================================================

float Primitives::materiau_amb[4]={0.2,0.2,0.2,1.};
float Primitives::materiau_diff[4]={0.8,0.8,0.8,1.};
float Primitives::materiau_lum[4]={0.,0.,0.,1.};
float Primitives::materiau_spec[4]={1.,1.,1.,1.};
float Primitives::materiau_brillance=40.;

//=============================================================
//          Génère les coordonnées de formes primitives
//=============================================================

void Primitives::genere_disque(float rayon,uint16_t numCotes,float* coordsTab, float y,float angleDepart)
{
	float astep=2.*M_PI/numCotes;
	float a=angleDepart;
	for(int i=0;i<numCotes;i++)
	{
		coordsTab[3*i]=rayon*cos(a);
		coordsTab[3*i+1]=y;
		coordsTab[3*i+2]=rayon*sin(a);
		a+=astep;
	}
}

void Primitives::genere_disque2d(float rayon,uint16_t numCotes,float* coordsTab)
{
	float astep=2.*M_PI/numCotes;
	float a=0.;
	for(int i=0;i<numCotes;i++)
	{
		coordsTab[2*i]=rayon*cos(a);
		coordsTab[2*i+1]=rayon*sin(a);
		a+=astep;
	}
}

void Primitives::genere_cylindre(float rayon, float hauteur, uint16_t numCotes, uint16_t numSections, float* coordsTab)
{
	float y=hauteur/2.;
	float ystep=hauteur/(numSections-1);
	for(int i=0;i<numSections;i++)
	{
		float* coordsTabSegment=coordsTab+i*3*numCotes;
		genere_disque(rayon,numCotes,coordsTabSegment,y);
		y-=ystep;
	}
}

void Primitives::genere_faces_cylindre(uint16_t numCotes, uint16_t numSections, uint16_t* facesTab)
{
	uint32_t k=0;
	uint16_t i,j,p;
	uint16_t n=numCotes*numSections;
	for(i=0;i<numSections-1;i++)
	{
		uint16_t m=(i*numCotes);
		for(j=0;j<numCotes;j++)
		{
			p=(j+1)%numCotes;
			facesTab[k]=m+j;
			facesTab[k+1]=m+p;
			facesTab[k+2]=m+p+numCotes;

			facesTab[k+3]=m+j;
			facesTab[k+4]=m+p+numCotes;
			facesTab[k+5]=m+j+numCotes;
			k+=6;
		}
	}
	//Les deux dernières faces sont les extrémités (fermeture du cylindre)
	for(i=0;i<numCotes;i++)
	{
		facesTab[k]=(numCotes-1)-i;
		facesTab[k+numCotes]=n-numCotes+i;
		k++;
	}
}

void Primitives::genere_normales_cylindre(uint32_t numPoints, float* coordsTab, float* normalesTab)
{
	for (uint32_t i=0;i<numPoints;i++)
	{
		float px=coordsTab[3*i];
		float pz=coordsTab[3*i+2];
		float n=sqrtf(px*px+pz*pz);
		normalesTab[3*i]=px/n;
		normalesTab[3*i+1]=0.;
		normalesTab[3*i+2]=pz/n;
	}
}

void Primitives::genere_tore(float rayon1,float rayon2, uint16_t numCotes, uint16_t numSections, float* coordsTab)
{
	float astep=M_PI*2./numCotes;
	float aSection=0.;
	for(int i=0;i<numCotes;i++)
	{
		float rSections=rayon1+rayon2*cos(aSection);
		float y=rayon2*sin(aSection);
		float* coordsTabSections=coordsTab+3*i*numSections;
		genere_disque(rSections,numSections,coordsTabSections,y);
		aSection+=astep;
	}
}

void Primitives::genere_faces_tore(uint16_t numCotes, uint16_t numSections, uint16_t* facesTab)
{
	uint32_t k=0;
	uint16_t i,j,p;
	uint16_t n=numSections*numCotes;
	for(i=0;i<numCotes;i++)
	{
		uint16_t m=(i*numSections);
		for(j=0;j<numSections;j++)
		{
			p=(j+1)%numSections;
			facesTab[k]=m+j;
			facesTab[k+1]=m+p;
			facesTab[k+2]=(m+p+numSections)%n;

			facesTab[k+3]=facesTab[k+2];
            facesTab[k+4]=(m+j+numSections)%n;
            facesTab[k+5]=facesTab[k];
			k+=6;
		}
	}
}

void Primitives::genere_normales_tore(float rayon,uint32_t numPoints,float* coordsTab, float* normalesTab)
{
	for (uint32_t i=0; i<numPoints;i++)
	{
		float px=coordsTab[3*i];
		float pz=coordsTab[3*i+2];
		float n=sqrtf(px*px+pz*pz);
		float cx=px/n*rayon;
		float cz=pz/n*rayon;
		float nx=px-cx;
		float ny=coordsTab[3*i+1];
		float nz=pz-cz;
		n=sqrtf(nx*nx+ny*ny+nz*nz);
		normalesTab[3*i]=nx/n;
		normalesTab[3*i+1]=ny/n;
		normalesTab[3*i+2]=nz/n;
	}
}

void Primitives::genere_sphere(float rayon, uint16_t numCotes, uint16_t numSegments, float* coordsTab)
{
	float astep=M_PI/numSegments;
	float aSections=astep;
	float r,y;
	coordsTab[0]=0.;
	coordsTab[1]=rayon;
	coordsTab[2]=0.;
	int dernier=3*(numCotes*(numSegments-1)+1);
	coordsTab[dernier]=0.;
	coordsTab[dernier+1]=-rayon;
	coordsTab[dernier+2]=0.;

	float* coordsTabSections=&coordsTab[3];
	for(int i=0;i<numSegments-1;i++)
	{
		y=rayon*cos(aSections);
		r=rayon*sin(aSections);
		float* coordsTabSection=&coordsTabSections[3*i*numCotes];
		genere_disque(r,numCotes,coordsTabSection,y);
		aSections+=astep;
	}
}

void Primitives::genere_faces_sphere(uint16_t numCotes, uint16_t numSegments, uint16_t* facesTab)
{
	uint32_t k=0;
	uint16_t i,j,p;
	uint16_t m;
	for(i=0;i<numSegments-2;i++)
	{
		m=(i*numCotes)+1;
		for(j=0;j<numCotes;j++)
		{
			p=(j+1)%numCotes;
			facesTab[k]=m+j;
			facesTab[k+1]=m+j+numCotes;
			facesTab[k+2]=m+p+numCotes;

			facesTab[k+3]=m+p+numCotes;
			facesTab[k+4]=m+p;
			facesTab[k+5]=m+j;
			k+=6;
		}
	}

	m=1+numCotes*(numSegments-1);	//indice du dernier point

	for(i=0;i<numCotes-1;i++)
	{
		facesTab[k]=0;
		facesTab[k+1]=i+1;
		facesTab[k+2]=i+2;

		facesTab[k+numCotes*3]=m;
		facesTab[k+numCotes*3+1]=m-i-1;
		facesTab[k+numCotes*3+2]=m-i-2;

		k+=3;
	}
	facesTab[k]=0;
	facesTab[k+1]=numCotes;
	facesTab[k+2]=1;

	facesTab[k+numCotes*3]=m;
	facesTab[k+numCotes*3+1]=m-numCotes;
	facesTab[k+numCotes*3+2]=m-1;
}

void Primitives::genere_normales_sphere(uint32_t numPoints, float* coordsTab, float* normalesTab)
{
	for(uint32_t i=0;i<numPoints;i++)
	{
		float px=coordsTab[3*i];
		float py=coordsTab[3*i+1];
		float pz=coordsTab[3*i+2];
		float n=sqrtf(px*px+py*py+pz*pz);
		normalesTab[3*i]=px/n;
		normalesTab[3*i+1]=py/n;
		normalesTab[3*i+2]=pz/n;
	}
}

void Primitives::genere_sphere_tesselation(float rayon, uint16_t numDivisions, float* coordsTab)
{
    /*
    float r=rayon*2.;
    int i;
    for(i=0;i<12;i++)
    {
        coordsTab[i*3]=icosaedre_sommets[i*3]*r;
        coordsTab[i*3+1]=icosaedre_sommets[i*3+1]*r;
        coordsTab[i*3+2]=icosaedre_sommets[i*3+2]*r;
    }
    */


    float astep=ANGLEDIAGONALE_OR/float(numDivisions);
	float aSections=astep;
	float r,y;
	int i;
	int numCotes=5;
	//int numSommets;
	coordsTab[0]=0.;
	coordsTab[1]=rayon;
	coordsTab[2]=0.;
	//numSommets=1;
	float* coordsTabSection=&coordsTab[3];
	for(i=0;i<numDivisions-1;i++)
	{
		y=rayon*cos(aSections);
		r=rayon*sin(aSections);
		genere_disque(r,numCotes,coordsTabSection,y);
		//numSommets+=numCotes;
		coordsTabSection=&coordsTabSection[numCotes*3];
		numCotes+=5;
		aSections+=astep;
	}
	float aDecal=float(M_PI)/(5.*float(numDivisions));
    float aDepart=0.;
	for(i=0;i<=numDivisions;i++)
	{
	    //if((i&0x1)==1)aDepart=aDecal;else aDepart=0.;
		y=rayon*cos(aSections);
		r=rayon*sin(aSections);
		genere_disque(r,numCotes,coordsTabSection,y,aDepart);
		//numSommets+=numCotes;
		coordsTabSection=&coordsTabSection[numCotes*3];
		aSections+=astep;
		aDepart+=aDecal;
	}

	//if((i&0x1)==1)aDepart=aDecal;else aDepart=0.;
    aDepart-=aDecal;    //Pour recaler les triangles...
	for(i=0;i<numDivisions-1;i++)
	{
		y=rayon*cos(aSections);
		r=rayon*sin(aSections);
		numCotes-=5;
		genere_disque(r,numCotes,coordsTabSection,y,aDepart);
		//numSommets+=numCotes;
		coordsTabSection=&coordsTabSection[numCotes*3];
		aSections+=astep;
	}
	coordsTabSection[0]=0.;
	coordsTabSection[1]=-rayon;
	coordsTabSection[2]=0.;
	//numSommets++;
	//cout<<"Nombre de sommets générés: "<<numSommets<<endl;
}

void Primitives::genere_faces_sphere_tesselation(uint16_t numDivisions, uint16_t* facesTab)
{
    int i,j,k,m,numTrianglesLigne;
    uint16_t n,p1,p2,p3,offset_l1,offset_l2,numCotes_l1,numCotes_l2,numSommets,numFaces;
    //Calcul le nombre de sommets et de faces:

    numFaces=20*numDivisions*numDivisions;

    j=0;
    for(i=0;i<numDivisions-1;i++) j+=i;
    numSommets=j*20+(numDivisions-1)*30+12;

    uint16_t* facesTab_fin=&facesTab[3*numFaces-3];

    //Relie les polygones des extrémités de l'icosaèdre:
    //5 triangles supérieurs:
    m=0;
    for(k=0;k<5;k++)
    {
        n=0;
        offset_l1=0;
        offset_l2=0;
        numCotes_l1=1;
        for (j=0;j<numDivisions;j++)
        {
            numTrianglesLigne=2*numCotes_l1/5+1;
            offset_l2+=numCotes_l1;
            numCotes_l2=(j+1)*5;
            //cout<<"numTrianglesLigne "<<numTrianglesLigne<<" offset_l1 "<<offset_l1<<" offset_l2 "<<offset_l2<<endl;
            for(i=0;i<numTrianglesLigne;i++)
            {
                p1=((n-offset_l1)%numCotes_l1)+offset_l1;
                if((i&0x1)==0)
                {
                    p2=n+numCotes_l1+k+1;
                    p3=n+numCotes_l1+k;
                    p2=((p2-offset_l2)%numCotes_l2)+offset_l2;
                }
                else
                {
                    p2=n+1;
                    p3=n+numCotes_l1+k+1;
                    p2=((p2-offset_l1)%numCotes_l1)+offset_l1;
                    n++;
                }
                facesTab[m]=p3;
                facesTab[m+1]=p2;
                facesTab[m+2]=p1;
                m+=3;
            }
            n=n-j+numCotes_l1+k;
            numCotes_l1=numCotes_l2;
            offset_l1=offset_l2;
        }
    }

    //Triangles équatoriaux:
    n=n-numDivisions*4;
    //cout<<"Valeurs intermédiaires:"<<n<<" "<<offset_l1<<" "<<numCotes_l1<<endl;
    //cout<<"Triangles:"<<endl;
    for(k=0;k<numDivisions;k++)
    {
        offset_l2=offset_l1+numCotes_l1;
        for(j=0;j<numCotes_l1*2;j++)
        {
            if((j&0x1)==0)
            {
                p1=n;
                p2=((n+1-offset_l1)%numCotes_l1)+offset_l1;
                p3=n+numCotes_l1;
                n++;
            }
            else
            {
                p1=((n-offset_l1)%numCotes_l1)+offset_l1;
                p2=((n+numCotes_l1-offset_l2)%numCotes_l1)+offset_l2;
                p3=n+numCotes_l1-1;
            }
            facesTab[m]=p3;
            facesTab[m+1]=p2;
            facesTab[m+2]=p1;
            //cout<<facesTab[m]<<" "<<facesTab[m+1]<<" "<<facesTab[m+2]<<endl;
            m+=3;
        }
        offset_l1=offset_l2;
    }


    // 5 triangles inférieurs:
    m=0;
    for(k=0;k<5;k++)
    {
        n=numSommets;
        numCotes_l1=1;
        offset_l1=numSommets-1;
        offset_l2=numSommets-1;
        for (j=0;j<numDivisions;j++)
        {
            numTrianglesLigne=2*numCotes_l1/5+1;
            numCotes_l2=(j+1)*5;
            offset_l2-=numCotes_l2;
            //cout<<"numTrianglesLigne "<<numTrianglesLigne<<" offset_l1 "<<offset_l1<<" offset_l2 "<<offset_l2<<endl;
            for(i=0;i<numTrianglesLigne;i++)
            {
                p1=((n-offset_l1)%numCotes_l1)+offset_l1;
                if((i&0x1)==0)
                {
                    p2=n-numCotes_l1-k-1;
                    p3=p2+1;
                    p3=((p3-offset_l2)%numCotes_l2)+offset_l2;
                }
                else
                {
                    p2=n-1;
                    p3=n-numCotes_l1-k-1;
                    n--;
                }
                facesTab_fin[m]=p3;
                facesTab_fin[m+1]=p2;
                facesTab_fin[m+2]=p1;
                //cout<<facesTab_fin[m]<<" "<<facesTab_fin[m+1]<<" "<<facesTab_fin[m+2]<<endl;
                m-=3;
            }
            offset_l1=offset_l2;
            n=n-numCotes_l1+j-k;
            numCotes_l1=numCotes_l2;
        }

    }
}

/// Optimisation possible, mais à faire si vraiment y'a besoin...
void Primitives::genere_normales_cone(uint16_t numCotes, float* coordsTab, float* normalesTab )
{
    vec3f v1,v2,v3,v4,v5;
    v1.init(&coordsTab[0]);
    normalesTab[0]=0.;  //hé ouai, le sommet du cône n'a pas de normale ! (et ça marche dans les calculs d'éclairage)
    normalesTab[1]=0.;
    normalesTab[2]=0.;
    for (uint32_t i=1;i<=numCotes;i++)
	{
	    v2.init(&coordsTab[3*i]);
	    v3.soustraction(v2,v1);

        v4.produit_vectoriel(v1,v2);
        v5.produit_vectoriel(v3,v4);
        v5.normalise();

		normalesTab[3*i]=v5.x;
		normalesTab[3*i+1]=v5.y;
		normalesTab[3*i+2]=v5.z;
	}
}

void Primitives::genere_normales_cone_tronc(uint16_t numCotes, float* coordsTab, float* normalesTab )
{
    float* coordsBase=&coordsTab[3*numCotes];
    float* normalesBase=&normalesTab[3*numCotes];
    vec3f v1;
    vec3f v2;
    vec3f v3;
    vec3f v4,v5;
    for (uint32_t i=0;i<numCotes;i++)
	{
	    v1.init(&coordsTab[3*i]);
	    v2.init(&coordsBase[3*i]);
	    v3.soustraction(v2,v1);

        v4.produit_vectoriel(v1,v2);
        v5.produit_vectoriel(v3,v4);
        v5.normalise();

		normalesTab[3*i]=v5.x;
		normalesTab[3*i+1]=v5.y;
		normalesTab[3*i+2]=v5.z;

		normalesBase[3*i]=v5.x;
		normalesBase[3*i+1]=v5.y;
		normalesBase[3*i+2]=v5.z;
	}
}

/************************************************************

    Génère les tableaux des sommets pour les VBOs
    Les matrices d'instance ne sont pas initialisées !!!
    Elles doivent être gérées par le conteneur des primitives

************************************************************/

    void Primitives::genere_buffers_phong2si(GLuint& vbo, GLuint& vao)
    {
        glGenBuffers(1,&vbo);
        glGenVertexArrays(1,&vao);

        glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER,vbo);

                glEnableVertexAttribArray(ShadersPrimitives::prim_phong2si_position);
                glEnableVertexAttribArray(ShadersPrimitives::prim_phong2si_normale);

                glVertexAttribPointer(ShadersPrimitives::prim_phong2si_position,3,GL_FLOAT,GL_FALSE,sizeof(float)*6,0);
                glVertexAttribPointer(ShadersPrimitives::prim_phong2si_normale,3,GL_FLOAT,GL_FALSE,sizeof(float)*6,(void*)(3*sizeof(float)));
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }

    // Une normale par face. Les normales doivent se présenter dans le même ordre que les faces.
    void Primitives::maj_tableau_sommets_vbo_flat(int numFaces,uint16_t* facesTab,float* sommets, float* normalesTab, float* sommetsVBO)
    {
        int i,j,k;
        uint16_t indice;
        float* normale;
        j=0;
        for (i=0;i<numFaces;i++)
        {
            normale=&normalesTab[i*3];
            for(k=0;k<3;k++)
            {
                indice=facesTab[i*3+k]*3;
                sommetsVBO[j]=sommets[indice];
                sommetsVBO[j+1]=sommets[indice+1];
                sommetsVBO[j+2]=sommets[indice+2];
                sommetsVBO[j+3]=normale[0];
                sommetsVBO[j+4]=normale[1];
                sommetsVBO[j+5]=normale[2];
                j+=6;
            }
        }
    }

    // Une normale par sommet. Les normales sont dans le même ordre que les sommets.
    void Primitives::maj_tableau_sommets_vbo_smooth(int numFaces,uint16_t* facesTab,float* sommets, float* normalesTab, float* sommetsVBO)
    {
        int i,j,k;
        uint16_t indice;
        j=0;
        for (i=0;i<numFaces;i++)
        {
            for(k=0;k<3;k++)
            {
                indice=facesTab[i*3+k]*3;
                sommetsVBO[j]=sommets[indice];
                sommetsVBO[j+1]=sommets[indice+1];
                sommetsVBO[j+2]=sommets[indice+2];
                sommetsVBO[j+3]=normalesTab[indice];
                sommetsVBO[j+4]=normalesTab[indice+1];
                sommetsVBO[j+5]=normalesTab[indice+2];
                j+=6;
            }
        }
    }


/********************** Matériaux ************************************/


void Primitives::init_materiau(float Ramb,float Vamb, float Bamb,float Aamb,
                                       float Rlum,float Vlum,float Blum,float Alum,
                                       float Rdiff,float Vdiff,float Bdiff,float Adiff,
                                       float Rspec,float Vspec,float Bspec,float Aspec,
                                       float brillance)
{
    materiau_amb[0]=Ramb;materiau_amb[1]=Vamb;materiau_amb[2]=Bamb;materiau_amb[3]=Aamb;
    materiau_lum[0]=Rlum;materiau_lum[1]=Vlum;materiau_lum[2]=Blum;materiau_lum[3]=Alum;
    materiau_diff[0]=Rdiff;materiau_diff[1]=Vdiff;materiau_diff[2]=Bdiff;materiau_diff[3]=Adiff;
    materiau_spec[0]=Rspec;materiau_spec[1]=Vspec;materiau_spec[2]=Bspec;materiau_spec[3]=Aspec;
    materiau_brillance=brillance;
}

void Primitives::init_materiau(uint32_t RVBAamb, uint32_t RVBAlum, uint32_t RVBAdiff, uint32_t RVBAspec,float brillance)
{
    materiau_amb[0]=float((RVBAamb>>24)&0xff)/255.;materiau_amb[1]=float((RVBAamb>>16)&0xff)/255.;materiau_amb[2]=float((RVBAamb>>8)&0xff)/255.;materiau_amb[3]=float(RVBAamb&0xff)/255.;
    materiau_lum[0]=float((RVBAlum>>24)&0xff)/255.;materiau_lum[1]=float((RVBAlum>>16)&0xff)/255.;materiau_lum[2]=float((RVBAlum>>8)&0xff)/255.;materiau_lum[3]=float(RVBAlum&0xff)/255.;
    materiau_diff[0]=float((RVBAdiff>>24)&0xff)/255.;materiau_diff[1]=float((RVBAdiff>>16)&0xff)/255.;materiau_diff[2]=float((RVBAdiff>>8)&0xff)/255.;materiau_diff[3]=float(RVBAdiff&0xff)/255.;
    materiau_spec[0]=float((RVBAspec>>24)&0xff)/255.;materiau_spec[1]=float((RVBAspec>>16)&0xff)/255.;materiau_spec[2]=float((RVBAspec>>8)&0xff)/255.;materiau_spec[3]=float(RVBAspec&0xff)/255.;
    materiau_brillance=brillance;
}

void Primitives::init_phong2si()
{
    glUseProgram(ShadersPrimitives::programme_prim_phong2si);
    glUniform4fv(ShadersPrimitives::prim_phong2si_materiau_ambiance,1,&materiau_amb[0]);
    glUniform4fv(ShadersPrimitives::prim_phong2si_materiau_diffusion,1,&materiau_diff[0]);
    glUniform4fv(ShadersPrimitives::prim_phong2si_materiau_luminosite,1,&materiau_lum[0]);
    glUniform4fv(ShadersPrimitives::prim_phong2si_materiau_specularite,1,&materiau_spec[0]);
    glUniform1f(ShadersPrimitives::prim_phong2si_materiau_brillance,materiau_brillance);
}

/*******************************************************************
                                Objets
/*******************************************************************/

Primitives::Primitives()
{
    vbo=0;
    vao=0;
    sommets=NULL;
    normales=NULL;
    sommetsVBO=NULL;
    faces=NULL;
    numSommets=0;
    numSommetsVBO=0;
    numFaces=0;
}

Primitives::~Primitives()
{
    if(glIsVertexArray(vao))glDeleteVertexArrays(1,&vao);
    if(glIsBuffer(vbo))glDeleteBuffers(1,&vbo);
    if(sommets!=NULL) free(sommets);
    if(normales!=NULL) free(normales);
    if(faces!=NULL) free(faces);
    if(sommetsVBO!=NULL) free(sommetsVBO);
}

/*****************************************************************************************************
                                        Plan
/*****************************************************************************************************/

Plan::Plan(float xs, float zs)
{
    xSize=xs;
    zSize=zs;
    numFaces=1;
    numSommets=4;
    mode=PRIM_PLAT;
    sommets=(float*)malloc(4*2*sizeof(float));
    sommetsVBO=(float*)malloc(4*6*sizeof(float));
    for(int i=0;i<4;i++)
    {
        sommets[i*2]=plan_sommets[i*2]*xSize;
        sommets[i*2+1]=plan_sommets[i*2+1]*zSize;
    }
    genere_buffers_phong2si(vbo,vao);
    maj_sommetsVBO();

}

Plan::~Plan()
{
}

void Plan::maj_sommetsVBO()
{
    int j,k;
    j=0;
    for(k=0;k<4;k++)
    {
        sommetsVBO[j]=sommets[2*k];
        sommetsVBO[j+1]=0.;
        sommetsVBO[j+2]=sommets[2*k+1];
        sommetsVBO[j+3]=0.;
        sommetsVBO[j+4]=1.;
        sommetsVBO[j+5]=0.;

        j+=6;
    }
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*4*6,sommetsVBO,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

// Au préalable, il faut que le shader d'affichage soit initialisé.
void Plan::affiche()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glBindVertexArray(0);
}

/************************************
            Double faces
************************************/

/* Version TRIANGLES */

Plan2Cotes::Plan2Cotes(float xs, float zs)
{
    xSize=xs;
    zSize=zs;
    numFaces=4;
    numSommets=4;
    mode=PRIM_PLAT;
    sommets=(float*)malloc(4*2*sizeof(float));
    sommetsVBO=(float*)malloc(12*6*sizeof(float));
    for(int i=0;i<4;i++)
    {
        sommets[i*2]=plan_sommets[i*2]*xSize;
        sommets[i*2+1]=plan_sommets[i*2+1]*zSize;
    }
    genere_buffers_phong2si(vbo,vao);
    maj_sommetsVBO();

}

Plan2Cotes::~Plan2Cotes()
{
}

void Plan2Cotes::maj_sommetsVBO()
{
    uint16_t facesIndexes[3*4]={0,1,3, 1,2,3, 3,2,1, 3,1,0};
    uint16_t index;
    int i,j,k;

    for(int i=0;i<6;i++)
    {
        j=i*6;
        k=(i+6)*6;
        index=facesIndexes[i];
        sommetsVBO[j]=sommets[index*2];
        sommetsVBO[j+1]=0.;
        sommetsVBO[j+2]=sommets[index*2+1];
        sommetsVBO[j+3]=0.;
        sommetsVBO[j+4]=1.;
        sommetsVBO[j+5]=0.;
        index=facesIndexes[i+6];
        sommetsVBO[k]=sommets[index*2];
        sommetsVBO[k+1]=0.;
        sommetsVBO[k+2]=sommets[index*2+1];
        sommetsVBO[k+3]=0.;
        sommetsVBO[k+4]=-1.;
        sommetsVBO[k+5]=0.;
    }

    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*12*6,sommetsVBO,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

// Au préalable, il faut que le shader d'affichage soit initialisé.
void Plan2Cotes::affiche()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES,0,12);
    glBindVertexArray(0);
}


/* Version TRIANGLE_STRIP */
/*
Plan2Cotes::Plan2Cotes(float xs, float zs)
{
    xSize=xs;
    zSize=zs;
    numFaces=1;
    numSommets=4;
    sommets=(float*)malloc(4*2*sizeof(float));
    sommetsVBO=(float*)malloc(8*6*sizeof(float));
    for(int i=0;i<4;i++)
    {
        sommets[i*2]=plan_sommets[i*2]*xSize;
        sommets[i*2+1]=plan_sommets[i*2+1]*zSize;
    }
    genere_buffers_phong2si(vbo,vao);
    maj_sommetsVBO();

}

Plan2Cotes::~Plan2Cotes()
{
}

void Plan2Cotes::maj_sommetsVBO()
{
    int k;

    sommetsVBO[0]=sommets[0];
    sommetsVBO[2]=sommets[1];

    sommetsVBO[6]=sommets[2];
    sommetsVBO[8]=sommets[3];

    sommetsVBO[12]=sommets[6];
    sommetsVBO[14]=sommets[7];

    sommetsVBO[18]=sommets[4];
    sommetsVBO[20]=sommets[5];

    sommetsVBO[24]=sommets[6];
    sommetsVBO[26]=sommets[7];

    sommetsVBO[30]=sommets[4];
    sommetsVBO[32]=sommets[5];

    sommetsVBO[36]=sommets[0];
    sommetsVBO[38]=sommets[1];

    sommetsVBO[42]=sommets[2];
    sommetsVBO[44]=sommets[3];

    for(k=0;k<4;k++)
    {
        sommetsVBO[k*6+1]=0.;
        sommetsVBO[k*6+3]=0.;
        sommetsVBO[k*6+4]=1.;
        sommetsVBO[k*6+5]=0.;
    }
    for(k=4;k<8;k++)
    {
        sommetsVBO[k*6+1]=0.;
        sommetsVBO[k*6+3]=0.;
        sommetsVBO[k*6+4]=-1.;
        sommetsVBO[k*6+5]=0.;
    }
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*8*6,sommetsVBO,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

// Au préalable, il faut que le shader d'affichage soit initialisé.
void Plan2Cotes::affiche()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP,0,8);
    glBindVertexArray(0);
}
*/

/*****************************************************************************************************
                                        Disque
/*****************************************************************************************************/

Disque::Disque(float r, uint16_t p_numSommets)
{
    rayon=r;
    numSommets=p_numSommets;
    numFaces=1;
    mode=PRIM_PLAT;
    sommets=(float*)malloc(numSommets*2*sizeof(float));
    sommetsVBO=(float*)malloc(numSommets*6*sizeof(float));
    genere_disque2d(rayon,numSommets,sommets);
    genere_buffers_phong2si(vbo,vao);
    maj_sommetsVBO();

}

Disque::~Disque()
{

}

void Disque::maj_sommetsVBO()
{
    int j,k;
    j=0;
    for(k=0;k<numSommets;k++)
    {
        sommetsVBO[j]=sommets[2*k];
        sommetsVBO[j+1]=0.;
        sommetsVBO[j+2]=sommets[2*k+1];
        sommetsVBO[j+3]=0.;
        sommetsVBO[j+4]=1.;
        sommetsVBO[j+5]=0.;

        j+=6;
    }
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*numSommets*6,sommetsVBO,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

// Au préalable, il faut que le shader d'affichage soit initialisé.
void Disque::affiche()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN,0,numSommets);
    glBindVertexArray(0);
}

/*************************************************
                Disque double face
**************************************************/

Disque2Cotes::Disque2Cotes(float r, uint16_t p_numSommets)
{
    rayon=r;
    numSommets=p_numSommets;
    numFaces=1;
    mode=PRIM_PLAT;
    sommets=(float*)malloc(numSommets*2*sizeof(float));
    numSommetsVBO=(numSommets+(numSommets&0x1))*2;
    sommetsVBO=(float*)malloc(numSommetsVBO*6*sizeof(float));
    genere_disque2d(rayon,numSommets,sommets);
    genere_buffers_phong2si(vbo,vao);
    maj_sommetsVBO();

}

Disque2Cotes::~Disque2Cotes()
{

}

void Disque2Cotes::maj_sommetsVBO()
{
    int i,j,k,k2,j2,jm;
    k=0;k2=(numSommetsVBO-2)*6;
    uint16_t demi_inc,demi_dec;
    uint16_t numCouples=(numSommets/2)+(numSommets&0x1);
    demi_dec=numSommets/2;
    demi_inc=demi_dec+1;
    for(i=0;i<numCouples;i++)
    {
        sommetsVBO[k]=sommets[2*(demi_dec)];
        sommetsVBO[k+1]=0.;
        sommetsVBO[k+2]=sommets[2*(demi_dec)+1];
        sommetsVBO[k+3]=0.;
        sommetsVBO[k+4]=1.;
        sommetsVBO[k+5]=0.;

        sommetsVBO[k+6]=sommets[2*(demi_inc)];
        sommetsVBO[k+7]=0.;
        sommetsVBO[k+8]=sommets[2*(demi_inc)+1];
        sommetsVBO[k+9]=0.;
        sommetsVBO[k+10]=1.;
        sommetsVBO[k+11]=0.;

        sommetsVBO[k2]=sommets[2*(demi_dec)];
        sommetsVBO[k2+1]=0.;
        sommetsVBO[k2+2]=sommets[2*(demi_dec)+1];
        sommetsVBO[k2+3]=0.;
        sommetsVBO[k2+4]=-1.;
        sommetsVBO[k2+5]=0.;

        sommetsVBO[k2+6]=sommets[2*(demi_inc)];
        sommetsVBO[k2+7]=0.;
        sommetsVBO[k2+8]=sommets[2*(demi_inc)+1];
        sommetsVBO[k2+9]=0.;
        sommetsVBO[k2+10]=-1.;
        sommetsVBO[k2+11]=0.;

        if(demi_dec>0)demi_dec--;
        if(numSommets/2+1+i<numSommets-1)demi_inc++;else demi_inc=0;

        k+=12;
        k2-=12;
    }
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*numSommetsVBO*6,sommetsVBO,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

// Au préalable, il faut que le shader d'affichage soit initialisé.
void Disque2Cotes::affiche()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP,0,numSommetsVBO);
    glBindVertexArray(0);
}

/******************************************************
           Cube
******************************************************/

/* Version TRIANGLES */

Cube::Cube(float xs, float ys,float zs)
{
    xSize=xs;
    ySize=ys;
    zSize=zs;
    numFaces=12;
    numSommets=8;
    mode=PRIM_PLAT;
    sommets=(float*)malloc(8*3*sizeof(float));
    sommetsVBO=(float*)malloc(12*3*6*sizeof(float));
    for(int i=0;i<8;i++)
    {
        sommets[i*3]=cube_sommets[i*3]*xSize;
        sommets[i*3+1]=cube_sommets[i*3+1]*ySize;
        sommets[i*3+2]=cube_sommets[i*3+2]*zSize;
    }
    genere_buffers_phong2si(vbo,vao);
    maj_sommetsVBO();

}

Cube::~Cube()
{
}

void Cube::maj_sommetsVBO()
{
    uint16_t facesIndexes[3*12]={0,3,2, 2,1,0, 7,1,2, 2,6,7, 4,7,6, 6,5,4, 0,4,5, 5,3,0, 0,1,7, 7,4,0, 2,3,5, 5,6,2};
    uint16_t index;
    int i,j,k;
    j=0;
    for(i=0;i<12*3;i++)
    {
        index=facesIndexes[i];
        k=i/6;
        sommetsVBO[j]=sommets[index*3];
        sommetsVBO[j+1]=sommets[index*3+1];
        sommetsVBO[j+2]=sommets[index*3+2];
        sommetsVBO[j+3]=cube_normales[k*3];
        sommetsVBO[j+4]=cube_normales[k*3+1];
        sommetsVBO[j+5]=cube_normales[k*3+2];
        j+=6;
    }

    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*12*3*6,sommetsVBO,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

// Au préalable, il faut que le shader d'affichage soit initialisé.
void Cube::affiche()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES,0,12*3);
    glBindVertexArray(0);
}

/*****************************************************************************************************
                                        Cylindre ouvert aux extrémités
        Affiché avec un TRIANGLE_STRIP. Chaque segment est lié par 4 "triangles dégénérés". Ca évite
        d'avoir plusieurs VBO.

/*****************************************************************************************************/

Cylindre::Cylindre(float r, float l,uint16_t p_numCotes,uint16_t p_numSections,uint8_t p_mode)
{
    rayon=r;
    longueur=l;
    numCotes=p_numCotes;
    numSections=p_numSections;
    numSommets=numCotes*numSections;
    numFaces=1;
    mode=p_mode;
    numSommetsVBO=(numCotes+1)*((numSections)+(numSections-2))+2*(numSections-2);
    //cout<<"Nombre magique:"<<numSommetsVBO<<endl;
    sommets=(float*)malloc((numSommets)*3*sizeof(float));
    normales=(float*)malloc((numSommets)*3*sizeof(float));
    sommetsVBO=(float*)malloc(numSommetsVBO*6*sizeof(float));
    genere_cylindre(rayon,longueur,numCotes,numSections,sommets);
    genere_normales_cylindre(numSommets,sommets,normales);
    genere_buffers_phong2si(vbo,vao);
    maj_sommetsVBO();
}

Cylindre::~Cylindre()
{

}

void Cylindre::maj_sommetsVBO()
{
    int i,j,k,j2,jm;
    k=0;

    for(i=0;i<numSections-1;i++)
    {
        for(j=0;j<=numCotes;j++)
        {
            jm=(j%numCotes)+i*numCotes;
            sommetsVBO[k]=sommets[3*jm];
            sommetsVBO[k+1]=sommets[3*jm+1];
            sommetsVBO[k+2]=sommets[3*jm+2];
            sommetsVBO[k+3]=normales[3*jm];
            sommetsVBO[k+4]=normales[3*jm+1];
            sommetsVBO[k+5]=normales[3*jm+2];
            j2=jm+numCotes;
            sommetsVBO[k+6]=sommets[3*j2];
            sommetsVBO[k+7]=sommets[3*j2+1];
            sommetsVBO[k+8]=sommets[3*j2+2];
            sommetsVBO[k+9]=normales[3*j2];
            sommetsVBO[k+10]=normales[3*j2+1];
            sommetsVBO[k+11]=normales[3*j2+2];
            k+=12;
        }
        if(i<numSections-2)
        {   //Triangles dégénérés:
            sommetsVBO[k]=sommetsVBO[k-6];
            sommetsVBO[k+1]=sommetsVBO[k-5];
            sommetsVBO[k+2]=sommetsVBO[k-4];
            sommetsVBO[k+3]=sommetsVBO[k-3];
            sommetsVBO[k+4]=sommetsVBO[k-2];
            sommetsVBO[k+5]=sommetsVBO[k-1];
            sommetsVBO[k+6]=sommetsVBO[k-6];
            sommetsVBO[k+7]=sommetsVBO[k-5];
            sommetsVBO[k+8]=sommetsVBO[k-4];
            sommetsVBO[k+9]=sommetsVBO[k-3];
            sommetsVBO[k+10]=sommetsVBO[k-2];
            sommetsVBO[k+11]=sommetsVBO[k-1];
            k+=12;
        }
    }
    //cout<<"Confirmation nombre magique:"<<k/6<<endl;


    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*numSommetsVBO*6,sommetsVBO,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

// Au préalable, il faut que le shader d'affichage soit initialisé.
void Cylindre::affiche()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP,0,numSommetsVBO);
    glBindVertexArray(0);
}

/*****************************************************************************************************
                                        Cylindre fermé aux extrémités
        Affiché avec un TRIANGLE_STRIP. Chaque segment est lié par 4 "triangles dégénérés". Ca évite
        d'avoir plusieurs VBO.
        Les extrémités sont également agencées de manière à tenir dans un TRIANGLE_STRIP

/*****************************************************************************************************/

CylindreFerme::CylindreFerme(float r, float l,uint16_t p_numCotes,uint16_t p_numSections,uint8_t p_mode)
{
    rayon=r;
    longueur=l;
    numCotes=p_numCotes;
    numSections=p_numSections;
    numSommets=numCotes*numSections;
    numFaces=1;
    mode=p_mode;
    numSommetsVBO=(numCotes+1)*((numSections)+(numSections-2))+2*(numSections-2) + 2*(numCotes+2-(numCotes&0x1));
    sommets=(float*)malloc((numSommets)*3*sizeof(float));
    normales=(float*)malloc((numSommets)*3*sizeof(float));
    sommetsVBO=(float*)malloc(numSommetsVBO*6*sizeof(float));
    genere_cylindre(rayon,longueur,numCotes,numSections,sommets);
    genere_normales_cylindre(numSommets,sommets,normales);
    genere_buffers_phong2si(vbo,vao);
    maj_sommetsVBO();
}

CylindreFerme::~CylindreFerme()
{

}

void CylindreFerme::maj_sommetsVBO()
{
    int i,j,k,k2,j2,jm;
    k=0;k2=(numSommetsVBO-2)*6;
    uint16_t demi_inc,demi_dec;
    uint16_t numCouples=(numCotes/2)+1;
    demi_dec=numCotes/2;
    demi_inc=demi_dec+1;
    float* sommets_face2=sommets+3*(numSommets-numCotes);
    for(i=0;i<numCouples;i++)
    {
        sommetsVBO[k]=sommets[3*(demi_dec)];
        sommetsVBO[k+1]=sommets[3*(demi_dec)+1];
        sommetsVBO[k+2]=sommets[3*(demi_dec)+2];
        sommetsVBO[k+3]=0.;
        sommetsVBO[k+4]=1.;
        sommetsVBO[k+5]=0.;

        sommetsVBO[k+6]=sommets[3*(demi_inc)];
        sommetsVBO[k+7]=sommets[3*(demi_inc)+1];
        sommetsVBO[k+8]=sommets[3*(demi_inc)+2];
        sommetsVBO[k+9]=0.;
        sommetsVBO[k+10]=1.;
        sommetsVBO[k+11]=0.;

        sommetsVBO[k2]=sommets_face2[3*(demi_dec)];
        sommetsVBO[k2+1]=sommets_face2[3*(demi_dec)+1];
        sommetsVBO[k2+2]=sommets_face2[3*(demi_dec)+2];
        sommetsVBO[k2+3]=0.;
        sommetsVBO[k2+4]=-1.;
        sommetsVBO[k2+5]=0.;

        sommetsVBO[k2+6]=sommets_face2[3*(demi_inc)];
        sommetsVBO[k2+7]=sommets_face2[3*(demi_inc)+1];
        sommetsVBO[k2+8]=sommets_face2[3*(demi_inc)+2];
        sommetsVBO[k2+9]=0.;
        sommetsVBO[k2+10]=-1.;
        sommetsVBO[k2+11]=0.;

        if(demi_dec>0)demi_dec--;
        if(numCotes/2+1+i<numCotes-1)demi_inc++;else demi_inc=0;

        k+=12;
        k2-=12;
    }

    for(i=0;i<numSections-1;i++)
    {
        for(j=0;j<=numCotes;j++)
        {
            jm=(j%numCotes)+i*numCotes;
            sommetsVBO[k]=sommets[3*jm];
            sommetsVBO[k+1]=sommets[3*jm+1];
            sommetsVBO[k+2]=sommets[3*jm+2];
            sommetsVBO[k+3]=normales[3*jm];
            sommetsVBO[k+4]=normales[3*jm+1];
            sommetsVBO[k+5]=normales[3*jm+2];
            j2=jm+numCotes;
            sommetsVBO[k+6]=sommets[3*j2];
            sommetsVBO[k+7]=sommets[3*j2+1];
            sommetsVBO[k+8]=sommets[3*j2+2];
            sommetsVBO[k+9]=normales[3*j2];
            sommetsVBO[k+10]=normales[3*j2+1];
            sommetsVBO[k+11]=normales[3*j2+2];
            k+=12;
        }
        if(i<numSections-2)
        {   //Triangles dégénérés:
            sommetsVBO[k]=sommetsVBO[k-6];
            sommetsVBO[k+1]=sommetsVBO[k-5];
            sommetsVBO[k+2]=sommetsVBO[k-4];
            sommetsVBO[k+3]=sommetsVBO[k-3];
            sommetsVBO[k+4]=sommetsVBO[k-2];
            sommetsVBO[k+5]=sommetsVBO[k-1];
            sommetsVBO[k+6]=sommetsVBO[k-6];
            sommetsVBO[k+7]=sommetsVBO[k-5];
            sommetsVBO[k+8]=sommetsVBO[k-4];
            sommetsVBO[k+9]=sommetsVBO[k-3];
            sommetsVBO[k+10]=sommetsVBO[k-2];
            sommetsVBO[k+11]=sommetsVBO[k-1];
            k+=12;
        }
    }
    //cout<<"Confirmation nombre magique:"<<k/6<<endl;


    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*numSommetsVBO*6,sommetsVBO,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

// Au préalable, il faut que le shader d'affichage soit initialisé.
void CylindreFerme::affiche()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP,0,numSommetsVBO);
    glBindVertexArray(0);
}

/******************************************************
           Sphère
******************************************************/

/* Version TRIANGLES */

Sphere::Sphere(float r, uint16_t p_numCotes,uint16_t p_numSegments,uint8_t p_mode)
{
    rayon=r;
    numCotes=p_numCotes;
    numSegments=p_numSegments;
    numFaces=(numSegments-1)*numCotes*2;
    numSommets=(numCotes*numSegments-1)+2;
    numSommetsVBO=numFaces*3;
    mode=p_mode;
    faces=(uint16_t*)malloc(numFaces*3*sizeof(uint16_t));
    sommets=(float*)malloc(numSommets*3*sizeof(float));
    normales=(float*)malloc(numSommets*3*sizeof(float));
    sommetsVBO=(float*)malloc(numSommetsVBO*6*sizeof(float));
    genere_sphere(rayon,numCotes,numSegments,sommets);
    genere_faces_sphere(numCotes,numSegments,faces);
    genere_normales_sphere(numSommets,sommets,normales);
    genere_buffers_phong2si(vbo,vao);
    maj_sommetsVBO();

}

Sphere::~Sphere()
{
}

void Sphere::maj_sommetsVBO()
{
    uint16_t index;
    int i,j,k;
    j=0;
    for(i=0;i<numSommetsVBO;i++)
    {
        index=faces[i];
        sommetsVBO[j]=sommets[index*3];
        sommetsVBO[j+1]=sommets[index*3+1];
        sommetsVBO[j+2]=sommets[index*3+2];
        sommetsVBO[j+3]=normales[index*3];
        sommetsVBO[j+4]=normales[index*3+1];
        sommetsVBO[j+5]=normales[index*3+2];
        j+=6;
    }

    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*numSommetsVBO*6,sommetsVBO,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

// Au préalable, il faut que le shader d'affichage soit initialisé.
void Sphere::affiche()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES,0,numSommetsVBO);
    glBindVertexArray(0);
}

/****************************************************************
           Sphère par tesselation de l'icosaèdre platonicien
           Icosaèdre:   12 sommets
                        20 faces
                        30 arêtes
****************************************************************/

/* Version TRIANGLES */

SphereTesselation::SphereTesselation(float r, uint16_t p_numDivisions,uint8_t p_mode)
{
    rayon=r;
    numDivisions=p_numDivisions;
    numFaces=20*numDivisions*numDivisions;
    mode=p_mode;
    uint16_t i,j;
    j=0;
    for(i=0;i<numDivisions-1;i++) j+=i;
    numSommets=j*20+(numDivisions-1)*30+12;
    numSommetsVBO=numFaces*3;

    faces=(uint16_t*)malloc(numFaces*3*sizeof(uint16_t));
    sommets=(float*)malloc(numSommets*3*sizeof(float));
    normales=(float*)malloc(numSommets*3*sizeof(float));
    sommetsVBO=(float*)malloc(numSommetsVBO*6*sizeof(float));

    cout<<numDivisions<<" "<<numFaces<<" "<<numSommets<<" "<<numSommetsVBO<<endl;

    genere_sphere_tesselation(rayon,numDivisions,sommets);
    genere_faces_sphere_tesselation(numDivisions,faces);
    genere_normales_sphere(numSommets,sommets,normales);
    genere_buffers_phong2si(vbo,vao);
    maj_sommetsVBO();


}

SphereTesselation::~SphereTesselation()
{
}

void SphereTesselation::maj_sommetsVBO()
{
    uint16_t index;
    int i,j,k;
    j=0;


    for(i=0;i<numSommetsVBO;i++)
    {
        index=faces[i];
        //if((i%3)==0)cout<<" -Face-"<<endl;
        //cout<<"index: "<<i<<" x: "<<sommets[index*3]<<" y:"<<sommets[index*3+1]<<" z:"<<sommets[index*3+2]<<endl;
        sommetsVBO[j]=sommets[index*3];
        sommetsVBO[j+1]=sommets[index*3+1];
        sommetsVBO[j+2]=sommets[index*3+2];
        sommetsVBO[j+3]=normales[index*3];
        sommetsVBO[j+4]=normales[index*3+1];
        sommetsVBO[j+5]=normales[index*3+2];
        j+=6;
    }

    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*numSommetsVBO*6,sommetsVBO,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

// Au préalable, il faut que le shader d'affichage soit initialisé.
void SphereTesselation::affiche()
{
    glPointSize(4.);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES,0,numSommetsVBO);
    glBindVertexArray(0);
}

/******************************************************
           Tore
******************************************************/

/* Version TRIANGLES */

Tore::Tore(float r, float r_section, uint16_t p_numCotes,uint16_t p_numSegments,uint8_t p_mode)
{
    rayon=r;
    rayon_section=r_section;
    numCotes=p_numCotes;
    numSegments=p_numSegments;
    mode=p_mode;

    numFaces=numSegments*numCotes*2;
    numSommets=numSegments*numCotes;
    numSommetsVBO=numFaces*3;

    faces=(uint16_t*)malloc(numFaces*3*sizeof(uint16_t));
    sommets=(float*)malloc(numSommets*3*sizeof(float));
    normales=(float*)malloc(numSommets*3*sizeof(float));
    sommetsVBO=(float*)malloc(numSommetsVBO*6*sizeof(float));

    genere_tore(rayon,rayon_section,numCotes,numSegments,sommets);
    genere_faces_tore(numCotes,numSegments,faces);
    genere_normales_tore(rayon,numSommets,sommets,normales);
    genere_buffers_phong2si(vbo,vao);
    maj_sommetsVBO();

}

Tore::~Tore()
{
}

void Tore::maj_sommetsVBO()
{
    uint16_t index;
    int i,j,k;
    j=0;
    for(i=0;i<numSommetsVBO;i++)
    {
        index=faces[i];
        sommetsVBO[j]=sommets[index*3];
        sommetsVBO[j+1]=sommets[index*3+1];
        sommetsVBO[j+2]=sommets[index*3+2];
        sommetsVBO[j+3]=normales[index*3];
        sommetsVBO[j+4]=normales[index*3+1];
        sommetsVBO[j+5]=normales[index*3+2];
        j+=6;
    }

    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*numSommetsVBO*6,sommetsVBO,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

// Au préalable, il faut que le shader d'affichage soit initialisé.
void Tore::affiche()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES,0,numSommetsVBO);
    glBindVertexArray(0);
}

/*****************************************************************************************************
                                        Cône ouvert à la base
/*****************************************************************************************************/

Cone::Cone( float r, float h, uint16_t p_numCotes,uint8_t p_mode)
{
    rayon=r;
    hauteur=h;
    numCotes=p_numCotes;
    mode=p_mode;

    numSommets=numCotes+1;
    numFaces=numCotes*2;
    numSommetsVBO=numSommets+1;
    sommets=(float*)malloc(numSommets*3*sizeof(float));
    normales=(float*)malloc(numSommets*3*sizeof(float));
    sommetsVBO=(float*)malloc(numSommetsVBO*6*sizeof(float));
    sommets[0]=0.;sommets[1]=hauteur;sommets[2]=0.;
    genere_disque(rayon,numCotes,&sommets[3]);
    genere_normales_cone(numCotes,sommets,normales );
    genere_buffers_phong2si(vbo,vao);
    maj_sommetsVBO();

}

Cone::~Cone()
{

}


void Cone::maj_sommetsVBO()
{
    int j,k;
    j=0;
    for(k=0;k<numSommets;k++)
    {
        sommetsVBO[j]=sommets[3*k];
        sommetsVBO[j+1]=sommets[3*k+1];
        sommetsVBO[j+2]=sommets[3*k+2];
        sommetsVBO[j+3]=normales[3*k];
        sommetsVBO[j+4]=normales[3*k+1];
        sommetsVBO[j+5]=normales[3*k+2];
        j+=6;
    }
    sommetsVBO[j]=sommets[3];
    sommetsVBO[j+1]=sommets[4];
    sommetsVBO[j+2]=sommets[5];
    sommetsVBO[j+3]=normales[3];
    sommetsVBO[j+4]=normales[4];
    sommetsVBO[j+5]=normales[5];


    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*numSommetsVBO*6,sommetsVBO,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

// Au préalable, il faut que le shader d'affichage soit initialisé.
void Cone::affiche()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN,0,numSommetsVBO);
    glBindVertexArray(0);
}


/*****************************************************************************************************
                                        Cône tronqué ouvert aux extrémités
/*****************************************************************************************************/

ConeTronc::ConeTronc(float rs, float rb, float h, uint16_t p_numCotes,uint8_t p_mode)
{
    rayon_sommet=rs;
    rayon_base=rb;
    hauteur=h;
    numCotes=p_numCotes;
    mode=p_mode;

    numSommets=numCotes*2;
    numFaces=numCotes*2;
    numSommetsVBO=numSommets+2;
    sommets=(float*)malloc(numSommets*3*sizeof(float));
    normales=(float*)malloc(numSommets*3*sizeof(float));
    sommetsVBO=(float*)malloc(numSommetsVBO*6*sizeof(float));
    genere_disque(rayon_sommet,numCotes,&sommets[0],hauteur);
    genere_disque(rayon_base,numCotes,&sommets[3*numCotes]);
    genere_normales_cone_tronc(numCotes,sommets,normales );
    genere_buffers_phong2si(vbo,vao);
    maj_sommetsVBO();

}

ConeTronc::~ConeTronc()
{

}

void ConeTronc::maj_sommetsVBO()
{
    int j,k;
    j=0;
    float* sommetsBase=&sommets[3*numCotes];
    float* normalesBase=&normales[3*numCotes];
    for(k=0;k<numCotes;k++)
    {
        sommetsVBO[j]=sommets[3*k];
        sommetsVBO[j+1]=sommets[3*k+1];
        sommetsVBO[j+2]=sommets[3*k+2];
        sommetsVBO[j+3]=normales[3*k];
        sommetsVBO[j+4]=normales[3*k+1];
        sommetsVBO[j+5]=normales[3*k+2];
        sommetsVBO[j+6]=sommetsBase[3*k];
        sommetsVBO[j+7]=sommetsBase[3*k+1];
        sommetsVBO[j+8]=sommetsBase[3*k+2];
        sommetsVBO[j+9]=normalesBase[3*k];
        sommetsVBO[j+10]=normalesBase[3*k+1];
        sommetsVBO[j+11]=normalesBase[3*k+2];
        j+=12;
    }
    sommetsVBO[j]=sommets[0];
    sommetsVBO[j+1]=sommets[1];
    sommetsVBO[j+2]=sommets[2];
    sommetsVBO[j+3]=normales[0];
    sommetsVBO[j+4]=normales[1];
    sommetsVBO[j+5]=normales[2];
    sommetsVBO[j+6]=sommetsBase[0];
    sommetsVBO[j+7]=sommetsBase[1];
    sommetsVBO[j+8]=sommetsBase[2];
    sommetsVBO[j+9]=normalesBase[0];
    sommetsVBO[j+10]=normalesBase[1];
    sommetsVBO[j+11]=normalesBase[2];


    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*numSommetsVBO*6,sommetsVBO,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

// Au préalable, il faut que le shader d'affichage soit initialisé.
void ConeTronc::affiche()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP,0,numSommetsVBO);
    glBindVertexArray(0);
}

