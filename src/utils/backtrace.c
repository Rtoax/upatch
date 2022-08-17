// SPDX-License-Identifier: GPL-2.0-or-later
/* Copyright (C) 2022 Rong Tao */
#include <stdio.h>

#include "log.h"
#include "util.h"


#if defined(HAVE_LIBUNWIND_H)
void do_backtrace(void)
{
	unw_cursor_t    cursor;
	unw_context_t   context;

	unw_getcontext(&context);
	unw_init_local(&cursor, &context);

	while (unw_step(&cursor) > 0) {

		unw_word_t offset, pc;
		char fname[64];

		unw_get_reg(&cursor, UNW_REG_IP, &pc);

		fname[0] = '\0';
		(void)unw_get_proc_name(&cursor, fname, sizeof(fname), &offset);

		printf("0x%lx : (%s+0x%lx) [0x%lx]\n", pc, fname, offset, pc);
	}

	return;
}
#endif
