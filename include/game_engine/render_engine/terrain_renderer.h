#pragma once

#include "cglm/mat4.h"
#include "game_engine/shaders/shader_program.h"
#include "game_engine/terrains/terrain.h"

typedef struct {
    shader_program_t* shader;
} terrain_renderer_t;

terrain_renderer_t terrain_renderer_new(shader_program_t* terrain_shader, mat4 projection_matrix);
void terrain_renderer_render(terrain_renderer_t* renderer, terrain_span_t terrains);
