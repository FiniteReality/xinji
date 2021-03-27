#include "stdio.h"
#include "inttypes.h"

#include "xinji.h"

#define ERROR(fmt, ...) \
    fprintf(stderr, "evaluate: " fmt, ## __VA_ARGS__)

void usage()
{
    fprintf(stderr,
        "Usage: evaluate [OPTION]... [FILE]...\n"
        "Evaluates one or more Xinji FILE(s).\n"
        "\n"
        "  No. There are no options yet.\n");
}

void reporterror(xinji_state* state, const char* message)
{
    ERROR("%s\n", message);
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        usage();
        return 1;
    }

    xinji_state* state = xinji_open();

    xinji_set_error_reporter(state, reporterror);

    if (state == NULL)
    {
        ERROR("failed to open xinji_state");
        return 1;
    }

    for (int n = 1; n < argc; n++)
    {
        printf("parsing %s...\n", argv[n]);
        if (!xinji_parse_file(state, argv[n]))
            break;
    }

    xinji_close(state);
    return 0;
}
