// SPDX-License-Identifier: GPL-2.0-or-later
/* Copyright (C) 2022-2023 CESTC, Co. Rong Tao <rongtao@cestc.cn> */
#include <stdio.h>
#include <unistd.h>


void print_hello(void)
{
	printf("Hello World.\n");
}

int main(int argc, char *argv[])
{
	while (1) {
		print_hello();
		sleep(1);
	}

	return 0;
}