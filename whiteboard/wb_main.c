
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

#include "pub_tool_basics.h"
#include "pub_tool_tooliface.h"

#include <stdint.h>

static void wb_post_clo_init(void)
{
}

uint64_t blocks = 0;
static void add_one_block()
{
   blocks++;
}

static void on_fn_entry(Addr addr)
{
   DiEpoch de = VG_(current_DiEpoch)();
   HChar *filename;
   HChar *dirname;
   HChar *fnname;
   UInt linenum;

   Bool r1 = VG_(get_fnname)(de, addr, &fnname);
   // Bool r2 = VG_(get_filename_linenum)(de, addr, &filename, &dirname, &linenum);
   // if (r1 && r2)
   //    VG_(printf)
   // ("Function call. fn=%s, file=%s, dir=%s, line=%d", fnname, filename, dirname, linenum);
   if (r1)
   {
      VG_(printf)
      ("Function entry. fn=%s, ", fnname);
      Vg_FnNameKind nameKind = VG_(get_fnname_kind)(fnname);
      switch (nameKind)
      {
      case Vg_FnNameNormal:
         VG_(printf)
         (" normal\n");
         break;
      case Vg_FnNameMain:
         VG_(printf)
         (" main\n");
         break;
      case Vg_FnNameBelowMain:
         VG_(printf)
         (" below main\n");
         break;
      }
   }
   else
   {
      VG_(printf)
      ("Function entry. addr=%p\n", addr);
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
   int i;
   IRDirty *di;
   DiEpoch ep = VG_(current_DiEpoch)();

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
      di = unsafeIRDirty_0_N(0, "add_one_block",
                             VG_(fnptr_to_fnentry)(&add_one_block),
                             mkIRExprVec_0());
      addStmtToIRSB(sbOut, IRStmt_Dirty(di));

      // detect fun entry
      if (st->tag == Ist_IMark)
      {
         const HChar *fnname;
         const Addr addr = st->Ist.IMark.addr;
         if (VG_(get_fnname_if_entry)(ep, addr,
                                      &fnname))
         {
            IRExpr **argv;
            argv = mkIRExprVec_1(mkIRExpr_HWord(addr));
            di = unsafeIRDirty_0_N(1, "on_fn_entry",
                                   VG_(fnptr_to_fnentry)(&on_fn_entry),
                                   argv);
            addStmtToIRSB(sbOut, IRStmt_Dirty(di));
         }
      }

      addStmtToIRSB(sbOut, st);
   }
   return sbOut;
}

static void wb_fini(Int exitcode)
{
   VG_(printf)
   ("wb_fini. blocks=%llu\n", blocks);
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

   /* No needs, no core events to track */
}

VG_DETERMINE_INTERFACE_VERSION(wb_pre_clo_init)

/*--------------------------------------------------------------------*/
/*--- end                                                          ---*/
/*--------------------------------------------------------------------*/
