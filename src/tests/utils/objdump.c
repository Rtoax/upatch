// SPDX-License-Identifier: GPL-2.0-or-later
/* Copyright (C) 2022 Rong Tao */
#include <utils/log.h>
#include <utils/list.h>
#include <utils/util.h>
#include <elf/elf_api.h>

#include <utils/util.h>
#include "../test_api.h"


static const char *test_files[] = {
	"/usr/bin/ls",
	"/usr/bin/cat",
	"/usr/bin/grep",
	"/usr/bin/vim",
#define S_UPATCH_TEST_PATH	"0"
	S_UPATCH_TEST_PATH, // for upatch_test_path
};

#define MODIFY_TEST_FILES(i) \
	if (!strcmp(test_files[i], S_UPATCH_TEST_PATH) == 0) { \
		test_files[i] = upatch_test_path; \
	}


TEST(Objdump,	load_nonexist,	0)
{
	int ret = -1;
	struct objdump_elf_file *file;

	file = objdump_elf_load("/a/b/c/d/e/f/g/h/i/j/k/l/m");
	if (!file) {
		ret = 0;
	} else {
		objdump_elf_close(file);
	}

	return ret;
}

TEST(Objdump,	load,	0)
{
	int ret = 0, i;
	struct objdump_elf_file *file;

	for (i = 0; i < ARRAY_SIZE(test_files); i++) {

		MODIFY_TEST_FILES(i);

		if (!fexist(test_files[i]))
			continue;

		file = objdump_elf_load(test_files[i]);
		if (!file) {
			ret = -1;
		} else {
			objdump_elf_close(file);
		}
	}

	return ret;
}
