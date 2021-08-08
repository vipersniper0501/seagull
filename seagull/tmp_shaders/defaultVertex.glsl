#version 400

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_TexCoords;
layout(location=3) in vec3 in_Tangents;
layout(location=4) in vec3 in_Bitangents;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

// out vec2 TexCoords;
// out vec3 Normal;
// out vec3 FragPos;

// out mat3 TBN;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main(void)
{
    vs_out.FragPos = vec3(ModelMatrix * vec4(in_Position, 1.0));
    vs_out.TexCoords = in_TexCoords;

    mat3 NormalMatrix = transpose(inverse(mat3(ModelMatrix)));

    vec3 T = normalize(NormalMatrix * in_Tangents);
    vec3 N = normalize(NormalMatrix * in_Normal);

    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;

    gl_Position = ProjectionMatrix * ViewMatrix * vec4(vs_out.FragPos, 1.0);
}
