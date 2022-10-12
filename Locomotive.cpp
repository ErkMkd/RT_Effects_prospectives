#include "Locomotive.h"


Locomotive::Locomotive()
{
    erreur=LOCO_OK;
    planTest=NULL;
    plan2cTest=NULL;
    diskTest=NULL;
    disk2cTest=NULL;
    cubeTest=NULL;
    cylindreTest=NULL;
    cylindreFermeTest=NULL;
    sphereTest=NULL;
    sphereTessTest=NULL;
    toreTest=NULL;
    coneTest=NULL;

    planTest=new Plan(.5,1.);
    plan2cTest=new Plan2Cotes(1.8,.25);
    diskTest=new Disque(.5,80);
    disk2cTest=new Disque2Cotes(.5,70);
    cubeTest=new Cube(1.,1.,1.);
    cylindreTest=new Cylindre(.2,1.,8,4);
    cylindreFermeTest=new CylindreFerme(.2,1.,8,4);
    sphereTest=new Sphere(.5,24,12);
    sphereTessTest=new SphereTesselation(.5,3);
    toreTest=new Tore(0.4,0.2,16,24);
    coneTest=new Cone(0.5,1.,12);
    coneTroncTest=new ConeTronc(0.2,0.5,1.,12);
}

Locomotive::~Locomotive()
{
    if(planTest!=NULL) delete planTest;
    if(plan2cTest!=NULL) delete plan2cTest;
    if(diskTest!=NULL) delete diskTest;
    if(disk2cTest!=NULL) delete disk2cTest;
    if(cubeTest!=NULL) delete cubeTest;
    if(cylindreTest!=NULL) delete cylindreTest;
    if(cylindreFermeTest!=NULL) delete cylindreFermeTest;
    if(sphereTest!=NULL) delete sphereTest;
    if(sphereTessTest!=NULL) delete sphereTessTest;
    if(toreTest!=NULL) delete toreTest;
    if(coneTest!=NULL) delete coneTest;
    if(coneTroncTest!=NULL) delete coneTroncTest;
}

void Locomotive::maj_matrices_primitives()
{
    for(int i=0;i<LOCO_NUM_PRIMITIVES;i++)
    {
        float f=-(float(i & 0x1)-0.5)*2*(2.+cosf(float(i)*0.1));
        glm::mat4 ms=glm::scale(glm::mat4(1.),glm::vec3(.1,.1,.1));
        glm::mat4 m=glm::translate(ms,glm::vec3(-1.2*i,0.,0.));
        glm::mat4 rm=glm::rotate(m,float(.1*f*GVars::temps),glm::vec3(0.,0.,1.));
        glm::mat4 tm=glm::rotate(rm,float(0.15*f*GVars::temps),glm::vec3(1.,0.,0.));
        matrices_primitives[i]=glm::rotate(tm,float(0.25*f*GVars::temps),glm::vec3(0.,1.,0.));
        matrices_normales_primitives[i]=glm::inverseTranspose(glm::mat3x3(matrices_primitives[i]));
    }
}

void Locomotive::affiche()
{
    maj_matrices_primitives();
    Primitives::init_materiau(0xffffffff,0x000000ff,0xff0000ff,0xffffffff,40.);
    Primitives::init_phong2si();

    glEnable(GL_CULL_FACE);
    glUniformMatrix3fv(ShadersPrimitives::prim_phong2si_matrice_normale_instance,1,GL_FALSE,&(matrices_normales_primitives[0])[0][0]);
    glUniformMatrix4fv(ShadersPrimitives::prim_phong2si_matrice_instance,1,GL_FALSE,&(matrices_primitives[0])[0][0]);
    planTest->affiche();
    glUniformMatrix3fv(ShadersPrimitives::prim_phong2si_matrice_normale_instance,1,GL_FALSE,&(matrices_normales_primitives[1])[0][0]);
    glUniformMatrix4fv(ShadersPrimitives::prim_phong2si_matrice_instance,1,GL_FALSE,&(matrices_primitives[1])[0][0]);
    plan2cTest->affiche();
    glUniformMatrix3fv(ShadersPrimitives::prim_phong2si_matrice_normale_instance,1,GL_FALSE,&(matrices_normales_primitives[2])[0][0]);
    glUniformMatrix4fv(ShadersPrimitives::prim_phong2si_matrice_instance,1,GL_FALSE,&(matrices_primitives[2])[0][0]);
    diskTest->affiche();
    glUniformMatrix3fv(ShadersPrimitives::prim_phong2si_matrice_normale_instance,1,GL_FALSE,&(matrices_normales_primitives[3])[0][0]);
    glUniformMatrix4fv(ShadersPrimitives::prim_phong2si_matrice_instance,1,GL_FALSE,&(matrices_primitives[3])[0][0]);
    disk2cTest->affiche();
    glUniformMatrix3fv(ShadersPrimitives::prim_phong2si_matrice_normale_instance,1,GL_FALSE,&(matrices_normales_primitives[4])[0][0]);
    glUniformMatrix4fv(ShadersPrimitives::prim_phong2si_matrice_instance,1,GL_FALSE,&(matrices_primitives[4])[0][0]);
    cubeTest->affiche();
    glUniformMatrix3fv(ShadersPrimitives::prim_phong2si_matrice_normale_instance,1,GL_FALSE,&(matrices_normales_primitives[5])[0][0]);
    glUniformMatrix4fv(ShadersPrimitives::prim_phong2si_matrice_instance,1,GL_FALSE,&(matrices_primitives[5])[0][0]);
    cylindreTest->affiche();
    glUniformMatrix3fv(ShadersPrimitives::prim_phong2si_matrice_normale_instance,1,GL_FALSE,&(matrices_normales_primitives[6])[0][0]);
    glUniformMatrix4fv(ShadersPrimitives::prim_phong2si_matrice_instance,1,GL_FALSE,&(matrices_primitives[6])[0][0]);
    cylindreFermeTest->affiche();
    glUniformMatrix3fv(ShadersPrimitives::prim_phong2si_matrice_normale_instance,1,GL_FALSE,&(matrices_normales_primitives[7])[0][0]);
    glUniformMatrix4fv(ShadersPrimitives::prim_phong2si_matrice_instance,1,GL_FALSE,&(matrices_primitives[7])[0][0]);
    sphereTest->affiche();
    glUniformMatrix3fv(ShadersPrimitives::prim_phong2si_matrice_normale_instance,1,GL_FALSE,&(matrices_normales_primitives[8])[0][0]);
    glUniformMatrix4fv(ShadersPrimitives::prim_phong2si_matrice_instance,1,GL_FALSE,&(matrices_primitives[8])[0][0]);
    sphereTessTest->affiche();
    glUniformMatrix3fv(ShadersPrimitives::prim_phong2si_matrice_normale_instance,1,GL_FALSE,&(matrices_normales_primitives[9])[0][0]);
    glUniformMatrix4fv(ShadersPrimitives::prim_phong2si_matrice_instance,1,GL_FALSE,&(matrices_primitives[9])[0][0]);
    toreTest->affiche();
    glUniformMatrix3fv(ShadersPrimitives::prim_phong2si_matrice_normale_instance,1,GL_FALSE,&(matrices_normales_primitives[10])[0][0]);
    glUniformMatrix4fv(ShadersPrimitives::prim_phong2si_matrice_instance,1,GL_FALSE,&(matrices_primitives[10])[0][0]);
    coneTest->affiche();
    glUniformMatrix3fv(ShadersPrimitives::prim_phong2si_matrice_normale_instance,1,GL_FALSE,&(matrices_normales_primitives[11])[0][0]);
    glUniformMatrix4fv(ShadersPrimitives::prim_phong2si_matrice_instance,1,GL_FALSE,&(matrices_primitives[11])[0][0]);
    coneTroncTest->affiche();
    glUseProgram(0);
}
