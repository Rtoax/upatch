// SPDX-License-Identifier: GPL-2.0-or-later
/* Copyright (C) 2023 Rong Tao <rtoax@foxmail.com> */
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

#include <elf/elf_api.h>

#include <utils/log.h>
#include <utils/list.h>
#include <utils/compiler.h>
#include <utils/task.h>


struct config config = {
	.log_level = -1,
};

enum {
	ARG_PATCH = 139,
	ARG_LOG_LEVEL,
	ARG_LOG_DEBUG,
	ARG_LOG_ERR,
};

static const char *prog_name = "upinfo";

static void print_help(void)
{
	printf(
	"\n"
	" Usage: upinfo [OPTION]... [FILE]...\n"
	"\n"
	" User space patch\n"
	"\n"
	" Mandatory arguments to long options are mandatory for short options too.\n"
	"\n"
	" Option argument:\n"
	"\n"
	"  --patch             specify an patch file to check\n"
	"\n");
	printf(
	" Common argument:\n"
	"\n"
	"  --log-level         set log level, default(%d)\n"
	"                      EMERG(%d),ALERT(%d),CRIT(%d),ERR(%d),WARN(%d)\n"
	"                      NOTICE(%d),INFO(%d),DEBUG(%d)\n"
	"  --log-debug         set log level to DEBUG(%d)\n"
	"  --log-error         set log level to ERR(%d)\n"
	"\n",
	config.log_level,
	LOG_EMERG, LOG_ALERT, LOG_CRIT, LOG_ERR, LOG_WARNING, LOG_NOTICE, LOG_INFO,
	LOG_DEBUG,
	LOG_DEBUG,
	LOG_ERR);
	printf(
	"  -h, --help          display this help and exit\n"
	"  -v, --version       output version information and exit\n"
	"\n");
	printf(
	" upinfo %s\n",
	upatch_version()
	);
	exit(0);
}

static int parse_config(int argc, char *argv[])
{
	struct option options[] = {
		{ "patch",          no_argument,       0, ARG_PATCH },
		{ "version",        no_argument,       0, 'v' },
		{ "help",           no_argument,       0, 'h' },
		{ "log-level",      required_argument, 0, ARG_LOG_LEVEL },
		{ "log-debug",      no_argument,       0, ARG_LOG_DEBUG },
		{ "log-error",      no_argument,       0, ARG_LOG_ERR },
		{ NULL }
	};

	while (1) {
		int c;
		int option_index = 0;
		c = getopt_long(argc, argv, "vh", options, &option_index);
		if (c < 0) {
			break;
		}
		switch (c) {
		case ARG_PATCH:
			break;
		case 'v':
			printf("%s %s\n", prog_name, upatch_version());
			exit(0);
		case 'h':
			print_help();
			break;
		case ARG_LOG_LEVEL:
			config.log_level = atoi(optarg);
			break;
		case ARG_LOG_DEBUG:
			config.log_level = LOG_DEBUG;
			break;
		case ARG_LOG_ERR:
			config.log_level = LOG_ERR;
			break;
		default:
			print_help();
			break;
		}
	}

	return 0;
}

int main(int argc, char *argv[])
{
	parse_config(argc, argv);

	set_log_level(config.log_level);

	return 0;
}
