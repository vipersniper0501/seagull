#version 400

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_TexCoords;
layout(location=3) in vec3 in_Tangents;
layout(location=4) in vec3 in_Bitangents;

#define MAX_NUMBER_LIGHTS 32

struct Light {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // attenuation
    float constant;
    float linear;
    float quadratic;
};

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    // vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    // Light Lights[4];
} vs_out;

// out vec2 TexCoords;
// out vec3 Normal;
// out vec3 FragPos;

out mat3 TBN;


// uniform Light lighting[64];
// uniform int nrLights;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform vec3 viewPos;

void main(void)
{

    // for (int i = 0; i < 8; i++)
    // {
    //     vs_out.Lights[i] = lighting[i];
    // }

    vs_out.FragPos = vec3(ModelMatrix * vec4(in_Position, 1.0));
    vs_out.TexCoords = in_TexCoords;

    mat3 NormalMatrix = transpose(inverse(mat3(ModelMatrix)));

    vec3 T = normalize(NormalMatrix * in_Tangents);
    vec3 N = normalize(NormalMatrix * in_Normal);

    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    TBN = transpose(mat3(T, B, N));
    // for (int i = 0; i < 8; i++)
    // {
    //     // vs_out.TangentLightPos = TBN * lightPos[i];
    //     // assigning light position with calculated Tangent Light Position
    //     vs_out.Lights[i].position = TBN * lightPos[i];
    // }
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;

    gl_Position = ProjectionMatrix * ViewMatrix * vec4(vs_out.FragPos, 1.0);
}
