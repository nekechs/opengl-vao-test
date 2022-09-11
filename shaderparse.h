#ifndef _SHADERPARSE_H_
#define _SHADERPARSE_H_

#define _SHADER_PREPROC "#shader"
#define _SHADER_PREPROC_LEN 7

#define _VERT_KEYWORD " vertex"
#define _VERT_KEYWORD_LEN 7

#define _FRAG_KEYWORD " fragment"
#define _FRAG_KEYWORD_LEN 9

struct vf_shader{
    char * vert_shader;
    char * frag_shader;
};

enum shader_type {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
};

// If shader parse is successful, return 0. Otherwise return -1.
int parse_shader(struct vf_shader * shaders, const char * filename);
void destroy_shader(struct vf_shader * shaders);

#endif