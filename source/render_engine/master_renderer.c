#include "game_engine/render_engine/master_renderer.h"

#include "game_engine/shaders/shader_program.h"
#include "game_engine/shaders/static_shader.h"
#include "game_engine/shaders/terrain_shader.h"
#include "game_engine/toolbox/math.h"
#include "stb_ds.h"

#define FOV 70
#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.0f

static void create_projection_matrix(master_renderer_t* renderer);

master_renderer_t master_renderer_new(const display_manager_t* display_manager) {
    master_renderer_t renderer;
    renderer.display_manager = display_manager;
    create_projection_matrix(&renderer);
    renderer.static_shader = static_shader_new();
    renderer.entity_renderer =
            entity_renderer_new(renderer.static_shader, renderer.projection_matrix);
    renderer.terrain_shader = terrain_shader_new();
    renderer.terrain_renderer =
            terrain_renderer_new(renderer.terrain_shader, renderer.projection_matrix);
    renderer.entities = NULL;
    renderer.terrains = (terrain_buffer_t)BUFFER_INIT;
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    return renderer;
}

void master_renderer_render(master_renderer_t* renderer, light_t sun, camera_t camera) {
    master_renderer_prepare(renderer);
    shader_program_start(renderer->static_shader);
    static_shader_load_light(renderer->static_shader, sun);
    static_shader_load_view_matrix(renderer->static_shader, camera);
    entity_renderer_render(&renderer->entity_renderer, renderer->entities);
    shader_program_stop(renderer->static_shader);
    shader_program_start(renderer->terrain_shader);
    terrain_shader_load_light(renderer->terrain_shader, sun);
    terrain_shader_load_view_matrix(renderer->terrain_shader, camera);
    terrain_renderer_render(
            &renderer->terrain_renderer, (terrain_span_t)BUFFER_AS_SPAN(renderer->terrains));
    shader_program_stop(renderer->terrain_shader);
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

void master_renderer_process_terrain(master_renderer_t* renderer, terrain_t terrain) {
    BUFFER_PUSH(&renderer->terrains, terrain);
}

void master_renderer_prepare(master_renderer_t* renderer) {
    (void)renderer;
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void master_renderer_clean_up(master_renderer_t* renderer) {
    shader_program_cleanup(renderer->static_shader);
    free(renderer->static_shader);
    shader_program_cleanup(renderer->terrain_shader);
    free(renderer->terrain_shader);
}

static void create_projection_matrix(master_renderer_t* renderer) {
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
