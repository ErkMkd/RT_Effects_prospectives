#version 330

in vec2 position;
out vec2 position_texCoords;

uniform float ratio_ecran;
uniform float distance_focale;

varying vec3 rayObs_dir;

void main()
{
    position_texCoords=position/2.+vec2(.5,.5);
    gl_Position = vec4(position,0.0, 1.0);
    rayObs_dir=vec3(position,0.)*vec3(0.5,ratio_ecran/2.,0.)+vec3(0.,0.,distance_focale);   //Valeur positive ici !
}
