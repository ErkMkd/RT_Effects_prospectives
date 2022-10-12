#ifndef IMAGE_PNG_H_INCLUDED
#define IMAGE_PNG_H_INCLUDED

#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "libpng12/png.h"

#include "Image_infos.h"

using namespace std;

#define DEBUG_IMAGE_PNG false

class Image_PNG
{
    public:

    static Image_infos* charge(const char *nom_fichier);
};

#endif // IMAGE_PNG_H_INCLUDED
