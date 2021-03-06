#pragma once

#include "game_engine/models/raw_model.h"
#include "game_engine/render_engine/loader.h"
#include "game_engine/textures/model_texture.h"
#include "span.h"

typedef struct {
    float x;
    float z;
    raw_model_t model;
    model_texture_t texture;
} terrain_t;

terrain_t terrain_new(int grid_x, int grid_z, loader_t* loader, model_texture_t texture);

typedef SPAN_TYPE(terrain_t) terrain_span_t;
typedef BUFFER_TYPE(terrain_t) terrain_buffer_t;
