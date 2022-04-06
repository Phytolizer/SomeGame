#pragma once

#include "cglm/mat4.h"
#include "game_engine/entities/entity.h"
#include "game_engine/render_engine/display_manager.h"
#include "game_engine/shaders/static_shader.h"

typedef struct {
    const display_manager_t* display_manager;
    mat4 projection_matrix;
    shader_program_t* shader;
} renderer_t;

renderer_t renderer_new(const display_manager_t* display_manager, shader_program_t* shader);
void renderer_prepare(renderer_t* renderer);
void renderer_render(renderer_t* renderer, entity_map_t* entities);
