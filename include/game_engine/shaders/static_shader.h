#pragma once

#include "game_engine/shaders/shader_program.h"

typedef struct {
    shader_program_t base;
} static_shader_t;

shader_program_t* static_shader_new(void);
