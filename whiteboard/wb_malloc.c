#include "wb_malloc.h"

#include "pub_tool_replacemalloc.h"
#include "pub_tool_tooliface.h"

void *wb_malloc(ThreadId tid, SizeT n)
{
    VG_(printf)
    ("malloc\n");
    return NULL;
}

void *wb___builtin_new(ThreadId tid, SizeT n)
{
    VG_(printf)
    ("__builtin_new\n");
    return NULL;
}

void *wb___builtin_vec_new(ThreadId tid, SizeT n)
{
    VG_(printf)
    ("builtin_vec_new\n");
    return NULL;
}

void *wb_memalign(ThreadId tid, SizeT align, SizeT n)
{
    VG_(printf)
    ("memalign\n");
    return NULL;
}

void *wb_calloc(ThreadId tid, SizeT nmemb, SizeT size1)
{
    VG_(printf)
    ("calloc\n");
    return NULL;
}

void wb_free(ThreadId tid, void *p)
{
    VG_(printf)
    ("free\n");
}

void wb___builtin_delete(ThreadId tid, void *p)
{
    VG_(printf)
    ("builtin_delete\n");
}

void wb___builtin_vec_delete(ThreadId tid, void *p)
{
    VG_(printf)
    ("__builtin_vec_delete\n");
}

void *wb_realloc(ThreadId tid, void *p, SizeT new_size)
{
    VG_(printf)
    ("realloc\n");
    return NULL;
}

SizeT wb_malloc_usable_size(ThreadId tid, void *p)
{
    return VG_(cli_malloc_usable_size)(p);
}
