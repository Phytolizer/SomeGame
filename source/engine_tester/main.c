#include "config.h"
#include "game_engine/models/textured_model.h"
#include "game_engine/render_engine/display_manager.h"
#include "game_engine/render_engine/loader.h"
#include "game_engine/render_engine/master_renderer.h"
#include "game_engine/render_engine/obj_loader.h"
#include "game_engine/render_engine/renderer.h"
#include "game_engine/shaders/static_shader.h"
#include "game_engine/textures/model_texture.h"

int main(void) {
    display_manager_t display_manager = display_manager_create_display();
    loader_t loader = loader_new();
    shader_program_t* shader = static_shader_new();
    renderer_t renderer = renderer_new(&display_manager, shader);

    raw_model_t model = load_obj_model(DATA_ROOT_PATH "/models/dragon.obj", &loader);
    model_texture_t texture =
            model_texture_new(loader_load_texture(&loader, DATA_ROOT_PATH "/textures/purple.png"));
    textured_model_t static_model = {
            .raw_model = model,
            .texture = texture,
    };
    texture.shine_damper = 10.0f;
    texture.reflectivity = 0.3f;
    entity_t entity = {
            .model = static_model,
            .position = {0.0f, -3.0f, -7.0f},
            .rotation = {0.0f, 0.0f, 0.0f},
            .scale = 1.0f,
    };
    light_t light = {
            .position = {-6.0f, 4.0f, 10.0f},
            .color = {0.5f, 0.5f, 0.5f},
    };
    camera_t camera = camera_new(&display_manager);

    master_renderer_t master_renderer = {
            .renderer = &renderer,
            .entities = NULL,
            .static_shader = shader,
    };

    while (!glfwWindowShouldClose(display_manager.glfw_window)) {
        entity_increase_rotation(&entity, (vec3){0.0f, 1.0f, 0.0});
        camera_move(&camera);
        master_renderer_process_entity(&master_renderer, entity);
        master_renderer_render(&master_renderer, light, camera);
        display_manager_update_display(display_manager);
    }
    shader_program_cleanup(shader);
    free(shader);
    master_renderer_clean_up(&master_renderer);
    loader_cleanup(&loader);
    display_manager_close_display(display_manager);
}
