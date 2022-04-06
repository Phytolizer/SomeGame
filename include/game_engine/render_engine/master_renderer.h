#pragma once

#include "buffer.h"
#include "cglm/mat4.h"
#include "game_engine/entities/entity.h"
#include "game_engine/render_engine/entity_renderer.h"
#include "game_engine/render_engine/terrain_renderer.h"
#include "game_engine/shaders/shader_program.h"
#include "game_engine/terrains/terrain.h"

typedef struct {
    const display_manager_t* display_manager;
    mat4 projection_matrix;
    shader_program_t* static_shader;
    entity_renderer_t entity_renderer;
    terrain_renderer_t terrain_renderer;
    shader_program_t* terrain_shader;
    entity_map_t* entities;
    terrain_buffer_t terrains;
} master_renderer_t;

master_renderer_t master_renderer_new(const display_manager_t* display_manager);
void master_renderer_render(master_renderer_t* renderer, light_t sun, camera_t camera);
void master_renderer_process_entity(master_renderer_t* renderer, entity_t entity);
void master_renderer_process_terrain(master_renderer_t* renderer, terrain_t terrain);
void master_renderer_prepare(master_renderer_t* renderer);
void master_renderer_clean_up(master_renderer_t* renderer);
