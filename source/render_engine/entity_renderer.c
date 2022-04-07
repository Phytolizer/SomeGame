#include "game_engine/render_engine/entity_renderer.h"

#include "game_engine/gl.h"
#include "game_engine/toolbox/math.h"
#include "stb_ds.h"

#include <string.h>

static void prepare_textured_model(entity_renderer_t* renderer, textured_model_t model);
static void unbind_textured_model(textured_model_t model);
static void prepare_instance(entity_renderer_t* renderer, entity_t entity);

entity_renderer_t entity_renderer_new(shader_program_t* shader, mat4 projection_matrix) {
    entity_renderer_t renderer;
    renderer.shader = shader;
    shader_program_start(shader);
    static_shader_load_projection_matrix(shader, projection_matrix);
    shader_program_stop(shader);
    return renderer;
}

void entity_renderer_render(entity_renderer_t* renderer, entity_map_t* entities) {
    for (ptrdiff_t i = 0; i < stbds_hmlen(entities); i++) {
        textured_model_t model = entities[i].key;
        prepare_textured_model(renderer, model);
        entity_buffer_t batch = stbds_hmget(entities, model);
        for (size_t j = 0; j < batch.length; j++) {
            entity_t entity = batch.data[j];
            prepare_instance(renderer, entity);
            glDrawElements(GL_TRIANGLES, model.raw_model.vertex_count, GL_UNSIGNED_INT, 0);
        }
        unbind_textured_model(model);
    }
}

static void prepare_textured_model(entity_renderer_t* renderer, textured_model_t model) {
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

static void prepare_instance(entity_renderer_t* renderer, entity_t entity) {
    mat4 transformation_matrix;
    create_transformation_matrix(
            transformation_matrix, entity.position, entity.rotation, entity.scale);
    static_shader_load_transformation_matrix(renderer->shader, transformation_matrix);
}
