#define UM_IMPLEMENTATION
#include "../um.h"

int main(void) {
    uar arena;
    char *src, *dst;

    arena = uar_new();

    src = uar_alloc(&arena, 20 * sizeof(char));
    dst = uar_alloc(&arena, 20 * sizeof(char));

    strcpy(src, "hello, ");
    strcpy(dst, "world!\n");

    strcat(src, dst);

    printf("%s", src);

    uar_destroy(&arena);
    return 0;
}
