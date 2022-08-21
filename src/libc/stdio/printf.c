#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

static bool print(const char* data, size_t length) {
    const unsigned char* bytes = (const unsigned char*) data;
    for (size_t i = 0; i < length; i++)
        if (putchar(bytes[i]) == EOF)
            return false;
    return true;
}
 
int printf(const char* restrict format, ...) {
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != '\0') {
        size_t maxrem = INT_MAX - written;

        if (format[0] != '%' || format[1] == '%') {
            if (format[0] == '%')
                format++;
            size_t amount = 1;
            while (format[amount] && format[amount] != '%')
                amount++;
            if (maxrem < amount) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(format, amount))
                return -1;
            format += amount;
            written += amount;
            continue;
        }

        const char* format_begun_at = format++;

        if (*format == 'c') {
            format++;
            char c = (char) va_arg(parameters, int /* char promotes to int */);
            if (!maxrem) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(&c, sizeof(c)))
                return -1;
            written++;
        } else if (*format == 's') {
            format++;
            const char* str = va_arg(parameters, const char*);
            size_t len = strlen(str);
            if (maxrem < len) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(str, len))
                return -1;
            written += len;
        } else if (*format == 'd') {
            format++;
            int d = va_arg(parameters, int);
            int original = d;
            int i = 1;
            for (; d / 10 != 0; i++) 
                d /= 10;
            char buf[i + 1];
            for (int j = 0; j < i; j++) {
                buf[j] = '0' + (char)(original % 10);
                original /= 10;
            }
            strrev(buf);
            print(buf, i);
            written += i;
        } else if (*format == 'p') {
            /*
            NOTE: this section needs to be fixed to display HEX values rather than DEC
            */
            format++;
            uint32_t d = (uint32_t)va_arg(parameters, void*);
            uint32_t original = d;
            int i = 1;
            for (; d / 10 != 0; i++) 
                d /= 10;
            char buf[i + 1];
            for (int j = 0; j < i; j++) {
                buf[j] = '0' + (char)(original % 10);
                original /= 10;
            }
            strrev(buf);
            print(buf, i);
            written += i;
        } else if (*format == 'x') {
            format++;
            uint32_t x = (uint32_t)va_arg(parameters, int);
            uint32_t original = x;
            int i = 1;
            for (; x / 16 != 0; i++) 
                x /= 16;
            char buf[i + 1];
            char template[17] = "0123456789ABCDEF";
            for (int j = 0; j < i; j++) {
                buf[j] = template[(original & (0x0F << (j * 4))) >> (j * 4)];
            }
            strrev(buf);
            print(buf, i);
            written += i;
        } else {
            format = format_begun_at;
            size_t len = strlen(format);
            if (maxrem < len) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(format, len))
                return -1;
            written += len;
            format += len;
        }
    }

    va_end(parameters);
    return written;
}
