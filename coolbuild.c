#include "coolbuild.h"

const char* const glad_src[] = {
        "glad/src/glad.c",
};

const char* const game_engine_src[] = {
        "source/entities/camera.c",
        "source/entities/entity.c",
        "source/render_engine/display_manager.c",
        "source/render_engine/entity_renderer.c",
        "source/render_engine/loader.c",
        "source/render_engine/master_renderer.c",
        "source/render_engine/obj_loader.c",
        "source/render_engine/terrain_renderer.c",
        "source/shaders/shader_program.c",
        "source/shaders/static_shader.c",
        "source/shaders/terrain_shader.c",
        "source/terrains/terrain.c",
        "source/textures/model_texture.c",
        "source/toolbox/math.c",
        "source/toolbox/string.c",
        "source/stb_ds.c",
        "source/stb_image.c",
};
#define GAME_ENGINE_CFLAGS \
    "-Wall", "-Wextra", "-Wpedantic", "-std=gnu99", "-Iinclude", "-Iglad/include", \
            "-Ibuild/include", "-Icglm/include", "-ggdb3", "-DSTBDS_NO_SHORT_NAMES"

#define GAME_ENGINE_LIBS "-lglfw", "-lglad", "-lcglm", "-lm", "-ldl"
#define GAME_ENGINE_LIBS_FMT "vvvvv"

const char* const game_engine_tester_src[] = {
        "source/engine_tester/main.c",
};

const char* cc(void) {
    return "gcc";
}

void build_glad(void) {
    MKDIRS("build", "obj", "glad", "src");
    const char** glad_obj;
    COMPILE_OBJECTS(glad_src, "-Iglad/include", &glad_obj);
    char** glad_lib = collect_args("vvvp", "ar", "rcs", "build/libglad.a", glad_obj);
    run_cmd(glad_lib);
}

void build_cglm(void) {
    CMD("cmake", "-B", "build/cglm", "-S", "cglm", "-DCGLM_SHARED=OFF", "-DCMAKE_BUILD_TYPE=Debug");
    CMD("cmake", "--build", "build/cglm");
}

void build_game_engine(void) {
    MKDIRS("build", "obj", "source", "entities");
    MKDIRS("build", "obj", "source", "render_engine");
    MKDIRS("build", "obj", "source", "shaders");
    MKDIRS("build", "obj", "source", "terrains");
    MKDIRS("build", "obj", "source", "textures");
    MKDIRS("build", "obj", "source", "toolbox");
    MKDIRS("build", "include");

    CONFIGURE_FILE("config.h.coolbuild_in", "build/include/config.h", "DATA_ROOT_PATH",
            PATH(curdir(), "data"));

    const char** game_engine_obj;
    COMPILE_OBJECTS(game_engine_src, GAME_ENGINE_CFLAGS, &game_engine_obj);
    char** game_engine_lib =
            collect_args("vvvp", "ar", "rcs", "build/libgame_engine.a", game_engine_obj);
    run_cmd(game_engine_lib);
}

void build_game_engine_tester(void) {
    MKDIRS("build", "obj", "source", "engine_tester");

    const char** game_engine_tester_obj;
    COMPILE_OBJECTS(game_engine_tester_src, GAME_ENGINE_CFLAGS, &game_engine_tester_obj);
    char** game_engine_tester_link_cmd = collect_args("vpvvvvv" GAME_ENGINE_LIBS_FMT, cc(),
            game_engine_tester_obj, "-o", "build/game_engine_tester", "-Lbuild", "-Lbuild/cglm",
            "-lgame_engine", GAME_ENGINE_LIBS);
    run_cmd(game_engine_tester_link_cmd);
}

int main(int argc, char** argv) {
    REBUILD_SELF(argc, argv);

    build_glad();
    build_cglm();
    build_game_engine();
    build_game_engine_tester();

    if (argc > 1) {
        if (strcmp(argv[1], "run") == 0) {
            CMD("build/game_engine_tester");
        } else {
            fprintf(stderr, "Unknown command: %s\n", argv[1]);
            return 1;
        }
    }
}
