#version 400

in vec4 ex_Color;
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D texture_diffuse1;

void main(void)
{
    // FragColor = ex_Color;
    FragColor = texture(texture_diffuse1, TexCoords);
}
