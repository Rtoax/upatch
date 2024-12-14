---
hide:
  - navigation
---

## 从源代码安装

### 安装依赖

#### Fedora/RHEL/AlmaLinux 操作系统

在 **RHEL** 系列兼容的发行版上，通过 [rpm](https://github.com/rpm-software-management) 和 [dnf/yum](https://github.com/rpm-software-management/dnf)管理软件。

```bash
# RHEL like distributions need epel-release, Fedora don't.
$ sudo dnf install -y epel-release
$ sudo dnf groupinstall -y "Development Tools" "Development Libraries"
$ sudo dnf install -y \
	binutils-devel \
	capstone-devel \
	cmake \
	elfutils-devel \
	elfutils-libelf-devel \
	gcc \
	gcc-c++ \
	glibc-devel \
	libunwind-devel
```


#### Debian/Ubuntu 操作系统

在 **Debian** 系列兼容的发行版上，通过 [apt](https://salsa.debian.org/apt-team/apt) 管理软件。

```bash
$ sudo apt install -y build-essential
$ sudo apt install -y \
	cmake \
	gcc \
	binutils-dev \
	libc6 \
	libcapstone-dev \
	libelf-dev \
	libunwind-dev
```

如果想要构建文档，需要安装：

```
$ sudo apt install -y mkdocs
```


### 源码编译软件

```bash
$ git clone https://github.com/rtoax/ulpatch
$ cd ulpatch
$ mkdir build
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX=/usr \
	-DCONFIG_BUILD_TESTING=OFF \
	-DCONFIG_BUILD_ULFTRACE=OFF \
	-DCONFIG_BUILD_ULTASK=OFF \
	..
$ make -j$(nproc)
```

或者你可以通过指定 `-B build` 参数，像 `cmake -B build -DCMAKE_BUILD_TYPE=Release`.


### CMake 相关宏

#### CMAKE_BUILD_TYPE

你可以通过`CMAKE_BUILD_TYPE`指定编译类型，例如 `-DCMAKE_BUILD_TYPE=Debug`(`Release`,`Debug`,`RelWithDebInfo`,`MinSizeRel`)，一个例子：

```
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
```

默认编译类型为 `Debug`。


#### CONFIG_BUILD_PIE_EXE

编译ULPatch可执行文件为PIE(`Position-Independent-Executable`)，可以执行：

```
$ cmake -DCONFIG_BUILD_PIE_EXE=1 ..
```

这个参数对编译PIE文件很有帮助。


#### CONFIG_BUILD_TESTING

通过指定 `CONFIG_BUILD_TESTING` 来决定是否编译测试程序 `ulpatch_test`，默认开启 `ON`。如果你想要关闭，可以：

```
$ cmake -DCONFIG_BUILD_TESTING=0 ..
```

#### CONFIG_BUILD_ULFTRACE

通过指定 `CONFIG_BUILD_ULFTRACE` 来决定是否编译 `ulftrace`，默认开启 `ON`。如果你想要关闭，可以：

```
$ cmake -DCONFIG_BUILD_ULFTRACE=0 ..
```

#### CONFIG_BUILD_ULTASK

通过指定 `CONFIG_BUILD_ULTASK` 来决定是否编译进程修改器`ultask`，默认关闭 `ON`。如果你想要关闭，可以：

```
$ cmake -DCONFIG_BUILD_ULTASK=0 ..
```

#### CONFIG_BUILD_MAN

通过指定 `CONFIG_BUILD_MAN` 来决定是否编译相关手册，默认开启 `ON`。如果你想要关闭，可以：

```
$ cmake -DCONFIG_BUILD_MAN=0 ..
```

#### CONFIG_CAPSTONE

CMake `CONFIG_CAPSTONE`选项决定是否支持capstone，默认开启 `ON`。如果你想要关闭，可以：

```
$ cmake -DCONFIG_CAPSTONE=OFF ..
```
如果 `CONFIG_CAPSTONE=ON`(default)，并且你的系统没有安装libunwind，`cmake`将报错。

#### CONFIG_LIBUNWIND

CMake `CONFIG_LIBUNWIND`选项决定是否支持 libunwind，默认开启 `ON`。如果你想要关闭，可以：

```
$ cmake -DCONFIG_CAPSTONE=OFF ..
```

如果 `CONFIG_LIBUNWIND=ON`(default)，并且你的系统没有安装libunwind，`cmake`将报错。


### 安装ULPatch

```bash
$ sudo make install
```

### 卸载ULPatch

```bash
$ sudo make uninstall
```


## 从RPM安装

### 直接下载

可以在 [发布记录](https://github.com/Rtoax/ulpatch/releases) 下载。

### 安装

然后，使用 `rpm` 或者 `dnf` 命令安装 RPM包。

```
$ sudo dnf install ulpatch-*.rpm
```

或者：

```
$ sudo rpm -ivh ulpatch-*.rpm
```

如果升级：

```
$ sudo rpm -iUh ulpatch-*.rpm
```

### 构建RPM包

> ULPatch当前只支持构建RPM包，你可以在 Fedora、RHEL 这些系统上构建。

你需要安装`rpm-build`软件包：

```
$ sudo dnf install rpm-build
```

然后，安装ULPatch构建所需要的依赖：

```
$ sudo dnf builddep ulpatch.spec
```

现在，你可以通过`rpmbuild`构建RPM包了。在ULPatch代码仓库的根目录下，我已经封装了`rpmbuild.sh`命令。当然，你需要先打包源代码。生成tar包：

```bash
$ ./archive.sh
```

构建RPM包：

```bash
$ ./rpmbuild.sh
```
