/*
 * um.h: small single header utility library compatible with ISO C90
 * including better strings, dynamic arrays and arena allocators.
 */

/*
 * note by the software's author (me):
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR ANY PARTICULAR PURPOSE.
 */

/*
 * DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 * Version 2, December 2004
 *
 * Copyright (C) 2026 Carlos G. S.
 *
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *
 * DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 * TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 * 0. You just DO WHAT THE FUCK YOU WANT TO.
 *
 * Contact: Instagram DMs @cgs.dev
 */

/*
 * configuration macros:

 *   #define UMDEF:
 *    set the way functions should be declared
 *    default: 'static inline' for C99+ and 'static' for C90/C89
 *
 *   #define UDA_INIT_CAP:
 *     set the um.h dynamic array initial capacity 
 *     default: 256
 *
 *   #define UAR_INIT_CAP:
 *     set the um.h arena allocator initial capacity 
 *     default: 4096
 *
 *   #define UBUF_INIT_CAP:
 *     set the initial capacity for temporary buffers used in um.h
 *     default: 1024
 *
 *   #define ULOG_DEFAULT_STREAM:
 *     set the default file stream for the um.h logging module
 *     default: stderr
 *
 *   #define ULOG_NO_COLORS:
 *     disable colors for the um.h logging module
 */


#ifndef _UM_H
#define _UM_H

#include <stdlib.h>
#include <stdio.h>

#include <assert.h>
#include <limits.h>

#include <string.h>
#include <ctype.h>

#include <stdarg.h>

#if (!defined(__STDC_VERSION__) && !defined(__cplusplus))

typedef char  i8;
typedef short i16;
typedef int   i32;
typedef long  i64;

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long  u64;

#define UM_SIZE_MAX ((size_t) - 1)

#ifndef UMDEF
#define UMDEF static
#endif /* UMDEF */

#else 

#include <stdint.h>

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define UM_SIZE_MAX SIZE_MAX

#ifndef UMDEF
#define UMDEF static inline
#endif /* UMDEF */

#endif

typedef u8 ubool;

#define utrue 1
#define ufalse 0

typedef float  f32;
typedef double f64;

#ifndef UM_ASSERT
#define UM_ASSERT assert
#endif /* UM_ASSERT */


#ifndef UM_MALLOC
#define UM_MALLOC malloc
#endif /* UM_MALLOC */

#ifndef UM_REALLOC
#define UM_REALLOC realloc
#endif /* UM_REALLOC */

#ifndef UM_FREE
#define UM_FREE free
#endif /* UM_FREE */


#ifndef UM_MAX
#define UM_MAX(a, b) (((a) > b)? (a) : (b))
#endif /* UM_MAX */

#ifndef UM_MIN
#define UM_MIN(a, b) (((a) < (b))? (a) : (b))
#endif /* UM_MIN */

#ifndef UM_CLAMP
#define UM_CLAMP(n, min, max) (UM_MIN(UM_MAX((n), (min)), (max)))
#endif /* UM_CLAMP */


#ifndef UBUF_INIT_CAP
#define UBUF_INIT_CAP 1024
#endif /* UBUG_INIT_CAP */

#ifndef UDA_INIT_CAP
#define UDA_INIT_CAP 256
#endif /* UDA_INIT_CAP */

#ifndef UAR_INIT_CAP
#define UAR_INIT_CAP 4064
#endif /* UAR_INIT_CAP */



/*
 * *---------------------------------------------*
 * |       um.h dynamic array header type        |
 * *---------------------------------------------*
 */

typedef struct {
    size_t cap, size;
    size_t typesize;
} udah;



/*
 * *---------------------------------------------*
 * |               um.h string types             |
 * *---------------------------------------------*
 */

/* string view */
typedef struct {
    size_t size;
    const char *data;
} usv;

/* string builder (just a dynamic array) */
typedef struct {
    size_t size, cap;
    char *data;
} usb;



/*
 * *---------------------------------------------*
 * |               um.h arena type               |
 * *---------------------------------------------*
 */

typedef struct {
    u8 *start, *current;
    size_t size, cap;
} uar;



/*
 * *---------------------------------------------*
 * |             um.h loglevel type              |
 * *---------------------------------------------*
 */

typedef enum {
    ULOG_INFO,
    ULOG_WARN,
    ULOG_ERROR
} ulog_level;



/*
 * *---------------------------------------------*
 * |   um.h dynamic array function prototypes    |
 * *---------------------------------------------*
 */

UMDEF void *uda_new(size_t typesize);
UMDEF void *uda_reserve(void *da, size_t n);
UMDEF ubool uda_destroy(void *da);

#ifndef uda_size
#define uda_size(arr) (((udah *) (arr) - 1)->size)
#endif /* uda_size */

#ifndef uda_cap
#define uda_cap(arr)  (((udah *) (arr) - 1)->cap)
#endif /* uda_cap */

#ifndef uda_typesize
#define uda_typesize(arr)  (((udah *) (arr) - 1)->typesize)
#endif /* uda_typesize */

#ifndef uda_append
#define uda_append(arr, v) do { \
    if (uda_size(arr) >= uda_cap(arr)) { \
        (arr) = uda_reserve((arr), uda_cap(arr) * 2); \
    } \
    arr[uda_size(arr)++] = (v); \
} while (0);
#endif /* uda_append */

#ifndef uda_pop
#define uda_pop(arr) (UM_ASSERT(uda_size(arr) > 0), (arr)[--uda_size(arr)])
#endif /* uda_pop */



/*
 * *---------------------------------------------*
 * |        um.h string function prototypes      |
 * *---------------------------------------------*
 */

UMDEF usb    usb_from_cstr(char *cstr);
UMDEF usv    usv_from_cstr(const char *cstr);
UMDEF usv    usv_from_usb(const usb b);

UMDEF ubool  usb_from_file(const char *filename, usb *b);
UMDEF void   usb_read_stream_until(FILE *stream, usb *b, const char *delims);

UMDEF char  *ucstr_from_file(const char *filename);
UMDEF char  *ucstr_read_stream_until(FILE *stream, const char *delims);

UMDEF void   usb_reserve(usb *b, size_t n);
UMDEF void   usb_append_char(usb *b, char c);
UMDEF void   usb_append_cstr(usb *b, const char *cstr);
UMDEF void   usb_append_usv(usb *b, usv v);

UMDEF void   usv_chop_right(usv *v, size_t n);
UMDEF void   usv_chop_left(usv *v, size_t n);
UMDEF size_t usv_trim_left(usv *v);
UMDEF size_t usv_trim_right(usv *v);
UMDEF size_t usv_trim(usv *v);
UMDEF int    usv_cmp(usv a, usv b);
UMDEF ubool  usv_eq(usv a, usv b);
UMDEF ubool  usv_isnum(usv v);
UMDEF i32    usv_to_i32(usv v);
UMDEF usv    usv_tok(usv *v, const char *delims);

#ifndef SV_FMT
#define USV_FMT "%.*s"
#endif /* SV_FMT */

#ifndef SV_ARG
#define USV_ARG(s) (int)(s).size, (s).data
#endif /* SV_ARG */



/*
 * *---------------------------------------------*
 * |    um.h dynamic array function prototypes   |
 * *---------------------------------------------*
 */

UMDEF uar   uar_new(void);
UMDEF void *uar_alloc(uar *a, size_t size);
UMDEF void  uar_destroy(uar *a);



/*
 * *---------------------------------------------*
 * |       um.h logging function prototypes      |
 * *---------------------------------------------*
 */

#ifndef ULOG_DEFAULT_STREAM
#define ULOG_DEFAULT_STREAM stderr
#endif /* ULOG_DEFAULT_STREAM */

UMDEF ubool ulog_fprintf(FILE *stream, ulog_level level, const char *fmt, ...);
UMDEF ubool ulog_printf(ulog_level level, const char *fmt, ...);

UMDEF ubool ulog_fprint(FILE *stream, ulog_level level, const char *msg);
UMDEF ubool ulog_print(ulog_level level, const char *msg);

#endif /* _UM_H */



#define UM_IMPLEMENTATION
#ifdef UM_IMPLEMENTATION



/*
 * *---------------------------------------------*
 * |      um.h dynamic array implementation      |
 * *---------------------------------------------*
 */

UMDEF void *uda_new(size_t typesize) {
    udah *h;

    h = (udah*) malloc(typesize * UDA_INIT_CAP + sizeof(udah)) ;  
    UM_ASSERT(h);

    h->size     = 0;
    h->cap      = UDA_INIT_CAP;
    h->typesize = typesize;

    return h + 1;
}

UMDEF void *uda_reserve(void *arr, size_t n) {
    udah *h, *new_h;
    size_t new_cap;

    h = (udah*) arr - 1;
    new_cap = (h->cap)? h->cap : 1;

    if (h->cap >= n) {
        return arr;
    }

    while (new_cap < n) {
        if (new_cap > UM_SIZE_MAX / 2) {
            new_cap = n;
            break;
        }
        new_cap *= 2;
    }

    new_h = (udah *) UM_REALLOC(h, new_cap * h->typesize + sizeof(udah));

    UM_ASSERT(new_h);

    h = new_h;
    h->cap = new_cap;

    arr = h + 1;
    UM_ASSERT(arr);

    return arr;
}

UMDEF ubool uda_destroy(void *arr) {
    udah *h;

    if (!arr) {
        return ufalse;
    }
    
    h = (udah*) arr - 1;

    free(h);
    
    return utrue;
}



/*
 * *---------------------------------------------*
 * |          um.h string implementation         |
 * *---------------------------------------------*
 */

UMDEF usb usb_from_cstr(char *cstr) {
    usb res;

    res.data = cstr;
    res.size = strlen(cstr);
    res.cap  = res.size;

    return res;
}

UMDEF usv usv_from_usb(const usb b) {
    usv res;

    res.data = b.data;
    res.size = b.size;

    return res;
}

UMDEF void usb_reserve(usb *b, size_t n) {

    if (n == 0) {
        n = UDA_INIT_CAP;
    }

    b->cap = n;

    if (!b->data) {
        b->data = (char *) UM_MALLOC(b->cap * sizeof(char));
    } else {
        b->data = (char *) UM_REALLOC(b->data, b->cap * sizeof(char));
    }

    UM_ASSERT(b->data);
}

UMDEF void usb_append_char(usb *b, char c) {
    if (b->size >= b->cap) {
        usb_reserve(b, b->cap * 2);   
    }

    b->data[b->size++] = c;
}

UMDEF void usb_append_cstr(usb *b, const char *cstr) {
    size_t n, i;
    n = strlen(cstr);

    if (b->size + n >= b->cap) {
        usb_reserve(b, b->cap + n);   
    }

    for (i = 0; i < n; ++i) {
        b->data[b->size + i] = cstr[i];
    }

    b->size += n;
}

UMDEF void usb_append_usv(usb *b, usv v) {
    size_t i;

    if (b->size + v.size >= b->cap) {
        usb_reserve(b, b->cap + v.size);
    }

    for (i = 0; i < v.size; ++i) {
        b->data[b->size + i] = v.data[i];
    }

    b->size += v.size;
}

UMDEF char  *ucstr_from_file(const char *filename) {
    FILE *f;
    size_t len;
    char *buf;
    int status;

    f= fopen(filename, "rb");

    if (!f) {
        return NULL;
    }

    status = fseek(f, 0, SEEK_END);

    if (status < 0) {
        fclose(f);
        return NULL;
    }

    len = ftell(f);

    status = fseek(f, 0, SEEK_SET);

    if (status < 0) {
        fclose(f);
        return NULL;
    }

    buf = (char *) UM_MALLOC((len + 1)* sizeof(char));

    UM_ASSERT(buf);

    fread(buf, 1, len, f);
    buf[len] = '\0';

    fclose(f);

    return buf;
}

UMDEF char *ucstr_read_stream_until(FILE *stream, const char *delims) {
    size_t buf_len;
    size_t cap;
    char *buf;
    int c;

    buf_len = 0;
    cap = UBUF_INIT_CAP;
    buf = (char *) UM_MALLOC(cap*sizeof(char));

    UM_ASSERT(buf);

    while ((c = getc(stream)) != EOF) {
        if (strchr(delims, c) != NULL) {
            break;
        }

        if (buf_len + 1 >= cap) {
            char *tmp;

            cap *= 2;
            tmp = (char *) UM_REALLOC(buf, cap * sizeof(char));

            UM_ASSERT(tmp);

            buf = tmp;
        }

        buf[buf_len++] = (char) c;
    }

    buf[buf_len] = '\0';

    return buf;
}

UMDEF ubool usb_from_file(const char *filename, usb *b) {
    char *buf;

    buf = ucstr_from_file(filename);

    if (!buf) {
        UM_FREE(buf);
        return ufalse;
    }

    b->data = buf;
    b->size = strlen(buf);
    b->cap = b->size;

    return utrue;
}

UMDEF void usb_read_stream_until(FILE *stream, usb *b, const char *delims) {
    char *buf;

    buf = ucstr_read_stream_until(stream, delims);

    usb_append_cstr(b, buf);
}

UMDEF ubool usv_isnum(usv v) {
    size_t i;

    for (i = 0; i < v.size; ++i) {
        if (!isdigit(v.data[i])) return ufalse;
    }

    return utrue;
}

UMDEF i32 usv_to_i32(usv v) {
    int sum;
    int sign;
    size_t i;

    i = 0;
    sign = 1;
    sum = 0;
    
    if (v.data[0] == '-') {
        i++;
        sign = -1;
    }

    for (; i < v.size; ++i) {
        if (!isdigit(v.data[i])) return 0;
        sum = sum * 10 + (int) v.data[i] - '0';
    }

    return sum * sign;
}

UMDEF int usv_cmp(usv a, usv b) {
    size_t i, len;

    len = UM_MIN(a.size, b.size);

    for (i = 0; i < len; ++i) {
        if (a.data[i] != b.data[i]) {
            return a.data[i] - b.data[i];
        }
    }

    return 0;
}

UMDEF ubool usv_eq(usv a, usv b) {
    return a.size == b.size && (usv_cmp(a, b) == 0);
}

usv usv_from_cstr(const char *cstr) {
    usv res;
    res.size = strlen(cstr);
    res.data = cstr;

    return res;
}

UMDEF void usv_chop_right(usv *v, size_t n) {
    if (v->size < n) {
        v->size = n;
    }

    v->size -= n;
}

UMDEF void usv_chop_left(usv *v, size_t n) {
    if (v->size < n) {
        v->size = n;
    }
    v->data += n;
    v->size -= n;
}

UMDEF size_t usv_trim_left(usv *v) {
    size_t n;

    n = 0;
    while (v->size > 0 && isspace(v->data[0])) {
        usv_chop_left(v, 1);
        n++;
    }

    return n;
}

UMDEF size_t usv_trim_right(usv *v) {
    size_t n;
    n = 0;

    while (v->size > 0 && isspace(v->data[v->size - 1])) {
        usv_chop_right(v, 1);
        n++;
    }

    return n;
}

UMDEF size_t usv_trim(usv *v) {
    return usv_trim_left(v) + usv_trim_right(v);
}

/* chops a sv by delimiters (like strtok), returns the chopped of sv and modifies the input sv */
UMDEF usv usv_tok(usv *v, const char *delims) {
    size_t i, j;
    size_t dlen;
    usv res;

    dlen = strlen(delims);

    for (i = 0; i < v->size; i += 1) {
        for (j = 0; j < dlen; j++) {
            if (v->data[i] == delims[j]) goto outside;
        }
    }

outside:
    if (i < v->size) {
        res.data = v->data;
        res.size = i;

        usv_chop_left(v, i + 1);
        return res;
    }

    res = *v;
    usv_chop_left(v, v->size);
    return res;
}



/*
 * *---------------------------------------------*
 * |     um.h arena allocator implementation     |
 * *---------------------------------------------*
 */

UMDEF uar uar_new(void) {
    uar res;

    res.size = 0;
    res.cap = UAR_INIT_CAP;
    res.start = (u8 *) UM_MALLOC(res.cap);

    UM_ASSERT(res.start);

    res.current = res.start;

    return res;
}

UMDEF void *uar_alloc(uar *a, size_t size) {
    void *res;

    if (a->size + size > a->cap) {
        size_t new_cap, required, old_size, offset;
        u8 *new_start;

        offset   = (size_t) (a->current - a->start);
        old_size = a->size;
        required = old_size + size;

        new_cap = a->cap;

        while (new_cap < required) {
            if (new_cap >= UM_SIZE_MAX / 2) {
                new_cap = required;
                break;
            }

            new_cap *= 2;
        }

        new_start = (u8 *) UM_REALLOC(a->start, new_cap);

        if (!new_start) {
            return NULL;
        }

        a->start   = new_start;
        a->current = new_start + offset;
        a->cap     = new_cap;
    }

    res = a->current;

    a->current += size;
    a->size    += size;

    return res;
}

UMDEF void uar_destroy(uar *a) {
    UM_FREE(a->start);

    a->start   = NULL;
    a->current = NULL;

    a->cap  = 0;
    a->size = 0;
}



/*
 * *---------------------------------------------*
 * |         um.h logging implementation         |
 * *---------------------------------------------*
 */


UMDEF ubool ulog_fprint(FILE *stream, ulog_level level, const char *msg) {
    if (!stream) {
        return ufalse;
    }

#ifndef ULOG_NO_COLORS
    if (stream == stdout || stream == stderr) {
        switch (level) {
            case ULOG_INFO:
                fprintf(stream, "\x1b[0;34m");
                break;
            case ULOG_WARN:
                fprintf(stream, "\x1b[0;33m");
                break;
            case ULOG_ERROR:
                fprintf(stream, "\x1b[0;31m");
                break;
            default:
                return ufalse;
        }
    }
#endif /* ULOG_NO_COLORS */

    fprintf(stream, "[");
    switch (level) {
        case ULOG_INFO:
            fprintf(stream, "INFO");
            break;
        case ULOG_WARN:
            fprintf(stream, "WARNING");
            break;
        case ULOG_ERROR:
            fprintf(stream, "ERROR");
            break;
        default:
            return ufalse;
    }

    fprintf(stream, "]: ");

    if (stream == stdout || stream == stderr) {
        fprintf(stream, "\x1b[0;39m");
    }

    fprintf(stream, "%s\n", msg);
    return utrue;
}

UMDEF ubool ulog_print(ulog_level level, const char *msg) {
    ubool res;

    res = ulog_fprint(ULOG_DEFAULT_STREAM, level, msg);

    return res;
}

UMDEF ubool ulog_fprintf(FILE *stream, ulog_level level, const char *fmt, ...) {
    va_list args, tmp_args;
    int len;
    char *buf;
    ubool res;

    if (!stream) {
        return ufalse;
    }

    va_start(args, fmt);
    va_copy(tmp_args, args);

    len = vsnprintf(NULL, 0, fmt, tmp_args);
    
    if (len < 0) {
        va_end(args);
        return ufalse;
    }

    buf = (char *) UM_MALLOC((len + 1) * sizeof(char));

    UM_ASSERT(buf);

    if (vsnprintf(buf, len + 1, fmt, args) < 0) {
        UM_FREE(buf);
        va_end(args);
        return ufalse;
    }

    va_end(args);

    res = ulog_fprint(stream, level, buf);

    UM_FREE(buf);

    return res;
}

UMDEF ubool ulog_printf(ulog_level level, const char *fmt, ...) {
    va_list args, tmp_args;
    int len;
    char *buf;
    ubool res;

    va_start(args, fmt);
    va_copy(tmp_args, args);

    len = vsnprintf(NULL, 0, fmt, tmp_args);
    
    if (len < 0) {
        va_end(args);
        return ufalse;
    }

    buf = (char *) UM_MALLOC((len + 1) * sizeof(char));

    UM_ASSERT(buf);

    if (vsnprintf(buf, len + 1, fmt, args) < 0) {
        UM_FREE(buf);
        va_end(args);
        return ufalse;
    }

    va_end(args);

    res = ulog_print(level, buf);

    UM_FREE(buf);

    return res;
}

#endif /* UM_IMPLEMENTATION */
