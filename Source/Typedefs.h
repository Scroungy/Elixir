#pragma once

#define WIN32_LEAN_AND_MEAN // Used to reduce size of included windows header files
#define NOMINMAX            // Used to specify we don't want to use windows defined min and max

// Vulkan specific defines
#define MAX_QUEUED_MESSAGES 16
#define SWAPCHAIN_IMAGE_COUNT 2
#define DEFAULT_WINDOW_HEIGHT 720
#define DEFAULT_WINDOW_WIDTH 1280
#define MAX_WINDOWS 8
#define MAX_WINDOW_SETTINGS 8
#define ASCII_MAIN_WINDOW_NAME "Elixir" // Window name
#ifndef VK_USE_PLATFORM_WIN32_KHR
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#define INSTANCE_EXTENSION_COUNT 3
#define DEVICE_EXTENSION_COUNT 1
#define QUEUE_FAMILY_INDEX_COUNT 1
#define DEFAULT_ALLOCATION_SIZE 1024 * 1024 * 16
#define DEFAULT_BUFFER_SIZE 1024 * 1024 * 4
#define UNIQUE_BUFFER_COUNT 1
#define MAX_IMAGE_COUNT 16
#define MAX_BLOCKS (UNIQUE_BUFFER_COUNT + MAX_IMAGE_COUNT)
#define MEMORY_ALLOCATION_COUNT 1
#define VK_NO_STDDEF_H
#define VK_NO_STDINT_H

// Windows specific defines
#define UNICODE_MAIN_WINDOW_NAME L"Elixir"

#define global_variable static
#define local_persist static
#define internal_linkage static
#define NULL 0
#define UINT_MAX 0xFFFFFFFF
#define UINT64_MAX 0xFFFFFFFFFFFFFFFF
#define INFINITE 0xFFFFFFFF

#include <Windows.h>
#include <immintrin.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;
typedef float f32;
typedef double f64;
typedef bool b8;
typedef wchar_t w16;

// Vulkan still expects stdint style types
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;
