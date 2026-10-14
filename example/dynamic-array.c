#define UM_IMPLEMENTATION
#include "../um.h"

int main(void) {
    f64 *arr;
    size_t i;
    arr = uda_new(sizeof(f64));

    for (i = 0; i < 500; ++i) {
        uda_append(arr,  i);
        printf("arr[%zu] = %f\n", i, arr[i]);
    }

    uda_destroy(arr);
    return 0;
}
