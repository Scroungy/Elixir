#pragma once

extern "C"
{
// Custom defines
#define global_variable static
#define local_persist static
#define internal_linkage static
#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)
#define RESTRICT __declspec(restrict)
#define ALLOCATE __declspec(allocator)
#define NORETURN __declspec(noreturn)
#define ALIGN(x) __declspec(align(x))
#define CONST const
#define VOID void
#define POINTER_32 __ptr32
#define POINTER_64 __ptr64
#define POINTER_SIGNED __sptr
#define POINTER_UNSIGNED __uptr
#define UINT_MAX 0xFFFFFFFF
#define NULL 0

    // Custom types
    typedef unsigned char u8, uint8_t;
    typedef unsigned short u16, uint16_t;
    typedef unsigned int u32, uint32_t;
    typedef unsigned long long u64, uint64_t;
    typedef char i8, int8_t;
    typedef short i16, int16_t;
    typedef int i32, int32_t;
    typedef long long i64, int64_t;
    typedef float f32;
    typedef double f64;
    typedef bool b8;
    typedef wchar_t w16;
}