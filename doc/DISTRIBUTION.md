# HOW TO DISTRIBUTE

<!--toc:start-->
- [Create Dynamic Distribution Build for Linux](#create-dynamic-distribution-build-for-linux)
- [Create Static Distribution Build for Linux](#create-static-distribution-build-for-linux)
- [Create Dynamic Distribution Build for MacOS](#create-dynamic-distribution-build-for-macos)
- [Create Dynamic Distribution Build for Windows](#create-dynamic-distribution-build-for-windows)
- [Set Appropriate GLIBC Version for Linux Builds](#set-appropriate-glibc-version-for-linux-builds)
  - [OPTION 1: Build on Ubuntu 22.04](#option-1-build-on-ubuntu-2204)
  - [OPTION 2: Use glibc-polyfill](#option-2-use-glibc-polyfill)
<!--toc:end-->

The ABC system is distributed three ways:

- statically linked executable
- dynamically linked executable
- source code

Executables are provided for the following platforms

- x86 Linux 64-bit and 32-bit (tested on Ubuntu 22.04, 24.04, 26.04)
- ARM MacOS 64-bit as `.dmg` (tested on MacOS 14)
- x86 Windows 64-bit (tested on Windows 11)

## Create Dynamic Distribution Build for Linux

> [!NOTE]
>
> In order to make sure that a dynamically linked executable runs on less resent
> operating systems, you need to make sure to link against the oldest GLIBC
> version possible. Read the *Set Appropriate GLIBC Version for Linux* section.

1. Build the project in release mode. Unfortunately, we cannot turn on
   optimisations since it causes segfaults.

   ```sh
   make -j`nproc` EXTRA_CFLAGS="-s"
   ```

2. Go to the `src/` folder.

   ```sh
   cd src/
   ```

3. Strip the executables.

   ```sh
   strip abc abckeys
   ```

4. Package the binaries together with the necessary files as
   `abc-linux-x86_64-dynamic.tar.gz` and also with `tar.xz`.

   ```sh
   files="abc abckeys abc.1 abc.hlp abc.msg"
   tar -czf ../dist/abc-linux-x86_64-dynamic.tar.gz $files --transform 's,^,ABC/,'
   tar -cJf ../dist/abc-linux-x86_64-dynamic.tar.xz $files --transform 's,^,ABC/,'
   ```

5. You are done. Enjoy!

## Create Static Distribution Build for Linux

> [!NOTE]
>
> If you have a Windows system and are using WSL to create the distribution, you
> need to have the source code on the Linux file system, not on your Windows
> filesystem.

1. Build the project statically with in release mode

   ```sh
   make static EXTRA_CFLAGS="-s -Os"
   ```

   > [!NOTE]
   >
   > Make sure the run `make clean` if you already have build artifacts.

2. Go to the `src/` folder

   ```sh
   cd src/
   ```

3. Strip the executables

   ```sh
   strip abc abckeys
   ```

4. Package the binary with necessary files

   ```sh
   files="abc abckeys abc.1 abc.hlp abc.msg"
   tar -czf ../dist/abc-linux-static.tar.gz $files --transform 's,^,ABC/,'
   tar -cJf ../dist/abc-linux-static.tar.xz $files --transform 's,^,ABC/,'
   ```

## Create Dynamic Distribution Build for MacOS

1. Go to the `src/` folder

   ```sh
   cd src/
   ```

2. Build the project with the strip flag

   ```sh
   make all CC=gcc-15 SYSTEM=macos EXTRA_FLAGS="-s"
   ```

3. Strip the executables

   ```sh
   strip abc abckeys
   ```

4. Make a new folder to put the necessary files in

   ```sh
   mkdir temp
   ```

5. Open Finder and copy `abc`, `abc.1`, `abc.hlp`, `abc.msg` and `abckeys` from
   `src/` into the newly created temp folder.

6. Open Disk Utility and go to  File > New Image > Image  from Folder.

7. Select the temp folder you created.

8. Save it under the name ABC-macos.dmg.

## Create Dynamic Distribution Build for Windows

1. Install Cygwin from https://www.cygwin.com/ and make sure the following
   packages are installed: gcc-core (at least 14.3), make, libncurses-devel

2. Open Cygwin, navigate to the location of the project

3. Go to source
  ```sh
  cd src/
  ```

4. Build the project
  ```sh
  make all
  ```

5. Make a new folder to place all the necessary files
  ```sh
  mkdir temp
  ```

6. Copy the following files into the new folder:
    `abc.exe`, `abc.hlp`, `abc.msg`, `abc.bat` and `abckeys.exe` from `src/`
  ```sh
  cp abc abc.hlp abc.msg abc.bat abckeys temp
  ```
7. Rename `abc` to  `abcexe` in your temp folder:
  ```sh
  cd ./temp
  mv abc.exe abcexe.exe
  ```

8. Find the location of `cygwin/` or `cygwin64/` on disk 

9. Locate the `cygwin1.dll` and `cygncurses-10.dll` in the `bin/`

10. Copy those files to the `temp/` folder

11. In the cygwin folder, find the folder '63' by searching usr/terminfo (it may be in a subfolder of terminfo)

12. Create a folder 'terminfo' inside your 'temp' folder, and copy the `63/` folder into that 'temp/terminfo' folder.

13. Add the desired Readme to the `temp/`

14. Zip the folder, rename it and ship!

## Set Appropriate GLIBC Version for Linux Builds

In order to make sure you link against the oldest GLIBC version possible, you
need to either

1. Build on a system with an old GLIBC version; or
2. Patch the ELF executable with a polyfill (not recommended).

> [!NOTE]
>
> - Ubuntu 22.04 LTS uses GLIBC 2.35
> - Debian 11 LTS uses GLIBC 2.31

### OPTION 1: Build on Ubuntu 22.04

To do this, set up an Ubuntu 22.04 container or virtual machine. Note that this
will still not work on Debian 11, but works on Debian 12+ and Ubuntu 22.04+.

1. Make sure you have `apt-add-repository` installed. If not, install it:

   ```sh
   sudo apt install software-properties-common
   ```

2. Add Ubuntu toolchain test repositories for a recent-enough GCC version, then
   make sure to update the local cache.

   ```sh
   sudo apt-add-repository ppa:ubuntu-toolchain-r/test
   sudo apt-get update
   ```

3. Install GCC 14 or later, GNU make and ncurses.

   ```sh
   sudo apt-get install gcc-14 make libncurses-dev
   ```

4. Follow the usual steps for building for distribution. You will need to pass
   `CC=gcc-14` to `make` invocations.

### OPTION 2: Use glibc-polyfill

This option is **NOT RECOMMENDED**. We have not conducted a security audit on
[`glibc-polyfill`][polyfill] nor can we guarantee that patched binaries exhibit
identical behaviour.

[polyfill]: https://github.com/corsix/polyfill-glibc

1. Build dynamic distribution binary according to the normal instructions.

2. Download [`glibc-polyfill`][polyfill] and build it from source.

3. Polyfill to GLIBC 2.31.

   ```sh
   /path/to/polyfill-glibc --target-glibc=2.31 abc
   ```
