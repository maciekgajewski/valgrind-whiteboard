#include "wb_malloc.h"

#include "wb_state.h"

#include "pub_tool_replacemalloc.h"
#include "pub_tool_tooliface.h"

void *wb_malloc(ThreadId tid, SizeT n)
{
    void *addr = VG_(cli_malloc)(VG_(clo_alignment), n);

    if (tid == wb_main_tid)
    {
        VG_(printf)
        ("malloc, type=malloc, size=%d, addr=%p\n", n, addr);
    }

    return addr;
}

void *wb___builtin_new(ThreadId tid, SizeT n)
{
    void *addr = VG_(cli_malloc)(VG_(clo_alignment), n);

    if (tid == wb_main_tid)
    {
        VG_(printf)
        ("malloc, type=new, size=%d, addr=%p\n", n, addr);
    }

    return addr;
}

void *wb___builtin_vec_new(ThreadId tid, SizeT n)
{
    void *addr = VG_(cli_malloc)(VG_(clo_alignment), n);

    if (tid == wb_main_tid)
    {
        VG_(printf)
        ("malloc, type=new[], size=%d, addr=%p\n", n, addr);
    }

    return addr;
}

void *wb_memalign(ThreadId tid, SizeT align, SizeT n)
{
    void *addr = VG_(cli_malloc)(align, n);

    if (tid == wb_main_tid)
    {
        VG_(printf)
        ("malloc, type=memalign, size=%d, addr=%p, align=%d\n", n, addr, align);
    }

    return addr;
}

void *wb_calloc(ThreadId tid, SizeT nmemb, SizeT size1)
{
    VG_(printf)
    ("calloc\n");
    return NULL;
}

void wb_free(ThreadId tid, void *p)
{
    VG_(cli_free)
    (p);
    if (tid == wb_main_tid)
    {
        VG_(printf)
        ("free, type=free, addr=%p\n", p);
    }
}

void wb___builtin_delete(ThreadId tid, void *p)
{
    VG_(cli_free)
    (p);
    if (tid == wb_main_tid)
    {
        VG_(printf)
        ("free, type=delete, addr=%p\n", p);
    }
}

void wb___builtin_vec_delete(ThreadId tid, void *p)
{
    VG_(cli_free)
    (p);
    if (tid == wb_main_tid)
    {
        VG_(printf)
        ("free, type=delete[], addr=%p\n", p);
    }
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
