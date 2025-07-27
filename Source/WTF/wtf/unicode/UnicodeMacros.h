#pragma once
#include <stdint.h>

using UChar = uint16_t;
using UChar32 = uint32_t;
using UBool = bool;

#define UCHAR_MAX_VALUE 0x10FFFF

#define U_IS_SURROGATE(c) (((c) & 0xFFFFF800u) == 0xD800u)
#define U_IS_SURROGATE_LEAD(c) (((c) & 0xFFFFFC00u) == 0xD800u)
#define U_IS_SURROGATE_TRAIL(c) (((c) & 0xFFFFFC00u) == 0xDC00u)

#define U16_IS_LEAD(c) (((c) & 0xFC00u) == 0xD800u)
#define U16_IS_TRAIL(c) (((c) & 0xFC00u) == 0xDC00u)
#define U16_LEAD(cp) static_cast<UChar>((((cp) >> 10) + 0xD7C0u))
#define U16_TRAIL(cp) static_cast<UChar>(((cp) & 0x3FFu) | 0xDC00u)

#define U16_GET(s, start, i, length, c) \
    do { \
        (c) = (s)[i]; \
        if (U16_IS_LEAD(c) && (i + 1) < (length) && U16_IS_TRAIL((s)[i + 1])) \
            (c) = (((c) - 0xD800u) << 10) + ((s)[i + 1] - 0xDC00u) + 0x10000u; \
    } while (0)

#define U16_FWD_1(s, i, length) \
    do { \
        if (U16_IS_LEAD((s)[i]) && ((i) + 1) < (length) && U16_IS_TRAIL((s)[(i) + 1])) \
            (i) += 2; \
        else \
            ++(i); \
    } while (0)

#define U16_NEXT(s, i, length, c) \
    do { \
        (c) = (s)[i++]; \
        if (U16_IS_LEAD(c) && (i) < (length) && U16_IS_TRAIL((s)[i])) { \
            (c) = (((c) - 0xD800u) << 10) + ((s)[i++] - 0xDC00u) + 0x10000u; \
        } \
    } while (0)

#define U16_APPEND(s, i, capacity, c, error) \
    do { \
        if ((c) <= 0xFFFF) { \
            if ((i) >= (capacity)) { error = true; break; } \
            (s)[(i)++] = static_cast<UChar>(c); \
        } else { \
            if ((i) + 1 >= (capacity)) { error = true; break; } \
            (s)[(i)++] = U16_LEAD(c); \
            (s)[(i)++] = U16_TRAIL(c); \
        } \
    } while (0)

#define U8_MAX_LENGTH 4

#define U8_APPEND(buffer, i, capacity, c, error) \
    do { \
        if ((c) <= 0x7F) { \
            if ((i) >= (capacity)) { error = true; break; } \
            (buffer)[(i)++] = static_cast<uint8_t>(c); \
        } else if ((c) <= 0x7FF) { \
            if ((i) + 1 >= (capacity)) { error = true; break; } \
            (buffer)[(i)++] = static_cast<uint8_t>(0xC0u | ((c) >> 6)); \
            (buffer)[(i)++] = static_cast<uint8_t>(0x80u | ((c) & 0x3Fu)); \
        } else if ((c) <= 0xFFFF) { \
            if ((i) + 2 >= (capacity)) { error = true; break; } \
            (buffer)[(i)++] = static_cast<uint8_t>(0xE0u | ((c) >> 12)); \
            (buffer)[(i)++] = static_cast<uint8_t>(0x80u | (((c) >> 6) & 0x3Fu)); \
            (buffer)[(i)++] = static_cast<uint8_t>(0x80u | ((c) & 0x3Fu)); \
        } else { \
            if ((i) + 3 >= (capacity)) { error = true; break; } \
            (buffer)[(i)++] = static_cast<uint8_t>(0xF0u | ((c) >> 18)); \
            (buffer)[(i)++] = static_cast<uint8_t>(0x80u | (((c) >> 12) & 0x3Fu)); \
            (buffer)[(i)++] = static_cast<uint8_t>(0x80u | (((c) >> 6) & 0x3Fu)); \
            (buffer)[(i)++] = static_cast<uint8_t>(0x80u | ((c) & 0x3Fu)); \
        } \
    } while (0)

#define U8_APPEND_UNSAFE(buffer, i, c) \
    do { \
        UBool err = false; \
        U8_APPEND(buffer, i, INT_MAX, c, err); \
    } while (0)

#define U8_NEXT(s, i, length, c) \
    do { \
        uint8_t __first = static_cast<uint8_t>((s)[i++]); \
        if (__first < 0x80u) { \
            (c) = __first; \
        } else if (__first < 0xE0u && (i) < (length)) { \
            (c) = ((__first & 0x1Fu) << 6) | ((s)[i++] & 0x3Fu); \
        } else if (__first < 0xF0u && (i) + 1 < (length)) { \
            (c) = ((__first & 0x0Fu) << 12) | (((s)[i++] & 0x3Fu) << 6) | ((s)[i++] & 0x3Fu); \
        } else if (__first < 0xF8u && (i) + 2 < (length)) { \
            (c) = ((__first & 0x07u) << 18) | (((s)[i++] & 0x3Fu) << 12) | (((s)[i++] & 0x3Fu) << 6) | ((s)[i++] & 0x3Fu); \
        } else { \
            (c) = 0xFFFDu; \
        } \
    } while (0)
