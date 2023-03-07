// SPDX-License-Identifier: GPL-2.0-or-later
/* Copyright (C) 2022-2023 Rong Tao */
#include <errno.h>

#include <utils/log.h>
#include <utils/list.h>
#include <utils/util.h>
#include <utils/task.h>
#include <elf/elf_api.h>
#include <patch/patch.h>

#include "../test_api.h"


/* see: root CMakeLists.txt */
static const struct upatch_object {
	enum patch_type type;
	char *path;
} upatch_objs[] = {
	/* /usr/share/upatch/ftrace-mcount.obj */
	{UPATCH_TYPE_FTRACE,	UPATCH_FTRACE_OBJ_PATH},
	/* /usr/share/upatch/upatch-hello.obj */
	{UPATCH_TYPE_PATCH,	UPATCH_HELLO_OBJ_PATH},
};


TEST(Object,	check_object,	0)
{
	int i, ret = 0;

	for (i = 0; i < ARRAY_SIZE(upatch_objs); i++) {

		struct load_info info = {};
		enum patch_type expect_type = upatch_objs[i].type;
		char *obj = upatch_objs[i].path;
		char *tmpfile = "copy.obj";

		if (!fexist(obj)) {
			ret = -EEXIST;
			fprintf(stderr, "\n%s is not exist, maybe: make install\n", obj);
		}
		ret = parse_load_info(obj, tmpfile, &info);
		if (ret) {
			fprintf(stderr, "Parse %s failed.\n", obj);
			return ret;
		}

		setup_load_info(&info);

		if (info.type != expect_type) {
			fprintf(stderr, "Unknow patch type %d(expect %d).\n",
				info.type, expect_type);
			return -1;
		}
	}

	return ret;
}
