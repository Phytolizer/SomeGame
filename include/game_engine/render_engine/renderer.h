#pragma once

#include "game_engine/entities/entity.h"
#include "game_engine/shaders/static_shader.h"

typedef struct {
    int nothing;
} renderer_t;

renderer_t renderer_new(void);
void renderer_prepare(renderer_t* renderer);
void renderer_render(renderer_t* renderer, entity_t entity, shader_program_t* shader);
