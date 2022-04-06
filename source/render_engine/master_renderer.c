#include "game_engine/render_engine/master_renderer.h"

#include "game_engine/shaders/shader_program.h"
#include "game_engine/shaders/static_shader.h"
#include "stb_ds.h"

void master_renderer_render(master_renderer_t* renderer, light_t sun, camera_t camera) {
    entity_renderer_prepare(renderer->renderer);
    shader_program_start(renderer->static_shader);
    static_shader_load_light(renderer->static_shader, sun);
    static_shader_load_view_matrix(renderer->static_shader, camera);
    entity_renderer_render(renderer->renderer, renderer->entities);
    shader_program_stop(renderer->static_shader);
    hmfree(renderer->entities);
}

void master_renderer_process_entity(master_renderer_t* renderer, entity_t entity) {
    textured_model_t entity_model = entity.model;
    entity_map_t* entry = hmgetp_null(renderer->entities, entity_model);
    if (entry != NULL) {
        BUFFER_PUSH(&entry->value, entity);
    } else {
        entity_buffer_t new_batch = BUFFER_INIT;
        BUFFER_PUSH(&new_batch, entity);
        hmput(renderer->entities, entity_model, new_batch);
    }
}

void master_renderer_clean_up(master_renderer_t* renderer) {
    shader_program_cleanup(renderer->static_shader);
}
