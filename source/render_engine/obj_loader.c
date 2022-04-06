#include "game_engine/render_engine/obj_loader.h"

#include "buffer.h"
#include "cglm/vec2.h"
#include "cglm/vec3.h"
#include "game_engine/toolbox/string.h"

#include <stdio.h>
#include <string.h>

typedef BUFFER_TYPE(vec3) vec3_buffer_t;
typedef BUFFER_TYPE(vec2) vec2_buffer_t;

raw_model_t load_obj_model(const char* file_name, loader_t* loader) {
    FILE* fp = fopen(file_name, "r");
    if (fp == NULL) {
        fprintf(stderr, "Couldn't load file!\n");
        exit(EXIT_FAILURE);
    }

    vec3_buffer_t vertices = BUFFER_INIT;
    vec2_buffer_t textures = BUFFER_INIT;
    vec3_buffer_t normals = BUFFER_INIT;
    uint_buffer_t indices = BUFFER_INIT;
    float_span_t vertex_array = {0};
    float_span_t normal_array = {0};
    float_span_t texture_array = {0};
    uint_span_t index_array = {0};
    char* line = NULL;
    size_t line_cap = 0;
    ssize_t line_len;
    while ((line_len = getline(&line, &line_cap, fp)) != -1) {
        char* token_buffer = strdup(line);
        string_array_buffer_t tokens = BUFFER_INIT;
        for (char* token = strtok(token_buffer, " \t\r\n"); token != NULL;
                token = strtok(NULL, " \t\r\n")) {
            BUFFER_PUSH(&tokens, token);
        }
        if (string_starts_with(line, "v ")) {
            vec3 vertex;
            sscanf(line, "v %f %f %f", &vertex[0], &vertex[1], &vertex[2]);
            BUFFER_EXPAND(&vertices);
            memcpy(vertices.data[vertices.length], vertex, sizeof(vertex));
            vertices.length++;
        } else if (string_starts_with(line, "vt ")) {
            vec2 texture;
            sscanf(line, "vt %f %f", &texture[0], &texture[1]);
            BUFFER_EXPAND(&textures);
            memcpy(textures.data[textures.length], texture, sizeof(texture));
            textures.length++;
        } else if (string_starts_with(line, "vn ")) {
            vec3 normal;
            sscanf(line, "vn %f %f %f", &normal[0], &normal[1], &normal[2]);
            BUFFER_EXPAND(&normals);
            memcpy(normals.data[normals.length], normal, sizeof(normal));
            normals.length++;
        } else if (string_starts_with(line, "f ")) {
            texture_array.length = vertices.length * 2;
            texture_array.begin = malloc(texture_array.length * sizeof(float));
            normal_array.length = vertices.length * 3;
            normal_array.begin = malloc(normal_array.length * sizeof(float));
            break;
        }
    }

    while ((line_len = getline(&line, &line_cap, fp)) != -1) {
        if (!string_starts_with(line, "f ")) {
            continue;
        }

        unsigned int vertices[3][3];
        sscanf(line, "f %u/%u/%u %u/%u/%u %u/%u/%u", &vertices[0][0], &vertices[0][1],
                &vertices[0][2], &vertices[1][0], &vertices[1][1], &vertices[1][2], &vertices[2][0],
                &vertices[2][1], &vertices[2][2]);
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                vertices[i][j]--;
            }
        }

        for (int i = 0; i < 3; i++) {
            unsigned int vertex_index = vertices[i][0];
            BUFFER_PUSH(&indices, vertex_index);
            vec2 current_texture;
            glm_vec2_copy(textures.data[vertices[i][1]], current_texture);
            texture_array.begin[vertex_index * 2] = current_texture[0];
            texture_array.begin[vertex_index * 2 + 1] = 1.0f - current_texture[1];
            vec3 current_normal;
            glm_vec3_copy(normals.data[vertices[i][2]], current_normal);
            normal_array.begin[vertex_index * 3] = current_normal[0];
            normal_array.begin[vertex_index * 3 + 1] = current_normal[1];
            normal_array.begin[vertex_index * 3 + 2] = current_normal[2];
        }
    }

    vertex_array.length = vertices.length * 3;
    vertex_array.begin = malloc(vertex_array.length * sizeof(float));
    index_array.length = indices.length;
    index_array.begin = malloc(index_array.length * sizeof(unsigned int));

    unsigned int vertex_pointer = 0;
    for (size_t i = 0; i < vertices.length; i++) {
        vertex_array.begin[vertex_pointer] = vertices.data[i][0];
        vertex_pointer++;
        vertex_array.begin[vertex_pointer] = vertices.data[i][1];
        vertex_pointer++;
        vertex_array.begin[vertex_pointer] = vertices.data[i][2];
        vertex_pointer++;
    }

    memcpy(index_array.begin, indices.data, indices.length * sizeof(unsigned int));

    return loader_load_to_vao(loader, vertex_array, texture_array, normal_array, index_array);
}
