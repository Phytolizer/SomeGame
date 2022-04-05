#pragma once

#include "game_engine/models/textured_model.h"

typedef struct {
    int nothing;
} renderer_t;

renderer_t renderer_new(void);
void renderer_prepare(renderer_t* renderer);
void renderer_render(renderer_t* renderer, textured_model_t model);
