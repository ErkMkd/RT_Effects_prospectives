#ifndef LOCOMOTIVE_H_INCLUDED
#define LOCOMOTIVE_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include "math.h"
#include "Primitives/Primitives.h"
#include "repere.h"
#include "Variables.h"

#define LOCO_NUM_COTES_DISQUE 32

#define LOCO_NUM_SECTIONS_CYL 2
#define LOCO_NUM_COTES_CYL 8
#define LOCO_NUM_POINTS_CYL LOCO_NUM_SECTIONS_CYL*LOCO_NUM_COTES_CYL
#define LOCO_NUM_FACES_CYL (LOCO_NUM_SECTIONS_CYL-1)*LOCO_NUM_COTES_CYL+2

#define LOCO_NUM_COTES_SPHERE 16
#define LOCO_NUM_SEGMENTS_SPHERE 16
#define LOCO_NUM_POINTS_SPHERE LOCO_NUM_COTES_SPHERE*(LOCO_NUM_SEGMENTS_SPHERE-1)+2
#define LOCO_NUM_FACES_SPHERE LOCO_NUM_SEGMENTS_SPHERE*LOCO_NUM_COTES_SPHERE

#define LOCO_NUM_COTES_TORE 12
#define LOCO_NUM_SECTIONS_TORE 32
#define LOCO_NUM_POINTS_TORE LOCO_NUM_COTES_TORE*LOCO_NUM_SECTIONS_TORE
#define LOCO_NUM_FACES_TORE LOCO_NUM_POINTS_TORE

//Erreurs:
#define LOCO_OK 0

#define LOCO_NUM_PRIMITIVES 12

class Locomotive : public Repere
{
    private:

    public:
        glm::mat4 matrices_primitives[LOCO_NUM_PRIMITIVES];
        glm::mat3 matrices_normales_primitives[LOCO_NUM_PRIMITIVES];
        uint16_t erreur;
        Plan* planTest;
        Plan2Cotes* plan2cTest;
        Disque* diskTest;
        Disque2Cotes* disk2cTest;
        Cube* cubeTest;
        Cylindre* cylindreTest;
        CylindreFerme* cylindreFermeTest;
        Sphere* sphereTest;
        SphereTesselation* sphereTessTest;
        Tore* toreTest;
        Cone* coneTest;
        ConeTronc* coneTroncTest;

    public:

        Locomotive();
        ~Locomotive();

        void maj_matrices_primitives();

        void affiche();

};

#endif // LOCOMOTIVE_H_INCLUDED
