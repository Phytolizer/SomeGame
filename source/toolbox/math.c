#include "game_engine/toolbox/math.h"

#include "cglm/affine.h"

void create_transformation_matrix(mat4 out, vec3 translation, vec3 rotation, float scale) {
    glm_mat4_identity(out);
    glm_translate(out, translation);
    vec3 x = {1.0f, 0.0f, 0.0f};
    glm_rotate(out, rotation[0], x);
    vec3 y = {0.0f, 1.0f, 0.0f};
    glm_rotate(out, rotation[1], y);
    vec3 z = {0.0f, 0.0f, 1.0f};
    glm_rotate(out, rotation[2], z);
    vec3 v_scale = {scale, scale, scale};
    glm_scale(out, v_scale);
}

float degrees_to_radians(float degrees) {
    return degrees * (M_PI / 180.0f);
}
