#include <stdio.h>
#include <string.h>

#include "shaderparse.h"
#include "utils/fileio.h"

#ifndef DEBUG
#define DEBUG 0
#endif

int parse_shader(struct vf_shader * shaders, const char * filename) {
    stream_t raw_text = open_raw_data(filename);
    
    if(raw_text.data == NULL && raw_text.size == 0) {
        // There is an error with reading the shader.
        return -1;
    }

    // inclusive
    int left_index = 0;

    // exclusive
    int right_index = -1;

    int mode = 0;
    int shader_indices[2][2];
    
    for(int i = 0; i < raw_text.size; i++) {
        if(raw_text.data[i] == '\n') {
            left_index = right_index + 1;
            right_index = i;

            if(DEBUG) printf("left: %d;\tright: %d\n", left_index, right_index);

            if(right_index - left_index >= _SHADER_PREPROC_LEN && !memcmp(_SHADER_PREPROC, raw_text.data + left_index, _SHADER_PREPROC_LEN)) {
                // We found the shader keyword!!!!
                if(right_index - left_index == _SHADER_PREPROC_LEN + _VERT_KEYWORD_LEN &&
                    !memcmp(_VERT_KEYWORD, raw_text.data + left_index + _SHADER_PREPROC_LEN, _VERT_KEYWORD_LEN)) {
                    // Here, the line is vertex keyword.
                    if(DEBUG) puts("\tvertex!");
                    shader_indices[VERTEX][0] = right_index + 1;
                    mode = VERTEX;
                } else if(right_index - left_index == _SHADER_PREPROC_LEN + _FRAG_KEYWORD_LEN &&
                    !memcmp(_FRAG_KEYWORD, raw_text.data + left_index + _SHADER_PREPROC_LEN, _FRAG_KEYWORD_LEN)) {
                    // Here, the line is shader keyword.
                    if(DEBUG) puts("\tfragment!");
                    shader_indices[FRAGMENT][0] = right_index + 1;
                    mode = FRAGMENT;
                }
            } else {
                shader_indices[mode][1] = i;
            }
        }
    }

    int shader_lengths[2];

    shader_lengths[VERTEX] = shader_indices[VERTEX][1] - shader_indices[VERTEX][0];
    shader_lengths[FRAGMENT] = shader_indices[FRAGMENT][1] - shader_indices[FRAGMENT][0];

    if(DEBUG) {
        printf("Vertex length: %d; Fragment length: %d\n", shader_lengths[VERTEX], shader_lengths[FRAGMENT]);
        printf("Vertex vertices: %d and %d\n", shader_indices[VERTEX][1], shader_indices[VERTEX][0]);
        printf("Fragment vertices: %d and %d\n", shader_indices[FRAGMENT][1], shader_indices[FRAGMENT][0]);
    }

    shaders->vert_shader = malloc(shader_lengths[VERTEX] + 1);
    shaders->frag_shader = malloc(shader_lengths[FRAGMENT] + 1);

    memcpy(shaders->vert_shader, raw_text.data + shader_indices[VERTEX][0], shader_lengths[VERTEX]); 
    memcpy(shaders->frag_shader, raw_text.data + shader_indices[FRAGMENT][0], shader_lengths[FRAGMENT]); 

    shaders->vert_shader[shader_lengths[VERTEX]] = '\0';
    shaders->frag_shader[shader_lengths[FRAGMENT]] = '\0';

    stream_destroy(&raw_text);
    return 0;
}

void destroy_shader(struct vf_shader * shaders) {
    free(shaders->frag_shader);
    free(shaders->vert_shader);
}
