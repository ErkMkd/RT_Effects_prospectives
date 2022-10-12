#ifndef VEC3F_HH_INCLUDED
#define VEC3F_HH_INCLUDED

#include "math.h"


class vec3f
{
    public:
        float x;
        float y;
        float z;

        static float produit_scalaire(vec3f& v1,vec3f& v2);

        vec3f(float px=0., float py=0.,float pz=0.);
        void init(float px,float py,float pz);
        void init(float* xyz);
        void normalise();
        void soustraction(vec3f& v1, vec3f& v2);
        void addition(vec3f& v1, vec3f& v2);
        void produit_vectoriel(vec3f v1,vec3f v2);
        float renvoie_norme();
};




#endif // VEC3F_HH_INCLUDED
