#include "game_engine/models/textured_model.h"
#include "game_engine/render_engine/display_manager.h"
#include "game_engine/render_engine/loader.h"
#include "game_engine/render_engine/renderer.h"
#include "game_engine/shaders/static_shader.h"
#include "game_engine/textures/model_texture.h"

int main(void) {
    display_manager_t display_manager = display_manager_create_display();
    loader_t loader = loader_new();
    shader_program_t* shader = static_shader_new();
    renderer_t renderer = renderer_new(&display_manager, shader);

    // clang-format off
    float vertices[] = {
            // v0
            -0.5f, 0.5f, 0.0f,
            // v1
            -0.5f, -0.5f, 0.0f,
            // v2
            0.5f, -0.5f, 0.0f,
            // v3
            0.5f, 0.5f, 0.0f,
    };
    // clang-format on

    unsigned int indices[] = {0, 1, 3, 3, 1, 2};

    // clang-format off
    float texture_coords[] = {
            // v0
            0.0f, 0.0f,
            // v1
            0.0f, 1.0f,
            // v2
            1.0f, 1.0f,
            // v3
            1.0f, 0.0f,
    };
    // clang-format on

    raw_model_t model = loader_load_to_vao(&loader, (float_span_t)SPAN_FROM_ARRAY(vertices),
            (float_span_t)SPAN_FROM_ARRAY(texture_coords), (uint_span_t)SPAN_FROM_ARRAY(indices));
    model_texture_t texture = {
            .texture_id = loader_load_texture(&loader, "data/textures/texture.png"),
    };
    textured_model_t static_model = {
            .raw_model = model,
            .texture = texture,
    };
    entity_t entity = {
            .model = static_model,
            .position = {-1.0f, 0.0f, 0.0f},
            .rotation = {0.0f, 0.0f, 0.0f},
            .scale = 1.0f,
    };

    while (!glfwWindowShouldClose(display_manager.glfw_window)) {
        entity_increase_position(&entity, (vec3){0.0f, 0.0f, -0.002f});
        renderer_prepare(&renderer);
        shader_program_start(shader);
        renderer_render(&renderer, entity, shader);
        shader_program_stop(shader);
        display_manager_update_display(display_manager);
    }
    shader_program_cleanup(shader);
    free(shader);
    loader_cleanup(&loader);
    display_manager_close_display(display_manager);
}
