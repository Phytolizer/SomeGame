#pragma once

#include "cglm/vec3.h"
#include "game_engine/models/textured_model.h"

typedef struct {
    textured_model_t model;
    vec3 position;
    vec3 rotation;
    float scale;
} entity_t;

void entity_increase_position(entity_t* entity, vec3 delta);
void entity_increase_rotation(entity_t* entity, vec3 delta);
