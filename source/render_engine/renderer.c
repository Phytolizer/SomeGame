#include "game_engine/render_engine/renderer.h"

#include "game_engine/gl.h"
#include "game_engine/toolbox/math.h"
#include "stb_ds.h"

#include <string.h>

#define FOV 70
#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.0f

static void create_projection_matrix(renderer_t* renderer);
static void prepare_textured_model(renderer_t* renderer, textured_model_t model);
static void unbind_textured_model(textured_model_t model);
static void prepare_instance(renderer_t* renderer, entity_t entity);

renderer_t renderer_new(const display_manager_t* display_manager, shader_program_t* shader) {
    renderer_t renderer;
    renderer.shader = shader;
    renderer.display_manager = display_manager;
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    create_projection_matrix(&renderer);
    shader_program_start(shader);
    static_shader_load_projection_matrix(shader, renderer.projection_matrix);
    shader_program_stop(shader);
    return renderer;
}

void renderer_prepare(renderer_t* renderer) {
    (void)renderer;
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_render(renderer_t* renderer, entity_map_t* entities) {
    for (ptrdiff_t i = 0; i < hmlen(entities); i++) {
        textured_model_t model = entities[i].key;
        prepare_textured_model(renderer, model);
        entity_buffer_t batch = hmget(entities, model);
        for (size_t j = 0; j < batch.length; j++) {
            entity_t entity = batch.data[j];
            prepare_instance(renderer, entity);
            glDrawElements(GL_TRIANGLES, model.raw_model.vertex_count, GL_UNSIGNED_INT, 0);
        }
        unbind_textured_model(model);
    }
}

static void create_projection_matrix(renderer_t* renderer) {
    float aspect_ratio =
            (float)renderer->display_manager->width / (float)renderer->display_manager->height;
    float y_scale = (float)((1.0f - tanf(degrees_to_radians(FOV / 2.0f))) * aspect_ratio);
    float x_scale = y_scale / aspect_ratio;
    float frustum_length = FAR_PLANE - NEAR_PLANE;

    glm_mat4_identity(renderer->projection_matrix);
    renderer->projection_matrix[0][0] = x_scale;
    renderer->projection_matrix[1][1] = y_scale;
    renderer->projection_matrix[2][2] = -((FAR_PLANE + NEAR_PLANE) / frustum_length);
    renderer->projection_matrix[2][3] = -1;
    renderer->projection_matrix[3][2] = -((2 * NEAR_PLANE * FAR_PLANE) / frustum_length);
    renderer->projection_matrix[3][3] = 0;
}

static void prepare_textured_model(renderer_t* renderer, textured_model_t model) {
    raw_model_t raw_model = model.raw_model;
    glBindVertexArray(raw_model.vao_id);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    model_texture_t texture = model.texture;
    static_shader_load_shine_variables(
            renderer->shader, texture.shine_damper, texture.reflectivity);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.texture_id);
}

static void unbind_textured_model(textured_model_t model) {
    (void)model;
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}

static void prepare_instance(renderer_t* renderer, entity_t entity) {
    mat4 transformation_matrix;
    create_transformation_matrix(
            transformation_matrix, entity.position, entity.rotation, entity.scale);
    static_shader_load_transformation_matrix(renderer->shader, transformation_matrix);
}
