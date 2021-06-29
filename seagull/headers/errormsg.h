#ifndef ERRORMSG_HEADER_FILE
#define ERRORMSG_HEADER_FILE

#include <stdio.h>
#include <stdbool.h>

#define ASSERT(x) if(!x) exit(-1);
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError(void);
bool GLLogCall(const char *function, const char *file, int line);

#endif
