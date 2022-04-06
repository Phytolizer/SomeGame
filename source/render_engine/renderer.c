#include "game_engine/render_engine/renderer.h"

#include "game_engine/gl.h"
#include "game_engine/toolbox/math.h"

#include <string.h>

#define FOV 70
#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.0f

static void create_projection_matrix(renderer_t* renderer);

renderer_t renderer_new(const display_manager_t* display_manager, shader_program_t* shader) {
    renderer_t renderer;
    renderer.display_manager = display_manager;
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

void renderer_render(renderer_t* renderer, entity_t entity, shader_program_t* shader) {
    (void)renderer;
    textured_model_t textured_model = entity.model;
    raw_model_t model = textured_model.raw_model;
    glBindVertexArray(model.vao_id);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    mat4 transformation_matrix;
    create_transformation_matrix(
            transformation_matrix, entity.position, entity.rotation, entity.scale);
    static_shader_load_transformation_matrix(shader, transformation_matrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textured_model.texture.texture_id);
    glDrawElements(GL_TRIANGLES, model.vertex_count, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
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
