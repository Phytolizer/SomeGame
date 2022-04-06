#pragma once

#include "game_engine/entities/camera.h"
#include "game_engine/entities/light.h"
#include "game_engine/shaders/shader_program.h"

typedef struct {
    shader_program_t base;
    GLint location_transformation_matrix;
    GLint location_projection_matrix;
    GLint location_view_matrix;
    GLint location_light_position;
    GLint location_light_color;
    GLint location_shine_damper;
    GLint location_reflectivity;
} terrain_shader_t;

shader_program_t* terrain_shader_new(void);
void terrain_shader_load_transformation_matrix(shader_program_t* terrain_shader, mat4 matrix);
void terrain_shader_load_projection_matrix(shader_program_t* terrain_shader, mat4 matrix);
void terrain_shader_load_view_matrix(shader_program_t* terrain_shader, camera_t camera);
void terrain_shader_load_light(shader_program_t* terrain_shader, light_t light);
void terrain_shader_load_shine_variables(
        shader_program_t* terrain_shader, float shine_damper, float reflectivity);
