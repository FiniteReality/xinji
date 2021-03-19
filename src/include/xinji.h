#ifndef XINJI_H
#define XINJI_H

#include "xinji_export.h"

typedef struct xinji_state xinji_state;

XINJI_EXPORT xinji_state* xinji_open();
XINJI_EXPORT void xinji_close(xinji_state* state);

XINJI_EXPORT void xinji_parse_file(xinji_state* state, const char* location);

#endif
