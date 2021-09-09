#version 400

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;


in VS_OUT_GEO {
    vec3 normals;
} gs_in[];

const float MAGNITUDE = 0.02;

uniform mat4 ProjectionMatrix;

// find a way to cull/omit lines that are hidden from view
void GenerateLine(int index)
{
    gl_Position = ProjectionMatrix * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = ProjectionMatrix * (gl_in[index].gl_Position +
                                      vec4(gs_in[index].normals, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}
void main(void)
{
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);

}