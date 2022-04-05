#include "game_engine/render_engine/renderer.h"

#include "game_engine/gl.h"
#include "game_engine/toolbox/math.h"

renderer_t renderer_new(void) {
    renderer_t renderer;
    renderer.nothing = 0;
    return renderer;
}

void renderer_prepare(renderer_t* renderer) {
    (void)renderer;
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer_render(renderer_t* renderer, entity_t entity, shader_program_t* shader) {
    (void)renderer;
    textured_model_t textured_model = entity.model;
    raw_model_t model = textured_model.raw_model;
    glBindVertexArray(model.vao_id);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    mat4 transformation_matrix;
    create_transformation_matrix(
            transformation_matrix, entity.position, entity.rotation, entity.scale);
    static_shader_load_transformation_matrix(shader, transformation_matrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textured_model.texture.texture_id);
    glDrawElements(GL_TRIANGLES, model.vertex_count, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}
