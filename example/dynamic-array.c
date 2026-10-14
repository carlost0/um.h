#define UM_IMPLEMENTATION
#include "../um.h"

int main(void) {
    size_t *arr;
    size_t i;
    arr = uda_new(sizeof(size_t));

    uda_append(arr, 0);
    uda_append(arr, 1);

    for (i = 1; i < 50; ++i) {
        uda_append(arr,  arr[i] + arr[i - 1]);
        ulog_printf(ULOG_INFO, "fib(%zu) = %zu", i, arr[i]);
    }

    uda_destroy(arr);
    return 0;
}
