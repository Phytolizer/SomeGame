#include "game_engine/shaders/shader_program.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

static GLuint load_shader(const char* file, int type) {
    FILE* fp = fopen(file, "r");
    int fd = fileno(fp);
    struct stat statbuf;
    fstat(fd, &statbuf);

    char* source = malloc(statbuf.st_size + 1);
    fread(source, statbuf.st_size, 1, fp);
    source[statbuf.st_size] = '\0';

    fclose(fp);

    // exists to suppress a warning about nested const in pointer types
    const char* csource = source;

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &csource, NULL);
    glCompileShader(shader);
    free(source);
    int compile_status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE) {
        int log_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        char* log = malloc(log_length);
        glGetShaderInfoLog(shader, log_length, NULL, log);
        fprintf(stderr, "Failed to compile shader: %s\n", log);
        free(log);
        glDeleteShader(shader);
        exit(EXIT_FAILURE);
    }

    return shader;
}

shader_program_t shader_program_new(const char* vertex_file, const char* fragment_file) {
    shader_program_t result;
    result.vertex_shader_id = load_shader(vertex_file, GL_VERTEX_SHADER);
    result.fragment_shader_id = load_shader(fragment_file, GL_FRAGMENT_SHADER);
    result.program_id = glCreateProgram();
    glAttachShader(result.program_id, result.vertex_shader_id);
    glAttachShader(result.program_id, result.fragment_shader_id);

    return result;
}

void shader_program_start(shader_program_t* program) {
    glUseProgram(program->program_id);
}

void shader_program_stop(shader_program_t* program) {
    (void)program;
    glUseProgram(0);
}

void shader_program_cleanup(shader_program_t* program) {
    glDetachShader(program->program_id, program->vertex_shader_id);
    glDetachShader(program->program_id, program->fragment_shader_id);
    glDeleteShader(program->vertex_shader_id);
    glDeleteShader(program->fragment_shader_id);
    glDeleteProgram(program->program_id);
}

void shader_program_finish_init(shader_program_t* program) {
    glLinkProgram(program->program_id);
    int link_status;
    glGetProgramiv(program->program_id, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE) {
        int log_length;
        glGetProgramiv(program->program_id, GL_INFO_LOG_LENGTH, &log_length);
        char* log = malloc(log_length);
        glGetProgramInfoLog(program->program_id, log_length, NULL, log);
        fprintf(stderr, "Failed to link shader program: %s\n", log);
        free(log);
        glDeleteProgram(program->program_id);
        exit(EXIT_FAILURE);
    }
}

void shader_program_bind_attribute(
        shader_program_t* program, int attribute, const char* variable_name) {
    glBindAttribLocation(program->program_id, attribute, variable_name);
}

GLint shader_program_get_uniform_location(shader_program_t* program, const char* uniform_name) {
    return glGetUniformLocation(program->program_id, uniform_name);
}

void shader_program_load_float(shader_program_t* program, GLint location, float value) {
    (void)program;
    glUniform1f(location, value);
}

void shader_program_load_vector(shader_program_t* program, GLint location, vec3 value) {
    (void)program;
    glUniform3fv(location, 1, value);
}

void shader_program_load_bool(shader_program_t* program, GLint location, bool value) {
    float to_load = 0.0f;
    if (value) {
        to_load = 1.0f;
    }
    shader_program_load_float(program, location, to_load);
}

void shader_program_load_matrix(shader_program_t* program, GLint location, mat4 value) {
    (void)program;
    glUniformMatrix4fv(location, 1, GL_FALSE, value[0]);
}
