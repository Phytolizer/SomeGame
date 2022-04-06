#pragma once

#include "game_engine/gl.h"

typedef struct {
    GLuint texture_id;
    float shine_damper;
    float reflectivity;
} model_texture_t;

model_texture_t model_texture_new(GLuint texture_id);
