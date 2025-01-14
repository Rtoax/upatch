
## Abbreviations

- PIE: Position-Independent-Executable


## Introductions

**How to resolve symbol addresses?**

GDB's implementation of symbol parsing, [binutils-gdb](https://sourceware.org/git/binutils-gdb) is helpful, we should use `BFD` for resolve symbols and relocations.


## Linux Kernel ELF File Map

See kernel `load_elf_binary()` function, it will load all `PT_LOAD` section to memory, the location is what we care about.

```
load_bias = 0
vaddr = elf_ppnt->p_vaddr
if (ET_EXEC)
elif (ET_DYN)
	load_bias = Non-Zero Value (random)

elf_map(file, load_bias + vaddr, ...) {
	size = p_filesz + ELF_PAGEOFFSET(p_vaddr);
	off = p_offset - ELF_PAGEOFFSET(p_vaddr);
	addr = load_bias + p_vaddr
	addr = ELF_PAGESTART(addr);
	size = ELF_PAGEALIGN(size);
	vm_mmap(filep, addr, size, ..., off);
}
```


## Process's VMAs

In `/proc/PID/maps`, we could see the process's VMAs, kernel will load `PT_LOAD` into memory, and `linker`(for example `/lib64/ld-linux-x86-64.so.2` on `x86_64` fedora40) will seperate some vma. for example:

non-PIE hello's `PT_LOAD`

```
Program Headers:
  Type           Offset             VirtAddr           PhysAddr
                 FileSiz            MemSiz              Flags  Align
  LOAD           0x0000000000000000 0x0000000000400000 0x0000000000400000
                 0x0000000000000650 0x0000000000000650  R      0x1000
  LOAD           0x0000000000001000 0x0000000000401000 0x0000000000401000
                 0x0000000000000379 0x0000000000000379  R E    0x1000
  LOAD           0x0000000000002000 0x0000000000402000 0x0000000000402000
                 0x00000000000001d4 0x00000000000001d4  R      0x1000
  LOAD           0x0000000000002df8 0x0000000000403df8 0x0000000000403df8
                 0x0000000000000248 0x0000000000000260  RW     0x1000
```

we just start the `hello` with gdb, and `break` on linker's `_dl_start()`:

```
$ gdb ./hello
(gdb) b _dl_start
(gdb) r
Breakpoint 1, _dl_start (arg=0x7fffffffd830) at rtld.c:517
517	{
```

Then, check VMAs:

```
$ cat /proc/$(pidof hello)/maps
00400000-00401000 r--p 00000000 08:10 3115204 /ulpatch/tests/hello/hello
00401000-00402000 r-xp 00001000 08:10 3115204 /ulpatch/tests/hello/hello
00402000-00403000 r--p 00002000 08:10 3115204 /ulpatch/tests/hello/hello
00403000-00405000 rw-p 00002000 08:10 3115204 /ulpatch/tests/hello/hello
```

Then, `continue` run process:

```
(gdb) continue
```

Check VMAs again:

```
$ cat /proc/$(pidof hello)/maps
00400000-00401000 r--p 00000000 08:10 3115204 /ulpatch/tests/hello/hello
00401000-00402000 r-xp 00001000 08:10 3115204 /ulpatch/tests/hello/hello
00402000-00403000 r--p 00002000 08:10 3115204 /ulpatch/tests/hello/hello
00403000-00404000 r--p 00002000 08:10 3115204 /ulpatch/tests/hello/hello
00404000-00405000 rw-p 00003000 08:10 3115204 /ulpatch/tests/hello/hello
```

Why linker split vma `00403000-00405000 rw-p 00002000` to two different vmas `00403000-00404000 r--p 00002000` and `00404000-00405000 rw-p 00003000`? Let's see the linker's call stack in [glibc](https://sourceware.org/git/glibc) source code(my version `glibc-2.40.9000-13-g22958014ab`).

```
_dl_start() {
  _dl_start_final() {
    _dl_sysdep_start() {
      dl_main(dl_main_args.phdr, dl_main_args.phnum, ...) {
        _dl_relocate_object() {
          _dl_protect_relro() {
            phdr = PT_GNU_RELRO
            start = PAGE_DOWN(load_bias + phdr->p_vaddr);
            end = PAGE_DOWN(load_bias + phdr->p_vaddr + phdr->p_memsz);
            if (start != end) {
              mprotect(start, end - start, PROT_READ);
            }
          }
        }
      }
    }
  }
}
```

Let's see the PIE program.

```
555555554000-555555555000 r--p 00000000 08:10 3115207 /ulpatch/tests/hello/hello-pie
555555555000-555555556000 r-xp 00001000 08:10 3115207 /ulpatch/tests/hello/hello-pie
555555556000-555555557000 r--p 00002000 08:10 3115207 /ulpatch/tests/hello/hello-pie
555555557000-555555559000 rw-p 00002000 08:10 3115207 /ulpatch/tests/hello/hello-pie
```

Tracing `mprotect(2)`:

```
mprotect(0x555555557000, 0x4096, PROT_READ);
```

```
555555554000-555555555000 r--p 00000000 08:10 3115207 /ulpatch/tests/hello/hello-pie
555555555000-555555556000 r-xp 00001000 08:10 3115207 /ulpatch/tests/hello/hello-pie
555555556000-555555557000 r--p 00002000 08:10 3115207 /ulpatch/tests/hello/hello-pie
555555557000-555555558000 r--p 00002000 08:10 3115207 /ulpatch/tests/hello/hello-pie
555555558000-555555559000 rw-p 00003000 08:10 3115207 /ulpatch/tests/hello/hello-pie
```

We should know why linker modify `addr=0x555555557000,len=0x4096` memory to readonly.

As we can see in `readelf -l /bin/bash` output, the `.data.rel.ro` in the last `PT_LOAD` program header and `PT_GNU_RELRO` program header, kernel will load all `PT_LOAD` into memory, then, GNU Linker will set the `.data.rel.ro` to readonly permission by `mprotect(2)` syscall, see the linker pseudocode show above. Thus, the vma `555555557000-555555559000 rw-p 00002000` will splited to two different vma `555555557000-555555558000 r--p 00002000` and `555555558000-555555559000 rw-p 00003000`.


## Links

- https://reverseengineering.stackexchange.com/questions/16036/how-can-i-view-the-dynamic-symbol-table-of-a-running-process
- https://jvns.ca/blog/2018/01/09/resolving-symbol-addresses/
- [How gdb loads symbol files](https://sourceware.org/gdb/wiki/How%20gdb%20loads%20symbol%20files)
- GitHub: [bpftrace](https://github.com/bpftrace/bpftrace)
