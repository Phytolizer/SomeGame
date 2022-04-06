#pragma once

#include "game_engine/models/raw_model.h"
#include "game_engine/render_engine/loader.h"

raw_model_t load_obj_model(const char* file_name, loader_t* loader);
