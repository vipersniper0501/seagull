#ifndef UTILS_HEADER_FILE
#define UTILS_HEADER_FILE

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <GL/glew.h>


static const double PI = 3.14159265358979323846;

typedef struct
{
    float Position[4];
    float Color[4];
} Vertex;

typedef struct Matrix
{
    float m[16];
} Matrix;

extern const Matrix IDENTITY_MATRIX;

float Cotangent(float angle);
float DegreesToRadians(float degrees);
float RadiansToDegrees(float radians);

Matrix MultiplyMatrices(const Matrix *m1, const Matrix *m2);
void RotateAboutX(Matrix *m, float angle);
void RotateAboutY(Matrix *m, float angle);
void RotateAboutZ(Matrix *m, float angle);
void ScaleMatrix(Matrix *m, float x, float y, float z);
void TranslateMatrix(Matrix *m, float x, float y, float z);

Matrix CreateProjectionMatrix(
    float fovy,
    float aspect_ratio,
    float near_plane,
    float far_plane
);

#define ASSERT(x) if(!x) exit(-1);
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError(void);
bool GLLogCall(const char *function, const char *file, int line);

#endif
