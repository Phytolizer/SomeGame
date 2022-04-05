#include "game_engine/render_engine/renderer.h"

#include "game_engine/gl.h"

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

void renderer_render(renderer_t* renderer, textured_model_t textured_model) {
    (void)renderer;
    raw_model_t model = textured_model.raw_model;
    glBindVertexArray(model.vao_id);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textured_model.texture.texture_id);
    glDrawElements(GL_TRIANGLES, model.vertex_count, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}
