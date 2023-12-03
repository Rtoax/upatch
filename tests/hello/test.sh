#!/bin/bash
set -e

pid=$(pidof hello)

[[ -z ${pid} ]] && echo "ERROR: Run ./hello first" && exit 1

make

upatch -p ${pid} --patch patch.up
cat /proc/${pid}/maps

patches_addr_range=( $(cat /proc/${pid}/maps | grep patch- | awk '{print $1}') )

for ((i = 0; i < ${#patches_addr_range[@]}; i++))
do
	rm -f patch-$i.up
	utask -p ${pid} --dump-vma ${patches_addr_range[$i]} -o patch-$i.up
done
