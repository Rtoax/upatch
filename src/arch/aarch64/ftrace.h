// SPDX-License-Identifier: GPL-2.0-or-later
/* Copyright (C) 2022-2024 Rong Tao <rtoax@foxmail.com> */
#include <stdint.h>
#include <stdbool.h>


struct task;

int ftrace_modify_code(struct task *task, unsigned long pc, uint32_t old,
		uint32_t new, bool validate);
