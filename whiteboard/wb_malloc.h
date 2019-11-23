#pragma once

#include "pub_tool_basics.h"

#define WB_MALLOC_DEFAULT_REDZONE_SZB 16

void *wb_malloc(ThreadId tid, SizeT n);
void *wb___builtin_new(ThreadId tid, SizeT n);
void *wb___builtin_vec_new(ThreadId tid, SizeT n);
void *wb_memalign(ThreadId tid, SizeT align, SizeT n);
void *wb_calloc(ThreadId tid, SizeT nmemb, SizeT size1);
void wb_free(ThreadId tid, void *p);
void wb___builtin_delete(ThreadId tid, void *p);
void wb___builtin_vec_delete(ThreadId tid, void *p);
void *wb_realloc(ThreadId tid, void *p, SizeT new_size);
SizeT wb_malloc_usable_size(ThreadId tid, void *p);
