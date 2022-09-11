#include <stdio.h>
#include <GL/glew.h>

#include "gl-debug.h"

void gl_check_error() {
    GLenum i = 0;
    while(i = glGetError()) {
        printf("[OpenGL error]: %d\n", i);
    }
}

void gl_clear_error() {
    while(glGetError() != GL_NO_ERROR);
}

int gl_log_call(const char * function, const char * file, int num) {
    GLenum error = glGetError();
    if(error) {
        printf("[OpenGL ERROR in %s; file %s, line %d]: %d\n", function, file, num, error);
        return 0;
    }

    return 1;
}