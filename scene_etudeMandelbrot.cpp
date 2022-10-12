#include "scene_etudeMandelbrot.h"

GLuint Fractale2d::vbo_mb=0;
GLuint Fractale2d::vao_mb=0;

double  Fractale2d::xpos=0.0;//0.0297733;
double  Fractale2d::ypos=0.0;//0.0071582;
double  Fractale2d::zoom=500.;//465.3;
float   Fractale2d::zoom_ar=.7;
double  Fractale2d::facteur_zoom=.99;
double  Fractale2d::azoom=1.;
double  Fractale2d::axpos=0.;
double  Fractale2d::aypos=0.;
double  Fractale2d::azpos=0.;
double  Fractale2d::deceleration=0.7;
double  Fractale2d::acceleration=1.5;
int     Fractale2d::iter=100;
float   Fractale2d::alpha=0.;
double  Fractale2d::puissance=2.;

bool    Fractale2d::dep_droite=false;
bool    Fractale2d::dep_gauche=false;
bool    Fractale2d::dep_avant=false;
bool    Fractale2d::dep_arriere=false;
bool    Fractale2d::dep_haut=false;
bool    Fractale2d::dep_bas=false;
bool    Fractale2d::zoom_in=false;
bool    Fractale2d::zoom_out=false;
bool    Fractale2d::rot_CCW=false;
bool    Fractale2d::rot_CW=false;
bool    Fractale2d::iter_plus=false;
bool    Fractale2d::iter_moins=false;
bool    Fractale2d::juliaC_r_dec=false;
bool    Fractale2d::juliaC_r_inc=false;
bool    Fractale2d::juliaC_i_dec=false;
bool    Fractale2d::juliaC_i_inc=false;
bool    Fractale2d::puissance_inc=false;
bool    Fractale2d::puissance_dec=false;
int     Fractale2d::depId=1;

double Fractale2d::point_x=0.;
double Fractale2d::point_y=0.;
double Fractale2d::juliaC[2]{0.3,0.5};

bool Fractale2d::init()
{
    GLfloat sommets_rendu[2*4]{0.,0., 0.,1., 1.,1., 1.,0.};
        glGenBuffers(1,&vbo_mb);
        glGenVertexArrays(1,&vao_mb);

        glBindVertexArray(vao_mb);
            glBindBuffer(GL_ARRAY_BUFFER,vbo_mb);
                glBufferData(GL_ARRAY_BUFFER,sizeof(sommets_rendu),sommets_rendu,GL_STATIC_DRAW);
                glVertexAttribPointer(ShadersDemo::attrib_mb_position,2,GL_FLOAT,GL_FALSE,0,0);
                glEnableVertexAttribArray(ShadersDemo::attrib_mb_position);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
    return true;
}

void Fractale2d::destruction()
{
    if (glIsBuffer(vbo_mb)) glDeleteBuffers(1,&vbo_mb);
    if (glIsVertexArray(vao_mb)) glDeleteVertexArrays(1,&vao_mb);
}

void Fractale2d::clavier(int key,bool etat)
{
            switch (key)
            {

                case GLFW_KEY_LEFT:dep_gauche=etat;break;
                case GLFW_KEY_UP:dep_haut=etat ;break;
                case GLFW_KEY_DOWN:dep_bas=etat;break;
                case GLFW_KEY_RIGHT:dep_droite=etat;break;

                case GLFW_KEY_PAGE_UP:dep_avant=etat ;break;
                case GLFW_KEY_PAGE_DOWN:dep_arriere=etat;break;

                case GLFW_KEY_KP_ADD:zoom_in=etat;break;
                case GLFW_KEY_KP_SUBTRACT:zoom_out=etat;break;
                case GLFW_KEY_A:juliaC_r_dec=etat;break;
                case GLFW_KEY_Z:juliaC_r_inc=etat;break;
                case GLFW_KEY_Q:juliaC_i_dec=etat;break;
                case GLFW_KEY_S:juliaC_i_inc=etat;break;
                case GLFW_KEY_KP_7:iter_moins=etat;break;
                case GLFW_KEY_KP_9:iter_plus=etat;break;
                case GLFW_KEY_KP_4:rot_CCW=etat;break;
                case GLFW_KEY_KP_6:rot_CW=etat;break;
                case GLFW_KEY_KP_1:puissance_dec=etat;break;
                case GLFW_KEY_KP_3:puissance_inc=etat;break;

                case GLFW_KEY_F1:depId=1;break;
                case GLFW_KEY_F2:depId=2;break;
                case GLFW_KEY_F3:depId=3;break;
                case GLFW_KEY_F4:depId=4;break;
                case GLFW_KEY_F5:depId=5;break;
            }

}

void Fractale2d::maj_eclairages()
{
}

void Fractale2d::maj_mouvements()
{
       //Déplacement fractale:

        axpos*=deceleration;
        aypos*=deceleration;
        azpos*=deceleration;

        if(dep_droite) {axpos=axpos/deceleration+acceleration;}
        if(dep_gauche){axpos=axpos/deceleration-acceleration;}
        if(dep_haut){aypos=aypos/deceleration+acceleration;}
        if(dep_bas){aypos=aypos/deceleration-acceleration;}
        if(dep_avant)azpos=azpos/deceleration+acceleration;
        if(dep_arriere)azpos=azpos/deceleration-acceleration;

    if(depId==1)
    {

        double c=cos(alpha);
        double s=sin(alpha);

        double depx=axpos*c-aypos*s;
        double depy=axpos*s+aypos*c;

        double echelle=pow(facteur_zoom,zoom);

        xpos+=depx*echelle;
        ypos+=depy*echelle;

        if(zoom_in){zoom+=5.;zoom_ar+=facteur_zoom/2000.;}
        else if(zoom_out)
            {
                zoom-=5.;
                //if(zoom<0.25)zoom=0.25;
                zoom_ar-=facteur_zoom/2000.;
            }
        if(rot_CCW)alpha+=0.01;
        else if (rot_CW)alpha-=0.01;
        if(iter_plus)iter++;
        else if (iter_moins){iter--;if (iter<1)iter=1;}

        double vJuliaC=0.001;
        if(juliaC_r_dec)juliaC[0]-=vJuliaC;
        else if (juliaC_r_inc)juliaC[0]+=vJuliaC;
        if(juliaC_i_dec)juliaC[1]-=vJuliaC;
        else if (juliaC_i_inc)juliaC[1]+=vJuliaC;

        if(puissance_dec)puissance-=0.01;
        else if (puissance_inc)puissance+=0.01;

         if(GVars::disp_donnees)
                {
                    cout<<"xpos:"<<xpos<<" ypos:"<<ypos<<" zoom:"<<zoom<<" Angle:"<<alpha*180./M_PI<<endl;
                    GVars::disp_donnees=false;
                }
    }

    else if(depId==2)
    {

        double c=cos(alpha);
        double s=sin(alpha);

        double depx=axpos*c-aypos*s;
        double depy=axpos*s+aypos*c;

        double echelle=pow(facteur_zoom,zoom);

        point_x+=depx*echelle;
        point_y+=depy*echelle;

         if(GVars::disp_donnees)
                {
                    cout<<"point_x:"<<point_x<<" point_y:"<<point_y<<endl;
                    GVars::disp_donnees=false;
                }
    }

}

void Fractale2d::convertion_coords_mb(double* pt,float* pt_ecran)
{
    double c=cos(-alpha);
    double s=sin(-alpha);
    double ex=(pt[0]-xpos)/pow(facteur_zoom,zoom);
    double ey=(pt[1]-ypos)/pow(facteur_zoom,zoom);
    double xr=(ex*c-ey*s)*2./double(RESOLUTION_X_TRAVAIL);
    double yr=(ex*s+ey*c)*2./double(RESOLUTION_Y_TRAVAIL);
    pt_ecran[0]=float(xr);
    pt_ecran[1]=float(yr);

}

void Fractale2d::affiche()
{
    glm::mat4 projection=glm::ortho(0.,1.,0.,1.,.5,10.);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glUseProgram(ShadersDemo::programme_mb);
	glUniformMatrix4fv(ShadersDemo::uniform_mb_matrice_projection,1,GL_FALSE,&projection[0][0]);
	glUniform1d(ShadersDemo::uniform_mb_zoom,pow(facteur_zoom,zoom));
	glUniform1d(ShadersDemo::uniform_mb_xpos,xpos);
	glUniform1d(ShadersDemo::uniform_mb_ypos,ypos);
	glUniform1d(ShadersDemo::uniform_mb_puissance,puissance);
	glUniform1f(ShadersDemo::uniform_mb_alpha,alpha);
	glUniform1i(ShadersDemo::uniform_mb_iter,iter);
	glUniform2f(ShadersDemo::uniform_mb_ecranDim,float(RESOLUTION_X_TRAVAIL),float(RESOLUTION_Y_TRAVAIL));
	glUniform2d(ShadersDemo::uniform_mb_juliaC,juliaC[0],juliaC[1]);

	glBindVertexArray(vao_mb);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    glUseProgram(0);


    #define NBRITR 100

    double pt[NBRITR*2];
    pt[0]=point_x;
    pt[1]=point_y;
    float pt_ecran[NBRITR*2];


    double Zr=0.;
    double Zi=0.;
    double tmp;
    int i;
    for(i=0;i<NBRITR;i++)
    {
        //Mandelbrot 2:

        tmp=Zr*Zr-Zi*Zi+pt[0];
        Zi=2.*Zr*Zi+pt[1];
        Zr=tmp;


        //Mandelbrot 3:
        /*
        tmp=Zr*Zr*Zr-Zi*Zi*3*Zr+pt[0];
        Zi=3.*Zr*Zr*Zi-Zi*Zi*Zi+pt[1];
        Zr=tmp;
        */

        //Mandelbrot pow:
        /*
                double rho,theta;

                rho=sqrt(Zr*Zr+Zi*Zi);
                if(rho==0.) theta=0.;
                else
                {
                    theta=acos(Zr/rho);
                    if(Zi<0.)theta=-theta;
                }

                rho=pow(rho,puissance);
                theta*=puissance;

                Zr=rho*cos(theta)+pt[0];
                Zi=rho*sin(theta)+pt[1];

        */
        pt[i*2]=Zr;
        pt[i*2+1]=Zi;
        convertion_coords_mb(&pt[i*2],&pt_ecran[i*2]);
    }
    /*

    double Zr=pt[0];
    double Zi=pt[1];
    double tmp;
    int i;
    for(i=0;i<NBRITR;i++)
    {
        pt[i*2]=Zr;
        pt[i*2+1]=Zi;
        tmp=Zr*Zr-Zi*Zi+juliaC[0];
        Zi=2.*Zr*Zi+juliaC[1];
        Zr=tmp;
        convertion_coords_mb(&pt[i*2],&pt_ecran[i*2]);
    }
    */



    glLineWidth(1.);

    glColor3f(1.,1.,0.);
    glBegin(GL_LINE_STRIP);

        for(i=0;i<NBRITR;i++)glVertex3f(pt_ecran[i*2],pt_ecran[i*2+1],-1.);

    glEnd();



     glPointSize(6.);
     glColor3f(1.,0.,0.);
    glBegin(GL_POINTS);

        glVertex3f(pt_ecran[0],pt_ecran[1],-1.);

    glEnd();

    glPointSize(3.);
     glColor3f(1.,1.,0.);
    glBegin(GL_POINTS);

        for(i=1;i<NBRITR;i++)glVertex3f(pt_ecran[i*2],pt_ecran[i*2+1],-1.);

    glEnd();


}
