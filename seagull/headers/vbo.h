#ifndef VBO_HEADER_FILE
#define VBO_HEADER_FILE

#include <GL/glew.h>


GLuint
  IndexBufferID[2],
  ActiveIndexBuffer;

GLuint BufferIDs[3];

void CreateVBO(void);
void DestroyVBO(void);

#endif
