#pragma once

#include "cglm/mat4.h"

// Uses an out parameter!
void create_transformation_matrix(mat4 out, vec3 translation, vec3 rotation, float scale);
