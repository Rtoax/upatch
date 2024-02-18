![ULPatch Logo](images/logo.png)

ULPatch
========

# Introduction

ULPatch is open source user space live patch tool.


## Background Introduction

For a process like Qemu that cannot be interrupted and restarted, vulnerability fixing is very difficult. Especially for cloud vendors, the live patch function is very important.

Hot patching in the kernel is already a relatively mature technology. Implementing [livepatch](https://docs.kernel.org/livepatch/livepatch.html) based on ftrace in the [linux kernel](https://github.com/torvalds/linux). Of course, the ULPatch project only discusses user-mode programs.


## Related Projects

ULPatch draws on several excellent open source projects, such as [cloudlinux/libcare](https://github.com/cloudlinux/libcare), and Huawei’s secondary development [openeuler/libcareplus](https://gitee.com/openeuler/libcareplus). SUSE has also open sourced its own hot patch solution [SUSE/libpulp](https://github.com/SUSE/libpulp).

At the same time, the implementation of the kernel's `finit_module(2)` and `init_module(2)` system calls is also of great reference value. Even in the early stages of development, the relocation code was transplanted from these two system calls.

Judging from the current research on outstanding projects, the hot patch function relies on modifying the assembly instructions at the function entrance to make it jump to a new function, thereby realizing the hot patch function.

I think I should detail the inspiration of ULPatch from these open source projects in another document instead of a README file.


# Support Architecture

- [ ] `x86_64`: ready to go
- [ ] `aarch64`: ready to go
- [ ] `loongarch64`: ready to go


# Installing

See [INSTALL.md](INSTALL.md) for installation steps on your platform.


# Theroy

## ULPatch

![ulpatch](docs/images/ulpatch.drawio.svg)


## Ftrace

Same as [linux](https://github.com/torvalds/linux) ftrace, need gcc `-pg` compile option.
`BUILD_ULFTRACE` decides whether to compile `ulftrace`.

