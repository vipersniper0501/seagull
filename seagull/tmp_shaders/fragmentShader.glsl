#version 400

in vec4 ex_Color;
out vec4 out_Color;

void main(void)
{
    out_Color = ex_Color;
    // out_Color = vec4(0.5, 0, 0.8, 1.0);
}
