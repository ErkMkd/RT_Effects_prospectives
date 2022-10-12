#include "OpenGlMatrix.h"

    OpenGlMatrixf::OpenGlMatrixf()
    {
        projectionMatrice.identite();
        ModelViewMatrice.identite();
    }

    OpenGlMatrixf::~OpenGlMatrixf()
    {

    }

    void OpenGlMatrixf::frustum(float l, float r, float b, float t, float n, float f)
    {
        projectionMatrice.zero();
        projectionMatrice.matrice[0]=2.*n/(r-l);
        projectionMatrice.matrice[5]=2.*n/(t-b);
        projectionMatrice.matrice[8]=(r+l)*(r-l);
        projectionMatrice.matrice[9]=(t+b)/(t-b);
        projectionMatrice.matrice[10]=-(f+n)/(f-n);
        projectionMatrice.matrice[11]=-1.;
        projectionMatrice.matrice[14]=-2*f*n/(f-n);
    }

    void OpenGlMatrixf::ortho(float l, float r, float b, float t, float n, float f)
    {
        projectionMatrice.zero();
        projectionMatrice.matrice[0]=2./(r-l);
        projectionMatrice.matrice[5]=2./(t-b);
        projectionMatrice.matrice[10]=-2/(f-n);
        projectionMatrice.matrice[12]=-(r+l)/(r-l);
        projectionMatrice.matrice[13]=-(t+b)/(t-b);
        projectionMatrice.matrice[14]=-(f+n)/(f-n);
        projectionMatrice.matrice[15]=1.;
    }
