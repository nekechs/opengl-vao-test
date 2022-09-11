#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>
#include <assert.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shaderparse.h"
#include "utils/gl-debug.h"

static unsigned int compile_shader(unsigned int type, const char * source) {
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(!result) {
        // Shader did NOT compile successfully
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char * message = malloc( (length + 1) * sizeof(char));

        glGetShaderInfoLog(id, length, &length, message);
        printf("Failed compilation of %s\n", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
        printf("msg: %s\n", message);

        free(message);
        glDeleteShader(id);
        return 0;
    }

    return id;
}

/**
 * @brief Create a shader object which is attached to a GPU program, and the program ID is returned
 * 
 * @param vertex_shader The text in the vertex shader
 * @param fragment_shader The text in the fragment shader
 * @return int: The integer ID of the program that is to be run on the GPU
 */
static int create_shader(const char * vertex_shader, const char * fragment_shader) {
    unsigned int program = glCreateProgram();

    // This is our vertex shader.
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(int argc, char ** argv) {
    GLFWwindow * window;

    if(!glfwInit()) {
        return -1;
    }

    window = glfwCreateWindow(800, 600, "HWORLD", NULL, NULL);

    if(!window) {
        glfwTerminate();
        return -1;
    } 

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if(glewInit() != GLEW_OK) {
        printf("ERROR! glew did not initialize properly.\n");
    }

    puts(glGetString(GL_VERSION));

    float triangle_arr[] = {
        -0.5f, -0.5f,           // 0
        0.5f, -0.5f,            // 1
        -0.5f, 0.5f,            // 2
        0.5f, 0.5f              // 3
    };

    unsigned int indices[] = {
        0, 1, 3,
        0, 2, 3
    };

    unsigned int buffer_id;
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(float), triangle_arr, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    // index buffer object
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    struct vf_shader shaders;
    parse_shader(&shaders, "res/shaders/basic.shader");

    // compile the shader
    unsigned int shader = create_shader(shaders.vert_shader, shaders.frag_shader);

    // Frees the dynamically allocated shaders
    destroy_shader(&shaders);

    GL_CALL(glUseProgram(shader));

    int location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);
    GL_CALL(glUniform4f(location, 0.2f, 0.4f, 0.8f, 1.0f));

    GL_CALL(glUseProgram(0));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    float r = 0.0f;
    float increment = 0.1f;

    while(!glfwWindowShouldClose(window)) {
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        GL_CALL(glUseProgram(shader));
        GL_CALL(glUniform4f(location, 1.0f, 0.1f, r, 1.0f));
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, buffer_id));
        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));


        if(r > 1.0f) {
            increment = -0.05f;
        } else if(r < 0.0f) {
            increment = 0.1f;
        }

        r += increment;

        GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

        // Standard stuff
        glfwSwapBuffers(window);
        glfwPollEvents();

        // current_frame = (current_frame - 1) % frame_limit;
    }

    glfwTerminate();
    return 0;
}