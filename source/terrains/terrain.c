#include "game_engine/terrains/terrain.h"

#define SIZE 800.0f
#define VERTEX_COUNT 128

static raw_model_t generate_terrain(loader_t* loader);

terrain_t terrain_new(int grid_x, int grid_z, loader_t* loader, model_texture_t texture) {
    terrain_t result = {
            .texture = texture,
            .x = grid_x * SIZE,
            .z = grid_z * SIZE,
            .model = generate_terrain(loader),
    };
    return result;
}

static raw_model_t generate_terrain(loader_t* loader) {
    size_t count = VERTEX_COUNT * VERTEX_COUNT;
    size_t vertex_count = count * 3;
    float* vertices = malloc(vertex_count * sizeof(float));
    size_t normal_count = count * 3;
    float* normals = malloc(normal_count * sizeof(float));
    size_t texcoord_count = count * 2;
    float* texture_coordinates = malloc(texcoord_count * sizeof(float));
    size_t index_count = 6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1);
    unsigned int* indices = malloc(index_count * sizeof(unsigned int));
    size_t vertex_pointer = 0;
    for (size_t i = 0; i < VERTEX_COUNT; i++) {
        for (size_t j = 0; j < VERTEX_COUNT; j++) {
            vertices[vertex_pointer * 3] = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
            vertices[vertex_pointer * 3 + 1] = 0.0f;
            vertices[vertex_pointer * 3 + 2] = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
            normals[vertex_pointer * 3] = 0.0f;
            normals[vertex_pointer * 3 + 1] = 1.0f;
            normals[vertex_pointer * 3 + 2] = 0.0f;
            texture_coordinates[vertex_pointer * 2] = (float)j / ((float)VERTEX_COUNT - 1);
            texture_coordinates[vertex_pointer * 2 + 1] = (float)i / ((float)VERTEX_COUNT - 1);
            vertex_pointer++;
        }
    }
    size_t pointer = 0;
    for (size_t gz = 0; gz < VERTEX_COUNT - 1; gz++) {
        for (size_t gx = 0; gx < VERTEX_COUNT - 1; gx++) {
            unsigned int top_left = (gz * VERTEX_COUNT) + gx;
            unsigned int top_right = top_left + 1;
            unsigned int bottom_left = ((gz + 1) * VERTEX_COUNT) + gx;
            unsigned int bottom_right = bottom_left + 1;
            indices[pointer++] = top_left;
            indices[pointer++] = bottom_left;
            indices[pointer++] = top_right;
            indices[pointer++] = top_right;
            indices[pointer++] = bottom_left;
            indices[pointer++] = bottom_right;
        }
    }
    return loader_load_to_vao(loader, (float_span_t)SPAN_WITH_LENGTH(vertices, vertex_count),
            (float_span_t)SPAN_WITH_LENGTH(normals, normal_count),
            (float_span_t)SPAN_WITH_LENGTH(texture_coordinates, texcoord_count),
            (uint_span_t)SPAN_WITH_LENGTH(indices, index_count));
}
