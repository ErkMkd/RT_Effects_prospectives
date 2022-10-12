#include "Source_Lumineuse.h"


    Source_Lumineuse::Source_Lumineuse()
    {
        determine_couleur(1.,1.,1.,1.);
        determine_ambiance(0.,0.,0.1,1.);
        intensite=1.;
        ouverture=45.;
        fondu=1.;
    }

    Source_Lumineuse::~Source_Lumineuse()
    {

    }

    void Source_Lumineuse::determine_couleur(float r, float v, float b, float a)
    {
        couleur[0]=r;couleur[1]=v;couleur[2]=b;couleur[3]=a;
    }

    void Source_Lumineuse::determine_couleur(uint32_t rvba)
    {
        couleur[0]=float(rvba>>24)/255.;
        couleur[1]=float((rvba>>16)&0xff)/255.;
        couleur[2]=float((rvba>>8)&0xff)/255.;
        couleur[3]=float(rvba&0xff)/255.;
    }

    void Source_Lumineuse::determine_ambiance(float r, float v, float b, float a)
    {
        ambiance[0]=r;ambiance[1]=v;ambiance[2]=b;ambiance[3]=a;
    }

    void Source_Lumineuse::determine_ambiance(uint32_t rvba)
    {
        ambiance[0]=float(rvba>>24)/255.;
        ambiance[1]=float((rvba>>16)&0xff)/255.;
        ambiance[2]=float((rvba>>8)&0xff)/255.;
        ambiance[3]=float(rvba&0xff)/255.;
    }

    vec3f* Source_Lumineuse::renvoie_direction() {return &axe_z;}
