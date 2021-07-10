#include "Utils.h"

int SubStrFind(char *text, char *search)
{
    int
        text_size = sizeof(&text)/sizeof(char) - 1,
        search_size = sizeof(&search)/sizeof(char) - 1,
        pos_search = 0,
        pos_text = 0;

    for (pos_text = 0; pos_text < text_size - search_size;++pos_text)
    {
        if(text[pos_text] == search[pos_search])
        {
            ++pos_search;
            if(pos_search == search_size)
            {
                // match
                printf("match from %d to %d\n",pos_text-search_size,pos_text);
                return 1;
            }
        }
        else
        {
           pos_text -=pos_search;
           pos_search = 0;
        }
    }
    // no match
    fprintf(stdout, "SUBSTRING WARNING: No match for %s was found in string: \n%s\n", search, text);
    return 0;
}


void GLClearError(void)
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char *function, const char *file, int line)
{
    GLenum error = glGetError();
    while (error) {
        fprintf(stdout, "[OpenGL Error] (%d): %s %s: %d\n", error, function, file, line);
        return false;
    }
    return true;
}

/*
 * Fixes rendering problem on MacOS where rendering would start in bottom left
 * corner of screen and only fix once window was moved.
 */
void fix_render_on_mac(GLFWwindow* window) {
#ifdef __APPLE__
    static bool macMoved = false;
    if(!macMoved) {
        int x, y;
        glfwGetWindowPos(window, &x, &y);
        glfwSetWindowPos(window, ++x, y);
        macMoved = true;
    }
#endif
}
