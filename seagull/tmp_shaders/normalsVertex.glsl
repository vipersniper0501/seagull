#version 400
layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;

out VS_OUT_GEO {
    vec3 normals;
} vs_out_geo;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform vec3 viewPos;


void main(void)
{
    mat3 NormalMatrix = mat3(transpose(inverse(ViewMatrix * ModelMatrix)));
    vs_out_geo.normals = vec3(vec4(NormalMatrix * in_Normal, 0.0));

    gl_Position = ViewMatrix * ModelMatrix * vec4(in_Position, 1.0);
}