#ifndef XINJI_H
#define XINJI_H

#include "xinji_export.h"

typedef struct xinji_state xinji_state;
typedef void (*xinji_error_report_fn)(xinji_state*, const char*);

XINJI_EXPORT xinji_state* xinji_open();
XINJI_EXPORT void xinji_close(xinji_state* state);

XINJI_EXPORT xinji_error_report_fn xinji_set_error_reporter(xinji_state* state,
    xinji_error_report_fn fn);

XINJI_EXPORT int xinji_parse_file(xinji_state* state, const char* location);

#endif /* XINJI_H */
