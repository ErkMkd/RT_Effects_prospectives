#version 330

layout (location=0) out vec4 outColor;
in vec2 position_texCoords;
uniform sampler2D texture_couleur;

void main()
{
    outColor = texture(texture_couleur,position_texCoords);
}
