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

void static_shader_load_view_matrix(shader_program_t* static_shader, camera_t* camera) {
    mat4 view_matrix;
    create_view_matrix(view_matrix, camera);
    static_shader_t* shader = (static_shader_t*)static_shader;
    shader_program_load_matrix(static_shader, shader->location_view_matrix, view_matrix);
}

static void bind_attributes(static_shader_t* shader) {
    shader_program_t* base = (shader_program_t*)shader;
    shader_program_bind_attribute(base, 0, "position");
    shader_program_bind_attribute(base, 1, "textureCoords");
}

static void get_all_uniform_locations(static_shader_t* shader) {
    shader_program_t* base = (shader_program_t*)shader;
    shader->location_transformation_matrix =
            shader_program_get_uniform_location(base, "transformationMatrix");
    shader->location_projection_matrix =
            shader_program_get_uniform_location(base, "projectionMatrix");
    shader->location_view_matrix = shader_program_get_uniform_location(base, "viewMatrix");
}
