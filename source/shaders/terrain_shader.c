#include "game_engine/shaders/terrain_shader.h"

#include "config.h"
#include "game_engine/toolbox/math.h"

#define VERTEX_FILE DATA_ROOT_PATH "/shaders/terrainShader.vert"
#define FRAGMENT_FILE DATA_ROOT_PATH "/shaders/terrainShader.frag"

static void bind_attributes(terrain_shader_t* shader);
static void get_all_uniform_locations(terrain_shader_t* shader);

shader_program_t* terrain_shader_new(void) {
    terrain_shader_t* result = malloc(sizeof(terrain_shader_t));
    result->base = shader_program_new(VERTEX_FILE, FRAGMENT_FILE);
    bind_attributes(result);
    shader_program_finish_init((shader_program_t*)result);
    get_all_uniform_locations(result);
    return (shader_program_t*)result;
}

void terrain_shader_load_transformation_matrix(shader_program_t* terrain_shader, mat4 matrix) {
    terrain_shader_t* shader = (terrain_shader_t*)terrain_shader;
    shader_program_load_matrix(terrain_shader, shader->location_transformation_matrix, matrix);
}

void terrain_shader_load_projection_matrix(shader_program_t* terrain_shader, mat4 matrix) {
    terrain_shader_t* shader = (terrain_shader_t*)terrain_shader;
    shader_program_load_matrix(terrain_shader, shader->location_projection_matrix, matrix);
}

void terrain_shader_load_light(shader_program_t* terrain_shader, light_t light) {
    terrain_shader_t* shader = (terrain_shader_t*)terrain_shader;
    shader_program_load_vector(terrain_shader, shader->location_light_position, light.position);
    shader_program_load_vector(terrain_shader, shader->location_light_color, light.color);
}

void terrain_shader_load_view_matrix(shader_program_t* terrain_shader, camera_t camera) {
    mat4 view_matrix;
    create_view_matrix(view_matrix, camera);
    terrain_shader_t* shader = (terrain_shader_t*)terrain_shader;
    shader_program_load_matrix(terrain_shader, shader->location_view_matrix, view_matrix);
}

void terrain_shader_load_shine_variables(
        shader_program_t* terrain_shader, float shine_damper, float reflectivity) {
    terrain_shader_t* shader = (terrain_shader_t*)terrain_shader;
    shader_program_load_float(terrain_shader, shader->location_shine_damper, shine_damper);
    shader_program_load_float(terrain_shader, shader->location_reflectivity, reflectivity);
}

static void bind_attributes(terrain_shader_t* shader) {
    shader_program_t* base = (shader_program_t*)shader;
    shader_program_bind_attribute(base, 0, "position");
    shader_program_bind_attribute(base, 1, "textureCoords");
    shader_program_bind_attribute(base, 2, "normal");
}

static void get_all_uniform_locations(terrain_shader_t* shader) {
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
