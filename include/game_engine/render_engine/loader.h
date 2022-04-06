#pragma once

#include "buffer.h"
#include "game_engine/models/raw_model.h"
#include "span.h"

typedef BUFFER_TYPE(GLuint) gluint_buffer_t;

typedef struct {
    gluint_buffer_t vaos;
    gluint_buffer_t vbos;
    gluint_buffer_t textures;
} loader_t;

loader_t loader_new(void);
raw_model_t loader_load_to_vao(loader_t* loader, float_span_t positions,
        float_span_t texture_coordinates, float_span_t normals, uint_span_t indices);
GLuint loader_create_vao(loader_t* loader);
void loader_store_data_in_attribute_list(
        loader_t* loader, GLuint attribute_number, GLint coordinate_size, float_span_t data);
void loader_unbind_vao(loader_t* loader);
void loader_bind_index_buffer(loader_t* loader, uint_span_t indices);
GLuint loader_load_texture(loader_t* loader, const char* file_name);
void loader_cleanup(loader_t* loader);
