#pragma once

#include "cglm/mat4.h"
#include "game_engine/entities/entity.h"
#include "game_engine/render_engine/display_manager.h"
#include "game_engine/shaders/static_shader.h"

typedef struct {
    shader_program_t* shader;
} entity_renderer_t;

entity_renderer_t entity_renderer_new(shader_program_t* shader, mat4 projection_matrix);
void entity_renderer_render(entity_renderer_t* renderer, entity_map_t* entities);
