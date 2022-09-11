#ifndef _GLDEBUG_H_
#define _GLDEBUG_H_

#include <signal.h>

#define ASSERT(x) if(!(x)) raise(SIGTRAP);
#define GL_CALL(x)      \
do {                    \
    gl_clear_error();   \
    x;                  \
    ASSERT(gl_log_call(#x, __FILE__, __LINE__));\
} while(0)

void gl_check_error();
void gl_clear_error();
int gl_log_call(const char * function, const char * file, int line);

// TODO: Implement this
int gen_log_call(const char * function, const char * file, int line);

#endif