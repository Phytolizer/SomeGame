#include "game_engine/render_engine/terrain_renderer.h"

#include "game_engine/models/textured_model.h"
#include "game_engine/shaders/terrain_shader.h"
#include "game_engine/toolbox/math.h"

static void prepare_terrain(terrain_renderer_t* renderer, terrain_t terrain);
static void unbind_textured_model(void);
static void load_model_matrix(terrain_renderer_t* renderer, terrain_t terrain);

terrain_renderer_t terrain_renderer_new(shader_program_t* terrain_shader, mat4 projection_matrix) {
    terrain_renderer_t terrain_renderer;
    terrain_renderer.shader = terrain_shader;
    shader_program_start(terrain_renderer.shader);
    terrain_shader_load_projection_matrix(terrain_renderer.shader, projection_matrix);
    shader_program_stop(terrain_renderer.shader);
    return terrain_renderer;
}

void terrain_renderer_render(terrain_renderer_t* renderer, terrain_span_t terrains) {
    for (terrain_t* terrain = terrains.begin; terrain != terrains.end; terrain++) {
        prepare_terrain(renderer, *terrain);
        load_model_matrix(renderer, *terrain);
        glDrawElements(GL_TRIANGLES, terrain->model.vertex_count, GL_UNSIGNED_INT, 0);
        unbind_textured_model();
    }
}

static void prepare_terrain(terrain_renderer_t* renderer, terrain_t terrain) {
    raw_model_t raw_model = terrain.model;
    glBindVertexArray(raw_model.vao_id);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    model_texture_t texture = terrain.texture;
    terrain_shader_load_shine_variables(
            renderer->shader, texture.shine_damper, texture.reflectivity);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.texture_id);
}

static void unbind_textured_model(void) {
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}

static void load_model_matrix(terrain_renderer_t* renderer, terrain_t terrain) {
    mat4 transformation_matrix;
    create_transformation_matrix(transformation_matrix, (vec3){terrain.x, 0.0f, terrain.z},
            (vec3){0.0f, 0.0f, 0.0f}, 1.0f);
    terrain_shader_load_transformation_matrix(renderer->shader, transformation_matrix);
}
