#pragma once

#include "cglm/vec3.h"
#include "game_engine/render_engine/display_manager.h"

typedef struct {
    const display_manager_t* display_manager;
    vec3 position;
    float pitch;
    float yaw;
    float roll;
} camera_t;

camera_t camera_new(const display_manager_t* display_manager);
void camera_move(camera_t* camera);
