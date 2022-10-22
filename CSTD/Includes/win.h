#pragma once

// ANCHOR: Includes

#include "typedefs.h"

// ANCHOR: WARNINGS

#pragma warning(push)
#pragma warning(disable : 4820)

// ANCHOR: Calling conventions

#define WINAPI __stdcall
#define NTAPI __stdcall
#define CALLBACK __stdcall
#define APIENTRY WINAPI
#define WINUSERAPI __declspec(dllimport)

extern "C"
{

    // ANCHOR: Modifiers

#define DECLARE_HANDLE(name) \
    struct name##__;         \
    typedef struct name##__ *name

    // ANCHOR: Base types

    typedef VOID *PVOID, *LPVOID, *HANDLE, *SC_LOCK, **LPHANDLE, **PHANDLE, *HACCEL, *HBITMAP, *HBRUSH, *HCOLORSPACE, *HCONV, *HCONVLIST, *HCURSOR, *HDC, *HDDEDATA, *HDESK, *HDROP, *HDWP, *HENHMETAFILE, *HFONT, *HGDIOBJ, *HGLOBAL, *HHOOK, *HICON, *HINSTANCE, *HKEY, **PHKEY, *HKL, *HLOCAL, *HMENU, *HMETAFILE, *HMODULE, *HMONITOR, *HPALLETE, *HPEN, *HRGN, *HRSRC, *HSZ, *WINSTA, *HWND, *SC_HANDLE, *SERVICE_STATUS_HANDLE, *PSID;
    typedef CONST void *LPCVOID;
    typedef unsigned char UCHAR, BYTE, *LPBYTE, *PBYTE, BOOLEAN, *PBOOLEAN, UINT8, *PUINT8, *PUCHAR;
    typedef char CHAR, CCHAR, *PCHAR, *PSTR, *LPSTR;
    typedef signed char INT8, *PINT8;
    typedef CONST CHAR *LPCSTR, *PCSTR;
    typedef unsigned short USHORT, WORD, LANGID, UINT16, *PUSHORT, *LPWORD, *PWORD, *PUINT16;
    typedef short SHORT, ATOM, *PSHORT;
    typedef signed short INT16, *PINT16;
    typedef wchar_t WCHAR, *LPWSTR, *PWCHAR, *PWSTR;
    typedef CONST WCHAR *LPCWSTR, *PCWSTR;
#ifdef UNICODE
    typedef WCHAR TBYTE, TCHAR, *PTSTR, *LPTSTR, *PTBYTE, *PTCHAR;
    typedef LPCWSTR PCTSTR, LPCTSTR;
#else
    typedef unsigned char TBYTE, *PTRSTR, *LPTSTR, *PTBYTE;
    typedef char TCHAR, *PTCHAR;
    typedef LPCSTR PCTSTR, LPCTSTR;
#endif
    typedef struct _UNICODE_STRING
    {
        USHORT Length;
        USHORT MaximumLength;
        PWSTR Buffer;
    } UNICODE_STRING, *PUNICODE_STRING;
    typedef const UNICODE_STRING *PCUNICODE_STRING;
    typedef unsigned int UINT, DWORD32, UHALF_PTR, UINT32, ULONG32, *PUINT, *PDWORD32, *PUHALF_PTR, *PUINT32, *PULONG32;
    typedef int INT, BOOL, HALF_PTR, *PHALF_PTR, *PINT, *LPINT, HFILE, *LPBOOL, *PBOOL;
    typedef signed int INT32, LONG32, *PINT32, *PLONG32;
    typedef unsigned long DWORD, COLORREF, LCID, LCTYPE, LGRPID, ULONG, *LPCOLORREF, *LPDWORD, *PDWORD, *PLCID, *PULONG;
    typedef long LONG, *LPLONG, HRESULT, *PLONG;
    typedef unsigned __int64 DWORDLONG, DWORD64, QWORD, UINT_PTR, UINT64, ULONGLONG, ULONG_PTR, ULONG64, SIZE_T, *PDWORDLONG, *PDWORD64, *PUINT_PTR, WPARAM, *PUINT64, *PULONGLONG, DWORD_PTR, *PULONG_PTR, *PULONG64, *PSIZE_T, *PDWORD_PTR, KAFFINITY, *PKAFFINITY;
    typedef __int64 LONGLONG, LONG_PTR, LONG64, LPARAM, LRESULT, USN, INT_PTR, SSIZE_T, *PLONGLONG, *PLONG_PTR, *PLONG64, *PINT_PTR, *PSSIZE_T;
    typedef signed __int64 INT64, *PINT64;
    typedef float FLOAT, *PFLOAT;

    // ANCHOR: Base defines

#define INVALID_HANDLE_VALUE ((HANDLE)-1)

    // ANCHOR: Base structs

    typedef struct tagPOINT
    {
        LONG x;
        LONG y;
    } POINT, *PPOINT, *NPPOINT, *LPPOINT;

    typedef struct tagRECT
    {
        LONG left;
        LONG top;
        LONG right;
        LONG bottom;
    } RECT, *PRECT, *NPRECT, *LPRECT;
    typedef const RECT *LPCRECT;

    typedef struct tagSIZE
    {
        LONG cx;
        LONG cy;
    } SIZE, *PSIZE, *LPSIZE, SIZEL, *PSIZEL, *LPSIZEL;

    typedef struct _LUID
    {
        ULONG LowPart;
        LONG HighPart;
    } LUID, *PLUID;

    typedef union _LARGE_INTEGER
    {
        struct
        {
            DWORD LowPart;
            LONG HighPart;
        } DUMMYSTRUCTNAME;
        struct
        {
            DWORD LowPart;
            LONG HighPart;
        } u;
        LONGLONG QuadPart;
    } LARGE_INTEGER;

    typedef struct _LIST_ENTRY
    {
        struct _LIST_ENTRY *Flink;
        struct _LIST_ENTRY *Blink;
    } LIST_ENTRY, *PLIST_ENTRY, PRLIST_ENTRY;

    typedef struct _SECURITY_ATTRIBUTES
    {
        DWORD nLength;
        LPVOID lpSecurityDescriptor;
        BOOL bInheritHandle;
    } SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

#ifndef NO_WINDOWS

    // INTRINSICS

#ifndef NO_INTRIN

    typedef struct ALIGN(16) _SETJMP_FLOAT128
    {
        unsigned __int64 Part[2];
    } SETJMP_FLOAT128;

#define _JBLEN 16
    typedef SETJMP_FLOAT128 _JBTYPE;

    typedef struct _JUMP_BUFFER
    {
        unsigned __int64 Frame;
        unsigned __int64 Rbx;
        unsigned __int64 Rsp;
        unsigned __int64 Rbp;
        unsigned __int64 Rsi;
        unsigned __int64 Rdi;
        unsigned __int64 R12;
        unsigned __int64 R13;
        unsigned __int64 R14;
        unsigned __int64 R15;
        unsigned __int64 Rip;
        unsigned long MxCsr;
        unsigned short FpCsr;
        unsigned short Spare;

        SETJMP_FLOAT128 Xmm6;
        SETJMP_FLOAT128 Xmm7;
        SETJMP_FLOAT128 Xmm8;
        SETJMP_FLOAT128 Xmm9;
        SETJMP_FLOAT128 Xmm10;
        SETJMP_FLOAT128 Xmm11;
        SETJMP_FLOAT128 Xmm12;
        SETJMP_FLOAT128 Xmm13;
        SETJMP_FLOAT128 Xmm14;
        SETJMP_FLOAT128 Xmm15;
    } _JUMP_BUFFER;

#define _JMP_BUF_DEFINED
    typedef _JBTYPE jmp_buf[_JBLEN];
#define setjmp _setjmp

    int __cdecl setjmp(jmp_buf _Buf);
    NORETURN void __cdecl longjmp(jmp_buf _Buf, int _Value) noexcept(false);

#define _INC_MALLOC
#include <immintrin.h>
#include <ammintrin.h>

#define __MACHINE(X) X;
#define __MACHINEZ(X)
#define __MACHINEX64 __MACHINE
#define __MACHINEX86_X64 __MACHINE
#define __MACHINECALL_CDECL_OR_DEFAULT

    __MACHINE(unsigned char _BitScanForward(unsigned long *_Index, unsigned long _Mask))
    __MACHINEX64(unsigned char _BitScanForward64(unsigned long *_Index, unsigned __int64 _Mask))
    __MACHINE(unsigned char _BitScanReverse(unsigned long *_Index, unsigned long _Mask))
    __MACHINEX64(unsigned char _BitScanReverse64(unsigned long *_Index, unsigned __int64 _Mask))
    __MACHINE(unsigned char _bittest(long const *, long))
    __MACHINE(long _InterlockedAnd(long volatile *_Value, long _Mask))
    __MACHINE(short _InterlockedAnd16(short volatile *_Value, short _Mask))
    __MACHINE(__int64 _interlockedand64(__int64 volatile *_Value, __int64 _Mask))
    __MACHINE(char _InterlockedAnd8(char volatile *_Value, char _Mask))
    __MACHINE(long __MACHINECALL_CDECL_OR_DEFAULT _InterlockedCompareExchange(long volatile *_Destination, long _Exchange, long _Comparand))
    __MACHINE(short _InterlockedCompareExchange16(short volatile *_Destination, short _Exchange, short _Comparand))
    __MACHINE(__int64 _InterlockedCompareExchange64(__int64 volatile *_Destination, __int64 _Exchange, __int64 _Comparand))
    __MACHINE(char _InterlockedCompareExchange8(char volatile *_Destination, char _Exchange, char _Comparand))
    __MACHINE(long __MACHINECALL_CDECL_OR_DEFAULT _InterlockedDecrement(long volatile *_Addend))
    __MACHINE(short _InterlockedDecrement16(short volatile *_Addend))
    __MACHINE(__int64 _interlockeddecrement64(__int64 volatile *_Addend))
    __MACHINE(long __MACHINECALL_CDECL_OR_DEFAULT _InterlockedExchange(long volatile *_Target, long _Value))
    __MACHINE(short _InterlockedExchange16(short volatile *_Target, short _Value))
    __MACHINE(__int64 _interlockedexchange64(__int64 volatile *_Target, __int64 _Value))
    __MACHINE(char _InterlockedExchange8(char volatile *_Target, char _Value))
    __MACHINE(long __MACHINECALL_CDECL_OR_DEFAULT _InterlockedExchangeAdd(long volatile *_Addend, long _Value))
    __MACHINE(short _InterlockedExchangeAdd16(short volatile *_Addend, short _Value))
    __MACHINE(__int64 _interlockedexchangeadd64(__int64 volatile *_Addend, __int64 _Value))
    __MACHINE(char _InterlockedExchangeAdd8(char volatile *_Addend, char _Value))
    __MACHINE(long __MACHINECALL_CDECL_OR_DEFAULT _InterlockedIncrement(long volatile *_Addend))
    __MACHINE(short _InterlockedIncrement16(short volatile *_Addend))
    __MACHINE(__int64 _interlockedincrement64(__int64 volatile *_Addend))
    __MACHINE(long _InterlockedOr(long volatile *_Value, long _Mask))
    __MACHINE(short _InterlockedOr16(short volatile *_Value, short _Mask))
    __MACHINE(__int64 _interlockedor64(__int64 volatile *_Value, __int64 _Mask))
    __MACHINE(char _InterlockedOr8(char volatile *_Value, char _Mask))
    __MACHINE(long _InterlockedXor(long volatile *_Value, long _Mask))
    __MACHINE(short _InterlockedXor16(short volatile *_Value, short _Mask))
    __MACHINE(__int64 _interlockedxor64(__int64 volatile *_Value, __int64 _Mask))
    __MACHINE(char _InterlockedXor8(char volatile *_Value, char _Mask))
    __MACHINE(void _ReadWriteBarrier(void))
    __MACHINE(__int16 __iso_volatile_load16(const volatile __int16 *))
    __MACHINE(__int32 __iso_volatile_load32(const volatile __int32 *))
    __MACHINE(__int64 __iso_volatile_load64(const volatile __int64 *))
    __MACHINE(__int8 __iso_volatile_load8(const volatile __int8 *))
    __MACHINE(void __iso_volatile_store16(volatile __int16 *, __int16))
    __MACHINE(void __iso_volatile_store32(volatile __int32 *, __int32))
    __MACHINE(void __iso_volatile_store64(volatile __int64 *, __int64))
    __MACHINE(void __iso_volatile_store8(volatile __int8 *, __int8))
    __MACHINE(unsigned char _interlockedbittestandset(long volatile *, long))
    __MACHINEX86_X64(void _mm_pause(void))
    __MACHINEX86_X64(unsigned int __lzcnt(unsigned int))
    __MACHINEX86_X64(unsigned short __lzcnt16(unsigned short))
    __MACHINEX64(unsigned __int64 __lzcnt64(unsigned __int64))
    __MACHINEX86_X64(unsigned int __popcnt(unsigned int))
    __MACHINEX86_X64(unsigned short __popcnt16(unsigned short))
    __MACHINEX64(unsigned __int64 __popcnt64(unsigned __int64))
    __MACHINEX64(unsigned __int64 __shiftright128(unsigned __int64 _LowPart, unsigned __int64 _HighPart, unsigned char _Shift))
    __MACHINEX86_X64(unsigned int _tzcnt_u32(unsigned int))
    __MACHINEX64(unsigned __int64 _tzcnt_u64(unsigned __int64))
    __MACHINEX64(unsigned __int64 _umul128(unsigned __int64 _Multiplier, unsigned __int64 _Multiplicand, unsigned __int64 *_HighProduct))
    __MACHINE(double __ceil(double))
    __MACHINE(float __ceilf(float))
    __MACHINE(double __floor(double))
    __MACHINE(float __floorf(float))
    __MACHINE(double __round(double))
    __MACHINE(float __roundf(float))
    __MACHINE(double __trunc(double))
    __MACHINE(float __truncf(float))
    __MACHINE(double __copysign(double, double))
    __MACHINE(float __copysignf(float, float))
    __MACHINE(unsigned __signbitvalue(double))
    __MACHINE(unsigned __signbitvaluef(float))
    __MACHINEX86_X64(int _cvt_ftoi_sat(float a))
    __MACHINEX86_X64(unsigned _cvt_ftoui_sat(float a))
    __MACHINEX86_X64(long long _cvt_ftoll_sat(float a))
    __MACHINEX86_X64(unsigned long long _cvt_ftoull_sat(float a))
    __MACHINEX86_X64(int _cvt_ftoi_sent(float a))
    __MACHINEX86_X64(unsigned _cvt_ftoui_sent(float a))
    __MACHINEX86_X64(long long _cvt_ftoll_sent(float a))
    __MACHINEX86_X64(unsigned long long _cvt_ftoull_sent(float a))
    __MACHINEX86_X64(int _cvt_dtoi_sat(double a))
    __MACHINEX86_X64(unsigned _cvt_dtoui_sat(double a))
    __MACHINEX86_X64(long long _cvt_dtoll_sat(double a))
    __MACHINEX86_X64(unsigned long long _cvt_dtoull_sat(double a))
    __MACHINEX86_X64(int _cvt_dtoi_sent(double a))
    __MACHINEX86_X64(unsigned _cvt_dtoui_sent(double a))
    __MACHINEX86_X64(long long _cvt_dtoll_sent(double a))
    __MACHINEX86_X64(unsigned long long _cvt_dtoull_sent(double a))

    __MACHINE(void *_AddressOfReturnAddress(void))
    __MACHINEX86_X64(int _cvt_dtoi_fast(double))
    __MACHINEX86_X64(int _cvt_dtoi_sat(double))
    __MACHINEX86_X64(int _cvt_dtoi_sent(double))
    __MACHINEX86_X64(long long _cvt_dtoll_fast(double))
    __MACHINEX86_X64(long long _cvt_dtoll_sat(double))
    __MACHINEX86_X64(long long _cvt_dtoll_sent(double))
    __MACHINEX86_X64(unsigned _cvt_dtoui_fast(double))
    __MACHINEX86_X64(unsigned _cvt_dtoui_sat(double))
    __MACHINEX86_X64(unsigned _cvt_dtoui_sent(double))
    __MACHINEX86_X64(unsigned long long _cvt_dtoull_fast(double))
    __MACHINEX86_X64(unsigned long long _cvt_dtoull_sat(double))
    __MACHINEX86_X64(unsigned long long _cvt_dtoull_sent(double))
    __MACHINEX86_X64(int _cvt_ftoi_fast(float))
    __MACHINEX86_X64(int _cvt_ftoi_sat(float))
    __MACHINEX86_X64(int _cvt_ftoi_sent(float))
    __MACHINEX86_X64(long long _cvt_ftoll_fast(float))
    __MACHINEX86_X64(long long _cvt_ftoll_sat(float))
    __MACHINEX86_X64(long long _cvt_ftoll_sent(float))
    __MACHINEX86_X64(unsigned _cvt_ftoui_fast(float))
    __MACHINEX86_X64(unsigned _cvt_ftoui_sat(float))
    __MACHINEX86_X64(unsigned _cvt_ftoui_sent(float))
    __MACHINEX86_X64(unsigned long long _cvt_ftoull_fast(float))
    __MACHINEX86_X64(unsigned long long _cvt_ftoull_sat(float))
    __MACHINEX86_X64(unsigned long long _cvt_ftoull_sent(float))
    __MACHINE(long _interlockedadd(long volatile *_Addend, long _Value))
    __MACHINE(__int64 _interlockedadd64(__int64 volatile *_Addend, __int64 _Value))
    __MACHINEX64(short _InterlockedAnd16_np(short volatile *_Value, short _Mask))
    __MACHINEX64(__int64 _InterlockedAnd64_np(__int64 volatile *_Value, __int64 _Mask))
    __MACHINEX64(char _InterlockedAnd8_np(char volatile *_Value, char _Mask))
    __MACHINEX64(long _InterlockedAnd_np(long volatile *_Value, long _Mask))
    __MACHINEX64(unsigned char _InterlockedCompareExchange128_np(__int64 volatile *_Destination, __int64 _ExchangeHigh, __int64 _ExchangeLow, __int64 *_ComparandResult))
    __MACHINEX64(short _InterlockedCompareExchange16_np(short volatile *_Destination, short _Exchange, short _Comparand))
    __MACHINEX64(__int64 _InterlockedCompareExchange64_np(__int64 volatile *_Destination, __int64 _Exchange, __int64 _Comparand))
    __MACHINE(void *_InterlockedCompareExchangePointer(void *volatile *_Destination, void *_Exchange, void *_Comparand))
    __MACHINEX64(void *_InterlockedCompareExchangePointer_np(void *volatile *_Destination, void *_Exchange, void *_Comparand))
    __MACHINEX64(long _InterlockedCompareExchange_np(long volatile *_Destination, long _Exchange, long _Comparand))
    __MACHINE(void *_InterlockedExchangePointer(void *volatile *_Target, void *_Value))
    __MACHINEX64(short _InterlockedOr16_np(short volatile *_Value, short _Mask))
    __MACHINEX64(__int64 _InterlockedOr64_np(__int64 volatile *_Value, __int64 _Mask))
    __MACHINEX64(char _InterlockedOr8_np(char volatile *_Value, char _Mask))
    __MACHINEX64(long _InterlockedOr_np(long volatile *_Value, long _Mask))
    __MACHINEX64(short _InterlockedXor16_np(short volatile *_Value, short _Mask))
    __MACHINEX64(__int64 _InterlockedXor64_np(__int64 volatile *_Value, __int64 _Mask))
    __MACHINEX64(char _InterlockedXor8_np(char volatile *_Value, char _Mask))
    __MACHINEX64(long _InterlockedXor_np(long volatile *_Value, long _Mask))
    __MACHINE(void _ReadBarrier(void))
    __MACHINE(void *_ReturnAddress(void))
    __MACHINE(void _WriteBarrier(void))
    __MACHINEX64(void __addgsbyte(unsigned long, unsigned char))
    __MACHINEX64(void __addgsdword(unsigned long, unsigned long))
    __MACHINEX64(void __addgsqword(unsigned long, unsigned __int64))
    __MACHINEX64(void __addgsword(unsigned long, unsigned short))
    __MACHINEX86_X64(void __clts(void))
    __MACHINE(void __code_seg(const char *))
    __MACHINEX86_X64(void __cpuid(int[4], int))
    __MACHINEX86_X64(void __cpuidex(int[4], int, int))
    __MACHINE(void __cdecl __debugbreak(void))
    __MACHINEX86_X64(__int64 __emul(int, int))
    __MACHINEX86_X64(unsigned __int64 __emulu(unsigned int, unsigned int))
    __MACHINE(__declspec(noreturn) void __fastfail(unsigned int))
    __MACHINEX64(void __faststorefence(void))
    __MACHINEX86_X64(unsigned int __getcallerseflags(void))
    __MACHINEX86_X64(void __halt(void))
    __MACHINEX86_X64(unsigned char __inbyte(unsigned short))
    __MACHINEX86_X64(void __inbytestring(unsigned short, unsigned char *, unsigned long))
    __MACHINEX64(void __incgsbyte(unsigned long))
    __MACHINEX64(void __incgsdword(unsigned long))
    __MACHINEX64(void __incgsqword(unsigned long))
    __MACHINEX64(void __incgsword(unsigned long))
    __MACHINEX86_X64(unsigned long __indword(unsigned short))
    __MACHINEX86_X64(void __indwordstring(unsigned short, unsigned long *, unsigned long))
    __MACHINEX86_X64(void __int2c(void))
    __MACHINEX86_X64(void __invlpg(void *))
    __MACHINEX86_X64(unsigned short __inword(unsigned short))
    __MACHINEX86_X64(void __inwordstring(unsigned short, unsigned short *, unsigned long))
    __MACHINEX86_X64(void __lidt(void *))
    __MACHINEX86_X64(unsigned __int64 __ll_lshift(unsigned __int64, int))
    __MACHINEX86_X64(__int64 __ll_rshift(__int64, int))
    __MACHINEX86_X64(void __movsb(unsigned char *, unsigned char const *, size_t))
    __MACHINEX86_X64(void __movsd(unsigned long *, unsigned long const *, size_t))
    __MACHINEX64(void __movsq(unsigned long long *, unsigned long long const *, size_t))
    __MACHINEX86_X64(void __movsw(unsigned short *, unsigned short const *, size_t))
    __MACHINE(void __nop(void))
    __MACHINEX86_X64(void __nvreg_restore_fence(void))
    __MACHINEX86_X64(void __nvreg_save_fence(void))
    __MACHINEX86_X64(void __outbyte(unsigned short, unsigned char))
    __MACHINEX86_X64(void __outbytestring(unsigned short, unsigned char *, unsigned long))
    __MACHINEX86_X64(void __outdword(unsigned short, unsigned long))
    __MACHINEX86_X64(void __outdwordstring(unsigned short, unsigned long *, unsigned long))
    __MACHINEX86_X64(void __outword(unsigned short, unsigned short))
    __MACHINEX86_X64(void __outwordstring(unsigned short, unsigned short *, unsigned long))
    __MACHINEX86_X64(unsigned __int64 __rdtsc(void))
    __MACHINEX86_X64(unsigned __int64 __rdtscp(unsigned int *))
    __MACHINEX64(unsigned __int64 __readcr0(void))
    __MACHINEX64(unsigned __int64 __readcr2(void))
    __MACHINEX64(unsigned __int64 __readcr3(void))
    __MACHINEX64(unsigned __int64 __readcr4(void))
    __MACHINEX64(unsigned __int64 __readcr8(void))
    __MACHINEX64(unsigned __int64 __readdr(unsigned int))
    __MACHINEX64(unsigned __int64 __readeflags(void))
    __MACHINEX64(unsigned char __readgsbyte(unsigned long))
    __MACHINEX64(unsigned long __readgsdword(unsigned long))
    __MACHINEX64(unsigned __int64 __readgsqword(unsigned long))
    __MACHINEX64(unsigned short __readgsword(unsigned long))
    __MACHINEX86_X64(unsigned __int64 __readmsr(unsigned long))
    __MACHINEX86_X64(unsigned __int64 __readpmc(unsigned long))
    __MACHINEX86_X64(unsigned long __segmentlimit(unsigned long))
    __MACHINEX64(unsigned __int64 __shiftleft128(unsigned __int64 _LowPart, unsigned __int64 _HighPart, unsigned char _Shift))
    __MACHINEX86_X64(void __sidt(void *))
    __MACHINEX86_X64(void __stosb(unsigned char *, unsigned char, size_t))
    __MACHINEX86_X64(void __stosd(unsigned long *, unsigned long, size_t))
    __MACHINEX64(void __stosq(unsigned __int64 *, unsigned __int64, size_t))
    __MACHINEX86_X64(void __stosw(unsigned short *, unsigned short, size_t))
    __MACHINEX86_X64(void __svm_clgi(void))
    __MACHINEX86_X64(void __svm_invlpga(void *, int))
    __MACHINEX86_X64(void __svm_skinit(int))
    __MACHINEX86_X64(void __svm_stgi(void))
    __MACHINEX86_X64(void __svm_vmload(size_t))
    __MACHINEX86_X64(void __svm_vmrun(size_t))
    __MACHINEX86_X64(void __svm_vmsave(size_t))
    __MACHINEX86_X64(void __ud2(void))
    __MACHINEX86_X64(unsigned __int64 __ull_rshift(unsigned __int64, int))
    __MACHINEX86_X64(void __vmx_off(void))
    __MACHINEX64(unsigned char __vmx_on(unsigned __int64 *))
    __MACHINEX64(unsigned char __vmx_vmclear(unsigned __int64 *))
    __MACHINEX64(unsigned char __vmx_vmlaunch(void))
    __MACHINEX64(unsigned char __vmx_vmptrld(unsigned __int64 *))
    __MACHINEX86_X64(void __vmx_vmptrst(unsigned __int64 *))
    __MACHINEX64(unsigned char __vmx_vmread(size_t, size_t *))
    __MACHINEX64(unsigned char __vmx_vmresume(void))
    __MACHINEX64(unsigned char __vmx_vmwrite(size_t, size_t))
    __MACHINEX86_X64(void __wbinvd(void))
    __MACHINEX64(void __writecr0(unsigned __int64))
    __MACHINEX64(void __writecr2(unsigned __int64))
    __MACHINEX64(void __writecr3(unsigned __int64))
    __MACHINEX64(void __writecr4(unsigned __int64))
    __MACHINEX64(void __writecr8(unsigned __int64))
    __MACHINEX64(void __writedr(unsigned int, unsigned __int64))
    __MACHINEX64(void __writeeflags(unsigned __int64))
    __MACHINEX64(void __writegsbyte(unsigned long, unsigned char))
    __MACHINEX64(void __writegsdword(unsigned long, unsigned long))
    __MACHINEX64(void __writegsqword(unsigned long, unsigned __int64))
    __MACHINEX64(void __writegsword(unsigned long, unsigned short))
    __MACHINEX86_X64(void __writemsr(unsigned long, unsigned __int64))
    __MACHINE(unsigned char _bittestandcomplement(long *, long))
    __MACHINE(unsigned char _bittestandreset(long *, long))
    __MACHINE(unsigned char _bittestandset(long *, long))
    __MACHINE(unsigned __int64 __cdecl _byteswap_uint64(unsigned __int64))
    __MACHINE(unsigned long __cdecl _byteswap_ulong(unsigned long))
    __MACHINE(unsigned short __cdecl _byteswap_ushort(unsigned short))
    __MACHINE(void __cdecl _disable(void))
    __MACHINE(void __cdecl _enable(void))
    __MACHINE(unsigned char _interlockedbittestandreset(long volatile *, long))
    __MACHINE(unsigned long __cdecl _lrotl(unsigned long, int))
    __MACHINE(unsigned long __cdecl _lrotr(unsigned long, int))
    __MACHINEX86_X64(void _m_prefetch(void *))
    __MACHINEX86_X64(void _m_prefetchw(volatile const void *))
    __MACHINEX86_X64(__m128i _mm_abs_epi16(__m128i))
    __MACHINEX86_X64(__m128i _mm_abs_epi32(__m128i))
    __MACHINEX86_X64(__m128i _mm_abs_epi8(__m128i))
    __MACHINEX86_X64(__m128i _mm_add_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_add_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_add_epi64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_add_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_add_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_add_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_add_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_add_ss(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_adds_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_adds_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_adds_epu16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_adds_epu8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_addsub_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_addsub_ps(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_alignr_epi8(__m128i, __m128i, int))
    __MACHINEX86_X64(__m128d _mm_and_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_and_ps(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_and_si128(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_andnot_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_andnot_ps(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_andnot_si128(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_avg_epu16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_avg_epu8(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_blend_epi16(__m128i, __m128i, int))
    __MACHINEX86_X64(__m128d _mm_blend_pd(__m128d, __m128d, int))
    __MACHINEX86_X64(__m128 _mm_blend_ps(__m128, __m128, int))
    __MACHINEX86_X64(__m128i _mm_blendv_epi8(__m128i, __m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_blendv_pd(__m128d, __m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_blendv_ps(__m128, __m128, __m128))
    __MACHINEX86_X64(void _mm_clflush(void const *))
    __MACHINEX86_X64(void _mm_clflushopt(void const *))
    __MACHINEX86_X64(void _mm_clwb(void const *))
    __MACHINEX86_X64(void _mm_clzero(void const *))
    __MACHINEX86_X64(__m128i _mm_cmpeq_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_cmpeq_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_cmpeq_epi64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_cmpeq_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_cmpeq_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpeq_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpeq_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpeq_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_cmpestra(__m128i, int, __m128i, int, int))
    __MACHINEX86_X64(int _mm_cmpestrc(__m128i, int, __m128i, int, int))
    __MACHINEX86_X64(int _mm_cmpestri(__m128i, int, __m128i, int, int))
    __MACHINEX86_X64(__m128i _mm_cmpestrm(__m128i, int, __m128i, int, int))
    __MACHINEX86_X64(int _mm_cmpestro(__m128i, int, __m128i, int, int))
    __MACHINEX86_X64(int _mm_cmpestrs(__m128i, int, __m128i, int, int))
    __MACHINEX86_X64(int _mm_cmpestrz(__m128i, int, __m128i, int, int))
    __MACHINEX86_X64(__m128d _mm_cmpge_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpge_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpge_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpge_ss(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_cmpgt_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_cmpgt_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_cmpgt_epi64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_cmpgt_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_cmpgt_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpgt_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpgt_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpgt_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_cmpistra(__m128i, __m128i, int))
    __MACHINEX86_X64(int _mm_cmpistrc(__m128i, __m128i, int))
    __MACHINEX86_X64(int _mm_cmpistri(__m128i, __m128i, int))
    __MACHINEX86_X64(__m128i _mm_cmpistrm(__m128i, __m128i, int))
    __MACHINEX86_X64(int _mm_cmpistro(__m128i, __m128i, int))
    __MACHINEX86_X64(int _mm_cmpistrs(__m128i, __m128i, int))
    __MACHINEX86_X64(int _mm_cmpistrz(__m128i, __m128i, int))
    __MACHINEX86_X64(__m128d _mm_cmple_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmple_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmple_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmple_ss(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_cmplt_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_cmplt_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_cmplt_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_cmplt_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmplt_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmplt_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmplt_ss(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpneq_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpneq_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpneq_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpneq_ss(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpnge_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpnge_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpnge_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpnge_ss(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpngt_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpngt_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpngt_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpngt_ss(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpnle_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpnle_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpnle_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpnle_ss(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpnlt_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpnlt_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpnlt_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpnlt_ss(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpord_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpord_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpord_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpord_ss(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpunord_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpunord_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpunord_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpunord_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_comieq_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_comieq_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_comige_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_comige_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_comigt_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_comigt_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_comile_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_comile_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_comilt_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_comilt_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_comineq_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_comineq_ss(__m128, __m128))
    __MACHINEX86_X64(unsigned int _mm_crc32_u16(unsigned int, unsigned short))
    __MACHINEX86_X64(unsigned int _mm_crc32_u32(unsigned int, unsigned int))
    __MACHINEX64(unsigned __int64 _mm_crc32_u64(unsigned __int64, unsigned __int64))
    __MACHINEX86_X64(unsigned int _mm_crc32_u8(unsigned int, unsigned char))
    __MACHINEX86_X64(__m128 _mm_cvt_si2ss(__m128, int))
    __MACHINEX86_X64(int _mm_cvt_ss2si(__m128))
    __MACHINEX86_X64(__m128i _mm_cvtepi16_epi32(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepi16_epi64(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepi32_epi64(__m128i))
    __MACHINEX86_X64(__m128d _mm_cvtepi32_pd(__m128i))
    __MACHINEX86_X64(__m128 _mm_cvtepi32_ps(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepi8_epi16(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepi8_epi32(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepi8_epi64(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepu16_epi32(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepu16_epi64(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepu32_epi64(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepu8_epi16(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepu8_epi32(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepu8_epi64(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtpd_epi32(__m128d))
    __MACHINEX86_X64(__m128 _mm_cvtpd_ps(__m128d))
    __MACHINEX86_X64(__m128i _mm_cvtps_epi32(__m128))
    __MACHINEX86_X64(__m128d _mm_cvtps_pd(__m128))
    __MACHINEX86_X64(int _mm_cvtsd_si32(__m128d))
    __MACHINEX64(__int64 _mm_cvtsd_si64(__m128d))
    __MACHINEX64(__int64 _mm_cvtsd_si64x(__m128d))
    __MACHINEX86_X64(__m128 _mm_cvtsd_ss(__m128, __m128d))
    __MACHINEX86_X64(int _mm_cvtsi128_si32(__m128i))
    __MACHINEX64(__int64 _mm_cvtsi128_si64(__m128i))
    __MACHINEX64(__int64 _mm_cvtsi128_si64x(__m128i))
    __MACHINEX86_X64(__m128d _mm_cvtsi32_sd(__m128d, int))
    __MACHINEX86_X64(__m128i _mm_cvtsi32_si128(int))
    __MACHINEX64(__m128d _mm_cvtsi64_sd(__m128d, __int64))
    __MACHINEX64(__m128i _mm_cvtsi64_si128(__int64))
    __MACHINEX64(__m128 _mm_cvtsi64_ss(__m128, __int64))
    __MACHINEX64(__m128d _mm_cvtsi64x_sd(__m128d, __int64))
    __MACHINEX64(__m128i _mm_cvtsi64x_si128(__int64))
    __MACHINEX64(__m128 _mm_cvtsi64x_ss(__m128, __int64))
    __MACHINEX86_X64(__m128d _mm_cvtss_sd(__m128d, __m128))
    __MACHINEX64(__int64 _mm_cvtss_si64(__m128))
    __MACHINEX64(__int64 _mm_cvtss_si64x(__m128))
    __MACHINEX86_X64(int _mm_cvtt_ss2si(__m128))
    __MACHINEX86_X64(__m128i _mm_cvttpd_epi32(__m128d))
    __MACHINEX86_X64(__m128i _mm_cvttps_epi32(__m128))
    __MACHINEX86_X64(int _mm_cvttsd_si32(__m128d))
    __MACHINEX64(__int64 _mm_cvttsd_si64(__m128d))
    __MACHINEX64(__int64 _mm_cvttsd_si64x(__m128d))
    __MACHINEX64(__int64 _mm_cvttss_si64(__m128))
    __MACHINEX64(__int64 _mm_cvttss_si64x(__m128))
    __MACHINEX86_X64(__m128d _mm_div_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_div_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_div_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_div_ss(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_dp_pd(__m128d, __m128d, int))
    __MACHINEX86_X64(__m128 _mm_dp_ps(__m128, __m128, int))
    __MACHINEX86_X64(int _mm_extract_epi16(__m128i, int))
    __MACHINEX86_X64(int _mm_extract_epi32(__m128i, int))
    __MACHINEX64(__int64 _mm_extract_epi64(__m128i, int))
    __MACHINEX86_X64(int _mm_extract_epi8(__m128i, int))
    __MACHINEX86_X64(int _mm_extract_ps(__m128, int))
    __MACHINEX86_X64(__m128i _mm_extract_si64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_extracti_si64(__m128i, int, int))
    __MACHINEX86_X64(unsigned int _mm_getcsr(void))
    __MACHINEX86_X64(__m128i _mm_hadd_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_hadd_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_hadd_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_hadd_ps(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_hadds_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_hsub_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_hsub_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_hsub_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_hsub_ps(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_hsubs_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_insert_epi16(__m128i, int, int))
    __MACHINEX86_X64(__m128i _mm_insert_epi32(__m128i, int, int))
    __MACHINEX64(__m128i _mm_insert_epi64(__m128i, __int64, int))
    __MACHINEX86_X64(__m128i _mm_insert_epi8(__m128i, int, int))
    __MACHINEX86_X64(__m128 _mm_insert_ps(__m128, __m128, int))
    __MACHINEX86_X64(__m128i _mm_insert_si64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_inserti_si64(__m128i, __m128i, int, int))
    __MACHINEX86_X64(__m128i _mm_lddqu_si128(__m128i const *))
    __MACHINEX86_X64(void _mm_lfence(void))
    __MACHINEX86_X64(__m128d _mm_load1_pd(double const *))
    __MACHINEX86_X64(__m128d _mm_load_pd(double const *))
    __MACHINEX86_X64(__m128 _mm_load_ps(float const *))
    __MACHINEX86_X64(__m128 _mm_load_ps1(float const *))
    __MACHINEX86_X64(__m128d _mm_load_sd(double const *))
    __MACHINEX86_X64(__m128i _mm_load_si128(__m128i const *))
    __MACHINEX86_X64(__m128 _mm_load_ss(float const *))
    __MACHINEX86_X64(__m128d _mm_loaddup_pd(double const *))
    __MACHINEX86_X64(__m128d _mm_loadh_pd(__m128d, double const *))
    __MACHINEX86_X64(__m128 _mm_loadh_pi(__m128, __m64 const *))
    __MACHINEX86_X64(__m128i _mm_loadl_epi64(__m128i const *))
    __MACHINEX86_X64(__m128d _mm_loadl_pd(__m128d, double const *))
    __MACHINEX86_X64(__m128 _mm_loadl_pi(__m128, __m64 const *))
    __MACHINEX86_X64(__m128d _mm_loadr_pd(double const *))
    __MACHINEX86_X64(__m128 _mm_loadr_ps(float const *))
    __MACHINEX86_X64(__m128d _mm_loadu_pd(double const *))
    __MACHINEX86_X64(__m128 _mm_loadu_ps(float const *))
    __MACHINEX86_X64(__m128i _mm_loadu_si128(__m128i const *))
    __MACHINEX86_X64(__m128i _mm_madd_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_maddubs_epi16(__m128i, __m128i))
    __MACHINEX86_X64(void _mm_maskmoveu_si128(__m128i, __m128i, char *))
    __MACHINEX86_X64(__m128i _mm_max_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_max_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_max_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_max_epu16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_max_epu32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_max_epu8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_max_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_max_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_max_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_max_ss(__m128, __m128))
    __MACHINEX86_X64(void _mm_mfence(void))
    __MACHINEX86_X64(__m128i _mm_min_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_min_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_min_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_min_epu16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_min_epu32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_min_epu8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_min_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_min_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_min_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_min_ss(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_minpos_epu16(__m128i))
    __MACHINEX86_X64(void _mm_monitor(void const *, unsigned int, unsigned int))
    __MACHINEX86_X64(__m128i _mm_move_epi64(__m128i))
    __MACHINEX86_X64(__m128d _mm_move_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_move_ss(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_movedup_pd(__m128d))
    __MACHINEX86_X64(__m128 _mm_movehdup_ps(__m128))
    __MACHINEX86_X64(__m128 _mm_movehl_ps(__m128, __m128))
    __MACHINEX86_X64(__m128 _mm_moveldup_ps(__m128))
    __MACHINEX86_X64(__m128 _mm_movelh_ps(__m128, __m128))
    __MACHINEX86_X64(int _mm_movemask_epi8(__m128i))
    __MACHINEX86_X64(int _mm_movemask_pd(__m128d))
    __MACHINEX86_X64(int _mm_movemask_ps(__m128))
    __MACHINEX86_X64(__m128i _mm_mpsadbw_epu8(__m128i, __m128i, int))
    __MACHINEX86_X64(__m128i _mm_mul_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_mul_epu32(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_mul_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_mul_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_mul_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_mul_ss(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_mulhi_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_mulhi_epu16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_mulhrs_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_mullo_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_mullo_epi32(__m128i, __m128i))
    __MACHINEX86_X64(void _mm_mwait(unsigned int, unsigned int))
    __MACHINEX86_X64(__m128d _mm_or_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_or_ps(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_or_si128(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_packs_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_packs_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_packus_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_packus_epi32(__m128i, __m128i))
    __MACHINEX86_X64(int _mm_popcnt_u32(unsigned int))
    __MACHINEX64(__int64 _mm_popcnt_u64(unsigned __int64))
    __MACHINEX86_X64(void _mm_prefetch(char const *, int))
    __MACHINEX86_X64(__m128 _mm_rcp_ps(__m128))
    __MACHINEX86_X64(__m128 _mm_rcp_ss(__m128))
    __MACHINEX86_X64(__m128d _mm_round_pd(__m128d, int))
    __MACHINEX86_X64(__m128 _mm_round_ps(__m128, int))
    __MACHINEX86_X64(__m128d _mm_round_sd(__m128d, __m128d, int))
    __MACHINEX86_X64(__m128 _mm_round_ss(__m128, __m128, int))
    __MACHINEX86_X64(__m128 _mm_rsqrt_ps(__m128))
    __MACHINEX86_X64(__m128 _mm_rsqrt_ss(__m128))
    __MACHINEX86_X64(__m128i _mm_sad_epu8(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_set1_epi16(short))
    __MACHINEX86_X64(__m128i _mm_set1_epi32(int))
    __MACHINEX86_X64(__m128i _mm_set1_epi64x(__int64))
    __MACHINEX86_X64(__m128i _mm_set1_epi8(char))
    __MACHINEX86_X64(__m128d _mm_set1_pd(double))
    __MACHINEX86_X64(__m128i _mm_set_epi16(short, short, short, short, short, short, short, short))
    __MACHINEX86_X64(__m128i _mm_set_epi32(int, int, int, int))
    __MACHINEX86_X64(__m128i _mm_set_epi64x(__int64, __int64))
    __MACHINEX86_X64(__m128i _mm_set_epi8(char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char))
    __MACHINEX86_X64(__m128d _mm_set_pd(double, double))
    __MACHINEX86_X64(__m128 _mm_set_ps(float, float, float, float))
    __MACHINEX86_X64(__m128 _mm_set_ps1(float))
    __MACHINEX86_X64(__m128d _mm_set_sd(double))
    __MACHINEX86_X64(__m128 _mm_set_ss(float))
    __MACHINEX86_X64(void _mm_setcsr(unsigned int))
    __MACHINEX86_X64(__m128i _mm_setl_epi64(__m128i))
    __MACHINEX86_X64(__m128i _mm_setr_epi16(short, short, short, short, short, short, short, short))
    __MACHINEX86_X64(__m128i _mm_setr_epi32(int, int, int, int))
    __MACHINEX86_X64(__m128i _mm_setr_epi64x(__int64, __int64))
    __MACHINEX86_X64(__m128i _mm_setr_epi8(char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char))
    __MACHINEX86_X64(__m128d _mm_setr_pd(double, double))
    __MACHINEX86_X64(__m128 _mm_setr_ps(float, float, float, float))
    __MACHINEX86_X64(__m128d _mm_setzero_pd(void))
    __MACHINEX86_X64(__m128 _mm_setzero_ps(void))
    __MACHINEX86_X64(__m128i _mm_setzero_si128(void))
    __MACHINEX86_X64(void _mm_sfence(void))
    __MACHINEX86_X64(__m128i _mm_shuffle_epi32(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_shuffle_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_shuffle_pd(__m128d, __m128d, int))
    __MACHINEX86_X64(__m128 _mm_shuffle_ps(__m128, __m128, unsigned int))
    __MACHINEX86_X64(__m128i _mm_shufflehi_epi16(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_shufflelo_epi16(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_sign_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_sign_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_sign_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_sll_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_sll_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_sll_epi64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_slli_epi16(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_slli_epi32(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_slli_epi64(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_slli_si128(__m128i, int))
    __MACHINEX86_X64(__m128d _mm_sqrt_pd(__m128d))
    __MACHINEX86_X64(__m128 _mm_sqrt_ps(__m128))
    __MACHINEX86_X64(__m128d _mm_sqrt_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_sqrt_ss(__m128))
    __MACHINEX86_X64(__m128i _mm_sra_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_sra_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_srai_epi16(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_srai_epi32(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_srl_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_srl_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_srl_epi64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_srli_epi16(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_srli_epi32(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_srli_epi64(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_srli_si128(__m128i, int))
    __MACHINEX86_X64(void _mm_store1_pd(double *, __m128d))
    __MACHINEX86_X64(void _mm_store_pd(double *, __m128d))
    __MACHINEX86_X64(void _mm_store_ps(float *, __m128))
    __MACHINEX86_X64(void _mm_store_ps1(float *, __m128))
    __MACHINEX86_X64(void _mm_store_sd(double *, __m128d))
    __MACHINEX86_X64(void _mm_store_si128(__m128i *, __m128i))
    __MACHINEX86_X64(void _mm_store_ss(float *, __m128))
    __MACHINEX86_X64(void _mm_storeh_pd(double *, __m128d))
    __MACHINEX86_X64(void _mm_storeh_pi(__m64 *, __m128))
    __MACHINEX86_X64(void _mm_storel_epi64(__m128i *, __m128i))
    __MACHINEX86_X64(void _mm_storel_pd(double *, __m128d))
    __MACHINEX86_X64(void _mm_storel_pi(__m64 *, __m128))
    __MACHINEX86_X64(void _mm_storer_pd(double *, __m128d))
    __MACHINEX86_X64(void _mm_storer_ps(float *, __m128))
    __MACHINEX86_X64(void _mm_storeu_pd(double *, __m128d))
    __MACHINEX86_X64(void _mm_storeu_ps(float *, __m128))
    __MACHINEX86_X64(void _mm_storeu_si128(__m128i *, __m128i))
    __MACHINEX86_X64(__m128i _mm_stream_load_si128(const __m128i *))
    __MACHINEX86_X64(void _mm_stream_pd(double *, __m128d))
    __MACHINEX86_X64(void _mm_stream_ps(float *, __m128))
    __MACHINEX86_X64(void _mm_stream_sd(double *, __m128d))
    __MACHINEX86_X64(void _mm_stream_si128(__m128i *, __m128i))
    __MACHINEX86_X64(void _mm_stream_si32(int *, int))
    __MACHINEX64(void _mm_stream_si64x(__int64 *, __int64))
    __MACHINEX86_X64(void _mm_stream_ss(float *, __m128))
    __MACHINEX86_X64(__m128i _mm_sub_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_sub_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_sub_epi64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_sub_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_sub_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_sub_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_sub_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_sub_ss(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_subs_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_subs_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_subs_epu16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_subs_epu8(__m128i, __m128i))
    __MACHINEX86_X64(int _mm_testc_si128(__m128i, __m128i))
    __MACHINEX86_X64(int _mm_testnzc_si128(__m128i, __m128i))
    __MACHINEX86_X64(int _mm_testz_si128(__m128i, __m128i))
    __MACHINEX86_X64(int _mm_ucomieq_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_ucomieq_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_ucomige_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_ucomige_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_ucomigt_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_ucomigt_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_ucomile_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_ucomile_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_ucomilt_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_ucomilt_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_ucomineq_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_ucomineq_ss(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_unpackhi_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_unpackhi_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_unpackhi_epi64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_unpackhi_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_unpackhi_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_unpackhi_ps(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_unpacklo_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_unpacklo_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_unpacklo_epi64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_unpacklo_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_unpacklo_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_unpacklo_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_xor_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_xor_ps(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_xor_si128(__m128i, __m128i))
    __MACHINEX64(__int64 _mul128(__int64 _Multiplier, __int64 _Multiplicand, __int64 *_HighProduct))
    __MACHINE(unsigned int __cdecl _rotl(unsigned int _Value, int _Shift))
    __MACHINE(unsigned short __cdecl _rotl16(unsigned short _Value, unsigned char _Shift))
    __MACHINE(unsigned __int64 __cdecl _rotl64(unsigned __int64 _Value, int _Shift))
    __MACHINE(unsigned char __cdecl _rotl8(unsigned char _Value, unsigned char _Shift))
    __MACHINE(unsigned int __cdecl _rotr(unsigned int _Value, int _Shift))
    __MACHINE(unsigned short __cdecl _rotr16(unsigned short _Value, unsigned char _Shift))
    __MACHINE(unsigned __int64 __cdecl _rotr64(unsigned __int64 _Value, int _Shift))
    __MACHINE(unsigned char __cdecl _rotr8(unsigned char _Value, unsigned char _Shift))
    __MACHINE(int __cdecl _setjmp(jmp_buf))
    __MACHINEX86_X64(void _rsm(void))
    __MACHINEX86_X64(void _lgdt(void *))
    __MACHINEX86_X64(void _sgdt(void *))
    __MACHINEX86_X64(void _clac(void))
    __MACHINEX86_X64(void _stac(void))
    __MACHINEX86_X64(unsigned char __cdecl _addcarry_u8(unsigned char, unsigned char, unsigned char, unsigned char *))
    __MACHINEX86_X64(unsigned char __cdecl _subborrow_u8(unsigned char, unsigned char, unsigned char, unsigned char *))
    __MACHINEX86_X64(unsigned char __cdecl _addcarry_u16(unsigned char, unsigned short, unsigned short, unsigned short *))
    __MACHINEX86_X64(unsigned char __cdecl _subborrow_u16(unsigned char, unsigned short, unsigned short, unsigned short *))
    __MACHINEX86_X64(unsigned char __cdecl _addcarry_u32(unsigned char, unsigned int, unsigned int, unsigned int *))
    __MACHINEX86_X64(unsigned char __cdecl _subborrow_u32(unsigned char, unsigned int, unsigned int, unsigned int *))
    __MACHINEX64(unsigned char __cdecl _addcarry_u64(unsigned char, unsigned __int64, unsigned __int64, unsigned __int64 *))
    __MACHINEX64(unsigned char __cdecl _subborrow_u64(unsigned char, unsigned __int64, unsigned __int64, unsigned __int64 *))
    __MACHINEX86_X64(void _mm_monitorx(void const *, unsigned int, unsigned int))
    __MACHINEX86_X64(void _mm_mwaitx(unsigned int, unsigned int, unsigned int))
    __MACHINEX64(unsigned int __rmpupdate(unsigned __int64, rmp_seg *, int))
    __MACHINEX64(unsigned int __psmash(unsigned __int64))
    __MACHINEX64(unsigned int __rmpadjust(unsigned __int64, int, int))
    __MACHINEX64(unsigned int __pvalidate(unsigned __int64, int, int, int *))
    __MACHINEX86_X64(void __svm_invlpgb(void *, int))
    __MACHINEX86_X64(void __svm_tlbsync(void))

    __MACHINE(void *_AddressOfReturnAddress(void))
    __MACHINEX86_X64(int _cvt_dtoi_fast(double))
    __MACHINEX86_X64(int _cvt_dtoi_sat(double))
    __MACHINEX86_X64(int _cvt_dtoi_sent(double))
    __MACHINEX86_X64(long long _cvt_dtoll_fast(double))
    __MACHINEX86_X64(long long _cvt_dtoll_sat(double))
    __MACHINEX86_X64(long long _cvt_dtoll_sent(double))
    __MACHINEX86_X64(unsigned _cvt_dtoui_fast(double))
    __MACHINEX86_X64(unsigned _cvt_dtoui_sat(double))
    __MACHINEX86_X64(unsigned _cvt_dtoui_sent(double))
    __MACHINEX86_X64(unsigned long long _cvt_dtoull_fast(double))
    __MACHINEX86_X64(unsigned long long _cvt_dtoull_sat(double))
    __MACHINEX86_X64(unsigned long long _cvt_dtoull_sent(double))
    __MACHINEX86_X64(int _cvt_ftoi_fast(float))
    __MACHINEX86_X64(int _cvt_ftoi_sat(float))
    __MACHINEX86_X64(int _cvt_ftoi_sent(float))
    __MACHINEX86_X64(long long _cvt_ftoll_fast(float))
    __MACHINEX86_X64(long long _cvt_ftoll_sat(float))
    __MACHINEX86_X64(long long _cvt_ftoll_sent(float))
    __MACHINEX86_X64(unsigned _cvt_ftoui_fast(float))
    __MACHINEX86_X64(unsigned _cvt_ftoui_sat(float))
    __MACHINEX86_X64(unsigned _cvt_ftoui_sent(float))
    __MACHINEX86_X64(unsigned long long _cvt_ftoull_fast(float))
    __MACHINEX86_X64(unsigned long long _cvt_ftoull_sat(float))
    __MACHINEX86_X64(unsigned long long _cvt_ftoull_sent(float))
    __MACHINE(long _interlockedadd(long volatile *_Addend, long _Value))
    __MACHINE(__int64 _interlockedadd64(__int64 volatile *_Addend, __int64 _Value))
    __MACHINEX64(short _InterlockedAnd16_np(short volatile *_Value, short _Mask))
    __MACHINEX64(__int64 _InterlockedAnd64_np(__int64 volatile *_Value, __int64 _Mask))
    __MACHINEX64(char _InterlockedAnd8_np(char volatile *_Value, char _Mask))
    __MACHINEX64(long _InterlockedAnd_np(long volatile *_Value, long _Mask))
    __MACHINEX64(unsigned char _InterlockedCompareExchange128_np(__int64 volatile *_Destination, __int64 _ExchangeHigh, __int64 _ExchangeLow, __int64 *_ComparandResult))
    __MACHINEX64(short _InterlockedCompareExchange16_np(short volatile *_Destination, short _Exchange, short _Comparand))
    __MACHINEX64(__int64 _InterlockedCompareExchange64_np(__int64 volatile *_Destination, __int64 _Exchange, __int64 _Comparand))
    __MACHINE(void *_InterlockedCompareExchangePointer(void *volatile *_Destination, void *_Exchange, void *_Comparand))
    __MACHINEX64(void *_InterlockedCompareExchangePointer_np(void *volatile *_Destination, void *_Exchange, void *_Comparand))
    __MACHINEX64(long _InterlockedCompareExchange_np(long volatile *_Destination, long _Exchange, long _Comparand))
    __MACHINE(void *_InterlockedExchangePointer(void *volatile *_Target, void *_Value))
    __MACHINEX64(short _InterlockedOr16_np(short volatile *_Value, short _Mask))
    __MACHINEX64(__int64 _InterlockedOr64_np(__int64 volatile *_Value, __int64 _Mask))
    __MACHINEX64(char _InterlockedOr8_np(char volatile *_Value, char _Mask))
    __MACHINEX64(long _InterlockedOr_np(long volatile *_Value, long _Mask))
    __MACHINEX64(short _InterlockedXor16_np(short volatile *_Value, short _Mask))
    __MACHINEX64(__int64 _InterlockedXor64_np(__int64 volatile *_Value, __int64 _Mask))
    __MACHINEX64(char _InterlockedXor8_np(char volatile *_Value, char _Mask))
    __MACHINEX64(long _InterlockedXor_np(long volatile *_Value, long _Mask))
    __MACHINE(void _ReadBarrier(void))
    __MACHINE(void *_ReturnAddress(void))
    __MACHINE(void _WriteBarrier(void))
    __MACHINEX64(void __addgsbyte(unsigned long, unsigned char))
    __MACHINEX64(void __addgsdword(unsigned long, unsigned long))
    __MACHINEX64(void __addgsqword(unsigned long, unsigned __int64))
    __MACHINEX64(void __addgsword(unsigned long, unsigned short))
    __MACHINEX86_X64(void __clts(void))
    __MACHINE(void __code_seg(const char *))
    __MACHINEX86_X64(void __cpuid(int[4], int))
    __MACHINEX86_X64(void __cpuidex(int[4], int, int))
    __MACHINE(void __cdecl __debugbreak(void))
    __MACHINEX86_X64(__int64 __emul(int, int))
    __MACHINEX86_X64(unsigned __int64 __emulu(unsigned int, unsigned int))
    __MACHINE(__declspec(noreturn) void __fastfail(unsigned int))
    __MACHINEX64(void __faststorefence(void))
    __MACHINEX86_X64(unsigned int __getcallerseflags(void))
    __MACHINEX86_X64(void __halt(void))
    __MACHINEX86_X64(unsigned char __inbyte(unsigned short))
    __MACHINEX86_X64(void __inbytestring(unsigned short, unsigned char *, unsigned long))
    __MACHINEX64(void __incgsbyte(unsigned long))
    __MACHINEX64(void __incgsdword(unsigned long))
    __MACHINEX64(void __incgsqword(unsigned long))
    __MACHINEX64(void __incgsword(unsigned long))
    __MACHINEX86_X64(unsigned long __indword(unsigned short))
    __MACHINEX86_X64(void __indwordstring(unsigned short, unsigned long *, unsigned long))
    __MACHINEX86_X64(void __int2c(void))
    __MACHINEX86_X64(void __invlpg(void *))
    __MACHINEX86_X64(unsigned short __inword(unsigned short))
    __MACHINEX86_X64(void __inwordstring(unsigned short, unsigned short *, unsigned long))
    __MACHINEX86_X64(void __lidt(void *))
    __MACHINEX86_X64(unsigned __int64 __ll_lshift(unsigned __int64, int))
    __MACHINEX86_X64(__int64 __ll_rshift(__int64, int))
    __MACHINEX86_X64(void __movsb(unsigned char *, unsigned char const *, size_t))
    __MACHINEX86_X64(void __movsd(unsigned long *, unsigned long const *, size_t))
    __MACHINEX64(void __movsq(unsigned long long *, unsigned long long const *, size_t))
    __MACHINEX86_X64(void __movsw(unsigned short *, unsigned short const *, size_t))
    __MACHINE(void __nop(void))
    __MACHINEX86_X64(void __nvreg_restore_fence(void))
    __MACHINEX86_X64(void __nvreg_save_fence(void))
    __MACHINEX86_X64(void __outbyte(unsigned short, unsigned char))
    __MACHINEX86_X64(void __outbytestring(unsigned short, unsigned char *, unsigned long))
    __MACHINEX86_X64(void __outdword(unsigned short, unsigned long))
    __MACHINEX86_X64(void __outdwordstring(unsigned short, unsigned long *, unsigned long))
    __MACHINEX86_X64(void __outword(unsigned short, unsigned short))
    __MACHINEX86_X64(void __outwordstring(unsigned short, unsigned short *, unsigned long))
    __MACHINEX86_X64(unsigned __int64 __rdtsc(void))
    __MACHINEX86_X64(unsigned __int64 __rdtscp(unsigned int *))
    __MACHINEX64(unsigned __int64 __readcr0(void))
    __MACHINEX64(unsigned __int64 __readcr2(void))
    __MACHINEX64(unsigned __int64 __readcr3(void))
    __MACHINEX64(unsigned __int64 __readcr4(void))
    __MACHINEX64(unsigned __int64 __readcr8(void))
    __MACHINEX64(unsigned __int64 __readdr(unsigned int))
    __MACHINEX64(unsigned __int64 __readeflags(void))
    __MACHINEX64(unsigned char __readgsbyte(unsigned long))
    __MACHINEX64(unsigned long __readgsdword(unsigned long))
    __MACHINEX64(unsigned __int64 __readgsqword(unsigned long))
    __MACHINEX64(unsigned short __readgsword(unsigned long))
    __MACHINEX86_X64(unsigned __int64 __readmsr(unsigned long))
    __MACHINEX86_X64(unsigned __int64 __readpmc(unsigned long))
    __MACHINEX86_X64(unsigned long __segmentlimit(unsigned long))
    __MACHINEX64(unsigned __int64 __shiftleft128(unsigned __int64 _LowPart, unsigned __int64 _HighPart, unsigned char _Shift))
    __MACHINEX86_X64(void __sidt(void *))
    __MACHINEX86_X64(void __stosb(unsigned char *, unsigned char, size_t))
    __MACHINEX86_X64(void __stosd(unsigned long *, unsigned long, size_t))
    __MACHINEX64(void __stosq(unsigned __int64 *, unsigned __int64, size_t))
    __MACHINEX86_X64(void __stosw(unsigned short *, unsigned short, size_t))
    __MACHINEX86_X64(void __svm_clgi(void))
    __MACHINEX86_X64(void __svm_invlpga(void *, int))
    __MACHINEX86_X64(void __svm_skinit(int))
    __MACHINEX86_X64(void __svm_stgi(void))
    __MACHINEX86_X64(void __svm_vmload(size_t))
    __MACHINEX86_X64(void __svm_vmrun(size_t))
    __MACHINEX86_X64(void __svm_vmsave(size_t))
    __MACHINEX86_X64(void __ud2(void))
    __MACHINEX86_X64(unsigned __int64 __ull_rshift(unsigned __int64, int))
    __MACHINEX86_X64(void __vmx_off(void))
    __MACHINEX64(unsigned char __vmx_on(unsigned __int64 *))
    __MACHINEX64(unsigned char __vmx_vmclear(unsigned __int64 *))
    __MACHINEX64(unsigned char __vmx_vmlaunch(void))
    __MACHINEX64(unsigned char __vmx_vmptrld(unsigned __int64 *))
    __MACHINEX86_X64(void __vmx_vmptrst(unsigned __int64 *))
    __MACHINEX64(unsigned char __vmx_vmread(size_t, size_t *))
    __MACHINEX64(unsigned char __vmx_vmresume(void))
    __MACHINEX64(unsigned char __vmx_vmwrite(size_t, size_t))
    __MACHINEX86_X64(void __wbinvd(void))
    __MACHINEX64(void __writecr0(unsigned __int64))
    __MACHINEX64(void __writecr2(unsigned __int64))
    __MACHINEX64(void __writecr3(unsigned __int64))
    __MACHINEX64(void __writecr4(unsigned __int64))
    __MACHINEX64(void __writecr8(unsigned __int64))
    __MACHINEX64(void __writedr(unsigned int, unsigned __int64))
    __MACHINEX64(void __writeeflags(unsigned __int64))
    __MACHINEX64(void __writegsbyte(unsigned long, unsigned char))
    __MACHINEX64(void __writegsdword(unsigned long, unsigned long))
    __MACHINEX64(void __writegsqword(unsigned long, unsigned __int64))
    __MACHINEX64(void __writegsword(unsigned long, unsigned short))
    __MACHINEX86_X64(void __writemsr(unsigned long, unsigned __int64))
    __MACHINE(unsigned char _bittestandcomplement(long *, long))
    __MACHINE(unsigned char _bittestandreset(long *, long))
    __MACHINE(unsigned char _bittestandset(long *, long))
    __MACHINE(unsigned __int64 __cdecl _byteswap_uint64(unsigned __int64))
    __MACHINE(unsigned long __cdecl _byteswap_ulong(unsigned long))
    __MACHINE(unsigned short __cdecl _byteswap_ushort(unsigned short))
    __MACHINE(void __cdecl _disable(void))
    __MACHINE(void __cdecl _enable(void))
    __MACHINE(unsigned char _interlockedbittestandreset(long volatile *, long))
    __MACHINE(unsigned long __cdecl _lrotl(unsigned long, int))
    __MACHINE(unsigned long __cdecl _lrotr(unsigned long, int))
    __MACHINEX86_X64(void _m_prefetch(void *))
    __MACHINEX86_X64(void _m_prefetchw(volatile const void *))
    __MACHINEX86_X64(__m128i _mm_abs_epi16(__m128i))
    __MACHINEX86_X64(__m128i _mm_abs_epi32(__m128i))
    __MACHINEX86_X64(__m128i _mm_abs_epi8(__m128i))
    __MACHINEX86_X64(__m128i _mm_add_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_add_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_add_epi64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_add_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_add_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_add_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_add_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_add_ss(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_adds_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_adds_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_adds_epu16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_adds_epu8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_addsub_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_addsub_ps(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_alignr_epi8(__m128i, __m128i, int))
    __MACHINEX86_X64(__m128d _mm_and_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_and_ps(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_and_si128(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_andnot_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_andnot_ps(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_andnot_si128(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_avg_epu16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_avg_epu8(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_blend_epi16(__m128i, __m128i, int))
    __MACHINEX86_X64(__m128d _mm_blend_pd(__m128d, __m128d, int))
    __MACHINEX86_X64(__m128 _mm_blend_ps(__m128, __m128, int))
    __MACHINEX86_X64(__m128i _mm_blendv_epi8(__m128i, __m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_blendv_pd(__m128d, __m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_blendv_ps(__m128, __m128, __m128))
    __MACHINEX86_X64(void _mm_clflush(void const *))
    __MACHINEX86_X64(void _mm_clflushopt(void const *))
    __MACHINEX86_X64(void _mm_clwb(void const *))
    __MACHINEX86_X64(void _mm_clzero(void const *))
    __MACHINEX86_X64(__m128i _mm_cmpeq_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_cmpeq_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_cmpeq_epi64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_cmpeq_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_cmpeq_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpeq_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpeq_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpeq_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_cmpestra(__m128i, int, __m128i, int, int))
    __MACHINEX86_X64(int _mm_cmpestrc(__m128i, int, __m128i, int, int))
    __MACHINEX86_X64(int _mm_cmpestri(__m128i, int, __m128i, int, int))
    __MACHINEX86_X64(__m128i _mm_cmpestrm(__m128i, int, __m128i, int, int))
    __MACHINEX86_X64(int _mm_cmpestro(__m128i, int, __m128i, int, int))
    __MACHINEX86_X64(int _mm_cmpestrs(__m128i, int, __m128i, int, int))
    __MACHINEX86_X64(int _mm_cmpestrz(__m128i, int, __m128i, int, int))
    __MACHINEX86_X64(__m128d _mm_cmpge_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpge_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpge_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpge_ss(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_cmpgt_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_cmpgt_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_cmpgt_epi64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_cmpgt_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_cmpgt_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpgt_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpgt_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpgt_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_cmpistra(__m128i, __m128i, int))
    __MACHINEX86_X64(int _mm_cmpistrc(__m128i, __m128i, int))
    __MACHINEX86_X64(int _mm_cmpistri(__m128i, __m128i, int))
    __MACHINEX86_X64(__m128i _mm_cmpistrm(__m128i, __m128i, int))
    __MACHINEX86_X64(int _mm_cmpistro(__m128i, __m128i, int))
    __MACHINEX86_X64(int _mm_cmpistrs(__m128i, __m128i, int))
    __MACHINEX86_X64(int _mm_cmpistrz(__m128i, __m128i, int))
    __MACHINEX86_X64(__m128d _mm_cmple_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmple_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmple_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmple_ss(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_cmplt_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_cmplt_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_cmplt_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_cmplt_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmplt_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmplt_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmplt_ss(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpneq_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpneq_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpneq_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpneq_ss(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpnge_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpnge_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpnge_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpnge_ss(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpngt_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpngt_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpngt_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpngt_ss(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpnle_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpnle_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpnle_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpnle_ss(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpnlt_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpnlt_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpnlt_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpnlt_ss(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpord_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpord_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpord_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpord_ss(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpunord_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpunord_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_cmpunord_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_cmpunord_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_comieq_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_comieq_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_comige_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_comige_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_comigt_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_comigt_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_comile_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_comile_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_comilt_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_comilt_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_comineq_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_comineq_ss(__m128, __m128))
    __MACHINEX86_X64(unsigned int _mm_crc32_u16(unsigned int, unsigned short))
    __MACHINEX86_X64(unsigned int _mm_crc32_u32(unsigned int, unsigned int))
    __MACHINEX64(unsigned __int64 _mm_crc32_u64(unsigned __int64, unsigned __int64))
    __MACHINEX86_X64(unsigned int _mm_crc32_u8(unsigned int, unsigned char))
    __MACHINEX86_X64(__m128 _mm_cvt_si2ss(__m128, int))
    __MACHINEX86_X64(int _mm_cvt_ss2si(__m128))
    __MACHINEX86_X64(__m128i _mm_cvtepi16_epi32(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepi16_epi64(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepi32_epi64(__m128i))
    __MACHINEX86_X64(__m128d _mm_cvtepi32_pd(__m128i))
    __MACHINEX86_X64(__m128 _mm_cvtepi32_ps(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepi8_epi16(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepi8_epi32(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepi8_epi64(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepu16_epi32(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepu16_epi64(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepu32_epi64(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepu8_epi16(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepu8_epi32(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtepu8_epi64(__m128i))
    __MACHINEX86_X64(__m128i _mm_cvtpd_epi32(__m128d))
    __MACHINEX86_X64(__m128 _mm_cvtpd_ps(__m128d))
    __MACHINEX86_X64(__m128i _mm_cvtps_epi32(__m128))
    __MACHINEX86_X64(__m128d _mm_cvtps_pd(__m128))
    __MACHINEX86_X64(int _mm_cvtsd_si32(__m128d))
    __MACHINEX64(__int64 _mm_cvtsd_si64(__m128d))
    __MACHINEX64(__int64 _mm_cvtsd_si64x(__m128d))
    __MACHINEX86_X64(__m128 _mm_cvtsd_ss(__m128, __m128d))
    __MACHINEX86_X64(int _mm_cvtsi128_si32(__m128i))
    __MACHINEX64(__int64 _mm_cvtsi128_si64(__m128i))
    __MACHINEX64(__int64 _mm_cvtsi128_si64x(__m128i))
    __MACHINEX86_X64(__m128d _mm_cvtsi32_sd(__m128d, int))
    __MACHINEX86_X64(__m128i _mm_cvtsi32_si128(int))
    __MACHINEX64(__m128d _mm_cvtsi64_sd(__m128d, __int64))
    __MACHINEX64(__m128i _mm_cvtsi64_si128(__int64))
    __MACHINEX64(__m128 _mm_cvtsi64_ss(__m128, __int64))
    __MACHINEX64(__m128d _mm_cvtsi64x_sd(__m128d, __int64))
    __MACHINEX64(__m128i _mm_cvtsi64x_si128(__int64))
    __MACHINEX64(__m128 _mm_cvtsi64x_ss(__m128, __int64))
    __MACHINEX86_X64(__m128d _mm_cvtss_sd(__m128d, __m128))
    __MACHINEX64(__int64 _mm_cvtss_si64(__m128))
    __MACHINEX64(__int64 _mm_cvtss_si64x(__m128))
    __MACHINEX86_X64(int _mm_cvtt_ss2si(__m128))
    __MACHINEX86_X64(__m128i _mm_cvttpd_epi32(__m128d))
    __MACHINEX86_X64(__m128i _mm_cvttps_epi32(__m128))
    __MACHINEX86_X64(int _mm_cvttsd_si32(__m128d))
    __MACHINEX64(__int64 _mm_cvttsd_si64(__m128d))
    __MACHINEX64(__int64 _mm_cvttsd_si64x(__m128d))
    __MACHINEX64(__int64 _mm_cvttss_si64(__m128))
    __MACHINEX64(__int64 _mm_cvttss_si64x(__m128))
    __MACHINEX86_X64(__m128d _mm_div_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_div_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_div_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_div_ss(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_dp_pd(__m128d, __m128d, int))
    __MACHINEX86_X64(__m128 _mm_dp_ps(__m128, __m128, int))
    __MACHINEX86_X64(int _mm_extract_epi16(__m128i, int))
    __MACHINEX86_X64(int _mm_extract_epi32(__m128i, int))
    __MACHINEX64(__int64 _mm_extract_epi64(__m128i, int))
    __MACHINEX86_X64(int _mm_extract_epi8(__m128i, int))
    __MACHINEX86_X64(int _mm_extract_ps(__m128, int))
    __MACHINEX86_X64(__m128i _mm_extract_si64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_extracti_si64(__m128i, int, int))
    __MACHINEX86_X64(unsigned int _mm_getcsr(void))
    __MACHINEX86_X64(__m128i _mm_hadd_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_hadd_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_hadd_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_hadd_ps(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_hadds_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_hsub_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_hsub_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_hsub_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_hsub_ps(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_hsubs_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_insert_epi16(__m128i, int, int))
    __MACHINEX86_X64(__m128i _mm_insert_epi32(__m128i, int, int))
    __MACHINEX64(__m128i _mm_insert_epi64(__m128i, __int64, int))
    __MACHINEX86_X64(__m128i _mm_insert_epi8(__m128i, int, int))
    __MACHINEX86_X64(__m128 _mm_insert_ps(__m128, __m128, int))
    __MACHINEX86_X64(__m128i _mm_insert_si64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_inserti_si64(__m128i, __m128i, int, int))
    __MACHINEX86_X64(__m128i _mm_lddqu_si128(__m128i const *))
    __MACHINEX86_X64(void _mm_lfence(void))
    __MACHINEX86_X64(__m128d _mm_load1_pd(double const *))
    __MACHINEX86_X64(__m128d _mm_load_pd(double const *))
    __MACHINEX86_X64(__m128 _mm_load_ps(float const *))
    __MACHINEX86_X64(__m128 _mm_load_ps1(float const *))
    __MACHINEX86_X64(__m128d _mm_load_sd(double const *))
    __MACHINEX86_X64(__m128i _mm_load_si128(__m128i const *))
    __MACHINEX86_X64(__m128 _mm_load_ss(float const *))
    __MACHINEX86_X64(__m128d _mm_loaddup_pd(double const *))
    __MACHINEX86_X64(__m128d _mm_loadh_pd(__m128d, double const *))
    __MACHINEX86_X64(__m128 _mm_loadh_pi(__m128, __m64 const *))
    __MACHINEX86_X64(__m128i _mm_loadl_epi64(__m128i const *))
    __MACHINEX86_X64(__m128d _mm_loadl_pd(__m128d, double const *))
    __MACHINEX86_X64(__m128 _mm_loadl_pi(__m128, __m64 const *))
    __MACHINEX86_X64(__m128d _mm_loadr_pd(double const *))
    __MACHINEX86_X64(__m128 _mm_loadr_ps(float const *))
    __MACHINEX86_X64(__m128d _mm_loadu_pd(double const *))
    __MACHINEX86_X64(__m128 _mm_loadu_ps(float const *))
    __MACHINEX86_X64(__m128i _mm_loadu_si128(__m128i const *))
    __MACHINEX86_X64(__m128i _mm_madd_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_maddubs_epi16(__m128i, __m128i))
    __MACHINEX86_X64(void _mm_maskmoveu_si128(__m128i, __m128i, char *))
    __MACHINEX86_X64(__m128i _mm_max_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_max_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_max_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_max_epu16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_max_epu32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_max_epu8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_max_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_max_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_max_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_max_ss(__m128, __m128))
    __MACHINEX86_X64(void _mm_mfence(void))
    __MACHINEX86_X64(__m128i _mm_min_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_min_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_min_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_min_epu16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_min_epu32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_min_epu8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_min_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_min_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_min_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_min_ss(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_minpos_epu16(__m128i))
    __MACHINEX86_X64(void _mm_monitor(void const *, unsigned int, unsigned int))
    __MACHINEX86_X64(__m128i _mm_move_epi64(__m128i))
    __MACHINEX86_X64(__m128d _mm_move_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_move_ss(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_movedup_pd(__m128d))
    __MACHINEX86_X64(__m128 _mm_movehdup_ps(__m128))
    __MACHINEX86_X64(__m128 _mm_movehl_ps(__m128, __m128))
    __MACHINEX86_X64(__m128 _mm_moveldup_ps(__m128))
    __MACHINEX86_X64(__m128 _mm_movelh_ps(__m128, __m128))
    __MACHINEX86_X64(int _mm_movemask_epi8(__m128i))
    __MACHINEX86_X64(int _mm_movemask_pd(__m128d))
    __MACHINEX86_X64(int _mm_movemask_ps(__m128))
    __MACHINEX86_X64(__m128i _mm_mpsadbw_epu8(__m128i, __m128i, int))
    __MACHINEX86_X64(__m128i _mm_mul_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_mul_epu32(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_mul_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_mul_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_mul_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_mul_ss(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_mulhi_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_mulhi_epu16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_mulhrs_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_mullo_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_mullo_epi32(__m128i, __m128i))
    __MACHINEX86_X64(void _mm_mwait(unsigned int, unsigned int))
    __MACHINEX86_X64(__m128d _mm_or_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_or_ps(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_or_si128(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_packs_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_packs_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_packus_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_packus_epi32(__m128i, __m128i))
    __MACHINEX86_X64(int _mm_popcnt_u32(unsigned int))
    __MACHINEX64(__int64 _mm_popcnt_u64(unsigned __int64))
    __MACHINEX86_X64(void _mm_prefetch(char const *, int))
    __MACHINEX86_X64(__m128 _mm_rcp_ps(__m128))
    __MACHINEX86_X64(__m128 _mm_rcp_ss(__m128))
    __MACHINEX86_X64(__m128d _mm_round_pd(__m128d, int))
    __MACHINEX86_X64(__m128 _mm_round_ps(__m128, int))
    __MACHINEX86_X64(__m128d _mm_round_sd(__m128d, __m128d, int))
    __MACHINEX86_X64(__m128 _mm_round_ss(__m128, __m128, int))
    __MACHINEX86_X64(__m128 _mm_rsqrt_ps(__m128))
    __MACHINEX86_X64(__m128 _mm_rsqrt_ss(__m128))
    __MACHINEX86_X64(__m128i _mm_sad_epu8(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_set1_epi16(short))
    __MACHINEX86_X64(__m128i _mm_set1_epi32(int))
    __MACHINEX86_X64(__m128i _mm_set1_epi64x(__int64))
    __MACHINEX86_X64(__m128i _mm_set1_epi8(char))
    __MACHINEX86_X64(__m128d _mm_set1_pd(double))
    __MACHINEX86_X64(__m128i _mm_set_epi16(short, short, short, short, short, short, short, short))
    __MACHINEX86_X64(__m128i _mm_set_epi32(int, int, int, int))
    __MACHINEX86_X64(__m128i _mm_set_epi64x(__int64, __int64))
    __MACHINEX86_X64(__m128i _mm_set_epi8(char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char))
    __MACHINEX86_X64(__m128d _mm_set_pd(double, double))
    __MACHINEX86_X64(__m128 _mm_set_ps(float, float, float, float))
    __MACHINEX86_X64(__m128 _mm_set_ps1(float))
    __MACHINEX86_X64(__m128d _mm_set_sd(double))
    __MACHINEX86_X64(__m128 _mm_set_ss(float))
    __MACHINEX86_X64(void _mm_setcsr(unsigned int))
    __MACHINEX86_X64(__m128i _mm_setl_epi64(__m128i))
    __MACHINEX86_X64(__m128i _mm_setr_epi16(short, short, short, short, short, short, short, short))
    __MACHINEX86_X64(__m128i _mm_setr_epi32(int, int, int, int))
    __MACHINEX86_X64(__m128i _mm_setr_epi64x(__int64, __int64))
    __MACHINEX86_X64(__m128i _mm_setr_epi8(char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char))
    __MACHINEX86_X64(__m128d _mm_setr_pd(double, double))
    __MACHINEX86_X64(__m128 _mm_setr_ps(float, float, float, float))
    __MACHINEX86_X64(__m128d _mm_setzero_pd(void))
    __MACHINEX86_X64(__m128 _mm_setzero_ps(void))
    __MACHINEX86_X64(__m128i _mm_setzero_si128(void))
    __MACHINEX86_X64(void _mm_sfence(void))
    __MACHINEX86_X64(__m128i _mm_shuffle_epi32(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_shuffle_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_shuffle_pd(__m128d, __m128d, int))
    __MACHINEX86_X64(__m128 _mm_shuffle_ps(__m128, __m128, unsigned int))
    __MACHINEX86_X64(__m128i _mm_shufflehi_epi16(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_shufflelo_epi16(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_sign_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_sign_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_sign_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_sll_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_sll_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_sll_epi64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_slli_epi16(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_slli_epi32(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_slli_epi64(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_slli_si128(__m128i, int))
    __MACHINEX86_X64(__m128d _mm_sqrt_pd(__m128d))
    __MACHINEX86_X64(__m128 _mm_sqrt_ps(__m128))
    __MACHINEX86_X64(__m128d _mm_sqrt_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_sqrt_ss(__m128))
    __MACHINEX86_X64(__m128i _mm_sra_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_sra_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_srai_epi16(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_srai_epi32(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_srl_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_srl_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_srl_epi64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_srli_epi16(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_srli_epi32(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_srli_epi64(__m128i, int))
    __MACHINEX86_X64(__m128i _mm_srli_si128(__m128i, int))
    __MACHINEX86_X64(void _mm_store1_pd(double *, __m128d))
    __MACHINEX86_X64(void _mm_store_pd(double *, __m128d))
    __MACHINEX86_X64(void _mm_store_ps(float *, __m128))
    __MACHINEX86_X64(void _mm_store_ps1(float *, __m128))
    __MACHINEX86_X64(void _mm_store_sd(double *, __m128d))
    __MACHINEX86_X64(void _mm_store_si128(__m128i *, __m128i))
    __MACHINEX86_X64(void _mm_store_ss(float *, __m128))
    __MACHINEX86_X64(void _mm_storeh_pd(double *, __m128d))
    __MACHINEX86_X64(void _mm_storeh_pi(__m64 *, __m128))
    __MACHINEX86_X64(void _mm_storel_epi64(__m128i *, __m128i))
    __MACHINEX86_X64(void _mm_storel_pd(double *, __m128d))
    __MACHINEX86_X64(void _mm_storel_pi(__m64 *, __m128))
    __MACHINEX86_X64(void _mm_storer_pd(double *, __m128d))
    __MACHINEX86_X64(void _mm_storer_ps(float *, __m128))
    __MACHINEX86_X64(void _mm_storeu_pd(double *, __m128d))
    __MACHINEX86_X64(void _mm_storeu_ps(float *, __m128))
    __MACHINEX86_X64(void _mm_storeu_si128(__m128i *, __m128i))
    __MACHINEX86_X64(__m128i _mm_stream_load_si128(const __m128i *))
    __MACHINEX86_X64(void _mm_stream_pd(double *, __m128d))
    __MACHINEX86_X64(void _mm_stream_ps(float *, __m128))
    __MACHINEX86_X64(void _mm_stream_sd(double *, __m128d))
    __MACHINEX86_X64(void _mm_stream_si128(__m128i *, __m128i))
    __MACHINEX86_X64(void _mm_stream_si32(int *, int))
    __MACHINEX64(void _mm_stream_si64x(__int64 *, __int64))
    __MACHINEX86_X64(void _mm_stream_ss(float *, __m128))
    __MACHINEX86_X64(__m128i _mm_sub_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_sub_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_sub_epi64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_sub_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_sub_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_sub_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_sub_sd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_sub_ss(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_subs_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_subs_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_subs_epu16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_subs_epu8(__m128i, __m128i))
    __MACHINEX86_X64(int _mm_testc_si128(__m128i, __m128i))
    __MACHINEX86_X64(int _mm_testnzc_si128(__m128i, __m128i))
    __MACHINEX86_X64(int _mm_testz_si128(__m128i, __m128i))
    __MACHINEX86_X64(int _mm_ucomieq_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_ucomieq_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_ucomige_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_ucomige_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_ucomigt_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_ucomigt_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_ucomile_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_ucomile_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_ucomilt_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_ucomilt_ss(__m128, __m128))
    __MACHINEX86_X64(int _mm_ucomineq_sd(__m128d, __m128d))
    __MACHINEX86_X64(int _mm_ucomineq_ss(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_unpackhi_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_unpackhi_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_unpackhi_epi64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_unpackhi_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_unpackhi_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_unpackhi_ps(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_unpacklo_epi16(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_unpacklo_epi32(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_unpacklo_epi64(__m128i, __m128i))
    __MACHINEX86_X64(__m128i _mm_unpacklo_epi8(__m128i, __m128i))
    __MACHINEX86_X64(__m128d _mm_unpacklo_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_unpacklo_ps(__m128, __m128))
    __MACHINEX86_X64(__m128d _mm_xor_pd(__m128d, __m128d))
    __MACHINEX86_X64(__m128 _mm_xor_ps(__m128, __m128))
    __MACHINEX86_X64(__m128i _mm_xor_si128(__m128i, __m128i))
    __MACHINEX64(__int64 _mul128(__int64 _Multiplier, __int64 _Multiplicand, __int64 *_HighProduct))
    __MACHINE(unsigned int __cdecl _rotl(unsigned int _Value, int _Shift))
    __MACHINE(unsigned short __cdecl _rotl16(unsigned short _Value, unsigned char _Shift))
    __MACHINE(unsigned __int64 __cdecl _rotl64(unsigned __int64 _Value, int _Shift))
    __MACHINE(unsigned char __cdecl _rotl8(unsigned char _Value, unsigned char _Shift))
    __MACHINE(unsigned int __cdecl _rotr(unsigned int _Value, int _Shift))
    __MACHINE(unsigned short __cdecl _rotr16(unsigned short _Value, unsigned char _Shift))
    __MACHINE(unsigned __int64 __cdecl _rotr64(unsigned __int64 _Value, int _Shift))
    __MACHINE(unsigned char __cdecl _rotr8(unsigned char _Value, unsigned char _Shift))
    __MACHINE(int __cdecl _setjmp(jmp_buf))
    __MACHINEX86_X64(void _rsm(void))
    __MACHINEX86_X64(void _lgdt(void *))
    __MACHINEX86_X64(void _sgdt(void *))
    __MACHINEX86_X64(void _clac(void))
    __MACHINEX86_X64(void _stac(void))
    __MACHINEX86_X64(unsigned char __cdecl _addcarry_u8(unsigned char, unsigned char, unsigned char, unsigned char *))
    __MACHINEX86_X64(unsigned char __cdecl _subborrow_u8(unsigned char, unsigned char, unsigned char, unsigned char *))
    __MACHINEX86_X64(unsigned char __cdecl _addcarry_u16(unsigned char, unsigned short, unsigned short, unsigned short *))
    __MACHINEX86_X64(unsigned char __cdecl _subborrow_u16(unsigned char, unsigned short, unsigned short, unsigned short *))
    __MACHINEX86_X64(unsigned char __cdecl _addcarry_u32(unsigned char, unsigned int, unsigned int, unsigned int *))
    __MACHINEX86_X64(unsigned char __cdecl _subborrow_u32(unsigned char, unsigned int, unsigned int, unsigned int *))
    __MACHINEX64(unsigned char __cdecl _addcarry_u64(unsigned char, unsigned __int64, unsigned __int64, unsigned __int64 *))
    __MACHINEX64(unsigned char __cdecl _subborrow_u64(unsigned char, unsigned __int64, unsigned __int64, unsigned __int64 *))
    __MACHINEX86_X64(void _mm_monitorx(void const *, unsigned int, unsigned int))
    __MACHINEX86_X64(void _mm_mwaitx(unsigned int, unsigned int, unsigned int))
    __MACHINEX64(unsigned int __rmpupdate(unsigned __int64, rmp_seg *, int))
    __MACHINEX64(unsigned int __psmash(unsigned __int64))
    __MACHINEX64(unsigned int __rmpadjust(unsigned __int64, int, int))
    __MACHINEX64(unsigned int __pvalidate(unsigned __int64, int, int, int *))
    __MACHINEX86_X64(void __svm_invlpgb(void *, int))
    __MACHINEX86_X64(void __svm_tlbsync(void))

#endif // NO_INTRIN

    // WINDOWS

#define WS_EX_ACCEPTFILES 0x00000010L                                             // Accepts drag and drop files
#define WS_EX_APPWINDOW 0x00040000L                                               // Forces top level window onto taskbar when window is visible
#define WS_EX_CLIENTEDGE 0x00000200L                                              // Window has border with sunken edge
#define WS_EX_COMPOSITED 0x02000000L                                              // Paints all descendants of window in bottom to top order using double buffering
#define WS_EX_CONTEXTHELP 0x00000400L                                             // Title bar of window includes question mark, can be clicked and then clicking a child window will send WM_HELP message to that window
#define WS_EX_CONTROLPARENT 0x00010000L                                           // Window contains child windows that should take part in dialog box navigation
#define WS_EX_DLGMODALFRAME 0x00000001L                                           // Window has double border
#define WS_EX_LAYERED 0x00080000                                                  // Window is layered window
#define WS_EX_LAYOUTRTL 0x00400000L                                               // If shell lang supports reading order alignment, horizontal origin of window is on right edge
#define WS_EX_LEFT 0x00000000L                                                    // Window has generic left aligned properties
#define WS_EX_LEFTSCROLLBAR 0x00004000L                                           // If shell lang supports reading order alignment, vertical scroll bar is to left of client area
#define WS_EX_LTRREADING 0x00000000L                                              // Window text display using left to right reading order
#define WS_EX_MDICHILD 0x00000040L                                                // Window is MDI child window
#define WS_EX_NOACTIVATE 0x08000000L                                              // Top level window does not become foreground window when clicked, minimized, or maximized, and does not appear on taskbar by default
#define WS_EX_NOINHERITLAYOUT 0x00100000L                                         // Window does not pass layout to child windows
#define WS_EX_NOPARENTNOTIFY 0x00000004L                                          // Child window does not send WM_PARENTNOTIFY message to parent when created or destroyed
#define WS_EX_NOREDIRECTIONBITMAP 0x00200000L                                     // Window does not render to redirection surface
#define WS_EX_RIGHT 0x00001000L                                                   // Window has generic right aligned properties
#define WS_EX_RIGHTSCROLLBAR 0x00000000L                                          // Vertical scrol lbar is to right of client area
#define WS_EX_RTLREADING 0x00002000L                                              // If shell lang supports reading order alignment, window text display right to left
#define WS_EX_STATICEDGE 0x00020000L                                              // Window has 3d border style
#define WS_EX_TOOLWINDOW 0x00000080L                                              // Window is intended to be used as a floating toolbar
#define WS_EX_TOPMOST 0x00000008L                                                 // Window should be placed above all non-topmost windows and stay above them
#define WS_EX_TRANSPARENT 0x00000020L                                             // Window should not be painted until siblings beneath have been painted
#define WS_EX_WINDOWEDGE 0x00000100L                                              // Window has border with raised edge
#define WS_EX_OVERLAPPEDWINDOW (WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE)              // Window is overlapped window
#define WS_EX_PALETTEWINDOW (WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST) // Window is palette window

#define WS_BORDER 0x00800000L                                                                                           // Window has thin line border
#define WS_CAPTION 0x00C00000L                                                                                          // Window has title bar
#define WS_CHILD 0x40000000L                                                                                            // Window is a child window
#define WS_CHILDWINDOW 0x40000000L                                                                                      // Window is a child window
#define WS_CLIPCHILDREN 0x02000000L                                                                                     // Exclude area occupied by child window when drawing parent
#define WS_CLIPSIBLINGS 0x04000000L                                                                                     // Clips child windows relative to each other
#define WS_DISABLED 0x08000000L                                                                                         // Window is initially disabled
#define WS_DLGFRAME 0x00400000L                                                                                         // Window has border of dialog box
#define WS_GROUP 0x00020000L                                                                                            // Window has first control of a group of controls
#define WS_HSCROLL 0x00100000L                                                                                          // Window has horizontal scroll bar
#define WS_ICONIC 0x20000000L                                                                                           // Window is initially minimized
#define WS_MAXIMIZE 0x01000000L                                                                                         // Window is initially maximized
#define WS_MAXIMIZEBOX 0x00010000L                                                                                      // Window has maximize box
#define WS_MINIMIZE 0x20000000L                                                                                         // Window is initially minimized
#define WS_MINIMIZEBOX 0x00020000L                                                                                      // Window has minimize box
#define WS_OVERLAPPED 0x00000000L                                                                                       // Window is an overlapped window
#define WS_POPUP 0x80000000L                                                                                            // Window is a pop-up window
#define WS_SIZEBOX 0x00040000L                                                                                          // Window has a sizing border
#define WS_SYSMENU 0x00080000L                                                                                          // Window has a window menu on title bar
#define WS_TABSTOP 0x00010000L                                                                                          // Window is a control that can recieve keyboard focus when pressing TAB
#define WS_THICKFRAME 0x00040000L                                                                                       // Window has a sizing border
#define WS_TILED 0x00000000L                                                                                            // Window is an overlapped window
#define WS_VISIBLE 0x10000000L                                                                                          // Window is initially visible
#define WS_VSCROLL 0x00200000L                                                                                          // Window has a vertical scroll bar
#define WS_TILEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)      // Window is an overlapped window
#define WS_POPUPWINDOW (WS_POPUP | WS_BORDER | WS_SYSMENU)                                                              // Window is a pop-up window
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX) // Window is an overlapped window

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))                                                                         // Get signed x coord from LPARAM
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))                                                                         // Get signed y coord from LPARAM
#define HIBYTE(w) ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))                                                                // Get high order byte from 16 bit value
#define HIWORD(l) ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))                                                             // Get high order word from 32 bit value
#define LOBYTE(w) ((BYTE)(((DWORD_PTR)(w)) & 0xff))                                                                       // Get low order byte from 16 bit value
#define LOWORD(l) ((WORD)(((DWORD_PTR)(l)) & 0xffff))                                                                     // Get low order word from 32 bit value
#define MAKELONG(a, b) ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16)) // Create LONG value by concatenating values
#define MAKELPARAM(l, h) ((LPARAM)(DWORD)MAKELONG(l, h))                                                                  // Create value for use as LPARAM in a message with concatenation
#define MAKELRESULT(l, h) ((LRESULT)(DWORD)MAKELONG(l, h))                                                                // Create value for use as LRESULT in a message with concatenation
#define MAKEWORD(a, b) ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))       // Creaate WORD value by concatenating values
#define MAKEWPARAM(l, h) ((WPARAM)(DWORD)MAKELONG(l, h))                                                                  // Create value for use as WPARAM in a message with concatenation

#define MN_GETHMENU 0x01E1               // Get menu handle for current window
#define WM_ERASEBKGND 0x0014             // Window background must be erased
#define WM_GETFONT 0x0031                // Get font with which control is current drawing text
#define WM_GETTEXT 0x000D                // Copy text corresponding with window into a buffer provided by caller
#define WM_GETTEXTLENGTH 0x000E          // Determine length in characters of text associated with a window
#define WM_SETFONT 0x0030                // Set font that control is to use when drawing text
#define WM_SETICON 0x0080                // Associate new large or small icon with window
#define WM_SETTEXT 0x000C                // Set text of a window
#define WM_ACTIVATEAPP 0x001C            // Window belonging to different app than active window is about to be activated
#define WM_CANCELMODE 0x001F             // Cancel certain modes
#define WM_CHILDACTIVATE 0x0022          // Child window's title bar is clicked, or window is actived, moved, or sized
#define WM_CLOSE 0x0010                  // Window or application should terminate
#define WM_COMPACTING 0x0041             // system detects more than 12.5 percent of system time over 30-60 seconds spent on compacting memory
#define WM_CREATE 0x0001                 // Application requests window to be created (after window is created but before it's visible)
#define WM_DESTROY 0x0002                // Window is being destroyed after window is removed from screen
#define WM_DPICHANGED 0x02E0             // Dpi for window has changed
#define WM_ENABLE 0x000A                 // Enabled state of window changed
#define WM_ENTERSIZEMOVE 0x0231          // Window has entered moving or sizing modal loop
#define WM_EXITSIZEMOVE 0x0232           // Window has exited moving or sizing modal loop
#define WM_GETICON 0x007F                // Get handle to icon associated with window
#define WM_GETMINMAXINFO 0x0024          // Size or position is about to change
#define WM_INPUTLANGCHANGE 0x0051        // Application input lang changed
#define WM_INPUTLANGCHANGEREQUEST 0x0050 // User chose new input lang
#define WM_MOVE 0x0003                   // Window moved
#define WM_MOVING 0x0216                 // Window is moving
#define WM_NCACTIVATE 0x0086             // Non-client area needs to be changed to indicate active or inactive state
#define WM_NCCALCSIZE 0x0083             // Size and pos of window client area must be calculated
#define WM_NCCREATE 0x0081               // Window is created (before WM_CREATE)
#define WM_NCDESTROY 0x0082              // Non-client area is being destroyed
#define WM_NULL 0x0000                   // No op
#define WM_QUERYDRAGICON 0x0037          // Minimized window about to be dragged by user
#define WM_QUERYOPEN 0x0013              // User requests window be restored
#define WM_QUIT 0x0012                   // Terminate application
#define WM_SHOWWINDOW 0x0018             // Window about to be hidden or shown
#define WM_SIZE 0x0005                   // Window size changed
#define WM_SIZING 0x0214                 // Window is resizing
#define WM_STYLECHANGED 0x007D           // Window style changed
#define WM_STYLECHANGING 0x007C          // Window style about to change
#define WM_THEMECHANGED 0x031A           // Theme change event
#define WM_USERCHANGED 0x0054            // User logged on or off
#define WM_WINDOWPOSCHANGED 0x0047       // Window size, position, or z order changed
#define WM_WINDOWPOSCHANGING 0x0046      // Window size, position, or z order about to change

#define CW_USEDEFAULT ((int)0x80000000)

#define CCHILDREN_TITLEBAR 5 // Amount of elements in title bar

    // Visual feeback associated with event
    typedef enum tagFEEDBACK_TYPE
    {
        FEEDBACK_TOUCH_CONTACTVISUALIZATION = 1,
        FEEDBACK_PEN_BARRELVISUALIZATION = 2,
        FEEDBACK_PEN_TAP = 3,
        FEEDBACK_PEN_DOUBLETAP = 4,
        FEEDBACK_PEN_PRESSANDHOLD = 5,
        FEEDBACK_PEN_RIGHTTAP = 6,
        FEEDBACK_TOUCH_TAP = 7,
        FEEDBACK_TOUCH_DOUBLETAP = 8,
        FEEDBACK_TOUCH_PRESSANDHOLD = 9,
        FEEDBACK_TOUCH_RIGHTTAP = 10,
        FEEDBACK_GESTURE_PRESSANDTAP = 11,
        FEEDBACK_MAX = 0xFFFFFFFF
    } FEEDBACK_TYPE;

    typedef struct _BLENDFUNCTION
    {
        BYTE BlendOp;
        BYTE BlendFlags;
        BYTE SourceConstantAlpha;
        BYTE AlphaFormat;
    } BLENDFUNCTION, *PBLENDFUNCTION;

    // Status info for application switching (alt + tab)
    typedef struct tagALTTABINFO
    {
        DWORD cbSize;
        int cItems;
        int cColumns;
        int cRows;
        int iColFocus;
        int iRowFocus;
        int cxItem;
        int cyItem;
        POINT ptStart;
    } ALTTABINFO, *PALTTABINFO, *LPALTTABINFO;

    // Extended result info from ChangeWindowMessageFilterEx
    typedef struct tagCHANGEFILTERSTRUCT
    {
        DWORD cbSize;
        DWORD ExtStatus;
    } CHANGEFILTERSTRUCT, *PCHANGEFILTERSTRUCT;

    // Info about menu and first MDI child window of MDI client window
    typedef struct tagCLIENTCREATESTRUCT
    {
        HANDLE hWindowMenu;
        UINT idFirstChild;
    } CLIENTCREATESTRUCT, *LPCLIENTCREATESTRUCT;

    // Info about a GUI thread
    typedef struct tagGUITHREADINFO
    {
        DWORD cbSize;
        DWORD flags;
        HWND hwndActive;
        HWND hwndFocus;
        HWND hwndCapture;
        HWND hwndMenuOwner;
        HWND hwndMoveSize;
        HWND hwndCaret;
        RECT rcCaret;
    } GUITHREADINFO, *PGUITHREADINFO, *LPGUITHREADINFO;

    // Info about a window's maximized size and pos, and minimum and maximum tracking size
    typedef struct tagMINMAXINFO
    {
        POINT ptReserved;
        POINT ptMaxSize;
        POINT ptMaxPosition;
        POINT ptMinTrackSize;
        POINT ptMaxTrackSize;
    } MINMAXINFO, *PMINMAXINFO, *LPMINMAXINFO;

    // Info about size and pos of window
    typedef struct tagWINDOWPOS
    {
        HWND hwnd;
        HWND hwndInsertAfter;
        int x;
        int y;
        int cx;
        int cy;
        UINT flags;
    } WINDOWPOS, *LPWINDOWPOS, *PWINDOWPOS;

    // Info for app to use while processing WM_NCCALCSIZE
    typedef struct tagNCCALCSIZE_PARAMS
    {
        RECT rgrc[3];
        PWINDOWPOS lppos;
    } NCCALCSIZE_PARAMS, *LPNCCALCSIZE_PARAMS;

    // Styles for a window
    typedef struct tagSTYLESTRUCT
    {
        DWORD styleOld;
        DWORD styleNew;
    } STYLESTRUCT, *LPSTYLESTRUCT;

    // Title bar info
    typedef struct tagTITLEBARINFO
    {
        DWORD cbSize;
        RECT rcTitleBar;
        DWORD rgstate[CCHILDREN_TITLEBAR + 1];
    } TITLEBARINFO, *PTITLEBARINFO, *LPTITLEBARINFO;

    // Title bar info with coords of each element
    typedef struct tagTITLEBARINFOEX
    {
        DWORD cbSize;
        RECT rcTitleBar;
        DWORD rgstate[CCHILDREN_TITLEBAR + 1];
        RECT rgrect[CCHILDREN_TITLEBAR + 1];
    } TITLEBARINFOEX, *PTITLEBARINFOEX, *LPTITLEBARINFOEX;

    // Pos, size, shape, content, and translucency info for layered window
    typedef struct tagUPDATELAYEREDWINDOWINFO
    {
        DWORD cbSize;
        HDC hdcDst;
        const POINT *pptDst;
        const SIZE *psize;
        HDC hdcSrc;
        const POINT *pptSrc;
        COLORREF crKey;
        const BLENDFUNCTION *pblend;
        DWORD dwFlags;
        const RECT *prcDirty;
    } UPDATELAYEREDWINDOWINFO, *PUPDATELAYEREDWINDOWINFO;

    // Window info
    typedef struct tagWINDOWINFO
    {
        DWORD cbSize;
        RECT rcWindow;
        RECT rcClient;
        DWORD dwStyle;
        DWORD dwExStyle;
        DWORD dwWindowStatus;
        UINT cxWindowBorders;
        UINT cyWindowBorders;
        ATOM atomWindowType;
        WORD wCreatorVersion;
    } WINDOWINFO, *PWINDOWINFO, *LPWINDOWINFO;

    // Info about placement of window on screen
    typedef struct tagWINDOWPLACEMENT
    {
        UINT length;
        UINT flags;
        UINT showCmd;
        POINT ptMinPosition;
        POINT ptMaxPosition;
        RECT rcNormalPosition;
        RECT rcDevice;
    } WINDOWPLACEMENT;

#ifdef UNICODE
    // Init parameters passed to window proc of application
    typedef struct tagCREATESTRUCTW
    {
        LPVOID lpCreateParams;
        HINSTANCE hInstance;
        HMENU hMenu;
        HWND hwndParent;
        int cy;
        int cx;
        int y;
        int x;
        LONG style;
        LPCWSTR lpszName;
        LPCWSTR lpszClass;
        DWORD dwExStyle;
    } CREATESTRUCTW, *LPCREATESTRUCTW;
    typedef CREATESTRUCTW CREATESTRUCT;
    typedef LPCREATESTRUCTW LPCREATESTRUCT;
#else
    typedef struct tagCREATESTRUCTA
    {
        LPVOID lpCreateParams;
        HINSTANCE hInstance;
        HMENU hMenu;
        HWND hwndParent;
        int cy;
        int cx;
        int y;
        int x;
        LONG style;
        LPCSTR lpszName;
        LPCSTR lpszClass;
        DWORD dwExStyle;
    } CREATESTRUCTA, *LPCREATESTRUCTA;
    typedef CREATESTRUCTA CREATESTRUCT;
    typedef LPCREATESTRUCTA LPCREATESTRUCT;
#endif

    typedef LRESULT(CALLBACK *WNDPROC)(HWND, UINT, WPARAM, LPARAM);                                                                                       // User-defined callback function to process messages sent to a window
    typedef BOOL(CALLBACK *WNDENUMPROC)(HWND, LPARAM);                                                                                                    // User-defined callback for enumerating windows from a context
    BOOL WINAPI AdjustWindowRect(LPRECT lpRect, DWORD dwStyle, BOOL bMenu);                                                                               // Calculate required size of window rect based on desired client rect
    BOOL WINAPI AdjustWindowRectEx(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle);                                                            // Calculate required size of window rect based on desired client rect (for CreateWindowEx)
    BOOL WINAPI AllowSetForegroundWindow(DWORD dwProcessId);                                                                                              // Enables process to set foreground window
    BOOL WINAPI AnimateWindow(HWND hWnd, DWORD dwTime, DWORD dwFlags);                                                                                    // Enables ability to produce special effects when hiding or showing window
    BOOL WINAPI AnyPopup();                                                                                                                               // Indicated whether owned, top level pop-up, or overlapped window exists on screen
    UINT WINAPI ArrangeIconicWindows(HWND hWnd);                                                                                                          // Arrange all minimized child windows of parent
    HDWP WINAPI BeginDeferWindowPos(int nNumWindows);                                                                                                     // Allocate memory for multiple window position struct
    BOOL WINAPI BringWindowToTop(HWND hWnd);                                                                                                              // Bring window to top of z order
    BOOL WINAPI CalculatePopupWindowPosition(const POINT *anchorPoint, const SIZE *windowSize, UINT flags, RECT *excludeRect, RECT *popupWindowPosition); // Calculate pop-up pos using anchor  point, pop-up window size, flags, and optional exclude rect
    WORD WINAPI CascadeWindows(HWND hwndParent, UINT wHow, const RECT *lpRect, UINT cKids, const HWND *lpKids);                                           // Cascade chlid windows of parent
    BOOL WINAPI ChangeWindowMessageFilter(UINT message, DWORD dwFlag);                                                                                    // Add or remove message from UIPI message filter (entire process)
    BOOL WINAPI ChangeWindowMessageFilterEx(HWND hwnd, UINT message, DWORD action, PCHANGEFILTERSTRUCT pChangeFilterStruct);                              // Modify UIPI message filter for window
    HWND WINAPI ChildWindowFromPoint(HWND hWndParent, POINT Point);                                                                                       // Determine which child windows contain specified point
    HWND WINAPI ChildWindowFromPointEx(HWND hwnd, POINT pt, UINT flags);                                                                                  // Determine which child windows contain specified point
    BOOL WINAPI CloseWindow(HWND hWnd);                                                                                                                   // Minimizes window
    HDWP WINAPI DeferWindowPos(HDWP hWinPosInfo, HWND hWnd, HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags);                             // Update multiple window pos struct for a window
    BOOL WINAPI DeregisterShellHookWindow(HWND hwnd);                                                                                                     // Unregister shell window that was registered to receive shell hook messages
    BOOL WINAPI DestroyWindow(HWND hWnd);                                                                                                                 // Destroys window
    BOOL WINAPI EndDeferWindowPos(HDWP hWinPosInfo);                                                                                                      // Update position and size of windows in single v-sync cycle
    BOOL WINAPI EndTask(HWND hWnd, BOOL fShutDown, BOOL fForce);                                                                                          // Forces window to close
    BOOL WINAPI EnumChildWindows(HWND hWndParent, WNDENUMPROC lpEnumFunc, LPARAM lParam);                                                                 // Enumerate child windows of parent
    BOOL WINAPI EnumThreadWindows(DWORD dwThreadId, WNDENUMPROC lpfn, LPARAM lParam);                                                                     // Enumerate non child windows of thread
    BOOL WINAPI EnumWindows(WNDENUMPROC lpEnumFunc, LPARAM lParam);                                                                                       // Enumerate all top level windows on screen
    HWND WINAPI GetAncestor(HWND hwnd, UINT gaFlags);                                                                                                     // Get handle to ancestor of window
    BOOL WINAPI GetClientRect(HWND hWnd, LPRECT lpRect);                                                                                                  // Get coords of window client area
    HWND WINAPI GetDesktopWindow();                                                                                                                       // Get handle to desktop window
    HWND WINAPI GetForegroundWindow();                                                                                                                    // Get handle to current active window
    BOOL WINAPI GetGUIThreadInfo(DWORD idThread, PGUITHREADINFO pgui);                                                                                    // Get info about actifve window or GUI thread
    HWND WINAPI GetLastActivePopup(HWND hWnd);                                                                                                            // Check which pop-up window was most recently active
    BOOL WINAPI GetLayeredWindowAttributes(HWND hwnd, COLORREF *pcrKey, BYTE *pbAlpha, DWORD *pdwFlags);                                                  // Get opacity and transparency color key of layered window
    HWND WINAPI GetParent(HWND hWnd);                                                                                                                     // Get handle to window's parent
    BOOL WINAPI GetProcessDefaultLayout(DWORD *pdwDefaultLayout);                                                                                         // Get default layout used when window are created without parent
    HWND WINAPI GetShellWindow();                                                                                                                         // Get handle to shell's desktop window
    DWORD WINAPI GetSysColor(int nIndex);                                                                                                                 // Get current color of display element
    BOOL WINAPI GetTitleBarInfo(HWND hwnd, PTITLEBARINFO pti);                                                                                            // Get info about title bar
    HWND WINAPI GetTopWindow(HWND hWnd);                                                                                                                  // Get window at top of z order
    HWND WINAPI GetWindow(HWND hWnd, UINT uCmd);                                                                                                          // Get window that has specified relationship to a window
#define GetNextWindow(hWnd, wCmd) GetWindow(hWnd, wCmd)
    BOOL WINAPI GetWindowDisplayAffinity(HWND hWnd, DWORD *pdwAffinity);                                                                                                 // Get current display affinity setting from process or window
    BOOL WINAPI GetWindowInfo(HWND hwnd, PWINDOWINFO pwi);                                                                                                               // Get info about window
    BOOL WINAPI GetWindowPlacement(HWND hWnd, WINDOWPLACEMENT *lpwndpl);                                                                                                 // Get the show state and restored, minimized, and maximized positions of window
    BOOL WINAPI GetWindowRect(HWND hWnd, LPRECT lpRect);                                                                                                                 // Get dimensions of bounding rect of window
    DWORD WINAPI GetWindowThreadProcessId(HWND hWnd, LPDWORD lpdwProcessId);                                                                                             // Get identifier of thread that created window, and id of process that created window
    int WINAPI InternalGetWindowText(HWND hWnd, LPWSTR pString, int cchMaxCount);                                                                                        // Get text of window's title bar as unicode directly from window struct
    BOOL WINAPI IsChild(HWND hWndParent, HWND hWnd);                                                                                                                     // Check if window is child of another window
    BOOL WINAPI IsGUIThread(BOOL bConvert);                                                                                                                              // Check if calling thread is already a GUI thread
    BOOL WINAPI IsHungAppWindow(HWND hwnd);                                                                                                                              // Check whether system considers application is not responding
    BOOL WINAPI IsIconic(HWND hWnd);                                                                                                                                     // Check if window is minimized
    BOOL WINAPI IsProcessDPIAware();                                                                                                                                     // Check if process is dpi aware
    BOOL WINAPI IsWindow(HWND hWnd);                                                                                                                                     // Check if window handle is valid
    BOOL WINAPI IsWindowUnicode(HWND hWnd);                                                                                                                              // Check if window is native unicode window
    BOOL WINAPI IsWindowVisible(HWND hWnd);                                                                                                                              // Check if window is visible
    BOOL WINAPI IsZoomed(HWND hWnd);                                                                                                                                     // Check if window is maximized
    BOOL WINAPI LockSetForegroundWindow(UINT uLockCode);                                                                                                                 // Disable calls to SetForegroundWindow
    BOOL WINAPI LogicalToPhysicalPoint(HWND hWnd, LPPOINT lpPoint);                                                                                                      // Convert logical coords of point in window to physical coords
    BOOL WINAPI MoveWindow(HWND hWnd, int X, int Y, int nWidth, int nHeight, BOOL bRepaint);                                                                             // Change pos and dimensions of window
    BOOL WINAPI OpenIcon(HWND hWnd);                                                                                                                                     // Restore minimized window to previous size and pos, and activate it
    BOOL WINAPI PhysicalToLogicalPoint(HWND hWnd, LPPOINT lpPoint);                                                                                                      // Convert physical coords of point in window to logical coords
    HWND WINAPI RealChildWindowFromPoint(HWND hwndParent, POINT ptParentClientCoords);                                                                                   // Get handle to child window at a point
    BOOL WINAPI RegisterShellHookWindow(HWND hwnd);                                                                                                                      // Register specified shell window to receive messages for events or notifications
    BOOL WINAPI SetForegroundWindow(HWND hWnd);                                                                                                                          // Bring thread that created window into foreground and activate window
    BOOL WINAPI SetLayeredWindowAttributes(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);                                                                       // Set opacity and transparency color key of layered window
    HWND WINAPI SetParent(HWND hWndChild, HWND hWndParent);                                                                                                              // Change parent of child window
    BOOL WINAPI SetProcessDefaultLayout(DWORD dwDefaultLayout);                                                                                                          // Change default layout when window are created with no parent
    BOOL WINAPI SetProcessDPIAware();                                                                                                                                    // Set process default dpi awareness
    BOOL WINAPI SetSysColors(int cElements, const INT *lpaElements, const COLORREF *lpaRgbValues);                                                                       // Set colords for display elements of a window
    BOOL WINAPI SetWindowDisplayAffinity(HWND hWnd, DWORD dwAffinity);                                                                                                   // Specifies where content of window can be displayed
    BOOL WINAPI SetWindowFeedbackSettings(HWND hwnd, FEEDBACK_TYPE feedback, DWORD flags, UINT32 size, const VOID *configuration);                                       // Set feedback setting for window
    BOOL WINAPI SetWindowPlacement(HWND hWnd, const WINDOWPLACEMENT *lpwndpl);                                                                                           // Set show state and restored, minimized, and maximized positions of window
    BOOL WINAPI SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);                                                                // Change size, position, and z order of child, pop-up, or top level window
    BOOL WINAPI ShowOwnedPopups(HWND hWnd, BOOL fShow);                                                                                                                  // Show or hide all pop-up windows owned
    BOOL WINAPI ShowWindow(HWND hWnd, int nCmdShow);                                                                                                                     // Set window's show state
    BOOL WINAPI ShowWindowAsync(HWND hWnd, int nCmdShow);                                                                                                                // Set window show state without waiting for operation to complete
    BOOL WINAPI SoundSentry();                                                                                                                                           // Trigger visual signal to indicate sound is playing
    void WINAPI SwitchToThisWindow(HWND hwnd, BOOL fUnknown);                                                                                                            // Switches focus to window and brings it to foreground
    WORD WINAPI TileWindows(HWND hwndParent, UINT wHow, const RECT *lpRect, UINT cKids, const HWND *lpKids);                                                             // Tiles child windows of parent
    BOOL WINAPI UpdateLayeredWindow(HWND hWnd, HDC hdcDst, POINT *pptDst, SIZE *psize, HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags); // Update position, size, shape, content, and translucency of layered window
    BOOL WINAPI UpdateLayeredWindowIndirect(HWND hwnd, const UPDATELAYEREDWINDOWINFO *pULWInfo);                                                                         // Update position size, shape, content, and translucency of layered window
    HWND WINAPI WindowFromPhysicalPoint(POINT Point);                                                                                                                    // Get window that contains specified physical point
    HWND WINAPI WindowFromPoint(POINT Point);                                                                                                                            // Get window that contains specified point
#ifdef UNICODE
    HWND WINAPI CreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam); // Create overlapped, pop-up, or child window
#define CreateWindowW(lpClassName, lpWindowName, dwStyle, x, y,               \
                      nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam) \
    CreateWindowExW(0L, lpClassName, lpWindowName, dwStyle, x, y,             \
                    nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)
#define CreateWindow CreateWindowW
    BOOL WINAPI FindWindowW(LPCWSTR lpClassName, LPCWSTR lpWindowName);                                       // Get handle to top level window of matching name and class
    HWND WINAPI FindWindowExW(HWND hWndParent, HWND hWndChildAfter, LPCWSTR lpszClass, LPCWSTR lpszWindow);   // Get handle to top level window of matching name and class
    BOOL WINAPI GetAltTabInfoW(HWND hwnd, int iItem, PALTTABINFO pati, LPWSTR pszItemText, UINT cchItemText); // Get status info for window if it's application switching window
#define FindWindow FindWindowW
#define FindWindowEx FindWindowExW
#define GetAltTabInfo GetAltTabInfoW
    UINT WINAPI GetWindowModuleFileNameW(HWND hwnd, LPWSTR pszFileName, UINT cchFileNameMax); // Get full path and file name of module associated with window
#define GetWindowModuleFileName GetWindowModuleFileNameW
    int WINAPI GetWindowTextW(HWND hWnd, LPWSTR lpString, int nMaxCount); // Copy text of window's title bar
    int WINAPI GetWindowTextLengthW(HWND hWnd);                           // Get length in characters of window's title bar text
#define GetWindowText GetWindowTextW
#define GetWindowTextLength GetWindowTextLengthW
    UINT WINAPI RealGetWindowClassW(HWND hwnd, LPWSTR ptszClassName, UINT cchClassNameMax); // Change text of window's title bar
#define RealGetWindowClass RealGetWindowClassW
    BOOL WINAPI SetWindowTextW(HWND hWnd, LPCWSTR lpString); // Change text of window's title bar
#define SetWindowText SetWindowTextW
#else
    HWND WINAPI CreateWindowExW(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam); // Create overlapped, pop-up, or child window
#define CreateWindowA(lpClassName, lpWindowName, dwStyle, x, y,               \
                      nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam) \
    CreateWindowExA(0L, lpClassName, lpWindowName, dwStyle, x, y,             \
                    nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)
#define CreateWindow CreateWindowA
    BOOL WINAPI FindWindowA(LPCSTR lpClassName, LPCSTR lpWindowName);                                                                                                                                               // Get handle to top level window of matching name and class
    HWND WINAPI FindWindowExA(HWND hWndParent, HWND hWndChildAfter, LPCSTR lpszClass, LPCSTR lpszWindow);                                                                                                           // Get handle to top level window of matching name and class
    BOOL WINAPI GetAltTabInfoA(HWND hwnd, int iItem, PALTTABINFO pati, LPSTR pszItemText, UINT cchItemText);                                                                                                        // Get status info for window if it's application switching window
#define FindWindow FindWindowA
#define FindWindowEx FindWindowExA
#define GetAltTabInfo GetAltTabInfoA
    UINT WINAPI GetWindowModuleFileNameA(HWND hwnd, LPSTR pszFileName, UINT cchFileNameMax);                                                                                                                        // Get full path and file name of module associated with window
#define GetWindowModuleFileName GetWindowModuleFileNameA
    int WINAPI GetWindowTextA(HWND hWnd, LPSTR lpString, int nMaxCount);                                                                                                                                            // Copy text of window's title bar
    int WINAPI GetWindowTextLengthA(HWND hWnd);                                                                                                                                                                     // Get length in characters of window's title bar text
#define GetWindowText GetWindowTextA
#define GetWindowTextLength GetWindowTextLengthA
    UINT WINAPI RealGetWindowClassA(HWND hwnd, LPSTR ptszClassName, UINT cchClassNameMax);                                                                                                                          // Change text of window's title bar
#define RealGetWindowClass RealGetWindowClassA
    BOOL WINAPI SetWindowTextA(HWND hWnd, LPCSTR lpString);                                                                                                                                                         // Change text of window's title bar
#define SetWindowText SetWindowTextA
#endif

#endif // NO_WINDOWS

    // WINDOW CLASSES

#ifndef NO_WINDOW_CLASSES

#define CS_BYTEALIGNCLIENT 0x1000
#define CS_BTYEALIGNWINDOW 0x2000
#define CS_CLASSDC 0x0040
#define CS_DBLCLKS 0x0008
#define CS_DROPSHADOW 0x00020000
#define CS_GLOBALCLASS 0x4000
#define CS_HREDRAW 0x0002
#define CS_NOCLOSE 0x0200
#define CS_OWNDC 0x0020
#define CS_PARENTDC 0x0080
#define CS_SAVEBITS 0x0800
#define CS_VREDRAW 0x0001

#ifdef UNICODE
    // Contains window class info used with RegisterClass
    typedef struct tagWNDCLASSW
    {
        UINT style;
        WNDPROC lpfnWndProc;
        int cbClsExtra;
        int cbWndExtra;
        HINSTANCE hInstance;
        HICON hIcon;
        HCURSOR hCursor;
        HBRUSH hbrBackground;
        LPCWSTR lpszMenuName;
        LPCWSTR lpszClassName;
    } WNDCLASSW, *PWNDCLASSW, *NPWNDCLASSW, *LPWNDCLASSW;
    typedef WNDCLASSW WNDCLASS;
    typedef LPWNDCLASSW LPWNDCLASS;
    // Contains window class info used with RegisterClassEx
    typedef struct tagWNDCLASSEXW
    {
        UINT cbSize;
        UINT style;
        WNDPROC lpfnWndProc;
        int cbClsExtra;
        int cbWndExtra;
        HINSTANCE hInstance;
        HICON hIcon;
        HCURSOR hCursor;
        HBRUSH hbrBackground;
        LPCWSTR lpszMenuName;
        LPCWSTR lpszClassName;
        HICON hIconSm;
    } WNDCLASSEXW, *PWNDCLASSEXW, *NPWNDCLASSEXW, *LPWNDCLASSEXW;
    typedef WNDCLASSEXW WNDCLASSEX;
    typedef LPWNDCLASSEXW LPWNDCLASSEX;
#else
    // Contains window class info used with RegisterClass
    typedef struct tagWNDCLASSA
    {
        UINT style;
        WNDPROC lpfnWndProc;
        int cbClsExtra;
        int cbWndExtra;
        HINSTANCE hInstance;
        HICON hIcon;
        HCURSOR hCursor;
        HBRUSH hbrBackground;
        LPCSTR lpszMenuName;
        LPCSTR lpszClassName;
    } WNDCLASSA, *PWNDCLASSA, *NPWNDCLASSA, *LPWNDCLASSA;
    typedef WNDCLASSA WNDCLASS;
    typedef LPWNDCLASSA LPWNDCLASS;
    // Contains window class info used with RegisterClass
    typedef struct tagWNDCLASSEXA
    {
        UINT cbSize;
        UINT style;
        WNDPROC lpfnWndProc;
        int cbClsExtra;
        int cbWndExtra;
        HINSTANCE hInstance;
        HICON hIcon;
        HCURSOR hCursor;
        HBRUSH hbrBackground;
        LPCSTR lpszMenuName;
        LPCSTR lpszClassName;
        HICON hIconSm;
    } WNDCLASSEXA, *PWNDCLASSEXA, *NPWNDCLASSEXA, *LPWNDCLASSEXA;
    typedef WNDCLASSEXA WNDCLASSEX;
    typedef LPWNDCLASSEXA LPWNDCLASSEX;
#endif

    WORD WINAPI GetClassWord(HWND hWnd, int nIndex);                // Get WORD value at offset into extra window class memory of window
    WORD WINAPI SetClassWord(HWND hWnd, int nIndex, WORD wNewWord); // Replace WORD value at offset into extra window class memory of window
#ifdef UNICODE
    BOOL WINAPI GetClassInfoW(HINSTANCE hInstance, LPCWSTR lpClassName, LPWNDCLASSW lpWndClass); // Get info of window class
    BOOL WINAPI GetClassInfoExW(HINSTANCE hInstance, LPCWSTR lpszClass, LPWNDCLASSEXW lpwcx);    // Get info of window class, including icon associated
    DWORD WINAPI GetClassLongW(HWND hWnd, int nIndex);                                           // Get DWORD from WNDCLASSEX associated with window
    ULONG_PTR WINAPI GetClassLongPtrW(HWND hWnd, int nIndex);                                    // Get specified value from WNDCLASSEX struct associated with window
    int WINAPI GetClassNameW(HWND hWnd, LPWSTR lpClassName, int nMaxCount);                      // Get name of class to window
#define GetClassInfo GetClassInfoW
#define GetClassInfoEx GetClassInfoExW
#define GetClassLong GetClassLongW
#define GetClassLongPtr GetClassLongPtrW
#define GetClassName GetClassNameW
    LONG WINAPI GetWindowLongW(HWND hWnd, int nIndex);                            // Get DWORD value at offset into extra window class memory of window
    LONG_PTR WINAPI GetWindowLongPtrW(HWND hWnd, int nIndex);                     // Get value at offset into extra window class memory of window
    ATOM WINAPI RegisterClassW(const WNDCLASSW *lpWndClass);                      // Register window class for calls to CreateWindow
    ATOM WINAPI RegisterClassExW(const WNDCLASSEXW *);                            // Register window class for call to CreateWindowEX
    DWORD WINAPI SetClassLongW(HWND hWnd, int nIndex, LONG dwNewLong);            // Replace long value at offset into extra window class memory of window
    ULONG_PTR WINAPI SetClassLongPtrW(HWND hWnd, int nIndex, LONG_PTR dwNewLong); // Replace value at offset in extra window class memory of window
#define GetWindowLong GetWindowLongW
#define GetWindowLongPtr GetWindowLongPtrW
#define RegisterClass RegisterClassW
#define RegisterClassEx RegisterClassExW
#define SetClassLong SetClassLongW
#define SetClassLongPtr SetClassLongPtrW
    LONG WINAPI SetWindowLongW(HWND hWnd, int nIndex, LONG dwNewLong);            // Set long value at offset into extra window class memory of window
    LONG_PTR WINAPI SetWindowLongPtrW(HWND hWnd, int nIndex, LONG_PTR dwNewLong); // Set value at offset into extra window class memory of window
    BOOL WINAPI UnregisterClassW(LPCWSTR lpClassName, HINSTANCE hInstance);       // Unregister window class
#define SetWindowLong SetWindowLongW
#define SetWindowLongPtr SetWindowLongPtrW
#define UnregisterClass UnregisterClassW
#else
    BOOL WINAPI GetClassInfoA(HINSTANCE hInstance, LPCSTR lpClassName, LPWNDCLASSA lpWndClass);                                                // Get info of window class
    BOOL WINAPI GetClassInfoExA(HINSTANCE hInstance, LPCSTR lpszClass, LPWNDCLASSEXA lpwcx);                                                   // Get info of window class, including icon associated
    DWORD WINAPI GetClassLongA(HWND hWnd, int nIndex);                                                                                         // Get DWORD from WNDCLASSEX associated with window
    ULONG_PTR WINAPI GetClassLongPtrA(HWND hWnd, int nIndex);                                                                                  // Get specified value from WNDCLASSEX struct associated with window
    int WINAPI GetClassNameA(HWND hWnd, LPSTR lpClassName, int nMaxCount);                                                                     // Get name of class to window
#define GetClassInfo GetClassInfoA
#define GetClassInfoEx GetClassInfoExA
#define GetClassLong GetClassLongA
#define GetClassLongPtr GetClassLongPtrA
#define GetClassName GetClassNameA
    LONG WINAPI GetWindowLongA(HWND hWnd, int nIndex);                                                                                         // Get DWORD value at offset into extra window class memory of window
    LONG_PTR WINAPI GetWindowLongPtrA(HWND hWnd, int nIndex);                                                                                  // Get value at offset into extra window class memory of window
    ATOM WINAPI RegisterClassA(const WNDCLASSA *lpWndClass);                                                                                   // Register window class for calls to CreateWindow
    ATOM WINAPI RegisterClassExA(const WNDCLASSEXA *);                                                                                         // Register window class for call to CreateWindowEX
    DWORD WINAPI SetClassLongA(HWND hWnd, int nIndex, LONG dwNewLong);                                                                         // Replace long value at offset into extra window class memory of window
    ULONG_PTR WINAPI SetClassLongPtrA(HWND hWnd, int nIndex, LONG_PTR dwNewLong);                                                              // Replace value at offset in extra window class memory of window
#define GetWindowLong GetWindowLongA
#define GetWindowLongPtr GetWindowLongPtrW
#define RegisterClass RegisterClassA
#define RegisterClassEx RegisterClassExA
#define SetClassLong SetClassLongA
#define SetClassLongPtr SetClassLongPtrA
    LONG WINAPI SetWindowLongA(HWND hWnd, int nIndex, LONG dwNewLong);                                                                         // Set long value at offset into extra window class memory of window
    LONG_PTR WINAPI SetWindowLongPtrA(HWND hWnd, int nIndex, LONG_PTR dwNewLong);                                                              // Set value at offset into extra window class memory of window
    BOOL WINAPI UnregisterClassA(LPCSTR lpClassName, HINSTANCE hInstance);                                                                     // Unregister window class
#define SetWindowLong SetWindowLongA
#define SetWindowLongPtr SetWindowLongPtrA
#define UnregisterClass UnregisterClassA
#endif

#endif // NO_WINDOW_CLASSES

    // WINDOW PROCEDURES

#ifndef NO_WINDOW_PROCEDURES

#ifdef UNICODE
    LRESULT WINAPI CallWindowProcW(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam); // Pass message info to specified window proc
    LRESULT WINAPI DefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);                         // Call default window proc
#define CallWindowProc CallWindowProcW
#define DefWindowProc DefWindowProcW
#else
    LRESULT WINAPI CallWindowProcA(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);                                  // Pass message info to specified window proc
    LRESULT WINAPI DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);                                                          // Call default window proc
#define CallWindowProc CallWindowProcA
#define DefWindowProc DefWindowProcA
#endif

#endif // NO_WINDOW_PROCEDURES

    // MESSAGES

#ifndef NO_WINDOW_MESSAGES

#define QS_KEY 0x0001
#define QS_MOUSEMOVE 0x0002
#define QS_MOUSEBUTTON 0x0004
#define QS_POSTMESSAGE 0x0008
#define QS_TIMER 0x0010
#define QS_PAINT 0x0020
#define QS_SENDMESSAGE 0x0040
#define QS_HOTKEY 0x0080
#define QS_RAWINPUT 0x0400
#define QS_MOUSE (QS_MOUSEMOVE | QS_MOUSEBUTTON)
#define QS_INPUT (QS_MOUSE | QS_KEY | QS_RAWINPUT)
#define QS_ALLINPUT (QS_INPUT | QS_POSTMESSAGE | QS_TIMER | QS_PAINT | QS_HOTKEY | QS_SENDMESSAGE)
#define QS_ALLEVENTS (QS_INPUT | QS_POSTMESSAGE | QS_TIMER | QS_PAINT | QS_HOTKEY)
#define WM_USER 0x0400                                                    // Define private messages
#define WM_APP 0x8000                                                     // Define private messages
#define OCM__BASE (WM_USER + 0x1c00)                                      // Define private messages for use by private window classes
#define PM_NOREMOVE 0x0000                                                // Messages are not removed from queue
#define PM_REMOVE 0x0001                                                  // Messages are removed from queue
#define PM_NOYIELD 0x0002                                                 // Prevent system from releasing thread waiting for caller to idle
#define PM_QS_INPUT (QS_INPUT << 16)                                      // Mouse and keyboard messages
#define PM_QS_PAINT (QS_PAINT << 16)                                      // Paint messages
#define PM_QS_POSTMESSAGE ((QS_POSTMESSAGE | QS_HOTKEY | QS_TIMER) << 16) // Posted messages
#define PM_QS_SENDMESSAGE (QS_SENDMESSAGE << 16)                          // Sent messages

    // Info about window that denied request from BroadcastSystemMessageEx
    typedef struct
    {
        UINT cbSize;
        HDESK hdesk;
        HWND hwnd;
        LUID luid;
    } BSMINFO, *PBSMINFO;

    // Message info from thread message queue
    typedef struct tagMSG
    {
        HWND hwnd;
        UINT message;
        WPARAM wParam;
        LPARAM lParam;
        DWORD time;
        POINT pt;
        DWORD lPrivate;
    } MSG, *PMSG, *NPMSG, *LPMSG;

    typedef void(CALLBACK *SENDASYNCPROC)(HWND, UINT, ULONG_PTR, LRESULT); // User defined callback to pass messages to a callback function after passing them to window proc
#ifdef UNICODE
    long WINAPI BroadcastSystemMessageW(DWORD flags, LPDWORD lpInfo, UINT Msg, WPARAM wParam, LPARAM lParam);                      // Send message to window recipients
    long WINAPI BroadcastSystemMessageExW(DWORD flags, LPDWORD lpInfo, UINT Msg, WPARAM wParam, LPARAM lParam, PBSMINFO pbsmInfo); // Send message to window recipients
    LRESULT WINAPI DispatchMessageW(const MSG *lpMsg);                                                                             // Dispatch message to window proc
#define BroadcastSystemMessage BroadcastSystemMessageW
#define BroadcastSystemMessageEx BroadcastSystemMessageExW
#define DispatchMessage DispatchMessageW
#else
    long WINAPI BroadcastSystemMessageA(DWORD flags, LPDWORD lpInfo, UINT Msg, WPARAM wParam, LPARAM lParam);                                  // Send message to window recipients
    long WINAPI BroadcastSystemMessageExA(DWORD flags, LPDWORD lpInfo, UINT Msg, WPARAM wParam, LPARAM lParam, PBSMINFO pbsmInfo);             // Send message to window recipients
    LRESULT WINAPI DispatchMessageA(const MSG *lpMsg);                                                                                         // Dispatch message to window proc
#define BroadcastSystemMessage BroadcastSystemMessageA
#define BroadcastSystemMessageEx BroadcastSystemMessageExA
#define DispatchMessage DispatchMessageW
#endif
    BOOL WINAPI GetInputState(); // Check if there are mouse button or keyboard messages in thread message queue
#ifdef UNICODE
    BOOL WINAPI GetMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax); // Get message from thread message queue
#define GetMessage GetMessageW
#else
    BOOL WINAPI GetMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);                                                   // Get message from thread message queue
#define GetMessage GetMessageA
#endif
    LPARAM WINAPI GetMessageExtraInfo();             // Get extra message info for current thread
    DWORD WINAPI GetMessagePos();                    // Get cursor position for last message
    LONG WINAPI GetMessageTime();                    // Get message time for last message
    DWORD WINAPI GetQueueStatus(UINT flags);         // Get type of messages in calling thread message queue
    BOOL WINAPI InSendMessage();                     // Check if current window proc is processing a message sent from another thread
    DWORD WINAPI InSendMessageEx(LPVOID lpReserved); // Check if current window proc is processing a message sent from another thread
#ifdef UNICODE
    BOOL WINAPI PeekMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg); // Dispatch any incoming nonqueued messages, check thread message for and retrieve
    BOOL WINAPI PostMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);                               // Place message in message queue of thread that created window
#define PeekMessage PeekMessageW
#define PostMessage PostMessageW
#else
    BOOL WINAPI PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);                                 // Dispatch any incoming nonqueued messages, check thread message for and retrieve
    BOOL WINAPI PostMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);                                                               // Place message in message queue of thread that created window
#define PeekMessage PeekMessageA
#define PostMessage PostMessageA
#endif
    void WINAPI PostQuitMessage(int nExitCode); // Indicate to system that thread has made request to quit
#ifdef UNICODE
    BOOL WINAPI PostThreadMessageW(DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam); // Post message to message queue of thread
    UINT WINAPI RegisterWindowMessageW(LPCWSTR lpString);                                   // Define new window message garunteed to be unique throughout the system
#define PostThreadMessage PostThreadMessageW
#define RegisterWindowMessage RegisterWindowMessageW
#else
    BOOL WINAPI PostThreadMessageA(DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam);                                                    // Post message to message queue of thread
    UINT WINAPI RegisterWindowMessageA(LPCSTR lpString);                                                                                       // Define new window message garunteed to be unique throughout the system
#define PostThreadMessage PostThreadMessageA
#define RegisterWindowMessage RegisterWindowMessageA
#endif
    BOOL WINAPI ReplyMessage(LRESULT lResult); // Replies to message sent from another thread
#ifdef UNICODE
    LRESULT WINAPI SendMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);                                                            // Send message to a window or windows
    BOOL WINAPI SendMessageCallbackW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, SENDASYNCPROC lpResultCallBack, ULONG_PTR dwData);     // Send message to a window or windows, and execute callback
    LRESULT WINAPI SendMessageTimeoutW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, UINT fuFlags, UINT uTimeout, PDWORD_PTR lpdwResult); // Send message to a window or windows
    BOOL WINAPI SendNotifyMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);                                                         // Send message to a window or windows
#define SendMessage SendMessageW
#define SendMessageCallback SendMessageCallbackW
#define SendMessageTimeout SendMessageTimeoutW
#define SendNotifyMessage SendNotifyMessageW
#else
    LRESULT WINAPI SendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);                                                            // Send message to a window or windows
    BOOL WINAPI SendMessageCallbackA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, SENDASYNCPROC lpResultCallBack, ULONG_PTR dwData);     // Send message to a window or windows, and execute callback
    LRESULT WINAPI SendMessageTimeoutA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, UINT fuFlags, UINT uTimeout, PDWORD_PTR lpdwResult); // Send message to a window or windows
    BOOL WINAPI SendNotifyMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);                                                         // Send message to a window or windows
#define SendMessage SendMessageA
#define SendMessageCallback SendMessageCallbackA
#define SendMessageTimeout SendMessageTimeoutA
#define SendNotifyMessage SendNotifyMessageA
#endif
    LPARAM WINAPI SendMessageExtraInfo(LPARAM lParam); // Set extra message info for current thread
    BOOL WINAPI TranslateMessage(const MSG *lpMsg);    // Translate virtual key messages into character messages
    BOOL WINAPI WaitMessage();                         // Yield control to other thread when thread has no messages in queue

#endif // NO_WINDOW_MESSAGES

    // TIMERS

#ifndef NO_WINDOW_TIMERS

#define WM_TIMER 0x0113 // Timer expired

    typedef void(CALLBACK *TIMERPROC)(HWND, UINT, UINT_PTR, DWORD);                                                                // User defined callback to process WM_TIMER messages
    BOOL WINAPI KillTimer(HWND hWnd, UINT_PTR uIDEvent);                                                                           // Destroy specified timer
    UINT_PTR WINAPI SetCoalescableTimer(HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc, ULONG uToleranceDelay); // Create timer with time out value and coalescing tolerance delay
    UINT_PTR WINAPI SetTimer(HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);                                   // Create timer with time out value

#endif // NO_WINDOW_TIMERS

    // WINDOW PROPERTIES

#ifndef NO_WINDOW_PROPERTIES

#ifdef UNICODE
    typedef BOOL(CALLBACK *PROPENUMPROCW)(HWND, LPCWSTR, HANDLE);                  // User defined callback used to enumerate properties
    typedef BOOL(CALLBACK *PROPENUMPROCEXW)(HWND, LPWSTR, HANDLE, ULONG_PTR);      // User defined callback used to enumerate properties
    int WINAPI EnumPropsW(HWND hWnd, PROPENUMPROCW lpEnumFunc);                    // Enumerate all entries in property list of window
    int WINAPI EnumPropsExW(HWND hWnd, PROPENUMPROCEXW lpEnumFunc, LPARAM lParam); // Enumerate all entries in property list of window
    HANDLE WINAPI GetPropW(HWND hWnd, LPCWSTR lpString);                           // Get data handle from property list of window
    HANDLE WINAPI RemovePropW(HWND hWnd, LPCWSTR lpString);                        // Remove entry from property list of window
    BOOL WINAPI SetPropW(HWND hWnd, LPCWSTR lpString, HANDLE hData);               // Add or change entry in property list of window
    typedef PROPENUMPROCW PROPENUMPROC;
    typedef PROPENUMPROCEXW PROPENUMPROCEX;
#define EnumProps EnumPropsW
#define EnumPropsEx EnumPropsExW
#define GetProp GetPropW
#define RemoveProp RemovePropW
#define SetProp SetPropW
#else
    typedef BOOL(CALLBACK *PROPENUMPROCA)(HWND, LPCSTR, HANDLE);                                                                               // User defined callback used to enumerate properties
    typedef BOOL(CALLBACK *PROPENUMPROCEXA)(HWND, LPSTR, HANDLE, ULONG_PTR);                                                                   // User defined callback used to enumerate properties
    int WINAPI EnumPropsA(HWND hWnd, PROPENUMPROCA lpEnumFunc);                                                                                // Enumerate all entries in property list of window
    int WINAPI EnumPropsExA(HWND hWnd, PROPENUMPROCEXA lpEnumFunc, LPARAM lParam);                                                             // Enumerate all entries in property list of window
    HANDLE WINAPI GetPropA(HWND hWnd, LPCSTR lpString);                                                                                        // Get data handle from property list of window
    HANDLE WINAPI RemovePropA(HWND hWnd, LPCSTR lpString);                                                                                     // Remove entry from property list of window
    BOOL WINAPI SetPropA(HWND hWnd, LPCSTR lpString, HANDLE hData);                                                                            // Add or change entry in property list of window
    typedef PROPENUMPROCA PROPENUMPROC;
    typedef PROPENUMPROCEXA PROPENUMPROCEX;
#define EnumProps EnumPropsA
#define EnumPropsEx EnumPropsExA
#define GetProp GetPropA
#define RemoveProp RemovePropA
#define SetProp SetPropA
#endif

#endif // NO_WINDOW_PROPERTIES

    // MULTIPLE DISPLAY MONITORS

#ifndef NO_MULTIPLE_DISPLAY_MONITORS

#define CCHDEVICENAME 32

    // Contain info about a display monitor
    typedef struct tagMONITORINFO
    {
        DWORD cbSize;
        RECT rcMonitor;
        RECT rcWork;
        DWORD dwFlags;
    } MONITORINFO, *LPMONITORINFO;

// Contain info about a display monitor, along with name for the monitor
#ifdef UNICODE
    typedef struct tagMONITORINFOEXW : public tagMONITORINFO
    {
        WCHAR szDevice[CCHDEVICENAME];
    } MONITORINFOEXW, *LPMONITORINFOEXW;
    typedef MONITORINFOEXW MONITORINFOEX;
    typedef LPMONITORINFOEXW LPMONITORINFOEX;
#else
    typedef struct tagMONITORINFOEXA : public tagMONITORINFO
    {
        CHAR szDevice[CCHDEVICENAME];
    } MONITORINFOEXA, *LPMONITORINFOEXA;
    typedef MONITORINFOEXA MONITORINFOEX;
    typedef LPMONITORINFOEXA LPMONITORINFOEX;
#endif

#ifdef UNICODE
#define GetMonitorInfo GetMonitorInfoW
#else
#define GetMonitorInfo GetMonitorInfoA
#endif

    typedef BOOL(CALLBACK *MONITORENUMPROC)(HMONITOR, HDC, LPRECT, LPARAM);                              // Application defined callback called by EnumDisplayMonitors
    BOOL WINAPI EnumDisplayMonitors(HDC hdc, LPCRECT lprcClip, MONITORENUMPROC lpfnEnum, LPARAM dwData); // Enumerates display monitor that intersect a region formed by intersection of specified clipping rect and visible region of device context
    BOOL WINAPI GetMonitorInfo(HMONITOR hMonitor, LPMONITORINFO lpmi);                                   // Get info about a display monitor
    HMONITOR WINAPI MonitorFromPoint(POINT pt, DWORD dwFlags);                                           // Get handle to display monitor that contains specified point
    HMONITOR WINAPI MonitorFromRect(LPCRECT lprc, DWORD dwFlags);                                        // Get handle to display monitor that has largest area of intersection with specified rect
    HMONITOR WINAPI MonitorFromWindow(HWND hwnd, DWORD dwFlags);                                         // Get handle to display monitor that has largest area of intersection with bounding rect of specified window

#endif // NO_MULTIPLE_DISPLAY_MONITORS

    // HIGH DPI AWARENESS

#ifndef NO_HIGH_DPI_AWARENESS

#define WM_DPICHANGED 0x02E0              // Notifies top level window that its DPI has changed
#define WM_DPICHANGED_BEFOREPARENT 0x02E2 // Notifies child window that DPI of containing window has changed (before parent window is notified)
#define WM_DPICHANGED_AFTERPARENT 0x02E3  // Notifies child window that DPI of containing window has changed (after parent window is notified)
#define WM_GETDPISCALEDSIZE 0x02E4        // Allows top level windows to resize non-linearly in response to DPI changes

    // Tokens that represent DPI virtualization mode and associated behaviors
    DECLARE_HANDLE(DPI_AWARENESS_CONTEXT);

#define DPI_AWARENESS_CONTEXT_UNAWARE ((DPI_AWARENESS_CONTEXT)-1)
#define DPI_AWARENESS_CONTEXT_SYSTEM_AWARE ((DPI_AWARENESS_CONTEXT)-2)
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE ((DPI_AWARENESS_CONTEXT)-3)
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 ((DPI_AWARENESS_CONTEXT)-4)
#define DPI_AWARENESS_CONTEXT_UNAWARE_GDISCALED ((DPI_AWARENESS_CONTEXT)-5)

    // DPI coordinate virtualization modes
    typedef enum DPI_AWARENESS
    {
        DPI_AWARENESS_INVALID = -1,
        DPI_AWARENESS_UNAWARE = 0,
        DPI_AWARENESS_SYSTEM_AWARE = 1,
        DPI_AWARENESS_PER_MONITOR_AWARE = 2
    } DPI_AWARENESS;

    // Per monitor DPI scaling behavior overrides for child windows within dialogs
    typedef enum DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS
    {
        DCDC_DEFAULT = 0x0000,
        DCDC_DISABLE_FONT_UPDATE = 0x0001,
        DCDC_DISABLE_RELAYOUT = 0x0002
    } DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS;

    // Per monitor DPI scaling behavior overrides for dialogs
    typedef enum DIALOG_DPI_CHANGE_BEHAVIORS
    {
        DDC_DEFAULT = 0x0000,
        DDC_DISABLE_ALL = 0x0001,
        DDC_DISABLE_RESIZE = 0x0002,
        DDC_DISABLE_CONTROL_RELAYOUT = 0x0004
    } DIALOG_DPI_CHANGE_BEHAVIORS;

    // Type of DPI associated with a monitor
    typedef enum MONITOR_DPI_TYPE
    {
        MDT_EFFECTIVE_DPI = 0,
        MDT_ANGULAR_DPI = 1,
        MDT_RAW_DPI = 2,
        MDT_DEFAULT
    } MONITOR_DPI_TYPE;

    // DPI coordinate virtualization mode of a process
    typedef enum PROCESS_DPI_AWARENESS
    {
        PROCESS_DPI_UNAWARE = 0,
        PROCESS_SYSTEM_DPI_AWARE = 1,
        PROCESS_PER_MONITOR_DPI_AWARE = 2
    } PROCESS_DPI_AWARENESS;

    // DPI hosting behavior for a window
    typedef enum DPI_HOSTING_BEHAVIOR
    {
        DPI_HOSTING_BEHAVIOR_INVALID = -1,
        DPI_HOSTING_BEHAVIOR_DEFAULT = 0,
        DPI_HOSTING_BEHAVIOR_MIXED = 1
    } DPI_HOSTING_BEHAVIOR;

    BOOL WINAPI AdjustWindowRectExForDpi(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle, UINT dpi);                                      // Calc required size of window rect based on desired size of client rect, adjusts values scaled to specific DPI
    BOOL WINAPI AreDpiAwarenessContextsEqual(DPI_AWARENESS_CONTEXT dpiContextA, DPI_AWARENESS_CONTEXT dpiContextB);                                 // Check if two DPI_AWARENESS_CONTEXT values are equivalent
    BOOL WINAPI EnableNonClientDpiScaling(HWND hwnd);                                                                                               // Enables automatic scaling of the non-client area of specified top level window
    DPI_AWARENESS WINAPI GetAwarenessFromDpiAwarenessContext(DPI_AWARENESS_CONTEXT value);                                                          // Get DPI_AWARENESS from a DPI_AWARENESS_CONTEXT
    HRESULT WINAPI GetDpiForMonitor(HMONITOR hmonitor, MONITOR_DPI_TYPE dpiType, UINT *dpiX, UINT *dpiY);                                           // Get DPI info associated with a monitor
    UINT WINAPI GetDpiForSystem();                                                                                                                  // Get system DPI
    UINT WINAPI GetDpiForWindow(HWND hwnd);                                                                                                         // Get current DPI from window
    HRESULT WINAPI GetProcessDpiAwareness(HANDLE hprocess, PROCESS_DPI_AWARENESS *value);                                                           // Get DPI virtualization mode of specified process
    int WINAPI GetSystemMetricsForDpi(int nIndex, UINT dpi);                                                                                        // Get specified system metric scaled to DPI
    DPI_AWARENESS_CONTEXT WINAPI GetWindowDpiAwarenessContext(HWND hwnd);                                                                           // Get DPI awareness context for a window
    BOOL WINAPI IsValidDpiAwarenessContext(DPI_AWARENESS_CONTEXT value);                                                                            // Check if DPI_AWARENESS_CONTEXT is valid and supported by current system
    BOOL WINAPI LogicalToPhysicalPointForPerMonitorDPI(HWND hWnd, LPPOINT lpPoint);                                                                 // Convert point in window from logical coords to physical coords, regarless of DPI awareness
    BOOL WINAPI PhysicalToLogicalPointForPerMonitorDPI(HWND hWnd, LPPOINT lpPoint);                                                                 // Convert point inf window from physical coords to logical coords regarless of DPI awareness
    HRESULT WINAPI SetProcessDpiAwareness(PROCESS_DPI_AWARENESS value);                                                                             // Set DPI virtualization mode for current process
    BOOL WINAPI SystemParametersInfoForDpi(UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni, UINT dpi);                                     // Get system wide parameters scaled to DPI
    BOOL WINAPI SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT value);                                                                         // Set DPI awareness context for current process
    BOOL WINAPI SetDialogDpiChangeBehavior(HWND hDlg, DIALOG_DPI_CHANGE_BEHAVIORS mask, DIALOG_DPI_CHANGE_BEHAVIORS values);                        // Override default per-monitor DPI scalring of a dialog
    DIALOG_DPI_CHANGE_BEHAVIORS WINAPI GetDialogDpiChangeBehavior(HWND hDlg);                                                                       // Get per-monitor DPI scaling of a dialog
    BOOL WINAPI SetDialogControlDpiChangeBehavior(HWND hWnd, DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS mask, DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS values); // Override default per-monitor DPI scaling of a child window in dialog
    DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS WINAPI GetDialogControlDpiChangeBehavior(HWND hWnd);                                                        // Get per-monitor DPI scaling overrides of child window in dialog
    UINT WINAPI GetSystemDpiForProcess(HANDLE hProcess);                                                                                            // Get system DPI associated with a process
    UINT WINAPI GetDpiFromDpiAwarenessContext(DPI_AWARENESS_CONTEXT value);                                                                         // Get DPI from a given DPI_AWARENESS_CONTEXT
#ifdef _WIN32_THREADS
    DPI_AWARENESS_CONTEXT WINAPI SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT dpiContext); // Change active API awareness context for current thread
    DPI_AWARENESS_CONTEXT WINAPI GetThreadDpiAwarenessContext();                                 // Get active DPI awareness context for current thread
    DPI_HOSTING_BEHAVIOR WINAPI SetThreadDpiHostingBehavior(DPI_HOSTING_BEHAVIOR value);         // Override default DPI hosting behavior of current thread
    DPI_HOSTING_BEHAVIOR WINAPI GetThreadDpiHostingBehavior();                                   // Get DPI hosting behavior of current thread
#endif
    DPI_HOSTING_BEHAVIOR WINAPI GetWindowDpiHostingBehavior(HWND hwnd); // Get DPI hosting behavior of specified window

#endif // NO_HIGH_DPI_AWARENESS

    // CONFIGURATION

#ifndef NO_WINDOWS_CONFIGURATION

    int WINAPI GetSystemMetrics(int nIndex); // Get system metric or system config setting
#ifdef UNICODE
    BOOL WINAPI SystemParametersInfoW(UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni); // Get or set value of system wide parameters
#define SystemParametersInfo SystemParametersInfoW
#else
    BOOL WINAPI SystemParametersInfoA(UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni); // Get or set value of system wide parameters
#define SystemParametersInfo SystemParametersInfoA
#endif
#ifdef UNICODE
    HMODULE WINAPI GetModuleHandleW(LPCWSTR lpModuleName);
#define GetModuleHandle GetModuleHandleW
#else
    HMODULE WINAPI GetModuleHandleA(LPCSTR lpModuleName);
#define GetModuleHandle GetModuleHandleA
#endif

#endif // NO_WINDOWS_CONFIGURATION

    // IO COMPLETION PORT

#ifndef NO_IO_COMPLETION_PORT

    typedef struct _OVERLAPPED
    {
        ULONG_PTR Internal;
        ULONG_PTR InternalHigh;
        union
        {
            struct
            {
                DWORD Offset;
                DWORD OffsetHigh;
            } DUMMYSTRUCTNAME;
            PVOID Pointer;
        } DUMMYUNIONNAME;
        HANDLE hEvent;
    } OVERLAPPED, *LPOVERLAPPED;

    typedef unsigned int(__stdcall *_beginthreadex_proc_type)(void *);

    HANDLE WINAPI CreateIoCompletionPort(HANDLE fileHandle, HANDLE existingCompletionPort, ULONG_PTR completionKey, DWORD numberOfConcurrentThreads);
    BOOL WINAPI GetQueuedCompletionStatus(HANDLE completionPort, LPDWORD numberOfBytesTransferred, PULONG_PTR completionKey, LPOVERLAPPED *overlapped, DWORD milliseconds);
    BOOL WINAPI PostQueuedCompletionStatus(HANDLE completionPort, DWORD numberOfBytesTransferred, ULONG_PTR completionKey, LPOVERLAPPED overlapped);
    BOOL WINAPI CloseHandle(HANDLE object);

#endif // NO_IO_COMPLETION_PORT

    // THREADS

#ifndef NO_THREADS

    typedef struct _PROC_THREAD_ATTRIBUTE_LIST *PPROC_THREAD_ATTRIBUTE_LIST, *LPPROC_THREAD_ATTRIBUTE_LIST;
    typedef struct _PROCESSOR_NUMBER
    {
        WORD Group;
        BYTE Number;
        BYTE Reserved;
    } PROCESSOR_NUMBER, *PPROCESSOR_NUMBER;

    typedef struct _GROUP_AFFINITY
    {
        KAFFINITY Mask;
        WORD Group;
        WORD Reserved[3];
    } GROUP_AFFINITY, *PGROUP_AFFINITY;

    typedef struct _FILETIME
    {
        DWORD dwLowDateTime;
        DWORD dwHighDateTime;
    } FILETIME, *PFILETIME, *LPFILETIME;

    typedef enum _THREAD_INFORMATION_CLASS
    {
        ThreadMemoryPriority,
        ThreadAbsoluteCpuPriority,
        ThreadDynamicCodePolicy,
        ThreadPowerThrottling,
        ThreadInformationClassMax
    } THREAD_INFORMATION_CLASS;

    typedef DWORD(CALLBACK *ThreadProc)(LPVOID parameter);
    BOOL WINAPI AttachThreadInput(DWORD idAttach, DWORD idAttachTo, BOOL attach);
    HANDLE WINAPI CreateRemoteThread(HANDLE process, LPSECURITY_ATTRIBUTES threadAttributes, SIZE_T stackSize, ThreadProc startAddress, LPVOID parameter, DWORD creationFlags, LPDWORD threadId);
    HANDLE WINAPI CreateRemoteThreadEx(HANDLE process, LPSECURITY_ATTRIBUTES threadAttributes, SIZE_T stackSize, ThreadProc startAddress, LPVOID parameter, DWORD creationFlags, LPPROC_THREAD_ATTRIBUTE_LIST attributeList, LPDWORD threadId);
    HANDLE WINAPI CreateThread(LPSECURITY_ATTRIBUTES threadAttributes, SIZE_T stackSize, ThreadProc startAddress, LPVOID parameter, DWORD creationFlags, LPDWORD threadId);
    void WINAPI ExitThread(DWORD exitCode);
    HANDLE WINAPI GetCurrentThread();
    DWORD WINAPI GetCurrentThreadId();
    BOOL WINAPI GetExitCodeThread(HANDLE thread, LPDWORD exitCode);
    DWORD WINAPI GetProcessIfOfThread(HANDLE thread);
    HRESULT WINAPI GetThreadDescription(HANDLE thread, PWSTR *threadDescription);
    BOOL WINAPI GetThreadGroupAffinity(HANDLE thread, PGROUP_AFFINITY groupAffinity);
    DWORD WINAPI GetThreadId(HANDLE thread);
    BOOL WINAPI GetThreadIdealProcessorEx(HANDLE thread, PPROCESSOR_NUMBER idealProcessor);
    BOOL WINAPI GetThreadInformation(HANDLE thread, THREAD_INFORMATION_CLASS threadInformationClass, LPVOID threadInformation, DWORD threadInformationSize);
    BOOL WINAPI GetThreadIOPendingFlag(HANDLE thread, PBOOL ioPending);
    int WINAPI GetThreadPriority(HANDLE thread);
    BOOL WINAPI GetThreadPriorityBoost(HANDLE thread, PBOOL disablePriorityBoost);
    BOOL WINAPI GetThreadTimes(HANDLE thread, LPFILETIME creationTime, LPFILETIME exitTime, LPFILETIME kernelTime, LPFILETIME userTime);
    HANDLE WINAPI OpenThread(DWORD desiredAccess, BOOL inheritHandle, DWORD threadId);
    BOOL WINAPI QueryIdleProcessorCycleTime(PULONG bufferLength, PULONG64 processorIdleCycleTime);
    BOOL WINAPI QueryThreadCycleTime(HANDLE threadHandle, PULONG64 cycleTime);
    DWORD WINAPI ResumeThread(HANDLE thread);
    DWORD_PTR WINAPI SetThreadAffinityMask(HANDLE thread, DWORD_PTR threadAffinityMask);
    HRESULT WINAPI SetThreadDescription(HANDLE thread, PCWSTR threadDescription);
    BOOL WINAPI SetThreadGroupAffinity(HANDLE thread, const GROUP_AFFINITY *groupAffinity, PGROUP_AFFINITY previousGroupAffinity);
    DWORD WINAPI SetThreadIdealProcessor(HANDLE thread, DWORD idealProcessor);
    BOOL WINAPI SetThreadIdealProcessorEx(HANDLE thread, PPROCESSOR_NUMBER idealProcessor, PPROCESSOR_NUMBER previousIdealProcessor);
    BOOL WINAPI SetThreadInformation(HANDLE thread, THREAD_INFORMATION_CLASS threadInformationClass, LPVOID threadInformation, DWORD threadInformationSize);
    BOOL WINAPI SetThreadPriority(HANDLE thread, int priority);
    BOOL WINAPI SetThreadPriorityBoost(HANDLE thread, BOOL disablePriorityBoost);
    BOOL WINAPI SetThreadStackGaruntee(PULONG stackSizeInBytes);
    void WINAPI Sleep(DWORD milliseconds);
    DWORD WINAPI SleepEx(DWORD milliseconds, BOOL altertable);
    DWORD WINAPI SuspendThread(HANDLE thread);
    BOOL WINAPI SwitchToThread();
    BOOL WINAPI TerminateThread(HANDLE thread, DWORD exitCode);
    DWORD WINAPI TlsAlloc();
    BOOL WINAPI TlsFree(DWORD tlsIndex);
    LPVOID WINAPI TlsGetValue(DWORD tlsIndex);
    BOOL WINAPI TlsSetValue(DWORD tlsIndex, LPVOID tlsValue);
    DWORD WINAPI WaitForInputIdle(HANDLE process, DWORD milliseconds);

#endif // NO_THREADS

#ifndef NO_SYNC

#define HasOverlappedIoCompleted(lpOverlapped) (((DWORD)(lpOverlapped)->Internal) != STATUS_PENDING)

#pragma intrinsic(__faststorefence)
#pragma intrinsic(_mm_pause)
#pragma intrinsic(_mm_lfence)
#pragma intrinsic(_mm_prefetch)
#pragma intrinsic(_mm_mfence)
#pragma intrinsic(_mm_sfence)
#pragma intrinsic(_m_prefetchw)
#define MemoryBarrier __faststorefence
#define PreFetchCacheLine(l, a) _mm_prefetch((CHAR CONST *)a, l)
#define YieldProcessor _mm_pause

    typedef struct _SLIST_ENTRY
    {
        struct _SLIST_ENTRY *Next;
    } SLIST_ENTRY, *PSLIST_ENTRY;

#endif // NO_SYNC

#ifndef NO_HEAP

    ALLOCATE void *WINAPI HeapAlloc(void *heap, unsigned long flags, unsigned __int64 bytes);
    int WINAPI HeapFree(void *heap, unsigned long flags, void *mem);
    ALLOCATE void *WINAPI HeapReAlloc(void *heap, unsigned long flags, void *mem, unsigned __int64 bytes);
    void *WINAPI GetProcessHeap();
    void WINAPI ExitProcess(unsigned int exitCode);

#endif // NO_HEAP
}

#pragma warning(pop)
/*
TODO:
https://learn.microsoft.com/en-us/windows/win32/apiindex/windows-api-list
- User Interface Elements
- Core Audio
*/