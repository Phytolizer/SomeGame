#pragma once

#include "game_engine/gl.h"

// This struct is more like an abstract class.
typedef struct {
    GLuint program_id;
    GLuint vertex_shader_id;
    GLuint fragment_shader_id;
} shader_program_t;

// This function is not meant to be called directly, but rather as part of a "subclass"'s
// initialization process.
//
// In order to initialize a shader_program_t "subclass":
//  * Call this function to initialize the base.
//  * Bind any needed attributes.
//  * Call shader_program_finish_init().
//  * Get all uniform locations as needed.
shader_program_t shader_program_new(const char* vertex_file, const char* fragment_file);
void shader_program_start(shader_program_t* program);
void shader_program_stop(shader_program_t* program);
void shader_program_cleanup(shader_program_t* program);

// "Protected" functions.
// ----

// Call after attributes are bound to link the program.
void shader_program_finish_init(shader_program_t* program);
// Call to bind an individual attribute.
void shader_program_bind_attribute(
        shader_program_t* program, int attribute, const char* variable_name);
// Call to get an individual uniform location.
GLint shader_program_get_uniform_location(shader_program_t* program, const char* uniform_name);
void shader_program_load_float(shader_program_t* program, GLint location, float value);
void shader_program_load_vector(shader_program_t* program, GLint location, float value[3]);
