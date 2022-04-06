#pragma once

#include "cglm/mat4.h"
#include "game_engine/entities/camera.h"

void create_transformation_matrix(mat4 out, vec3 translation, vec3 rotation, float scale);
void create_view_matrix(mat4 out, camera_t camera);
float degrees_to_radians(float degrees);
