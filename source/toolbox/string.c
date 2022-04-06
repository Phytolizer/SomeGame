#include "game_engine/toolbox/string.h"

#include <stddef.h>
#include <string.h>

bool string_starts_with(const char* string, const char* prefix) {
    size_t prefix_length = strlen(prefix);
    size_t string_length = strlen(string);
    if (prefix_length > string_length) {
        return false;
    }

    return memcmp(string, prefix, prefix_length) == 0;
}
