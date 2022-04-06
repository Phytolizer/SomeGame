#include "game_engine/toolbox/math.h"

#include "cglm/affine.h"

void create_transformation_matrix(mat4 out, vec3 translation, vec3 rotation, float scale) {
    glm_mat4_identity(out);
    glm_translate(out, translation);
    vec3 x = {1.0f, 0.0f, 0.0f};
    glm_rotate(out, degrees_to_radians(rotation[0]), x);
    vec3 y = {0.0f, 1.0f, 0.0f};
    glm_rotate(out, degrees_to_radians(rotation[1]), y);
    vec3 z = {0.0f, 0.0f, 1.0f};
    glm_rotate(out, degrees_to_radians(rotation[2]), z);
    vec3 v_scale = {scale, scale, scale};
    glm_scale(out, v_scale);
}

void create_view_matrix(mat4 out, camera_t camera) {
    glm_mat4_identity(out);
    glm_rotate(out, degrees_to_radians(camera.pitch), (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(out, degrees_to_radians(camera.yaw), (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(out, degrees_to_radians(camera.roll), (vec3){0.0f, 0.0f, 1.0f});
    vec3 camera_pos;
    glm_vec3_copy(camera.position, camera_pos);
    vec3 negative_camera_pos = {-camera_pos[0], -camera_pos[1], -camera_pos[2]};
    glm_translate(out, negative_camera_pos);
}

float degrees_to_radians(float degrees) {
    return degrees * (M_PI / 180.0f);
}
