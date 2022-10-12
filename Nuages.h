#ifndef NUAGES_H_INCLUDED
#define NUAGES_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include "math.h"
#include "Bruit.h"
#include "Contexte3d.h"
//#include "Fonctions.h"
#include "ShadersDemo.h"
#include "repere.h"
#include "Variables.h"

#define NUAGES_OK 0
#define NUAGES_MEMOIRE 1
#define NUAGES_TEXTURES 2

#define NUAGES_NUM_BM 3 /// Attention à modifier le shader Nuages si modif de ce chiffre.

class Nuages : public Repere
{
    public:
        GLuint vbo;
        GLuint vao;
        uint16_t erreur;
        float* bitmaps_nuages[NUAGES_NUM_BM];
        int taille_textures;
        GLuint textures_id[NUAGES_NUM_BM];
        float altitude;

        Nuages(int p_taille_textures);
        ~Nuages();
        void affiche(Contexte3d* c3d);
};

#endif // NUAGES_H_INCLUDED
