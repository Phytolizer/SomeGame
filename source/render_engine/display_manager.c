#include "game_engine/render_engine/display_manager.h"

#include "game_engine/gl.h"

#include <stdio.h>
#include <stdlib.h>

enum {
    width = 1920,
    height = 1080,
};

static void glfw_error_callback(int error, const char* description) {
    (void)error;
    fprintf(stderr, "GLFW error: %s\n", description);
}

display_manager_t display_manager_create_display(void) {
    display_manager_t result;

    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    result.glfw_window = glfwCreateWindow(width, height, "Game Engine", NULL, NULL);
    if (result.glfw_window == NULL) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(result.glfw_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSwapInterval(1);
    glViewport(0, 0, width, height);

    return result;
}

void display_manager_update_display(display_manager_t display_manager) {
    glfwPollEvents();
    glfwSwapBuffers(display_manager.glfw_window);
}

void display_manager_close_display(display_manager_t display_manager) {
    glfwDestroyWindow(display_manager.glfw_window);
    glfwTerminate();
}
