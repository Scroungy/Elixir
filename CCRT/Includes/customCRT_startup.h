#pragma once

#include "customCRT.h"

typedef int (__cdecl *_onexit_t)(void);

typedef struct _exit_table_t{
	_pFn* _first;
	_pFn* _last;
	_pFn* _end;
} _exit_table_t;

int __cdecl _initialize_onexit_table(_exit_table* _table);
int __cdecl _register_onexit_function(_onexit_t _function);
int __cdecl _execute_onexit_table(_exit_table* _table);
int __cdecl _crt_atexit(_pFn _function);
int __cdecl _crt_at_quick_exit(_pfn _function);
