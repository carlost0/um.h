#define UM_IMPLEMENTATION
#include "../um.h"

int main(void) {
    usb contents;
    usv tschüss, hallo, zahlen;
    usb hello, world;
    char *input;

    contents.size  = 0;
    contents.cap   = 0;
    contents.data  = NULL;

    zahlen = usv_from_cstr("Hallo Tschüss 123");
    hallo = usv_tok(&zahlen, " ");
    tschüss = usv_tok(&zahlen, " ");

    ulog_printf(ULOG_INFO, USV_FMT, USV_ARG(hallo));
    ulog_printf(ULOG_INFO, USV_FMT, USV_ARG(tschüss));
    ulog_printf(ULOG_INFO, USV_FMT, USV_ARG(zahlen));

    if (!usb_from_file("Makefile", &contents)) {
        ulog_print(ULOG_ERROR, "unable to open file");
        return 1;
    }

    ulog_printf(ULOG_INFO, "contents =>\n" USV_FMT, USV_ARG(contents));
    ulog_printf(ULOG_INFO, "size => %zu", contents.size);

    hello.size  = 0;
    hello.cap   = 0;
    hello.data  = NULL;

    world.size  = 0;
    world.cap   = 0;
    world.data  = NULL;

    usb_append_usv(&hello, usv_from_cstr("hello "));
    usb_append_cstr(&world, "world");

    ulog_printf(ULOG_INFO, USV_FMT, USV_ARG(hello));
    ulog_printf(ULOG_INFO, USV_FMT, USV_ARG(world));

    free(contents.data);
    free(hello.data);
    free(world.data);

    printf("wie heißt du? ");
    fflush(stdout);
    input = ucstr_read_stream_until(stdin, "\n");
    ulog_printf(ULOG_INFO, "hallo %s", input);

    free(input);
    return 0;
}
