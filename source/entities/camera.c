#include "game_engine/entities/camera.h"

camera_t camera_new(const display_manager_t* display_manager) {
    camera_t result = {
            .display_manager = display_manager,
            .position = {0.0f, 0.0f, 0.0f},
            .pitch = 0.0f,
            .yaw = 0.0f,
            .roll = 0.0f,
    };

    return result;
}

void camera_move(camera_t* camera) {
    if (glfwGetKey(camera->display_manager->glfw_window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->position[2] -= 0.02f;
    }
    if (glfwGetKey(camera->display_manager->glfw_window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->position[0] += 0.02f;
    }
    if (glfwGetKey(camera->display_manager->glfw_window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->position[0] -= 0.02f;
    }
    if (glfwGetKey(camera->display_manager->glfw_window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->position[2] += 0.02f;
    }
}
