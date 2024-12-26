#!/bin/bash
set -e

# Use half of CPU cores
np=$(( $(nproc) / 2 ))

without_capstone=
without_libunwind=
nodebuginfo=

__usage__() {
	echo -e "
--without-capstone  build without capstone
--without-libunwind build without libunwind

--nodebuginfo       skip debuginfo and debugsource packages
-h, --help          print this info
"
	exit ${1-0}
}

TEMP=$( getopt --options h \
	--long help \
	--long without-capstone \
	--long without-libunwind \
	--long nodebuginfo \
	--name rpmbuild-ulpatch -- "$@" )
test $? != 0 && __usage__ 1

eval set -- "${TEMP}"

while true ; do
	case $1 in
	-h | --help)
		shift
		__usage__
		;;
	--without-capstone)
		shift
		without_capstone=YES
		;;
	--without-libunwind)
		shift
		without_libunwind=YES
		;;
	--nodebuginfo)
		shift
		nodebuginfo=YES
		;;
	--)
		shift
		break
		;;
	esac
done

rpmbuild -ba \
	--define "_topdir $PWD" \
	--define "_sourcedir $PWD" \
	--define "_smp_mflags -j${np}" \
	${without_capstone:+--without capstone} \
	${without_libunwind:+--without libunwind} \
	${nodebuginfo:+--nodebuginfo} \
	ulpatch.spec
