#ifndef MATHS_STRING_SWITCH_H
#define MATHS_STRING_SWITCH_H

#include <cstdint>
#include <cstddef>

namespace mathS {
    constexpr uint64_t strlen(const char v[]) {
        size_t len = 0;
        while (v[len++]) ;
        return len - 1;
    }
    constexpr uint64_t hash(const char v[]) {
        uint64_t t = 0;
        size_t len = 0;
        while (v[len++]) ;
        --len;
        for (size_t i = 0; i < len; i++) {
            t <<= 8;
            t |= v[i];
        }
        return t;
    }
}
#define SWITCH(n) switch (mathS::hash(n))
#define CASE(s, n) case mathS::hash(n): if (s != n) break;

#endif //MATHS_STRING_SWITCH_H
