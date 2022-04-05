#pragma once

#include "game_engine/models/raw_model.h"
#include "game_engine/textures/model_texture.h"

typedef struct {
    raw_model_t raw_model;
    model_texture_t texture;
} textured_model_t;
