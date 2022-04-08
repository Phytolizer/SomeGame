#include "game_engine/render_engine/loader.h"

#include "stb_image.h"

loader_t loader_new(void) {
    loader_t loader = {
            .vaos = BUFFER_INIT,
            .vbos = BUFFER_INIT,
            .textures = BUFFER_INIT,
    };
    return loader;
}

raw_model_t loader_load_to_vao(loader_t* loader, float_span_t positions,
        float_span_t texture_coordinates, float_span_t normals, uint_span_t indices) {
    GLuint vao_id = loader_create_vao(loader);
    loader_bind_index_buffer(loader, indices);
    loader_store_data_in_attribute_list(loader, 0, 3, positions);
    loader_store_data_in_attribute_list(loader, 1, 2, texture_coordinates);
    loader_store_data_in_attribute_list(loader, 2, 3, normals);
    loader_unbind_vao(loader);
    return (raw_model_t){
            .vao_id = vao_id,
            .vertex_count = indices.length,
    };
}

GLuint loader_create_vao(loader_t* loader) {
    GLuint vao_id;
    glGenVertexArrays(1, &vao_id);
    BUFFER_PUSH(&loader->vaos, vao_id);
    glBindVertexArray(vao_id);
    return vao_id;
}

void loader_store_data_in_attribute_list(
        loader_t* loader, GLuint attribute_number, GLint coordinate_size, float_span_t data) {
    GLuint vbo_id;
    glGenBuffers(1, &vbo_id);
    BUFFER_PUSH(&loader->vbos, vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, data.length * sizeof(float), data.begin, GL_STATIC_DRAW);
    glVertexAttribPointer(attribute_number, coordinate_size, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void loader_unbind_vao(loader_t* loader) {
    (void)loader;
    glBindVertexArray(0);
}

void loader_bind_index_buffer(loader_t* loader, uint_span_t indices) {
    GLuint vbo_id;
    glGenBuffers(1, &vbo_id);
    BUFFER_PUSH(&loader->vbos, vbo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.length * sizeof(unsigned int), indices.begin,
            GL_STATIC_DRAW);
}

GLuint loader_load_texture(loader_t* loader, const char* file_name) {
    int width;
    int height;
    int channels;
    void* pixels = stbi_load(file_name, &width, &height, &channels, STBI_rgb_alpha);
    if (pixels == NULL) {
        printf("Failed to load texture %s: %s\n", file_name, stbi_failure_reason());
        return 0;
    }

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);

    BUFFER_PUSH(&loader->textures, texture_id);
    stbi_image_free(pixels);
    return texture_id;
}

void loader_cleanup(loader_t* loader) {
    for (size_t i = 0; i < loader->vaos.length; i++) {
        glDeleteVertexArrays(1, &loader->vaos.data[i]);
    }
    for (size_t i = 0; i < loader->vbos.length; i++) {
        glDeleteBuffers(1, &loader->vbos.data[i]);
    }
    for (size_t i = 0; i < loader->textures.length; i++) {
        glDeleteTextures(1, &loader->textures.data[i]);
    }
    free(loader->vaos.data);
    free(loader->vbos.data);
    free(loader->textures.data);
}
