#ifndef SOURCE_LUMINEUSE_H_INCLUDED
#define SOURCE_LUMINEUSE_H_INCLUDED

#include "../repere.h"
#include "../vec3f.h"

class Source_Lumineuse :public Repere
{
    public:
        float couleur[4];
        float ambiance[4];
        float intensite;
        float ouverture;
        float fondu;

    public:
        Source_Lumineuse();
        ~Source_Lumineuse();
        void determine_couleur(float r, float v, float b, float a);
        void determine_couleur(uint32_t rvba);
        void determine_ambiance(float r, float v, float b, float a);
        void determine_ambiance(uint32_t rvba);
        vec3f* renvoie_direction();
};

#endif // SOURCE_LUMINEUSE_H_INCLUDED
