#include "coolbuild.h"

const char* const glad_src[] = {
        "glad/src/glad.c",
};

const char* const game_engine_src[] = {
        "source/render_engine/display_manager.c",
        "source/render_engine/loader.c",
        "source/render_engine/renderer.c",
        "source/shaders/shader_program.c",
        "source/shaders/static_shader.c",
        "source/stb_image.c",
};
#define GAME_ENGINE_CFLAGS \
    "-Wall", "-Wextra", "-Wpedantic", "-std=gnu99", "-Iinclude", "-Iglad/include", \
            "-Ibuild/include", "-ggdb3"

#define GAME_ENGINE_LIBS "-lglfw"
#define GAME_ENGINE_LIBS_FMT "v"

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

void build_game_engine(void) {
    MKDIRS("build", "obj", "source", "render_engine");
    MKDIRS("build", "obj", "source", "shaders");
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
    char** game_engine_tester_link_cmd = collect_args("vpvvvvvvv", cc(), game_engine_tester_obj,
            "-o", "build/game_engine_tester", "-Lbuild", "-lglfw", "-lgame_engine", "-lglad",
            "-lm");
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
