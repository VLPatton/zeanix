#include <string.h>

void strrev(char* str) {
    size_t size = strlen(str);
    char temp;
    for (size_t i = 0; i < size / 2; i++) {
        temp = str[i];
        str[i] = str[size - i - 1];
        str[size - i - 1] = temp;
    }
}