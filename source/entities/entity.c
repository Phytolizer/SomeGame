#include "game_engine/entities/entity.h"

void entity_increase_position(entity_t* entity, vec3 delta) {
    glm_vec3_add(entity->position, delta, entity->position);
}

void entity_increase_rotation(entity_t* entity, vec3 delta) {
    glm_vec3_add(entity->rotation, delta, entity->rotation);
}
