#include "path.h"

bool path_concat(const char* p1, const char* p2, char* dest, u32 maxLength) {
    u32 l1 = (u32)strlen(p1);
    u32 l2 = (u32)strlen(p2);

    if (l1 + l2 + 2 >= maxLength) {
        return false;
    }

    strcpy(dest, p1);
    dest[l1] = '/';
    strcpy(&dest[l1 + 1], p2);

    return true;
}