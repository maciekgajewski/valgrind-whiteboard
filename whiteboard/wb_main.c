
/*--------------------------------------------------------------------*/
/*--- Whiteboard.               wb_main.c ---*/
/*--------------------------------------------------------------------*/

/*

   Copyright (C) 2017 Maciej Gajewski
      maciej.gajewski0@gmail.com
      
   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, see <http://www.gnu.org/licenses/>.

   The GNU General Public License is contained in the file COPYING.
*/

#include "wb_malloc.h"
#include "wb_state.h"

#include "pub_tool_basics.h"
#include "pub_tool_tooliface.h"

#include <stdint.h>

static void wb_post_clo_init(void)
{
}

static print_tag(IRStmtTag tag)
{
   switch (tag)
   {
   case Ist_IMark:
      VG_(printf)
      ("Ist_IMark");
      break;
   case Ist_AbiHint:
      VG_(printf)
      ("Ist_AbiHint");
      break;
   case Ist_Put:
      VG_(printf)
      ("Ist_Put");
      break;
   case Ist_NoOp:
      VG_(printf)
      ("Ist_NoOp");
      break;
   case Ist_PutI:
      VG_(printf)
      ("Ist_PutI");
      break;
   case Ist_WrTmp:
      VG_(printf)
      ("Ist_WrTmp");
      break;
   case Ist_Store:
      VG_(printf)
      ("Ist_Store");
      break;
   case Ist_LoadG:
      VG_(printf)
      ("Ist_LoadG");
      break;
   case Ist_StoreG:
      VG_(printf)
      ("Ist_StoreG");
      break;
   case Ist_CAS:
      VG_(printf)
      ("Ist_CAS");
      break;
   case Ist_LLSC:
      VG_(printf)
      ("Ist_LLSC");
      break;
   case Ist_Dirty:
      VG_(printf)
      ("Ist_Dirty");
      break;
   case Ist_MBE:
      VG_(printf)
      ("Ist_MBE");
      break;
   case Ist_Exit:
      VG_(printf)
      ("Ist_Exit");
      break;
   }
}

static void instrumentExpression(IRSB *sbOut, IRExpr *expr)
{
   ppIRExpr(expr);
   VG_(printf)
   (", tag=0x%x\n", expr->tag);
   if (expr->tag == Iex_Binop)
   {
      if (expr->Iex.Binop.op == Iop_Add32)
         VG_(printf)
      (">>> the above is Add32\n");
   }
}

static IRSB *wb_instrument(VgCallbackClosure *closure,
                           IRSB *sbIn,
                           const VexGuestLayout *layout,
                           const VexGuestExtents *vge,
                           const VexArchInfo *archinfo_host,
                           IRType gWordTy, IRType hWordTy)
{
   IRSB *sbOut;
   IRTypeEnv *tyenv = sbIn->tyenv;
   int i;
   IRDirty *di;
   DiEpoch ep = VG_(current_DiEpoch)();
   Bool instrument = 0;

   if (gWordTy != hWordTy)
   {
      /* We don't currently support this case. */
      VG_(tool_panic)
      ("host/guest word size mismatch");
   }

   /* Set up SB */
   sbOut = deepCopyIRSBExceptStmts(sbIn);

   // Copy verbatim any IR preamble preceding the first IMark
   i = 0;
   while (i < sbIn->stmts_used && sbIn->stmts[i]->tag != Ist_IMark)
   {
      addStmtToIRSB(sbOut, sbIn->stmts[i]);
      i++;
   }

   for (/*use current i*/; i < sbIn->stmts_used; i++)
   {
      IRStmt *st = sbIn->stmts[i];
      if (!st || st->tag == Ist_NoOp)
         continue;
      // detect fun entry
      if (st->tag == Ist_IMark)
      {
         const HChar *fnname;
         HChar *filename;
         HChar *dirname;
         UInt linenum;

         const Addr addr = st->Ist.IMark.addr;

         DiEpoch de = VG_(current_DiEpoch)();
         Bool r1 = VG_(get_fnname)(de, addr, &fnname);
         Bool r2 = VG_(get_filename_linenum)(de, addr, &filename, &dirname, &linenum);
         if (r1)
         {
            if (VG_(strlen)(fnname) >= 12 && VG_(strncmp)(fnname, "instrumentme", 12) == 0)
            {
               VG_(printf)
               ("instrumentme!\n");
               instrument = 1;
               if (r2)
               {
                  VG_(printf)
                  ("%s:%d\n", filename, linenum);
               }
               else
               {
                  VG_(printf)
                  ("dupa\n");
               }
            }
         }
      }

      // add original statement
      if (instrument)
      {
         VG_(printf)
         ("-------- tag=");
         print_tag(st->tag);
         VG_(printf)
         ("\n");
         ppIRStmt(st);
         VG_(printf)
         ("\n");

         // WrTmp
         if (st->tag == Ist_WrTmp)
         {
            VG_(printf)
            (">>> WrTmp statemement\n");
            instrumentExpression(sbOut, st->Ist.WrTmp.data);
         }
      }
      addStmtToIRSB(sbOut, st);
   }
   return sbOut;
}

static void wb_fini(Int exitcode)
{
}

static void wb_pre_clo_init(void)
{
   VG_(details_name)
   ("Whiteboard");
   VG_(details_version)
   (NULL);
   VG_(details_description)
   ("Whiteboard event log generator");
   VG_(details_copyright_author)
   (
       "Copyright (C) 2019, Maciej Gajewski.");
   VG_(details_bug_reports_to)
   (VG_BUGS_TO);

   VG_(details_avg_translation_sizeB)
   (275);

   VG_(basic_tool_funcs)
   (wb_post_clo_init,
    wb_instrument,
    wb_fini);

   VG_(needs_malloc_replacement)
   (wb_malloc,
    wb___builtin_new,
    wb___builtin_vec_new,
    wb_memalign,
    wb_calloc,
    wb_free,
    wb___builtin_delete,
    wb___builtin_vec_delete,
    wb_realloc,
    wb_malloc_usable_size,
    WB_MALLOC_DEFAULT_REDZONE_SZB);
}

VG_DETERMINE_INTERFACE_VERSION(wb_pre_clo_init)

/*--------------------------------------------------------------------*/
/*--- end                                                          ---*/
/*--------------------------------------------------------------------*/
