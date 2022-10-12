#ifndef OPENGLMATRIX_H_INCLUDED
#define OPENGLMATRIX_H_INCLUDED

#include "Matrice4x4.h"

class OpenGlMatrixf : public Matrice4x4f
{
    public:
        Matrice4x4f projectionMatrice;
        Matrice4x4f ModelViewMatrice;
        float distanceFocale;
    public:
        OpenGlMatrixf();
        ~OpenGlMatrixf();
        void frustum(float l, float r, float b, float t, float n, float f);
        void ortho(float l,float r,float b,float t,float n,float f);
};
#endif // OPENGLMATRIX_H_INCLUDED
