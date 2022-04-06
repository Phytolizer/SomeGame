#include "game_engine/shaders/static_shader.h"

#include "config.h"
#include "game_engine/toolbox/math.h"

#include <stdlib.h>

#define VERTEX_FILE DATA_ROOT_PATH "/shaders/shader.vert"
#define FRAGMENT_FILE DATA_ROOT_PATH "/shaders/shader.frag"

static void bind_attributes(static_shader_t* shader);
static void get_all_uniform_locations(static_shader_t* shader);

shader_program_t* static_shader_new(void) {
    static_shader_t* result = malloc(sizeof(static_shader_t));
    result->base = shader_program_new(VERTEX_FILE, FRAGMENT_FILE);
    bind_attributes(result);
    shader_program_finish_init((shader_program_t*)result);
    get_all_uniform_locations(result);
    return (shader_program_t*)result;
}

void static_shader_load_transformation_matrix(shader_program_t* static_shader, mat4 matrix) {
    static_shader_t* shader = (static_shader_t*)static_shader;
    shader_program_load_matrix(static_shader, shader->location_transformation_matrix, matrix);
}

void static_shader_load_projection_matrix(shader_program_t* static_shader, mat4 matrix) {
    static_shader_t* shader = (static_shader_t*)static_shader;
    shader_program_load_matrix(static_shader, shader->location_projection_matrix, matrix);
}

void static_shader_load_view_matrix(shader_program_t* static_shader, camera_t camera) {
    mat4 view_matrix;
    create_view_matrix(view_matrix, camera);
    static_shader_t* shader = (static_shader_t*)static_shader;
    shader_program_load_matrix(static_shader, shader->location_view_matrix, view_matrix);
}

void static_shader_load_light(shader_program_t* static_shader, light_t light) {
    static_shader_t* shader = (static_shader_t*)static_shader;
    shader_program_load_vector(static_shader, shader->location_light_position, light.position);
    shader_program_load_vector(static_shader, shader->location_light_color, light.color);
}

void static_shader_load_shine_variables(
        shader_program_t* static_shader, float shine_damper, float reflectivity) {
    static_shader_t* shader = (static_shader_t*)static_shader;
    shader_program_load_float(static_shader, shader->location_shine_damper, shine_damper);
    shader_program_load_float(static_shader, shader->location_reflectivity, reflectivity);
}

static void bind_attributes(static_shader_t* shader) {
    shader_program_t* base = (shader_program_t*)shader;
    shader_program_bind_attribute(base, 0, "position");
    shader_program_bind_attribute(base, 1, "textureCoords");
    shader_program_bind_attribute(base, 2, "normal");
}

static void get_all_uniform_locations(static_shader_t* shader) {
    shader_program_t* base = (shader_program_t*)shader;
    shader->location_transformation_matrix =
            shader_program_get_uniform_location(base, "transformationMatrix");
    shader->location_projection_matrix =
            shader_program_get_uniform_location(base, "projectionMatrix");
    shader->location_view_matrix = shader_program_get_uniform_location(base, "viewMatrix");
    shader->location_light_position = shader_program_get_uniform_location(base, "lightPosition");
    shader->location_light_color = shader_program_get_uniform_location(base, "lightColor");
    shader->location_shine_damper = shader_program_get_uniform_location(base, "shineDamper");
    shader->location_reflectivity = shader_program_get_uniform_location(base, "reflectivity");
}
