#include "game_engine/textures/model_texture.h"

model_texture_t model_texture_new(GLuint texture_id) {
    model_texture_t model_texture = {
            .texture_id = texture_id,
            .shine_damper = 1.0f,
            .reflectivity = 0.0f,
    };
    return model_texture;
}
