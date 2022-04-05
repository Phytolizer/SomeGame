#pragma once

#include "cglm/mat4.h"
#include "game_engine/entities/camera.h"
#include "game_engine/shaders/shader_program.h"

typedef struct {
    shader_program_t base;
    GLint location_transformation_matrix;
    GLint location_projection_matrix;
    GLint location_view_matrix;
} static_shader_t;

shader_program_t* static_shader_new(void);
void static_shader_load_transformation_matrix(shader_program_t* static_shader, mat4 matrix);
void static_shader_load_projection_matrix(shader_program_t* static_shader, mat4 matrix);
void static_shader_load_view_matrix(shader_program_t* static_shader, camera_t* camera);
