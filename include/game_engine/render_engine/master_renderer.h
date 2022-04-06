#pragma once

#include "buffer.h"
#include "game_engine/entities/entity.h"
#include "game_engine/render_engine/entity_renderer.h"
#include "game_engine/shaders/shader_program.h"

typedef struct {
    shader_program_t* static_shader;
    entity_renderer_t* renderer;
    entity_map_t* entities;
} master_renderer_t;

void master_renderer_render(master_renderer_t* renderer, light_t sun, camera_t camera);
void master_renderer_process_entity(master_renderer_t* renderer, entity_t entity);
void master_renderer_clean_up(master_renderer_t* renderer);
