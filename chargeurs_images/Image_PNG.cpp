#include "Image_PNG.h"

Image_infos* Image_PNG::charge(const char *nom_fichier)
{
    //--- Ouverture du fichier:

        FILE* fichier;
        if ((fichier=fopen(nom_fichier,"rb"))==NULL)
        {
            cout<<"Erreur dans Image_PNG::charge() - Impossible de charger le fichier ' "<<" '"<<endl;
            return NULL;
        }

    //--- Création des structures de traitement:

        png_structp png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, (png_voidp)NULL,NULL, NULL);
        if (png_ptr==NULL)
        {
            cout<<"Impossible de créer la structure png_structp"<<endl;
            fclose(fichier);
            return NULL;
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr==NULL)
        {
            png_destroy_read_struct(&png_ptr,png_infopp_NULL, png_infopp_NULL);
            fclose(fichier);
            cout<<"Impossible de créer la structure png_infop - info_ptr"<<endl;
            return NULL;
        }

        if (setjmp(png_jmpbuf(png_ptr)))
        {
          /* Free all of the memory associated with the png_ptr and info_ptr */
          png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
          fclose(fichier);
          /* If we get here, we had a problem reading the file */
          cout<<"!!! ERREUR !!! Impossible de lire le fichier PNG: "<<nom_fichier<<endl;
          return NULL;
        }


    //--- Initialisation de la lecture:
        #if DEBUG_IMAGE_PNG
        cout<<"Décompression ficier PNG: "<<nom_fichier<<endl;
        #endif
        png_init_io(png_ptr,fichier);

    //--- Lecture:
        Image_infos* parametres=new Image_infos();
        png_uint_32 width, height;
        int bit_depth, color_type, interlace_type, number_of_passes;

        png_read_info(png_ptr, info_ptr);
        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,&interlace_type, int_p_NULL, int_p_NULL);
        #if DEBUG_IMAGE_PNG
        cout<<"*** Avant transformation:"<<endl;
        cout<<"largeur:"<<width<<endl;
        cout<<"hauteur:"<<height<<endl;
        cout<<"profondeur:"<<bit_depth<<endl;
        cout<<"type couleur:"<<color_type<<endl;
        cout<<"type entrelace:"<<interlace_type<<endl;
        #endif

        //Les pixels de moins de 8 bits (plusieurs pixels par octets) sont étendus à 8bits (1 pixel par octet):
        png_set_packing(png_ptr);

        //Couleurs indexées converties en RGB
        if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);
        //Images en niveaux de gris <8bits étendues à 8bits/pixel
        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_gray_1_2_4_to_8(png_ptr);
        //Couleurs indexées ou RGB avec transparence convertie en RGBA:
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);

        //Le gestionnaire d'entrelacement doit être initialisé:
        number_of_passes = png_set_interlace_handling(png_ptr);

        //Mise à jour des paramètres de l'image:
        png_read_update_info(png_ptr, info_ptr);
        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,&interlace_type, int_p_NULL, int_p_NULL);
        #if DEBUG_IMAGE_PNG
        cout<<"*** Après transformation:"<<endl;
        cout<<"largeur:"<<width<<endl;
        cout<<"hauteur:"<<height<<endl;
        cout<<"profondeur:"<<bit_depth<<endl;
        cout<<"type couleur:"<<color_type<<endl;
        cout<<"type entrelace:"<<interlace_type<<endl;
        #endif

        //Initialisation du block d'information:
        parametres->largeur=width;
        parametres->hauteur=height;
        switch(color_type)
        {
            case PNG_COLOR_TYPE_GRAY: parametres->octets_par_pixel=1;break;
            case PNG_COLOR_TYPE_GRAY_ALPHA: parametres->octets_par_pixel=2;break;
            case PNG_COLOR_TYPE_RGB: parametres->octets_par_pixel=3;break;
            case PNG_COLOR_TYPE_RGB_ALPHA: parametres->octets_par_pixel=4;break;
            default: parametres->octets_par_pixel=4;break;
        }
        parametres->bits_par_pixel=parametres->octets_par_pixel*8;
        //Allocation:
        parametres->image=(uint8_t*)malloc(parametres->octets_par_pixel*parametres->largeur*parametres->hauteur);
        if (parametres->image==NULL)
        {
            cout<<"Erreur dans Image_PNG::charge() - Mémoire insuffisante pour l'image ' "<<nom_fichier<<" '"<<endl;
            png_destroy_read_struct(&png_ptr, &info_ptr,png_infopp_NULL);
            fclose(fichier);
            delete parametres;
            return NULL;
        }
        png_bytep row_pointers[height];

        for (int row = 0; row < height; row++)
        {
          row_pointers[row] = parametres->image+(row*parametres->octets_par_pixel*int(width));
        }
        //Lecture:
        png_read_image(png_ptr, row_pointers);
        png_read_end(png_ptr, info_ptr);

    //--- Destruction des données de lecture:
        png_destroy_read_struct(&png_ptr, &info_ptr,png_infopp_NULL);
        fclose(fichier);
    #if DEBUG_IMAGE_PNG
    cout<<"Fichier PNG décompressé avec succès :)"<<endl;
    #endif // DEBUG_IMAGE_PNG
    return parametres;
}
