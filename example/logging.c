#define UM_IMPLEMENTATION
#include <errno.h>
#include "../um.h"

int main(void) {
    FILE *f;
    const char *filename;

    ulog_print(ULOG_INFO, "im an info");
    ulog_print(ULOG_WARN, "im a warning");
    ulog_print(ULOG_ERROR, "im an error");

    filename = "logs.log";
    f = fopen(filename, "w");

    if (!f) {
        ulog_printf(ULOG_ERROR, "unable to open file: %s", strerror(errno));
        return 1;
    }

    ulog_printf(ULOG_INFO, "opened file %s", filename);

    ulog_fprint(f, ULOG_INFO, "im an info");
    ulog_fprint(f, ULOG_WARN, "im a warning");
    ulog_fprint(f, ULOG_ERROR, "im an error");

    fclose(f);
    return 0;
}
