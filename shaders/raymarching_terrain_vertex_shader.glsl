#version 400

in vec2 position;

uniform float ratio_ecran;      //Rapport hauteur/largeur
uniform float distanceFocale;

out vec3 rayObs_dir;

void main( void )
{
    gl_Position = vec4(position,0., 1.0);
    rayObs_dir=vec3(position,0.)*vec3(0.5,ratio_ecran/2.,0.)+vec3(0.,0.,-distanceFocale);
}
