#pragma once

#include "game_engine/gl.h"

typedef struct {
    GLFWwindow* glfw_window;
    int width;
    int height;
} display_manager_t;

display_manager_t display_manager_create_display(void);
void display_manager_update_display(display_manager_t display_manager);
void display_manager_close_display(display_manager_t display_manager);
