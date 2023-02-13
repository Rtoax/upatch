# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2022-2023 Rong Tao
#
# - Try to find capstone
# Once done this will define
#
#  CAPSTONE_FOUND - system has capstone
#  CAPSTONE_INCLUDE_DIRS - the capstone include directory
#  CAPSTONE_CAPSTONE_H - the capstone has capstone.h header

find_path(CAPSTONE_INCLUDE_DIRS
  NAMES
    capstone.h
  PATH_SUFFIXES
    capstone
  PATHS
    ENV CPATH)

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(capstone-devel "Please install the capstone-devel development package"
  CAPSTONE_INCLUDE_DIRS)

SET(CMAKE_REQUIRED_LIBRARIES capstone)
INCLUDE(CheckCSourceCompiles)
CHECK_C_SOURCE_COMPILES("
#include <capstone/capstone.h>
int main() {
	return 0;
}" CAPSTONE_CAPSTONE_H)
SET(CMAKE_REQUIRED_LIBRARIES)

mark_as_advanced(CAPSTONE_INCLUDE_DIRS CAPSTONE_CAPSTONE_H)

