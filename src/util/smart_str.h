/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010- Facebook, Inc. (http://www.facebook.com)         |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/
#ifndef SMART_STR_H
#define SMART_STR_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    char *c; 
    size_t len;
    size_t a;
} smart_str;

#ifndef SMART_STR_PREALLOC
#define SMART_STR_PREALLOC 128
#endif

#ifndef SMART_STR_START_SIZE
#define SMART_STR_START_SIZE 78
#endif

inline int smart_str_alloc(smart_str *str, int size) {
    size_t newlen;

    if (!str->c) {
        str->len = 0;
        newlen = size;
        str->a = newlen < SMART_STR_START_SIZE
            ? SMART_STR_START_SIZE
            : newlen + SMART_STR_PREALLOC;
        str->c = (char *) realloc((void *)str->c, str->a);
    } else {
        newlen = str->len + size;
        if (newlen >= str->a) {
            str->a = newlen + SMART_STR_PREALLOC;
            str->c = (char *) realloc((void *)str->c, str->a);
        }
    }
    return newlen;
}

inline void smart_str_free(smart_str *str) {
    str->len = 0;
    str->a = 0;
    free((void *)str->c);
    str->c = NULL;
}

inline void smart_str_appendc(smart_str *str, char ch) {
    str->len = smart_str_alloc(str, 1);
    str->c[str->len - 1] = ch;
}

inline void smart_str_appendl(smart_str *dest, const char *src, int nlen) {
    smart_str_alloc(dest, nlen);
    memcpy((void *)(dest->c + dest->len), (void *)src, nlen);
    dest->len += nlen;
}

inline void smart_str_append_unsigned(smart_str *dest, unsigned value) {
    char aux[32];
    int written = snprintf(aux, 32, "%u", value);
    smart_str_appendl(dest, aux, written);
}

inline void smart_str_append_double(smart_str *dest, double value) {
    char buf[256];
    int len = snprintf(buf, 256, "%.14g", value);
    smart_str_appendl(dest, buf, len);
}

#endif
