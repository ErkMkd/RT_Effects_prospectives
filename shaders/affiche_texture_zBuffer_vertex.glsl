#version 330

in vec2 position;
out vec2 position_texCoords;

void main()
{
    position_texCoords=position/2.+vec2(.5,.5);
    gl_Position = vec4(position,0.0, 1.0);
}
