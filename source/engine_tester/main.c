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
            -0.5f,0.5f,-0.5f,	
            -0.5f,-0.5f,-0.5f,	
            0.5f,-0.5f,-0.5f,	
            0.5f,0.5f,-0.5f,		
            
            -0.5f,0.5f,0.5f,	
            -0.5f,-0.5f,0.5f,	
            0.5f,-0.5f,0.5f,	
            0.5f,0.5f,0.5f,
            
            0.5f,0.5f,-0.5f,	
            0.5f,-0.5f,-0.5f,	
            0.5f,-0.5f,0.5f,	
            0.5f,0.5f,0.5f,
            
            -0.5f,0.5f,-0.5f,	
            -0.5f,-0.5f,-0.5f,	
            -0.5f,-0.5f,0.5f,	
            -0.5f,0.5f,0.5f,
            
            -0.5f,0.5f,0.5f,
            -0.5f,0.5f,-0.5f,
            0.5f,0.5f,-0.5f,
            0.5f,0.5f,0.5f,
            
            -0.5f,-0.5f,0.5f,
            -0.5f,-0.5f,-0.5f,
            0.5f,-0.5f,-0.5f,
            0.5f,-0.5f,0.5f
            
    };
    
    float texture_coords[] = {
            0,0,
            0,1,
            1,1,
            1,0,			
            0,0,
            0,1,
            1,1,
            1,0,			
            0,0,
            0,1,
            1,1,
            1,0,
            0,0,
            0,1,
            1,1,
            1,0,
            0,0,
            0,1,
            1,1,
            1,0,
            0,0,
            0,1,
            1,1,
            1,0,
    };
    
    unsigned int indices[] = {
            0,1,3,	
            3,1,2,	
            4,5,7,
            7,5,6,
            8,9,11,
            11,9,10,
            12,13,15,
            15,13,14,	
            16,17,19,
            19,17,18,
            20,21,23,
            23,21,22,
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
            .position = {0.0f, 0.0f, -5.0f},
            .rotation = {0.0f, 0.0f, 0.0f},
            .scale = 1.0f,
    };
    camera_t camera = camera_new(&display_manager);

    while (!glfwWindowShouldClose(display_manager.glfw_window)) {
        entity_increase_rotation(&entity, (vec3){1.0f, 1.0f, 0.0});
        camera_move(&camera);
        renderer_prepare(&renderer);
        shader_program_start(shader);
        static_shader_load_view_matrix(shader, &camera);
        renderer_render(&renderer, entity, shader);
        shader_program_stop(shader);
        display_manager_update_display(display_manager);
    }
    shader_program_cleanup(shader);
    free(shader);
    loader_cleanup(&loader);
    display_manager_close_display(display_manager);
}
