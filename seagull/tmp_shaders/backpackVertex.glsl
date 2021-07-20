#version 400

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_TexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(void)
{
    FragPos = vec3(ModelMatrix * vec4(in_Position, 1.0));
    TexCoords = in_TexCoords;
    Normal = mat3(transpose(inverse(ModelMatrix))) * in_Normal;

    gl_Position = ProjectionMatrix * ViewMatrix * vec4(FragPos, 1.0);
}
