#pragma once

#include "game_engine/gl.h"

#include <stddef.h>

typedef struct {
    GLuint vao_id;
    size_t vertex_count;
} raw_model_t;
