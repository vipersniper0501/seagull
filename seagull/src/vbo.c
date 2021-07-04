#include "vbo.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utils.h"

void extractDataFromPLY(void)
{

    //NOTE: Find way to either include python into project (ie ensure that I end
    //up with a single executable) or replace with C text parser  :(
    char *filename = "../../seagull/src/parser.py";

    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        fprintf(stderr, "Failed reading parser.py\n");

    Py_Initialize();
    //Setting Correct Path
    PyObject *sysPath = PySys_GetObject("path");
    PyList_Insert(sysPath, 0, PyUnicode_FromString("../../seagull/src/"));
    PyObject *module_string = PyUnicode_FromString((char*)"parser");
    PyObject *py_module = PyImport_Import(module_string);
    //Calling Function
    PyObject *args = Py_BuildValue("si", "../tmp/cross.ply", 0);
    PyObject *py_function = PyObject_GetAttrString(py_module, (char*)"parsePly");
    /* PyObject *py_result = PyObject_CallObject(py_function, args); */
    PyObject_CallObject(py_function, args);

    //Convert python result into something C can understand
    /* int result = (int)PyLong_AsLong(py_result); */
    /* fprintf(stdout, "RESULT FROM PYTHON: %d", result); */
    Py_Finalize();

}

/*
 * Create a Vertex Buffer Object
 */
void CreateVBO(void)
{

    extractDataFromPLY();


    // NOTE: Could eventually use *.ply file format for loading in data.
    Vertex TriangleVertices[] = {
        //          Vertices                      Colors
        //    X      Y     Z     W         R     G     B     A
        { { -0.8f, -0.8f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { {  0.0f,  0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { {  0.8f, -0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
    };

    Vertex Vertices[] = {
        { { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
        // Top
        { { -0.2f, 0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { 0.2f, 0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
        { { 0.0f, 0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
        { { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        // Bottom
        { { -0.2f, -0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
        { { 0.2f, -0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { 0.0f, -0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
        { { 0.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        // Left
        { { -0.8f, -0.2f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.8f, 0.2f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
        { { -0.8f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
        { { -1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        // Right
        { { 0.8f, -0.2f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
        { { 0.8f, 0.2f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { 0.8f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
        { { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }
    };

    GLubyte Indices[] = {
        // Top
        0, 1, 3,
        0, 3, 2,
        3, 1, 4,
        3, 4, 2,
        // Bottom
        0, 5, 7,
        0, 7, 6,
        7, 5, 8,
        7, 8, 6,
        // Left
        0, 9, 11,
        0, 11, 10,
        11, 9, 12,
        11, 12, 10,
        // Right
        0, 13, 15,
        0, 15, 14,
        15, 13, 16,
        15, 16, 14
    };

    GLubyte AlternateIndices[] = {
        // Outer square border:
        3, 4, 16,
        3, 15, 16,
        15, 16, 8,
        15, 7, 8,
        7, 8, 12,
        7, 11, 12,
        11, 12, 4,
        11, 3, 4,

        // Inner square
        0, 11, 3,
        0, 3, 15,
        0, 15, 7,
        0, 7, 11
    };
    
    const size_t BufferSize = sizeof(Vertices);
    const size_t VertexSize = sizeof(Vertices[0]);
    const size_t RgbOffset  = sizeof(Vertices[0].Position);

    GLCall(glGenVertexArrays(1, &BufferIDs[0]));
    GLCall(glBindVertexArray(BufferIDs[0]));


    GLCall(glGenBuffers(1, &BufferIDs[1]));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, BufferIDs[1]));
    GLCall(glBufferData(GL_ARRAY_BUFFER, BufferSize, Vertices, GL_STATIC_DRAW));

    GLCall(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VertexSize, 0));
    GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)RgbOffset));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));


    GLCall(glGenBuffers(2, IndexBufferID));

    // Cross Object
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID[0]));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW));

    // Diamond/Turned Square Object
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID[1]));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(AlternateIndices), AlternateIndices, GL_STATIC_DRAW));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID[0]));

    GLCall(glBindVertexArray(0));
}

void DestroyVBO(void)
{
    /* GLCall(glDisableVertexAttribArray(1)); */
    /* GLCall(glDisableVertexAttribArray(0)); */

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glDeleteBuffers(1, &BufferIDs[1]));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glDeleteBuffers(2, IndexBufferID));

    GLCall(glBindVertexArray(0));
    GLCall(glDeleteVertexArrays(1, &BufferIDs[0]));
}
